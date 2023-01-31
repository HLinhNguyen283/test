/**
	* @file		Ini_PrinterDescriptor.h
	* @brief	PrinterDescriptor.ini操作クラス ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/
#pragma once

#include "Ini_Template.h"

/**
	@brief	PrinterDescriptor.ini操作クラス
*/
class Ini_PrinterDescriptor : public CIni_Template
{
public:
	/**
		* @brief  Ini_PrinterDescriptor コンストラクタ
	**/
	Ini_PrinterDescriptor();

	/**
		* @brief  Ini_PrinterDescriptor デストラクタ
	**/
	virtual ~Ini_PrinterDescriptor();

	/**
		* @brief  「UW連携機能有/無情報」取得
		* @retval TRUE：機能あり、FALSE:機能なし
	**/
	UINT getUnwinderOption();

	/**
		* @brief  「UWへ通知するデバイスID」取得
	**/
	std::string getDeviceID();

	/**
		* @brief  「自機のIPアドレス」取得
	**/
	std::string getMyIpAddress();

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
