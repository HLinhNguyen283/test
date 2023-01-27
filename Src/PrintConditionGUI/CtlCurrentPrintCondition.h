/*! \file CtlCurrentPrintCondition.h
 *  \brief Current print condition setting control
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/

#if !defined(_CTL_CURRENT_PRINT_CONDITION_H_)
#define _CTL_CURRENT_PRINT_CONDITION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief Used to change the current print condition of the printer to it which is selecting on the screen.
*/
class CCtlCurrentPrintCondition : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlCurrentPrintCondition();
	virtual ~CCtlCurrentPrintCondition();

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
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	
};


#endif // !defined(_CTL_CURRENT_PRINT_CONDITION_H_)
