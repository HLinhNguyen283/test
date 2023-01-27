/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml起動パラメータ作成
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <map>
#include "XmlLogData.h"
#include "MakeComposeUwRwData.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CMakeComposeUwRwDataコンストラクタ
**/
CMakeComposeUwRwData::CMakeComposeUwRwData()
{
}

/**
	* @brief  CMakeComposeUwRwDataデストラクタ
**/
CMakeComposeUwRwData::~CMakeComposeUwRwData()
{
}

/**
 * @brief Init data
 */
void CMakeComposeUwRwData::initialize()
{
	m_IniPrinterDescriptor.Initialize(TRUE);
	m_IniUwRwManager.Initialize(TRUE);
	m_IniUwRwManager_work.Initialize(TRUE);
}

/**
 * @brief clean up data
 */
void CMakeComposeUwRwData::finalize()
{
	m_IniPrinterDescriptor.Finalize();
	m_IniUwRwManager.Finalize();
	m_IniUwRwManager_work.Finalize();
}

/**
	* @brief  状態監視用チャネル登録（QueryStatus）の送信データを作成する
	* @retval XML送信データMAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterQueryStatus(const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryStatus);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryStatus, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryStatus);
		NameValueTable["URL"]				= GetURL(E_Compose_QueryStatus, inDeviceName);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  用紙情報通知用チャネル登録（QueryResource）の送信データを作成する
	* @retval XML送信データMAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterQueryResource(const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryResource, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryResource);
		NameValueTable["ChannelMode"]		= GetChannelMode();
		NameValueTable["URL"]				= GetURL(E_Compose_QueryResource, inDeviceName);
		NameValueTable["ResourceDetails"]	= GetResourceDetails();
		NameValueTable["ResourceName"]		= GetResourceName();
		NameValueTable["Scope"]				= GetScope();

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  チャネル削除（StopPersChParams）の送信データを作成する
	* @param  inType : [i]削除チャネル種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval XML送信データMAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterStopPersChParams(int inType, const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_StopPersChParams);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_StopPersChParams, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["ChannelID"]			= GetChannelID(inType, inDeviceName);
		NameValueTable["MessageType"]		= GetMessageType();
		NameValueTable["URL"]				= GetURL(inType, inDeviceName);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  印刷条件設定（CommandResource）の送信データを作成する
	* @param  inUnwinderPaper : [i]印刷条件情報
	* @retval XML送信データMAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterCommandResource(const CUwRwPaper& inUnwinderPaper, const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_CommandResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandResource, inDeviceName);
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
	
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  現在時刻をゾーン形式で取得する
	* @retval ゾーン形式の現在時刻
**/
std::string CMakeComposeUwRwData::GetTimeStamp()
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
std::string CMakeComposeUwRwData::GetDeviceID()
{
	std::string deviceID = m_IniPrinterDescriptor.GetPrinterModelType(true, NULL);
	deviceID.append("_");
	char cTemp[10];
	_snprintf_s(cTemp, _TRUNCATE, "%0.4d", m_IniPrinterDescriptor.GetSerialNum(true));
	deviceID.append(cTemp);
	return deviceID;
};

/**
	* @brief  XJMFの種別文字列を取得する
	* @param  inType : [i]XJMFの種別
	* @retval XJMFの種別文字列
**/
std::string CMakeComposeUwRwData::GetType(int inType)
{
	switch(inType) {
	case E_Compose_QueryStatus: 	return "QueryStatus";
	case E_Compose_QueryResource:	return "QueryResource";
	case E_Compose_StopPersChParams:return "StopPersChParams";
	case E_Compose_CommandResource:	return "CommandResource";
	}
	return "";		// あり得ない
}

/**
	* @brief  XJMFに設定するリピートタイムを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval リピートタイム
**/
std::string CMakeComposeUwRwData::GetRepeatTime(int inType)
{

	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用チャネル登録
		return m_IniUwRwManager.getQueryStatus_RepeatTime();
	}
	else
	{	// 用紙情報通知用チャネル登録
		return m_IniUwRwManager.getQueryResource_RepeatTime();
	}
}

/**
	* @brief  XJMFに設定するURLを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval URL
**/
std::string CMakeComposeUwRwData::GetURL(int inType, const char* inDeviceName)
{
	std::string IpAddress  = m_IniPrinterDescriptor.GetClientPcIPAddr(true);

	std::string ReceiverPort = m_IniUwRwManager.getReceiver_Port(inDeviceName);
	std::string FolderName;
	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用チャネル登録
		FolderName = m_IniUwRwManager.getStatus_Folder(inDeviceName);
	}
	else
	{	// 用紙情報通知用チャネル登録
		FolderName = m_IniUwRwManager.getResource_Folder(inDeviceName);
	}

	std::string URL = "http://" + IpAddress + ":" + ReceiverPort;
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
std::string CMakeComposeUwRwData::GetChannelMode()
{
	return m_IniUwRwManager.getQueryResource_ChannelMode();
}

/**
	* @brief  XJMFに設定するResourceDetailsを取得する
	* @retval ResourceDetails
**/
std::string CMakeComposeUwRwData::GetResourceDetails()
{
	return m_IniUwRwManager.getQueryResource_ResourceDetails();
}

/**
	* @brief  XJMFに設定するResourceNameを取得する
	* @retval ResourceName
**/
std::string CMakeComposeUwRwData::GetResourceName()
{
	return m_IniUwRwManager.getQueryResource_ResourceName();
}

/**
	* @brief  XJMFに設定するScopeを取得する
	* @retval Scope
**/
std::string CMakeComposeUwRwData::GetScope()
{
	return m_IniUwRwManager.getQueryResource_Scope();
}

/**
	* @brief  XJMFに設定するチャネルＩＤを取得する
	* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval チャネルＩＤ
**/
std::string CMakeComposeUwRwData::GetChannelID(int inType, const char* inDeviceName)
{

	std::string CannelID;
	if ( inType == E_Compose_QueryStatus )
	{	// 状態監視用のチャネルＩＤ
		CannelID = m_IniUwRwManager_work.getQueryStatus_CannelID(inDeviceName);
	}
	else
	{	// 用紙情報通知用のチャネルＩＤ
		CannelID = m_IniUwRwManager_work.getQueryResource_CannelID(inDeviceName);
	}
	return CannelID;
}

/**
	* @brief  XJMFに設定するMessageTypeを取得する
	* @retval MessageType
**/
std::string CMakeComposeUwRwData::GetMessageType()
{
	return m_IniUwRwManager.getStopPersChParams_MessageType();
}

/**
	* @brief  XJMFに設定するUpdateMethodを取得する
	* @retval UpdateMethod
**/
std::string CMakeComposeUwRwData::GetUpdateMethod()
{
	return m_IniUwRwManager.getCommandResource_UpdateMethod();
}

/**
	* @brief  XJMFに設定するNameを取得する
	* @retval Name
**/
std::string CMakeComposeUwRwData::GetName()
{
	return m_IniUwRwManager.getCommandResource_Name();
}

/**
	* @brief  XJMFに設定するIDを取得する
	*         ＩＤは、接頭文字_日付(YYYYMMDD)_日単位・JMF単位の連番(1～n)
	* @param  inType : [i]JMF種別
	* @retval ID
**/
std::string CMakeComposeUwRwData::GetID(int inType, const char* inDeviceName)
{
	std::string NowDate = CXmlLogData::GetStrDate();		// 現在日を取得
	UINT nSendID = 1;

	if ( GetLastSendData(inDeviceName) == NowDate )
	{	// 日付の変化なし
		nSendID = GetSendID(inType, inDeviceName);	// 送信ID番号取得
		nSendID++;						// IDをインクリメント
		SetSendID(inType, nSendID, inDeviceName);		// 送信ID番号更新
	}
	else
	{	// 日付の変化あり
		PutLastSendData(NowDate, inDeviceName);		// 本日の日付を設定
		ResetSendID(inDeviceName);					// 全てのSendIDをリセット
		nSendID = 1;					// 送信IDは1から
		SetSendID(inType, nSendID, inDeviceName);		// 送信ID番号更新
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
UINT CMakeComposeUwRwData::GetSendID(int inType, const char* inDeviceName)
{
	UINT nID;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nID = GetQueryStatusID(inDeviceName);
		break;
	case E_Compose_QueryResource:
		nID = GetQueryResourceID(inDeviceName);
		break;
	case E_Compose_StopPersChParams:
		nID = GetStopPersChParamsID(inDeviceName);
		break;
	case E_Compose_CommandResource:
		nID = GetCommandResourceID(inDeviceName);
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
BOOL CMakeComposeUwRwData::SetSendID(int inType, UINT inSendID, const char* inDeviceName)
{
	char buf[64];
	_snprintf_s(buf, sizeof(buf) - 1, "%d", inSendID);
	const std::string strSendID(buf);

	BOOL nRet = FALSE;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nRet = PutQueryStatusID(strSendID, inDeviceName);
		break;
	case E_Compose_QueryResource:
		nRet = PutQueryResourceID(strSendID, inDeviceName);
		break;
	case E_Compose_StopPersChParams:
		nRet = PutStopPersChParamsID(strSendID, inDeviceName);
		break;
	case E_Compose_CommandResource:
		nRet = PutCommandResourceID(strSendID, inDeviceName);
		break;
	}
	return nRet;
}

/**
	* @brief  JMF種別毎の送信IDをリセットする
**/
void CMakeComposeUwRwData::ResetSendID(const char* inDeviceName)
{
	PutQueryStatusID("0", inDeviceName);
	PutQueryResourceID("0", inDeviceName);
	PutStopPersChParamsID("0", inDeviceName);
	PutCommandResourceID("0", inDeviceName);
}

/**
	* @brief  JMF種別毎のIDの接頭文字を取得する
	* @param  inType : [i]JMF種別
	* @retval 接頭文字
**/
std::string CMakeComposeUwRwData::GetPrefix(int inType)
{

	std::string Prefix;
	switch(inType) {
	case E_Compose_QueryStatus: 
		Prefix = m_IniUwRwManager.getPrefix_QueryStatus();
		break;
	case E_Compose_QueryResource:
		Prefix = m_IniUwRwManager.getPrefix_QueryResource();
		break;
	case E_Compose_StopPersChParams:
		Prefix = m_IniUwRwManager.getPrefix_StopPersChParams();
		break;
	case E_Compose_CommandResource:
		Prefix = m_IniUwRwManager.getPrefix_CommandResource();
		break;
	}
	return Prefix;
}

/**
	* @brief  前回JMF要求した日付を取得する
	* @retval 日付
**/
std::string CMakeComposeUwRwData::GetLastSendData(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getLastSendDate(inDeviceName);
}

/**
	* @brief  状態監視用チャネル登録の送信IDを取得する
	* @retval 状態監視用チャネル登録の送信ID
**/
UINT CMakeComposeUwRwData::GetQueryStatusID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getQueryStatusID(inDeviceName);
}

/**
	* @brief  用紙情報通知用チャネル登録の送信IDを取得する
	* @retval 用紙情報通知用チャネル登録の送信ID
**/
UINT CMakeComposeUwRwData::GetQueryResourceID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getQueryResourceID(inDeviceName);
}

/**
	* @brief  チャネル削除の送信IDを取得する
	* @retval チャネル削除の送信ID
**/
UINT CMakeComposeUwRwData::GetStopPersChParamsID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getStopPersChParamsID(inDeviceName);
}

/**
	* @brief  印刷条件設定の送信IDを取得する
	* @retval 印刷条件設定の送信ID
**/
UINT CMakeComposeUwRwData::GetCommandResourceID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getCommandResourceID(inDeviceName);
}

/**
	* @brief  JMF要求した日付を更新する
	* @param  inDate : [i]日付(YYYYMMDD)
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutLastSendData(const std::string& inDate, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putLastSendDate(inDate, inDeviceName);
}

/**
	* @brief  状態監視用チャネル登録の送信IDを更新する
	* @param  inID : 状態監視用チャネル登録の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutQueryStatusID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putQueryStatusID(inID, inDeviceName);
}

/**
	* @brief  用紙情報通知用チャネル登録を更新する
	* @param  inID : 用紙情報通知用チャネル登録の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutQueryResourceID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putQueryResourceID(inID, inDeviceName);
}

/**
	* @brief  チャネル削除の送信IDを更新する
	* @param  inID : チャネル削除の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutStopPersChParamsID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putStopPersChParamsID(inID, inDeviceName);
}

/**
	* @brief  印刷条件設定の送信IDを更新する
	* @param  inID : 印刷条件設定の送信ID
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutCommandResourceID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putCommandResourceID(inID, inDeviceName);
}

/**
	* @brief  状態監視用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  inCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutStatusCannelID(const std::string& inCannelID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putStatusCannelID(inCannelID, inDeviceName);
}

/**
	* @brief  用紙情報通知用チャネル登録のチャネルIDをINIファイルに保存する
	* @param  inCannelID : [i]チャネルＩＤ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CMakeComposeUwRwData::PutResourceCannelID(const std::string& inCannelID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putResourceCannelID(inCannelID, inDeviceName);
}

/**
 * @brief Get ExternalID from ini
 * @return value as string
**/
std::string CMakeComposeUwRwData::GetCommandResourceExternalID()
{
	return m_IniUwRwManager.getCommandResource_ExternalID();
}

/**
 * @brief Get SEND_RETRY_COUNT value from ini
 * @return value as uint
**/
UINT CMakeComposeUwRwData::GetSendRetryCount(const char* inDeviceName)
{
	return m_IniUwRwManager.getSend_Retry_Count(inDeviceName);
}

/**
 * @brief Get SEND_RETRY_INTERVAL value from ini
 * @return value as uint
**/
UINT CMakeComposeUwRwData::GetSendRetryInterval(const char* inDeviceName)
{
	return m_IniUwRwManager.getSend_Retry_Interval(inDeviceName);
}

/**
 * Get timeout Timer Status
 * @return timeout in milliseconds
**/
UINT CMakeComposeUwRwData::GetTimeoutTimerStatus(const char* inDeviceName)
{
	return (atoi(m_IniUwRwManager.getQueryStatus_RepeatTime().c_str()) + m_IniUwRwManager.getSignalstatus_Timeout_Judegment_Waittime(inDeviceName)) * 1000;
}

/**
 * Get timeout Timer Paper
 * @return timeout in milliseconds
**/
UINT CMakeComposeUwRwData::GetTimeoutTimerPaper(const char* inDeviceName)
{
	return (atoi(m_IniUwRwManager.getQueryResource_RepeatTime().c_str()) + m_IniUwRwManager.getSignalstatus_Timeout_Judegment_Waittime(inDeviceName)) * 1000;
}

};	// namespace uwrw_manager