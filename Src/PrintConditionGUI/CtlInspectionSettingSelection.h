/*! \file CtlInspectionSettingSelection.h
 *  \brief The Inspection setting pull down menu
 *  \author GCS Manh Nguyen
 *  \date 2013/12/14 GCS Manh Nguyen created
*/

#if !defined(_CTL_INSPECTION_SETTING_SELECTION_H_)
#define _CTL_INSPECTION_SETTING_SELECTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control locks to edit settings of the selection print condition.
*/
class CCtlInspectionSettingSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlInspectionSettingSelection();
	virtual ~CCtlInspectionSettingSelection();

	//イベント
	virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

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
};


#endif // !defined(_CTL_INSPECTION_SETTING_SELECTION_H_)
