/**
 * @file	Ini_PrintConditionExporter.cpp
 * @brief	PrintConditionExporter.ini ファイル操作クラス
 * @author	pt.ju-tanaka
 * @date	2019/02/12 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Ini_PrintConditionExporter.h"

// ==========================================================================
// Local definitions
static const char* st_mutexName = _T("CIni_PrintConditionExporter");

static const std::string SectionDebug = "DEBUG";
static const std::string SectionOption = "OPTION";

static const std::string KeyLog = "LOG";
static const std::string KeyXmlExportLog = "XML_EXPORT_LOG";
static const std::string KeyEQRootFolder = "OUTPUT_ROOT_FOLDER";
static const std::string KeyEQRootFolderXML = "OUTPUT_XML_ROOT_FOLDER";
static const std::string KeyEQRetryInterval = "AUTOUPDATE_PRINTCONDITION_TEXT_RETRY_INTERVAL";
static const std::string KeyEQRetryIntervalXML = "AUTOUPDATE_PRINTCONDITION_XML_RETRY_INTERVAL";
static const std::string KeyEnableAutoupdate = "ENABLE_AUTOUPDATE_PRINTCONDITION_TEXT";
static const std::string KeyEnableAutoupdateXML = "ENABLE_AUTOUPDATE_PRINTCONDITION_XML";

static const std::string KeyEnableAutoUpdateFixIP = "ENABLE_AUTOUPDATE_FIX_IP";
static const std::string KeyEQRootFolderJIData = "OUTPUT_JIDATA_ROOT_FOLDER";

// ==========================================================================

CIni_PrintConditionExporter::CIni_PrintConditionExporter()
{
}

CIni_PrintConditionExporter::~CIni_PrintConditionExporter()
{
}

const char* CIni_PrintConditionExporter::GetMutexName()
{
	return st_mutexName;
}

void CIni_PrintConditionExporter::SetFilePath(BOOL /**bSelf**/)
{
	_snprintf(m_filePath, sizeof(m_filePath) - 1, "%s%s%s", GetModuleDirectory(), _T("Preferences\\"), _T("PrintConditionExporter.ini"));
}

// Log.htmlに出力するかどうか
bool CIni_PrintConditionExporter::IsDebugLog()
{
	return ((GetValueInt(SectionDebug.c_str(), KeyLog.c_str(), 0) == 0)? false : true);
}

// Log.htmlにXMLエクスポート処理のログを出力するかどうか
bool CIni_PrintConditionExporter::IsDebugXmlExportLog()
{
	return ((GetValueInt(SectionDebug.c_str(), KeyXmlExportLog.c_str(), 0) == 0)? false : true);
}

// EQ制御サーバー上にあるテキスト出力先のルートフォルダ
void CIni_PrintConditionExporter::GetOutputRootFolder(std::string& outPath)
{
	char path[_MAX_PATH] = {0};
	GetValueString(SectionOption.c_str(), KeyEQRootFolder.c_str(), _T("#work_ex\\ExportJDPaper_text"), path, sizeof(path));	
	outPath = path;
}

// EQ制御サーバー上にあるXML出力先のルートフォルダ
void CIni_PrintConditionExporter::GetOutputRootFolderXML(std::string& outPath)
{
	char path[_MAX_PATH] = {0};
	GetValueString(SectionOption.c_str(), KeyEQRootFolderXML.c_str(), _T("#work_ex\\ExportJDPaper_xml"), path, sizeof(path));	
	outPath = path;
}

// EQ制御サーバー上にあるJI関連情報出力先のルートフォルダ
void CIni_PrintConditionExporter::GetOutputRootFolderInspectionData(std::string& outPath)
{
	char path[_MAX_PATH] = {0};
	GetValueString(SectionOption.c_str(), KeyEQRootFolderJIData.c_str(), _T("#work_ex\\InspectionData"), path, sizeof(path));	
	outPath = path;
}

// EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔
long CIni_PrintConditionExporter::GetAutoUpdatePrintConditionRetryInterval()
{
	return GetValueInt(SectionOption.c_str(), KeyEQRetryInterval.c_str(), 10);
}

// EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔
long CIni_PrintConditionExporter::GetAutoUpdatePrintConditionRetryIntervalXML()
{
	return GetValueInt(SectionOption.c_str(), KeyEQRetryIntervalXML.c_str(), 10);
}

// EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能の有効状態
bool CIni_PrintConditionExporter::IsEnableAutoUpdatePrintConditionText()
{
	return ((GetValueInt(SectionOption.c_str(), KeyEnableAutoupdate.c_str(), 0) == 0)? false : true);
}

// EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能の有効状態
bool CIni_PrintConditionExporter::IsEnableAutoUpdatePrintConditionXML()
{
	return ((GetValueInt(SectionOption.c_str(), KeyEnableAutoupdateXML.c_str(), 0) == 0)? false : true);
}

// （デバッグ用）XML/TEXT出力先の<IPAddress>-<構成名>フォルダを192.168.0.30固定で処理するか
bool CIni_PrintConditionExporter::IsEnableAutoUpdateFixIP()
{
	return ((GetValueInt(SectionDebug.c_str(), KeyEnableAutoUpdateFixIP.c_str(), 0) == 0)? false : true);
}
