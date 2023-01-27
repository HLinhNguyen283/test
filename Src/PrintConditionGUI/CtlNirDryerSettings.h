/*! \file CtlNirDryerSettings.h
 *  \brief The control edits a nir setting for printer 1, printer 2.
 *  \author SCREEN
 *  \date 2018/01/25 SCREEN created
*/

#if !defined(_CTL_NIR_DRYER_SETTINGS_H_)
#define _CTL_NIR_DRYER_SETTINGS_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control edits a precoast setting for printer 1, printer 2.
*/
class CCtlNirDryerSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlNirDryerSettings();
	virtual ~CCtlNirDryerSettings();

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


#endif // !defined(_CTL_NIR_DRYER_SETTINGS_H_)
