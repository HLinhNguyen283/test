/**
	* @file		RequestUnwinder.cpp
	* @brief	UWへXJMF要求を行う
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "MakeComposeUnwinderData.h"
#include "Utility.h"
#include "Ini_UnwinderManager.h"
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
	WriteToLogBuf(LOG_DEBUG,"Start RequestQueryStatus");

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
	WriteToLogBuf(LOG_DEBUG,"Start RequestQueryResource");

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

	WriteToLogBuf(LOG_DEBUG,"End RequestQueryResource");

	return ResultData;
}

/**
	* @brief  UWへStopPersChParamsを要求する
	* @param  InStopType : [i]チャネル種別
	*			CMakeComposeUnwinderData::E_Compose_QueryStatus(状態監視用チャネル)
	*			CMakeComposeUnwinderData::E_Compose_QueryResource(用紙情報通知用チャネル)
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestStopPersChParams(int InStopType)
{
	WriteToLogBuf(LOG_DEBUG,"Start RequestStopPersChParams");

	// StopPersChParamsに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterStopPersChParams(InStopType);
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

	WriteToLogBuf(LOG_DEBUG,"End RequestStopPersChParams");

	return ResultData;
}

/**
	* @brief  UWへCommandResourceを要求する
	* @param  InUnwinderPaper : [i]印刷条件情報
	* @retval 異常の場合 [ERROR]エラー内容
	* 　　　　・UWからのレスポンスのReturnCodeが0以外の場合は、 [ERROR]ReturnCode=番号
	* @retval 正常の場合 [SUCCESS]レスポンス内容
**/
std::string CRequestUnwinder::RequestCommandResource(CUnwinderPaper* InUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"Start RequestCommandResource");

	// CommandResourceに必要なデータを取得する
	CMakeComposeUnwinderData sUnwinderData;
	std::string ComposeData = sUnwinderData.ParameterCommandResource(*InUnwinderPaper);
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

	WriteToLogBuf(LOG_DEBUG,"End RequestCommandResource");

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
	WriteToLogBuf(LOG_DEBUG,"Start RequestCommandWakeup");

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

	WriteToLogBuf(LOG_DEBUG,"End RequestCommandWakeup");

	return ResultData;
}

/**
	* @brief  InComposeDataをXML化してUWに送信しレスポンスから必要な文字列を取り出す
	* @param  InComposeData : [i]UWへ通知するXJMFに必要なデータ文字列
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 レスポンス内容
**/
std::string CRequestUnwinder::ExecuteRequest( const std::string& InComposeData )
{
	WriteToLogBuf(LOG_DEBUG,"Start ExecuteRequest");

	std::string OutResultData;
	// InComposeDataをXJMFデータに変換する
	std::string XjmfData = ExecuteComposeXml(InComposeData);
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

	WriteToLogBuf(LOG_DEBUG,"End ExecuteRequest");

	return OutResultData;
}

/**
	* @brief  InComposeDataをXML化するUWandRW_Compose_Xml.exeを呼び出し終了を待つ
	* @param  InComposeData : [i]XJMFに必要なデータ文字列
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 SUCCESS + XJMFデータ
**/
std::string CRequestUnwinder::ExecuteComposeXml( const std::string& InComposeData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"Start ExecuteComposeXml");

		std::string applicationName = "UWandRW_Compose_Xml.exe";
		std::string command = "\"" + InComposeData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Compose_Xml.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string XjmfData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"End ExecuteComposeXml");

		return XjmfData;
	}
	catch(...)
	{	// UWandRW_Compose_Xml.exeがない場合等
		WriteToLogBuf(LOG_DEBUG,"[Exception] ExecuteComposeXml Startup UWandRW_Compose_Xml.exe");
		return std::string( ErrorLabel) + " Startup UWandRW_Compose_Xml.exe";
	}
}

/**
	* @brief  InXjmfDataをUWに通知するUWandRW_Sender.exeを呼び出し終了を待つ
	* @param  InXjmfData : [i]UWに要求するXJMFデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 SUCCESS + レスポンスデータ
**/
std::string CRequestUnwinder::ExecuteSendToUnwinder( std::string& InXjmfData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"Start ExecuteSendToUnwinder");

		CUtility::replaceString(InXjmfData,"\"", "\\\"");		// XML内のダブルコートはエスケープする必要がある
		CUtility::replaceString(InXjmfData,"\"/>", "\" />");	// 何故かダブルコートと１行でノード終了の/>の間にブランクがないとXMLロードで例外が発生する

		std::string applicationName = "UWandRW_Sender.exe";
		std::string command = "\"" + InXjmfData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Sender.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string ResponseData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"End ExecuteSendToUnwinder");

		return ResponseData;
	}
	catch(...)
	{	// UWandRW_Sender.exeがない場合等
		WriteToLogBuf(LOG_DEBUG,"[Exception] ExecuteSendToUnwinder Startup UWandRW_Sender.exe");
		return std::string( ErrorLabel) + " Startup UWandRW_Sender.exe";
	}
}

/**
	* @brief  InResponseDataを解析するUWandRW_Parse_Xml.exeを呼び出し終了を待つ
	* @param  InResponseData : [i]UWからのレスポンスデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
**/
std::string CRequestUnwinder::ExecuteParseXml( std::string& InResponseData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"Start ExecuteParseXml");

		CUtility::replaceString(InResponseData,"\"", "\\\"");		// XML内のダブルコートはエスケープする必要がある
		CUtility::replaceString(InResponseData,"\"/>", "\" />");	// 何故かダブルコートと１行でノード終了の/>の間にブランクがないとXMLロードで例外が発生する

		std::string applicationName = "UWandRW_Parse_Xml.exe";
		std::string command = "\"" + InResponseData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Parse_Xml.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string PickupData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"End ExecuteParseXml");

		return PickupData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ExecuteParseXml Startup UWandRW_Parse_Xml.exe");
		return std::string( ErrorLabel) + " Startup UWandRW_Parse_Xml.exe";
	}
}

/**
	* @brief  UWandRW_Compose_Xml.exeが正常にXJMFデータを出力したかチェックする
	* @param  InData        : [i]UWandRW_Compose_Xml.exeの結果データ
	* @param  OutResultData : [o]UWandRW_Compose_Xml.exeの結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckXjmfData( const std::string& InData, std::string& OutResultData )
{
	if ( InData.find(ErrorLabel) == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		OutResultData = InData;
		return FALSE;	// 異常
	}

	return TRUE;	// 正常
}

/**
	* @brief  UWandRW_Sender.exeが正常にレスポンスデータを出力したかチェックする
	* @param  InData        : [i]UWandRW_Sender.exeの結果データ
	* @param  OutResultData : [o]レスポンスのXJMFデータ(正常の場合のみ)、UWandRW_Sender.exeの結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckResponseData( const std::string& InData, std::string& OutResultData )
{
	if ( InData.find(ErrorLabel) == std::string::npos )
	{	
		size_t pos = InData.find("<");
		if ( std::string::npos != pos )
		{	// XJMFデータのみ出力データに設定
			OutResultData = InData.substr(pos);
			return TRUE;	// 正常
		}
		else
		{
			OutResultData = InData;
			return FALSE;
		}
	}
	else
	{
		OutResultData = InData;
		return FALSE;	// 異常
	}
}

/**
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  InData        : [i]UWandRW_Parse_Xml.exeの結果データ
	* @param  OutResultData : [o]UWandRW_Parse_Xml.exeの結果データ(異常の場合のみ)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::CheckPickupData( const std::string& InData, std::string& OutResultData )
{
	if ( InData.find(ErrorLabel) == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		OutResultData = InData;
		return FALSE;	// 異常
	}
}

/**
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  InScrData    : [i]UWandRW_Parse_Xml.exeの結果データ
	* @param  InSelectName : [i]抽出データ名称
	* @retval 抽出データ
**/
std::string CRequestUnwinder::SelectPickupData( const std::string& InScrData, const std::string& InSelectName )
{
	std::vector<std::string> strList = CUtility::splitString(InScrData, ' ');
	for( auto ite = strList.begin(); ite != strList.end(); ite++ )
	{
		if ( ite->compare(0,InSelectName.size(),InSelectName) == 0 )
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
	* @param  InReturnCode : [i]リターンコード
	* @retval [ERROR]ReturnCode=引数のコード
**/
std::string  CRequestUnwinder::ReturnCodeError( const std::string& InReturnCode )
{
	return std::string( ErrorLabel) + "ReturnCode=" + InReturnCode;
}

/**
	* @brief  状態監視用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  InCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::PutStatusCannelID( const std::string& InCannelID )
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.putStatusCannelID(InCannelID);
}

/**
	* @brief  用紙情報通知用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  InCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CRequestUnwinder::PutResourceCannelID( const std::string& InCannelID )
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.putResourceCannelID(InCannelID);
}

};	// namespace ink_code_manager