/*! \file CtlLockSettings.h
 *  \brief The control locks to edit settings of the selection print condition.
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/

#if !defined(_CTL_LOCK_SETTINGS_H_)
#define _CTL_LOCK_SETTINGS_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control locks to edit settings of the selection print condition.
*/
class CCtlLockSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlLockSettings();
	virtual ~CCtlLockSettings();

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


#endif // !defined(_CTL_LOCK_SETTINGS_H_)
