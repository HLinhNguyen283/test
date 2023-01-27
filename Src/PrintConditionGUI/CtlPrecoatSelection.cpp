/*! \file CtlPrecoatSelection.cpp
 *  \brief The control select a precoat settings for printer 2/printer 1 of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrecoatSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PrintConditionGUIDef.h"

// ==========================================================================
// Local definitions
#define DEF_COLOR_LIGHT_GRAY		RGB(135, 135, 135)
// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< Static-box: Precoat icon
	CTRLID_SC_BACKGROUND,				//!< Static-box: Background
	CTRLID_SC_PRINTER_2_ICON,			//!< Static-box: Printer 2 icon
	CTRLID_PD_PRINTER_2_PRECOAT,		//!< Pull-down: Precoat for Printer 2
	CTRLID_SC_PRINTER_1_ICON,			//!< Static-box: Printer 1 icon
	CTRLID_PD_PRINTER_1_PRECOAT,		//!< Pull-down: Precoat for Printer 1

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlPrecoatSelection::CCtlPrecoatSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPrecoatSelection::~CCtlPrecoatSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPrecoatSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: Precoat icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 690, 80, 690 + 60, 80 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRECOAT_ICON, RESOURCE_BMP);
	}

	// static-box: background
	{
		long ctlId = CTRLID_SC_BACKGROUND;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 760, 77, 760 + 310, 77 + 66);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_GRAY_BACKGROUND, RESOURCE_BMP);
	}

	// static-box: printer 2 icon
	{
		long ctlId = CTRLID_SC_PRINTER_2_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		m_ctlAttribute[ctlId].ownerID = CTRLID_SC_BACKGROUND;
		SetRect(&m_ctlAttribute[ctlId].rect, 3, 13, 3 + 40, 13 + 40);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINTER_2_ICON, RESOURCE_BMP);
	}

	// pull-down: precoat for printer 2
	{
		long ctlId = CTRLID_PD_PRINTER_2_PRECOAT;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		m_ctlAttribute[ctlId].ownerID = CTRLID_SC_BACKGROUND;
		SetRect(&m_ctlAttribute[ctlId].rect, 47, 3, 47 + 100, 3 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}

	// static-box: printer 1 icon
	{
		long ctlId = CTRLID_SC_PRINTER_1_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		m_ctlAttribute[ctlId].ownerID = CTRLID_SC_BACKGROUND;
		SetRect(&m_ctlAttribute[ctlId].rect, 160, 13, 160 + 40, 13 + 40);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINTER_1_ICON, RESOURCE_BMP);
	}

	// pull-down: precoat for printer 1
	{
		long ctlId = CTRLID_PD_PRINTER_1_PRECOAT;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		m_ctlAttribute[ctlId].ownerID = CTRLID_SC_BACKGROUND;
		SetRect(&m_ctlAttribute[ctlId].rect, 202, 3, 202 + 100, 3 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrecoatSelection::OnCreateItem()
{
	// Set light gray color to background
	SetControlColor(m_ctl[CTRLID_SC_BACKGROUND], CC_IMAGE_BLEND, DEF_COLOR_LIGHT_GRAY);

	// Registers items for print speed menu pull-down
	RegisterItems();
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPrecoatSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Precoat for Printer 2
			if(ctlWnd == m_ctl[CTRLID_PD_PRINTER_2_PRECOAT]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINTER_2_PRECOAT]);
				// Set the selection item to DataIF
				pData->SetSelectPrecoat(DEF_PRINTER_2, nSelection);
			}
			// Precoat for Printer 1
			else if(ctlWnd == m_ctl[CTRLID_PD_PRINTER_1_PRECOAT]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINTER_1_PRECOAT]);
				// Set the selection item to DataIF
				pData->SetSelectPrecoat(DEF_PRINTER_1, nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPrecoatSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		bool isHavePrecoater = pData->IsExistPrecoater();
		bool isDED = pData->IsDED();

		DWORD dwState;

		// Display setting
		{
			bool hideCtl[CTLID_COUNT] = {false};
			hideCtl[CTRLID_SC_ICON] = !isHavePrecoater;
			hideCtl[CTRLID_SC_BACKGROUND] = !isHavePrecoater;
			hideCtl[CTRLID_SC_PRINTER_2_ICON] = !isHavePrecoater || !isDED;
			hideCtl[CTRLID_PD_PRINTER_2_PRECOAT] = !isHavePrecoater || !isDED;
			hideCtl[CTRLID_SC_PRINTER_1_ICON] = !isHavePrecoater;
			hideCtl[CTRLID_PD_PRINTER_1_PRECOAT] = !isHavePrecoater;

			for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
				dwState = hideCtl[ctlID] ? CST_HIDE : CST_SHOW;
				SetControlState(m_ctl[ctlID], dwState);
			}
		}

		// Valid setting
		{
			dwState = CST_ENABLED;

			// Disable control when the selected print settings is locked.
			if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
				dwState = CST_DISABLED;
			}

			SetControlState(m_ctl[CTRLID_PD_PRINTER_1_PRECOAT], dwState);
			SetControlState(m_ctl[CTRLID_PD_PRINTER_2_PRECOAT], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrecoatSelection::OnUpdateValue()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Display the selection item of Precoat for Printer 2
		long nSelection = -1;
		pData->GetSelectPrecoat(DEF_PRINTER_2, &nSelection);
		SetControlData(m_ctl[CTRLID_PD_PRINTER_2_PRECOAT], nSelection);

		// Display the selection item of Precoat for Printer 1
		nSelection = -1;
		pData->GetSelectPrecoat(DEF_PRINTER_1, &nSelection);
		SetControlData(m_ctl[CTRLID_PD_PRINTER_1_PRECOAT], nSelection);
	}
}

/*! \brief Register all items for Precoat selection pull-down
*/
void CCtlPrecoatSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	PDITEMINFO pd_item = {0};

	long nIndex = 0;

	long min = 0;			//!< min value of precoat
	long max = 0;			//!< max value of precoat

	// Get the range of precoat
	pData->GetPrecoatRange(&min, &max);

	for(long precoatVal = min; precoatVal <= max; ++precoatVal){
		char strPrecoat[10] = {0};
		_snprintf(strPrecoat, sizeof(strPrecoat) - 1, "%ld", precoatVal);

		pd_item.text = strPrecoat;

		SetControlItem(m_ctl[CTRLID_PD_PRINTER_2_PRECOAT], nIndex, &pd_item);
		SetControlItem(m_ctl[CTRLID_PD_PRINTER_1_PRECOAT], nIndex++, &pd_item);
	}
}