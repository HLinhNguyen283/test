/*! \file CtlDuplicateJIAreaSetting.h
 *  \brief duplicate JI area setting control for adding print settings
 *  \author SCREEN mizuta
 *  \date 2014/12/01 SCREEN mizuta new
*/

#if !defined(_CTL_DUPLICATE_JI_AREA_SETTING_H_)
#define _CTL_DUPLICATE_JI_AREA_SETTING_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to selects a print setting name for duplicating
*/
class CCtlDuplicateJIAreaSetting : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDuplicateJIAreaSetting();
	virtual ~CCtlDuplicateJIAreaSetting();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:

	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	virtual void OnDeleteItem();

	//部品

	//変数

private:

};



#endif // !defined(_CTL_DUPLICATE_JI_AREA_SETTING_H_)
