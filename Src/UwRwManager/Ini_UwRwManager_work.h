/**
	* @file		Ini_UnwinderManager_work.h
	* @brief	UnwinderManager_work.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include "Ini_Template.h"
#define DEF_UW	"UW"
#define DEF_RW	"RW"
/**
	@brief	UnwinderManager_work.ini操作クラス
*/
class CIni_UwRwManager_work : public CIni_Template
{
public:
	/**
		* @brief  CIni_UwRwManager_work コンストラクタ
	**/
	CIni_UwRwManager_work();

	/**
		* @brief  CIni_UwRwManager_work デストラクタ
	**/
	virtual ~CIni_UwRwManager_work();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
		* @brief  「UWへ通知を実施した最終日」取得
	**/
	std::string getLastSendDate(const char* inDeviceName);

	/**
		* @brief  「QueryStatusの通番(日単位)」取得
	**/
	UINT getQueryStatusID(const char* inDeviceName);

	/**
		* @brief  「QueryResourceの通番(日単位)」取得
	**/
	UINT getQueryResourceID(const char* inDeviceName);

	/**
		* @brief  「StopPersChParamsの通番(日単位)」取得
	**/
	UINT getStopPersChParamsID(const char* inDeviceName);

	/**
		* @brief  「CommandResourceの通番(日単位)」取得
	**/
	UINT getCommandResourceID(const char* inDeviceName);

	/**
		* @brief  「状態監視用チャネルＩＤ」取得
	**/
	std::string getQueryStatus_CannelID(const char* inDeviceName);

	/**
		* @brief  「用紙情報通知用チャネルＩＤ」取得
	**/
	std::string getQueryResource_CannelID(const char* inDeviceName);

	/**
		* @brief  「UWのステータス状態」取得
	**/
	std::string getUwRwStatus(const char* inDeviceName);

	/**
		* @brief  「ロール径」取得
	**/
	std::string getPaperRollDiameter(const char* inDeviceName);

	/**
		* @brief  「用紙残量」取得
	**/
	std::string getPaperRemainingAmount(const char* inDeviceName);

	/**
		* @brief  「ログレベル」取得
	**/
	UINT getDebugLogLevel();

	/**
	* @brief  get Stop Roll Diameter for UW/RW
	**/
	std::string getStopRollDiameter(const char* inDeviceName);

	/**
	* @brief	get UW Enable Thickness Measure OP
	**/
	std::string getEnableThicknessMeasureOP();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
		* @brief  「UWへ通知を実施した最終日」更新
		* @param  inLastSendDate : [i]UWへ通知を実施した最終日
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putLastSendDate( const std::string& inLastSendDate, const char* inDeviceName);

	/**
		* @brief  「QueryStatusの通番(日単位)」更新
		* @param  inID : [i]QueryStatusの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putQueryStatusID( const std::string& inID, const char* inDeviceName);

	/**
		* @brief  「QueryResourceの通番(日単位)」更新
		* @param  inID : [i]QueryResourceの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putQueryResourceID( const std::string& inID, const char* inDeviceName);

	/**
		* @brief  「StopPersChParamsの通番(日単位)」更新
		* @param  inID : [i]StopPersChParamsの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putStopPersChParamsID( const std::string& inID, const char* inDeviceName);

	/**
		* @brief  「CommandResourceの通番(日単位)」更新
		* @param  inLastSendDate : [i]CommandResourceの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putCommandResourceID( const std::string& inID, const char* inDeviceName);

	/**
		* @brief  「状態監視用チャネルＩＤ」更新
		* @param  inCannelID : [i]状態監視用チャネルＩＤ
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putStatusCannelID( const std::string& inCannelID, const char* inDeviceName);

	/**
		* @brief  「用紙情報通知用チャネルＩＤ」更新
		* @param  inLastSendDate : [i]用紙情報通知用チャネルＩＤ
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putResourceCannelID( const std::string& inCannelID, const char* inDeviceName);

	/**
		* @brief  「UWのステータス状態」更新
		* @param  inCannelID : [i]UWのステータス状態
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putUwRwStatus( const std::string& inStatus, const char* inDeviceName);

	/**
	* @brief	put UW Enable Thickness Measure OP
	**/
	BOOL putEnableThicknessMeasureOP(const std::string& inEnableThickness);

	/**
		* @brief  「ロール径」更新
		* @param  inRollDiameter : [i]ロール径
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putPaperRollDiameter( const std::string& inRollDiameter, const char* inDeviceName);

	/**
		* @brief  「用紙残量」更新
		* @param  inRemainingAmount : [i]用紙残量
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putPaperRemainingAmount( const std::string& inRemainingAmount, const char* inDeviceName);

	/**
	* @brief  put Stop Roll Diameter for UW
	**/
	BOOL putPaperStopRollDiameter(const std::string& inStopRollDiameter, const char* inDeviceName);

protected:
	/**
		* @brief  Mutex名を取得する
		* @retval Mutex名
	**/
	virtual const char* GetMutexName();

	/**
		* @brief  iniファイルパスを設定する
		* @param  bSelf : 自機かどうか
	**/
	virtual void SetFilePath(BOOL bSelf);
};
