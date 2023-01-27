/*! \file CtlClosePrintConditionDeleteDlg.h
 *  \brief Close screen control of Delete Print Settings dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho Created
*/

#if !defined(_CTL_CLOSE_PRINT_CONDITION_DELETE_DLG_H_)
#define _CTL_CLOSE_PRINT_CONDITION_DELETE_DLG_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief Close screen control of Delete Print Settings dialog
*/
class CCtlClosePrintConditionDeleteDlg : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlClosePrintConditionDeleteDlg();
	virtual ~CCtlClosePrintConditionDeleteDlg();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual void OnUpdateState();
	//virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	
};


#endif // !defined(_CTL_CLOSE_PRINT_CONDITION_DELETE_DLG_H_)
