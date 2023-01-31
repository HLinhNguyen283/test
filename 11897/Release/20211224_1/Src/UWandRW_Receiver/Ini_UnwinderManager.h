/**
	* @file		Ini_UnwinderManager.h
	* @brief	TP-UW_Communication.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include "Ini_Template.h"

/**
	@brief	UnwinderManager.ini操作クラス
*/
class Ini_UnwinderManager : public CIni_Template
{
public:
	/**
		* @brief  Ini_UnwinderManager コンストラクタ
	**/
	Ini_UnwinderManager();

	/**
		* @brief  Ini_UnwinderManager デストラクタ
	**/
	virtual ~Ini_UnwinderManager();

	// [CONNECTION]セクション 
	/**
		* @brief  「コントローラの送信ポート番号(UWの受信ポート番号)」取得
	**/
	std::string getSender_Port();

	/**
		* @brief  「コントローラの受信ポート番号(UWの送信ポート番号)」取得
	**/
	std::string getReceiver_Port();

	/**
		* @brief  「UWのコントローラ要求受信IPアドレス」取得
	**/
	std::string getUW_IpAddress();

	/**
		* @brief  「状態監視受信フォルダ」取得
	**/
	std::string getStatus_Folder();

	/**
		* @brief  「用紙測定受信フォルダ」取得
	**/
	std::string getResource_Folder();

	/**
		* @brief  「SignalStatusが通知されていないと判断する時間（秒）」取得
	**/
	UINT getSignalstatus_Timeout_Judegment_Waittime();

	/**
		* @brief  「UW通知エラー発生時のリトライ回数」取得
	**/
	UINT getSend_Retry_Count();

	/**
		* @brief  「UW通知エラー発生時のリトライ通知の間隔（ミリ秒）」取得
	**/
	UINT getSend_Retry_Interval();

	// [CHANNEL]セクション 
	/**
		* @brief  「状態監視間隔」取得
	**/
	std::string getQueryStatus_RepeatTime();

	/**
		* @brief  「用紙情報通知間隔」取得
	**/
	std::string getQueryResource_RepeatTime();

	/**
		* @brief  「QueryResourceで使用するChannelMode」取得
	**/
	std::string getQueryResource_ChannelMode();

	/**
		* @brief  「QueryResourceで使用するResourceDetails」取得
	**/
	std::string getQueryResource_ResourceDetails();

	/**
		* @brief  「QueryResourceで使用するResourceName」取得
	**/
	std::string getQueryResource_ResourceName();

	/**
		* @brief  「QueryResourceで使用するScope」取得
	**/
	std::string getQueryResource_Scope();

	/**
		* @brief  「StopPersChParamsで使用するMessageType」取得
	**/
	std::string getStopPersChParams_MessageType();

	// [PERIOD]セクション 
	/**
		* @brief  「コントローラ-UW間の通信ログの保管期限」取得
	**/
	UINT getPeriod_day();

	// [COMMAND]セクション 
	/**
		* @brief  「CommandResourceで使用するUpdateMethod」取得
	**/
	std::string getCommandResource_UpdateMethod();

	/**
		* @brief  「CommandResourceで使用するName」取得
	**/
	std::string getCommandResource_Name();

	// [PREFIX]セクション 
	/**
		* @brief  「QueryStatusで使用するIDの接頭文字」取得
	**/
	std::string getPrefix_QueryStatus();

	/**
		* @brief  「QueryResourceで使用するIDの接頭文字」取得
	**/
	std::string getPrefix_QueryResource();

	/**
		* @brief  「StopPersChParamsで使用するIDの接頭文字」取得
	**/
	std::string getPrefix_StopPersChParams();

	/**
		* @brief  「CommandResourceで使用するIDの接頭文字」取得
	**/
	std::string getPrefix_CommandResource();

	/**
		* @brief  「CommandWakeupで使用するIDの接頭文字」取得
	**/
	std::string getPrefix_CommandWakeup();

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
