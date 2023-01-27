/*! \file CtlPrintHeadAlignSelection.cpp
 *  \brief The control selects a print head alignment of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintHeadAlignSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

#include "DlgDeletePrintHeadAlignment.h"
#include "DlgAddPrintHeadAlignment.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< Static-box: Print head alignment icon
	CTRLID_PD_PRINT_HEAD_ALIGNMENT,		//!< Pull-down: Print head alignment selection

	CTLID_COUNT
};

//! Print head alignment operation
typedef enum{
	DEF_PRINT_HEAD_OPERATOR_ADD = 1,		//!< Add operator
	DEF_PRINT_HEAD_OPERATOR_DELETE			//!< Delete operator
}DEF_PRINT_HEAD_OPERATION;

// ==========================================================================

/*! \brief constructor
*/
CCtlPrintHeadAlignSelection::CCtlPrintHeadAlignSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
	m_PrintHeadAlignStartIndex = 0;
}

/*! \brief destructor
*/
CCtlPrintHeadAlignSelection::~CCtlPrintHeadAlignSelection()
{
}


/*! \brief [event]set the control's property
*/
void CCtlPrintHeadAlignSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static: print head alignment icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 80, 10 + 60, 80 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINT_HEAD_ALIGNMENT, RESOURCE_BMP);
	}

	//pull-down: print head alignment selection
	{
		long ctlId = CTRLID_PD_PRINT_HEAD_ALIGNMENT;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 80, 75 + 600, 80 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrintHeadAlignSelection::OnCreateItem()
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
long CCtlPrintHeadAlignSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_PRINT_HEAD_ALIGNMENT_LIST:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

			PDITEMINFO pd_item = {0};

			const char* strAlignment = NULL;

			switch(wParam){
			case CDataIF::DEF_UPDATE_ADD_ITEM:
				{
					// add the item
					if(TRUE == pData->GetPrintHeadAlignmentList(lParam, &strAlignment)){
						pd_item.text = (char*)strAlignment;

						SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], m_PrintHeadAlignStartIndex + lParam, &pd_item);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_EDIT_ITEM:
				{
					// change the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], lParam);
					if(selectedItem){
						if(TRUE == pData->GetPrintHeadAlignmentList(lParam, &strAlignment)){
							selectedItem->text = (char*)strAlignment;

							// change and update item at selected index
							SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], m_PrintHeadAlignStartIndex + lParam, selectedItem);
						}
					}
				}
				break;
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// delete the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], lParam);
					if(selectedItem){
						// delete the selected item
						SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], m_PrintHeadAlignStartIndex + lParam, NULL);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], 0)){
						SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], 0, NULL);
					}

					// re-regiseter all items of print head alignment
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
long CCtlPrintHeadAlignSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Print head alignment menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT]){
				// Get the current selection item
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT]);
				PDITEMINFO* pd_item = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], nSelection);

				if(pd_item){
					if(NULL == pd_item->data){
						// Set the selection print head alignment to DataIF
						pData->SetSelectPrintHeadAlignment(nSelection - m_PrintHeadAlignStartIndex);
					}
					else{
						long nOperation = (long)pd_item->data;

						// if user click on Add.. item
						if(DEF_PRINT_HEAD_OPERATOR_ADD == nOperation){
							// Notify to display Add print head alignment dialog
							pData->InitAddPrintHeadAlignmentDlg();

							// Display dialog
							DlgAddPrintHeadAlignment dlg;
							dlg.Create(m_data, m_printer);
						}
						// if user click on Delete item
						else if(DEF_PRINT_HEAD_OPERATOR_DELETE == nOperation){
							// Notify to display Delete print head alignment dialog
							pData->InitDeletePrintHeadAlignmentDlg();

							// Display dialog
							DlgDeletePrintHeadAlignment dlg;
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
void CCtlPrintHeadAlignSelection::OnUpdateState()
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

		SetControlState(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrintHeadAlignSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print mode pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectPrintHeadAlignment(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], m_PrintHeadAlignStartIndex + nSelection);
	}
}

/*! \brief Register all items for Print head alignment pull-down
*/
void CCtlPrintHeadAlignSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long nItemId = 0;

	// Add/Delete/Seperator item
	{
		PDITEMINFO pd_item = {0};
		pd_item.no_query = TRUE;

		// add an add operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_ADD, RESOURCE_STR);
		pd_item.data = (void*)DEF_PRINT_HEAD_OPERATOR_ADD;
		SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], nItemId++, &pd_item);

		// add a delete operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_DELETE, RESOURCE_STR);
		pd_item.data = (void*)DEF_PRINT_HEAD_OPERATOR_DELETE;
		SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], nItemId++, &pd_item);


		// add a seperator
		char chSeparator[4] = {0};
		_snprintf(chSeparator, sizeof(chSeparator) - 1, "-");

		pd_item.text = chSeparator;
		pd_item.data = NULL;
		SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], nItemId++, &pd_item);
	}

	// All Print head alignment list
	{
		// Backup the start index of Print head alignment list
		m_PrintHeadAlignStartIndex = nItemId;

		PDITEMINFO pd_item = {0};

		const char* strAlignment = NULL;
		long printHeadId = 0;

		// Get the print head alignment list
		while(true == pData->GetPrintHeadAlignmentList(printHeadId++, &strAlignment)){
			// Register item to pull-down
			pd_item.text = (char*)strAlignment;
			SetControlItem(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], nItemId++, &pd_item);

			strAlignment = NULL;
		}
	}

	// Get the selected ID
	long selectedID = 0;
	if (pData->GetSelectPrintHeadAlignment(&selectedID))
	{
		SetControlData(m_ctl[CTRLID_PD_PRINT_HEAD_ALIGNMENT], m_PrintHeadAlignStartIndex + selectedID);
	}
}