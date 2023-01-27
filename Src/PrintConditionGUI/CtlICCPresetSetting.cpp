/*! \file CtlICCPresetSetting.cpp
 *  \brief ICC preset setting control
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlICCPresetSetting.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "DlgAddICCPreset.h"
#include "PrintConditionGUIDef.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICC_PRESET_ICON = 0,				//!< ICC preset icon
	CTRLID_PD_ICC_PRESET_SELECTION,				//!< ICC preset selection pull-down

	CTLID_COUNT
};
//! ICC Preset operation
#define DEF_ICC_PRESET_OPERATOR_ADD	1			//!< Add operator
#define DEF_ICC_START_INDEX			2			//!< 0 : Add..., 1 : Seperator
// ==========================================================================

/*! \brief constructor
*/
CCtlICCPresetSetting::CCtlICCPresetSetting()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlICCPresetSetting::~CCtlICCPresetSetting()
{
}


/*! \brief [event]set the control's property
*/
void CCtlICCPresetSetting::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static: ICC preset icon
	{
		long ctlId = CTRLID_SC_ICC_PRESET_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 220, 10 + 60, 220 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_ICCPRESET, RESOURCE_BMP);
	}

	//pull-down: ICC preset selection
	{
		long ctlId = CTRLID_PD_ICC_PRESET_SELECTION;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 220, 75 + 600, 220 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief [event] set the control's item
*/
void CCtlICCPresetSetting::OnCreateItem()
{
	// Registers items for ICC preset menu pull-down
	RegisterICCPresetItems();
}

/*! \brief Window procedure
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE
*/
long CCtlICCPresetSetting::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_UPDATE_ICC_PRESET_LIST:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

			switch(wParam){
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], 0)){
						SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], 0, NULL);
					}

					// re-regiseter all items of ICC Preset selection
					RegisterICCPresetItems();
					pData->SetChangeParam();
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
long CCtlICCPresetSetting::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// ICC Preset menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_ICC_PRESET_SELECTION]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION]);
				PDITEMINFO* pd_item = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], nSelection);

				if(pd_item) {
					if(NULL == pd_item->data) {
						// change the selection ICC preset
						pData->SetSelectICCPreset(nSelection - DEF_ICC_START_INDEX);

						if(!pData->IsExistName()) {
							if(0 != nSelection) {
								//remove the red inspection setting from the list
								pData->RemoveICCPresetNameNotExist();

								// Clear all items
								while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], 0)){
									SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], 0, NULL);
								}

								// re-regiseter all items of ICC Preset selection
								RegisterICCPresetItems();

								// item is removed, so decrease the index
								pData->SetSelectICCPreset(nSelection - 1 - DEF_ICC_START_INDEX);
							}
						}
					}
					//User click Add... item
					else if(DEF_ICC_PRESET_OPERATOR_ADD == (long)pd_item->data) {
						//Show add ICC preset dialog
						DlgAddICCPreset addDlg;
						addDlg.Create(m_data, m_printer);
					}
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief Control state update event
*/
void CCtlICCPresetSetting::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			DWORD dwState = CST_SHOW;

			// IPDSモードの場合、非表示
			if (pData->IsIpdsMode() == TRUE) {
				dwState = CST_HIDE;
			}

			switch(ctlId)
			{
			case CTRLID_SC_ICC_PRESET_ICON:
				break;
			case CTRLID_PD_ICC_PRESET_SELECTION:
				{
					if(!pData->IsStartFromSystemSetting()			// When dialog show from Job List Screen
						|| pData->IsDisablePrintCondition()			// When General mode and editing print condition is prohibited
						|| pData->GetLockPrintCondtion())			// When the selection print settings is locked the edit.
					{
						dwState |= CST_DISABLED;
					}else {
						dwState |= CST_ENABLED;
					}
				}
				break;
			}

			SetControlState(m_ctl[ctlId], dwState);
		}
	}
}

/*! \brief Control value update event
*/
void CCtlICCPresetSetting::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to ICC preset pull-down menu
	long nSelection = 0;
	
	if(pData->GetSelectICCPreset(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], nSelection + DEF_ICC_START_INDEX);
	}
}

/*! \brief Register all items for ICC preset pull-down
*/
void CCtlICCPresetSetting::RegisterICCPresetItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	if (pData->IsIpdsMode()) {
		return; // nothing to do
	}
	PDITEMINFO pd_item = {0};
	long nItemID = 0;

	//	Add item / Separator
	{
		// add an add operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_ADD, RESOURCE_STR);
		pd_item.data = (void*)DEF_ICC_PRESET_OPERATOR_ADD;
		SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], nItemID++, &pd_item);

		// add a seperator
		char chSeparator[4] = {0};
		_snprintf_s(chSeparator, sizeof(chSeparator), _TRUNCATE, "-");

		pd_item.text = chSeparator;
		pd_item.data = NULL;
		SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], nItemID++, &pd_item);
	}

	//All ICC preset list
	{
		const char* strIccPreset = NULL;
		long iccIndex = 0;
		// Get ICC preset items
		while(pData->GetICCPresetList(iccIndex++, &strIccPreset)){
			pd_item.text = (char*)strIccPreset;
			SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], nItemID++, &pd_item);
			strIccPreset = NULL;
		}
	}

	if(!pData->IsExistName()) {
		const char* strICCPresetTop = NULL;
		pData->GetICCPresetList(0, &strICCPresetTop);

		PDITEMINFO pd_item = {0};
		pd_item.crTextEnabled = TRUE;
		pd_item.text = (char*)strICCPresetTop;
		//Set text color to red
		pd_item.crText = DEF_COLOR_RED;
		SetControlItem(m_ctl[CTRLID_PD_ICC_PRESET_SELECTION], DEF_ICC_START_INDEX, &pd_item);
	}
}