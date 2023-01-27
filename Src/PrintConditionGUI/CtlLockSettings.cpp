/*! \file CtlLockSettings.cpp
 *  \brief The control locks to edit settings of the selection print condition.
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlLockSettings.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! the control id list
enum{
	CTRLID_BN_LOCK = 0,				//!< button: lock

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_lock = "CTRLID_BN_LOCK";
// ==========================================================================

/*! \brief constructor
*/
CCtlLockSettings::CCtlLockSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlLockSettings::~CCtlLockSettings()
{
}

/*! \brief [event]set the control's property
*/
void CCtlLockSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button：close screen
	{
		long ctlId = CTRLID_BN_LOCK;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION | BNST_STICKY | BNST_PUSH_IMAGE_SET;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BUTTON_LOCK, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 45, 790, 45 + 80, 790 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_LOCK, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_lock;
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
long CCtlLockSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam){
	case BN_CLICKED:
		{
			// Lock settings button
			if(ctlWnd == m_ctl[CTRLID_BN_LOCK]){
				long isLock = GetControlData(m_ctl[CTRLID_BN_LOCK]);
				// Notify to DataIF
				pData->SetLockPrintCondtion(isLock == 0 ? false : true);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlLockSettings::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Always show control
		DWORD dwState = CST_SHOW;

		// The user mode of the system is General
		if(USER_ID_GENERAL == pData->GetSystemUserMode()){
//			// Disable the control
//			dwState |= CST_DISABLED;
			// Manage the control according to access rights setting
			if(pData->IsStartFromSystemSetting()){
				if(pData->IsPermissionDeletePrintCondition() || pData->IsPermissionAddPrintCondition())
					dwState |= CST_ENABLED;
				else
					dwState |= CST_DISABLED;
			} else {
				if(pData->IsPermissionEditJobPrintCondition())
					dwState |= CST_ENABLED;
				else
					dwState |= CST_DISABLED;
			}
		}else{
			dwState |= CST_ENABLED;
		}

		SetControlState(m_ctl[CTRLID_BN_LOCK], dwState);
	}
}

/*! \brief control value update event
*/
void CCtlLockSettings::OnUpdateValue()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Get lock state
		bool isLock = pData->GetLockPrintCondtion();

		long resourceID = IDB_UNLOCK_ICON;		// unlock icon
		// change icon of lock control
		if(isLock)
		{
			// set Lock icon 
			resourceID = IDB_LOCK;
		}

		BNITEMINFO lItemInfo = {0};
		lItemInfo.nMask = OT_BITMAP;
		lItemInfo.hImage = LoadResourceBitmap(resourceID, RESOURCE_BMP);
		SetControlItem(m_ctl[CTRLID_BN_LOCK],0,&lItemInfo);

		// Display the lock state
		SetControlData(m_ctl[CTRLID_BN_LOCK], isLock ? 1 : 0);
	}
}

