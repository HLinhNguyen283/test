/*! \file CtlPrintModeSelection.cpp
 *  \brief Print Mode Selection control
 *  \author GCS Tan Ho
 *  \date 2013/05/13 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintModeSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_PRINT_MODE_ICON = 0,				//!< Print mode icon
	CTRLID_PD_PRINT_MODE_SELECTION,				//!< Print mode selection pull-down

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlPrintModeSelection::CCtlPrintModeSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPrintModeSelection::~CCtlPrintModeSelection()
{
}


/*! \brief [event]set the control's property
*/
void CCtlPrintModeSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static: print mode icon
	{
		long ctlId = CTRLID_SC_PRINT_MODE_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 10, 10 + 60, 10 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINT_MODE, RESOURCE_BMP);
	}

	//pull-down: print mode selection
	{
		long ctlId = CTRLID_PD_PRINT_MODE_SELECTION;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 10, 75 + 600, 10 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrintModeSelection::OnCreateItem()
{
	// Registers items for print mode menu pull-down
	RegisterPrintModeItems();
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlPrintModeSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_PRINT_MODE_LIST:
		{
			if(wParam == CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM){
				// Delete all item
				while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], 0)){
					SetControlItem(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], 0, NULL);
				}

				// Reregister the all items of printing speed
				RegisterPrintModeItems();
			}
		}

		break;
	}

	return DEF_NONE;
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPrintModeSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Print mode menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_PRINT_MODE_SELECTION]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION]);

				// change the selection print mode
				pData->SetSelectPrintMode(nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPrintModeSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}

		// the effective for print mode selection pull-down menu
		DWORD dwState = CST_ENABLED;

		// Disable control when the selected print settings is locked.
		if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
			dwState = CST_DISABLED;
		}
		// Disable control when dialog show from Job List Screen
		else if(!pData->IsStartFromSystemSetting())
		{
			dwState = CST_DISABLED;
		}
		SetControlState(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrintModeSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print mode pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectPrintMode(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], nSelection);
	}
	else{
		SetControlData(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], -1);
	}
}

/*! \brief Register all items for Print mode pull-down
*/
void CCtlPrintModeSelection::RegisterPrintModeItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	PDITEMINFO pd_item = {0};

	long nItemID = 0;
	const char* strPrintMode = NULL;

	// Get print mode items
	while(true == pData->GetPrintModeList(nItemID, &strPrintMode)){
		pd_item.text = (char*)strPrintMode;

		SetControlItem(m_ctl[CTRLID_PD_PRINT_MODE_SELECTION], nItemID++, &pd_item);

		strPrintMode = NULL;
	}

	OnUpdateValue();
}