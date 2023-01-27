/**
	* @file		SendThread.cpp
	* @brief	SignalStatusをプラグインへ通知スレッド 実装ファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Common.h"
#include "WriteDebugLog.h"
#include "SendThread.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief  CSendThread コンストラクション
**/
CSendThread::CSendThread(const char* inDeviceName):
	m_DeviceName(inDeviceName)
{
	m_hInitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);		// 初期化完了イベント
	m_hPipe = INVALID_HANDLE_VALUE;
}

/**
	* @brief  CSendThread デストラクション
**/
CSendThread::~CSendThread()
{
}

/**
	* @brief  スレッドを起動する
	* @param  InWriteDebugLog : [i]ログクラスのポインタ
	* @retval スレッドＩＤ
**/
UINT CSendThread::Start( CWriteDebugLog* InWriteDebugLog )
{
	// 情報を保存
	m_pWriteLog = InWriteDebugLog;
	m_pWriteLog->WriteDebuglog("Start CSendThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// スレッドの初期化待ち（スレッドにメッセージキューが出来るまで待つ）
	::WaitForSingleObject( m_hInitEvent, INFINITE );

	m_pWriteLog->WriteDebuglog("End.. CSendThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	return threadID;
}

/**
	* @brief  スレッドメイン処理
	* @retval 0(固定)
**/
UINT CSendThread::MainLoop()
{
	m_pWriteLog->WriteDebuglog("Start CSendThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	try
	{
		MSG	message;
		// メッセージキューの作成
		::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
		// スレッド初期化完了(メッセージキュー生成完了)
		::SetEvent( m_hInitEvent );

		bool loop = true;
		while ( loop )
		{
			// メッセージ取得
			BOOL bRet = ::GetMessage( &message, NULL, 0, 0 );
			if ( bRet == 0 || bRet == -1 )
			{
				break;
			}

			// メッセージ処理
			switch( message.message ) {
			case CCommon::WM_USER_SIGNAL_STATUS:	
				{
					CString* p = (CString*)message.wParam;
					CString s;
					s.Append( p->GetString() );
					delete p;

					// プラグインへSignalStatusを通知
					WriteSignalStatus( s );
				}
				break;
			}
		}
		m_hThread = NULL;
	}
	catch(...)
	{
		m_pWriteLog->WriteDebuglog("ERROR CSendThread::MainLoop Exception", CWriteDebugLog::E_LOG_LEVEL_1);
	}

	m_pWriteLog->WriteDebuglog("End.. CSendThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	return 0;
}

/**
	* @brief  受信スレッド
	* @param  pData : [i]CSendThreadのポインタ
	* @retval 0(固定)
**/
UINT __stdcall CSendThread::ThreadFunction( void* pData )
{
	return ((CSendThread*)pData)->MainLoop();
}

/**
	* @brief  SignalStatusをプラグインに通知する
**/
void CSendThread::WriteSignalStatus(const CString& InData)
{
	m_pWriteLog->WriteDebuglog("Start CSendThread::WriteSignalStatus", CWriteDebugLog::E_LOG_LEVEL_1);

	int nMax = 3;
	int i = 0;
	bool isUW = (strcmp(m_DeviceName, "UW") == 0);
	for( ; i<nMax; i++ )
	{
		if ( m_hPipe == INVALID_HANDLE_VALUE )
		{
			// Pipe作成
			m_hPipe = CreateFile(std::string("\\\\.\\pipe\\").append(isUW ? "Unwinder": "Rewinder").c_str(),
								  GENERIC_WRITE,
								  0,
								  NULL,
								  OPEN_EXISTING, 
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);
	
			if (m_hPipe == INVALID_HANDLE_VALUE)
			{
				std::stringstream ss;
				ss << "ERROR CSendThread::WriteSignalStatus CreateFile GetLastError=" << GetLastError();
				m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
				return;
			}
		}

		BOOL nRet = FALSE;

		// データ長書き込み
		CString SendLength;
		SendLength.Format("%08d",InData.GetLength());
		if ( WriteData( m_hPipe, (TCHAR*)SendLength.GetString(), SendLength.GetLength()) )
		{
			// データ書き込み
			if ( WriteData( m_hPipe, (TCHAR*)InData.GetString(), InData.GetLength()) )
			{
				nRet = TRUE;
			}
		}
		if( nRet )
		{
			break;
		}
		else
		{
			std::stringstream ss;
			ss << "ERROR CSendThread::WriteSignalStatus WriteFile count=" << i <<  " GetLastError=" << GetLastError();
			m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
			// Pipe終了
			CloseHandle(m_hPipe);
			m_hPipe = INVALID_HANDLE_VALUE;
		}
	}
	if ( i < nMax )
	{	// Pipe書き込み正常
		m_pWriteLog->WriteDebuglog("End.. CSendThread::WriteSignalStatus", CWriteDebugLog::E_LOG_LEVEL_1);
	}
	else
	{	// Pipe書き込み異常
		std::stringstream ss;
		ss << "ERROR CSendThread::WriteSignalStatus WriteFile GetLastError=" << GetLastError();
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
	}
}

/**
	* @brief  Pipe書き込み
	* @param  InPipe : [i]PIPEハンドル
	* @param  InData : [i]書き込みデータのポインタ
	* @param  InSize : [i]書き込みデータの長さ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CSendThread::WriteData( HANDLE InPipe, TCHAR* InData, DWORD InSize )
{
	TCHAR* p = InData;

	while( InSize )
	{
		DWORD WriteSize;
		BOOL nRet = WriteFile(InPipe, p, InSize, &WriteSize, NULL);
		if ( nRet )
		{
			InSize -= WriteSize;
			InData += WriteSize;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}