/**
	* @file		Server.h
	* @brief	UW����̗v����M �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
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
	CServer(UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog);

	/**
		* @brief  CServer �f�X�g���N�V����
	**/
	virtual ~CServer();

	/**
		* @brief  �T�[�o�[(UW�����JMF����M���鏈��)
	**/
	void HttpServerMain();

private:
	/**
		* @brief  POST��M����
		* @param  hReqQueue		: [i]HttpCreateRequestQueue�̃|�C���^
		* @param  pHttpRequest	: [i]PHTTP_REQUEST�̃|�C���^
		* @retval 0(�Œ�)
	**/
	DWORD HttpPostReceive(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest);

private:
	UINT				m_nSendThreadID;		//!< ���M�X���b�h�̂h�c
	CWriteDebugLog*		m_pWriteLog;			//!< ���O�N���X�|�C���^
};