/**
	* @file		ReceiveThread.cpp
	* @brief	UWからのSignalStatus受信スレッド 実装ファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include "WriteDebugLog.h"
#include "Server.h"
#include "ReceiveThread.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief  CReceiveThread コンストラクション
**/
CReceiveThread::CReceiveThread()
{
}

/**
	* @brief  CReceiveThread デストラクション
**/
CReceiveThread::~CReceiveThread()
{
}

/**
	* @brief  スレッドを起動する
	* @param  InSendThreadID : [i]送信スレッドＩＤ
	* @param  InWriteDebugLog : [i]ログクラスのポインタ
	* @retval スレッドＩＤ
**/
UINT CReceiveThread::Start( UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog )
{
	// 情報を保存
	m_nSendThreadID = InSendThreadID;
	m_pWriteLog = InWriteDebugLog;
	m_pWriteLog->WriteDebuglog("Start CReceiveThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	// スレッド起動
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	m_pWriteLog->WriteDebuglog("End.. CReceiveThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	return threadID;
}

/**
	* @brief  スレッドメイン処理
	* @retval 0(固定)
**/
UINT CReceiveThread::MainLoop()
{
	m_pWriteLog->WriteDebuglog("Start CReceiveThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	try
	{
		CServer sServer(m_nSendThreadID, m_pWriteLog);
		sServer.HttpServerMain();
	}
	catch(...)
	{
		m_pWriteLog->WriteDebuglog("ERROR CReceiveThread::MainLoop Exception", CWriteDebugLog::E_LOG_LEVEL_1);
	}

	m_pWriteLog->WriteDebuglog("End.. CReceiveThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	return 0;
}

/**
	* @brief  受信スレッド
	* @param  pData : [i]CServerThreadのポインタ
	* @retval 0(固定)
**/
UINT __stdcall CReceiveThread::ThreadFunction( void* pData )
{
	return ((CReceiveThread*)pData)->MainLoop();
}

