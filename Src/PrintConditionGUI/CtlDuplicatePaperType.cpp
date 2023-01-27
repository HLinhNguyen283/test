/*! \file CtlDuplicatePaperType.cpp
 *  \brief duplicate paper type for adding paper type
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDuplicatePaperType.h"
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
CCtlDuplicatePaperType::CCtlDuplicatePaperType()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlDuplicatePaperType::~CCtlDuplicatePaperType()
{
}


/*! \brief [event]set the control's property
*/
void CCtlDuplicatePaperType::OnSetAttribute()
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

	//pull-down: paper type
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
void CCtlDuplicatePaperType::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// registers all paper type items
	PDITEMINFO pd_item = {0};
	long lItemId = 0;
	const char* strPaperTypeName = NULL;
	while(pData->GetPaperTypeList_AddDlg(lItemId, &strPaperTypeName)){

		// add a paper type name
		pd_item.text = (char*)strPaperTypeName;
		SetControlItem(m_ctl[CTRLID_PD_DUPLICATE], lItemId++, &pd_item);

		// reset data
		strPaperTypeName = NULL;
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
long CCtlDuplicatePaperType::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// duplicate paper type name menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_DUPLICATE]){
				// change the selection paper type name
				long lItemID = GetControlData(m_ctl[CTRLID_PD_DUPLICATE]);
				pData->SetSelectPaperType_AddDlg(lItemID);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDuplicatePaperType::OnUpdateState()
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
void CCtlDuplicatePaperType::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	// display the selection paper type name
	long lItemID = 0;
	if(pData->GetSelectPaperType_AddDlg(&lItemID)){
		SetControlData(m_ctl[CTRLID_PD_DUPLICATE], lItemID);
	}
}
