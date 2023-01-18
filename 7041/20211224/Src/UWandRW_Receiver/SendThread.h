/**
	* @file		SendThread.h
	* @brief	SignalStatusをプラグインへ通知スレッド ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

/**
* @brief JI JMF通信 JMFを受信するスレッド
*/
class CWriteDebugLog;
class CSendThread
{
public:
	/**
		* @brief  CSendThread コンストラクション
	**/
	CSendThread();

	/**
		* @brief  CSendThread デストラクション
	**/
	virtual ~CSendThread();

	/**
		* @brief  スレッドを起動する
		* @param  InWriteDebugLog : [i]ログクラスのポインタ
		* @retval スレッドＩＤ
	**/
	UINT	Start( CWriteDebugLog* InWriteDebugLog );

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

	/**
		* @brief  SignalStatusをプラグインに通知する
	**/
	void WriteSignalStatus(const CString& InData);

	/**
		* @brief  Pipe書き込み
		* @param  InPipe : [i]PIPEハンドル
		* @param  InData : [i]書き込みデータのポインタ
		* @param  InSize : [i]書き込みデータの長さ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL WriteData( HANDLE InPipe, TCHAR* InData, DWORD InSize );

protected:
	HANDLE				m_hThread;				//!< スレッドハンドル
	CWriteDebugLog*		m_pWriteLog;			//!< ログクラスのポインタ
	HANDLE				m_hInitEvent;			//!< 初期化完了イベントハンドル
	HANDLE				m_hPipe;				//!< Pipeハンドル
};