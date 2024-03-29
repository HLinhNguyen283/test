﻿/*! \file CtlClose_DlgDeleteDryer.h
 *  \brief Close screen control of Delete dryer settings dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho Created
*/

#if !defined(_CTL_CLOSE_DLG_DELETE_DRYER_H_)
#define _CTL_CLOSE_DLG_DELETE_DRYER_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief Close screen control of Delete dryer settings dialog
*/
class CCtlClose_DlgDeleteDryer : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlClose_DlgDeleteDryer();
	virtual ~CCtlClose_DlgDeleteDryer();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual void OnUpdateState();
	//virtual void OnUpdateValue();

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


#endif // !defined(_CTL_CLOSE_DLG_DELETE_DRYER_H_)
