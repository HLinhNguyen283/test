/**
	* @file		Ini_UwRwManager.h
	* @brief	TP-UWRW_Communication.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include "Ini_Template.h"
#define DEF_UW	"UW"
#define DEF_RW	"RW"
/**
	@brief	UnwinderManager.ini操作クラス
*/
class CIni_UwRwManager : public CIni_Template
{
public:
	/**
		* @brief  CIni_UwRwManager コンストラクタ
	**/
	CIni_UwRwManager();

	/**
		* @brief  CIni_UwRwManager デストラクタ
	**/
	virtual ~CIni_UwRwManager();

	// [CONNECTION]セクション 
	/**
		* @brief  「コントローラの送信ポート番号(UWの受信ポート番号)」取得
	**/
	std::string getSender_Port(const char* inDeviceName);

	/**
		* @brief  「コントローラの受信ポート番号(UWの送信ポート番号)」取得
	**/
	std::string getReceiver_Port(const char* inDeviceName);

	/**
		* @brief  「UWのコントローラ要求受信IPアドレス」取得
	**/
	std::string getUwRw_IpAddress(const char* inDeviceName);

	/**
		* @brief  「状態監視受信フォルダ」取得
	**/
	std::string getStatus_Folder(const char* inDeviceName);

	/**
		* @brief  「用紙測定受信フォルダ」取得
	**/
	std::string getResource_Folder(const char* inDeviceName);

	/**
		* @brief  「SignalStatusが通知されていないと判断する時間（秒）」取得
	**/
	UINT getSignalstatus_Timeout_Judegment_Waittime(const char* inDeviceName);

	/**
		* @brief  「UW通知エラー発生時のリトライ回数」取得
	**/
	UINT getSend_Retry_Count(const char* inDeviceName);

	/**
		* @brief  「UW通知エラー発生時のリトライ通知の間隔（ミリ秒）」取得
	**/
	UINT getSend_Retry_Interval(const char* inDeviceName);

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

	/**
	 * @brief Get the [CommandResource] ExternalID value
	 * @return  value as string
	**/
	std::string getCommandResource_ExternalID();

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
	virtual void SetFilePath(BOOL bSelf);
};
