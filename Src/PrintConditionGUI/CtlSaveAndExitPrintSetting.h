/*! \file CtlSaveAndExitPrintSetting.h
 *  \brief commit add paper name for adding print setting
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

#if !defined(_CTL_SAVE_AND_EXIT_PRINT_SETTING_H_)
#define _CTL_SAVE_AND_EXIT_PRINT_SETTING_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to commit/cancel to add the print setting name
*/
class CCtlSaveAndExitPrintSetting : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlSaveAndExitPrintSetting();
	virtual ~CCtlSaveAndExitPrintSetting();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
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

	//関数
};


#endif // !defined(_CTL_SAVE_AND_EXIT_PRINT_SETTING_H_)
