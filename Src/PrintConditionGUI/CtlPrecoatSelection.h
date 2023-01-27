/*! \file CtlPrecoatSelection.h
 *  \brief The control select a precoat settings for printer 2/printer 1 of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/

#if !defined(_CTL_PRECOAT_SELECTION_H_)
#define _CTL_PRECOAT_SELECTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control select a precoat settings for printer 2/printer 1 of the selection print settings.
*/
class CCtlPrecoatSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrecoatSelection();
	virtual ~CCtlPrecoatSelection();

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
	void RegisterItems();
	//変数

private:
	//変数

	//関数
	
};


#endif // !defined(_CTL_PRECOAT_SELECTION_H_)
