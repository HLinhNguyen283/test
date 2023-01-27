/**
	* @file		GetIniFileParam.cpp
	* @brief	INIファイル値取得 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "GetIniFileParam.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// TP-UW_Communication.iniのファイルパス取得
CString CGetIniFileParam::GetUnwinderIniFilePath()
{
	return CCommon::GetModuleDirectory() + "\\Preferences\\TP-UW_Communication.ini";
}

// PrinterDescriptor.iniのファイルパス取得
CString CGetIniFileParam::GetPrinterDescriptorIniFilePath()
{
	return CCommon::GetModuleDirectory() + "\\Preferences\\PrinterDescriptor.ini";
}

// Unwindwe機能の有効/無効
BOOL CGetIniFileParam::GetUnwinderOption()
{
	return GetPrivateProfileInt("OPTION", "UW_CONNECT_FUNCTION", 0, GetPrinterDescriptorIniFilePath());
}

// UWからのSignalStatusを受信するポート
UINT CGetIniFileParam::GetRecvPortNumber()
{
	return GetPrivateProfileInt("CONNECTION", "RESPONSE_PORT", 50102, GetUnwinderIniFilePath());
}

// UWからのSignalStatusを受信するIPアドレス
CString CGetIniFileParam::GetRecvIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "TP_IPADDRESS", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Status受信フォルダ名
CString CGetIniFileParam::GetRecvStatusFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "Status_Folder", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Paper受信フォルダ名
CString CGetIniFileParam::GetRecvPaperFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "Resource_Folder", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Status受信URL名
CString CGetIniFileParam::GetRecvStatusURL()
{
	CString URL;
	URL.Format("http://%s:%d/", GetRecvIpAddress(), GetRecvPortNumber());

	if ( !GetRecvStatusFolder().IsEmpty() )
	{
		URL.Append(GetRecvStatusFolder());
		URL.Append("/");
	}
	return URL;
}

// Paper受信URL名
CString CGetIniFileParam::GetRecvPaperURL()
{
	CString URL;
	URL.Format("http://%s:%d/", GetRecvIpAddress(), GetRecvPortNumber());

	if ( !GetRecvPaperFolder().IsEmpty() )
	{
		URL.Append(GetRecvPaperFolder());
		URL.Append("/");
	}
	return URL;
}

// ログ保存期間
BOOL CGetIniFileParam::GetLogRetentionPeriod()
{
	return GetPrivateProfileInt("PERIOD", "DAY", 30, GetUnwinderIniFilePath());
}

// ログレベル
UINT CGetIniFileParam::GetDebugLogLevel()
{
	return GetPrivateProfileInt("DEBUGLOG", "LEVEL", 1, GetUnwinderIniFilePath());
}
