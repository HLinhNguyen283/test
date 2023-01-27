/**
 * @file	Ini_PrintConditionImporter.cpp
 * @brief	PrintConditionImporter.ini ファイル操作クラス
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Ini_PrintConditionImporter.h"

// ==========================================================================
// Local definitions
static const char* st_mutexName = _T("CIni_PrintConditionImporter");

// ==========================================================================

CIni_PrintConditionImporter::CIni_PrintConditionImporter()
{
}

CIni_PrintConditionImporter::~CIni_PrintConditionImporter()
{
}

const char* CIni_PrintConditionImporter::GetMutexName()
{
	return st_mutexName;
}

void CIni_PrintConditionImporter::SetFilePath(BOOL /**bSelf**/)
{
	_snprintf(m_filePath, sizeof(m_filePath) - 1, "%s%s%s", GetModuleDirectory(), _T("Preferences\\"), _T("PrintConditionImporter.ini"));
}

void CIni_PrintConditionImporter::GetBasePrintConditionName(std::string& outName)
{
	char baseName[256] = {0};
	GetValueString(_T("GEN"), _T("BASE_NAME"), _T(""), baseName, sizeof(baseName));
	outName = baseName;
}

bool CIni_PrintConditionImporter::IsRegisterJetInspection()
{
	return ((GetValueInt(_T("OPTION"), _T("REGISTER_JET_INSPECTION"), 0) == 0)? FALSE : TRUE);
}

bool CIni_PrintConditionImporter::IsDebugLog()
{
	return ((GetValueInt(_T("DEBUG"), _T("LOG"), 0) == 0)? FALSE : TRUE);
}

DWORD CIni_PrintConditionImporter::GetTimeoutImport()
{
	return GetValueInt(_T("OPTION"), _T("TIMEOUT_IMPORT"), 7200);
}