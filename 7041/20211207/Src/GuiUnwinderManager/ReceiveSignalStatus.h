/**
	* @file		ReceiveSignalStatus.h
	* @brief	SignalStatus受信 ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

#pragma once

namespace unwinder_manager
{

class CReceiveSignalStatus
{
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
	BOOL AnalyzeData( std::string& inXmldata );

	/**
		* @brief  inSignalDataを解析するUWandRW_Parse_Xml.exeを呼び出し終了を待つ
		* @param  inSignalData : [i]UWからのSignalStatusデータ
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
	**/
	std::string ExecuteParseXml( std::string& inSignalData );

	/**
		* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
		* @param  inData        : [i]UWandRW_Parse_Xml.exeの結果データ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckPickupData( const std::string& inData );

	/**
		* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
		* @param  inScrData    : [i]UWandRW_Parse_Xml.exeの結果データ
		* @param  inSelectName : [i]抽出データ名称
		* @retval 抽出データ
	**/
	std::string SelectPickupData( const std::string& inScrData, const std::string& inSelectName );

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

public:
	CReceiveSignalStatus(CWnd* pWnd);
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

	// ★★★テスト用★★★
	void DataDisp( const std::string& s );
	CWnd* m_pWnd;
};

};	// namespace ink_code_manager