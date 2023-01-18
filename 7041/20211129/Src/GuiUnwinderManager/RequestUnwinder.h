/**
	* @file		RequestUnwinder.h
	* @brief	UWへXJMF要求を行う ヘッダファイル
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
**/

#pragma once

#define ErrorLabel		"[ERROR]"
#define SuccessLabel	"[SUCCESS]"

namespace unwinder_manager
{

/**
	@brief	UWへXJMF要求を行う
*/
class CUnwinderPaper;
class CRequestUnwinder
{
public:
	CRequestUnwinder();
	virtual ~CRequestUnwinder();

	/**
		* @brief  UWへQueryStatusを要求する
		* @retval 異常の場合 [ERROR]エラー内容
		* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
		* @retval 正常の場合 [SUCCESS]レスポンス内容
	**/
	std::string RequestQueryStatus();

	/**
		* @brief  UWへQueryResourceを要求する
		* @retval 異常の場合 [ERROR]エラー内容
		* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
		* @retval 正常の場合 [SUCCESS]レスポンス内容
	**/
	std::string RequestQueryResource();

	/**
		* @brief  UWへStopPersChParamsを要求する
		* @param  InStopType : [i]チャネル種別
		*			CMakeComposeUnwinderData::E_Compose_QueryStatus(状態監視用チャネル)
		*			CMakeComposeUnwinderData::E_Compose_QueryResource(用紙情報通知用チャネル)
		* @retval 異常の場合 [ERROR]エラー内容
		* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
		* @retval 正常の場合 [SUCCESS]レスポンス内容
	**/
	std::string RequestStopPersChParams(int InStopType);

	/**
		* @brief  UWへCommandResourceを要求する
		* @param  InUnwinderPaper : [i]印刷条件情報
		* @retval 異常の場合 [ERROR]エラー内容
		* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
		* @retval 正常の場合 [SUCCESS]レスポンス内容
	**/
	std::string RequestCommandResource(CUnwinderPaper* InUnwinderPaper);

	/**
		* @brief  UWへCommandWakeupを要求する
		* @retval 異常の場合 [ERROR]エラー内容
		* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
		* @retval 正常の場合 [SUCCESS]レスポンス内容
	**/
	std::string RequestCommandWakeup();

protected:
	/**
		* @brief  InComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
		* @param  InComposeData : [i]UWへ通知するXJMFに必要なデータ文字列
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 レスポンス内容
	**/
	std::string ExecuteRequest( const std::string& InComposeData );

	/**
		* @brief  InComposeDataをXML化するUWandRW_Compose_Xml.exeを呼び出し終了を待つ
		* @param  InComposeData : [i]XJMFに必要なデータ文字列
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 SUCCESS + XJMFデータ
	**/
	std::string ExecuteComposeXml( const std::string& InComposeData );

	/**
		* @brief  InXjmfDataをUWに通知するUWandRW_Sender.exeを呼び出し終了を待つ
		* @param  InXjmfData : [i]UWに要求するXJMFデータ
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 SUCCESS + レスポンスデータ
	**/
	std::string ExecuteSendToUnwinder( std::string& InXjmfData );

	/**
		* @brief  InResponseDataを解析するUWandRW_Parse_Xml.exeを呼び出し終了を待つ
		* @param  InResponseData : [i]UWからのレスポンスデータ
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
	**/
	std::string ExecuteParseXml( std::string& InResponseData );

	/**
		* @brief  UWandRW_Compose_Xml.exeが正常にXJMFデータを出力したかチェックする
		* @param  InData        : [i]UWandRW_Compose_Xml.exeの結果データ
		* @param  OutResultData : [o]UWandRW_Compose_Xml.exeの結果データ(異常の場合のみ)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckXjmfData( const std::string& InData, std::string& OutResultData );

	/**
		* @brief  UWandRW_Sender.exeが正常にレスポンスデータを出力したかチェックする
		* @param  InData        : [i]UWandRW_Sender.exeの結果データ
		* @param  OutResultData : [o]レスポンスのXJMFデータ(正常の場合のみ)、UWandRW_Sender.exeの結果データ(異常の場合のみ)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckResponseData( const std::string& InData, std::string& OutResultData );

	/**
		* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
		* @param  InData        : [i]UWandRW_Parse_Xml.exeの結果データ
		* @param  OutResultData : [o]UWandRW_Parse_Xml.exeの結果データ(異常の場合のみ)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckPickupData( const std::string& InData, std::string& OutResultData );

	/**
		* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
		* @param  InScrData    : [i]UWandRW_Parse_Xml.exeの結果データ
		* @param  InSelectName : [i]抽出データ名称
		* @retval 抽出データ
	**/
	std::string SelectPickupData( const std::string& InScrData, const std::string& InSelectName );

	/**
		* @brief  UWからのレスポンスのReturnCodeが0以外の場合の結果編集
		* @param  InReturnCode : [i]リターンコード
		* @retval [ERROR]ReturnCode=引数のコード
	**/
	std::string ReturnCodeError( const std::string& InReturnCode );

	/**
		* @brief  状態監視用チャネル登録のチャネルIDをINIファイルに保存する
		* @param  InCannelID : [i]チャネルＩＤ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutStatusCannelID( const std::string& InCannelID );

	/**
		* @brief  用紙情報通知用チャネル登録のチャネルIDをINIファイルに保存する
		* @param  InCannelID : [i]チャネルＩＤ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutResourceCannelID( const std::string& InCannelID );
};

};	// namespace ink_code_manager