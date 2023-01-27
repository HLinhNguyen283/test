/*! \file CtlPaperOption.cpp
 *  \brief Paper option control
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPaperOption.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

#include "DlgSelectPreprintedMarkSide.h"

// ==========================================================================
// Local definitions
#define DEF_MAX_LENGTH_PERFORATION				3
#define DEF_MAX_LENGTH_PREPRINTED_MARK			6
#define DEF_COLOR_BLACK							RGB(0, 0, 0)
#define DEF_COLOR_WHITE							RGB(255, 255, 255)

// ==========================================================================

//! the control id list
enum{
	CTRLID_GB_BACKGROUND_A,						//!< Group-box: Background A
	CTRLID_BN_TRACTOR_HOLES,					//!< Button: Tractor holes
	CTRLID_BN_TRACTOR_HOLES_PERF,				//!< Button: Tractor holes and perforations
	CTRLID_EB_PERFORATIONS,						//!< Edit-box: Intervals range of perforations
	CTRLID_SC_PERFORATIONS_UNIT,				//!< Static-box: Unit of intervals range of perforations
	CTRLID_GB_BACKGROUND_B,						//!< Group-box: Background B
	CTRLID_BN_PREPRINTED_MARK,					//!< Button: Pre-printed mark
	CTRLID_EB_PREPRINTED_MARK,					//!< Edit-box: Offset of pre-printed mark
	CTRLID_SC_PREPRINTED_MARK_UNIT,				//!< Static-box: Unit of offset of pre-printed mark
	CTRLID_BN_PREPRINTED_PAPER,					//!< Button: Pre-printed paper

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_ctl[CTLID_COUNT] = {NULL, "CTRLID_BN_TRACTOR_HOLES", "CTRLID_BN_TRACTOR_HOLES_PERF", NULL, NULL, NULL, "CTRLID_BN_PREPRINTED_MARK", NULL, NULL, "CTRLID_BN_PREPRINTED_PAPER"};
// ==========================================================================

/*! \brief constructor
*/
CCtlPaperOption::CCtlPaperOption()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

	memset(&m_MarkedRepTime, 0x0, sizeof(m_MarkedRepTime));
	m_buttonPreprintedMarkDownFlg = false;
}

/*! \brief destructor
*/
CCtlPaperOption::~CCtlPaperOption()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPaperOption::OnSetAttribute()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Get the unit of the system setting
	DEF_LENGTH_UNIT lengthUnit = DEF_LENGTH_MM;
	const char* strLengthUnit = pData->GetSystemUnitLength(&lengthUnit);

	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]
	//!< Group-box: Background A
	{
		long ctlId = CTRLID_GB_BACKGROUND_A;
		m_ctlAttribute[ctlId].type = CT_GROUPBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | GBST_SUB;
		m_ctlAttribute[ctlId].text = NULL;
		// for NX
//		SetRect(&m_ctlAttribute[ctlId].rect, 13, 518, 13 + 284, 518 + 79);
		// for HD
		SetRect(&m_ctlAttribute[ctlId].rect, 13, 518, 13 + 160, 518 + 79);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//!< Button: Tractor holes
	{
		long ctlId = CTRLID_BN_TRACTOR_HOLES;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_STICKY | BNST_LED_MANUAL | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_HOLE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 4, 4, 4 + 71, 4 + 71);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_A;
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_PUNCH, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//!< Button: Tractor holes and perforations
	{
		long ctlId = CTRLID_BN_TRACTOR_HOLES_PERF;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_STICKY | BNST_LED_MANUAL | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_PERFORATED, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 82, 4, 82 + 71, 4 + 71);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_A;
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_TRACKHOLES_PERFORATION, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//!< Edit-box: Intervals range of perforations
	{
		long ctlId = CTRLID_EB_PERFORATIONS;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 164, 4, 164 + 100, 4 + 40);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_A;
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH_PERFORATION;
	}

	//!< Static-box: Unit of intervals range of perforations
	{
		long ctlId = CTRLID_SC_PERFORATIONS_UNIT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SHEETS, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 164, 47, 164 + 100, 47 + 25);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_A;
		m_ctlAttribute[ctlId].param = NULL;
	}

	//!< Group-box: Background B
	{
		long ctlId = CTRLID_GB_BACKGROUND_B;
		m_ctlAttribute[ctlId].type = CT_GROUPBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | GBST_SUB;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 300, 518, 300 + 204, 518 + 79);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//!< Button: Pre-printed mark
	{
		long ctlId = CTRLID_BN_PREPRINTED_MARK;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_AUTOREP | BNST_NORMAL | BNST_LED_MANUAL | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 4, 4, 4 + 71, 4 + 71);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_B;
		m_ctlAttribute[ctlId].param = NULL;
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}

	//!< Edit-box: Offset of pre-printed mark
	{
		long ctlId = CTRLID_EB_PREPRINTED_MARK;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 84, 4, 84 + 100, 4 + 40);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_B;
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH_PREPRINTED_MARK;
	}

	//!< Static-box: Unit of offset of pre-printed mark
	{
		long ctlId = CTRLID_SC_PREPRINTED_MARK_UNIT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = strLengthUnit;
		SetRect(&m_ctlAttribute[ctlId].rect, 84, 47, 84 + 100, 47 + 25);
		m_ctlAttribute[ctlId].ownerID = CTRLID_GB_BACKGROUND_B;
		m_ctlAttribute[ctlId].param = NULL;
	}

	//!< Button: Pre-printed paper
	{
		long ctlId = CTRLID_BN_PREPRINTED_PAPER;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_STICKY | BNST_LED_MANUAL | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_PREPRINTED, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 507, 522, 507 + 70, 522 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_PREPRINTED, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPaperOption::OnCreateItem()
{
	// Set black color for two group box
	SetControlColor(m_ctl[CTRLID_GB_BACKGROUND_A], CC_FACE, DEF_COLOR_BLACK);
	SetControlColor(m_ctl[CTRLID_GB_BACKGROUND_B], CC_FACE, DEF_COLOR_BLACK);

	// Set while text color for perforations unit and preprinted mark unit
	SetControlColor(m_ctl[CTRLID_SC_PERFORATIONS_UNIT], CC_TEXT, DEF_COLOR_WHITE);
	SetControlColor(m_ctl[CTRLID_SC_PREPRINTED_MARK_UNIT], CC_TEXT, DEF_COLOR_WHITE);
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlPaperOption::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(message){
	case UWM_COMMAND_END_REPEAT_BUTTON:
		{
			// Pre-printed mark button
			if((HWND)lParam == m_ctl[CTRLID_BN_PREPRINTED_MARK]){
				DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

				// Notify to dataIF
				bool isPrePrintedMark = pData->GetOptionPreprintedMark(&printSide);
				pData->SetOptionPreprintedMark(!isPrePrintedMark);

				m_buttonPreprintedMarkDownFlg = false;
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
long CCtlPaperOption::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			// Tractor holes button
			if(ctlWnd == m_ctl[CTRLID_BN_TRACTOR_HOLES]){
				long isUse = GetControlData(m_ctl[CTRLID_BN_TRACTOR_HOLES]);

				// Change the selection print settings has tractor holes
				pData->SetOptionTractorHole(isUse == 0 ? false : true);
			}
			// Tractor holes and preforations button
			else if(ctlWnd == m_ctl[CTRLID_BN_TRACTOR_HOLES_PERF]){
				long isUse = GetControlData(m_ctl[CTRLID_BN_TRACTOR_HOLES_PERF]);

				// Change the selection print settings has both of tractor holes and perforations
				pData->SetOptionTractorholesAndPerforations(isUse == 0 ? false : true);
			}
			// Pre-printed mark button
			else if(ctlWnd == m_ctl[CTRLID_BN_PREPRINTED_MARK]){
				time_t now_time = {0};
				time(&now_time);

				if(!m_buttonPreprintedMarkDownFlg){
					m_MarkedRepTime = now_time;
					m_buttonPreprintedMarkDownFlg = true;
				}

				long clickSec = (long)difftime(now_time, m_MarkedRepTime);
				if(clickSec >= 1){
					// Notify to display Select side of pre-printed mark dialog
					pData->InitSelectPreprintedMarkSideDlg();
					// Display the dialog
					DlgSelectPreprintedMarkSide dlg;
					dlg.Create(m_data, m_printer);
				}
			}
			// Pre-printed paper button
			else if(ctlWnd == m_ctl[CTRLID_BN_PREPRINTED_PAPER]){
				long isUse = GetControlData(m_ctl[CTRLID_BN_PREPRINTED_PAPER]);

				// Change the selection print settings has pre-printed paper
				pData->SetOptionPreprintedPaper(isUse == 0 ? false : true);
			}
		}
		break;
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Intervals range of perforations edit-box
			if(ctlWnd == m_ctl[CTRLID_EB_PERFORATIONS]){
				long interval = atol((char*)GetControlData(m_ctl[CTRLID_EB_PERFORATIONS]));
				// Change the intervals range of perforations
				pData->SetOptionPerforationInterval(interval);
			}
			// Offset of pre-printed mark edit-box
			else if(ctlWnd == m_ctl[CTRLID_EB_PREPRINTED_MARK]){
				long offset = (long)(atof((char*)GetControlData(m_ctl[CTRLID_EB_PREPRINTED_MARK])) * 1000);
				// Change the offset of pre-printed mark
				pData->SetOptionPreprintedMarkOffset(offset);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPaperOption::OnUpdateState()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		// Always show all the controls
		for(long ctlID = 0; ctlID < m_ctlCount; ++ctlID){
			SetControlState(m_ctl[ctlID], CST_SHOW);
		}

		// Hide perforation pattern count if HD
		SetControlState(m_ctl[CTRLID_EB_PERFORATIONS], CST_HIDE);
		SetControlState(m_ctl[CTRLID_SC_PERFORATIONS_UNIT], CST_HIDE);

		// Valid setting
		DWORD dwState;				//!< State of control

		// Check the print settings is locked the edit or not.
		bool bIsLockPrintCondition = pData->GetLockPrintCondtion();

		// Tractor holes button/ Tractor holes and perforations button/ Pre-printed mark button/ Pre-printed paper button
		{
			HWND listHwnd[4] = {
				m_ctl[CTRLID_BN_TRACTOR_HOLES],
				m_ctl[CTRLID_BN_TRACTOR_HOLES_PERF],
				m_ctl[CTRLID_BN_PREPRINTED_MARK],
				m_ctl[CTRLID_BN_PREPRINTED_PAPER]
			};

			dwState = CST_ENABLED;

			// Disable control when the selected print settings is locked.
			if(true == bIsLockPrintCondition || pData->IsDisablePrintCondition()){
				dwState = CST_DISABLED;
			}

			for(int i = 0; i < 4; ++i){
				SetControlState(listHwnd[i], dwState);
			}
		}

		// Intervals range of perforations edit-box
		{
			dwState = CST_ENABLED;

			// Disable control when one of the next conditions met:
			// - The selection print settings is locked the edit.
			// - The selection print settings has not both of tractor holes and perforations
			if(true == bIsLockPrintCondition
				|| false == pData->GetOptionTractorHolesAndPerforations())
			{
				dwState = CST_DISABLED;
			}

			SetControlState(m_ctl[CTRLID_EB_PERFORATIONS], dwState);
		}

		// Offset of pre-printed mark edit-box
		{
			DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

			dwState = CST_ENABLED;

			// Disable control when one of the next conditions met:
			// - The selection print settings is locked the edit.
			// - The selection print settings does not have pre-printed mark
			if(true == bIsLockPrintCondition
				|| false == pData->GetOptionPreprintedMark(&printSide))
			{
				dwState = CST_DISABLED;
			}

			SetControlState(m_ctl[CTRLID_EB_PREPRINTED_MARK], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPaperOption::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	BNITEMINFO bn = {0};

	// Tractor holes button
	{
		bool isTractorHoleON = pData->GetOptionTractorHole();
		bn.bLED_On = isTractorHoleON  ? TRUE : FALSE;

		SetControlData(m_ctl[CTRLID_BN_TRACTOR_HOLES], isTractorHoleON ? 1 : 0);
		SetControlItem(m_ctl[CTRLID_BN_TRACTOR_HOLES], 0, &bn);
	}

	// Tractor holes and perforations button
	{
		bool isTractorHoleAndPerfON = pData->GetOptionTractorHolesAndPerforations();
		bn.bLED_On = isTractorHoleAndPerfON  ? TRUE : FALSE;

		SetControlData(m_ctl[CTRLID_BN_TRACTOR_HOLES_PERF], isTractorHoleAndPerfON ? 1 : 0);
		SetControlItem(m_ctl[CTRLID_BN_TRACTOR_HOLES_PERF], 0, &bn);
	}

	// Intervals range of perforations edit-box
	{
		// Get the range of perforations and display it
		long interval = pData->GetOptionPerforationInterval();
		char strInterval[256] = { 0 };
		_snprintf_s(strInterval, _TRUNCATE, "%ld", interval);
		SetControlData(m_ctl[CTRLID_EB_PERFORATIONS], (DWORD)strInterval);
	}

	// Pre-printed mark button
	{
		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
		long bitmapID = IDB_PAPER_MARK_FRONT;

		// Get option preprinted mark
		bool isPreprintedMarkON = pData->GetOptionPreprintedMark(&printSide);

		if(DEF_PRINT_BACK == printSide){
			bitmapID = IDB_PAPER_MARK_BACK;
		}

		// Update button info
		bn.bLED_On = isPreprintedMarkON ? TRUE : FALSE;
		bn.nMask = OT_BITMAP | OT_TEXT;
		bn.hImage = LoadResourceBitmap(bitmapID, RESOURCE_BMP);
		bn.lpszText = (char*)LoadResourceString(IDS_PREPRINTED_MARK, RESOURCE_STR);

		SetControlItem(m_ctl[CTRLID_BN_PREPRINTED_MARK], 0, &bn);
		SetControlData(m_ctl[CTRLID_BN_PREPRINTED_MARK], isPreprintedMarkON ? 1 : 0);

		bn.hImage = NULL;
		bn.nMask = 0;
	}

	// Offset of pre-printed mark edit-box
	{
		// Get the offset and display it
		double offset = (double)pData->GetOptionPreprintedMarkOffset() / 1000;
		char strOffset[256] = { 0 };
		_snprintf_s(strOffset, _TRUNCATE, "%0.1f", offset);
		SetControlData(m_ctl[CTRLID_EB_PREPRINTED_MARK], (DWORD)strOffset);
	}

	// Pre-printed paper button
	{
		bool isPrePrintedPaper = pData->GetOptionPreprintedPaper();
		bn.bLED_On = isPrePrintedPaper ? TRUE : FALSE;

		SetControlItem(m_ctl[CTRLID_BN_PREPRINTED_PAPER], 0, &bn);
		SetControlData(m_ctl[CTRLID_BN_PREPRINTED_PAPER], isPrePrintedPaper ? 1 : 0);
	}
}
