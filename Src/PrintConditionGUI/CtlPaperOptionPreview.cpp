/*! \file CtlPaperOptionPreview.cpp
 *  \brief The control displays a preview of paper option.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPaperOptionPreview.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_W_VIEW					475
#define DEF_H_VIEW					230
#define DEF_OFFSET_X				695
#define DEF_OFFSET_Y				370

//! the control id list
enum{
	CTRLID_SC_PREVIEW = 0,				//!< Static-box: Preview

	CTLID_COUNT
};

// ==========================================================================

CCtlPaperOptionPreview* st_Object = NULL;			// Pointer to this class

/*! \brief constructor
*/
CCtlPaperOptionPreview::CCtlPaperOptionPreview()
{
	st_Object = this;
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPaperOptionPreview::~CCtlPaperOptionPreview()
{
}

/*! \brief コントロール作成イベント
*  \param inControlList コントロールクラス一覧
*  \param inDefaultOwner デフォルトのオーナーウィンドウ
*  \return 成功：TRUE
*  \return 失敗：FALSE
*/
BOOL CCtlPaperOptionPreview::OnCreate(CBaseCtl** inControlList, HWND inDefaultOwner)
{
	// Create special scanned image control
	{
		RECT rect = {DEF_OFFSET_X + 30, DEF_OFFSET_Y + 90, DEF_OFFSET_X + 30 + DEF_W_VIEW - 1, DEF_OFFSET_Y + 90 + DEF_H_VIEW - 1};
		m_ScannedImageCtrl.SetInterface(m_data, m_printer);
		m_ScannedImageCtrl.Initialize(inDefaultOwner, rect);
	}

	// base's method
	CBaseCtl::OnCreate(inControlList, inDefaultOwner);

	return TRUE;
}

/*! \brief コントロール破棄イベント
*/
void CCtlPaperOptionPreview::OnDestroy()
{
	// base's method
	CBaseCtl::OnDestroy();

	// destroy special scanned image control
	m_ScannedImageCtrl.Finalize();
}

/*! \brief [event]set the control's property
*/
void CCtlPaperOptionPreview::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Static-box: Preview
	{
		long ctlId = CTRLID_SC_PREVIEW;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_SUNKEN | SCST_BITMAP | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, DEF_OFFSET_X, DEF_OFFSET_Y, DEF_OFFSET_X + DEF_W_VIEW, DEF_OFFSET_Y + DEF_H_VIEW);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPaperOptionPreview::OnCreateItem()
{
	// Start a timer event
	SetTimer(GetMainWindow(), EVENTID_DISPLAY_SIDE, 2000, (TIMERPROC)UpdatePaperViewer);

	m_ScannedImageCtrl.Show(TRUE);
	m_ScannedImageCtrl.UpdateControl();
}

/*! \brief コントロールのアイテムを破棄するイベント
*/
void CCtlPaperOptionPreview::OnDeleteItem()
{
	// End the timer event
	KillTimer(GetMainWindow(), EVENTID_DISPLAY_SIDE);
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPaperOptionPreview::OnUpdateState()
{
	// Show control
	SetControlState(m_ctl[CTRLID_SC_PREVIEW], CST_SHOW);
}

/*! \brief Update timer event
*/
void CCtlPaperOptionPreview::UpdatePaperViewer()
{
	st_Object->_UpdatePaperViewer();
}

/*! \brief Update timer event
*/
void CCtlPaperOptionPreview::_UpdatePaperViewer()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(true == pData->IsDED()){
		// Get the current paper preview side
		DEF_PRINT_SIDE printSide = pData->GetPaperPreviewSide();

		// Change the displaying side of paper preview
		if(DEF_PRINT_FRONT == printSide){
			pData->SetPaperPreviewSide(DEF_PRINT_BACK);
		}else{
			pData->SetPaperPreviewSide(DEF_PRINT_FRONT);
		}
	}

	m_ScannedImageCtrl.Show(TRUE);
	m_ScannedImageCtrl.UpdateControl();
}