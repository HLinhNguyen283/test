/**
	* @file		Ini_UnwinderManager_work.h
	* @brief	UnwinderManager_work.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include "Ini_Template.h"

/**
	@brief	UnwinderManager_work.ini操作クラス
*/
class Ini_UnwinderManager_work : public CIni_Template
{
public:
	/**
		* @brief  Ini_UnwinderManager_work コンストラクタ
	**/
	Ini_UnwinderManager_work();

	/**
		* @brief  Ini_UnwinderManager_work デストラクタ
	**/
	virtual ~Ini_UnwinderManager_work();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
		* @brief  「UWへ通知を実施した最終日」取得
	**/
	std::string getLastSendDate();

	/**
		* @brief  「QueryStatusの通番(日単位)」取得
	**/
	UINT getQueryStatusID();

	/**
		* @brief  「QueryResourceの通番(日単位)」取得
	**/
	UINT getQueryResourceID();

	/**
		* @brief  「StopPersChParamsの通番(日単位)」取得
	**/
	UINT getStopPersChParamsID();

	/**
		* @brief  「CommandResourceの通番(日単位)」取得
	**/
	UINT getCommandResourceID();

	/**
		* @brief  「CommandWakeupの通番(日単位)」取得
	**/
	UINT getCommandWakeupID();

	/**
		* @brief  「状態監視用チャネルＩＤ」取得
	**/
	std::string getQueryStatus_CannelID();

	/**
		* @brief  「用紙情報通知用チャネルＩＤ」取得
	**/
	std::string getQueryResource_CannelID();

	/**
		* @brief  「UWのステータス状態」取得
	**/
	std::string getUnwinderStatus();

	/**
		* @brief  「ロール径」取得
	**/
	std::string getPaperRollDiameter();

	/**
		* @brief  「用紙残量」取得
	**/
	std::string getPaperRemainingAmount();

	/**
		* @brief  「ログレベル」取得
	**/
	UINT getDebugLogLevel();

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
	BOOL putLastSendDate( const std::string& inLastSendDate );

	/**
		* @brief  「QueryStatusの通番(日単位)」更新
		* @param  inID : [i]QueryStatusの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putQueryStatusID( const std::string& inID );

	/**
		* @brief  「QueryResourceの通番(日単位)」更新
		* @param  inID : [i]QueryResourceの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putQueryResourceID( const std::string& inID );

	/**
		* @brief  「StopPersChParamsの通番(日単位)」更新
		* @param  inID : [i]StopPersChParamsの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putStopPersChParamsID( const std::string& inID );

	/**
		* @brief  「CommandResourceの通番(日単位)」更新
		* @param  inLastSendDate : [i]CommandResourceの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putCommandResourceID( const std::string& inID );

	/**
		* @brief  「CommandWakeupの通番(日単位)」更新
		* @param  inID : [i]CommandWakeupの通番(日単位)
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putCommandWakeupID( const std::string& inID );

	/**
		* @brief  「状態監視用チャネルＩＤ」更新
		* @param  inCannelID : [i]状態監視用チャネルＩＤ
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putStatusCannelID( const std::string& inCannelID );

	/**
		* @brief  「用紙情報通知用チャネルＩＤ」更新
		* @param  inLastSendDate : [i]用紙情報通知用チャネルＩＤ
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putResourceCannelID( const std::string& inCannelID );

	/**
		* @brief  「UWのステータス状態」更新
		* @param  inCannelID : [i]UWのステータス状態
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putUnwinderStatus( const std::string& inStatus );

	/**
		* @brief  「ロール径」更新
		* @param  inRollDiameter : [i]ロール径
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putPaperRollDiameter( const std::string& inRollDiameter );

	/**
		* @brief  「用紙残量」更新
		* @param  inRemainingAmount : [i]用紙残量
		* @retval 処理結果（FALSE:異常）
	**/
	BOOL putPaperRemainingAmount( const std::string& inRemainingAmount );

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
	virtual void SetFilePath(bool bSelf);
};
