/**
	* @file		RequestUnwinder.cpp
	* @brief	UWへXJMF要求を行う
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include "MakeComposeUnwinderData.h"
#include "Utility.h"
#include "Ini_UnwinderManager_work.h"
#include "ExecCommand.h"
#include "RequestUnwinder.h"
// ===========================================================================

namespace unwinder_manager
{

//const std::string ErrorLabel("[ERROR]");
//const std::string SuccessLabel("[SUCCESS]");

// constructor
CRequestUnwinder::CRequestUnwinder()
{
}

// destructor
CRequestUnwinder::~CRequestUnwinder()
{
}

/**
	* @brief  UWへQueryStatusを要求する
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestQueryStatus()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryStatus] Start");

	// QueryStatusに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterQueryStatus();
	// ComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( ComposeData );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= SelectPickupData(ResultData, "Type");				// 処理結果
		std::string ReturnCode	= SelectPickupData(ResultData, "ReturnCode");		// UWからの応答コード
		std::string refID		= SelectPickupData(ResultData, "refID");			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	// チャネルＩＤを設定
				PutStatusCannelID(refID);
				ResultData = SuccessLabel + ResultData;
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
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestQueryResource()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestQueryResource] Start");

	// QueryResourceに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterQueryResource();
	// ComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( ComposeData );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= SelectPickupData(ResultData, "Type");				// 処理結果
		std::string ReturnCode	= SelectPickupData(ResultData, "ReturnCode");		// UWからの応答コード
		std::string refID		= SelectPickupData(ResultData, "refID");			// チャネルＩＤ
		if ( Type == "ResponseStatus" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	// チャネルＩＤを設定
				PutResourceCannelID(refID);
				ResultData = SuccessLabel + ResultData;
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
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestStopPersChParams(int inStopType)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestStopPersChParams] Start");

	// StopPersChParamsに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterStopPersChParams(inStopType);
	// ComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( ComposeData );


	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		// レスポンス文字列を分解する
		std::string Type		= SelectPickupData(ResultData, "Type");				// 処理結果
		std::string ReturnCode	= SelectPickupData(ResultData, "ReturnCode");		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	
				ResultData = SuccessLabel + ResultData;
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
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestCommandResource(CUnwinderPaper* inUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandResource] Start");

	// CommandResourceに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterCommandResource(*inUnwinderPaper);
	// ComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( ComposeData );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		std::string Type		= SelectPickupData(ResultData, "Type");				// 処理結果
		std::string ReturnCode	= SelectPickupData(ResultData, "ReturnCode");		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	
				ResultData = SuccessLabel + ResultData;
			}
			else
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
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestCommandWakeup()
{
	WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::RequestCommandWakeup] Start");

	// CommandWakeupに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterCommandWakeup();
	// ComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	std::string ResultData = ExecuteRequest( ComposeData );

	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{	// 正常の場合
		std::string Type		= SelectPickupData(ResultData, "Type");				// 処理結果
		std::string ReturnCode	= SelectPickupData(ResultData, "ReturnCode");		// UWからの応答コード
		if ( Type == "Response" )
		{	// 正常
			if ( ReturnCode == "0" )
			{	
				ResultData = SuccessLabel + ResultData;
			}
			else
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
	* @param  inComposeData : [i]UWへ通知するXJMFに必要なデータ文字列
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 レスポンス内容
**/
std::string CRequestUnwinder::ExecuteRequest( const std::string& inComposeData )
{
	std::string OutResultData;
	// InComposeDataをXJMFデータに変換する
	std::string XjmfData = ExecuteComposeXml(inComposeData);
	if ( CheckXjmfData( XjmfData, OutResultData ) )
	{
		// XJMFデータをUWに送信しレスポンスを受け取る
		std::string ResponseData = ExecuteSendToUnwinder(XjmfData);
		if ( CheckResponseData( ResponseData, OutResultData ) )
		{
			// UWからのレスポンスデータを解析する
			std::string PickupData = ExecuteParseXml(OutResultData);
			if ( CheckPickupData( PickupData, OutResultData ) )
			{
				// 正常の場合はレスポンスから取得した必要データを設定する
				OutResultData = PickupData;
			}
		}
	}

	return OutResultData;
}

/**
	* @brief  inComposeDataをXML化するUWandRW_Compose_Xml.exeを呼び出し終了を待つ
	* @param  inComposeData : [i]XJMFに必要なデータ文字列
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 SUCCESS + XJMFデータ
**/
std::string CRequestUnwinder::ExecuteComposeXml( const std::string& inComposeData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] Start");

		std::string applicationName = "UWandRW_Compose_Xml.exe";
		std::string command = "\"" + inComposeData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Compose_Xml.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string XjmfData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] End");

		return XjmfData;
	}
	catch(...)
	{	// UWandRW_Compose_Xml.exeがない場合等
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteComposeXml] Exception Startup UWandRW_Compose_Xml.exe");
		return std::string(ErrorLabel) + " Startup UWandRW_Compose_Xml.exe";
	}
}

/**
	* @brief  inXjmfDataをUWに通知するUWandRW_Sender.exeを呼び出し終了を待つ
	* @param  inXjmfData : [i]UWに要求するXJMFデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 SUCCESS + レスポンスデータ
**/
std::string CRequestUnwinder::ExecuteSendToUnwinder( std::string& inXjmfData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] Start");

		CUtility::replaceString(inXjmfData,"\"", "\\\"");		// XML内のダブルコートはエスケープする必要がある
		CUtility::replaceString(inXjmfData,"\"/>", "\" />");	// 何故かダブルコートと１行でノード終了の/>の間にブランクがないとXMLロードで例外が発生する

		std::string applicationName = "UWandRW_Sender.exe";
		std::string command = "\"" + inXjmfData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Sender.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string ResponseData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] End");

		return ResponseData;
	}
	catch(...)
	{	// UWandRW_Sender.exeがない場合等
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteSendToUnwinder] Exception Startup UWandRW_Sender.exe");
		return std::string(ErrorLabel) + " Startup UWandRW_Sender.exe";
	}
}

/**
	* @brief  inResponseDataを解析するUWandRW_Parse_Xml.exeを呼び出し終了を待つ
	* @param  inResponseData : [i]UWからのレスポンスデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
**/
std::string CRequestUnwinder::ExecuteParseXml( std::string& inResponseData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] Start");

		CUtility::replaceString(inResponseData,"\"", "\\\"");		// XML内のダブルコートはエスケープする必要がある
		CUtility::replaceString(inResponseData,"\"/>", "\" />");	// 何故かダブルコートと１行でノード終了の/>の間にブランクがないとXMLロードで例外が発生する

		std::string applicationName = "UWandRW_Parse_Xml.exe";
		std::string command = "\"" + inResponseData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Parse_Xml.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string PickupData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] End");

		return PickupData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::ExecuteParseXml] Exception Startup UWandRW_Parse_Xml.exe");
		return std::string(ErrorLabel) + " Startup UWandRW_Parse_Xml.exe";
	}
}

/**
	* @brief  UWandRW_Compose_Xml.exeが正常にXJMFデータを出力したかチェックする
	* @param  inData        : [i]UWandRW_Compose_Xml.exeの結果データ
	* @param  outResultData : [o]UWandRW_Compose_Xml.exeの結果データ(異常の場合のみ)
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
	* @brief  UWandRW_Sender.exeが正常にレスポンスデータを出力したかチェックする
	* @param  inData        : [i]UWandRW_Sender.exeの結果データ
	* @param  outResultData : [o]レスポンスのXJMFデータ(正常の場合のみ)、UWandRW_Sender.exeの結果データ(異常の場合のみ)
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
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  inData        : [i]UWandRW_Parse_Xml.exeの結果データ
	* @param  outResultData : [o]UWandRW_Parse_Xml.exeの結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckPickupData( const std::string& inData, std::string& outResultData )
{
	if ( inData.find(ErrorLabel) == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CRequestUnwinder::CheckPickupData] Error");
		outResultData = inData;
		return FALSE;	// 異常
	}
}

/**
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  inScrData    : [i]UWandRW_Parse_Xml.exeの結果データ
	* @param  inSelectName : [i]抽出データ名称
	* @retval 抽出データ
**/
std::string CRequestUnwinder::SelectPickupData( const std::string& inScrData, const std::string& inSelectName )
{
	std::vector<std::string> strList = CUtility::splitString(inScrData, ' ');
	for( auto ite = strList.begin(); ite != strList.end(); ite++ )
	{
		if ( ite->compare(0,inSelectName.size(),inSelectName) == 0 )
		{
			size_t pos = ite->find("=");
			if ( std::string::npos != pos )
			{
				return ite->substr(pos+1);
			}
		}
	}
	return "";
}

/**
	* @brief  UWからのレスポンスのReturnCodeが0以外の場合の結果編集
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

};	// namespace ink_code_manager