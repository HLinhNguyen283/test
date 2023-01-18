/**
	* @file		ReceiveThread.h
	* @brief	UWからのSignalStatus受信スレッド ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/
#pragma once

/**
* @brief JI JMF通信 JMFを受信するスレッド
*/
class CWriteDebugLog;
class CReceiveThread
{
public:
	/**
		* @brief  CReceiveThread コンストラクション
	**/
	CReceiveThread();

	/**
		* @brief  CReceiveThread デストラクション
	**/
	virtual ~CReceiveThread();

	/**
		* @brief  スレッドを起動する
		* @param  InSendThreadID : [i]送信スレッドＩＤ
		* @param  InWriteDebugLog : [i]ログクラスのポインタ
		* @retval スレッドＩＤ
	**/
	UINT	Start( UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog );

	/**
		* @brief  スレッドメイン処理
		* @retval 0(固定)
	**/
	UINT	MainLoop();

	HANDLE	GetThreadHandle() { return m_hThread; }

protected:
	/**
		* @brief  スレッド処理
		* @param  pData : [i]CReceiveThreadのポインタ
		* @retval 0(固定)
	**/
	static UINT __stdcall ThreadFunction( void* pData );

protected:
	UINT				m_nSendThreadID;		//!< 送信スレッドＩＤ
	HANDLE				m_hThread;				//!< スレッドハンドル
	CWriteDebugLog*		m_pWriteLog;			//!< ログクラスのポインタ
};