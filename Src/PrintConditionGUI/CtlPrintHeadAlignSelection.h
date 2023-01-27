/*! \file CtlPrintHeadAlignSelection.h
 *  \brief The control selects a print head alignment of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/

#if !defined(_CTL_PRINT_HEAD_ALIGN_SELECTION_H_)
#define _CTL_PRINT_HEAD_ALIGN_SELECTION_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects a print head alignment of the selection print settings.
*/
class CCtlPrintHeadAlignSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintHeadAlignSelection();
	virtual ~CCtlPrintHeadAlignSelection();

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
	void RegisterItems();
	//変数

private:
	//変数
	long m_PrintHeadAlignStartIndex;			//!< The start index of print head alignment
	//関数
	
};


#endif // !defined(_CTL_PRINT_HEAD_ALIGN_SELECTION_H_)
