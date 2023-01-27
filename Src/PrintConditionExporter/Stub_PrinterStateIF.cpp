/**
 * @file	Stub_PrinterStateIF.cpp
 * @brief	stub of PrinterState plugin
 * @author	pt.ju-tanaka
 * @date	2019/06/26 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CommonCBRProc_OP.h"
#include "PrinterState_Callbacks.h"
#include "Stub_PrinterStateIF.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SPrinterState_Callbacks* outCbr);

// ==========================================================================

/**
 * @brief	I/F取得
 * @param[out]	outCbr : I/F構造体
 * @retval	true : 成功
 * @retval	false : 失敗
**/
static bool GetCallbacks(SPrinterState_Callbacks* outCbr)
{
	if(!PrinterState_GetCallbacks(outCbr)){
		return false;
	}
	return true;	
}

// DualSimplexオプションを取得する
bool Stub_PrinterStateIF::IsDualSimplexOption(bool inIsSelf)
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PrinterState_IsDualSimplexOption(inIsSelf);
}

// 温風ファン存在有無を取得する
long Stub_PrinterStateIF::IsLoadingHeaterFan(bool inIsSelf)
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PrinterState_IsLoadingHeaterFan(inIsSelf);
}

// NIR搭載状況を取得する
bool Stub_PrinterStateIF::IsUnitOfNir(bool inIsSelf)
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PrinterState_IsUnitOfNir(inIsSelf);
}

// プレコートの存在有無を取得する
bool Stub_PrinterStateIF::IsLoadingPreCoat()
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PrinterState_IsLoadingPreCoat();
}

//OCRデコード有効環境
bool Stub_PrinterStateIF::IsOCRandDECEnable()
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.isOCRandDECEnable();	
}

// Ranger搭載状況を取得する
bool Stub_PrinterStateIF::IsUnitOfRanger()
{
	SPrinterState_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PrinterState_IsUnitOfRanger();
}

/**
	@brief	ノズル抜けチェック機能の有効無効の取得
	@retval	true : 有効
	@retval	false : 無効
*/
bool Stub_PrinterStateIF::IsRTMandNCCEnable()
{
	SPrinterState_Callbacks cbr;
	if (!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.isRTMandNCCEnable();
}