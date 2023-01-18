/**
	* @file		Ini_UnwinderManager_work.cpp
	* @brief	UnwinderManager_work.ini操作クラス
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
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

BOOL Ini_UnwinderManager_work::putLastSendDate( const std::string& InLastSendDate )
{
	return WriteValueString("SETTING", "LastSendDate", InLastSendDate.c_str());
}

BOOL Ini_UnwinderManager_work::putQueryStatusID( const std::string& InID )
{
	return WriteValueString("SETTING", "QueryStatus", InID.c_str());
}

BOOL Ini_UnwinderManager_work::putQueryResourceID( const std::string& InID )
{
	return WriteValueString("SETTING", "QueryResource", InID.c_str());
}

BOOL Ini_UnwinderManager_work::putStopPersChParamsID( const std::string& InID )
{
	return WriteValueString("SETTING", "StopPersChParams", InID.c_str());
}

BOOL Ini_UnwinderManager_work::putCommandResourceID( const std::string& InID )
{
	return WriteValueString("SETTING", "CommandResource", InID.c_str());
}

BOOL Ini_UnwinderManager_work::putCommandWakeupID( const std::string& InID )
{
	return WriteValueString("SETTING", "CommandWakeup", InID.c_str());
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
