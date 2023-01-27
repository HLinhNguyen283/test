/*! \file CtlImageStorage.h
 *  \brief CtlImageStorage.hの処理
 *  \author omori
 *  \date 2019/02/28 omori 新規作成
*/

#pragma once

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

class CCtlImageStorage : public CBaseCtl
{
public:

	CCtlImageStorage();

	virtual ~CCtlImageStorage();
	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数

};