/*! \file CtlClosePrintConditionDeleteDlg.cpp
 *  \brief Close screen control of Delete Print Settings dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlClosePrintConditionDeleteDlg.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! the control id list
enum{
	CTRLID_BN_CLOSE = 0,			//!< button: to close dialog

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_close = "CTRLID_BN_CLOSE";
// ==========================================================================

/*! \brief constructor
*/
CCtlClosePrintConditionDeleteDlg::CCtlClosePrintConditionDeleteDlg()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlClosePrintConditionDeleteDlg::~CCtlClosePrintConditionDeleteDlg()
{
}


/*! \brief [event]set the control's property
*/
void CCtlClosePrintConditionDeleteDlg::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button: close dialog
	{
		long ctlId = CTRLID_BN_CLOSE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 600, 570, 600 + 155, 570 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_EXIT, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_close;
	}
}

/**
	@brief	[event]コントロールのプロパティを設定するイベント
*/
void CCtlClosePrintConditionDeleteDlg::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if (pData->IsIpdsMode()) {
		//IPDSモードの場合は何もしない
	}
	else {
		//ジョブ管理にQueueStatusの発行を依頼する
		pData->SendQueueStatusCommand();
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
long CCtlClosePrintConditionDeleteDlg::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case BN_CLICKED:
		{
			// Exit button
			if(ctlWnd == m_ctl[CTRLID_BN_CLOSE]){
				// Notify close dialog
				if(pData->ExitDeletePrintConditionDlg()){
					return DEF_EXIT_PAGE;
				}
			}
		}
		break;
	}

	return DEF_NONE;
}
