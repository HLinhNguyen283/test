/*! \file CtlDuplicatePaperType.h
 *  \brief duplicate paper type for adding paper type
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

#if !defined(_CTL_DUPLICATE_PAPER_TYPE_H_)
#define _CTL_DUPLICATE_PAPER_TYPE_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to selects a paper type name for duplicating
*/
class CCtlDuplicatePaperType : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDuplicatePaperType();
	virtual ~CCtlDuplicatePaperType();

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

	//関数
};


#endif // !defined(_CTL_DUPLICATE_PAPER_TYPE_H_)
