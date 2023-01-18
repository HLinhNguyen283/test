/**
	* @file		Ini_UnwinderManager.cpp
	* @brief	TP-UW_Communication.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_UnwinderManager.h"
// ==========================================================================

// ==========================================================================
// Local definitions
static const std::string MutexName = _T("Ini_UnwinderManager");
static const std::string FileName =  _T("TP-UW_Communication.ini");
// ==========================================================================

// constructor
Ini_UnwinderManager::Ini_UnwinderManager()
{
}

// destructor
Ini_UnwinderManager::~Ini_UnwinderManager()
{
}

std::string Ini_UnwinderManager::getSender_Port()
{
	char cBuf[256];
	GetValueString("CONNECTION", "SENDER_PORT", "50131", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getReceiver_Port()
{
	char cBuf[256];
	GetValueString("CONNECTION", "RECEIVER_PORT", "50102", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getUW_IpAddress()
{
	char cBuf[256];
	GetValueString("CONNECTION", "UW_IPADDRESS", "192.168.0.40", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getTP_IpAddress()
{
	char cBuf[256];
	GetValueString("CONNECTION", "TP_IPADDRESS", "192.168.0.40", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getStatus_Folder()
{
	char cBuf[256];
	GetValueString("CONNECTION", "Status_Folder", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getResource_Folder()
{
	char cBuf[256];
	GetValueString("CONNECTION", "Resource_Folder", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryStatus_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Status_RepeatTime", "30", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryResource_RepeatTime()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_RepeatTime", "10", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryResource_ChannelMode()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ChannelMode", "FireAndForget", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryResource_ResourceDetails()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceDetails", "Full", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryResource_ResourceName()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_ResourceName", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getQueryResource_Scope()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_Scope", "Present", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getStopPersChParams_MessageType()
{
	char cBuf[256];
	GetValueString("CHANNEL", "StopPersCh_MessageType", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}


std::string Ini_UnwinderManager::getCommandResource_UpdateMethod()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_UpdateMethod", "CompleteSet", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getCommandResource_Name()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_Name", "Media", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getCommandResource_ExternalID()
{
	char cBuf[256];
	GetValueString("COMMAND", "Resource_ExternalID", "ID_1", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getPrefix_QueryStatus()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryStatus", "QS", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getPrefix_QueryResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "QueryResource", "QR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getPrefix_StopPersChParams()
{
	char cBuf[256];
	GetValueString("PREFIX", "StopPersChParams", "SP", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getPrefix_CommandResource()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandResource", "CR", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager::getPrefix_CommandWakeup()
{
	char cBuf[256];
	GetValueString("PREFIX", "CommandWakeup", "CW", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

// Mutex名を取得する
const char* Ini_UnwinderManager::GetMutexName()
{
	return MutexName.c_str();
}

// iniファイルパスを設定する
void Ini_UnwinderManager::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}