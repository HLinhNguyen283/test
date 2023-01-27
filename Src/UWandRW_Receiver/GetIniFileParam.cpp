/**
	* @file		GetIniFileParam.cpp
	* @brief	INI�t�@�C���l�擾 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
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

// TP-UW_Communication.ini�̃t�@�C���p�X�擾
CString CGetIniFileParam::GetUnwinderIniFilePath()
{
	return CCommon::GetModuleDirectory() + "\\Preferences\\TP-UW_Communication.ini";
}

// PrinterDescriptor.ini�̃t�@�C���p�X�擾
CString CGetIniFileParam::GetPrinterDescriptorIniFilePath()
{
	return CCommon::GetModuleDirectory() + "\\Preferences\\PrinterDescriptor.ini";
}

// Unwindwe�@�\�̗L��/����
BOOL CGetIniFileParam::GetUnwinderOption()
{
	return GetPrivateProfileInt("OPTION", "UW_CONNECT_FUNCTION", 0, GetPrinterDescriptorIniFilePath());
}

// UW�����SignalStatus����M����|�[�g
UINT CGetIniFileParam::GetRecvPortNumber()
{
	return GetPrivateProfileInt("CONNECTION", "RESPONSE_PORT", 50102, GetUnwinderIniFilePath());
}

// UW�����SignalStatus����M����IP�A�h���X
CString CGetIniFileParam::GetRecvIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "TP_IPADDRESS", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Status��M�t�H���_��
CString CGetIniFileParam::GetRecvStatusFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "Status_Folder", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Paper��M�t�H���_��
CString CGetIniFileParam::GetRecvPaperFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString("CONNECTION", "Resource_Folder", "", tcWork, sizeof(tcWork), GetUnwinderIniFilePath());
	return CString(tcWork);
}

// Status��MURL��
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

// Paper��MURL��
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

// ���O�ۑ�����
BOOL CGetIniFileParam::GetLogRetentionPeriod()
{
	return GetPrivateProfileInt("PERIOD", "DAY", 30, GetUnwinderIniFilePath());
}

// ���O���x��
UINT CGetIniFileParam::GetDebugLogLevel()
{
	return GetPrivateProfileInt("DEBUGLOG", "LEVEL", 1, GetUnwinderIniFilePath());
}
