/**
	* @file		Server.h
	* @brief	UWからの要求受信 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
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
	CServer(UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog, const char* inDeviceName);

	/**
		* @brief  CServer デストラクション
	**/
	virtual ~CServer();

	/**
		* @brief  サーバー(UWからのJMFを受信する処理)
	**/
	void HttpServerMain();

protected:
	/**
		* @brief  POST受信処理
		* @param  hReqQueue		: [i]HttpCreateRequestQueueのポインタ
		* @param  pHttpRequest	: [i]PHTTP_REQUESTのポインタ
		* @retval 0(固定)
	**/
	DWORD HttpPostReceive(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest);

	/**
		* @brief  コード変換(UTF8 → Shift-JIS)
		* @param  InUTF8 : [i]UTF8の文字列
		* @retval Shift-JISの文字列
	**/
	std::string ConvUTF8toShiftJIS(const std::string& InUTF8);

	/**
		* @brief  SignalStatus(状態監視)のURL取得
		* @retval SignalStatus(状態監視)のURL
	**/
	CString GetRecvStatusURL();

	/**
		* @brief  SignalStatus(用紙測定)のURL取得
		* @retval SignalStatus(用紙測定)のURL
	**/
	CString GetRecvPaperURL();

protected:
	UINT				m_nSendThreadID;		//!< 送信スレッドのＩＤ
	CWriteDebugLog*		m_pWriteLog;			//!< ログクラスポインタ
	const char*			m_DeviceName;
};