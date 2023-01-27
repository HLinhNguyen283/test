/*! \file CtlNewPrintSettingName.h
 *  \brief new print setting name control for adding print settings
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

#if !defined(_CTL_NEW_PRINT_SETTING_NAME_H_)
#define _CTL_NEW_PRINT_SETTING_NAME_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to edits to add a print setting name
*/
class CCtlNewPrintSettingName : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlNewPrintSettingName();
	virtual ~CCtlNewPrintSettingName();

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

	//関数
	
	// 合成処理
	
};


#endif // !defined(_CTL_NEW_PRINT_SETTING_NAME_H_)
