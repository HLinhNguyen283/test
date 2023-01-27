﻿/*! \file CtlDeleteDryerSettingsSelection_DlgDeleteDryer.h
 *  \brief The control selects to delete items of dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDeleteDryerSettingsSelection_DlgDeleteDryer.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include <vector>
using namespace std;

// ==========================================================================
// Local definitions
#define DEF_H_LISTBOX				510
#define DEF_W_LISTBOX				720
#define DEF_H_BUTTON				70
#define DEF_W_BUTTON				155
#define DEF_MAX_ITEMS_DISPLAY		10
#define DEF_MAX_CHARS_APPENDED(strMessage)			(sizeof(strMessage) - strlen(strMessage) - 1)

//! the control id list
enum{
	CTRLID_LB_DRYER_SETTINGS,				//!< list-box: Dryer settings
	CTRLID_BN_DELETE,						//!< button: delete

	CTRLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_delete = "CTRLID_BN_DELETE";
// ==========================================================================

/*! \brief constructor
*/
CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::CCtlDeleteDryerSettingsSelection_DlgDeleteDryer()
{
	//set to create the number of control.
	m_numOfItems = 0;

	m_ctlCount = CTRLID_COUNT;
}

/*! \brief destructor
*/
CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::~CCtlDeleteDryerSettingsSelection_DlgDeleteDryer()
{
}

/*! \brief [event]set the control's property
*/
void CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//list-box: dryer settings
	{
		long ctlId = CTRLID_LB_DRYER_SETTINGS;
		m_ctlAttribute[ctlId].type = CT_LISTBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | LBST_LINEDISPLAY | LBST_VSCROLL | LBST_TEXT | LBST_MULTISELECT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 40, 40, 40 + DEF_W_LISTBOX, 40 + DEF_H_LISTBOX);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//button: delete
	{
		long ctlId = CTRLID_BN_DELETE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_OPERATOR_DELETE, RESOURCE_STR);;
		SetRect(&m_ctlAttribute[ctlId].rect, 40, 570, 40 + DEF_W_BUTTON, 570 + DEF_H_BUTTON);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_DELETE, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_delete;
	}
}

/*! \brief event to set the properties of the control
*/
void CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::OnCreateItem()
{
	// register all item of job
	RegisterItems();
}

/*! \brief ウィンドウプロシージャ
 *  \param hWnd ウィンドウハンドル
 *  \param Message メッセージ番号
 *  \param wParam パラメータ
 *  \param lParam パラメータ
 *  \return DEF_NONE
*/
long CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case UWM_DELETE_DRYER_SETTING_LIST:
		{
			switch (wParam){
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// Delete the dryer settings item
					SetControlItem(m_ctl[CTRLID_LB_DRYER_SETTINGS], lParam, NULL);
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// clear all items
					while(NULL != (LBITEMINFO*)GetControlItem(m_ctl[CTRLID_LB_DRYER_SETTINGS], 0)){
						SetControlItem(m_ctl[CTRLID_LB_DRYER_SETTINGS], 0, NULL);
					}

					// registers list of items for dryer settings list
					RegisterItems();
				}
				break;
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
long CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]button/checkbox clicked : wParam == BN_CLICKED
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case LBN_SELCHANGE:
		{
			// Dryer settings list-box
			if(ctlWnd == m_ctl[CTRLID_LB_DRYER_SETTINGS]){
				return DEF_UPDATE_CONTROL;
			}
		}
		break;
	case BN_CLICKED:
		{
			// Delete button
			if(ctlWnd == m_ctl[CTRLID_BN_DELETE]){
				vector<long> listSelectionItems;			//!< Array use to save the selection items.

				char msg[1024] = {0};
				_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_DELETE_CONFIRMATION, LoadResourceString(IDM_DELETE_CONFIRMATION, RESOURCE_MSG_STR));

				const char* strDryerSetting = NULL;			//!< Name of dryer setting

				long delCount = 0;					//!< count number of items is selected

				for(long itemID = 0; itemID < m_numOfItems; ++itemID){
					if(TRUE == (*(BOOL*)GetControlOption(m_ctl[CTRLID_LB_DRYER_SETTINGS], itemID))){
						++delCount;

						// Save the index of selection item.
						listSelectionItems.push_back(itemID);

						// Add name of item to message
						if(delCount <= DEF_MAX_ITEMS_DISPLAY){
							// Get the name of item
							pData->GetDeleteDryerSettingList_DelDlg(itemID, &strDryerSetting);

							// Add the name of Dryer Setting item to message
							strncat(msg, strDryerSetting, DEF_MAX_CHARS_APPENDED(msg));
							strncat(msg, "\n", DEF_MAX_CHARS_APPENDED(msg));

							strDryerSetting = NULL;
						}
					}
				}

				// In case the selection items are more than DEF_MAX_ITEMS_DISPLAY items
				if(delCount > DEF_MAX_ITEMS_DISPLAY){
					// Add the number of the items which did not display
					char temp[64] = {0};
					_snprintf(temp, sizeof(temp) - 1, LoadResourceString(IDM_OTHER_ITEMS, RESOURCE_MSG_STR), delCount - DEF_MAX_ITEMS_DISPLAY); 

					strncat(msg, temp, DEF_MAX_CHARS_APPENDED(msg));
					strncat(msg, "\n", DEF_MAX_CHARS_APPENDED(msg));
				} 

				// Display message box
				if(IDYES == ShowMessageBox(msg, MBST_YESNO | MBST_ICONINFORMATION, NULL)){
					// Register to delete dryer setting
					unsigned long sizeOfListSelectionItems = listSelectionItems.size();
					for(unsigned long index = 0; index < sizeOfListSelectionItems; ++index){
						pData->SetDeleteDryerSetting_DelDlg(listSelectionItems.at(index));
					}

					// delete dryer settings
					pData->DeleteDryerSettings_DelDlg();
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::OnUpdateState()
{
	if(m_ctl){
		DWORD dwState = CST_SHOW;
		for(long ctlID = 0; ctlID < CTRLID_COUNT; ++ctlID){
			dwState = CST_SHOW;

			// Delete button
			if(ctlID == CTRLID_BN_DELETE){

				long selectCount = 0;
				for(long itemID = 0; itemID < m_numOfItems; ++itemID){
					if(TRUE == (*(BOOL*)GetControlOption(m_ctl[CTRLID_LB_DRYER_SETTINGS], itemID))){
						++selectCount;
						break;
					}
				}

				// Dryer settings list-box has selection items.
				if(0 != selectCount){
					dwState |= CST_ENABLED;
				}
				// Dryer settings list-box is non-selection.
				else{
					dwState |= CST_DISABLED;
				}
			}

			// Set control state
			SetControlState(m_ctl[ctlID], dwState);
		}
	}
}

/*! \brief register all item for list box
*/
void CCtlDeleteDryerSettingsSelection_DlgDeleteDryer::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	LBITEMINFO lb = {0};
	lb.nMask = OT_TEXT | OT_PARAM;

	m_numOfItems = 0;
	const char* strDryerName = NULL;				//!< Dryer settings name
	// Get dryer settings name
	while(pData->GetDeleteDryerSettingList_DelDlg(m_numOfItems, &strDryerName)){
		// add a job item
		lb.lpszText = (char*)strDryerName;
		SetControlItem(m_ctl[CTRLID_LB_DRYER_SETTINGS], m_numOfItems++, &lb);
		strDryerName = NULL;
	}

	SetControlData(m_ctl[CTRLID_LB_DRYER_SETTINGS], -1);
}
