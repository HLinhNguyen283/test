/**
	* @file		ReceiveSignalStatus.h
	* @brief	SignalStatus受信 ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#define ErrorLabel		"[ERROR]"

namespace unwinder_manager
{
typedef std::map<std::string, std::string> UwXjmfDataMap;

class CReceiveSignalStatus
{
public:
	/**
		* @brief  CReceiveSignalStatusコンストラクタ
		* @param  pWnd  : [i]★★★テスト用★★★ 親ウィンドウハンドル
	**/
	CReceiveSignalStatus(CWnd* pWnd);

	/**
		* @brief  CReceiveSignalStatusデストラクタ
	**/
	virtual ~CReceiveSignalStatus();

	/**
		* @brief  UWandRW_Receiverから通知されるSignalStatusを受信する
	**/
	BOOL MainLoop();

	/**
		* @brief  SignalStatus(STATUS)タイムアウトタイマー設定
	**/
	void SetTimerStatusReceive();

	/**
		* @brief  SignalStatus(PAPER)タイムアウトタイマー設定
	**/
	void SetTimerPaperReceive();

	/**
		* @brief  SignalStatus(STATUS)タイムアウトタイマー停止
	**/
	void KillTimerStatusReceive();

	/**
		* @brief  SignalStatus(PAPER)タイムアウトタイマー停止
	**/
	void KillTimerPaperReceive();

protected:
	/**
		* @brief  Pipe読み込み
		* @param  inPipe  : [i]PIPEハンドル
		* @param  outData : [i]読み込みデータのポインタ
		* @param  inSize  : [i]読み込みデータの長さ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL ReadData( HANDLE inPipe, char* outData, DWORD inSize );

	/**
		* @brief  UWandRW_Receiverから通知されるSignalStatusを受信する
		* @param  inSize : [i]読み込みデータの長さ
	**/
	BOOL AnalyzeData( const std::string& inXmldata );

	/**
		* @brief  inSignalDataを解析する
		* @param  inSignalData     : [i]UWからのSignalStatusデータ
		* @param  outUwXjmfDataMap : [o]SignalStatusから取り出したデータ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	std::string ExecuteParseXml( const std::string& inSignalData, UwXjmfDataMap& outUwXjmfDataMap );

	/**
		* @brief  ステータス情報受信時の処理
		* @param  inStatus : [i]UWから通知されたステータス
		* @retval TRUE(固定)
	**/
	BOOL ReceiveStatusInfo( const std::string& inStatus );

	/**
		* @brief  用紙情報受信時の処理
		* @param  inDescriptiveName	 : [i]UWから通知された印刷条件名
		* @param  inDimension		 : [i]UWから通知されたDimension
		* @param  inMediaType		 : [i]UWから通知されたMediaType
		* @param  inRollDiameter	 : [i]UWから通知されたRollDiameter
		* @param  inThickness		 : [i]UWから通知されたThickness
		* @retval iRUE(固定)
	**/
	BOOL ReceivePaperInfo( const std::string& inDescriptiveName,
			const std::string& inDimension,
			const std::string& inMediaType,
			const std::string& inRollDiameter,
			const std::string& inThickness );

protected:
	// ★★★テスト用★★★
	void DataDisp( const std::string& inResultData );
	CWnd* m_pWnd;
};

};	// namespace unwinder_manager