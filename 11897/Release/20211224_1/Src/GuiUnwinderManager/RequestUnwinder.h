/**
	* @file		RequestUnwinder.h
	* @brief	UWへXJMF要求を行う ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include <map>

#define ErrorLabel		"[ERROR]"
#define SuccessLabel	"[SUCCESS]"

namespace unwinder_manager
{
typedef std::map<std::string, std::string> UwXjmfDataMap;

/**
	@brief	UWへXJMF要求を行う
*/
class CUnwinderPaper;
class CRequestUnwinder
{
public:
	/**
		* @brief  CRequestUnwinderコンストラクタ
	**/
	CRequestUnwinder();

	/**
		* @brief  CRequestUnwinderデストラクタ
	**/
	virtual ~CRequestUnwinder();

	/**
		* @brief  UWへQueryStatusを要求する
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string RequestQueryStatus();

	/**
		* @brief  UWへQueryResourceを要求する
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string RequestQueryResource();

	/**
		* @brief  UWへStopPersChParamsを要求する
		* @param  inStopType : [i]チャネル種別
		*			CMakeComposeUnwinderData::E_Compose_QueryStatus(状態監視用チャネル)
		*			CMakeComposeUnwinderData::E_Compose_QueryResource(用紙情報通知用チャネル)
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string RequestStopPersChParams(int inStopType);

	/**
		* @brief  UWへCommandResourceを要求する
		* @param  inUnwinderPaper : [i]印刷条件情報
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string RequestCommandResource(CUnwinderPaper* inUnwinderPaper);

	/**
		* @brief  UWへCommandWakeupを要求する
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string RequestCommandWakeup();

protected:
	/**
		* @brief  inComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
			* @param  inUwSendDataMap      : [i]UWへ通知するXJMFに必要なデータMAP
			* @param  outUwResponseDataMap : [o]UWからのレスポンスXMLより抽出した必要データMAP
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string ExecuteRequest( UwXjmfDataMap& inUwSendDataMap, UwXjmfDataMap& outUwResponseDataMap );

	/**
		* @brief  inUwXjmfDataMapよりXMLデータを作成する
		* @param  inUwXjmfDataMap : [i]XJMFに必要なデータMAP
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 XMLデータ
	**/
	std::string ExecuteComposeXml( UwXjmfDataMap& inUwXjmfDataMap );

	/**
		* @brief  inXjmfDataをUWに要求する
		* @param  inXjmfData : [i]UWに要求するXMLデータ
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 レスポンスデータ
	**/
	std::string ExecuteSendToUnwinder( std::string& inXjmfData );

	/**
		* @brief  XML(inResponseData)を解析する
		* @param  inResponseData       : [i]UWからのレスポンスデータ(XML)
		* @param  outUwResponseDataMap : [o]UWからのレスポンスデータ(MAP)
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 [SUCCESS]ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
	**/
	std::string ExecuteParseXml( std::string& inResponseData, UwXjmfDataMap& outUwResponseDataMap );

	/**
		* @brief  CXmlCompose.Doit()で正常にXMLデータを作成したかチェックする
		* @param  inData        : [i]CXmlCompose.Doit()の結果データ
		* @param  outResultData : [o]CXmlCompose.Doit()の結果データ(異常の場合のみ)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckXjmfData( const std::string& inData, std::string& outResultData );

	/**
		* @brief  CXmlSender.Doit()で正常にXMLレスポンスを受け取ったかチェックする
		* @param  inData        : [i]CXmlSender.Doit()の結果データ
		* @param  outResultData : [o]CXmlSender.Doit()の結果データ(異常の場合のみ)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL CheckResponseData( const std::string& inData, std::string& outResultData );

	/**
		* @brief  UWからのレスポンスのReturnCodeが0以外の場合の結果編集（仮作成）
		*		  ★★★エラーコードに対してのエラー文言が必要かもしれない
		* @param  inReturnCode : [i]リターンコード
		* @retval [ERROR]ReturnCode=引数のコード
	**/
	std::string ReturnCodeError( const std::string& inReturnCode );

	/**
		* @brief  状態監視用チャネル登録のチャネルIDをINIファイルに保存する
		* @param  inCannelID : [i]チャネルＩＤ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutStatusCannelID( const std::string& inCannelID );

	/**
		* @brief  用紙情報通知用チャネル登録のチャネルIDをINIファイルに保存する
		* @param  inCannelID : [i]チャネルＩＤ
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutResourceCannelID( const std::string& inCannelID );
};

};	// namespace unwinder_manager