/*! \file CtlPaperOptionPreview.h
 *  \brief The control displays a preview of paper option.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/

#if !defined(_CTL_PAPER_OPTION_PREVIEW_H_)
#define _CTL_PAPER_OPTION_PREVIEW_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"
#include "CtlScannedImage.h"

// ==========================================================================

/*!
 *  \brief The control displays a preview of paper option.
*/
class CCtlPaperOptionPreview : public CBaseCtl
{
public:
	//定数
	#define EVENTID_DISPLAY_SIDE			1000

	//関数
	CCtlPaperOptionPreview();
	virtual ~CCtlPaperOptionPreview();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreate(CBaseCtl** inControlList, HWND inDefaultOwner);
	virtual void OnDestroy();
	//virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	//virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	virtual void OnDeleteItem();

	static void UpdatePaperViewer();
	void _UpdatePaperViewer();

	//部品

	//変数

private:
	//変数
	CCtlScannedImage	m_ScannedImageCtrl;		//!< The scanned image control.

};


#endif // !defined(_CTL_PAPER_OPTION_PREVIEW_H_)
