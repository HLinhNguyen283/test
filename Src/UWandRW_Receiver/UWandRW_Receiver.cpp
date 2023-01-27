/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus受信処理メイン 実装ファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <vector>
#include "Common.h"
#include "GetTime.h"
#include "WriteDebugLog.h"
#include "Ini_UwRwManager.h"
#include "SendThread.h"
#include "ReceiveThread.h"
#include "DeleteLogFolder.h"
#include "UWandRW_Receiver.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
	* @brief  UWandRW_Receiver起動処理
	* @retval 0(固定)
**/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC を初期化して、エラーの場合は結果を印刷します。
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
			nRetCode = 1;
		}
		else
		{
			// 起動時処理
			HANDLE hMutex = ::CreateMutex( NULL, TRUE, "UWandRW_Receiver" );
			if( GetLastError() == ERROR_ALREADY_EXISTS)
			{
				// 同一PCからの二重起動（ツールはすでに起動されています。）
				// ::MessageBox( NULL, "UWandRW_Receiver is already running.", "UWandRW_Receiver", MB_OK | MB_ICONHAND );
				::ReleaseMutex( hMutex );
				CloseHandle( hMutex );
				return FALSE;
			}

			// 主処理
			CMainFunction sMainFunction;
			sMainFunction.Doit();

			// 終了時処理
			::ReleaseMutex( hMutex );
			CloseHandle( hMutex );
		}
	}
	else
	{
		_tprintf(_T("致命的なエラー: GetModuleHandle が失敗しました\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

/**
	* @brief  CMainFunction コンストラクタ
**/
CMainFunction::CMainFunction()
	: m_pSendThread()
	, m_pReceiveThread()
{
}

/**
	* @brief  CMainFunction デストラクタ
**/
CMainFunction::~CMainFunction()
{
	for (int index = 0; index < DEF_MAX_NUMBER_DEVICE; index++) {
		if (m_pSendThread[index])
		{
			delete m_pSendThread[index];
		}
		if (m_pReceiveThread[index])
		{
			delete m_pReceiveThread[index];
		}
	}
}

/**
	* @brief  UWandRW_Receiverの主処理
**/
void CMainFunction::Doit()
{
	// ログの初期化
	CString LogFileName = "UWandRW_Receiver_" + CGetTime::GetStrDate() + ".log";
	CWriteDebugLog sWriteDebugLog;
	sWriteDebugLog.Initialize(LogFileName);
	sWriteDebugLog.WriteDebuglog("Start UWandRW_Receiver.exe", CWriteDebugLog::E_LOG_LEVEL_1);
	sWriteDebugLog.WriteDebuglog("Start CMainFunction::Doit", CWriteDebugLog::E_LOG_LEVEL_1);

	try
	{
		// プラグインにSignalStatusを通知するスレッドを起動
		m_pSendThread[0] = new CSendThread("UW");
		m_nSendThreadID[0] = m_pSendThread[0]->Start(&sWriteDebugLog);

		// UWからSignalStatusを受信するスレッドを起動
		m_pReceiveThread[0] = new CReceiveThread("UW");
		m_nReceiveThreadID[0] = m_pReceiveThread[0]->Start(m_nSendThreadID[0],&sWriteDebugLog);

		// プラグインにSignalStatusを通知するスレッドを起動
		m_pSendThread[1] = new CSendThread("RW");
		m_nSendThreadID[1] = m_pSendThread[1]->Start(&sWriteDebugLog);

		// UWからSignalStatusを受信するスレッドを起動
		m_pReceiveThread[1] = new CReceiveThread("RW");
		m_nReceiveThreadID[1] = m_pReceiveThread[1]->Start(m_nSendThreadID[1], &sWriteDebugLog);

		// 古いログを削除
		DeleteLogFolder(&sWriteDebugLog, "UW");
		DeleteLogFolder(&sWriteDebugLog, "RW");

		// スレッドの終了を待つ(致命的なエラーが発生しない限り終了しない)
		HANDLE waitEvent[4] = {m_pSendThread[0]->GetThreadHandle(), m_pReceiveThread[0]->GetThreadHandle(), m_pSendThread[1]->GetThreadHandle(), m_pReceiveThread[1]->GetThreadHandle() };
		DWORD result = WaitForMultipleObjects(4, waitEvent, FALSE, INFINITE);

		sWriteDebugLog.WriteDebuglog("End.. CMainFunction::Doit **********\n", CWriteDebugLog::E_LOG_LEVEL_1);
	}
	catch(...)
	{
		sWriteDebugLog.WriteDebuglog("ERROR Exeption CMainFunction::Doit *********\n", CWriteDebugLog::E_LOG_LEVEL_1);
	}
}

/**
	* @brief  古いログを削除する
**/
void CMainFunction::DeleteLogFolder(CWriteDebugLog* inWriteDebugLog, const char* inDeviceName)
{
	CString LogTopFoler = CCommon::GetModuleDirectory() + "\\Log\\" + inDeviceName + "_CONNECT\\";

	CIni_UwRwManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	CDeleteLogFolder sDeleteLogFolder(inWriteDebugLog);
	sDeleteLogFolder.Doit( LogTopFoler, iniUnwinderManager.getPeriod_day() );
}
