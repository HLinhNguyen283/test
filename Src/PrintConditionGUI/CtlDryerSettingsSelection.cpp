/*! \file CtlDryerSettingsSelection.cpp
 *  \brief The control selects settings of a dryer settings for printing.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDryerSettingsSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

#include "DlgAddDryerSettings.h"
#include "DlgAddDryerSettingsWithNir.h"
#include "DlgDeleteDryerSettings.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< Static-box: Dryer settings icon
	CTRLID_PD_DRYER_SETTINGS,			//!< Pull-down: Dryer settings selection

	CTLID_COUNT
};

//! Dryer settings operation
typedef enum{
	DEF_DRYER_SETTINGS_OPERATOR_ADD = 1,		//!< Add operator
	DEF_DRYER_SETTINGS_OPERATOR_DELETE			//!< Delete operator
}DEF_DRYER_SETTINGS_OPERATION;

// ==========================================================================

/*! \brief constructor
*/
CCtlDryerSettingsSelection::CCtlDryerSettingsSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
	m_DryerSettingsStartIndex = 0;
}

/*! \brief destructor
*/
CCtlDryerSettingsSelection::~CCtlDryerSettingsSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlDryerSettingsSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Static: Dryer settings icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 290, 10 + 60, 290 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_DRYER_SETTINGS_ICON, RESOURCE_BMP);
	}

	// Pull-down: Dryer settings selection
	{
		long ctlId = CTRLID_PD_DRYER_SETTINGS;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 290, 75 + 600, 290 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlDryerSettingsSelection::OnCreateItem()
{
	// Registers items for print head alignment menu pull-down
	RegisterItems();
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlDryerSettingsSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_DRYER_SETTING_LIST:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

			PDITEMINFO pd_item = {0};

			const char* strDryerSetting = NULL;

			switch(wParam){
			case CDataIF::DEF_UPDATE_ADD_ITEM:
				{
					// add the item
					if(TRUE == pData->GetPrintDryerSettingList(lParam, &strDryerSetting)){
						pd_item.text = (char*)strDryerSetting;

						SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], m_DryerSettingsStartIndex + lParam, &pd_item);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_EDIT_ITEM:
				{
					// change the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], lParam);
					if(selectedItem){
						if(TRUE == pData->GetPrintDryerSettingList(lParam, &strDryerSetting)){
							selectedItem->text = (char*)strDryerSetting;

							// change and update item at selected index
							SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], m_DryerSettingsStartIndex + lParam, selectedItem);
						}
					}
				}
				break;
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// delete the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], lParam);
					if(selectedItem){
						// delete the selected item
						SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], m_DryerSettingsStartIndex + lParam, NULL);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], 0)){
						SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], 0, NULL);
					}

					// re-regiseter all items of paper type selection
					this->RegisterItems();
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
long CCtlDryerSettingsSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Dryer settings menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_DRYER_SETTINGS]){
				// Get the current selection item
				long nSelection = GetControlData(m_ctl[CTRLID_PD_DRYER_SETTINGS]);
				PDITEMINFO* pd_item = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], nSelection);

				if(pd_item){
					if(NULL == pd_item->data){
						// Set the selection Dryer setting to DataIF
						pData->SetSelectPrintDryerSetting(nSelection - m_DryerSettingsStartIndex);
					}
					else{
						long nOperation = (long)pd_item->data;

						// if user click on Add.. item
						if(DEF_DRYER_SETTINGS_OPERATOR_ADD == nOperation){
							// Display Add dryer settings dialog
							pData->InitAddDryerSettingDlg();

							// 2018.01.25
							// Precoater is not supported because it's non-product functiona and nir unit is supported.
#if 0
							// The print has a precoater
							if(true == pData->IsExistPrecoater()){
								// Display Add dryer setting dialog with precoat
								DlgAddDryerSettingsWithPrecoat dlg;
								dlg.Create(m_data, m_printer);
							}
#endif
							if((true == pData->IsExistNirDryerUnit(1)) || (true == pData->IsExistNirDryerUnit(2))){
								// Display Add dryer setting dialog with precoat
								DlgAddDryerSettingsWithNir dlg;
								dlg.Create(m_data, m_printer);
							}
							else{
								// Display Add dryer setting dialog without precoat
								DlgAddDryerSettings dlg;
								dlg.Create(m_data, m_printer);
							}
						}
						// if user click on Delete item
						else if(DEF_DRYER_SETTINGS_OPERATOR_DELETE == nOperation){
							// Notify to display Delete dryer settings dialog
							pData->InitDeleteDryerSettingDlg();

							// Display dialog
							DlgDeleteDryerSettings dlg;
							dlg.Create(m_data, m_printer);
						}
					}
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDryerSettingsSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		bool isDED = pData->IsDED();
		bool isHeater1 = pData->IsExistHeatRoller(DEF_PRINTER_1);
		bool isHeater2 = pData->IsExistHeatRoller(DEF_PRINTER_2);
		bool isNIR1 = pData->IsExistNirDryerUnit(DEF_PRINTER_1);
		bool isNIR2 = pData->IsExistNirDryerUnit(DEF_PRINTER_2);

		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			DWORD dwState = CST_SHOW;
			// If heaterless printer and NIR is not installed, hide heater controls
			if((!isDED && !isHeater1 && !isNIR1) || (isDED && !isHeater1 && !isHeater2 && !isNIR1 && !isNIR2)){
				dwState = CST_HIDE;
			}
			
			if(dwState == CST_SHOW){
				switch (ctlId)
				{
				case CTRLID_SC_ICON:
					dwState |= CST_ENABLED;
					break;
				case CTRLID_PD_DRYER_SETTINGS:
					// Disable control when the selected print settings is locked.
					if(pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
						dwState |= CST_DISABLED;
					}
					else{
						dwState |= CST_ENABLED;
					}
					break;
				}
			}
			SetControlState(m_ctl[ctlId], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlDryerSettingsSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		long selectionID = -1;
		// Get the selection item
		if(true == pData->GetSelectPrintDryerSetting(&selectionID)){
			// Display the selection item
			SetControlData(m_ctl[CTRLID_PD_DRYER_SETTINGS], m_DryerSettingsStartIndex + selectionID);
		}
	}
}

/*! \brief Register all items for Dryer settings pull-down
*/
void CCtlDryerSettingsSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long nItemId = 0;

	// Add/Delete/Seperator item
	{
		PDITEMINFO pd_item = {0};
		pd_item.no_query = TRUE;

		// add an add operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_ADD, RESOURCE_STR);
		pd_item.data = (void*)DEF_DRYER_SETTINGS_OPERATOR_ADD;
		SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], nItemId++, &pd_item);

		// add a delete operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_DELETE, RESOURCE_STR);
		pd_item.data = (void*)DEF_DRYER_SETTINGS_OPERATOR_DELETE;
		SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], nItemId++, &pd_item);


		// add a seperator
		char chSeparator[4] = {0};
		_snprintf(chSeparator, sizeof(chSeparator) - 1, "-");

		pd_item.text = chSeparator;
		pd_item.data = NULL;
		SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], nItemId++, &pd_item);
	}

	// All Dryer settings list
	{
		// Backup the start index of Dryer settings list
		m_DryerSettingsStartIndex = nItemId;

		PDITEMINFO pd_item = {0};

		const char* strDryerName = NULL;
		long printHeadId = 0;

		// Get the print head alignment list
		while(true == pData->GetPrintDryerSettingList(printHeadId++, &strDryerName)){
			// Register item to pull-down
			pd_item.text = (char*)strDryerName;
			SetControlItem(m_ctl[CTRLID_PD_DRYER_SETTINGS], nItemId++, &pd_item);

			strDryerName = NULL;
		}
	}

	long nSelectionID = 0;
	if (pData->GetSelectPrintDryerSetting(&nSelectionID))
	{
		SetControlData(m_ctl[CTRLID_PD_DRYER_SETTINGS], m_DryerSettingsStartIndex + nSelectionID);
	}
}