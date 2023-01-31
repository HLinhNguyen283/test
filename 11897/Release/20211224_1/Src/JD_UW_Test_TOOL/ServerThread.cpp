/**
 * @file  ServerThread.cpp
 * @brief UWからの要求受信スレッド 実装ファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "WriteDebugLog.h"
#include "ServerThread.h"
#include "Server.h"
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
	* @param  pWriteDebugLog  : [i]ログクラスのポインタ
	* @retval スレッドＩＤ
**/
/* ==================================================================================================== */
UINT CServerThread::Start( CWnd* pParent, CWriteDebugLog* pWriteDebugLog )
{
	// 情報を保存
	m_pParent = pParent;
	m_pWriteLog = pWriteDebugLog;
	m_pWriteLog->WriteDebuglog(_T("[CServerThread::Start()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	m_pWriteLog->WriteDebuglog(_T("[CServerThread::Start()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
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
	((CServerThread*)pData)->m_pWriteLog->WriteDebuglog(_T("Server用スレッド作成に成功"), CWriteDebugLog::E_LOG_LEVEL_1);

	// COM利用に伴い、初期化を行う
	::CoInitialize( NULL );

	CServer sServer(((CServerThread*)pData)->m_pParent,((CServerThread*)pData)->m_pWriteLog);
	sServer.HttpServerMain();

	// Comの開放
	::CoUninitialize();

	return 0;
}

