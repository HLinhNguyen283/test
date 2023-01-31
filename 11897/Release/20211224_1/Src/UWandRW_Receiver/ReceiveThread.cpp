/**
	* @file		ReceiveThread.cpp
	* @brief	UW�����SignalStatus��M�X���b�h �����t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
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
	* @brief  CReceiveThread �R���X�g���N�V����
**/
CReceiveThread::CReceiveThread()
{
}

/**
	* @brief  CReceiveThread �f�X�g���N�V����
**/
CReceiveThread::~CReceiveThread()
{
}

/**
	* @brief  �X���b�h���N������
	* @param  InSendThreadID : [i]���M�X���b�h�h�c
	* @param  InWriteDebugLog : [i]���O�N���X�̃|�C���^
	* @retval �X���b�h�h�c
**/
UINT CReceiveThread::Start( UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog )
{
	// ����ۑ�
	m_nSendThreadID = InSendThreadID;
	m_pWriteLog = InWriteDebugLog;
	m_pWriteLog->WriteDebuglog("Start CReceiveThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	m_pWriteLog->WriteDebuglog("End.. CReceiveThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	return threadID;
}

/**
	* @brief  �X���b�h���C������
	* @retval 0(�Œ�)
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
	* @brief  ��M�X���b�h
	* @param  pData : [i]CServerThread�̃|�C���^
	* @retval 0(�Œ�)
**/
UINT __stdcall CReceiveThread::ThreadFunction( void* pData )
{
	return ((CReceiveThread*)pData)->MainLoop();
}

