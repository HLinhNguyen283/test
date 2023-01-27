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
#include "Ini_UwRwManager_work.h"
#include "MakeComposeUwRwData.h"
#include "XmlCompose.h"
#include "XmlSender.h"
#include "XmlParse.h"
#include "RequestUwRw.h"
#include "RequestUwRwThread.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CRequestUwRwコンストラクタ
**/
CRequestUwRw::CRequestUwRw()
{
}

/**
	* @brief  CRequestUwRwデストラクタ
**/
CRequestUwRw::~CRequestUwRw()
{
}

/**
 * @ brief Initialize data
 * @param inUnwinderData  dataIF object
 * @param inThread main process thread object
**/
void CRequestUwRw::Initialize(CMakeComposeUwRwData* inUnwinderData, CRequestUwRwThread* inThread)
{
	m_UwRwData = inUnwinderData;
	m_Thread = inThread;
}

/**
 * @ brief Finalize data
**/
void CRequestUwRw::Finalize()
{
}

/**
	* @brief  UWへQueryStatusを要求する
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUwRw::RequestQueryStatus()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestQueryStatus] Start");

	// QueryStatusに必要なデータを取得する
	UwRwXjmfDataMap sUwRwSendDataMap = m_UwRwData->ParameterQueryStatus(m_Thread->getDevicename());
	UwRwXjmfDataMap sUwRwResponseDataMap;
	// sUwRwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwRwSendDataMap, sUwRwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= sUwRwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwRwResponseDataMap["ReturnCode"];		// UWからの応答コード
		std::string refID		= sUwRwResponseDataMap["refID"];			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	// チャネルＩＤを設定
				m_UwRwData->PutStatusCannelID(refID, m_Thread->getDevicename());
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestQueryStatus] End");

	return ResultData;
}

/**
	* @brief  UWへQueryResourceを要求する
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUwRw::RequestQueryResource()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestQueryResource] Start");

	// QueryResourceに必要なデータを取得する
	UwRwXjmfDataMap sUwSendDataMap = m_UwRwData->ParameterQueryResource(m_Thread->getDevicename());
	UwRwXjmfDataMap sUwResponseDataMap;
	// sUwRwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( sUwSendDataMap, sUwResponseDataMap );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= sUwResponseDataMap["Type"];			// 処理結果
		std::string ReturnCode	= sUwResponseDataMap["ReturnCode"];		// UWからの応答コード
		std::string refID		= sUwResponseDataMap["refID"];			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" || ReturnCode == "18")
			{	// チャネルＩＤを設定
				m_UwRwData->PutResourceCannelID(refID, m_Thread->getDevicename());
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}

		if (strcmp(m_Thread->getDevicename(), "UW") == 0)
		{
			CIni_UwRwManager_work  ini_UwRwManager_work;
			ini_UwRwManager_work.Initialize(true);

			if (ReturnCode == "0")
			{
				ini_UwRwManager_work.putEnableThicknessMeasureOP("1");
			}
			else if (ReturnCode == "18")
			{
				ini_UwRwManager_work.putEnableThicknessMeasureOP("0");
			}

			ini_UwRwManager_work.Finalize();

		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestQueryResource] End");

	return ResultData;
}

/**
	* @brief  UWへStopPersChParamsを要求する
	* @param  inStopType : [i]チャネル種別
	*			CMakeComposeUwRwData::E_Compose_QueryStatus(状態監視用チャネル)
	*			CMakeComposeUwRwData::E_Compose_QueryResource(用紙情報通知用チャネル)
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUwRw::RequestStopPersChParams(int inStopType)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestStopPersChParams] Start");

	// StopPersChParamsに必要なデータを取得する
	UwRwXjmfDataMap sUwSendDataMap = m_UwRwData->ParameterStopPersChParams(inStopType, m_Thread->getDevicename());
	UwRwXjmfDataMap sUwResponseDataMap;
	// sUwRwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
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
				if ( inStopType == CMakeComposeUwRwData::E_Compose_QueryStatus )
				{	// QueryStatusチャネルID削除
					m_UwRwData->PutStatusCannelID(CannelID, m_Thread->getDevicename());
				}
				else
				{	// QueryResourceチャネルID削除
					m_UwRwData->PutResourceCannelID(CannelID, m_Thread->getDevicename());
				}
			}
			else
			{	// 異常
				ResultData = ReturnCodeError(ReturnCode);
			}
		}
	}
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestStopPersChParams] End");

	return ResultData;
}

/**
	* @brief  UWへCommandResourceを要求する
	* @param  inUnwinderPaper : [i]印刷条件情報
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUwRw::RequestCommandResource(CUwRwPaper* inUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestCommandResource] Start");

	// CommandResourceに必要なデータを取得する
	UwRwXjmfDataMap sUwSendDataMap = m_UwRwData->ParameterCommandResource(*inUnwinderPaper, m_Thread->getDevicename());
	UwRwXjmfDataMap sUwResponseDataMap;
	// sUwRwSendDataMapをXML化してUWに送信しレスポンスから必要な文字列を取り出す
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
	WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::RequestCommandResource] End");

	return ResultData;
}

/**
	* @brief  inComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
		* @param  inUwSendDataMap      : [i]UWへ通知するXJMFに必要なデータMAP
		* @param  outUwRwResponseDataMap : [o]UWからのレスポンスXMLより抽出した必要データMAP
	* @retval 異常の場合 "[ERROR] エラー内容"
	* @retval 正常の場合 "[SUCCESS] 抽出した必要データの文字列（参考データ）"
**/
std::string CRequestUwRw::ExecuteRequest( UwRwXjmfDataMap& inUwRwXjmfDataMap, UwRwXjmfDataMap& outUwRwResponseDataMap )
{
	std::string OutResultData;
	// InComposeDataをXJMFデータに変換する
	std::string XjmfData = ExecuteComposeXml(inUwRwXjmfDataMap);
	if ( CheckXjmfData( XjmfData, OutResultData ) )
	{
		// XJMFデータをUWに送信しレスポンスを受け取る
		std::string ResponseData = ExecuteSendToUwRw(XjmfData);
		if ( CheckResponseData( ResponseData, OutResultData ) )
		{
			// UWからのレスポンスデータを解析する
			OutResultData = ExecuteParseXml(OutResultData, outUwRwResponseDataMap);
			if ( OutResultData.find(ErrorLabel) == std::string::npos )
			{	// 正常の場合
				WriteToLogBuf(LOG_DEBUG, (char*)OutResultData.c_str());
			}
		}
	}

	return OutResultData;
}

/**
	* @brief  inUwRwXjmfDataMapよりXMLデータを作成する
	* @param  inUwRwXjmfDataMap : [i]XJMFに必要なデータMAP
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 XMLデータ
**/
std::string CRequestUwRw::ExecuteComposeXml( UwRwXjmfDataMap& inUwRwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteComposeXml] Start");

		CXmlCompose sComposeXml(inUwRwXjmfDataMap, m_Thread->getDevicename());
		std::string XmlData = sComposeXml.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteComposeXml] End");

		return XmlData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteComposeXml] Exception");
		return std::string(ErrorLabel) + " CRequestUwRw::ExecuteComposeXml Exception";
	}
}

/**
	* @brief  inXjmfDataをUWに要求する
	* @param  inXjmfData : [i]UWに要求するXMLデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 レスポンスデータ
**/
std::string CRequestUwRw::ExecuteSendToUwRw( std::string& inXjmfData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteSendToUwRw] Start");

		// リトライ値を取得する
		UINT RetryCount = m_UwRwData->GetSendRetryCount(m_Thread->getDevicename());
		UINT RetryInterval = m_UwRwData->GetSendRetryInterval(m_Thread->getDevicename());

		CXmlSender sXmlSender(inXjmfData, m_Thread->getDevicename());
		std::string ResponseXmlData;
		for (UINT i = 0; i < RetryCount; i++)
		{
			BOOL nRet = sXmlSender.Doit(ResponseXmlData);
			if (nRet)
			{
				break;
			}
			if (m_Thread->WaitCheckExit(RetryInterval))
			{
				break;
			}
		}

		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteSendToUwRw] End");

		return ResponseXmlData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteSendToUwRw] Exception");
		return std::string(ErrorLabel) + " CRequestUwRw::ExecuteSendToUwRw Exception";
	}
}

/**
	* @brief  XML(inResponseData)を解析する
	* @param  inResponseData       : [i]UWからのレスポンスデータ(XML)
	* @param  outUwRwResponseDataMap : [o]UWからのレスポンスデータ(MAP)
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 [SUCCESS]ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
**/
std::string CRequestUwRw::ExecuteParseXml( std::string& inResponseData, UwRwXjmfDataMap& outUwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteParseXml] Start");

		// UWからのレスポンスデータを解析する
		CXmlParseData sParseXmlData(inResponseData, outUwXjmfDataMap);
		std::string PickupData = sParseXmlData.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteParseXml] End");

		return PickupData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::ExecuteParseXml] Exception");
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
BOOL CRequestUwRw::CheckXjmfData( const std::string& inData, std::string& outResultData )
{
	if ( inData.find(ErrorLabel) == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::CheckXjmfData] Error");
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
BOOL CRequestUwRw::CheckResponseData( const std::string& inData, std::string& outResultData )
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
			WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::CheckResponseData] Data Error");
			outResultData = inData;
			return FALSE;
		}
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUwRw::CheckResponseData] Error");
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
std::string  CRequestUwRw::ReturnCodeError( const std::string& inReturnCode )
{
	std::stringstream ss;
	ss << "[CRequestUwRw::ReturnCodeError] ReturnCode=" << inReturnCode;
	WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());

	return std::string(ErrorLabel) + "ReturnCode=" + inReturnCode;
}

};	// namespace uwrw_manager