/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml起動パラメータ作成
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <map>
#include "Utility.h"
#include "Ini_UnwinderManager.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_UnwinderManager_work.h"
#include "MakeComposeUnwinderData.h"
// ===========================================================================

namespace unwinder_manager
{

CMakeComposeUnwinderData::CMakeComposeUnwinderData()
{
}
CMakeComposeUnwinderData::~CMakeComposeUnwinderData()
{
}

/**
	* @brief  状態監視用チャネル登録（QueryStatus）の送信データを作成する
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::ParameterQueryStatus()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] Start");
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable.clear();
		NameValueTable["Type"]				= GetType(E_Compose_QueryStatus);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryStatus);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryStatus);
		NameValueTable["URL"]				= GetURL(E_Compose_QueryStatus);

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] Exception");
		return "";
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] End");
}

/**
	* @brief  用紙情報通知用チャネル登録（QueryResource）の送信データを作成する
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::ParameterQueryResource()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] Start");
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable.clear();
		NameValueTable["Type"]				= GetType(E_Compose_QueryResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryResource);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryResource);
		NameValueTable["ChannelMode"]		= GetChannelMode();
		NameValueTable["URL"]				= GetURL(E_Compose_QueryResource);
		NameValueTable["ResourceDetails"]	= GetResourceDetails();
		NameValueTable["ResourceName"]		= GetResourceName();
		NameValueTable["Scope"]				= GetScope();

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] Exception");
		return "";
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] End");
}

/**
	* @brief  チャネル削除（StopPersChParams）の送信データを作成する
	* @param  inType : [i]削除チャネル種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::ParameterStopPersChParams(int inType)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] Start");
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable.clear();
		NameValueTable["Type"]				= GetType(E_Compose_StopPersChParams);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_StopPersChParams);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["ChannelID"]			= GetChannelID(inType);
		NameValueTable["MessageType"]		= GetMessageType();
		NameValueTable["URL"]				= GetURL(inType);

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] Exception");
		return "";
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] End");
}

/**
	* @brief  印刷条件設定（CommandResource）の送信データを作成する
	* @param  inUnwinderPaper : [i]印刷条件情報
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::ParameterCommandResource(const CUnwinderPaper& inUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] Start");
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable.clear();
		NameValueTable["Type"]				= GetType(E_Compose_CommandResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandResource);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["UpdateMethod"]		= GetUpdateMethod();
		NameValueTable["Name"]				= GetName();
		NameValueTable["DescriptiveName"]	= inUnwinderPaper.DescriptiveName;
		NameValueTable["ExternalID"]		= inUnwinderPaper.ExternalID;
		NameValueTable["Dimension"]			= inUnwinderPaper.Dimension;
		NameValueTable["MediaType"]			= inUnwinderPaper.MediaType;
		NameValueTable["Thickness"]			= inUnwinderPaper.Thickness;
		NameValueTable["UWTension"]			= inUnwinderPaper.UWTension;
		NameValueTable["MaxRunSpeed"]		= inUnwinderPaper.MaxRunSpeed;
	
		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] Exception");
		return "";
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] End");
}

/**
	* @brief  READY状態への遷移（CommandWakeup）の送信データを作成する
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::ParameterCommandWakeup()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] Start");
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable.clear();
		NameValueTable["Type"]				= GetType(E_Compose_CommandWakeup);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandWakeup);
		NameValueTable["Time"]				= GetTimeStamp();

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] Exception");
		return "";
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] End");
}

/**
	* @brief  名称MAPよりXML送信データを作成する
	* @param  inNameValueTable : [i]名称と値をペアとしたテーブル
	* @retval XML送信データ（項目名１=値１△項目名２=値２..）
**/
std::string CMakeComposeUnwinderData::MakeString(const std::map<std::string,std::string>& inNameValueTable)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::MakeString] Start");
	std::string strData;
	bool bFst = true;
	for( auto ite = inNameValueTable.begin(); ite != inNameValueTable.end(); ite++ )
	{
		if ( !bFst ) strData.append( " " );
		strData.append( ite->first );
		strData.append( "=" );

		std::string s = ite->second;
		CUtility::replaceString(s," ", "@@@");		// ブランクは@@@に変換する
		strData.append( s );
		bFst = false;
	}
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::MakeString] End");
	return strData;
}

/**
	* @brief  現在時刻をゾーン形式で取得する
	* @retval ゾーン形式の現在時刻
**/
std::string CMakeComposeUnwinderData::GetTimeStamp()
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);

	std::string Bias;
	if (tzInfo.Bias > 0 ) Bias = "-";
	else Bias = _T("+");

	char cTemp[512];
	_snprintf_s( cTemp, sizeof(cTemp)-1,"%04d-%02d-%02dT%02d:%02d:%02d%s%02d:%02d",
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond,
			Bias.c_str(), abs(tzInfo.Bias) / 60, abs(tzInfo.Bias) % 60);

	return std::string(cTemp);
}

/**
	* @brief  XJMFに設定するDeviceIDを取得する
	* @retval DeviceID
**/
std::string CMakeComposeUnwinderData::GetDeviceID()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	return iniPrinterDescriptor.getDeviceID();
};

/**
	* @brief  XJMFの種別文字列を取得する
	* @param  inType : [i]XJMFの種別
	* @retval XJMFの種別文字列
**/
std::string CMakeComposeUnwinderData::GetType(int inType)
{
	switch(inType) {
	case E_Compose_QueryStatus: 	return "QueryStatus";
	case E_Compose_QueryResource:	return "QueryResource";
	case E_Compose_StopPersChParams:return "StopPersChParams";
	case E_Compose_CommandResource:	return "CommandResource";
	case E_Compose_CommandWakeup:	return "CommandWakeup";
	}
	return "";		// あり得ない
}

/**
	* @brief  XJMFに設定するリピートタイムを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval リピートタイム
**/
std::string CMakeComposeUnwinderData::GetRepeatTime(int inType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用チャネル登録
		return iniUnwinderManager.getQueryStatus_RepeatTime();
	}
	else
	{	// 用紙情報通知用チャネル登録
		return iniUnwinderManager.getQueryResource_RepeatTime();
	}
}

/**
	* @brief  XJMFに設定するURLを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval URL
**/
std::string CMakeComposeUnwinderData::GetURL(int inType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string IpAddress  = iniUnwinderManager.getUW_IpAddress();
	std::string SenderPort = iniUnwinderManager.getSender_Port();
	std::string FolderName;
	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用チャネル登録
		FolderName = iniUnwinderManager.getStatus_Folder();
	}
	else
	{	// 用紙情報通知用チャネル登録
		FolderName = iniUnwinderManager.getResource_Folder();
	}

	std::string URL = "http://" + IpAddress + ":" + SenderPort;
	if ( !FolderName.empty() )
	{
		URL.append("/");
		URL.append(FolderName);
	}
	return URL;
}

/**
	* @brief  XJMFに設定するChannelModeを取得する
	* @retval ChannelMode
**/
std::string CMakeComposeUnwinderData::GetChannelMode()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ChannelMode();
}

/**
	* @brief  XJMFに設定するResourceDetailsを取得する
	* @retval ResourceDetails
**/
std::string CMakeComposeUnwinderData::GetResourceDetails()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ResourceDetails();
}

/**
	* @brief  XJMFに設定するResourceNameを取得する
	* @retval ResourceName
**/
std::string CMakeComposeUnwinderData::GetResourceName()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ResourceName();
}

/**
	* @brief  XJMFに設定するScopeを取得する
	* @retval Scope
**/
std::string CMakeComposeUnwinderData::GetScope()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_Scope();
}

/**
	* @brief  XJMFに設定するチャネルＩＤを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval チャネルＩＤ
**/
std::string CMakeComposeUnwinderData::GetChannelID(int inType)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);

	std::string CannelID;
	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用のチャネルＩＤ
		CannelID = iniUnwinderManager_work.getQueryStatus_CannelID();
	}
	else
	{	// 用紙情報通知用のチャネルＩＤ
		CannelID = iniUnwinderManager_work.getQueryResource_CannelID();
	}
	return CannelID;
}

/**
	* @brief  XJMFに設定するMessageTypeを取得する
	* @retval MessageType
**/
std::string CMakeComposeUnwinderData::GetMessageType()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getStopPersChParams_MessageType();
}

/**
	* @brief  XJMFに設定するUpdateMethodを取得する
	* @retval UpdateMethod
**/
std::string CMakeComposeUnwinderData::GetUpdateMethod()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getCommandResource_UpdateMethod();
}

/**
	* @brief  XJMFに設定するNameを取得する
	* @retval Name
**/
std::string CMakeComposeUnwinderData::GetName()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getCommandResource_Name();
}

/**
	* @brief  XJMFに設定するIDを取得する
	*         ＩＤは、接頭文字_日付(YYYYMMDD)_日単位・JMF単位の連番(1～n)
	* @param  inType : [i]JMF種別
	* @retval ID
**/
std::string CMakeComposeUnwinderData::GetID(int inType)
{
	std::string NowDate = CUtility::GetStrDate();		// 現在日を取得
	UINT nSendID = 1;

	if ( GetLastSendData() == NowDate )
	{	// 日付の変化なし
		nSendID = GetSendID(inType);	// 送信ID番号取得
		nSendID++;						// IDをインクリメント
		SetSendID(inType, nSendID);		// 送信ID番号更新
	}
	else
	{	// 日付の変化あり
		PutLastSendData(NowDate);		// 本日の日付を設定
		ResetSendID();					// 全てのSendIDをリセット
		nSendID = 1;					// 送信IDは1から
		SetSendID(inType, nSendID);		// 送信ID番号更新
	}

	// ＩＤは、接頭文字_日付(YYYYMMDD)_日単位・JMF単位の連番(1～n)
	char buf[64];
	_snprintf_s( buf, sizeof(buf) - 1, "%s_%s_%05d", GetPrefix(inType).c_str(), NowDate.c_str(), nSendID);

	return std::string(buf);
}

/**
	* @brief  JMF種別毎の前回要求した送信IDを取得する
	* @param  inType : [i]JMF種別
	* @retval 送信ID
**/
UINT CMakeComposeUnwinderData::GetSendID(int inType)
{
	UINT nID;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nID = GetQueryStatusID();
		break;
	case E_Compose_QueryResource:
		nID = GetQueryResourceID();
		break;
	case E_Compose_StopPersChParams:
		nID = GetStopPersChParamsID();
		break;
	case E_Compose_CommandResource:
		nID = GetCommandResourceID();
		break;
	case E_Compose_CommandWakeup:
		nID = GetCommandWakeupID();
		break;
	}
	return nID;
}

/**
	* @brief  JMF種別毎の送信IDを保存する
	* @param  inType   : [i]JMF種別
	* @param  inSendID : [i]送信ＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::SetSendID(int inType, UINT inSendID)
{
	char buf[64];
	_snprintf_s(buf, sizeof(buf) - 1, "%d", inSendID);
	const std::string strSendID(buf);

	BOOL nRet = FALSE;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nRet = PutQueryStatusID(strSendID);
		break;
	case E_Compose_QueryResource:
		nRet = PutQueryResourceID(strSendID);
		break;
	case E_Compose_StopPersChParams:
		nRet = PutStopPersChParamsID(strSendID);
		break;
	case E_Compose_CommandResource:
		nRet = PutCommandResourceID(strSendID);
		break;
	case E_Compose_CommandWakeup:
		nRet = PutCommandWakeupID(strSendID);
		break;
	}
	return nRet;
}

/**
	* @brief  JMF種別毎の送信IDをリセットする
**/
void CMakeComposeUnwinderData::ResetSendID()
{
	PutQueryStatusID(0);
	PutQueryResourceID(0);
	PutStopPersChParamsID(0);
	PutCommandResourceID(0);
	PutCommandWakeupID(0);
}

/**
	* @brief  JMF種別毎のIDの接頭文字を取得する
	* @param  inType : [i]JMF種別
	* @retval 接頭文字
**/
std::string CMakeComposeUnwinderData::GetPrefix(int inType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string Prefix;
	switch(inType) {
	case E_Compose_QueryStatus: 
		Prefix = iniUnwinderManager.getPrefix_QueryStatus();
		break;
	case E_Compose_QueryResource:
		Prefix = iniUnwinderManager.getPrefix_QueryResource();
		break;
	case E_Compose_StopPersChParams:
		Prefix = iniUnwinderManager.getPrefix_StopPersChParams();
		break;
	case E_Compose_CommandResource:
		Prefix = iniUnwinderManager.getPrefix_CommandResource();
		break;
	case E_Compose_CommandWakeup:
		Prefix = iniUnwinderManager.getPrefix_CommandWakeup();
		break;
	}
	return Prefix;
}

/**
	* @brief  前回JMF要求した日付を取得する
	* @retval 日付
**/
std::string CMakeComposeUnwinderData::GetLastSendData()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getLastSendDate();
}

/**
	* @brief  状態監視用チャネル登録の送信IDを取得する
	* @retval 状態監視用チャネル登録の送信ID
**/
UINT CMakeComposeUnwinderData::GetQueryStatusID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getQueryStatusID();
}

/**
	* @brief  用紙情報通知用チャネル登録の送信IDを取得する
	* @retval 用紙情報通知用チャネル登録の送信ID
**/
UINT CMakeComposeUnwinderData::GetQueryResourceID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getQueryResourceID();
}

/**
	* @brief  チャネル削除の送信IDを取得する
	* @retval チャネル削除の送信ID
**/
UINT CMakeComposeUnwinderData::GetStopPersChParamsID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getStopPersChParamsID();
}

/**
	* @brief  印刷条件設定の送信IDを取得する
	* @retval 印刷条件設定の送信ID
**/
UINT CMakeComposeUnwinderData::GetCommandResourceID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getCommandResourceID();
}

/**
	* @brief  READY状態への遷移の送信IDを取得する
	* @retval READY状態への遷移の送信ID
**/
UINT CMakeComposeUnwinderData::GetCommandWakeupID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getCommandWakeupID();
}

/**
	* @brief  JMF要求した日付を更新する
	* @param  inDate : [i]日付(YYYYMMDD)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutLastSendData(const std::string& inDate)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putLastSendDate(inDate);
}

/**
	* @brief  状態監視用チャネル登録の送信IDを更新する
	* @param  inID : 状態監視用チャネル登録の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutQueryStatusID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryStatusID(inID);
}

/**
	* @brief  用紙情報通知用チャネル登録を更新する
	* @param  inID : 用紙情報通知用チャネル登録の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutQueryResourceID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryResourceID(inID);
}

/**
	* @brief  チャネル削除の送信IDを更新する
	* @param  inID : チャネル削除の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutStopPersChParamsID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putStopPersChParamsID(inID);
}

/**
	* @brief  印刷条件設定の送信IDを更新する
	* @param  inID : 印刷条件設定の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutCommandResourceID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandResourceID(inID);
}

/**
	* @brief  READY状態への遷移の送信IDを更新する
	* @param  inID : READY状態への遷移の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUnwinderData::PutCommandWakeupID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandWakeupID(inID);
}

};	// namespace ink_code_manager