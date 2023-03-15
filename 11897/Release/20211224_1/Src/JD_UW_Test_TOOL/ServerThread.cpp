/**
 * @file  ServerThread.cpp
 * @brief UW����̗v����M�X���b�h �����t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
	* @brief  CServerThread �R���X�g���N�V����
**/
/* ==================================================================================================== */
CServerThread::CServerThread()
{
}

/* ==================================================================================================== */
/**
	* @brief  CServerThread �f�X�g���N�V����
**/
/* ==================================================================================================== */
CServerThread::~CServerThread()
{
}

/* ==================================================================================================== */
/**
	* @brief  �X���b�h���N������
	* @param  pParent       �@: [i]�e��ʂ̃E�B���h�E�n���h��
	* @param  pWriteDebugLog  : [i]���O�N���X�̃|�C���^
	* @retval �X���b�h�h�c
**/
/* ==================================================================================================== */
UINT CServerThread::Start( CWnd* pParent, CWriteDebugLog* pWriteDebugLog )
{
	// ����ۑ�
	m_pParent = pParent;
	m_pWriteLog = pWriteDebugLog;
	m_pWriteLog->WriteDebuglog(_T("[CServerThread::Start()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	m_pWriteLog->WriteDebuglog(_T("[CServerThread::Start()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
	return threadID;
}

/* ==================================================================================================== */
/**
	* @brief  ��M�X���b�h
	* @param  pData : [i]CServerThread�̃|�C���^
	* @retval 0(�Œ�)
**/
/* ==================================================================================================== */
UINT __stdcall CServerThread::ThreadFunction( void* pData )
{
	((CServerThread*)pData)->m_pWriteLog->WriteDebuglog(_T("Server�p�X���b�h�쐬�ɐ���"), CWriteDebugLog::E_LOG_LEVEL_1);

	// COM���p�ɔ����A���������s��
	::CoInitialize( NULL );

	CServer sServer(((CServerThread*)pData)->m_pParent,((CServerThread*)pData)->m_pWriteLog);
	sServer.HttpServerMain();

	// Com�̊J��
	::CoUninitialize();

	return 0;
}

