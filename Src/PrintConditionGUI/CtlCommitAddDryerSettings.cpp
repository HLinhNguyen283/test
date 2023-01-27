/*! \file CtlCommitAddDryerSettings.cpp
 *  \brief The control commit/cancel to add the dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlCommitAddDryerSettings.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_W_BUTTON			130
#define DEF_H_BUTTON			60

//! the control id list
enum{
	CTRLID_BN_SAVE = 0,			//!< button: save
	CTRLID_BN_CLOSE,			//!< button: close

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_ctl[CTLID_COUNT] = {"CTRLID_BN_SAVE", "CTRLID_BN_CLOSE"};
// ==========================================================================

/*! \brief constructor
*/
CCtlCommitAddDryerSettings::CCtlCommitAddDryerSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

	m_ctrlOffset.x = 0;
	m_ctrlOffset.y = 0;
}

/*! \brief destructor
*/
CCtlCommitAddDryerSettings::~CCtlCommitAddDryerSettings()
{
}

/*! \brief [function]set the personal control's property
*/
void CCtlCommitAddDryerSettings::SetPersonalInfo(POINT inOffset)
{
	m_ctrlOffset = inOffset;
}

/*! \brief [event]set the control's property
*/
void CCtlCommitAddDryerSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button: save
	{
		long ctlId = CTRLID_BN_SAVE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SAVE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 0, 0, DEF_W_BUTTON, DEF_H_BUTTON);
		OffsetRect(&m_ctlAttribute[ctlId].rect, m_ctrlOffset.x,  m_ctrlOffset.y);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_SAVE, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//button: close
	{
		long ctlId = CTRLID_BN_CLOSE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 150, 0, 150 + DEF_W_BUTTON, DEF_H_BUTTON);
		OffsetRect(&m_ctlAttribute[ctlId].rect, m_ctrlOffset.x,  m_ctrlOffset.y);
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
long CCtlCommitAddDryerSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case BN_CLICKED:
		{
			// Save button
			if(ctlWnd == m_ctl[CTRLID_BN_SAVE]){
				// Notify to save parameter
				if(pData->SaveAddDryerSettingDlg()){
					// Close the dialog
					return DEF_EXIT_PAGE;
				}
			}
			// Exit button
			else if(ctlWnd == m_ctl[CTRLID_BN_CLOSE]){
				if(false == pData->IsSavedDryerSetting_AddDlg()){
					char msg[256];
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_DISCARD_CHANGES_CONFIRMATION, LoadResourceString(IDM_DISCARD_CHANGES_CONFIRMATION, RESOURCE_MSG_STR));

					if(IDNO == ShowMessageBox(msg, MBST_YESNO | MBST_ICONINFORMATION, NULL)){
						return DEF_NONE;
					}
				}
				
				// Notify to cancel the dialog to DataIF
				if(true == pData->CancelAddDryerSettingDlg()){
					return DEF_EXIT_PAGE;
				}
			}
		}
		break;
	}

	return DEF_NONE;
}
