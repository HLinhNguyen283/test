/**
	* @file		Server.h
	* @brief	UWからの要求受信 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/
#pragma once

#include "CommonNet.h"

/**
* JI JMF通信 JMFを受信するクラス
* 共通部分のメソッド
* @brief JI JMF通信 JMFを受信するクラス
*/
class CWriteDebugLog;
class CServer
{
public:
	/**
		* @brief  CServer コントラクション
		* @param  InSendThreadID   : [i]送信スレッドのＩＤ
		* @param  InWriteDebugLog  : [i]ログクラスのポインタ
	**/
	CServer(UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog);

	/**
		* @brief  CServer デストラクション
	**/
	virtual ~CServer();

	/**
		* @brief  サーバー(UWからのJMFを受信する処理)
	**/
	void HttpServerMain();

private:
	/**
		* @brief  POST受信処理
		* @param  hReqQueue		: [i]HttpCreateRequestQueueのポインタ
		* @param  pHttpRequest	: [i]PHTTP_REQUESTのポインタ
		* @retval 0(固定)
	**/
	DWORD HttpPostReceive(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest);

private:
	UINT				m_nSendThreadID;		//!< 送信スレッドのＩＤ
	CWriteDebugLog*		m_pWriteLog;			//!< ログクラスポインタ
};