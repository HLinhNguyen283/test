/**
	@file	CtlPaperWeight.h
	@brief	The control edits a page size of the selection print settings on the screen.
*/

#pragma once

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/**
	@brief	The control edits a page size of the selection print settings on the screen.
*/
class CCtlPaperWeight : public CBaseCtl
{
public:
	//定数

	//関数
	/**
		@brief	constructor
	*/
	CCtlPaperWeight();
	/**
		@brief	destructor
	*/
	virtual ~CCtlPaperWeight();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	/**
		@brief	[event]the control operated
		@param[in]	hWnd : window handle
		@param[in]	Message : messag id
		@param[in]	wParam : parameter 1
		@param[in]	lParam : parameter 2
		@retval	DEF_NONE : continue showing this screen.
		@retval	DEF_EXIT_PAGE : close this screen.
	*/
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	/**
		@brief	コントロールステータス更新イベント
	*/
	virtual void OnUpdateState();
	/**
		@brief	コントロール表示値更新イベント
	*/
	virtual void OnUpdateValue();

protected:
	//関数
	
	//イベント
	/**
		@brief	[event]set the control's property
	*/
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数

	//関数
	
};
