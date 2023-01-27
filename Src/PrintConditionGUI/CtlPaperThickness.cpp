/*! \file CtlPaperThickness.cpp
 *  \brief The control edits a paper thickness of the selection print settings on the screen.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPaperThickness.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_MAX_LENGTH				4

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,				//!< Static-box: icon
	CTRLID_EB_THICKNESS,			//!< Edit-box: Thickness
	CTRLID_SC_UNIT,					//!< Static-box: unit
	
	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlPaperThickness::CCtlPaperThickness()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPaperThickness::~CCtlPaperThickness()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPaperThickness::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Static-box: icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 445, 10 + 60, 445 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_THICKNESS_ICON, RESOURCE_BMP);
	}

	// Edit-box: Thickness
	{
		long ctlId = CTRLID_EB_THICKNESS;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 80, 445, 80 + 130, 445 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// Static-box: unit
	{
		long ctlId = CTRLID_SC_UNIT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_UNIT_MM, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 215, 455, 215 + 45, 455 + 45);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPaperThickness::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Paper thickness edit-box
			if(ctlWnd == m_ctl[CTRLID_EB_THICKNESS]){
				// Change the paper thickness
				long thickness = (long)(atof((char*)GetControlData(m_ctl[CTRLID_EB_THICKNESS])) * 1000);
				pData->SetPaperThickness(thickness);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPaperThickness::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Display setting
		{
			// Show all controls
			for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
				SetControlState(m_ctl[ctlID], CST_SHOW);
			}
		}

		// Valid setting
		{
			DWORD dwState = CST_ENABLED;

			// Disable control when the selected print settings is locked.
			if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
				dwState = CST_DISABLED;
			}

			// Set state for edit-box
			SetControlState(m_ctl[CTRLID_EB_THICKNESS], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPaperThickness::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Display the paper thickness value
	double thickness = (double)pData->GetPaperThickness() / 1000;
	char strThickness[256] = { 0 };
	_snprintf_s(strThickness, _TRUNCATE, "%0.2f", thickness);
	SetControlData(m_ctl[CTRLID_EB_THICKNESS], (DWORD)strThickness);
}
