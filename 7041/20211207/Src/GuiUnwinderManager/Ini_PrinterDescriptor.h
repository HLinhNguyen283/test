/**
	* @file		Ini_PrinterDescriptor.h
	* @brief	PrinterDescriptor.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/
#pragma once

// ==========================================================================
// Include files
#include "Ini_Template.h"
// ==========================================================================

/**
	@brief	PrinterDescriptor.ini操作クラス
*/
class Ini_PrinterDescriptor : public CIni_Template
{
public:
	/**
		@brief	constructor
	*/
	Ini_PrinterDescriptor();

	/**
		@brief	destructor
	*/
	virtual ~Ini_PrinterDescriptor();

	// Read
	UINT getUnwinderOption();
	std::string getDeviceID();

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
