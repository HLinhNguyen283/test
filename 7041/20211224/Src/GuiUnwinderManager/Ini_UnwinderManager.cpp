/**
	* @file		Ini_UnwinderManager.cpp
	* @brief	TP-UW_Communication.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_UnwinderManager.h"
// ==========================================================================

// ==========================================================================
// Local definitions
static const std::string MutexName = _T("TP-UW_Communication");
static const std::string FileName =  _T("TP-UW_Communication.ini");
// ==========================================================================

/**
	* @brief  Ini_UnwinderManager_work コンストラクタ
**/
Ini_UnwinderManager::Ini_UnwinderManager()
{
}

/**
	* @brief  Ini_UnwinderManager デストラクタ
**/
Ini_UnwinderManager::~Ini_UnwinderManager()
{
}

/**
	* @brief  「コントローラの送信ポート番号(UWの受信ポート番号)」取得
**/
std::string Ini_UnwinderManager::getSender_Port()
{
	char cBuf[256];
	GetValueString("CONNECTION", "SENDER_PORT", "50131", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「コントローラの受信ポート番号(UWの送信ポート番号)」取得
**/
std::string Ini_UnwinderManager::getReceiver_Port()
{
	char cBuf[256];
	GetValueString("CONNECTION", "RECEIVER_PORT", "50102", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「UWのコントローラ要求受信IPアドレス」取得
**/
std::string Ini_UnwinderManager::getUW_IpAddress()
{
	char cBuf[256];
	GetValueString("CONNECTION", "UW_IPADDRESS", "192.168.0.40", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「状態監視受信フォルダ」取得
**/
std::string Ini_UnwinderManager::getStatus_Folder()
{
	char cBuf[256];
	GetValueString("CONNECTION", "Status_Folder", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙測定受信フォルダ」取得
**/
std::string Ini_UnwinderManager::getResource_Folder()
{
	char cBuf[256];
	GetValueString("CONNECTION", "Resource_Folder", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「SignalStatusが通知されていないと判断する時間（秒）」取得
**/
UINT Ini_UnwinderManager::getSignalstatus_Timeout_Judegment_Waittime()
{
	return GetValueInt("CONNECTION", "SIGNALSTATUS_TIMEOUT_JUDEGMENT_WAITTIME", 10);
}

/**
	* @brief  「UW通知エラー発生時のリトライ回数」取得
**/
UINT Ini_UnwinderManager::getSend_Retry_Count()
{
	return GetValueInt("CONNECTION", "SEND_RETRY_COUNT", 3);
}

/**
	* @brief  「UW通知エラー発生時のリトライ通知の間隔（ミリ秒）」取得
**/
UINT Ini_UnwinderManager::getSend_Retry_Interval()
{
	return GetValueInt("CONNECTION", "SEND_RETRY_INTERVAL", 1000);
}

/**
	* @brief  「状態監視間隔」取得
**/
std::string Ini_UnwinderManager::getQueryStatus_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Status_RepeatTime", "30", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙情報通知間隔」取得
**/
std::string Ini_UnwinderManager::getQueryResource_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_RepeatTime", "10", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するChannelMode」取得
**/
std::string Ini_UnwinderManager::getQueryResource_ChannelMode()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ChannelMode", "FireAndForget", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するResourceDetails」取得
**/
std::string Ini_UnwinderManager::getQueryResource_ResourceDetails()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceDetails", "Full", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するResourceName」取得
**/
std::string Ini_UnwinderManager::getQueryResource_ResourceName()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceName", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するScope」取得
**/
std::string Ini_UnwinderManager::getQueryResource_Scope()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_Scope", "Present", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「StopPersChParamsで使用するMessageType」取得
**/
std::string Ini_UnwinderManager::getStopPersChParams_MessageType()
{
	char cBuf[256];
	GetValueString("CHANNEL", "StopPersCh_MessageType", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「コントローラ-UW間の通信ログの保管期限」取得
**/
UINT Ini_UnwinderManager::getPeriod_day()
{
	return GetValueInt("PERIOD", "DAY", 30);
}

/**
	* @brief  「CommandResourceで使用するUpdateMethod」取得
**/
std::string Ini_UnwinderManager::getCommandResource_UpdateMethod()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_UpdateMethod", "CompleteSet", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandResourceで使用するName」取得
**/
std::string Ini_UnwinderManager::getCommandResource_Name()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_Name", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryStatusで使用するIDの接頭文字」取得
**/
std::string Ini_UnwinderManager::getPrefix_QueryStatus()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryStatus", "QS", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryResourceで使用するIDの接頭文字」取得
**/
std::string Ini_UnwinderManager::getPrefix_QueryResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryResource", "QR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「StopPersChParamsで使用するIDの接頭文字」取得
**/
std::string Ini_UnwinderManager::getPrefix_StopPersChParams()
{
	char cBuf[256];
	GetValueString("PREFIX", "StopPersChParams", "SP", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandResourceで使用するIDの接頭文字」取得
**/
std::string Ini_UnwinderManager::getPrefix_CommandResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandResource", "CR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「CommandWakeupで使用するIDの接頭文字」取得
**/
std::string Ini_UnwinderManager::getPrefix_CommandWakeup()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandWakeup", "CW", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  Mutex名を取得する
	* @retval Mutex名
**/
const char* Ini_UnwinderManager::GetMutexName()
{
	return MutexName.c_str();
}

/**
	* @brief  iniファイルパスを設定する
	* @param  bSelf : 自機かどうか
**/
void Ini_UnwinderManager::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}