/*! \file CtlInspectionSettingSelection.h
 *  \brief The Inspection setting pull down menu
 *  \author GCS Manh Nguyen
 *  \date 2013/12/14 GCS Manh Nguyen created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlInspectionSettingSelection.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! the control id list
enum{
	CTRLID_SC_ICON = 0,				//!< Static-box: Inspection setting icon
	CTRLID_PD_INSPECTION_SETTING,	//!< Pull-down: Inspection setting selection

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlInspectionSettingSelection::CCtlInspectionSettingSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}


CCtlInspectionSettingSelection::~CCtlInspectionSettingSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlInspectionSettingSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Static: Dryer settings icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 610, 10 + 60, 610 + 60);
		// TODO: load correct icon
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_JET_INSPECTION, RESOURCE_BMP);
	}

	// Pull-down: Dryer settings selection
	{
		long ctlId = CTRLID_PD_INSPECTION_SETTING;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 610, 75 + 500, 610 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlInspectionSettingSelection::OnCreateItem()
{
	// Registers items for print head alignment menu pull-down
	RegisterItems();
}

/*! \brief Window procedure
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE
*/
long CCtlInspectionSettingSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_UPDATE_JET_INSPECTION_LIST:
		{
			switch(wParam){
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_INSPECTION_SETTING], 0)){
						SetControlItem(m_ctl[CTRLID_PD_INSPECTION_SETTING], 0, NULL);
					}

					// re-regiseter all items of Jet Inspection
					RegisterItems();
				}
				break;
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
long CCtlInspectionSettingSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Inspection setting menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_INSPECTION_SETTING]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_INSPECTION_SETTING]);
				pData->SetSelectJetInspection(nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlInspectionSettingSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		DWORD dwState = CST_SHOW;

		// Hide the control when system is not support Jet Inspection
		if (!pData->IsSystemSupportJetInspection()) {
			for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
				SetControlState(m_ctl[ctlId], CST_HIDE);
			}
		} else {
			SetControlState(m_ctl[CTRLID_SC_ICON], CST_SHOW);
			if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
				// Disable control when the selected print settings is locked.
				dwState |= CST_DISABLED;
				SetControlState(m_ctl[CTRLID_PD_INSPECTION_SETTING], dwState);
			} else {
				dwState |= CST_ENABLED;
				SetControlState(m_ctl[CTRLID_PD_INSPECTION_SETTING], dwState);
			}
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlInspectionSettingSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print mode pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectJetInspection(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_INSPECTION_SETTING], nSelection);
	}
}

/*! \brief Register all items for Dryer settings pull-down
*/
void CCtlInspectionSettingSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long nItemID = 0;
	const char* strJetInspection = NULL;

	// Check Jet Inspection setting of Print Condition is existed in the system Jet Inspection list
	if (pData->GetJetInspectionList(nItemID, &strJetInspection))
	{
		PDITEMINFO tmpPDItem = {0};
		tmpPDItem.text = (char*)strJetInspection;

		if (false == pData->IsJIExistInJIList())
		{
			tmpPDItem.crTextEnabled = TRUE;
			tmpPDItem.crText = RGB(255, 0, 0);
		}

		SetControlItem(m_ctl[CTRLID_PD_INSPECTION_SETTING], nItemID++, &tmpPDItem);
	}

	PDITEMINFO pd_item = {0};
	while (pData->GetJetInspectionList(nItemID, &strJetInspection))
	{
		pd_item.text = (char*)strJetInspection;

		SetControlItem(m_ctl[CTRLID_PD_INSPECTION_SETTING], nItemID++, &pd_item);
	}

	// Get the current selection item
	long nSelectionID = 0;
	if (pData->GetSelectJetInspection(&nSelectionID))
	{
		SetControlData(m_ctl[CTRLID_PD_INSPECTION_SETTING], nSelectionID);
	}
}