/*! \file CtlDryerSettingsSelection.h
 *  \brief The control selects settings of a dryer settings for printing.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

#if !defined(_CTL_DRYER_SETTINGS_SELECTION_H_)
#define _CTL_DRYER_SETTINGS_SELECTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects settings of a dryer settings for printing.
*/
class CCtlDryerSettingsSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDryerSettingsSelection();
	virtual ~CCtlDryerSettingsSelection();

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
	long m_DryerSettingsStartIndex;			//!< The start index of Dryer settings
	//関数
	
};


#endif // !defined(_CTL_DRYER_SETTINGS_SELECTION_H_)
