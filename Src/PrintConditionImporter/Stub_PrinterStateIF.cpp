/**
 *	@file	Stub_PrinterStateIF.cpp
 *	@brief	Stub of PrinterState plugin
 *	@author	pt.sh-yasuda
 *	@date	2019/6/18 pt.sh-yasuda 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Stub_PrinterStateIF.h"

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"

// ==========================================================================
// Local definitions

//! I/F構造体名
#define CBR SPrinterState_Callbacks

//! I/Fゲッター名
#define GetCallbacks PrinterState_GetCallbacks

// ==========================================================================


// constructor
Stub_PrinterStateIF::Stub_PrinterStateIF()
{
}

// destructor
Stub_PrinterStateIF::~Stub_PrinterStateIF()
{
}

//OCRデコード有効環境
bool Stub_PrinterStateIF::isOCRandDECEnable()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isOCREnabled = printerStateCb.isOCRandDECEnable();

	return isOCREnabled;
}

//プレコート搭載
bool Stub_PrinterStateIF::isLoadingPreCoat()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isLoadingPreCoat = printerStateCb.PrinterState_IsLoadingPreCoat();

	return isLoadingPreCoat;
}

//温風ファン搭載 状況取得
bool Stub_PrinterStateIF::isLoadingHeaterFan(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isLoadingHeaterFan = (printerStateCb.PrinterState_IsLoadingHeaterFan(inSelf) == 0 ? false : true);

	return isLoadingHeaterFan;
}

//NIR搭載 状況取得
bool Stub_PrinterStateIF::isUnitOfNir(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isUnitOfNir = printerStateCb.PrinterState_IsUnitOfNir(inSelf);

	return isUnitOfNir;
}

//Ranger搭載 状況取得
bool Stub_PrinterStateIF::isUnitOfRanger()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isUnitOfRanger = printerStateCb.PrinterState_IsUnitOfRanger();

	return isUnitOfRanger;
}

//150mpm対応 状況取得
bool Stub_PrinterStateIF::isSupportOf150mpm(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isSupport150mpm = (printerStateCb.PrinterState_GetLoadingSpeedOption(inSelf) == 2 ? true : false); 

	return isSupport150mpm;
}

//100mpm対応 状況取得
bool Stub_PrinterStateIF::isSupportOf100mpm(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isSupport100mpm = (printerStateCb.PrinterState_GetLoadingSpeed100Option(inSelf) == 0 ? false : true);

	return isSupport100mpm;
}

//標準 （オフセットコート紙）の搭載 状況取得
bool Stub_PrinterStateIF::isUnitOfPrintModeForCoatedOffset(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isPrintModeForCoatedOffset = printerStateCb.PrinterState_IsUnitOfPrintModeForCoatedOffset(inSelf);

	return isPrintModeForCoatedOffset;
}

//６色の搭載 状況取得
bool Stub_PrinterStateIF::isUnitOfSixColors(bool inSelf)
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isUnitOfSixColors = printerStateCb.PrinterState_IsUnitOfSixColors(inSelf);

	return isUnitOfSixColors;
}

//全画像保管（IMS）が有効かどうか
bool Stub_PrinterStateIF::IsIMSEnable()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isIMSEnable = printerStateCb.isIMSEnable();

	return isIMSEnable;
}

//OCRデコード（OCRandDEC）が有効かどうか
bool Stub_PrinterStateIF::IsOCRandDECEnable()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isOCREnable = printerStateCb.isOCRandDECEnable();

	return isOCREnable;
}

//印刷モニター＆ノズル抜けチェック（RTM&NCC）が有効かどうか
bool Stub_PrinterStateIF::IsRTMandNCCEnable()
{
	CBR printerStateCb;
	if(!GetCallbacks(&printerStateCb)){
		return false;
	}
	bool isRTMandNCCEnable = printerStateCb.isRTMandNCCEnable();

	return isRTMandNCCEnable;
}
