/*! \file CtlPrintConditionCommitting.h
 *  \brief The control saves the settings of the selection print settings on the screen.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

#if !defined(_CTL_PRINT_CONDITION_COMMITTING_H_)
#define _CTL_PRINT_CONDITION_COMMITTING_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control saves the settings of the selection print settings on the screen.
*/
class CCtlPrintConditionCommitting : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintConditionCommitting();
	virtual ~CCtlPrintConditionCommitting();

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
	
};


#endif // !defined(_CTL_PRINT_CONDITION_COMMITTING_H_)
