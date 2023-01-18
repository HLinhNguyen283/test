/**
	* @file		Ini_PrinterDescriptor.cpp
	* @brief	PrinterDescriptor.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_PrinterDescriptor.h"
// ==========================================================================

// ==========================================================================
// Local definitions
static const std::string MutexName = _T("Ini_PrinterDescriptor");
static const std::string FileName =  _T("PrinterDescriptor.ini");
// ==========================================================================

// constructor
Ini_PrinterDescriptor::Ini_PrinterDescriptor()
{
}

// destructor
Ini_PrinterDescriptor::~Ini_PrinterDescriptor()
{
}

UINT Ini_PrinterDescriptor::getUnwinderOption()
{
	return GetValueInt("OPTION", "UW_CONNECT_FUNCTION", 0);
}

std::string Ini_PrinterDescriptor::getDeviceID()
{
	char cBuf[256];
	GetValueString("OPTION", "MODEL_TYPE ", "HD1", (LPTSTR)cBuf, sizeof(cBuf));
	std::string deviceID(cBuf);
	deviceID.append("_");
	GetValueString("LOADING", "SERIAL_NUM_SELF ", "0001", (LPTSTR)cBuf, sizeof(cBuf));
	deviceID.append(cBuf);

	return deviceID;
}

// Mutex名を取得する
const char* Ini_PrinterDescriptor::GetMutexName()
{
	return MutexName.c_str();
}

// iniファイルパスを設定する
void Ini_PrinterDescriptor::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}

