/*! \file CtlCloseScreen.cpp
 *  \brief The control exits the screen
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlCloseScreen.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! the control id list
enum{
	CTRLID_BN_CLOSE = 0,				//!< ボタン：閉じる

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_close = "CTRLID_BN_CLOSE";
// ==========================================================================

/*! \brief constructor
*/
CCtlCloseScreen::CCtlCloseScreen()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlCloseScreen::~CCtlCloseScreen()
{
}

/*! \brief [event]set the control's property
*/
void CCtlCloseScreen::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button：close screen
	{
		long ctlId = CTRLID_BN_CLOSE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BUTTON_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 1040, 790, 1040 + 160, 790 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_EXIT, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_close;
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
long CCtlCloseScreen::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam){
	case BN_CLICKED:
		{
			// Exit screen button
			if(ctlWnd == m_ctl[CTRLID_BN_CLOSE]){
				// The selected print settings are not saved yet
				if(false == pData->IsSavedPrintSetting()){
					//Display a dialog-box to confirm
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_DISCARD_CHANGES_CONFIRMATION, LoadResourceString(IDM_DISCARD_CHANGES_CONFIRMATION, RESOURCE_MSG_STR));

					if(IDNO == ShowMessageBox(msg, MBST_YESNO | MBST_ICONINFORMATION, NULL)){
						return DEF_NONE;
					}
				}

				if(pData->IsStartFromSystemSetting()){
					// The displayed print settings are not assigned as current
					bool isCurrentPrintCondition = false;
					const char* strCurrentPrintCondition = NULL;
					bool hasCurrentPrintCondition = pData->GetCurrentPrintCondition(&isCurrentPrintCondition, &strCurrentPrintCondition);
					if(!hasCurrentPrintCondition || !isCurrentPrintCondition){
						if(false == isCurrentPrintCondition){
							//Display a dialog-box to confirm
							char msg[256] = {0};
							_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_NOT_SELECT_PRINT_SETTING, LoadResourceString(IDM_NOT_SELECT_PRINT_SETTING, RESOURCE_MSG_STR));

							if(IDCANCEL == ShowMessageBox(msg, MBST_OKCANCEL | MBST_ICONINFORMATION, NULL)){
								return DEF_NONE;
							}
						}
					}

					// check ICC preset not used in any print setting, display dialog 
					pData->CheckICCPresetUse();
				} else {
					//ジョブ設定時はOffからOnになったらカレント設定を変更する
					if(!pData->GetCurrentConditionBtnOnOffOpen()
						&& pData->GetCurrentConditionBtnOnOff()
					) {
						pData->SetCurrentPrintCondition(false);
					}
				}

				// Close dialog
				if(true == pData->ExitScreen()){
					return DEF_EXIT_PAGE;
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

