/**
 * @file  ServerThread.h
 * @brief UW����̗v����M�X���b�h �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/
#pragma once

/**
* @brief JI JMF�ʐM JMF����M����X���b�h
*/
class CWriteDebugLog;
class CServerThread
{
public:
	/* ==================================================================================================== */
	/**
		* @brief  CServerThread �R���X�g���N�V����
	**/
	/* ==================================================================================================== */
	CServerThread();

	/* ==================================================================================================== */
	/**
		* @brief  CServerThread �f�X�g���N�V����
	**/
	/* ==================================================================================================== */
	virtual ~CServerThread();

	/* ==================================================================================================== */
	/**
		* @brief  �X���b�h���N������
		* @param  pParent       �@: [i]�e��ʂ̃E�B���h�E�n���h��
		* @param  pWriteDebugLog  : [i]���O�N���X�̃|�C���^
		* @retval �X���b�h�h�c
	**/
	/* ==================================================================================================== */
	UINT	Start( CWnd* pParent, CWriteDebugLog* pWriteDebugLog );

	/* ==================================================================================================== */
	/**
		* @brief  ��M�X���b�h
		* @param  pData : [i]CServerThread�̃|�C���^
		* @retval 0(�Œ�)
	**/
	/* ==================================================================================================== */
	HANDLE	GetThreadHandle() { return m_hThread; }

private:
	static UINT __stdcall ThreadFunction( void* pData );

public:
	CWnd*				m_pParent;				//!< �X���b�h�Ăяo�����N���X
	HANDLE				m_hThread;				//!< �X���b�h�n���h��
	CWriteDebugLog*		m_pWriteLog;			//!< ���O�N���X�̃|�C���^
};