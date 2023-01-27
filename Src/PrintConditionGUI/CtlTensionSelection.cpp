/*! \file CtlTensionSelection.cpp
 *  \brief The control selects a tension of the selection print settings on the screen.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlTensionSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,				//!< Static-box: Tension icon
	CTRLID_PD_TENSION,				//!< Pull-down: Tension selection

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlTensionSelection::CCtlTensionSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlTensionSelection::~CCtlTensionSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlTensionSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: Tension icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 980, 10, 980 + 60, 10 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_TENSION_ICON, RESOURCE_BMP);
	}

	// pull-down: Tension selection
	{
		long ctlId = CTRLID_PD_TENSION;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 1045, 10, 1045 + 125, 10 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlTensionSelection::OnCreateItem()
{
	// Registers items for print speed menu pull-down
	RegisterTensionItems();
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlTensionSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case CBN_SELCHANGE:
		{
			// Tension selection menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_TENSION]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_TENSION]);

				// change the selection tension
				pData->SetSelectPaperTension(nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlTensionSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}

		// Valid setting
		DWORD dwState = CST_ENABLED;

		// Disable control when the selected print settings is locked.
		if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
			dwState = CST_DISABLED;
		}

		SetControlState(m_ctl[CTRLID_PD_TENSION], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlTensionSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print speed pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectPaperTension(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_TENSION], nSelection);
	}
}

/*! \brief Register all items for Tension selection pull-down
*/
void CCtlTensionSelection::RegisterTensionItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	PDITEMINFO pd_item = {0};

	long min = 0;				//!< min value of tension
	long max = 0;				//!< max value of tension
	long nItemID = 0;

	// Get the paper tension range
	pData->GetPaperTensionRange(&min, &max);
	char strTension[5] = {0};

	// Set the tension value to pull-down
	for(long value = min; value <= max; ++value){
		_snprintf(strTension, 4, "%ld", value);
		pd_item.text = (char*)strTension;

		SetControlItem(m_ctl[CTRLID_PD_TENSION], nItemID++, &pd_item);
	}

}