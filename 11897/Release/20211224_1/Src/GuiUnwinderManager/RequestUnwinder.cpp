/**
	* @file		RequestUnwinder.cpp
	* @brief	UWへXJMF要求を行う
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include "Ini_UnwinderManager_work.h"
#include "MakeComposeUnwinderData.h"
#include "XmlCompose.h"
#include "XmlSender.h"
#include "XmlParse.h"
#include "RequestUnwinder.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  CRequestUnwinderコンストラクタ
**/
CRequestUnwinder::CRequestUnwinder()
{
}

/**
	* @brief  CRequestUnwinderデストラクタ
**/
CRequestUnwinder::~CRequestUnwinder()
{
}

/**
	* @brief  UWへQueryStatusを要求する
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::RequestQueryStatus()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryStatus] Start");

	// QueryStatusに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	UwXjmfDataMap sUwSendDataMap = sUnwinderData.ParameterQueryStatus();
	UwXjmfDataMap sUwResponseDataMap;
	// sUwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= sUwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		std::string refID		= sUwResponseDataMap["refID"];			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	// チャネルＩＤを設定
				PutStatusCannelID(refID);
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryStatus] End");

	return ResultData;
}

/**
	* @brief  UWへQueryResourceを要求する
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::RequestQueryResource()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryResource] Start");

	// QueryResourceに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	UwXjmfDataMap sUwSendDataMap = sUnwinderData.ParameterQueryResource();
	UwXjmfDataMap sUwResponseDataMap;
	// sUwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= sUwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		std::string refID		= sUwResponseDataMap["refID"];			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	// チャネルＩＤを設定
				PutResourceCannelID(refID);
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryResource] End");

	return ResultData;
}

/**
	* @brief  UWへStopPersChParamsを要求する
	* @param  inStopType : [i]チャネル種別
	*			CMakeComposeUnwinderData::E_Compose_QueryStatus(状態監視用チャネル)
	*			CMakeComposeUnwinderData::E_Compose_QueryResource(用紙情報通知用チャネル)
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::RequestStopPersChParams(int inStopType)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestStopPersChParams] Start");

	// StopPersChParamsに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	UwXjmfDataMap sUwSendDataMap = sUnwinderData.ParameterStopPersChParams(inStopType);
	UwXjmfDataMap sUwResponseDataMap;
	// sUwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= sUwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode == "0" )
			{
				std::string CannelID("");
				if ( inStopType == CMakeComposeUnwinderData::E_Compose_QueryStatus )
				{	// QueryStatusチャネルID削除
					PutStatusCannelID(CannelID);
				}
				else
				{	// QueryResourceチャネルID削除
					PutResourceCannelID(CannelID);
				}
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestStopPersChParams] End");

	return ResultData;
}

/**
	* @brief  UWへCommandResourceを要求する
	* @param  inUnwinderPaper : [i]印刷条件情報
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::RequestCommandResource(CUnwinderPaper* inUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandResource] Start");

	// CommandResourceに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	UwXjmfDataMap sUwSendDataMap = sUnwinderData.ParameterCommandResource(*inUnwinderPaper);
	UwXjmfDataMap sUwResponseDataMap;
	// sUwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		std::string Type		= sUwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode != "0" )
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandResource] End");

	return ResultData;
}

/**
	* @brief  UWへCommandWakeupを要求する
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::RequestCommandWakeup()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandWakeup] Start");

	// CommandWakeupに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	UwXjmfDataMap sUwSendDataMap = sUnwinderData.ParameterCommandWakeup();
	UwXjmfDataMap sUwResponseDataMap;
	// sUwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		std::string Type		= sUwResponseDataMap["Type"];				// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode != "0" )
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandWakeup] End");

	return ResultData;
}

/**
	* @brief  inComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
		* @param  inUwSendDataMap      : [i]UWへ通知するXJMFに必要なデータMAP
		* @param  outUwResponseDataMap : [o]UWからのレスポンスXMLより抽出した必要データMAP
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUnwinder::ExecuteRequest( UwXjmfDataMap& inUwXjmfDataMap, UwXjmfDataMap& outUwResponseDataMap )
{
	std::string OutResultData;
	// InComposeDataをXJMFデータに変換する
	std::string XjmfData = ExecuteComposeXml(inUwXjmfDataMap);
	if ( CheckXjmfData( XjmfData, OutResultData ) )
	{
		// XJMFデータをUWに送信しレスポンスを受け取る
		std::string ResponseData = ExecuteSendToUnwinder(XjmfData);
		if ( CheckResponseData( ResponseData, OutResultData ) )
		{
			// UWからのレスポンスデータを解析する
			OutResultData = ExecuteParseXml(OutResultData, outUwResponseDataMap);
			if ( OutResultData.find(ErrorLabel) == std::string::npos )
			{	// 正常の場合
				WriteToLogBuf(LOG_DEBUG, (char*)OutResultData.c_str());
			}
		}
	}

	return OutResultData;
}

/**
	* @brief  inUwXjmfDataMapよりXMLデータを作成する
	* @param  inUwXjmfDataMap : [i]XJMFに必要なデータMAP
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 XMLデータ
**/
std::string CRequestUnwinder::ExecuteComposeXml( UwXjmfDataMap& inUwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] Start");

		CXmlCompose sComposeXml(inUwXjmfDataMap);
		std::string XmlData = sComposeXml.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] End");

		return XmlData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] Exception");
		return std::string(ErrorLabel) + " CRequestUnwinder::ExecuteComposeXml Exception";
	}
}

/**
	* @brief  inXjmfDataをUWに要求する
	* @param  inXjmfData : [i]UWに要求するXMLデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 レスポンスデータ
**/
std::string CRequestUnwinder::ExecuteSendToUnwinder( std::string& inXjmfData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] Start");

		CXmlSender sXmlSender(inXjmfData);
		std::string ResponseXmlData = sXmlSender.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] End");

		return ResponseXmlData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] Exception");
		return std::string(ErrorLabel) + " CRequestUnwinder::ExecuteSendToUnwinder Exception";
	}
}

/**
	* @brief  XML(inResponseData)を解析する
	* @param  inResponseData       : [i]UWからのレスポンスデータ(XML)
	* @param  outUwResponseDataMap : [o]UWからのレスポンスデータ(MAP)
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 [SUCCESS]ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
**/
std::string CRequestUnwinder::ExecuteParseXml( std::string& inResponseData, UwXjmfDataMap& outUwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] Start");

		// UWからのレスポンスデータを解析する
		CXmlParseData sParseXmlData(inResponseData, outUwXjmfDataMap);
		std::string PickupData = sParseXmlData.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] End");

		return PickupData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] Exception");
		return std::string(ErrorLabel) + " SCRequestUnwinder::ExecuteParseXml Exception";
	}
}

/**
	* @brief  CXmlCompose.Doit()で正常にXMLデータを作成したかチェックする
	* @param  inData        : [i]CXmlCompose.Doit()の結果データ
	* @param  outResultData : [o]CXmlCompose.Doit()の結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckXjmfData( const std::string& inData, std::string& outResultData )
{
	if ( inData.find(ErrorLabel) == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::CheckXjmfData] Error");
		outResultData = inData;
		return FALSE;	// 異常
	}

	return TRUE;	// 正常
}

/**
	* @brief  CXmlSender.Doit()で正常にXMLレスポンスを受け取ったかチェックする
	* @param  inData        : [i]CXmlSender.Doit()の結果データ
	* @param  outResultData : [o]CXmlSender.Doit()の結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckResponseData( const std::string& inData, std::string& outResultData )
{
	if ( inData.find(ErrorLabel) == std::string::npos )
	{	
		size_t pos = inData.find("<");
		if ( std::string::npos != pos )
		{	// XJMFデータのみ出力データに設定
			outResultData = inData.substr(pos);
			return TRUE;	// 正常
		}
		else
		{
			WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::CheckResponseData] Data Error");
			outResultData = inData;
			return FALSE;
		}
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::CheckResponseData] Error");
		outResultData = inData;
		return FALSE;	// 異常
	}
}

/**
	* @brief  UWからのレスポンスのReturnCodeが0以外の場合の結果編集（仮作成）
	*		  ★★★エラーコードに対してのエラー文言が必要かもしれない
	* @param  inReturnCode : [i]リターンコード
	* @retval [ERROR]ReturnCode=引数のコード
**/
std::string  CRequestUnwinder::ReturnCodeError( const std::string& inReturnCode )
{
	std::stringstream ss;
	ss << "[CRequestUnwinder::ReturnCodeError] ReturnCode=" << inReturnCode;
	WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());

	return std::string(ErrorLabel) + "ReturnCode=" + inReturnCode;
}

/**
	* @brief  状態監視用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  inCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::PutStatusCannelID( const std::string& inCannelID )
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putStatusCannelID(inCannelID);
}

/**
	* @brief  用紙情報通知用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  inCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::PutResourceCannelID( const std::string& inCannelID )
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putResourceCannelID(inCannelID);
}

};	// namespace unwinder_manager