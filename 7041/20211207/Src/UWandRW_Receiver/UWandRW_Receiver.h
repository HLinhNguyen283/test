/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus受信処理メイン ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
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

