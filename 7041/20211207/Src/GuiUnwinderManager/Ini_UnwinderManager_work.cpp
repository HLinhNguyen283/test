/**
	* @file		Ini_UnwinderManager_work.cpp
	* @brief	UnwinderManager_work.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_UnwinderManager_work.h"
// ==========================================================================

// ==========================================================================
// Local definitions
static const std::string MutexName = _T("Ini_UnwinderManager_work");
static const std::string FileName =  _T("UnwinderManager_work.ini");
// ==========================================================================

// constructor
Ini_UnwinderManager_work::Ini_UnwinderManager_work()
{
}

// destructor
Ini_UnwinderManager_work::~Ini_UnwinderManager_work()
{
}

std::string Ini_UnwinderManager_work::getLastSendDate()
{
	char cBuf[256];
	GetValueString("SETTING", "LastSendDate", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

UINT Ini_UnwinderManager_work::getQueryStatusID()
{
	return GetValueInt("SETTING", "QueryStatus", 0);
}

UINT Ini_UnwinderManager_work::getQueryResourceID()
{
	return GetValueInt("SETTING", "QueryResource", 0);
}

UINT Ini_UnwinderManager_work::getStopPersChParamsID()
{
	return GetValueInt("SETTING", "StopPersChParams", 0);
}

UINT Ini_UnwinderManager_work::getCommandResourceID()
{
	return GetValueInt("SETTING", "CommandResource", 0);
}

UINT Ini_UnwinderManager_work::getCommandWakeupID()
{
	return GetValueInt("SETTING", "CommandWakeup", 0);
}

std::string Ini_UnwinderManager_work::getQueryStatus_CannelID()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Status_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

std::string Ini_UnwinderManager_work::getQueryResource_CannelID()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}


BOOL Ini_UnwinderManager_work::putLastSendDate( const std::string& inLastSendDate )
{
	return WriteValueString("SETTING", "LastSendDate", inLastSendDate.c_str());
}

BOOL Ini_UnwinderManager_work::putQueryStatusID( const std::string& inID )
{
	return WriteValueString("SETTING", "QueryStatus", inID.c_str());
}

BOOL Ini_UnwinderManager_work::putQueryResourceID( const std::string& inID )
{
	return WriteValueString("SETTING", "QueryResource", inID.c_str());
}

BOOL Ini_UnwinderManager_work::putStopPersChParamsID( const std::string& inID )
{
	return WriteValueString("SETTING", "StopPersChParams", inID.c_str());
}

BOOL Ini_UnwinderManager_work::putCommandResourceID( const std::string& inID )
{
	return WriteValueString("SETTING", "CommandResource", inID.c_str());
}

BOOL Ini_UnwinderManager_work::putCommandWakeupID( const std::string& inID )
{
	return WriteValueString("SETTING", "CommandWakeup", inID.c_str());
}

BOOL Ini_UnwinderManager_work::putStatusCannelID( const std::string& inCannelID )
{
	return WriteValueString("CHANNEL", "Status_CannelID", inCannelID.c_str());
}

BOOL Ini_UnwinderManager_work::putResourceCannelID( const std::string& inCannelID )
{
	return WriteValueString("CHANNEL", "Resource_CannelID", inCannelID.c_str());
}

BOOL Ini_UnwinderManager_work::putUnwinderStatus( const std::string& inStatus )
{
	return WriteValueString("UnWinder", "Status", inStatus.c_str());
}

// Mutex名を取得する
const char* Ini_UnwinderManager_work::GetMutexName()
{
	return MutexName.c_str();
}

// iniファイルパスを設定する
void Ini_UnwinderManager_work::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}
