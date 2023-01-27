/*! \file CtlPrintingSpeedSelection.h
 *  \brief Printing Speed Selection control
 *  \author GCS Tan Ho
 *  \date 2013/05/13 GCS Tan Ho created
*/

#if !defined(_CTL_PRINTING_SPEED_SELECTION_H_)
#define _CTL_PRINTING_SPEED_SELECTION_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects to use the printing speed when the printer prints with the selection print settings of the screen.
*/
class CCtlPrintingSpeedSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintingSpeedSelection();
	virtual ~CCtlPrintingSpeedSelection();

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
	void RegisterPrintSpeedItems();
	//変数

private:
	//変数

	//関数
	
};

#endif // !defined(_CTL_PRINTING_SPEED_SELECTION_H_)
