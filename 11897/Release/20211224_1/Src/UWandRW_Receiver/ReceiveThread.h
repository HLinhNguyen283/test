/**
	* @file		ReceiveThread.h
	* @brief	UW�����SignalStatus��M�X���b�h �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/
#pragma once

/**
* @brief JI JMF�ʐM JMF����M����X���b�h
*/
class CWriteDebugLog;
class CReceiveThread
{
public:
	/**
		* @brief  CReceiveThread �R���X�g���N�V����
	**/
	CReceiveThread();

	/**
		* @brief  CReceiveThread �f�X�g���N�V����
	**/
	virtual ~CReceiveThread();

	/**
		* @brief  �X���b�h���N������
		* @param  InSendThreadID : [i]���M�X���b�h�h�c
		* @param  InWriteDebugLog : [i]���O�N���X�̃|�C���^
		* @retval �X���b�h�h�c
	**/
	UINT	Start( UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog );

	/**
		* @brief  �X���b�h���C������
		* @retval 0(�Œ�)
	**/
	UINT	MainLoop();

	HANDLE	GetThreadHandle() { return m_hThread; }

protected:
	/**
		* @brief  �X���b�h����
		* @param  pData : [i]CReceiveThread�̃|�C���^
		* @retval 0(�Œ�)
	**/
	static UINT __stdcall ThreadFunction( void* pData );

protected:
	UINT				m_nSendThreadID;		//!< ���M�X���b�h�h�c
	HANDLE				m_hThread;				//!< �X���b�h�n���h��
	CWriteDebugLog*		m_pWriteLog;			//!< ���O�N���X�̃|�C���^
};