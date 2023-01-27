/**
 *	@file	Stub_PrinterStateIF.h
 *	@brief	Stub of PrinterState plugin
 *	@author	pt.sh-yasuda
 *	@date	2019/5/23 pt.sh-yasuda 新規作成
*/

#pragma once

#include "PrinterStateDef_OP.h"
#include "PrinterState_Callbacks.h"


/**
	@brief	Stub of PrinterState plugin
*/
class Stub_PrinterStateIF
{
private:
	/**
		@brief	constructor
	*/
	Stub_PrinterStateIF();

public:
	/**
		@brief	destructor
	*/
	~Stub_PrinterStateIF();

	/**
	 * @brief		OCRデコード有効環境
	 * @return		true: OCRデコード有効環境, false: OCRデコード無効環境
	**/
	static bool Stub_PrinterStateIF::isOCRandDECEnable();

	/**
	 * @brief		プレコート搭載
	 * @return		true: プレコート搭載, false: プレコート未搭載
	**/
	static bool Stub_PrinterStateIF::isLoadingPreCoat();

	/**
	 * @brief		温風ファン搭載 状況取得
	 * @param[in]	対象機 (true: 自機 / false: 相手機)
	 * @return		true: 温風ファン搭載, false: 温風ファン未搭載
	**/
	static bool Stub_PrinterStateIF::isLoadingHeaterFan(bool inSelf);

	/**
	 * @brief		NIR搭載 状況取得
	 * @param[in]	対象機 (true: 自機 / false: 相手機)
	 * @return		true: NIR搭載, false: NIR未搭載
	**/
	static bool Stub_PrinterStateIF::isUnitOfNir(bool inSelf);

	/**
	 * @brief		Ranger搭載 状況取得
	 * @return		true: Ranger搭載, false: Ranger未搭載
	**/
	static bool Stub_PrinterStateIF::isUnitOfRanger();

	/**
	 * @brief		150mpm対応 状況取得
	 				非対応の場合、最高速150mpmの印刷モードが使用不可
	 * @return		true: 150mpm対応, false: 150mpm非対応
	**/
	static bool Stub_PrinterStateIF::isSupportOf150mpm(bool inSelf);

	/**
	 * @brief		100mpm対応 状況取得
	 				非対応の場合、最高速100mpmの印刷モードが使用不可
	 * @return		true: 100mpm対応, false: 100mpm非対応
	**/
	static bool Stub_PrinterStateIF::isSupportOf100mpm(bool inSelf);

	/**
	 * @brief		標準 （オフセットコート紙）の搭載 状況取得
	 * @return		true: 搭載, false: 非搭載
	**/
	static bool Stub_PrinterStateIF::isUnitOfPrintModeForCoatedOffset(bool inSelf);

	/**
	 * @brief		６色の搭載 状況取得
	 * @return		true: 搭載, false: 非搭載
	**/
	static bool Stub_PrinterStateIF::isUnitOfSixColors(bool inSelf);

	/**
	 * @brief		全画像保管（IMS）が有効かどうか
	 * @return		true: 搭載, false: 非搭載
	**/
	static bool Stub_PrinterStateIF::IsIMSEnable();

	/**
	 * @brief		OCRデコード（OCRandDEC）が有効かどうか
	 * @return		true: 搭載, false: 非搭載
	**/
	static bool Stub_PrinterStateIF::IsOCRandDECEnable();

	/**
	 * @brief		印刷モニター＆ノズル抜けチェック（RTM&NCC）が有効かどうか
	 * @return		true: 搭載, false: 非搭載
	**/
	static bool Stub_PrinterStateIF::IsRTMandNCCEnable();
};

