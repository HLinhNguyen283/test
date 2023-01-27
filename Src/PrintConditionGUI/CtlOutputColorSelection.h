/*! \file CtlOutputColorSelection.h
 *  \brief CtlOutputColorSelection.hの処理
 *  \author SCREEN
 *  \date 2017/11/20 SCREEN 新規作成
*/

#if !defined(_CTL_OUTPUTCOLOR_SELECTION_H_)
#define _CTL_OUTPUTCOLOR_SELECTION_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects a tension of the selection print settings on the screen.
*/
class CCtlOutputColorSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlOutputColorSelection();
	virtual ~CCtlOutputColorSelection();

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
	void RegisterOutputColorItems();
	//変数

private:
	//変数

	//関数
	
};

#endif // !defined(_CTL_OUTPUTCOLOR_SELECTION_H_)
