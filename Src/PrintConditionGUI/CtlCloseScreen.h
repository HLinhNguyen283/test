/*! \file CtlCloseScreen.h
 *  \brief The control exits the screen
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/

#if !defined(_CTL_CLOSE_SCREEN_H_)
#define _CTL_CLOSE_SCREEN_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control exits the screen
*/
class CCtlCloseScreen : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlCloseScreen();
	virtual ~CCtlCloseScreen();

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
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	
};


#endif // !defined(_CTL_CLOSE_SCREEN_H_)
