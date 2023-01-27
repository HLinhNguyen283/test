/*! @file DataIF.cpp
 *  @brief data interface class
 *  @author pt.ju-tanaka
 *  @date 2019/2/5 pt.ju-tanaka new
*/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataIF.h"
#include "CommonUiMsg.h"
#include "ReceiveExportPaperData.h"
#include "Stub_CenterTransIF.h"
#include "DebugLogUtil.h"
#include "Utility.h"
#include "DataSystemSetting.h"

#include <sstream>
// ==========================================================================
// Local Definition
#define FUNC_PIM_LASTWORK			"PIM_LastWork()"			//!< プラグインタイムアウトチェック用関数名(※管理モジュール内の定義と同じ必要あり)
static ST_THREAD_INFO UpdateThread;								//!< テキスト更新用のスレッド
static ST_THREAD_INFO UpdateXMLThread;							//!< XML更新用のスレッド
static HANDLE UpdateThreadOperation;							//!< テキスト更新用のスレッドの操作イベント
static HANDLE UpdateThreadFinish;								//!< テキスト更新用のスレッドの終了時用イベント
static HANDLE UpdateXMLThreadOperation;							//!< XML更新用のスレッドの操作イベント
static HANDLE UpdateXMLThreadFinish;							//!< XML更新用のスレッドの終了時用イベント

/**
 * @brief	CDataIFが使うAPI
**/
class CDataIF::API
{
public:
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

//コンストラクタ
CDataIF::CDataIF()
{
	m_RegisterReceiveCommandID = static_cast<size_t>(-1);
	m_ThreadFinish = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ExportThread.close_func = nullptr;
	m_ExportThread.thread_handle = nullptr;
	m_ExportThread.thread_id = 0;
	UpdateThreadOperation = CreateEvent(NULL, TRUE, FALSE, NULL);
	UpdateThreadFinish = CreateEvent(NULL, TRUE, FALSE, NULL);
	UpdateThread.close_func = nullptr;
	UpdateThread.thread_handle = nullptr;
	UpdateXMLThreadOperation = CreateEvent(NULL, TRUE, FALSE, NULL);
	UpdateXMLThreadFinish = CreateEvent(NULL, TRUE, FALSE, NULL);
	UpdateXMLThread.close_func = nullptr;
	UpdateXMLThread.thread_handle = nullptr;
}

//デストラクタ
CDataIF::~CDataIF()
{

}

//プラグイン初期化イベント(他プラグインとの接続不可)
BOOL CDataIF::Initialize()
{
	CDataSystemSetting::CreateInstance();

	return TRUE;
}

//プラグイン終了イベント(他プラグインからの接続切断)
void CDataIF::Finalize()
{
	CDataSystemSetting::DeleteInstance();
}

//プラグイン初期化イベント２(他プラグインとの接続可能)
BOOL CDataIF::PIM_InitSystem()
{
	//エクスポート処理実施用スレッド起動
	UINT thrd_id = 0;
	m_DataRequestExport = std::unique_ptr<CDataRequestExport>(new CDataRequestExport());
	m_ExportThread = TM_CreateThread(NULL, 0, ExportProc, (LPVOID)this, 0, &thrd_id, "PrintConditionExporter.dll", "ExportProc", NULL);

	m_UpdateRequestExport = std::unique_ptr<UpdateRequestExport>(new UpdateRequestExport());

	// 「EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能」の設定が有効の場合のみ、更新スレッドを起動する
	bool isEnableAutoUpdatePrintConditionText = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionText();
	if(isEnableAutoUpdatePrintConditionText)
	{
		UpdateThread = TM_CreateThread(NULL, 0, updateProc, (LPVOID)this, 0, &thrd_id, "PrintConditionExporter.dll", "updateProc", NULL);
	}

	m_DataRequestExportXML = std::unique_ptr<DataRequestExportXML>(new DataRequestExportXML());

	// 「EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能」の設定が有効の場合のみ、更新スレッドを起動する
	bool isEnableAutoUpdatePrintConditionXML = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML();
	if(isEnableAutoUpdatePrintConditionXML)
	{
		UpdateXMLThread = TM_CreateThread(NULL, 0, updateXMLProc, (LPVOID)this, 0, &thrd_id, "PrintConditionExporter.dll", "updateXMLProc", NULL);

		//	EQ制御サーバ上に書き出している、JI検査条件ファイルおよび一時マスク設定ファイルを起動時に更新する
		onExportInspectionData("Preferences\\InspectionParams");
		onExportInspectionData("Preferences\\TemporalMask");
	}

	//センター通信プラグにJMF受信イベント通知用の関数を登録する
	m_RegisterReceiveCommandID = API::Register_ReceiveProc(CDataIF::OnReceiveJMF, this);

	return TRUE;
}

//全プラグイン終了開始前イベント
BOOL CDataIF::PIM_ExitSystem()
{
	// 「EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能」の設定が有効の場合のみ、更新スレッドを終了する
	bool isEnableAutoUpdatePrintConditionText = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionText();
	if(isEnableAutoUpdatePrintConditionText)
	{
		// 更新用のスレッドを終了する
		SetEvent(UpdateThreadFinish);
		TM_DeleteThread(&UpdateThread);
	}

	// 「EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能」の設定が有効の場合のみ、更新スレッドを終了する
	bool isEnableAutoUpdatePrintConditionXML = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML();
	if(isEnableAutoUpdatePrintConditionXML)
	{
		// 更新用のスレッドを終了する
		SetEvent(UpdateXMLThreadFinish);
		TM_DeleteThread(&UpdateXMLThread);
	}

	return TRUE;
}

//最終処理実施イベント(他プラグインへの接続切断)
void CDataIF::PIM_LastWork()
{
	//PIM関数タイムアウトOFF
	SetPluginTimerPauseState(FUNC_PIM_LASTWORK, TRUE);

	//センター通信プラグからのJMF受信イベント通知用の関数を登録解除する
	API::Unregister_ReceiveProc(m_RegisterReceiveCommandID);
	
	//ドメイン部のオブジェクトにアプリケーション終了イベント通知
	m_DataRequestExport->OnExitSystem();
	
	//印刷条件エクスポート実施用スレッド終了
	SetEvent(m_ThreadFinish);
	TM_DeleteThread(&m_ExportThread);
	
	m_DataRequestExport.reset();

	//PIM関数タイムアウトON
	SetPluginTimerPauseState(FUNC_PIM_LASTWORK, FALSE);

}

//印刷条件エクスポート実施用スレッド
UINT WINAPI CDataIF::ExportProc(LPVOID pParam)
{
	auto dataObj = reinterpret_cast<CDataIF*>(pParam);
	while(WaitForSingleObject(dataObj->m_ThreadFinish, 1000) == WAIT_TIMEOUT){
		//ドメイン部のオブジェクトにエクスポート実施イベント通知
		dataObj->m_DataRequestExport->OnStartExport();
	}
	return 1;
}

//テキスト更新用のスレッド
UINT WINAPI CDataIF::updateProc(LPVOID pParam)
{
	auto dataObj = reinterpret_cast<CDataIF*>(pParam);
	//UpdateRequestExportクラスに初期化イベントを通知する
	if(!dataObj->m_UpdateRequestExport->onInitialize(&UpdateThread))
	{
		// 初期化に失敗した場合は更新用のスレッドを終了する
		TM_DeleteThreadForce(&UpdateThread);
		return 0;
	}

	SetEvent(UpdateThreadOperation);
	dataObj->onUpdateThred();

	return 0;
}

//XML更新用のスレッド
UINT WINAPI CDataIF::updateXMLProc(LPVOID pParam)
{
	auto dataObj = reinterpret_cast<CDataIF*>(pParam);
	//DataRequestExportXMLクラスに初期化イベントを通知する
	if(!dataObj->m_DataRequestExportXML->onInitialize())
	{
		// 初期化に失敗した場合は更新用のスレッドを終了する
		TM_DeleteThreadForce(&UpdateXMLThread);
		return 0;
	}

	SetEvent(UpdateXMLThreadOperation);
	dataObj->onUpdateXMLThred();

	return 0;
}

//JMF受信イベント
void CDataIF::OnReceiveJMF(ReceiveParamRef ioParam, void* ioUserData)
{
	try{
		CUtility::ThrowIfNull(ioParam, "Arg receiveparam is NULL.");
		CUtility::ThrowIfNull(ioUserData, "Arg userdata is NULL.");

		//ExportPaperDataを受信したときのみ、エクスポート依頼を受け付ける処理を行う.
		auto recvCommandParam = CUtility::ThrowIfNull(reinterpret_cast<IReceiveParam*>(ioParam), "Arg receiveparam can't convert to IReceiveParam*.");
		auto recvParamKind = recvCommandParam->getKind();
		if(IReceiveParam::kParamKind_ExportPaperData == recvParamKind)
		{//->ExportPaperData受信時
			//ドメイン部のオブジェクトにExportPrintCondition受信イベント通知
			auto param = CUtility::ThrowIfNull(dynamic_cast<ReceiveExportPaperData*>(recvCommandParam), "Arg receiveparam can't convert to ReceiveExportPaperData*.");
			auto dataObj = reinterpret_cast<CDataIF*>(ioUserData);
			auto ReturnCode = dataObj->m_DataRequestExport->OnReceiveExportPaperData(param->getPaperDataURL());

			if(CDataSystemSetting::GetInstance()->IsDebugLog())
			{
				std::stringstream log;
				log << "CDataRequestExport::OnReceiveExportPaperData ReturnCode = " << ReturnCode;
				WriteToLogBuf(LOG_DEBUG, log.str());
			}

			//処理した証として処理結果を設定する
			param->setReturnCode(ReturnCode);

			return;
		}
		else
		{//->それ以外
			//何もしない
			return;
		}
	}
	catch(std::exception& e)
	{
		WriteDebugLog(e.what());

		return;
	}
	
}

// システムの印刷条件更新を通知する
void CDataIF::onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	auto dataObj = reinterpret_cast<CDataIF*>(this);

	// 「EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能」の設定が有効かつ、更新スレッドが起動中の場合のみ、下記処理を実行する
	bool isEnableAutoUpdatePrintConditionText = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionText();
	if(isEnableAutoUpdatePrintConditionText && (nullptr != UpdateThread.thread_handle))
	{
		//UpdateRequestExportクラスにシステムの印刷条件更新イベント通知
		dataObj->m_UpdateRequestExport->onUpdatePrintCondition(inPrintConditionName, inMigrationData, &UpdateThread);
		SetEvent(UpdateThreadOperation);
	}

	// 「EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能」の設定が有効かつ、更新スレッドが起動中の場合のみ、下記処理を実行する
	bool isEnableAutoUpdatePrintConditionXML = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML();
	if(isEnableAutoUpdatePrintConditionXML && (nullptr != UpdateXMLThread.thread_handle))
	{
		//DataRequestExportXMLクラスにシステムの印刷条件更新イベント通知
		dataObj->m_DataRequestExportXML->onUpdatePrintCondition(inPrintConditionName, inMigrationData);
		SetEvent(UpdateXMLThreadOperation);
	}
}

// システムの印刷条件削除を通知する
void CDataIF::onDeletePrintCondition(const std::string& inPrintConditionName)
{
	auto dataObj = reinterpret_cast<CDataIF*>(this);

	// 「EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能」の設定が有効かつ、更新スレッドが起動中の場合のみ、下記処理を実行する
	bool isEnableAutoUpdatePrintConditionText = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionText();
	if(isEnableAutoUpdatePrintConditionText && (nullptr != UpdateThread.thread_handle))
	{
		//UpdateRequestExportクラスにシステムの印刷条件削除イベント通知
		dataObj->m_UpdateRequestExport->onDeletePrintCondition(inPrintConditionName, &UpdateThread);
		SetEvent(UpdateThreadOperation);
	}

	// 「EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能」の設定が有効かつ、更新スレッドが起動中の場合のみ、下記処理を実行する
	bool isEnableAutoUpdatePrintConditionXML = CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML();
	if(isEnableAutoUpdatePrintConditionXML && (nullptr != UpdateXMLThread.thread_handle))
	{
		//DataRequestExportXMLクラスにシステムの印刷条件削除イベント通知
		dataObj->m_DataRequestExportXML->onDeletePrintCondition(inPrintConditionName);
		SetEvent(UpdateXMLThreadOperation);
	}
}

// JI、検査関連フォルダのエクスポート（ミラーリングコピー）
bool CDataIF::onExportInspectionData(const std::string& inJIDataFolder)
{
	auto dataObj = reinterpret_cast<CDataIF*>(this);

	return	dataObj->m_DataRequestExportXML->MirroringInspectionData(inJIDataFolder);
}

// テキスト更新依頼の実施
void CDataIF::onUpdateThred()
{
	// 待機待ちのイベント.
	HANDLE WaitEvent[2] = {0};
	WaitEvent[0] = UpdateThreadFinish;
	WaitEvent[1] = UpdateThreadOperation;
	DWORD retWait = 0;
	DWORD interval = 0;
	long retryInterval = CDataSystemSetting::GetInstance()->GetAutoUpdatePrintConditionRetryInterval();

	if(retryInterval <= 0)
	{
		// EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔が0以下の場合、自動出力リトライを実施しない
		interval = INFINITE;
	}
	else
	{
		interval = static_cast<DWORD>(retryInterval * 1000);
	}

	bool isContinue = true;
	do {
		retWait = WaitForMultipleObjects( 2, WaitEvent, FALSE, interval );
		switch(retWait)
		{
			case WAIT_OBJECT_0:			// 終了イベント
			{
				isContinue = false;
				break;
			}
			case WAIT_OBJECT_0 + 1:		// 更新 or 削除通知による更新通知(直ちに処理開始する)
			{
				ResetEvent(UpdateThreadOperation);
				//UpdateRequestExportクラスにエクスポート更新イベント通知
				m_UpdateRequestExport->onUpdateExport(&UpdateThread);
				break;
			}
			case WAIT_TIMEOUT:		// リトライ処理による更新通知
			{
				//UpdateRequestExportクラスにエクスポート更新イベント通知
				m_UpdateRequestExport->onUpdateExport(&UpdateThread);
				break;
			}
			default: 
			{
				isContinue = false;
				break;
			}
		}
	} while(isContinue);
}

// XML更新依頼の実施
void CDataIF::onUpdateXMLThred()
{
	// 待機待ちのイベント.
	HANDLE WaitEvent[2] = {0};
	WaitEvent[0] = UpdateXMLThreadFinish;
	WaitEvent[1] = UpdateXMLThreadOperation;
	DWORD retWait = 0;
	DWORD interval = 0;
	long retryInterval = CDataSystemSetting::GetInstance()->GetAutoUpdatePrintConditionRetryIntervalXML();

	if(retryInterval <= 0)
	{
		// EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔が0以下の場合、自動出力リトライを実施しない
		interval = INFINITE;
	}
	else
	{
		interval = static_cast<DWORD>(retryInterval * 1000);
	}

	bool isContinue = true;
	do 
	{
		retWait = WaitForMultipleObjects( 2, WaitEvent, FALSE, interval );
		switch(retWait)
		{
			case WAIT_OBJECT_0:			// 終了イベント
			{
				isContinue = false;
				break;
			}
			case WAIT_OBJECT_0 + 1:		// 更新 or 削除通知による更新通知(直ちに処理開始する)
			{
				ResetEvent(UpdateXMLThreadOperation);
				//DataRequestExportXMLクラスにエクスポート更新イベント通知
				m_DataRequestExportXML->onUpdateXMLExport(&UpdateXMLThread);
				break;
			}
			case WAIT_TIMEOUT:		// リトライ処理による更新通知
			{
				//DataRequestExportXMLクラスにエクスポート更新イベント通知
				m_DataRequestExportXML->onUpdateXMLExport(&UpdateXMLThread);
				break;
			}
			default: 
			{
				isContinue = false;
				break;
			}
		}
	} while(isContinue);
}
