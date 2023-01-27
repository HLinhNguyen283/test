/*! \file CtlOvercoatSelection.h
 *  \brief The control select an overcoat settings for printer 2/printer 1 of the selection print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlOvercoatSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_PRINTER_1				1
#define DEF_PRINTER_2				2
#define DEF_COLOR_LIGHT_GRAY		RGB(135, 135, 135)
// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< Static-box: Overcoat icon
	CTRLID_SC_BACKGROUND,				//!< Static-box: Background
	CTRLID_SC_PRINTER_2_ICON,			//!< Static-box: Printer 2 icon
	CTRLID_PD_PRINTER_2_OVERCOAT,		//!< Pull-down: Overcoat for Printer 2
	CTRLID_SC_PRINTER_1_ICON,			//!< Static-box: Printer 1 icon
	CTRLID_PD_PRINTER_1_OVERCOAT,		//!< Pull-down: Overcoat for Printer 1

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlOvercoatSelection::CCtlOvercoatSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlOvercoatSelection::~CCtlOvercoatSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlOvercoatSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: Overcoat icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 690, 150, 690 + 60, 150 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_OVERCOAT_ICON, RESOURCE_BMP);
	}

	// static-box: background
	{
		long ctlId = CTRLID_SC_BACKGROUND;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 760, 147, 760 + 310, 147 + 66);
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

	// pull-down: Overcoat for printer 2
	{
		long ctlId = CTRLID_PD_PRINTER_2_OVERCOAT;
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

	// pull-down: Overcoat for printer 1
	{
		long ctlId = CTRLID_PD_PRINTER_1_OVERCOAT;
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
void CCtlOvercoatSelection::OnCreateItem()
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
long CCtlOvercoatSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Overcoat for Printer 2
			if(ctlWnd == m_ctl[CTRLID_PD_PRINTER_2_OVERCOAT]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINTER_2_OVERCOAT]);

				// Set the selection item to DataIF
				pData->SetSelectOvercoat(DEF_PRINTER_2, nSelection);
			}
			// Overcoat for Printer 1
			else if(ctlWnd == m_ctl[CTRLID_PD_PRINTER_1_OVERCOAT]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINTER_1_OVERCOAT]);

				// Set the selection item to DataIF
				pData->SetSelectOvercoat(DEF_PRINTER_1, nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlOvercoatSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		DWORD dwState;			//!< state of control

		// Check the printer 1/2 has an overcoater unit or not.
		bool bIsExistOvercoatP1 = pData->IsExistOvercoater(DEF_PRINTER_1);
		bool bIsExistOvercoatP2 = pData->IsExistOvercoater(DEF_PRINTER_2);

		// Check system runs with DED or SES mode
		bool isDED = pData->IsDED();

		// Display setting
		{
			bool hideCtl[CTLID_COUNT] = {false};
			hideCtl[CTRLID_SC_ICON] = !bIsExistOvercoatP1 && !bIsExistOvercoatP2;
			hideCtl[CTRLID_SC_BACKGROUND] = !bIsExistOvercoatP1 && !bIsExistOvercoatP2;
			hideCtl[CTRLID_SC_PRINTER_2_ICON] = !bIsExistOvercoatP2 || !isDED || (isDED && pData->IsSimplexJob());
			hideCtl[CTRLID_PD_PRINTER_2_OVERCOAT] = !bIsExistOvercoatP2 || !isDED || (isDED && pData->IsSimplexJob());
			hideCtl[CTRLID_SC_PRINTER_1_ICON] = !bIsExistOvercoatP1;
			hideCtl[CTRLID_PD_PRINTER_1_OVERCOAT] = !bIsExistOvercoatP1;
			for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
				dwState = hideCtl[ctlID] ? CST_HIDE : CST_SHOW;
				SetControlState(m_ctl[ctlID], dwState);
			}
		}

		// Valid setting
		{
			// the effective for print mode selection pull-down menu
			dwState = CST_ENABLED;

			// Disable control when the selected print settings is locked.
			if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
				dwState = CST_DISABLED;
			}

			SetControlState(m_ctl[CTRLID_PD_PRINTER_1_OVERCOAT], dwState);
			SetControlState(m_ctl[CTRLID_PD_PRINTER_2_OVERCOAT], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlOvercoatSelection::OnUpdateValue()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Display the selection item of Overcoat for Printer 2
		long nSelection = -1;
		pData->GetSelectOvercoat(DEF_PRINTER_2, &nSelection);
		SetControlData(m_ctl[CTRLID_PD_PRINTER_2_OVERCOAT], nSelection);

		// Display the selection item of Overcoat for Printer 1
		nSelection = -1;
		pData->GetSelectOvercoat(DEF_PRINTER_1, &nSelection);
		SetControlData(m_ctl[CTRLID_PD_PRINTER_1_OVERCOAT], nSelection);
	}
}

/*! \brief Register all items for Overcoat selection pull-down
*/
void CCtlOvercoatSelection::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	PDITEMINFO pd_item = {0};

	long nIndex = 0;

	long min = 0;			//!< min value of overcoat
	long max = 0;			//!< max value of overcoat

	// Get the range of overcoat
	pData->GetOvercoatRange(&min, &max);

	// Set overcoat value to pull-down
	for(long overcoatVal = min; overcoatVal <= max; ++overcoatVal){
		char strOvercoat[10] = {0};
		_snprintf(strOvercoat, sizeof(strOvercoat) - 1, "%ld", overcoatVal);
		
		pd_item.text = strOvercoat;

		SetControlItem(m_ctl[CTRLID_PD_PRINTER_2_OVERCOAT], nIndex, &pd_item);
		SetControlItem(m_ctl[CTRLID_PD_PRINTER_1_OVERCOAT], nIndex++, &pd_item);
	}
}