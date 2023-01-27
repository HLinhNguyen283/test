/*! \file CtlOutputColorSelection.cpp
 *  \brief CtlOutputColorSelection.cppの処理
 *  \author SCREEN
 *  \date 2017/11/20 SCREEN 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlOutputColorSelection.h"
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
	CTRLID_PD_OUTPUTCOLOR,				//!< Pull-down: Output color selection

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlOutputColorSelection::CCtlOutputColorSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlOutputColorSelection::~CCtlOutputColorSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlOutputColorSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// pull-down: Output color selection
	{
		long ctlId = CTRLID_PD_OUTPUTCOLOR;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_BITMAP;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 760, 220, 760 + 135, 220 + 70);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlOutputColorSelection::OnCreateItem()
{
	// Registers items for print speed menu pull-down
	RegisterOutputColorItems();
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlOutputColorSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Output color selection menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_OUTPUTCOLOR]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_OUTPUTCOLOR]);

				// change the selection output color
				pData->SetSelectOutputColor(nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlOutputColorSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Valid setting
		DWORD dwState = CST_SHOW;

		// hide control when a printer do not have multi color option.
		if (!pData->IsExistMultiColor())
		{
			dwState = CST_HIDE;
		}
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], dwState);
		}

		// Disable control
		//  - when the selected print settings is locked.
		//  - when the print settings is not permitted to edit.
		//  - when dialog show from Job List Screen
		if(true == pData->GetLockPrintCondtion()
			|| true == pData->IsDisablePrintCondition()
			|| false == pData->IsStartFromSystemSetting()){
			dwState |= CST_DISABLED;
		} else {
			dwState |= CST_ENABLED;
		}

		SetControlState(m_ctl[CTRLID_PD_OUTPUTCOLOR], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlOutputColorSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print speed pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectOutputColor(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_OUTPUTCOLOR], nSelection);
	}
}

/*! \brief Register all items for Output color selection pull-down
*/
void CCtlOutputColorSelection::RegisterOutputColorItems()
{
	HBITMAP src[2] = {0};
	src[0] = LoadResourceBitmap(IDB_OUTPUTCOLOR_CMYK, RESOURCE_BMP);
	src[1] = LoadResourceBitmap(IDB_OUTPUTCOLOR_CMYKRO, RESOURCE_BMP);
	
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set the output color value to pull-down
	for(long value = 0; value < 2; ++value){
		PDITEMINFO pd_item = {0};
		pd_item.hImage = src[value];
		SetControlItem(m_ctl[CTRLID_PD_OUTPUTCOLOR], value, &pd_item);
	}

	OnUpdateValue();
}
