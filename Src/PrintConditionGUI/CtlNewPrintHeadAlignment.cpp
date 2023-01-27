/*! \file CtlNewPrintHeadAlignment.cpp
 *  \brief new print head alignment name for adding print head alignment
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlNewPrintHeadAlignment.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_MAX_LENGTH				64

//! the control id list
enum{
	CTRLID_ST_NAME_LABEL = 0,			//!< print head alignment name label
	CTRLID_EB_NAME,								//!< print head alignment name edit-box

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlNewPrintHeadAlignmentName::CCtlNewPrintHeadAlignmentName()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlNewPrintHeadAlignmentName::~CCtlNewPrintHeadAlignmentName()
{
}


/*! \brief [event]set the control's property
*/
void CCtlNewPrintHeadAlignmentName::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//label: print head alignment name
	{
		long ctlId = CTRLID_ST_NAME_LABEL;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_TEXT | SCST_LEFT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_LABEL_PRINT_SETTING_NAME, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 15, 35, 15 + 95, 35 + 40);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//edit-box: print head alignment name
	{
		long ctlId = CTRLID_EB_NAME;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL | EBST_IME;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 120,  25, 120 + 670, 25 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
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
long CCtlNewPrintHeadAlignmentName::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// print head alignment name edit-box
			if(ctlWnd == m_ctl[CTRLID_EB_NAME]){
				// change the print head alignment name
				char* strPrintHeadAlignmentName = (char*)GetControlData(m_ctl[CTRLID_EB_NAME]);
				pData->SetAddPrintHeadAlignmentName_AddDlg(strPrintHeadAlignmentName);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlNewPrintHeadAlignmentName::OnUpdateState()
{
	if(m_ctl){
		// always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlNewPrintHeadAlignmentName::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// display the print head alignment name
	const char* strPrintHeadAlignmentName = pData->GetAddPrintHeadAlignmentName_AddDlg();
	SetControlData(m_ctl[CTRLID_EB_NAME], (DWORD)strPrintHeadAlignmentName);
}