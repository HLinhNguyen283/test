/*! \file CtlDeleteDryerSettingsSelection_DlgDeleteDryer.h
 *  \brief The control selects to delete items of dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/

#if !defined(_CTL_DELETE_DRYER_SETTINGS_SELECTION_H_)
#define _CTL_DELETE_DRYER_SETTINGS_SELECTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control selects to delete items of dryer settings.
*/
class CCtlDeleteDryerSettingsSelection_DlgDeleteDryer : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDeleteDryerSettingsSelection_DlgDeleteDryer();
	virtual ~CCtlDeleteDryerSettingsSelection_DlgDeleteDryer();

	//イベント
	virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	//virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品
	void RegisterItems();
	//変数

private:
	//変数
	long m_numOfItems;				//!< number of items
};


#endif // !defined(_CTL_DELETE_DRYER_SETTINGS_SELECTION_H_)
