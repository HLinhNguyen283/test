/*! \file CtlPaperTypeSelection.cpp
 *  \brief The control selects a paper type of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPaperTypeSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

#include "DlgDeletePaperType.h"
#include "DlgAddPaperType.h"

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< Static-box: Paper type icon
	CTRLID_PD_PAPER_TYPE,				//!< Pull-down: Paper type selection

	CTLID_COUNT
};

//! Paper type operation
typedef enum{
	DEF_PAPER_TYPE_OPERATOR_ADD = 1,		//!< Add operator
	DEF_PAPER_TYPE_OPERATOR_DELETE			//!< Delete operator
}DEF_PAPER_TYPE_OPERATION;

// ==========================================================================

/*! \brief constructor
*/
CCtlPaperTypeSelection::CCtlPaperTypeSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
	m_PaperTypeStartIndex = -1;
}

/*! \brief destructor
*/
CCtlPaperTypeSelection::~CCtlPaperTypeSelection()
{
}


/*! \brief [event]set the control's property
*/
void CCtlPaperTypeSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static: Paper type icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 150, 10 + 60, 150 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_TYPE_ICON, RESOURCE_BMP);
	}

	//pull-down: Paper type selection
	{
		long ctlId = CTRLID_PD_PAPER_TYPE;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 75, 150, 75 + 600, 150 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPaperTypeSelection::OnCreateItem()
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
long CCtlPaperTypeSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_PAPER_TYPE_LIST:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

			PDITEMINFO pd_item = {0};

			const char* strPaperType = NULL;

			switch(wParam){
			case CDataIF::DEF_UPDATE_ADD_ITEM:
				{
					// add the item
					if(TRUE == pData->GetPaperTypeList(lParam, &strPaperType)){
						pd_item.text = (char*)strPaperType;

						SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], m_PaperTypeStartIndex + lParam, &pd_item);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_EDIT_ITEM:
				{
					// change the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], lParam);
					if(selectedItem){
						if(TRUE == pData->GetPaperTypeList(lParam, &strPaperType)){
							selectedItem->text = (char*)strPaperType;

							// change and update item at selected index
							SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], m_PaperTypeStartIndex + lParam, selectedItem);
						}
					}
				}
				break;
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// delete the item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], lParam);
					if(selectedItem){
						// delete the selected item
						SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], m_PaperTypeStartIndex + lParam, NULL);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], 0)){
						SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], 0, NULL);
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
long CCtlPaperTypeSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Paper type menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_PAPER_TYPE])
			{
				// Get the current selection item
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PAPER_TYPE]);
				PDITEMINFO* pd_item = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], nSelection);

				if(pd_item)
				{
					if(NULL == pd_item->data)
					{
						// Set the selection Paper type to DataIF
						pData->SetSelectPaperType(nSelection - m_PaperTypeStartIndex);
					}
					else
					{
						long nOperation = (long)pd_item->data;

						// if user click on Add.. item
						if(DEF_PAPER_TYPE_OPERATOR_ADD == nOperation)
						{
							// Notify to display Add paper type dialog
							pData->InitAddPaperTypeDlg();

							// Display dialog
							DlgAddPaperType dlg;
							dlg.Create(m_data, m_printer);
						}
						// if user click on Delete item
						else if(DEF_PAPER_TYPE_OPERATOR_DELETE == nOperation)
						{
							// Notify to display Delete paper type dialog
							pData->InitDeletePaperTypeDlg();

							// Display dialog
							DlgDeletePaperType dlg;
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
void CCtlPaperTypeSelection::OnUpdateState()
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

		SetControlState(m_ctl[CTRLID_PD_PAPER_TYPE], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPaperTypeSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	long nSelection = -1;

	// Get the selection paper type
	if(true == pData->GetSelectPaperType(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_PAPER_TYPE], m_PaperTypeStartIndex + nSelection);
	}
}

/*! \brief Register all items for Paper type pull-down
*/
void CCtlPaperTypeSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long nItemId = 0;

	// Add/Delete/Seperator item
	{
		PDITEMINFO pd_item = {0};
		pd_item.no_query = TRUE;

		// add an add operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_ADD, RESOURCE_STR);
		pd_item.data = (void*)DEF_PAPER_TYPE_OPERATOR_ADD;
		SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], nItemId++, &pd_item);

		// add a delete operator
		pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_DELETE, RESOURCE_STR);
		pd_item.data = (void*)DEF_PAPER_TYPE_OPERATOR_DELETE;
		SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], nItemId++, &pd_item);


		// add a seperator
		char chSeparator[4] = {0};
		_snprintf(chSeparator, sizeof(chSeparator) - 1, "-");

		pd_item.text = chSeparator;
		pd_item.data = NULL;
		SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], nItemId++, &pd_item);
	}

	// All Print head alignment list
	{
		// Backup the start index of Print head alignment list
		m_PaperTypeStartIndex = nItemId;

		PDITEMINFO pd_item = {0};

		const char* strPaperType = NULL;
		long printHeadId = 0;

		// Get the print head alignment list
		while(true == pData->GetPaperTypeList(printHeadId++, &strPaperType)){
			// Register item to pull-down
			pd_item.text = (char*)strPaperType;
			SetControlItem(m_ctl[CTRLID_PD_PAPER_TYPE], nItemId++, &pd_item);

			strPaperType = NULL;
		}
	}

	// Update the selection item
	long nSelectionID = 0;
	if (pData->GetSelectPaperType(&nSelectionID))
	{
		SetControlData(m_ctl[CTRLID_PD_PAPER_TYPE], m_PaperTypeStartIndex + nSelectionID);
	}
}