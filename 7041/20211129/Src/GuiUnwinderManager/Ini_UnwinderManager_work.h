/**
	* @file		Ini_UnwinderManager_work.cpp
	* @brief	UnwinderManager_work.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
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

	// Write
	BOOL putLastSendDate( const std::string& InLastSendDate );
	BOOL putQueryStatusID( const std::string& InID );
	BOOL putQueryResourceID( const std::string& InID );
	BOOL putStopPersChParamsID( const std::string& InID );
	BOOL putCommandResourceID( const std::string& InID );
	BOOL putCommandWakeupID( const std::string& InID );

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
