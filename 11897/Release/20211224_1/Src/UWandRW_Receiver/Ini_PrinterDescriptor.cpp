/**
	* @file		Ini_PrinterDescriptor.cpp
	* @brief	PrinterDescriptor.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
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

/**
	* @brief  Ini_PrinterDescriptor コンストラクタ
**/
Ini_PrinterDescriptor::Ini_PrinterDescriptor()
{
}

/**
	* @brief  Ini_PrinterDescriptor デストラクタ
**/
Ini_PrinterDescriptor::~Ini_PrinterDescriptor()
{
}

/**
	* @brief  「UW連携機能有/無情報」取得
	* @retval TRUE：機能あり、FALSE:機能なし
**/
UINT Ini_PrinterDescriptor::getUnwinderOption()
{
	return GetValueInt("OPTION", "UW_CONNECT_FUNCTION", 0);
}

/**
	* @brief  「UWへ通知するデバイスID」取得
**/
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

/**
	* @brief  「自機のIPアドレス」取得
**/
std::string Ini_PrinterDescriptor::getMyIpAddress()
{
	char cBuf[256];
	GetValueString("CLIENT_PC_SELF", "IP_ADDR", "192.168.0.40", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  Mutex名を取得する
	* @retval Mutex名
**/
const char* Ini_PrinterDescriptor::GetMutexName()
{
	return MutexName.c_str();
}

/**
	* @brief  iniファイルパスを設定する
	* @param  bSelf : 自機かどうか
**/
void Ini_PrinterDescriptor::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
}

