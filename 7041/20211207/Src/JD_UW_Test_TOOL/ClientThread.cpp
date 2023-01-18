/**
 * @file  ClientThread.cpp
 * @brief UW�ւ̗v�����M�X���b�h �����t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
	* @brief  CClientThread �R���X�g���N�V����
**/
/* ==================================================================================================== */
CClientThread::CClientThread()
{
	m_hInitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);		// �����������C�x���g
}

/* ==================================================================================================== */
/**
	* @brief  CClientThread �f�X�g���N�V����
**/
/* ==================================================================================================== */
CClientThread::~CClientThread()
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
UINT CClientThread::Start( CWnd* pParent, CWriteDebugLog* pWriteDebugLog )
{
	// ����ۑ�
	m_pParent = pParent;
	m_pWriteLog = pWriteDebugLog;
	m_pWriteLog->WriteDebuglog(_T("[CClientThread::Start()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// �X���b�h�̏������҂��i�X���b�h�Ƀ��b�Z�[�W�L���[���o����܂ő҂j
	::WaitForSingleObject( m_hInitEvent, INFINITE );

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::Start()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
	return threadID;
}

/* ==================================================================================================== */
/**
	* @brief  ��M�X���b�h
	* @param  pData : [i]CClientThread�̃|�C���^
	* @retval 0(�Œ�)
**/
/* ==================================================================================================== */
UINT __stdcall CClientThread::ThreadFunction( void* pData )
{
	return ((CClientThread*)pData)->MainLoop();
}

/* ==================================================================================================== */
/**
	* @brief  �X���b�h���C�����[�v
	* @retval 0(�Œ�)
**/
/* ==================================================================================================== */
UINT CClientThread::MainLoop()
{
	// COM���p�ɔ����A���������s��
	::CoInitialize( NULL );

	MSG	message;
	// ���b�Z�[�W�L���[�̍쐬
	::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
	// �X���b�h����������(���b�Z�[�W�L���[��������)
	::SetEvent( m_hInitEvent );

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::MainLoop()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	bool roop = true;
	while ( roop )
	{
		// ���b�Z�[�W�擾
		BOOL bRet = ::GetMessage( &message, NULL, 0, 0 );
		if ( bRet == 0 || bRet == -1 )
		{
			break;
		}

		// ���b�Z�[�W����
		switch( message.message ) {
		case CCommon::WM_USER_JMF_SEND:		// JMF���M����
			CClient sClient;
			sClient.HttpClientMain(m_pParent, (UINT)message.wParam, m_pWriteLog);
			PostMessage(m_pParent->m_hWnd, CCommon::WM_USER_JMF_SEND_END, (WPARAM)0, (LPARAM)0);
			break;
		}
	}

	// Com�̊J��
	::CoUninitialize();
	::CloseHandle( m_hThread );
	m_hThread = NULL;

	m_pWriteLog->WriteDebuglog(_T("[CClientThread::MainLoop()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return 0;
}
