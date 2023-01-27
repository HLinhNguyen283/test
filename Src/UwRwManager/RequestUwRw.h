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

namespace uwrw_manager
{
typedef std::map<std::string, std::string> UwRwXjmfDataMap;

/**
	@brief	UWへXJMF要求を行う
*/
class CUwRwPaper;
class CMakeComposeUwRwData;
class CRequestUwRwThread;
class CRequestUwRw
{
public:
	/**
		* @brief  CRequestUwRwコンストラクタ
	**/
	CRequestUwRw();

	/**
		* @brief  CRequestUwRwデストラクタ
	**/
	virtual ~CRequestUwRw();

	/**
	 * @ brief Initialize data
	 * @param inUnwinderData  dataIF object
	 * @param inThread main process thread object
	**/
	void Initialize(CMakeComposeUwRwData* inUnwinderData, CRequestUwRwThread* inThread);
	
	/**
	 * @ brief Finalize data
	**/
	void Finalize();

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
		*			CMakeComposeUwRwData::E_Compose_QueryStatus(状態監視用チャネル)
		*			CMakeComposeUwRwData::E_Compose_QueryResource(用紙情報通知用チャネル)
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
	std::string RequestCommandResource(CUwRwPaper* inUnwinderPaper);

protected:
	/**
		* @brief  inComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
			* @param  inUwSendDataMap      : [i]UWへ通知するXJMFに必要なデータMAP
			* @param  outUwResponseDataMap : [o]UWからのレスポンスXMLより抽出した必要データMAP
		* @retval 異常の場合 "[ERROR] エラー内容"
		* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
	**/
	std::string ExecuteRequest( UwRwXjmfDataMap& inUwSendDataMap, UwRwXjmfDataMap& outUwResponseDataMap );

	/**
		* @brief  inUwXjmfDataMapよりXMLデータを作成する
		* @param  inUwXjmfDataMap : [i]XJMFに必要なデータMAP
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 XMLデータ
	**/
	std::string ExecuteComposeXml( UwRwXjmfDataMap& inUwXjmfDataMap );

	/**
		* @brief  inXjmfDataをUWに要求する
		* @param  inXjmfData : [i]UWに要求するXMLデータ
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 レスポンスデータ
	**/
	std::string ExecuteSendToUwRw( std::string& inXjmfData );

	/**
		* @brief  XML(inResponseData)を解析する
		* @param  inResponseData       : [i]UWからのレスポンスデータ(XML)
		* @param  outUwResponseDataMap : [o]UWからのレスポンスデータ(MAP)
		* @retval 異常の場合 [ERROR]エラー内容
		* @retval 正常の場合 [SUCCESS]ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
	**/
	std::string ExecuteParseXml( std::string& inResponseData, UwRwXjmfDataMap& outUwResponseDataMap );

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

	CMakeComposeUwRwData* m_UwRwData;
	CRequestUwRwThread* m_Thread;
};

};	// namespace uwrw_manager