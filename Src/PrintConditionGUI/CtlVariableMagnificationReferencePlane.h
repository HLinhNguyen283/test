/*! \file CtlVariableMagnificationReferencePlane.h
 *  \brief 基準面指定コントロール.
*/

#pragma once

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief 基準面指定コントロールクラス
*/
class CCtlVariableMagnificationReferencePlane : public CBaseCtl
{
public:
	//定数

	//関数

	/**
		@brief	コンストラクタ
	**/
	CCtlVariableMagnificationReferencePlane();

	/**
		@brief	デストラクタ
	**/
	virtual ~CCtlVariableMagnificationReferencePlane();

	//イベント

	/**
		@brief コントロール操作イベント
		@param[in] hWnd : ウィンドウハンドル
		@param[in] essage : メッセージ番号
		@param[in] wParam : パラメータ
		@param[in] lParam : パラメータ
		@retval	DEF_NONE:通知なし, DEF_EXIT_PAGE:画面を閉じる
	**/
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	/**
		@brief	コントロールステータスイベント
	**/
	virtual void OnUpdateState();

	/**
		@brief	コントロール表示内容更新イベント
	**/
	virtual void OnUpdateValue();

protected:
	//関数
	//イベント

	/**
		@brief	コントロール属性設定イベント
	**/
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数

};

