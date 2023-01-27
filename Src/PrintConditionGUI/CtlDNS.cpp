/*! \file CtlDNS.cpp
 *  \brief The control edits dynamic nozzle shift setting of the selected print settings on the screen.
 *  \author Screen Matsuda
 *  \date 2014/03/20 Screen Matsuda created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDNS.h"
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
	CTRLID_BN_DNS = 0,				//!< Button: Dynamic nozzle shift
	
	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_dns = "CTRLID_BN_DNS";
// ==========================================================================

/*! \brief constructor
*/
CCtlDNS::CCtlDNS()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlDNS::~CCtlDNS()
{
}

/*! \brief [event]set the control's property
*/
void CCtlDNS::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Button: Dynamic nozzle shift
	{
		long ctlId = CTRLID_BN_DNS;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_DNS, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 263, 445, 263 + 70, 445 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_DNS_ICON, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_dns;
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
long CCtlDNS::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Dynamic nozzle shift button
			if(ctlWnd == m_ctl[CTRLID_BN_DNS]){
				long isON = GetControlData(m_ctl[CTRLID_BN_DNS]);

				// Notify status of dynamic nozzle shift button to DataIF
				pData->SetDynamicNozzleShift(isON == 0 ? false : true);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDNS::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Display setting
		{
			DWORD dwState = CST_SHOW;
			if (!pData->IsExistDNS())	// Hide control when DNS is not exist.
			{
				dwState = CST_HIDE;
			}

			// Show all controls
			for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
				SetControlState(m_ctl[ctlID], dwState);
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
			SetControlState(m_ctl[CTRLID_BN_DNS], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlDNS::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Display the status of dynamic nozzle shift
	bool isOn = pData->GetDynamicNozzleShift();
	BNITEMINFO bn_item = {0};
	bn_item.bLED_On = isOn;

	SetControlItem(m_ctl[CTRLID_BN_DNS], 0, &bn_item);
	SetControlData(m_ctl[CTRLID_BN_DNS], (DWORD)isOn);
}
