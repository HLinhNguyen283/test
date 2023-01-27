/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus��M�������C�� �w�b�_�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

#include "resource.h"

#define DEF_MAX_NUMBER_DEVICE	2

class CSendThread;
class CReceiveThread;
class CMainFunction
{
public:
	/**
		* @brief  CMainFunction �R���X�g���N�^
	**/
	CMainFunction();

	/**
		* @brief  CMainFunction �f�X�g���N�^
	**/
	virtual ~CMainFunction();

	/**
		* @brief  UWandRW_Receiver�̎又��
	**/
	void Doit();

	/**
		* @brief  �Â����O���폜����
	**/
	void DeleteLogFolder(CWriteDebugLog* inWriteDebugLog, const char* inDeviceName);

private:
	CSendThread*	m_pSendThread[DEF_MAX_NUMBER_DEVICE];				// SignalStatus���v���O�C���֒ʒm�X���b�h�̃|�C���^
	CReceiveThread*	m_pReceiveThread[DEF_MAX_NUMBER_DEVICE];			// UW�����SignalStatus��M�X���b�h�̃|�C���^
	UINT			m_nSendThreadID[DEF_MAX_NUMBER_DEVICE];			// SignalStatus���v���O�C���֒ʒm�X���b�h��ID
	UINT			m_nReceiveThreadID[DEF_MAX_NUMBER_DEVICE];			//  UW�����SignalStatus��M�X���b�h��ID
};

