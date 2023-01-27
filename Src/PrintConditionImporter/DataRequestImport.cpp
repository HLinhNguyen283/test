/**
 * @file	DataRequestImport.cpp
 * @brief	印刷条件のインポート要求に対する処理を行う
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestImport.h"
#include <list>
#include <sstream>
#include "ResDef.h"
#include "DataSystemSetting.h"
#include "ProcPluginCBRProc.h"
#include "PrintConditionsParser.h"
#include "PrintConditionForXML.h"
#include "PrintConditionRegister.h"
#include "EndTriggerCreator.h"
#include "CriticalSectionLocker.h"
#include <iomanip>
#include <functional>
#include "CommonCBRProc.h"
#include "WaitTrigger.h"
#include "OperationImportProgress.h"
#include "OperationAddProfileTypeList.h"

#define ThrowIfFalse(x, m) if ((x) == false) { std::runtime_error e(m); throw e; }
#define ThrowTimeoutIfFalse(x) if ((x) == false) { throw 1; }

// ==========================================================================


/**
 * @brief	XMLパーサー用のログ
**/
class Logger : public PrintConditionsParser::ILogger {
public:
	virtual void writeToDebugLog(
		const std::string&	inMessage)
	{
		WriteToLogBuf(LOG_DEBUG, inMessage);
	}
};

/**
 * @brief	CDataRequestImportが使うAPI
**/
class CDataRequestImport::API
{
private:
	/**
	 * @brief constructor
	**/
	API();
public:

	/**
	 * @brief destructor
	**/
	virtual ~API();

	/**
	 * @brief	インポート・エクスポート用の排他処理（読み書き禁止）を開始する
	 * @param[out]	outLockObj : 排他制御のオブジェクト
	 * @param[in]	inTimeOutFunc : タイムアウト判定関数
	 * @param[in]	inStartTime : 開始時刻(開始時にGetTickCount()で取得した値)
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	 * @note	タイムアウトあり。上位（取得先）でこの排他制御のオブジェクトが破棄されたら自動開放
	**/
	static bool GetUniqueLock(NS_SRWLock::CUniqueTransaction<bool>& outLockObj, std::function<bool(DWORD)> inTimeOutFunc, DWORD inStartTime)
	{
		bool isEnd = false;
		while(!isEnd){
			if(inTimeOutFunc(inStartTime)){
				//タイムアウト時間を経過したので、エラーを返す
				return false;
			}
			isEnd = CStub_PaperDBIF::PDB_TryUniqueLock_forImportExport(outLockObj);
		}
		return true;
	}

	/**
	 * @brief	指定の印刷条件の存在チェック. / Check the existence of the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition to be checked
	 * @retval	true : 存在する / Found
	 * @retval	false : 存在しない / Not found
	**/
	static bool IsPrintConditionName(const std::string& inPrintConditionName)
	{
		return CStub_PaperDBIF::PDB_IsPrintConditionName(inPrintConditionName.c_str());
	}
};


/**
 * @brief	依頼受付
**/
class CDataRequestImport::CRequestQueue
{
public:

	/**
	 * @brief	constructor
	**/
	CRequestQueue(CDataSystemSetting& inDataSystemSetting);

	/**
	 * @brief	destructor
	**/
	virtual ~CRequestQueue();

	/**
	 * @brief	依頼を追加する
	 * @param[in]	inJmfCommandId : JMFのコマンドID
	 * @param[in]	inPaperDataURL : 取り込み元のURL(UNC)
	 * @param[in]	inIsOverwritePaper : 同名異内容の印刷条件があった場合上書きするかどうか
	 * @param[in]	inIsAppendPaper : 同名の印刷条件がない場合に追加するかどうか
	 * @retval	ADD_SUCCESS : 追加成功
	 * @retval	ADD_ERROR_FILE_COPY : 追加失敗[元ファイルのコピー失敗(元ファイルのパスが無効でコピーできないも含む)]
	 * @retval	ADD_ERROR_INVALID_BASE_DATA : 追加失敗[ベースとなる印刷条件が無効]
	**/
	CDataRequestImport::E_ADD_RESULT_TYPE AddRequest(const std::string& inJmfCommandId, const std::string& inPaperDataURL, bool inIsOverwritePaper, bool inIsAppendPaper);

	/**
	 * @brief	依頼１件分の取り込み開始イベント (依頼があれば１件分の情報が取得できる)
	 * @param[out]	outJmfCommandId : JMFのコマンドID
	 * @param[out]	outPaperDataURL : 取り込み元のURL(UNC)
	 * @param[out]	outXMLFilePath : 取り込むXMLファイルのフルパス
	 * @param[out]	outIsOverwritePaper : 同名異内容の印刷条件があった場合上書きするかどうか
	 * @param[out]	outIsAppendPaper : 同名の印刷条件がない場合に追加するかどうか
	 * @param[out]	outAddTime : 受付時刻
	 * @retval	true : 成功 [依頼がある]
	 * @retval	false : 失敗 [依頼がない]
	 * @note	本関数をコールし成功したら、必ずEndImport関数をコールすること
	**/
	bool OnStartImport(std::string& outJmfCommandId, std::string& outPaperDataURL, std::string& outXMLFilePath, bool& outIsOverwritePaper, bool& outIsAppendPaper, DWORD& outAddTime);

	/**
	 * @brief	依頼１件分の取り込み終了イベント
	**/
	void OnEndImport();

	/**
	 * @brief	依頼状況を確認する
	 * @param[out]	outTotalCount : 未処理の依頼数合計
	 * @param[out]	outIsImporting : 処理中の依頼の有無(true:あり、false:なし)
	**/
	void GetRequestState(size_t& outTotalCount, bool& outIsImporting);

	/**
	 * @brief	依頼があるかどうかを確認する
	 * @retval	true : あり
	 * @retval	false : なし
	**/
	bool IsRequest();

protected:
	//! 1件分の依頼データ
	struct S_QUEUE
	{
		std::string PaperDataURL;	//!< 取り込み元のURL(UNC)
		std::string XMLFilePath;	//!< 印刷条件のXMLファイルのフルパス
		bool IsOverwritePaper;		//!< 同名異内容の印刷条件があった場合上書きするかどうか
		bool IsAppendPaper;			//!< 同名の印刷条件がない場合に追加するかどうか
		std::string JmfCommandId;	//!< JMFのコマンドID
		DWORD AddTime;				//!< 受付時刻.

		S_QUEUE(){
			IsOverwritePaper = false;
			IsAppendPaper = false;
			AddTime = 0;
		}
	};

	CDataSystemSetting& m_DataSystemSetting;		//!< システムのデータ

	bool m_IsImporting;					//!< インポート中かどうか(true:インポート中, false:待機中)[依頼一覧データが存在しなくても、本フラグがtrueならOnExitSystem()を終了してはいけない]
	std::list<S_QUEUE> m_List;			//!< 依頼一覧
	ST_CRITICAL_SECTION_INFO m_CS;		//!< 処理の排他制御用
};

CDataRequestImport::CRequestQueue::CRequestQueue(CDataSystemSetting& inDataSystemSetting)
	: m_DataSystemSetting(inDataSystemSetting)
{
	memset(&m_CS, 0x00, sizeof(m_CS));
	CSM_InitializeCriticalSection(&m_CS, MOD_PRINT_CONDITION_IMPORTER, "Import", 5000);
	m_IsImporting = false;
}

CDataRequestImport::CRequestQueue::~CRequestQueue()
{
	if(m_List.empty() == false) {
		WriteToLogBuf(LOG_DEBUG, "CRequestQueue::~CRequestQueue() Error. Data is left.");
	}
	CSM_DeleteCriticalSection(&m_CS);
}

CDataRequestImport::E_ADD_RESULT_TYPE CDataRequestImport::CRequestQueue::AddRequest(const std::string& inJmfCommandId, const std::string& inPaperDataURL, bool inIsOverwritePaper, bool inIsAppendPaper)
{
	auto getSrcFullPath = [&](const std::string& inPaperDataURL)->std::string
	{
		std::stringstream path;
		path << inPaperDataURL;

		if(m_DataSystemSetting.IsDED() == true){
			path << "\\DED\\PrintConditions.xml";
		}
		else{
			path << "\\SES\\PrintConditions.xml";
		}
		return path.str();
	};

	auto getDstFullPath = [&]()->std::string
	{
		SYSTEMTIME systemTime = {0};
		GetLocalTime(&systemTime);
		std::stringstream path;
		path << GetModuleDirectory() << "TEMP\\ImportPrintCondition_";
		path << std::setfill('0') << std::setw(4) << systemTime.wYear;
		path << std::setfill('0') << std::setw(2) << systemTime.wMonth;
		path << std::setfill('0') << std::setw(2) << systemTime.wDay;
		path << std::setfill('0') << std::setw(2) << systemTime.wHour;
		path << std::setfill('0') << std::setw(2) << systemTime.wMinute;
		path << std::setfill('0') << std::setw(2) << systemTime.wSecond;
		path << m_DataSystemSetting.GetFileCounter();
		path << ".xml";
		return path.str();
	};

	auto getLastErrorString = [](DWORD inErrID)->const std::string
	{
		LPVOID errBuf = GetLastErrorString(inErrID);
		if(errBuf == nullptr){
			return "";
		}

		const std::string errStr = static_cast<LPTSTR>(errBuf);
		FreeLastErrorString(errBuf);
		return errStr;
	};


	StCriticalSectionLocker cs(&m_CS);

	std::string dstFullPath = "";
	try {
		//取り込み元パスが有効かチェックする
		if(inPaperDataURL.empty() == true){
			WriteToLogBuf(LOG_DEBUG, "CRequestQueue::AddRequest() Error. PaperDataURL is empty.");
			//無効時は失敗(リターンコード1023)を返す
			throw ADD_ERROR_FILE_COPY;
		}

		//PrintConditions.xmlをローカル環境にコピーする
		std::string srcFullPath = getSrcFullPath(inPaperDataURL);
		dstFullPath = getDstFullPath();
		if(CopyFile(srcFullPath.c_str(), dstFullPath.c_str(), TRUE) == 0){
			const std::string lastErr = getLastErrorString(GetLastError());
			std::stringstream log;
			log << "CRequestQueue::AddRequest() Error. Cannot copy file.\nsrc:" << srcFullPath << "\ndst:" << dstFullPath << "\n" << lastErr;
			WriteToLogBuf(LOG_DEBUG, log.str());

			//コピーできていないので削除する必要がない。同時間にできた別ファイルを消してしまう可能性があるためここでクリアする
			dstFullPath = "";

			//当該ファイルのコピー失敗時は失敗(リターンコード1023)を返す
			throw ADD_ERROR_FILE_COPY;
		}

		//ベースとなる印刷条件名が有効かチェックする
		const std::string basePrintConditionName = m_DataSystemSetting.GetBasePrintConditionName();
		if(basePrintConditionName.empty() == true){
			WriteToLogBuf(LOG_DEBUG, "CRequestQueue::AddRequest() Error. BASE_NAME is empty.");

			//INIファイルにベースとなる印刷条件名が記載されていない場合は失敗(リターンコード1002)を返す
			throw ADD_ERROR_INVALID_BASE_DATA;
		}

		if(API::IsPrintConditionName(basePrintConditionName) == false){
			std::stringstream log;
			log << "CRequestQueue::AddRequest() Error. BASE_NAME[ " << basePrintConditionName << " ] is not exist.";
			WriteToLogBuf(LOG_DEBUG, log.str());

			//ベースとなる印刷条件が紙DBに存在しない場合は失敗(リターンコード1002)を返す
			throw ADD_ERROR_INVALID_BASE_DATA;
		}

		if(CWaitTrigger::OnAddRequest(inPaperDataURL) == false){
			//待機中トリガーファイルの作成に失敗した場合
			std::stringstream log;
			log << "CRequestQueue::AddRequest() Error. createWaitTriger.";
			WriteToLogBuf(LOG_DEBUG, log.str());
			throw ADD_ERROR_FILE_COPY;
		}
		//インポート依頼をリストに登録する
		S_QUEUE data;
		data.PaperDataURL = inPaperDataURL;
		data.XMLFilePath = dstFullPath;
		data.IsOverwritePaper = inIsOverwritePaper;
		data.IsAppendPaper = inIsAppendPaper;
		data.JmfCommandId = inJmfCommandId;
		data.AddTime = GetTickCount();
		m_List.push_back(data);

		return ADD_SUCCESS;
	}
	catch (CDataRequestImport::E_ADD_RESULT_TYPE& error){
		if(dstFullPath.empty() == false){
			//PrintConditions.xmlをローカル環境にコピー済みであれば削除する
			if(DeleteFile(dstFullPath.c_str()) == FALSE){
				std::stringstream log;
				log << "CRequestQueue::AddRequest() Error. Cannot delete file. " << dstFullPath;
				WriteToLogBuf(LOG_DEBUG, log.str());
			}
		}
		return error;
	}
}

bool CDataRequestImport::CRequestQueue::OnStartImport(std::string& outJmfCommandId, std::string& outPaperDataURL, std::string& outXMLFilePath, 
	bool& outIsOverwritePaper, bool& outIsAppendPaper, DWORD& outAddTime)
{
	StCriticalSectionLocker cs(&m_CS);
	if(m_List.empty() == true) {
		//依頼がない
		return false;
	}

	//依頼一覧の先頭データを取得し、引数に設定する
	auto data = m_List.begin();

	outJmfCommandId = data->JmfCommandId;
	outPaperDataURL = data->PaperDataURL;
	outXMLFilePath = data->XMLFilePath;
	outIsOverwritePaper = data->IsOverwritePaper;
	outIsAppendPaper = data->IsAppendPaper;
	outAddTime = data->AddTime;

	m_List.erase(data);

	//インポート中にする
	m_IsImporting = true;

	std::stringstream log;
	log << "StartImport [" << outJmfCommandId << "]";
	log << std::boolalpha << "(IsOverWrite:" << outIsOverwritePaper << ", IsAppend:" << outIsAppendPaper;
	log << ", URL:" << outPaperDataURL << ")";
	WriteToLogBuf(LOG_DEBUG, log.str());
	//依頼がある
	return true;
}

void CDataRequestImport::CRequestQueue::OnEndImport()
{
	StCriticalSectionLocker cs(&m_CS);
	//待機中にする
	m_IsImporting = false;

	WriteToLogBuf(LOG_DEBUG, "EndImport");
}

void CDataRequestImport::CRequestQueue::GetRequestState(size_t& outTotalCount, bool& outIsImporting)
{
	StCriticalSectionLocker cs(&m_CS);
	outTotalCount = m_List.size();
	outIsImporting = m_IsImporting;
}

bool CDataRequestImport::CRequestQueue::IsRequest()
{
	StCriticalSectionLocker cs(&m_CS);
	bool ret = m_List.empty();
	return !ret;
}

//-----CDataRequestImport-------//
CDataRequestImport::CDataRequestImport(CDataSystemSetting& inDataSystemSetting)
	: m_DataSystemSetting(inDataSystemSetting)
{
	m_RequestQueue = std::unique_ptr<CRequestQueue>(new CRequestQueue(inDataSystemSetting));
}

CDataRequestImport::~CDataRequestImport()
{
}

CDataRequestImport::E_ADD_RESULT_TYPE CDataRequestImport::OnReceiveImportPaperData(const std::string& inJmfCommandId, const std::string& inPaperDataURL, bool inIsOverwritePaper, bool inIsAppendPaper)
{
	//依頼を追加する
	E_ADD_RESULT_TYPE result = m_RequestQueue->AddRequest(inJmfCommandId, inPaperDataURL, inIsOverwritePaper, inIsAppendPaper);
	return result;
}

using namespace printconditionsparser;
void CDataRequestImport::OnStartImport()
{
	//依頼がたまっている時は時間がたたなくても処理を行う
	while(m_RequestQueue->IsRequest()){
		OneImport();
	}
}

void CDataRequestImport::OneImport()
{
	auto getPrintConditionName = [&](CPrintConditionForXML& inXML, std::string& outName)-> bool{
		const CValue<std::string>* printConditionNameValue = inXML.GetString(CPrintConditionForXML::kPrintConditionName);
		if(printConditionNameValue == nullptr){
			return false;
		}
		std::string name;
		if(printConditionNameValue->Get(name) == false){
			return false;
		}
		outName = name;
		return true;
	};

	auto checkImportStart = [&](CPrintConditionRegister& inRegister, bool inIsAppendPaper, bool inIsOverwritePaper, bool& outIsAppend) -> bool
	{
		//指定の印刷条件が紙DBに登録済みかチェック
		if(inRegister.IsRegisteredPrintConditionName() == false){
			//登録されていない場合
			if(inIsAppendPaper == true){
				//JMFで追加指示されている場合、新規追加のインポートする
				outIsAppend = true;
				return true;
			}else{
				//JMFでスキップor確認ダイアログ表示が指示されている場合、インポートしない
				return false;
			}
		}else{
			//登録されている場合
			if(inIsOverwritePaper == true){
				//JMFで上書き指示されている場合、上書きのインポート
				outIsAppend = false;
				return true;
			}else{
				//JMFでスキップor確認ダイアログ表示が指示されている場合、インポートしない
				return false;
			}
		}
	};

	auto isTimeOut = [&](DWORD inAddTime) -> bool
	{
		DWORD diffTime = GetTickCount() - inAddTime;
		if(diffTime >= m_DataSystemSetting.GetTimeoutImport()){
			//タイムアウト時間を経過したので、エラーを返す
			std::stringstream log;
			log << "Import Timeout Error.[" << diffTime << " >= "<< m_DataSystemSetting.GetTimeoutImport() << "]";
			WriteToLogBuf(LOG_DEBUG, log.str());
			return true;
		}
		return false;
	};

	auto writeTimeoutErrLog = [&](const std::string& jmfCommandId, const std::string& inPrintConditionName)
	{
		std::string msg = (char*)LoadResourceString(IDM_ERR_IMPORT_TIMEOUT, RESOURCE_MSG_STR);
		size_t posId = msg.find("%s");
		if(posId != std::string::npos) {
			msg.replace(posId, 2, jmfCommandId);
		}
		size_t posName = msg.find("%s");
		if(posName != std::string::npos) {
			msg.replace(posName, 2, inPrintConditionName);
		}
		std::stringstream log;
		log << (ERR_PRINT_CONDITION_IMPORTER + IDM_ERR_IMPORT_TIMEOUT) << std::endl << msg;
		WriteToLogBuf(LOG_ERROR, log.str());
	};

	//インポート開始通知
	std::string jmfCommandId;
	std::string paperDataURL;
	std::string XMLFilePath;
	bool isOverwritePaper = false;
	bool isAppendPaper = false;
	DWORD addTime = 0;
	bool retStart = m_RequestQueue->OnStartImport(jmfCommandId, paperDataURL, XMLFilePath, isOverwritePaper, isAppendPaper, addTime);
	if(!retStart){
		//開始する依頼がない
		return;
	}

	// 印刷条件取り込みの(開始)の通知を呼び出す
	CStub_PaperDBIF::PDB_OnPrintConditionImport(false);
	COperationImportProgress importProgress(paperDataURL);

	importProgress.createFile();

	if(isTimeOut(addTime)){
		//xmlを解析する前にタイムアウトした場合は、
		//インポート待機中トリガーファイルを削除し、インポートタイムアウト発生トリガーファイルを作成する。
		CWaitTrigger::OnTimeoutFinsh(paperDataURL);
		
		importProgress.deleteImportProggressFile();

		// 印刷条件取り込みの(終了)の通知
		CStub_PaperDBIF::PDB_OnPrintConditionImport(true);

		//インポート終了通知
		m_RequestQueue->OnEndImport();
		return;
	}

	CEndTriggerCreator endTriggerCreator(paperDataURL);

	//PrintConditions.xmlの解析してインポート用データリストに追加
	//XMLを読み込む
	Logger logger;
	PrintConditionsParser parser(logger);
	bool res = parser.read(XMLFilePath);

	ForceToDeleteFile(XMLFilePath.c_str());//不要になったXMLファイルは削除する
	if(res == false){
		//インポート待機中トリガーファイルを削除する
		CWaitTrigger::OnFinsh(paperDataURL);

		importProgress.deleteImportProggressFile();

		//インポート失敗のトリガーファイルを作成する
		endTriggerCreator.WriteFailed();

		// 印刷条件取り込みの(終了)の通知
		CStub_PaperDBIF::PDB_OnPrintConditionImport(true);

		//インポート終了通知
		m_RequestQueue->OnEndImport();
		return;
	}

	//XMLパーサーから全値を取得する
	size_t numXML = parser.getNum();
	std::list<CPrintConditionForXML> printConditionsForXML;
	for(size_t index = 0; index < numXML; ++index){
		try{
			const IPrintCondition& condition = parser.getCondition(index);
			CPrintConditionForXML oneData(m_DataSystemSetting);
			oneData.ReadFromXML(condition);
			printConditionsForXML.push_back(oneData);
		}catch(...){
			//値取得に失敗したときは失敗リストに追加する
			const IPrintCondition& condition = parser.getCondition(index);
			auto name = condition.getDescriptiveName();
			if(name.first == true){
				endTriggerCreator.AddResult(name.second, CEndTriggerCreator::END_RESULT_ERROR);
				importProgress.WriteSuccess(name.second, COperationImportProgress::END_RESULT_ERROR);
			}
		}
	}

	COperationAddPaperTypeList	addPaperTypeProgress(paperDataURL);
	COperationAddAllignmentList	addAllignmentProgress(paperDataURL);

	//インポート用データ(CPrintConditionForXML)がある間ループ
	for(auto xml = printConditionsForXML.begin(); xml != printConditionsForXML.end(); ++xml){
		std::string printConditionName = "";	//エラー処理で使用するため、外に宣言している
		try{
			//印刷条件名を取得する(エラー処理で必要なので先に取っておく)
			getPrintConditionName(*xml, printConditionName);

			if( xml->IsImportSkipped() )
			{
				// XML解析時にスキップ可能と判断済のため、インポート結果をスキップで登録
				endTriggerCreator.AddResult(printConditionName, CEndTriggerCreator::END_RESULT_SKIP);
				importProgress.WriteSuccess(printConditionName, COperationImportProgress::END_RESULT_SKIP);
			}
			else
			{
				//印刷条件１件毎に排他制御開始
				NS_SRWLock::CUniqueTransaction<bool> lockObj;
				ThrowTimeoutIfFalse(API::GetUniqueLock(lockObj, isTimeOut, addTime));

				//XMLの必須項目有無チェック
				xml->JudgeRequiredItem();

				CPrintConditionRegister printConditionRegister(printConditionName, m_DataSystemSetting);

				//指定の印刷条件をインポートするかどうかを判断する.
				bool isAppend = false;
				bool isStart = checkImportStart(printConditionRegister, isAppendPaper, isOverwritePaper, isAppend);

				//ベースとなる印刷条件からインポート対象の全レコードを取得
				printConditionRegister.LoadInitValue(isAppend);

				if(isStart){
					//インポートする場合、
					printConditionRegister.Import(isAppend, *xml);

					// １件でもインポート完了したら、インポート待機中トリガーファイルを削除する。（DZ-20-0041対策）
					CWaitTrigger::OnFinsh(paperDataURL);

					//当該印刷条件名のインポート結果を成功で登録
					endTriggerCreator.AddResult(printConditionName, CEndTriggerCreator::END_RESULT_SUCCESS);
					importProgress.WriteSuccess(printConditionName, COperationImportProgress::END_RESULT_SUCCESS);

					std::string addPaperType;
					std::string addAlignment;
					if( printConditionRegister.IsAppendPaperType(addPaperType) )
					{
						addPaperTypeProgress.WriteSuccess(addPaperType);
					}
					if( printConditionRegister.IsAppendAllignment(addAlignment) )
					{
						addAllignmentProgress.WriteSuccess(addAlignment);
					}
				}else{
					//インポートしない場合、当該印刷条件名のインポート結果をスキップで登録
					endTriggerCreator.AddResult(printConditionName, CEndTriggerCreator::END_RESULT_SKIP);
					importProgress.WriteSuccess(printConditionName, COperationImportProgress::END_RESULT_SKIP);
				}
			}
		}
		catch(LogSupressException& error)
		{	//1件分のインポート失敗を受け取る(個別メッセージ:throw時に指定)
			std::stringstream info;
			info << error.what();
			WriteToLogBuf(LOG_STATUS, info.str());

			//当該印刷条件名のインポート結果を失敗で登録
			endTriggerCreator.AddResult(printConditionName, CEndTriggerCreator::END_RESULT_ERROR);
			importProgress.WriteSuccess(printConditionName, COperationImportProgress::END_RESULT_ERROR);
		}
		catch(std::runtime_error& error)
		{	//1件分のインポート失敗を受け取る
			std::stringstream log;
			log << "CDataRequestImport::OnStartImport() [" << printConditionName << "]" << error.what();
			WriteToLogBuf(LOG_DEBUG, log.str());

			//　外部からの指示情報に問題があり、印刷条件[%s]を取り込むことが出来ません。
			std::string msg = (char*)LoadResourceString(IDS_STAT_IMPORT_NOT_EXECUTE, RESOURCE_STR);
			size_t posIdx = msg.find("%s");
			if(posIdx != std::string::npos)
			{
				msg.replace(posIdx, 2, printConditionName);
			}
			WriteToLogBuf(LOG_STATUS, msg);

			//当該印刷条件名のインポート結果を失敗で登録
			endTriggerCreator.AddResult(printConditionName, CEndTriggerCreator::END_RESULT_ERROR);
			importProgress.WriteSuccess(printConditionName, COperationImportProgress::END_RESULT_ERROR);
		}
		catch(int){
			//タイムアウトエラー発生を受け取って、インポートを終了する

			//タイムアウトのログ出力
			writeTimeoutErrLog(jmfCommandId, printConditionName);
			//処理しなかった印刷条件はSkipにする
			for(auto it = xml; it != printConditionsForXML.end(); ++it){
				std::string name;
				getPrintConditionName(*it, name);
				endTriggerCreator.AddResult(name, CEndTriggerCreator::END_RESULT_SKIP);
			}
			//インポート待機中トリガーファイルを削除し、インポートタイムアウト発生トリガーファイルを作成する。
			CWaitTrigger::OnTimeoutFinsh(paperDataURL);

			importProgress.deleteImportProggressFile();

			//インポート完了のトリガーファイルを作成する
			endTriggerCreator.WriteSuccess();

			// 印刷条件取り込みの(終了)の通知
			CStub_PaperDBIF::PDB_OnPrintConditionImport(true);

			//インポート終了通知
			m_RequestQueue->OnEndImport();
			return;
		}
	}

	//インポート待機中トリガーファイルを削除する
	CWaitTrigger::OnFinsh(paperDataURL);

	importProgress.deleteImportProggressFile();

	//インポート完了のトリガーファイルを作成する
	endTriggerCreator.WriteSuccess();

	// 印刷条件取り込みの(終了)の通知
	CStub_PaperDBIF::PDB_OnPrintConditionImport(true);

	//インポート終了通知
	m_RequestQueue->OnEndImport();
}

void CDataRequestImport::OnExitSystem()
{
	auto getCount = [&]() -> long
	{
		size_t totalCount = 0;
		bool isImporting = false;
		m_RequestQueue->GetRequestState(totalCount, isImporting);
		if(isImporting){
			++totalCount;
		}
		return static_cast<long>(totalCount);
	};

	//インポート依頼が残っている場合は、なくなるまで進捗ダイアログを表示する
	long totalCount = getCount();
	if(totalCount == 0){
		return;
	}
	std::stringstream log;
	log << "Remaining request : " << totalCount;
	WriteToLogBuf(LOG_DEBUG, log.str());

	// 進捗ダイアログ生成
	SIZE sizeProgDlg = {750, 0};
	long progressID = ShowProgressDialog(sizeProgDlg, PROGDLG_MSG_CENTER | PROGDLG_PROGRESS_BAR, 0, totalCount, 0, nullptr, (char*)LoadResourceString(IDM_EXIT_PROGRESS, RESOURCE_MSG_STR));
	if(progressID == 0){
		WriteToLogBuf(LOG_DEBUG, "CDataRequestImport::OnExitSystem() Error. Cannnot open progress dialog.");
		//処理を継続しないと、インポート処理が終わってないのに終了しようとする
	}

	long nowCount = totalCount;
	while(nowCount > 0){
		nowCount = getCount();
		long progressVal = totalCount - nowCount;
		SetProgressMessage(progressID, PROGDLG_SET_VALUE, progressVal, nullptr);
		::Sleep(1000);
	}
	CloseProgressDialog(progressID);
}