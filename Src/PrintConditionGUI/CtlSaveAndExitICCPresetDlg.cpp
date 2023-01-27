/*! \file CtlSaveAndExitICCPresetDlg.cpp
 *  \brief Commit new ICC preset name control
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlSaveAndExitICCPresetDlg.h"
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
CCtlSaveAndExitICCPresetDlg::CCtlSaveAndExitICCPresetDlg()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlSaveAndExitICCPresetDlg::~CCtlSaveAndExitICCPresetDlg()
{
}

/*! \brief [event]set the control's property
*/
void CCtlSaveAndExitICCPresetDlg::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button: save
	{
		long ctlId = CTRLID_BN_SAVE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SAVE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 25, 165, 25 + 145, 165 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_SAVE, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//button: cancel
	{
		long ctlId = CTRLID_BN_CANCEL;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BUTTON_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 415, 165, 415 + 145, 165 + 70);
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
long CCtlSaveAndExitICCPresetDlg::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				if(pData->IsAddICCPresetNameExist()) {
					//display a message to select different name
					char msg[256] = {0};
					_snprintf_s(msg, sizeof(msg), _TRUNCATE, "%d\n%s", IDM_SELECT_DIFFERENT_NAME + ID_MESSAGE_ERROR, LoadResourceString(IDM_SELECT_DIFFERENT_NAME, RESOURCE_MSG_STR));
					ShowMessageBox(msg, MBST_ICONWARNING | MBST_OK, NULL);

					//abort the save operation
					pData->CancelAddICCPresetNameDlg();

					return DEF_NONE;
				}else {
					// notify to save parameter
					if(pData->AddSystemICCPreset()){
						// close dialog
						return DEF_EXIT_PAGE;
					}
				}
			}
			// cancel button
			else if(ctlWnd == m_ctl[CTRLID_BN_CANCEL]){
				// notify to cancel to add parameter
				pData->CancelAddICCPresetNameDlg();
				// close dialog
				return DEF_EXIT_PAGE;
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief Control state update event
*/
void CCtlSaveAndExitICCPresetDlg::OnUpdateState()
{
	if(m_ctl){
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			DWORD dwState = CST_SHOW;

			switch(ctlId)
			{
			case CTRLID_BN_SAVE:
				{
					CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
					const char* strICCPresetName = pData->GetAddICCPresetNameDlg();

					if(NULL == strICCPresetName || 0 == strlen(strICCPresetName)) {
						dwState |= CST_DISABLED;
					}else {
						dwState |= CST_ENABLED;
					}
				}
				break;
			case CTRLID_BN_CANCEL:
				// always show this control
				break;
			}

			SetControlState(m_ctl[ctlId], dwState);
		}
	}
}