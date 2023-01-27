/**
 * @file	Stub_PrinterStateIF.h
 * @brief	stub of PrinterState plugin
 * @author	pt.ju-tanaka
 * @date	2019/06/26 pt.ju-tanaka 新規作成
**/

#pragma once

// ==========================================================================
// Include files

// ==========================================================================

/**
 * @brief	stub of Stub_PrinterState plugin
**/
class Stub_PrinterStateIF
{
public:
	/**
		@brief	DualSimplexオプションを取得する
		@param[in]	inIsSelf : true：自機、false：相手機
		@retval	true : DualSimplexオプション状態有効
		@retval	false : DualSimplexオプション状態無効
	*/
	static bool IsDualSimplexOption(bool inIsSelf);

	/**
		@brief	温風ファン存在有無を取得する
		@param[in]	inIsSelf ：true：自機、false：相手機.
		@retval	温風ファン搭載状況(0：未搭載、1：Type1、2：Type2、以降 Type番号)
	*/
	static long IsLoadingHeaterFan(bool inIsSelf);

	/**
		@brief	NIR搭載状況を取得する
		@param[in]	inIsSelf : true：自機、false：相手機
		@retval	true : NIR搭載
		@retval	false : NIR未搭載
	*/
	static bool IsUnitOfNir(bool inIsSelf);

	/**
		@brief	プレコートの存在有無を取得する
		@retval	true : プレコート搭載
		@retval	false : プレコート未搭載
	*/
	static bool IsLoadingPreCoat();
	
	/**
		@brief	OCRデコード有効環境を取得する
		@retval	true : OCRデコード有効環境
		@retval	false : OCRデコード無効環境
	*/	
	static bool IsOCRandDECEnable();

	/**
	 * @brief		Ranger搭載 状況取得
	 * @return		true: Ranger搭載, false: Ranger未搭載
	**/
	static bool IsUnitOfRanger();

	/**
		@brief	ノズル抜けチェック機能の有効無効の取得
		@retval	true : 有効
		@retval	false : 無効
	*/
	static bool IsRTMandNCCEnable();
};