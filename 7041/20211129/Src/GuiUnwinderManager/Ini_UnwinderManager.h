/**
	* @file		Ini_UnwinderManager.h
	* @brief	TP-UW_Communication.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "Ini_Template.h"
// ==========================================================================

/**
	@brief	UnwinderManager.ini操作クラス
*/
class Ini_UnwinderManager : public CIni_Template
{
public:
	/**
		@brief	constructor
	*/
	Ini_UnwinderManager();

	/**
		@brief	destructor
	*/
	virtual ~Ini_UnwinderManager();

	// Read
	std::string getUW_IpAddress();
	std::string getTP_IpAddress();
	std::string getSender_Port();
	std::string getReceiver_Port();
	std::string getStatus_Folder();
	std::string getResource_Folder();

	std::string getQueryStatus_RepeatTime();
	std::string getQueryResource_RepeatTime();
	std::string getQueryStatus_CannelID();
	std::string getQueryResource_CannelID();
	std::string getQueryResource_ChannelMode();
	std::string getQueryResource_ResourceDetails();
	std::string getQueryResource_ResourceName();
	std::string getQueryResource_Scope();
	std::string getStopPersChParams_MessageType();

	std::string getCommandResource_UpdateMethod();
	std::string getCommandResource_Name();
	std::string getCommandResource_ExternalID();

	std::string getPrefix_QueryStatus();
	std::string getPrefix_QueryResource();
	std::string getPrefix_StopPersChParams();
	std::string getPrefix_CommandResource();
	std::string getPrefix_CommandWakeup();

	// Write
	BOOL putStatusCannelID( const std::string& InCannelID );
	BOOL putResourceCannelID( const std::string& InCannelID );

protected:
	/**
		@brief	Mutex名を取得する
		@retval	Mutex名
	*/
	virtual const char* GetMutexName();

	/**
		@brief	iniファイルパスを設定する
		@param	bSelf : 自機かどうか
	*/
	virtual void SetFilePath(bool bSelf);
};
