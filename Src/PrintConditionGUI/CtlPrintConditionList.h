/*! \file CtlPrintConditionList.h
 *  \brief The control selects to delete items of Print settings
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
*/

#if !defined(_CTL_PRINT_CONDITION_LIST_H_)
#define _CTL_PRINT_CONDITION_LIST_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control selects to delete items of Print settings
*/
class CCtlPrintConditionList : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintConditionList();
	virtual ~CCtlPrintConditionList();

	//イベント
	virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	//virtual void OnUpdateValue();

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
	long m_numOfItems;				//!< number of items
};


#endif // !defined(_CTL_PRINT_CONDITION_LIST_H_)
