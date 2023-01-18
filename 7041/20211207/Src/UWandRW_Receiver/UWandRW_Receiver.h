/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus��M�������C�� �w�b�_�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/

#pragma once

#include "resource.h"

class CSendThread;
class CReceiveThread;
class CMainFunction
{
public:
	void Doit();
	void DeleteLogFolder(CWriteDebugLog* inWriteDebugLog);

private:
	CSendThread*	m_pSendThread;
	CReceiveThread*	m_pReceiveThread;
	UINT			m_nSendThreadID;
	UINT			m_nReceiveThreadID;
};

