/**
	* @file		Ini_UnwinderManager_work.cpp
	* @brief	UnwinderManager_work.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "Ini_Template.h"
// ==========================================================================

/**
	@brief	UnwinderManager_work.ini操作クラス
*/
class Ini_UnwinderManager_work : public CIni_Template
{
public:
	/**
		@brief	constructor
	*/
	Ini_UnwinderManager_work();

	/**
		@brief	destructor
	*/
	virtual ~Ini_UnwinderManager_work();

	// Read
	std::string getLastSendDate();
	UINT getQueryStatusID();
	UINT getQueryResourceID();
	UINT getStopPersChParamsID();
	UINT getCommandResourceID();
	UINT getCommandWakeupID();
	std::string getQueryStatus_CannelID();
	std::string getQueryResource_CannelID();

	// Write
	BOOL putLastSendDate( const std::string& inLastSendDate );
	BOOL putQueryStatusID( const std::string& inID );
	BOOL putQueryResourceID( const std::string& inID );
	BOOL putStopPersChParamsID( const std::string& inID );
	BOOL putCommandResourceID( const std::string& inID );
	BOOL putCommandWakeupID( const std::string& inID );
	BOOL putStatusCannelID( const std::string& inCannelID );
	BOOL putResourceCannelID( const std::string& inCannelID );
	BOOL putUnwinderStatus( const std::string& inStatus );

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
