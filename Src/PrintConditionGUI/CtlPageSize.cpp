/*! \file CtlPageSize.cpp
 *  \brief The control edits a page size of the selection print settings on the screen.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPageSize.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
//#define DEF_MAX_LENGTH_PAGE_WITH			5
#define DEF_MAX_LENGTH_PAGE_WIDTH			6
#define DEF_MAX_LENGTH_PAGE_FEED			6
// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_WIDTH_ICON = 0,			//!< Static-box: Width icon
	CTRLID_EB_PAGE_WIDTH,				//!< Edit-box: Page width
	CTRLID_SC_WIDTH_UNIT,				//!< Static-box: Width unit
	CTRLID_BN_USE_PAGE_FEED,			//!< Button: Use page feed setting
	CTRLID_EB_PAGE_FEED_LENGTH,			//!< Edit-box: Page feed length
	CTRLID_PD_FEED_UNIT,				//!< Pull-down: Feed unit

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_use_page_feed = "CTRLID_BN_USE_PAGE_FEED";
// ==========================================================================

/*! \brief constructor
*/
CCtlPageSize::CCtlPageSize()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPageSize::~CCtlPageSize()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPageSize::OnSetAttribute()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Get the unit of the system setting
	DEF_LENGTH_UNIT lengthUnit = DEF_LENGTH_MM;
	const char* strLengthUnit = pData->GetSystemUnitLength(&lengthUnit);

	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// Static-box: Width icon
	{
		long ctlId = CTRLID_SC_WIDTH_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 10, 365, 10 + 60, 365 + 75);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAGE_WIDTH_ICON, RESOURCE_BMP);
	}

	// Edit-box: Page width
	{
		long ctlId = CTRLID_EB_PAGE_WIDTH;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 80, 375, 80 + 130, 375 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH_PAGE_WIDTH;
	}

	// Static-box: Page width unit
	{
		long ctlId = CTRLID_SC_WIDTH_UNIT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = strLengthUnit;
		SetRect(&m_ctlAttribute[ctlId].rect, 215, 380, 215 + 45, 380 + 45);
		m_ctlAttribute[ctlId].param = NULL;
	}

	// Button: Use page feed setting
	{
		long ctlId = CTRLID_BN_USE_PAGE_FEED;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_LENGTH, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 263, 370, 263 + 70, 370 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAGE_FEED_ICON, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_use_page_feed;
	}

	// Edit-box: Page feed length
	{
		long ctlId = CTRLID_EB_PAGE_FEED_LENGTH;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 345, 375, 345 + 130, 375 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH_PAGE_FEED;
	}

	// Pull-down: Feed unit
	{
		long ctlId = CTRLID_PD_FEED_UNIT;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 490, 375, 490 + 180, 375 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPageSize::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	PDITEMINFO pd_item = {0};

	long nItemID = 0;
	const char* strUnit = NULL;

	// Register the items of all units to pull-down
	while(true == pData->GetPaperFeedUnitList(nItemID, &strUnit)){
		pd_item.text = (char*)strUnit;

		SetControlItem(m_ctl[CTRLID_PD_FEED_UNIT], nItemID++, &pd_item);

		strUnit = NULL;
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
long CCtlPageSize::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Page width edit-box
			if(ctlWnd == m_ctl[CTRLID_EB_PAGE_WIDTH]){
				// Change the page width
				long width = (long)(atof((char*)GetControlData(m_ctl[CTRLID_EB_PAGE_WIDTH])) * 1000);
				pData->SetPaperWidth(width);
			}
			// Page feed length edit-box
			else if(ctlWnd == m_ctl[CTRLID_EB_PAGE_FEED_LENGTH]){
				// Change the feed length
				long feedLength = (long)(atof((char*)GetControlData(m_ctl[CTRLID_EB_PAGE_FEED_LENGTH])) * 1000);
				pData->SetPaperFeedLength(feedLength);
			}
		}
		break;
	case BN_CLICKED:
		{
			// Use page feed setting button
			if(ctlWnd == m_ctl[CTRLID_BN_USE_PAGE_FEED]){
				long isON = GetControlData(m_ctl[CTRLID_BN_USE_PAGE_FEED]);

				// Notify status of Page feed length setting button to DataIF
				pData->SetUsePaperFeedLengthSetting(isON == 0 ? false : true);
			}
		}
		break;
	case CBN_SELCHANGE:
		{
			// Feed unit pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_FEED_UNIT]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_FEED_UNIT]);

				// Change the selection feed unit
				pData->SetSelectPaperFeedUnit(nSelection);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPageSize::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		DWORD dwState;		//!< state of control

		// Display setting
		{
			// Always show: Width icon, page width edid-box, width unit, use page feed setting button
			{
				for(long ctlID = CTRLID_SC_WIDTH_ICON; ctlID <= CTRLID_BN_USE_PAGE_FEED; ++ctlID){
					SetControlState(m_ctl[ctlID], CST_SHOW);
				}
			}

			// Page feed length edit-box, feed unit pull-down
			{
				dwState = CST_SHOW;
				// The display status of Page feed length setting is OFF
				if(false == pData->GetUsePaperFeedLengthSetting()){
					dwState = CST_HIDE;
				}

				SetControlState(m_ctl[CTRLID_EB_PAGE_FEED_LENGTH], dwState);
				SetControlState(m_ctl[CTRLID_PD_FEED_UNIT], dwState);
			}
		}

		// Valid setting
		{
			HWND listHwnd[4] = {
				m_ctl[CTRLID_EB_PAGE_WIDTH],			// Page width edit-box
				m_ctl[CTRLID_BN_USE_PAGE_FEED],			// Use page feed length button
				m_ctl[CTRLID_EB_PAGE_FEED_LENGTH],		// Page feed length edit-box
				m_ctl[CTRLID_PD_FEED_UNIT]				// Feed unit pull-down
			};

			dwState = CST_ENABLED;

			// Disable control when the selected print settings is locked.
			if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
				dwState = CST_DISABLED;
			}

			// Set state for controls
			for(int hWndId = 0; hWndId < 4; ++ hWndId){
				SetControlState(listHwnd[hWndId], dwState);
			}
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPageSize::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Display the page width
	double width = (double)pData->GetPaperWidth() / 1000;
	char strWidth[256] = { 0 };
	_snprintf_s(strWidth, _TRUNCATE, "%0.1f", width);
	SetControlData(m_ctl[CTRLID_EB_PAGE_WIDTH], (DWORD)strWidth);

	// Display the status of Page feed length setting
	bool isOn = pData->GetUsePaperFeedLengthSetting();
	BNITEMINFO bn_item = {0};
	bn_item.bLED_On = isOn;

	SetControlItem(m_ctl[CTRLID_BN_USE_PAGE_FEED], 0, &bn_item);
	SetControlData(m_ctl[CTRLID_BN_USE_PAGE_FEED], (DWORD)isOn);

	// Display page feed length
	double length = (double)pData->GetPaperFeedLength() / 1000;
	char strLength[256] = { 0 };
	_snprintf_s(strLength, _TRUNCATE, "%0.1f", length);
	SetControlData(m_ctl[CTRLID_EB_PAGE_FEED_LENGTH], (DWORD)strLength);

	// Display the selection feed unit
	long nSelection = -1;
	pData->GetSelectPaperFeedUnit(&nSelection);
	SetControlData(m_ctl[CTRLID_PD_FEED_UNIT], nSelection);
}
