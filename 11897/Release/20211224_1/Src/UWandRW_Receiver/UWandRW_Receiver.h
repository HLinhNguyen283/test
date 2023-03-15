/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus��M�������C�� �w�b�_�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

#include "resource.h"

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
	void DeleteLogFolder(CWriteDebugLog* inWriteDebugLog);

private:
	CSendThread*	m_pSendThread;				// SignalStatus���v���O�C���֒ʒm�X���b�h�̃|�C���^
	CReceiveThread*	m_pReceiveThread;			// UW�����SignalStatus��M�X���b�h�̃|�C���^
	UINT			m_nSendThreadID;			// SignalStatus���v���O�C���֒ʒm�X���b�h��ID
	UINT			m_nReceiveThreadID;			//  UW�����SignalStatus��M�X���b�h��ID
};

