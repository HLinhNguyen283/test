/**
 * @file  ClientThread.cpp
 * @brief UWへの要求送信スレッド 実装ファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "WriteDebugLog.h"
#include "ClientThread.h"
#include "Client.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief  CClientThread コンストラクション
**/
/* ==================================================================================================== */
CClientThread::CClientThread()
{
	m_hInitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);		// 初期化完了イベント
}

/* ==================================================================================================== */
/**
	* @brief  CClientThread デストラクション
**/
/* ==================================================================================================== */
CClientThread::~CClientThread()
{
}

/* ==================================================================================================== */
/**
	* @brief  スレッドを起動する
	* @param  pParent       　: [i]親画面のウィンドウハンドル
	* @param  pWriteDebugLog  : [i]ログクラスのポインタ
	* @retval スレッドＩＤ
**/
/* ==================================================================================================== */
UINT CClientThread::Start( CWnd* pParent, CWriteDebugLog* pWriteDebugLog )
{
	// 情報を保存
	m_pParent = pParent;
	m_pWriteLog = pWriteDebugLog;
	m_pWriteLog->WriteDebuglog(_T("[CClientThread::Start()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// スレッドの初期化待ち（スレッドにメッセージキューが出来るまで待つ）
	::WaitForSingleObject( m_hInitEvent, INFINITE );

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::Start()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
	return threadID;
}

/* ==================================================================================================== */
/**
	* @brief  受信スレッド
	* @param  pData : [i]CClientThreadのポインタ
	* @retval 0(固定)
**/
/* ==================================================================================================== */
UINT __stdcall CClientThread::ThreadFunction( void* pData )
{
	return ((CClientThread*)pData)->MainLoop();
}

/* ==================================================================================================== */
/**
	* @brief  スレッドメインループ
	* @retval 0(固定)
**/
/* ==================================================================================================== */
UINT CClientThread::MainLoop()
{
	// COM利用に伴い、初期化を行う
	::CoInitialize( NULL );

	MSG	message;
	// メッセージキューの作成
	::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
	// スレッド初期化完了(メッセージキュー生成完了)
	::SetEvent( m_hInitEvent );

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::MainLoop()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	bool roop = true;
	while ( roop )
	{
		// メッセージ取得
		BOOL bRet = ::GetMessage( &message, NULL, 0, 0 );
		if ( bRet == 0 || bRet == -1 )
		{
			break;
		}

		// メッセージ処理
		switch( message.message ) {
		case CCommon::WM_USER_JMF_SEND:		// JMF送信処理
			CClient sClient;
			sClient.HttpClientMain(m_pParent, (UINT)message.wParam, m_pWriteLog);
			PostMessage(m_pParent->m_hWnd, CCommon::WM_USER_JMF_SEND_END, (WPARAM)0, (LPARAM)0);
			break;
		}
	}

	// Comの開放
	::CoUninitialize();
	::CloseHandle( m_hThread );
	m_hThread = NULL;

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::MainLoop()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return 0;
}
