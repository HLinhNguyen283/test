/*! \file Stub_PrinterStateIF.cpp
 *  \brief Stub_PrinterStateIF.cppの処理
 *  \author omori
 *  \date 2019/03/05 omori 新規作成
*/

// ==========================================================================
// Include files
// ==========================================================================
#include "stdafx.h"
#include "Stub_PrinterStateIF.h"
#include "PrinterState_Callbacks.h"
#include "CommonCBRProc_OP.h"


// ==========================================================================
// Local definitions
// ==========================================================================
//! I/F構造体名
#define CBR SPrinterState_Callbacks

//! I/Fゲッター名
#define GetCallbacks PrinterState_GetCallbacks


//全画像保管（IMS）が有効かどうか
bool Stub_PrinterStateIF::IsIMSEnable()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.isIMSEnable();
}

//OCRデコード（OCRandDEC）が有効かどうか
bool Stub_PrinterStateIF::IsOCREnable()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.isOCRandDECEnable();
}

// Ranger搭載機かどうか
bool Stub_PrinterStateIF::IsUnitOfRanger()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.PrinterState_IsUnitOfRanger();

}

// 印刷機の動作情報を取得する
ST_PRINTER_ACTION Stub_PrinterStateIF::GetPrinterAction()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		ST_PRINTER_ACTION errPrinterAction;
		// 異常ケースにより印刷機動作中では無い設定とする
		errPrinterAction.adjustPrint.working = false;
		return errPrinterAction;
	}
	return cb.PrinterState_GetPrinterAction();
}

// 印刷モニター＆ノズル抜けチェック（RTM&NCC）が有効かどうか
bool Stub_PrinterStateIF::IsRTMandNCCEnable()
{
	CBR cb;
	if(!PrinterState_GetCallbacks(&cb)) {
		return false;
	}

	return cb.isRTMandNCCEnable();
}

// モノクロプリンターかどうか
bool Stub_PrinterStateIF::IsMonochromePrinter()
{
	CBR cb;
	if(!PrinterState_GetCallbacks(&cb)){
		return false;
	}

	return cb.IsMonochromePrinter();
}

// プライマー接続の有無を取得する
bool Stub_PrinterStateIF::IsConnectedPrimer()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.IsConnectedPrimer();
}