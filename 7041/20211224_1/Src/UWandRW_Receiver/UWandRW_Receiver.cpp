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
#include "Ini_UnwinderManager.h"
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
	: m_pSendThread(NULL)
	, m_pReceiveThread(NULL)
{
}

/**
	* @brief  CMainFunction デストラクタ
**/
CMainFunction::~CMainFunction()
{
	if ( m_pSendThread )
	{
		delete m_pSendThread;
	}
	if ( m_pReceiveThread )
	{
		delete m_pReceiveThread;
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
		m_pSendThread = new CSendThread();
		m_nSendThreadID = m_pSendThread->Start(&sWriteDebugLog);

		// UWからSignalStatusを受信するスレッドを起動
		m_pReceiveThread = new CReceiveThread();
		m_nReceiveThreadID = m_pReceiveThread->Start(m_nSendThreadID,&sWriteDebugLog);

		// 古いログを削除
		DeleteLogFolder(&sWriteDebugLog);

		// スレッドの終了を待つ(致命的なエラーが発生しない限り終了しない)
		HANDLE waitEvent[2] = {m_pSendThread->GetThreadHandle(), m_pReceiveThread->GetThreadHandle()};
		DWORD result = WaitForMultipleObjects(2, waitEvent, FALSE, INFINITE);

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
void CMainFunction::DeleteLogFolder(CWriteDebugLog* inWriteDebugLog)
{
	CString LogTopFoler = CCommon::GetModuleDirectory() + "\\Log\\UW_CONNECT\\";

	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	CDeleteLogFolder sDeleteLogFolder(inWriteDebugLog);
	sDeleteLogFolder.Doit( LogTopFoler, iniUnwinderManager.getPeriod_day() );
}
