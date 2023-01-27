/**
	* @file		Ini_UwRwManager.cpp
	* @brief	TP-UWRW_Communication.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_UwRwManager.h"
// ==========================================================================

// ==========================================================================
// Local definitions

//#define DEF_UW	"UW"
//#define DEF_RW	"RW"

static const std::string MutexName = _T("TP-UWRW_Communication");
static const std::string FileName =  _T("TP-UWRW_Communication.ini");
// ==========================================================================

/**
	* @brief  CIni_UwRwManager_work コンストラクタ
**/
CIni_UwRwManager::CIni_UwRwManager()
{
}

/**
	* @brief  CIni_UwRwManager デストラクタ
**/
CIni_UwRwManager::~CIni_UwRwManager()
{
}

/**
	* @brief  「コントローラの送信ポート番号(UW/RWの受信ポート番号)」取得
**/
std::string CIni_UwRwManager::getSender_Port(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CONNECTION_").append(inDeviceName).c_str(), "SENDER_PORT", (strcmp(inDeviceName, DEF_UW)? "50211": "50131"), (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「コントローラの受信ポート番号(UW/RWの送信ポート番号)」取得
**/
std::string CIni_UwRwManager::getReceiver_Port(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CONNECTION_").append(inDeviceName).c_str(), "RECEIVER_PORT", (strcmp(inDeviceName, DEF_UW)? "50212": "50102"), (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「UWのコントローラ要求受信IPアドレス」取得
**/
std::string CIni_UwRwManager::getUwRw_IpAddress(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CONNECTION_").append(inDeviceName).c_str(), std::string(inDeviceName).append("_IPADDRESS").c_str(), (strcmp(inDeviceName, DEF_UW)? "192.168.0.41": "192.168.0.40"), (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「状態監視受信フォルダ」取得
**/
std::string CIni_UwRwManager::getStatus_Folder(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CONNECTION_").append(inDeviceName).c_str(), "Status_Folder", "/xjmfurl", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙測定受信フォルダ」取得
**/
std::string CIni_UwRwManager::getResource_Folder(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CONNECTION_").append(inDeviceName).c_str(), "Resource_Folder", "/xjmfurl", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「SignalStatusが通知されていないと判断する時間（秒）」取得
**/
UINT CIni_UwRwManager::getSignalstatus_Timeout_Judegment_Waittime(const char* inDeviceName)
{
	return GetValueInt(std::string("CONNECTION_").append(inDeviceName).c_str(), "SIGNALSTATUS_TIMEOUT_JUDEGMENT_WAITTIME", 10);
}

/**
	* @brief  「UW通知エラー発生時のリトライ回数」取得
**/
UINT CIni_UwRwManager::getSend_Retry_Count(const char* inDeviceName)
{
	return GetValueInt(std::string("CONNECTION_").append(inDeviceName).c_str(), "SEND_RETRY_COUNT", 3);
}

/**
	* @brief  「UW通知エラー発生時のリトライ通知の間隔（ミリ秒）」取得
**/
UINT CIni_UwRwManager::getSend_Retry_Interval(const char* inDeviceName)
{
	return GetValueInt(std::string("CONNECTION_").append(inDeviceName).c_str(), "SEND_RETRY_INTERVAL", 1000);
}

/**
	* @brief  「状態監視間隔」取得
**/
std::string CIni_UwRwManager::getQueryStatus_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Status_RepeatTime", "30", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙情報通知間隔」取得
**/
std::string CIni_UwRwManager::getQueryResource_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_RepeatTime", "10", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するChannelMode」取得
**/
std::string CIni_UwRwManager::getQueryResource_ChannelMode()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ChannelMode", "FireAndForget", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するResourceDetails」取得
**/
std::string CIni_UwRwManager::getQueryResource_ResourceDetails()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceDetails", "Full", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するResourceName」取得
**/
std::string CIni_UwRwManager::getQueryResource_ResourceName()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceName", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するScope」取得
**/
std::string CIni_UwRwManager::getQueryResource_Scope()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_Scope", "Present", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「StopPersChParamsで使用するMessageType」取得
**/
std::string CIni_UwRwManager::getStopPersChParams_MessageType()
{
	char cBuf[256];
	GetValueString("CHANNEL", "StopPersCh_MessageType", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「コントローラ-UW間の通信ログの保管期限」取得
**/
UINT CIni_UwRwManager::getPeriod_day()
{
	return GetValueInt("PERIOD", "DAY", 30);
}

/**
	* @brief  「CommandResourceで使用するUpdateMethod」取得
**/
std::string CIni_UwRwManager::getCommandResource_UpdateMethod()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_UpdateMethod", "CompleteSet", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandResourceで使用するName」取得
**/
std::string CIni_UwRwManager::getCommandResource_Name()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_Name", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
 * @brief Get the [CommandResource] ExternalID value
 * @return  value as string
**/
std::string CIni_UwRwManager::getCommandResource_ExternalID()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_ExternalID", "ID_1", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryStatusで使用するIDの接頭文字」取得
**/
std::string CIni_UwRwManager::getPrefix_QueryStatus()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryStatus", "QS", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するIDの接頭文字」取得
**/
std::string CIni_UwRwManager::getPrefix_QueryResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryResource", "QR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「StopPersChParamsで使用するIDの接頭文字」取得
**/
std::string CIni_UwRwManager::getPrefix_StopPersChParams()
{
	char cBuf[256];
	GetValueString("PREFIX", "StopPersChParams", "SP", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandResourceで使用するIDの接頭文字」取得
**/
std::string CIni_UwRwManager::getPrefix_CommandResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandResource", "CR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandWakeupで使用するIDの接頭文字」取得
**/
std::string CIni_UwRwManager::getPrefix_CommandWakeup()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandWakeup", "CW", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  Mutex名を取得する
	* @retval Mutex名
**/
const char* CIni_UwRwManager::GetMutexName()
{
	return MutexName.c_str();
}

/**
	* @brief  iniファイルパスを設定する
	* @param  bSelf : 自機かどうか
**/
void CIni_UwRwManager::SetFilePath(BOOL bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}