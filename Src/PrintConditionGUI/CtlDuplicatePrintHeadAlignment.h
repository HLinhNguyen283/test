/*! \file CtlDuplicatePrintHeadAlignment.h
 *  \brief duplicate print head alignment name control for adding print head alignment
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

#if !defined(_CTL_DUPLICATE_PRINT_HEAD_ALIGNMENT_H_)
#define _CTL_DUPLICATE_PRINT_HEAD_ALIGNMENT_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to selects a print head alignment name for duplicating
*/
class CCtlDuplicatePrintHeadAlignment : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDuplicatePrintHeadAlignment();
	virtual ~CCtlDuplicatePrintHeadAlignment();

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


#endif // !defined(_CTL_DUPLICATE_PRINT_HEAD_ALIGNMENT_H_)
