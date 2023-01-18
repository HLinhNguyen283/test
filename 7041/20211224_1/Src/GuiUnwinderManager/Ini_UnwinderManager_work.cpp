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
#include "Ini_UnwinderManager_work.h"
// ==========================================================================

// ==========================================================================
// Local definitions
static const std::string MutexName = _T("TP-UW_Communication_work");
static const std::string FileName =  _T("TP-UW_Communication_work.ini");
// ==========================================================================

/**
	* @brief  Ini_UnwinderManager_work コンストラクタ
**/
Ini_UnwinderManager_work::Ini_UnwinderManager_work()
{
}

/**
	* @brief  Ini_UnwinderManager_work デストラクタ
**/
Ini_UnwinderManager_work::~Ini_UnwinderManager_work()
{
}

/**
	* @brief  「UWへ通知を実施した最終日」取得
**/
std::string Ini_UnwinderManager_work::getLastSendDate()
{
	char cBuf[256];
	GetValueString("SETTING", "LastSendDate", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「QueryStatusの通番(日単位)」取得
**/
UINT Ini_UnwinderManager_work::getQueryStatusID()
{
	return GetValueInt("SETTING", "QueryStatus", 0);
}

/**
	* @brief  「QueryResourceの通番(日単位)」取得
**/
UINT Ini_UnwinderManager_work::getQueryResourceID()
{
	return GetValueInt("SETTING", "QueryResource", 0);
}

/**
	* @brief  「StopPersChParamsの通番(日単位)」取得
**/
UINT Ini_UnwinderManager_work::getStopPersChParamsID()
{
	return GetValueInt("SETTING", "StopPersChParams", 0);
}

/**
	* @brief  「CommandResourceの通番(日単位)」取得
**/
UINT Ini_UnwinderManager_work::getCommandResourceID()
{
	return GetValueInt("SETTING", "CommandResource", 0);
}

/**
	* @brief  「CommandWakeupの通番(日単位)」取得
**/
UINT Ini_UnwinderManager_work::getCommandWakeupID()
{
	return GetValueInt("SETTING", "CommandWakeup", 0);
}

/**
	* @brief  「状態監視用チャネルＩＤ」取得
**/
std::string Ini_UnwinderManager_work::getQueryStatus_CannelID()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Status_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙情報通知用チャネルＩＤ」取得
**/
std::string Ini_UnwinderManager_work::getQueryResource_CannelID()
{
	char cBuf[256];
	GetValueString("CHANNEL", "Resource_CannelID", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「UWのステータス状態」取得
**/
std::string Ini_UnwinderManager_work::getUnwinderStatus()
{
	char cBuf[256];
	GetValueString("UnWinder", "Status", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「ロール径」取得
**/
std::string Ini_UnwinderManager_work::getPaperRollDiameter()
{
	char cBuf[256];
	GetValueString("PAPER_INFO", "ROLL_DIAMETER", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「用紙残量」取得
**/
std::string Ini_UnwinderManager_work::getPaperRemainingAmount()
{
	char cBuf[256];
	GetValueString("PAPER_INFO", "REMAINING_AMOUNT", "", (LPTSTR)cBuf, sizeof(cBuf));
	return std::string(cBuf);
}

/**
	* @brief  「ログレベル」取得
**/
UINT Ini_UnwinderManager_work::getDebugLogLevel()
{
	return GetValueInt("DEBUG", "LEVEL", 1);
}

/**
	* @brief  「UWへ通知を実施した最終日」更新
	* @param  inLastSendDate : [i]UWへ通知を実施した最終日
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putLastSendDate( const std::string& inLastSendDate )
{
	return WriteValueString("SETTING", "LastSendDate", inLastSendDate.c_str());
}

/**
	* @brief  「QueryStatusの通番(日単位)」更新
	* @param  inID : [i]QueryStatusの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putQueryStatusID( const std::string& inID )
{
	return WriteValueString("SETTING", "QueryStatus", inID.c_str());
}

/**
	* @brief  「QueryResourceの通番(日単位)」更新
	* @param  inID : [i]QueryResourceの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putQueryResourceID( const std::string& inID )
{
	return WriteValueString("SETTING", "QueryResource", inID.c_str());
}

/**
	* @brief  「StopPersChParamsの通番(日単位)」更新
	* @param  inID : [i]StopPersChParamsの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putStopPersChParamsID( const std::string& inID )
{
	return WriteValueString("SETTING", "StopPersChParams", inID.c_str());
}

/**
	* @brief  「CommandResourceの通番(日単位)」更新
	* @param  inLastSendDate : [i]CommandResourceの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putCommandResourceID( const std::string& inID )
{
	return WriteValueString("SETTING", "CommandResource", inID.c_str());
}

/**
	* @brief  「CommandWakeupの通番(日単位)」更新
	* @param  inID : [i]CommandWakeupの通番(日単位)
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putCommandWakeupID( const std::string& inID )
{
	return WriteValueString("SETTING", "CommandWakeup", inID.c_str());
}

/**
	* @brief  「状態監視用チャネルＩＤ」更新
	* @param  inCannelID : [i]状態監視用チャネルＩＤ
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putStatusCannelID( const std::string& inCannelID )
{
	return WriteValueString("CHANNEL", "Status_CannelID", inCannelID.c_str());
}

/**
	* @brief  「用紙情報通知用チャネルＩＤ」更新
	* @param  inLastSendDate : [i]用紙情報通知用チャネルＩＤ
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putResourceCannelID( const std::string& inCannelID )
{
	return WriteValueString("CHANNEL", "Resource_CannelID", inCannelID.c_str());
}

/**
	* @brief  「UWのステータス状態」更新
	* @param  inCannelID : [i]UWのステータス状態
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putUnwinderStatus( const std::string& inStatus )
{
	return WriteValueString("UnWinder", "Status", inStatus.c_str());
}

/**
	* @brief  「ロール径」更新
	* @param  inRollDiameter : [i]ロール径
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putPaperRollDiameter( const std::string& inRollDiameter )
{
	return WriteValueString("UnWinder", "Status", inRollDiameter.c_str());
}

/**
	* @brief  「用紙残量」更新
	* @param  inRemainingAmount : [i]用紙残量
	* @retval 処理結果（FALSE:異常）
**/
BOOL Ini_UnwinderManager_work::putPaperRemainingAmount( const std::string& inRemainingAmount )
{
	return WriteValueString("UnWinder", "Status", inRemainingAmount.c_str());
}


/**
	* @brief  Mutex名を取得する
	* @retval Mutex名
**/
const char* Ini_UnwinderManager_work::GetMutexName()
{
	return MutexName.c_str();
}

/**
	* @brief  iniファイルパスを設定する
	* @param  bSelf : 自機かどうか
**/
void Ini_UnwinderManager_work::SetFilePath(bool bSelf)
{
	std::stringstream path;
	path << GetModuleDirectory() << _T("Preferences\\") << FileName;
	path.str().copy(m_filePath, sizeof(m_filePath));
	// ※path.str().copyのwarningを取るためにmemcpyに変更するとmemcpyでアサートする場合がある
}
