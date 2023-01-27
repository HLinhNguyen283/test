/*! \file CtlPrecoatDryerSettings.h
 *  \brief The control edits a precoast setting for printer 1, printer 2.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/

#if !defined(_CTL_PRECOAT_DRYER_SETTINGS_H_)
#define _CTL_PRECOAT_DRYER_SETTINGS_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control edits a precoast setting for printer 1, printer 2.
*/
class CCtlPrecoatDryerSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrecoatDryerSettings();
	virtual ~CCtlPrecoatDryerSettings();

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


#endif // !defined(_CTL_PRECOAT_DRYER_SETTINGS_H_)
