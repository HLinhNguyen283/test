/*! \file CtlSaveAndExitPrintSetting.cpp
 *  \brief commit add paper name for adding print setting
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlSaveAndExitPrintSetting.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================
//! the control id list
enum{
	CTRLID_BN_SAVE = 0,			//!< Save button
	CTRLID_BN_CANCEL,			//!< Cancel button

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_ctl[CTLID_COUNT] = {"CTRLID_BN_SAVE", "CTRLID_BN_CANCEL"};
// ==========================================================================

/*! \brief constructor
*/
CCtlSaveAndExitPrintSetting::CCtlSaveAndExitPrintSetting()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlSaveAndExitPrintSetting::~CCtlSaveAndExitPrintSetting()
{
}

/*! \brief [event]set the control's property
*/
void CCtlSaveAndExitPrintSetting::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button: save
	{
		long ctlId = CTRLID_BN_SAVE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SAVE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 500, 235, 500 + 125, 235 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_SAVE, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//button: cancel
	{
		long ctlId = CTRLID_BN_CANCEL;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BUTTON_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 650,  235, 650 + 125, 235 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_EXIT, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
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
long CCtlSaveAndExitPrintSetting::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	switch(wParam)
	{
	case BN_CLICKED:
		{
			// save button
			if(ctlWnd == m_ctl[CTRLID_BN_SAVE]){
				// notify to save parameter
				if(pData->SaveAddPrintSettingDlg()){
					// close dialog
					return DEF_EXIT_PAGE;
				}
			}
			// cancel button
			else if(ctlWnd == m_ctl[CTRLID_BN_CANCEL]){
				// notify to cancel to add parameter
				if(pData->CancelAddPrintSettingDlg()){
					// close dialog
					return DEF_EXIT_PAGE;
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlSaveAndExitPrintSetting::OnUpdateState()
{
	if(m_ctl){
		// always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}

		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		// valid setting for save button
		DWORD dwState = CST_ENABLED;

		// Get add print condition name
		const char* strPrintConditionName = pData->GetAddPrintConditionName_AddDlg();
		// the print setting name is non-input
		if(NULL == strPrintConditionName
			|| 0 == strlen(strPrintConditionName))
		{
			// disable the control
			dwState = CST_DISABLED;
		}
		SetControlState(m_ctl[CTRLID_BN_SAVE], dwState);
	}
}