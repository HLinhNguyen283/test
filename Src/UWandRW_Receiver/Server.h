/**
	* @file		Server.h
	* @brief	UW����̗v����M �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/
#pragma once

#include "CommonNet.h"

/**
* JI JMF�ʐM JMF����M����N���X
* ���ʕ����̃��\�b�h
* @brief JI JMF�ʐM JMF����M����N���X
*/
class CWriteDebugLog;
class CServer
{
public:
	/**
		* @brief  CServer �R���g���N�V����
		* @param  InSendThreadID   : [i]���M�X���b�h�̂h�c
		* @param  InWriteDebugLog  : [i]���O�N���X�̃|�C���^
	**/
	CServer(UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog, const char* inDeviceName);

	/**
		* @brief  CServer �f�X�g���N�V����
	**/
	virtual ~CServer();

	/**
		* @brief  �T�[�o�[(UW�����JMF����M���鏈��)
	**/
	void HttpServerMain();

protected:
	/**
		* @brief  POST��M����
		* @param  hReqQueue		: [i]HttpCreateRequestQueue�̃|�C���^
		* @param  pHttpRequest	: [i]PHTTP_REQUEST�̃|�C���^
		* @retval 0(�Œ�)
	**/
	DWORD HttpPostReceive(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest);

	/**
		* @brief  �R�[�h�ϊ�(UTF8 �� Shift-JIS)
		* @param  InUTF8 : [i]UTF8�̕�����
		* @retval Shift-JIS�̕�����
	**/
	std::string ConvUTF8toShiftJIS(const std::string& InUTF8);

	/**
		* @brief  SignalStatus(��ԊĎ�)��URL�擾
		* @retval SignalStatus(��ԊĎ�)��URL
	**/
	CString GetRecvStatusURL();

	/**
		* @brief  SignalStatus(�p������)��URL�擾
		* @retval SignalStatus(�p������)��URL
	**/
	CString GetRecvPaperURL();

protected:
	UINT				m_nSendThreadID;		//!< ���M�X���b�h�̂h�c
	CWriteDebugLog*		m_pWriteLog;			//!< ���O�N���X�|�C���^
	const char*			m_DeviceName;
};