/*! \file CtlCurrentPrintCondition.cpp
 *  \brief Current print condition setting control
 *  \author GCS Tan Ho
 *  \date 2013/05/14 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlCurrentPrintCondition.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PrinterIF.h"
#include "Stub_PrinterStateIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================
//! the control id list
enum{
	CTRLID_BTN_CURRENT_SETTING = 0,		//!< current print condition

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_current_setting = "CTRLID_BTN_CURRENT_SETTING";
// ===========================================================================

/*! \brief constructor
*/
CCtlCurrentPrintCondition::CCtlCurrentPrintCondition()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlCurrentPrintCondition::~CCtlCurrentPrintCondition()
{
}


/*! \brief [event]set the control's property
*/
void CCtlCurrentPrintCondition::OnSetAttribute()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	//button：current print condition setting
	{
		long ctlId = CTRLID_BTN_CURRENT_SETTING;
		m_ctlAttribute[ctlId].type = CT_BUTTON;

		if (pData->IsStartFromSystemSetting()) {
			//PUSH(Off->On)動作のみ
			m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION | BNST_PUSHONLY;
		} else {
			//トグル動作
			m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION | BNST_STICKY;
		}

		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BUTTON_CURRENT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 795, 15, 795 + 80, 15 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_SET_CURRENT_ROLL, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_current_setting;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlCurrentPrintCondition::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	//開始時のカレント設定を保持
	bool bIsPrintCondition = FALSE;
	pData->CmpSelectingAndSystemCurrentPrintCondition(bIsPrintCondition);
	pData->SetCurrentConditionBtnOnOffOpen(bIsPrintCondition);
	pData->SetCurrentConditionBtnOnOff(bIsPrintCondition);
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlCurrentPrintCondition::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// set current print condition
			if(ctlWnd == m_ctl[CTRLID_BTN_CURRENT_SETTING]){
				// change the current print condition
				bool bShowMsg = false;
				// change the current print condition
				if(pData->IsStartFromSystemSetting()) {
					//Check blank item
					if(pData->IsBlankNotAllowed()) {
						// show message box.
						char msg[256] = {0};
						_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_MSG_BLANK_NOT_ALLOWED, LoadResourceString(IDM_MSG_BLANK_NOT_ALLOWED, RESOURCE_MSG_STR));
						ShowMessageBox(msg, MBST_OK | MBST_ICONERROR, NULL);
						return DEF_NONE;
					}
					//システム設定は即時変更	
					pData->SetCurrentPrintCondition();
					bShowMsg = true;
				} else {
					//ジョブ設定は画面を閉じるときに変更
					DWORD bIsPrintCondition = GetControlData(m_ctl[CTRLID_BTN_CURRENT_SETTING]);
					pData->SetCurrentConditionBtnOnOff(bIsPrintCondition==0? false: true);
					if(bIsPrintCondition) {
						//OffからOnの場合に表示
						bShowMsg = true;
					}
				}

				if(bShowMsg) {
					DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

					// The selection paper has tractor holes and preforations.
					// The selection paper does not have pre-printed mark.
					if(true == pData->GetOptionTractorHolesAndPerforations()
						&& false == pData->GetOptionPreprintedMark(&printSide))
					{
						// Display a dialog box
						char msg[256] = {0};
						_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_SET_CURRENT_PRINT_INFO, LoadResourceString(IDM_SET_CURRENT_PRINT_INFO, RESOURCE_MSG_STR));

						ShowMessageBox(msg, MBST_OK | MBST_ICONINFORMATION, NULL);
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
void CCtlCurrentPrintCondition::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		CPrinterIF* pPrinter = dynamic_cast<CPrinterIF*>(m_printer);

		bool bIsPrintCondition = false;
		pData->CmpSelectingAndSystemCurrentPrintCondition(bIsPrintCondition);

		DWORD dwState = CST_SHOW;
//		// hide control when dialog is started from job list screen.
//		if (!pData->IsStartFromSystemSetting())
//		{
//			dwState = CST_HIDE;
//		}

		// Disable control when one of the following conditions occur:
		if(true == pData->IsJobRunning()			// a job is running
			|| true == pPrinter->IsDuringAdjustPrinting()	// 調整印刷中
			|| false == pPrinter->IsPrinterStop()	// the printer does not stop.
			|| (!pData->IsStartFromSystemSetting() && bIsPrintCondition)	// ジョブ設定時、ジョブとカレントの印刷条件が一致
			)
		{
			dwState |= CST_DISABLED;
		}else{
			// enable the control
			dwState |= CST_ENABLED;
		}

		SetControlState(m_ctl[CTRLID_BTN_CURRENT_SETTING], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlCurrentPrintCondition::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	bool bIsPrintCondition = FALSE;
	const char* strPrinterPrintCondition = NULL;
	if(pData->IsStartFromSystemSetting()) {
		pData->GetCurrentPrintCondition(&bIsPrintCondition, &strPrinterPrintCondition);
	} else {
		//ボタンの状態を維持
		bIsPrintCondition = pData->GetCurrentConditionBtnOnOff();
	}
	// Toggle ON/OFF the control
	SetControlData(m_ctl[CTRLID_BTN_CURRENT_SETTING], bIsPrintCondition);
}
