/**
 * @file  ClientThread.cpp
 * @brief UWへの要求送信スレッド 実装ファイル
 * @author screen
 * @date  20211119 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "CallUnwinderRequest.h"
#include "ClientThread.h"
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
	* @retval スレッドＩＤ
**/
/* ==================================================================================================== */
UINT CClientThread::Start( CWnd* pParent )
{
	// 情報を保存
	m_pParent = pParent;

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// スレッドの初期化待ち（スレッドにメッセージキューが出来るまで待つ）
	::WaitForSingleObject( m_hInitEvent, INFINITE );

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
	MSG	message;
	// メッセージキューの作成
	::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
	// スレッド初期化完了(メッセージキュー生成完了)
	::SetEvent( m_hInitEvent );

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
		case WM_USER_JMF_QueryStatus:			// 状態監視
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallQueryStatus();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_QueryStatus_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_QueryResource:			// 用紙情報登録
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallQueryResource();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_QueryResource_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_StopPersChParams:		// チャネル削除
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallStopPersChParams();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_StopPersChParams_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_CommandResource:		// 印刷条件設定
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallCommandResource();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_CommandResource_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_CommandWakeup:			//  READY状態への遷移要請完了通知
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallCommandWakeup();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_CommandWakeup_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		}
	}
	m_hThread = NULL;

	return 0;
}
