/**
	* @file		SendThread.h
	* @brief	SignalStatus���v���O�C���֒ʒm�X���b�h �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

/**
* @brief JI JMF�ʐM JMF����M����X���b�h
*/
class CWriteDebugLog;
class CSendThread
{
public:
	/**
		* @brief  CSendThread �R���X�g���N�V����
	**/
	CSendThread();

	/**
		* @brief  CSendThread �f�X�g���N�V����
	**/
	virtual ~CSendThread();

	/**
		* @brief  �X���b�h���N������
		* @param  InWriteDebugLog : [i]���O�N���X�̃|�C���^
		* @retval �X���b�h�h�c
	**/
	UINT	Start( CWriteDebugLog* InWriteDebugLog );

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

	/**
		* @brief  SignalStatus���v���O�C���ɒʒm����
	**/
	void WriteSignalStatus(const CString& InData);

	/**
		* @brief  Pipe��������
		* @param  InPipe : [i]PIPE�n���h��
		* @param  InData : [i]�������݃f�[�^�̃|�C���^
		* @param  InSize : [i]�������݃f�[�^�̒���
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL WriteData( HANDLE InPipe, TCHAR* InData, DWORD InSize );

protected:
	HANDLE				m_hThread;				//!< �X���b�h�n���h��
	CWriteDebugLog*		m_pWriteLog;			//!< ���O�N���X�̃|�C���^
	HANDLE				m_hInitEvent;			//!< �����������C�x���g�n���h��
	HANDLE				m_hPipe;				//!< Pipe�n���h��
};