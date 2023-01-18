/**
 * @file  ServerThread.cpp
 * @brief UWからの要求受信スレッド 実装ファイル
 * @author screen
 * @date  20211119 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "CallUnwinderRequest.h"
#include "ServerThread.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief  CServerThread コンストラクション
**/
/* ==================================================================================================== */
CServerThread::CServerThread()
{
}

/* ==================================================================================================== */
/**
	* @brief  CServerThread デストラクション
**/
/* ==================================================================================================== */
CServerThread::~CServerThread()
{
}

/* ==================================================================================================== */
/**
	* @brief  スレッドを起動する
	* @param  pParent       　: [i]親画面のウィンドウハンドル
	* @retval スレッドＩＤ
**/
/* ==================================================================================================== */
UINT CServerThread::Start( CWnd* pParent )
{
	// 情報を保存
	m_pParent = pParent;

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	return threadID;
}

/* ==================================================================================================== */
/**
	* @brief  受信スレッド
	* @param  pData : [i]CServerThreadのポインタ
	* @retval 0(固定)
**/
/* ==================================================================================================== */
UINT __stdcall CServerThread::ThreadFunction( void* pData )
{
	while(1)
	{
		std::string s = unwinder_manager::CCallUnwinderRequest::CallSignalStatus();
	}
	return 0;
}

