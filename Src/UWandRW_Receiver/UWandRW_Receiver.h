/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus受信処理メイン ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
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
		* @brief  CMainFunction コンストラクタ
	**/
	CMainFunction();

	/**
		* @brief  CMainFunction デストラクタ
	**/
	virtual ~CMainFunction();

	/**
		* @brief  UWandRW_Receiverの主処理
	**/
	void Doit();

	/**
		* @brief  古いログを削除する
	**/
	void DeleteLogFolder(CWriteDebugLog* inWriteDebugLog, const char* inDeviceName);

private:
	CSendThread*	m_pSendThread[DEF_MAX_NUMBER_DEVICE];				// SignalStatusをプラグインへ通知スレッドのポインタ
	CReceiveThread*	m_pReceiveThread[DEF_MAX_NUMBER_DEVICE];			// UWからのSignalStatus受信スレッドのポインタ
	UINT			m_nSendThreadID[DEF_MAX_NUMBER_DEVICE];			// SignalStatusをプラグインへ通知スレッドのID
	UINT			m_nReceiveThreadID[DEF_MAX_NUMBER_DEVICE];			//  UWからのSignalStatus受信スレッドのID
};

