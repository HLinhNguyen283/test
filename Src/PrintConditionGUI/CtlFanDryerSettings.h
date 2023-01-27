/*! \file CtlFanDryerSettings.h
 *  \brief Fan dryer settings control for Add dryer settings dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/

#if !defined(_CTL_FAN_DRYER_SETTINGS_H_)
#define _CTL_FAN_DRYER_SETTINGS_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief Fan dryer settings control for Add dryer settings dialog
*/
class CCtlFanDryerSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlFanDryerSettings();
	virtual ~CCtlFanDryerSettings();

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


#endif // !defined(_CTL_FAN_DRYER_SETTINGS_H_)
