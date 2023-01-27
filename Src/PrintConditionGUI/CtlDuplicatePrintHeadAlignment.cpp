/*! \file CtlDuplicatePrintHeadAlignment.cpp
 *  \brief duplicate print head alignment name control for adding print head alignment
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDuplicatePrintHeadAlignment.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
//! the control id list
enum{
	CTRLID_ST_NAME = 0,					//!< label
	CTRLID_PD_DUPLICATE,				//!< duplicate print head alignment name menu pull-down

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlDuplicatePrintHeadAlignment::CCtlDuplicatePrintHeadAlignment()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlDuplicatePrintHeadAlignment::~CCtlDuplicatePrintHeadAlignment()
{
}


/*! \brief [event]set the control's property
*/
void CCtlDuplicatePrintHeadAlignment::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//label: name
	{
		long ctlId = CTRLID_ST_NAME;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_TEXT | SCST_LEFT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_LABLE_PRINT_SETTING_NAME_DUPLICATE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 15, 105, 15 + 95, 105 + 40);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//pull-down: print head alignment
	{
		long ctlId = CTRLID_PD_DUPLICATE;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 120,  95, 120 + 670, 95 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlDuplicatePrintHeadAlignment::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// registers all print head alignment items
	PDITEMINFO pd_item = {0};
	long lItemId = 0;
	const char* strPrintHeadAlignmentName = NULL;
	while(pData->GetPrintHeadAlignmentList_AddDlg(lItemId, &strPrintHeadAlignmentName)){
		// add a print head alignment name
		pd_item.text = (char*)strPrintHeadAlignmentName;
		SetControlItem(m_ctl[CTRLID_PD_DUPLICATE], lItemId++, &pd_item);

		// reset data
		strPrintHeadAlignmentName = NULL;
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
long CCtlDuplicatePrintHeadAlignment::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// duplicate print head alignment name menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_DUPLICATE]){
				// change the selection print head alignment name
				long lItemID = GetControlData(m_ctl[CTRLID_PD_DUPLICATE]);
				pData->SetSelectPrintHeadAlignment_AddDlg(lItemID);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDuplicatePrintHeadAlignment::OnUpdateState()
{
	if(m_ctl){
		// always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlDuplicatePrintHeadAlignment::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	// display the selection print head alignment name
	long lItemID = 0;
	if(pData->GetSelectPrintHeadAlignment_AddDlg(&lItemID)){
		SetControlData(m_ctl[CTRLID_PD_DUPLICATE], lItemID);
	}
}