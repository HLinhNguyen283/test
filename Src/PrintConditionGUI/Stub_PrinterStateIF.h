/*! \file Stub_PrinterStateIF.h
 *  \brief Stub_PrinterStateIF.hの処理
 *  \author omori
 *  \date 2019/03/05 omori 新規作成
*/

#pragma once
#include "CommonCBRProc.h"
#include "PrinterStateDef_OP.h"

class Stub_PrinterStateIF
{

public:

	/**
	 * @brief	コンストラクタ
	**/
	Stub_PrinterStateIF();

	/**
	 * @brief	デストラクタ
	**/
	~Stub_PrinterStateIF();


	/**
	@brief	全画像保管（IMS）が有効かどうか
	@retval	true : 有効
	@retval	false : 無効
	*/
	static bool IsIMSEnable();

	/**
	@brief	OCRデコード（OCRandDEC）が有効かどうか
	@retval	true : 有効
	@retval	false : 無効
	*/
	static bool IsOCREnable();

	/*!
	*  \brief Ranger搭載有無を取得（false:未搭載、true:搭載）
	*  \return false:未搭載、true:搭載
	*/
	static bool IsUnitOfRanger();

	/**
	 *	@brief 印刷機の動作情報を取得する
	 *	@retval	印刷機の動作情報
	**/
	static ST_PRINTER_ACTION GetPrinterAction();

	/**
		@brief	印刷モニター＆ノズル抜けチェック（RTM&NCC）が有効かどうか
		@retval	true : 有効
		@retval	false : 無効
	*/
	static bool IsRTMandNCCEnable();

	/*!
		@brief モノクロプリンターかどうか
		@retval	true : モノクロ機
		@retval	false : カラー機
	*/
	static bool IsMonochromePrinter();

	/**
		@brief	プライマー接続の有無を取得する
		@retval	true : プライマー接続有り
		@retval	false : プライマー接続無し
	*/
	static bool IsConnectedPrimer();
};