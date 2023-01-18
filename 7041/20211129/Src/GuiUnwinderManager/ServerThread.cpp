/**
 * @file  ServerThread.cpp
 * @brief UW����̗v����M�X���b�h �����t�@�C��
 * @author screen
 * @date  20211119 screen �V�K�쐬
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
	* @retval �X���b�h�h�c
**/
/* ==================================================================================================== */
UINT CServerThread::Start( CWnd* pParent )
{
	// ����ۑ�
	m_pParent = pParent;

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

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
	while(1)
	{
		std::string s = unwinder_manager::CCallUnwinderRequest::CallSignalStatus();
	}
	return 0;
}

