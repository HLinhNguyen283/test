/**
	@file	CtlPaperWeight.cpp
	@brief	The control edits a paper weight of the selection print settings on the screen.
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPaperWeight.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_MAX_LENGTH_WEIGHT_GSM		5
// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_WEIGHT_GSM_ICON = 0,		//!< Static-box: Weight gsm icon
	CTRLID_EB_WEIGHT_GSM,				//!< Edit-box: Weight gsm

	CTLID_COUNT
};

// ==========================================================================

// constructor
CCtlPaperWeight::CCtlPaperWeight()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

// destructor
CCtlPaperWeight::~CCtlPaperWeight()
{
}

// [event]set the control's property
void CCtlPaperWeight::OnSetAttribute()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Static-box: Paper weight icon
	{
		long ctlId = CTRLID_SC_WEIGHT_GSM_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 485, 451, 485 +48, 451 + 48);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_WEIGHT_GSM_ICON, RESOURCE_BMP);
	}
	// Edit-box: Paper weight
	{
		long ctlId = CTRLID_EB_WEIGHT_GSM;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 540, 445, 540 + 130, 445 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH_WEIGHT_GSM;
	}
}

// [event]the control operated
long CCtlPaperWeight::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Paper weight edit-box
			if (ctlWnd == m_ctl[CTRLID_EB_WEIGHT_GSM])
			{
				// Change the page width
				DWORD controlValue = GetControlData(m_ctl[CTRLID_EB_WEIGHT_GSM]);
				double weight = atof(reinterpret_cast<char*>(controlValue));
				pData->setPaperWeight(weight);
			}
		}
		break;
	case BN_CLICKED:
		{
		}
		break;
	case CBN_SELCHANGE:
		{
		}
		break;
	}

	return DEF_NONE;
}

// コントロールステータス更新イベント
void CCtlPaperWeight::OnUpdateState()
{
	if (m_ctl)
	{
		// Static-box: Weight gsm icon
		{
			SetControlState(m_ctl[CTRLID_SC_WEIGHT_GSM_ICON], CST_SHOW);
		}

		// Edit-box: Weight gsm
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
			DWORD dwState = CST_SHOW | CST_ENABLED;		//!< state of control

			// Disable control when the selected print settings is locked.
			if (true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition())
			{
				dwState = CST_DISABLED;
			}

			// Set state for controls
			SetControlState(m_ctl[CTRLID_EB_WEIGHT_GSM], dwState);
		}
	}
}

// コントロール表示値更新イベント
void CCtlPaperWeight::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Display the Weight gsm
	double weight = pData->getPaperWeight();
	char strWeight[256] = { 0 };
	_snprintf_s(strWeight, _TRUNCATE, "%0.1f", weight);
	SetControlData(m_ctl[CTRLID_EB_WEIGHT_GSM], (DWORD)strWeight);
}
