/**
	* @file		Ini_UnwinderManager_work.cpp
	* @brief	UnwinderManager_work.ini操作クラス
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Ini_UwRwManager_work.h"
// ==========================================================================

// ==========================================================================
// Local definitions

//#define DEF_UW	"UW"
//#define DEF_RW	"RW"

static const std::string MutexName = _T("TP-UWRW_Communication_work");
static const std::string FileName =  _T("TP-UWRW_Communication_work.ini");
// ==========================================================================

/**
	* @brief  CIni_UwRwManager_work コンストラクタ
**/
CIni_UwRwManager_work::CIni_UwRwManager_work()
{
}

/**
	* @brief  CIni_UwRwManager_work デストラクタ
**/
CIni_UwRwManager_work::~CIni_UwRwManager_work()
{
}

/**
	* @brief  「UWへ通知を実施した最終日」取得
**/
std::string CIni_UwRwManager_work::getLastSendDate(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("SETTING_").append(inDeviceName).c_str(), "LastSendDate", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryStatusの通番(日単位)」取得
**/
UINT CIni_UwRwManager_work::getQueryStatusID(const char* inDeviceName)
{
	return GetValueInt(std::string("SETTING_").append(inDeviceName).c_str(), "QueryStatus", 0);
}

/**
	* @brief  「QueryResourceの通番(日単位)」取得
**/
UINT CIni_UwRwManager_work::getQueryResourceID(const char* inDeviceName)
{
	return GetValueInt(std::string("SETTING_").append(inDeviceName).c_str(), "QueryResource", 0);
}

/**
	* @brief  「StopPersChParamsの通番(日単位)」取得
**/
UINT CIni_UwRwManager_work::getStopPersChParamsID(const char* inDeviceName)
{
	return GetValueInt(std::string("SETTING_").append(inDeviceName).c_str(), "StopPersChParams", 0);
}

/**
	* @brief  「CommandResourceの通番(日単位)」取得
**/
UINT CIni_UwRwManager_work::getCommandResourceID(const char* inDeviceName)
{
	return GetValueInt(std::string("SETTING_").append(inDeviceName).c_str(), "CommandResource", 0);
}

/**
	* @brief  「状態監視用チャネルＩＤ」取得
**/
std::string CIni_UwRwManager_work::getQueryStatus_CannelID(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CHANNEL_").append(inDeviceName).c_str(), "Status_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙情報通知用チャネルＩＤ」取得
**/
std::string CIni_UwRwManager_work::getQueryResource_CannelID(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("CHANNEL_").append(inDeviceName).c_str(), "Resource_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「UWのステータス状態」取得
**/
std::string CIni_UwRwManager_work::getUwRwStatus(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("STATUS_INFO_").append(inDeviceName).c_str(), "Status", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
* @brief	get UW Enable Thickness Measure OP
**/
std::string CIni_UwRwManager_work::getEnableThicknessMeasureOP()
{
	char cBuf[256];
	GetValueString("STATUS_INFO_UW", "Enable_ThicknessMeasureOP", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「ロール径」取得
**/
std::string CIni_UwRwManager_work::getPaperRollDiameter(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "ROLL_DIAMETER", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙残量」取得
**/
std::string CIni_UwRwManager_work::getPaperRemainingAmount(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "REMAINING_AMOUNT", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  get Stop Roll Diameter for UW
**/
std::string CIni_UwRwManager_work::getStopRollDiameter(const char* inDeviceName)
{
	char cBuf[256];
	GetValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "Stop_Roll_Diameter", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「ログレベル」取得
**/
UINT CIni_UwRwManager_work::getDebugLogLevel()
{
	return GetValueInt("DEBUG", "LEVEL", 1);
}

/**
	* @brief  「UWへ通知を実施した最終日」更新
	* @param  inLastSendDate : [i]UWへ通知を実施した最終日
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putLastSendDate( const std::string& inLastSendDate, const char* inDeviceName)
{
	return WriteValueString(std::string("SETTING_").append(inDeviceName).c_str(), "LastSendDate", inLastSendDate.c_str());
}

/**
	* @brief  「QueryStatusの通番(日単位)」更新
	* @param  inID : [i]QueryStatusの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putQueryStatusID( const std::string& inID, const char* inDeviceName)
{
	return WriteValueString(std::string("SETTING_").append(inDeviceName).c_str(), "QueryStatus", inID.c_str());
}

/**
	* @brief  「QueryResourceの通番(日単位)」更新
	* @param  inID : [i]QueryResourceの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putQueryResourceID( const std::string& inID, const char* inDeviceName)
{
	return WriteValueString(std::string("SETTING_").append(inDeviceName).c_str(), "QueryResource", inID.c_str());
}

/**
	* @brief  「StopPersChParamsの通番(日単位)」更新
	* @param  inID : [i]StopPersChParamsの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putStopPersChParamsID( const std::string& inID, const char* inDeviceName)
{
	return WriteValueString(std::string("SETTING_").append(inDeviceName).c_str(), "StopPersChParams", inID.c_str());
}

/**
	* @brief  「CommandResourceの通番(日単位)」更新
	* @param  inLastSendDate : [i]CommandResourceの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putCommandResourceID( const std::string& inID, const char* inDeviceName)
{
	return WriteValueString(std::string("SETTING_").append(inDeviceName).c_str(), "CommandResource", inID.c_str());
}

/**
	* @brief  「状態監視用チャネルＩＤ」更新
	* @param  inCannelID : [i]状態監視用チャネルＩＤ
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putStatusCannelID( const std::string& inCannelID, const char* inDeviceName)
{
	return WriteValueString(std::string("CHANNEL_").append(inDeviceName).c_str(), "Status_CannelID", inCannelID.c_str());
}

/**
	* @brief  「用紙情報通知用チャネルＩＤ」更新
	* @param  inLastSendDate : [i]用紙情報通知用チャネルＩＤ
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putResourceCannelID( const std::string& inCannelID, const char* inDeviceName)
{
	return WriteValueString(std::string("CHANNEL_").append(inDeviceName).c_str(), "Resource_CannelID", inCannelID.c_str());
}

/**
	* @brief  「UWのステータス状態」更新
	* @param  inCannelID : [i]UWのステータス状態
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putUwRwStatus( const std::string& inStatus, const char* inDeviceName)
{
	return WriteValueString(std::string("STATUS_INFO_").append(inDeviceName).c_str(), "Status", inStatus.c_str());
}

/**
* @brief	put UW Enable Thickness Measure OP
**/
BOOL CIni_UwRwManager_work::putEnableThicknessMeasureOP(const std::string& inEnableThickness)
{
	return WriteValueString("STATUS_INFO_UW", "Enable_ThicknessMeasureOP", inEnableThickness.c_str());
}

/**
	* @brief  「ロール径」更新
	* @param  inRollDiameter : [i]ロール径
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putPaperRollDiameter( const std::string& inRollDiameter, const char* inDeviceName)
{
	return WriteValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "ROLL_DIAMETER", inRollDiameter.c_str());
}

/**
	* @brief  put Stop Roll Diameter for UW
**/
BOOL CIni_UwRwManager_work::putPaperStopRollDiameter(const std::string& inStopRollDiameter, const char* inDeviceName)
{
	return WriteValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "Stop_Roll_Diameter", inStopRollDiameter.c_str());
}

/**
	* @brief  「用紙残量」更新
	* @param  inRemainingAmount : [i]用紙残量
	* @retval 処理結果（FALSE:異常）
**/
BOOL CIni_UwRwManager_work::putPaperRemainingAmount( const std::string& inRemainingAmount, const char* inDeviceName)
{
	return WriteValueString(std::string("PAPER_INFO_").append(inDeviceName).c_str(), "REMAINING_AMOUNT", inRemainingAmount.c_str());
}


/**
	* @brief  Mutex名を取得する
	* @retval Mutex名
**/
const char* CIni_UwRwManager_work::GetMutexName()
{
	return MutexName.c_str();
}

/**
	* @brief  iniファイルパスを設定する
	* @param  bSelf : 自機かどうか
**/
void CIni_UwRwManager_work::SetFilePath(BOOL bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}
