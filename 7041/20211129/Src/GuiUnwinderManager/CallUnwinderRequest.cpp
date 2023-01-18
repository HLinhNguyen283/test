/**
	* @file		CallUnwinderRequest.cpp
	* @brief	Unwinder要求窓口サンプル
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "RequestUnwinder.h"
#include "MakeComposeUnwinderData.h"
#include "Ini_PrinterDescriptor.h"
#include "CallUnwinderRequest.h"
// ==========================================================================

namespace unwinder_manager
{

/**
	* @brief  Unwionder処理の有効/無効を取得する
	* @retval TRUE：Unwionder処理有効
	* @retval FALSE：Unwionder処理無効
**/
BOOL CCallUnwinderRequest::IsUnwinderFunction()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	return iniPrinterDescriptor.getUnwinderOption();
}

/**
	* @brief  Unwionder処理無効の場合の処理
	* @retval 無効の文字列(仮)
**/
std::string CCallUnwinderRequest::NotSupportedUnwinderFunction()
{
	// 仮作成(未サポートの場合にログを出力する等の場合はこの部分で行う)
	return "Not supported unwinder function.";
}

/**
	* @brief  UWに状態監視用チャネル登録を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallQueryStatus()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		// UWに状態監視用チャネル登録を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestQueryStatus();
		if ( Result.find(SuccessLabel) == std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CallQueryStatus] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CallQueryStatus] ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CallQueryStatus] Exception");
		return "Exception CallQueryStatus";
	}
}

/**
	* @brief  UWに用紙情報通知用チャネル登録を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallQueryResource()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		// UWに用紙情報通知用チャネル登録を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestQueryResource();
		if ( Result.find(SuccessLabel) == std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CallQueryResource] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CallQueryResource] ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CallQueryResource] Exception");
		return "Exception CallQueryResource";
	}
}

/**
	* @brief  UWにチャネル削除を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallStopPersChParams()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		// 削除するチャネル種別を引数にしてUWにチャネル削除を要求する
		// ・状態監視用チャネルを削除する場合は、CMakeComposeUnwinderData::E_Compose_QueryStatus
		// ・用紙情報通知用チャネルを削除する場合は、CMakeComposeUnwinderData::E_Compose_QueryResource
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestStopPersChParams(CMakeComposeUnwinderData::E_Compose_QueryStatus);		// 例：状態監視用チャネルの場合
		if ( Result.find(SuccessLabel) == std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CallStopPersChParams] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CallStopPersChParams] ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CallStopPersChParams] Exception");
		return "Exception CallStopPersChParams";
	}
}

/**
	* @brief  UWに印刷条件設定を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallCommandResource()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		// 何らかの方法で印刷条件を取得する
		CUnwinderPaper sUnwinderPaper;
		sUnwinderPaper.DescriptiveName = "Next IJ #1";
		sUnwinderPaper.MediaType = "NextIJ";
		sUnwinderPaper.Dimension = "520.00 305.00";
		sUnwinderPaper.MaxRunSpeed = "120";
		sUnwinderPaper.Thickness = "130";
		sUnwinderPaper.UWTension = "5";
		sUnwinderPaper.ExternalID = "Ex001";

		// 印刷条件を引数にしてUWに印刷条件設定を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestCommandResource(&sUnwinderPaper);
		if ( Result.find(SuccessLabel) == std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CallCommandResource] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CallCommandResource] ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CallCommandResource] Exception");
		return "Exception CallCommandResource";
	}
}

/**
	* @brief  UWにREADY状態への遷移を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallCommandWakeup()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		// UWにREADY状態への遷移を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestCommandWakeup();
		if ( Result.find(SuccessLabel) == std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CallCommandWakeup] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CallCommandWakeup] ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CallCommandWakeup] Exception");
		return "Exception CallCommandWakeup";
	}
}

/**
	* @brief  UWからSignalStatusを受信するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallSignalStatus()
{
	Sleep(1000);	// 仮
	return "";
}

};	// namespace ink_code_manager