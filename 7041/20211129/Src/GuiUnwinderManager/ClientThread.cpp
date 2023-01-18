/**
 * @file  ClientThread.cpp
 * @brief UW�ւ̗v�����M�X���b�h �����t�@�C��
 * @author screen
 * @date  20211119 screen �V�K�쐬
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
	* @retval �X���b�h�h�c
**/
/* ==================================================================================================== */
UINT CClientThread::Start( CWnd* pParent )
{
	// ����ۑ�
	m_pParent = pParent;

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// �X���b�h�̏������҂��i�X���b�h�Ƀ��b�Z�[�W�L���[���o����܂ő҂j
	::WaitForSingleObject( m_hInitEvent, INFINITE );

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
	MSG	message;
	// ���b�Z�[�W�L���[�̍쐬
	::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
	// �X���b�h����������(���b�Z�[�W�L���[��������)
	::SetEvent( m_hInitEvent );

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
		case WM_USER_JMF_QueryStatus:			// ��ԊĎ�
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallQueryStatus();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_QueryStatus_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_QueryResource:			// �p�����o�^
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallQueryResource();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_QueryResource_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_StopPersChParams:		// �`���l���폜
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallStopPersChParams();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_StopPersChParams_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_CommandResource:		// ��������ݒ�
			{
				std::string s = unwinder_manager::CCallUnwinderRequest::CallCommandResource();
				CString *p = new CString();
				*p = s.c_str();
				PostMessage(m_pParent->m_hWnd, WM_USER_DSP_SEND, (WPARAM)p, 0);
				PostMessage(m_pParent->m_hWnd, WM_USER_JMF_CommandResource_END, (WPARAM)0, (LPARAM)0);
			}
			break;
		case WM_USER_JMF_CommandWakeup:			//  READY��Ԃւ̑J�ڗv�������ʒm
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
