/**
 * @file	CtlPrimer.h
 * @brief	プライマーのコントロール
**/

#pragma once

// ==========================================================================
// Include files
#include "BaseCtl.h"

// ==========================================================================

/**
 * @brief	印刷で使用するチェックボックス表示
**/
class CCtlPrimer : public CBaseCtl
{
public:
	//定数

	//関数

	/**
		@brief	コンストラクタ
	**/
	CCtlPrimer();

	/**
		@brief	デストラクタ
	**/
	virtual ~CCtlPrimer();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();

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
		@brief	コントロール表示値更新イベント
	**/
	virtual void OnUpdateState();

	/**
		@brief	コントロールステータスイベント
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
