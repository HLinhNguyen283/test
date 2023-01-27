/**
 * @file	DataIF.cpp
 * @brief	data interface class
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataIF.h"
#include "CommonUiMsg.h"
#include "ReceiveImportPaperData.h"
#include "Stub_CenterTransIF.h"
#include <sstream>
// ==========================================================================
#define FUNC_PIM_LASTWORK			"PIM_LastWork()"			//!< プラグインタイムアウトチェック用関数名
// ==========================================================================

/**
 * @brief	CDataIFが使うAPI
**/
class CDataIF::API
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
	 * @brief	コマンド受信通知 callback 登録定義
	 * @param[in]	inProc : 通知を受ける callback
	 * @param[in]	inUserData : ユーザー定義データ
	 * @retval	登録解除時に必要になる ID
	**/
	static size_t Register_ReceiveProc(receiveCallbackProcPtr inProc, void* inUserData)
	{
		return CStub_CenterTransIF::Register_ReceiveProc(inProc, inUserData);
	}

	/**
	 * @brief	コマンド受信通知 callback 登録解除定義
	 * @param[in]	inID : 登録時に返された ID
	**/
	static void Unregister_ReceiveProc(size_t inID)
	{
		CStub_CenterTransIF::Unregister_ReceiveProc(inID);
	}
};


CDataIF::CDataIF()
{
	m_RegisterReceiveCommandID = static_cast<size_t>(-1);
	m_ThreadFinish = CreateEvent(NULL, TRUE, FALSE, NULL);
	memset(&m_ImportThread, 0x00, sizeof(m_ImportThread));
	m_DataRequestImport.reset();
}

CDataIF::~CDataIF()
{

}

BOOL CDataIF::Initialize()
{
	m_DataSystemSetting.Initialize();
	return TRUE;
}

void CDataIF::Finalize()
{
}

BOOL CDataIF::PIM_InitSystem()
{
	m_DataRequestImport = std::unique_ptr<CDataRequestImport>(new CDataRequestImport(m_DataSystemSetting));
	UINT thrd_id = 0;
	m_ImportThread = TM_CreateThread(NULL, 0, ImportProc, (LPVOID)this, 0, &thrd_id, "PrintConditionImporter.dll", "ImportProc", NULL);
	m_RegisterReceiveCommandID = API::Register_ReceiveProc(CDataIF::OnReceiveJMF, this);
	return TRUE;
}

void CDataIF::PIM_LastWork()
{
	//インポート終了待ちに時間が掛るときがあるので、PIM関数タイムアウトOFF
	SetPluginTimerPauseState(FUNC_PIM_LASTWORK, TRUE);
	API::Unregister_ReceiveProc(m_RegisterReceiveCommandID);
	m_DataRequestImport->OnExitSystem();
	SetEvent(m_ThreadFinish);
	TM_DeleteThread(&m_ImportThread);
	m_DataRequestImport.reset();
	//PIM関数タイムアウトON
	SetPluginTimerPauseState(FUNC_PIM_LASTWORK, FALSE);
}

bool CDataIF::IsBasePrintCondition(const std::string& inPrintConditionName)
{
	//入力値とDataSystemSettingの持ってるベース名と比較
	if(inPrintConditionName == m_DataSystemSetting.GetBasePrintConditionName()){
		return true;
	}
	return false;
}

UINT WINAPI CDataIF::ImportProc(LPVOID pParam)
{
	CDataIF* cls = reinterpret_cast<CDataIF*>(pParam);
	if(cls == nullptr){
		return 1;
	}
	while(WaitForSingleObject(cls->m_ThreadFinish, 1000) == WAIT_TIMEOUT){
		//定期的にインポート依頼を行う
		cls->m_DataRequestImport->OnStartImport();
	}
	return 1;
}

void CDataIF::OnReceiveJMF(ReceiveParamRef ioParam, void* ioUserData)
{
	if(ioParam == nullptr){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::OnReceiveJMF arg receiveparam NULL.");
		return;
	}
	if(ioUserData == nullptr){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::OnReceiveJMF arg userdata NULL.");
		return;
	}
	//ImportPaperDataを受信したときのみ、インポート依頼を受け付ける処理を行う.
	CDataIF* dataObj = reinterpret_cast<CDataIF*>(ioUserData);
	IReceiveParam* recvCommandParam = reinterpret_cast<IReceiveParam*>(ioParam);
	IReceiveParam::EParamKind recvParamKind = recvCommandParam->getKind();
	if(IReceiveParam::kParamKind_ImportPaperData != recvParamKind){
		//処理しない
		return;
	}
	ReceiveImportPaperData* param = dynamic_cast<ReceiveImportPaperData*>(recvCommandParam);
	if(param == nullptr){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::OnReceiveJMF receive [IReceiveParam::kParamKind_ImportPaperData] ReceiveImportPaperData NULL.");
		return;
	}
	//インポート依頼の依頼を行って、結果をもらう
	CDataRequestImport::E_ADD_RESULT_TYPE result;
	{
		bool isOverwritePaper = param->getDifferenceMode() == ReceiveImportPaperData::kDifferenceMode_Overwrite ? true : false;
		bool isAppendPaper = param->getAppendMode() == ReceiveImportPaperData::kAppendMode_Append ? true : false;
		result = dataObj->m_DataRequestImport->OnReceiveImportPaperData(param->getCommandID(), param->getPaperDataURL(), isOverwritePaper, isAppendPaper);
	}

	//インポータープラグでの処理した結果を設定
	{
		ReceiveImportPaperData::EReturnCode ReturnCode = ReceiveImportPaperData::kReturnCode_Success;
		switch(result){
			case CDataRequestImport::ADD_SUCCESS:
				ReturnCode =  ReceiveImportPaperData::kReturnCode_Success;
				break;
			case CDataRequestImport::ADD_ERROR_FILE_COPY:
				ReturnCode =  ReceiveImportPaperData::kReturnCode_InvalidPaperDataURL;
				break;
			case CDataRequestImport::ADD_ERROR_INVALID_BASE_DATA:
				ReturnCode =  ReceiveImportPaperData::kReturnCode_InvalidParam;
				break;
			default:
				std::stringstream log;
				log << "CDataIF::OnReceiveJMF invalid result type. " << ReturnCode;
				WriteToLogBuf(LOG_DEBUG, log.str());
				break;
		}
		param->setReturnCode(ReturnCode);
	}
}
