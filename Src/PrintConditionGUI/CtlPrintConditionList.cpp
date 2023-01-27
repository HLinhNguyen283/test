/*! \file CtlPrintConditionList.cpp
 *  \brief The control selects to delete items of Print settings
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintConditionList.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include <vector>
#include <algorithm>
#include <sstream>
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
	CTRLID_LB_PRINT_CONDITION,				//!< list-box: Print settings
	CTRLID_BN_DELETE,						//!< button: delete

	CTRLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_delete = "CTRLID_BN_DELETE";
// ==========================================================================

/*! \brief constructor
*/
CCtlPrintConditionList::CCtlPrintConditionList()
{
	//set to create the number of control.
	m_numOfItems = 0;

	m_ctlCount = CTRLID_COUNT;
}

/*! \brief destructor
*/
CCtlPrintConditionList::~CCtlPrintConditionList()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPrintConditionList::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//list-box: Print settings
	{
		long ctlId = CTRLID_LB_PRINT_CONDITION;
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
void CCtlPrintConditionList::OnCreateItem()
{
	// register all item of job
	RegisterItems();
	
	SetControlData(m_ctl[CTRLID_LB_PRINT_CONDITION], -1);
}

/*! \brief ウィンドウプロシージャ
 *  \param hWnd ウィンドウハンドル
 *  \param Message メッセージ番号
 *  \param wParam パラメータ
 *  \param lParam パラメータ
 *  \return DEF_NONE
*/
long CCtlPrintConditionList::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case UWM_DELETE_PRINT_CONDITION_LIST:
		{
			switch (wParam){
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// Delete the paper type item
					SetControlItem(m_ctl[CTRLID_LB_PRINT_CONDITION], lParam, NULL);
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// clear all items
					while(NULL != (LBITEMINFO*)GetControlItem(m_ctl[CTRLID_LB_PRINT_CONDITION], 0)){
						SetControlItem(m_ctl[CTRLID_LB_PRINT_CONDITION], 0, NULL);
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
long CCtlPrintConditionList::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Print settings list-box
			if(ctlWnd == m_ctl[CTRLID_LB_PRINT_CONDITION]){
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

				const char* strPrintConditionName = NULL;			//!< Name of print condition

				long delCount = 0;					//!< count number of items is selected

				std::vector<std::string> strRunningJobPrintConditionNames;			//!< Print condition names of running job
				std::vector<std::string> listPrintConditionCannotDelete;		//!< The list of print condtion name cannot be deleted
				bool isSuccess = false;

				//get print condition names of running job
				if (pData->IsJobRunning()){
					isSuccess = pData->GetPrintConditionNamesOfRunningJob(strRunningJobPrintConditionNames);
				}

				for(long itemID = 0; itemID < m_numOfItems; ++itemID){
					if(TRUE == (*(BOOL*)GetControlOption(m_ctl[CTRLID_LB_PRINT_CONDITION], itemID))){
						// Get the name of item
						pData->GetDeletePrintConditionList_DelDlg(itemID, &strPrintConditionName);

						if (pData->IsCancelDeleteBasePrintCondition(strPrintConditionName)) {
							//対象が業務移行時の基準印刷条件の場合、確認した結果がキャンセルの場合、処理中断。
							return DEF_NONE;
						}
						
						//print condition cannot be deleted if it has the same name with the print condition of the job being printed or paused
						if (isSuccess && strRunningJobPrintConditionNames.end() != ::find(strRunningJobPrintConditionNames.begin(), strRunningJobPrintConditionNames.end(), strPrintConditionName)) {
							listPrintConditionCannotDelete.push_back(strPrintConditionName);

							strPrintConditionName = NULL;
							continue;
						}

						//ジョブに使われている印刷条件名は削除できない
						if (pData->IsIpdsMode()) {
							//IPDSモードの場合は何もしない
						}
						else {
							std::string usingJobName = "";
							if (pData->IsPrintConditionUsedForJobs(strPrintConditionName, usingJobName)){
								std::stringstream baseMsg;
								baseMsg << ID_MESSAGE_ERROR + IDM_ERR_CANNOT_DELETE_PRINT_SETTING << "\n" << LoadResourceString(IDM_ERR_CANNOT_DELETE_PRINT_SETTING, RESOURCE_MSG_STR);
								std::string msg = baseMsg.str();
								size_t pos = msg.find("%s");
								if (pos != std::string::npos) {
								   msg.replace(pos, 2, strPrintConditionName);
								}
								pos = msg.find("%s");
								if (pos != std::string::npos) {
								   msg.replace(pos, 2, usingJobName);
								}
								ShowMessageBox(const_cast<char*>(msg.c_str()), MBST_OK | MBST_ICONERROR, NULL);

								strPrintConditionName = NULL;
								continue;
							}
						}

						++delCount;

						// Save the index of selection item.
						listSelectionItems.push_back(itemID);

						// Add name of item to message
						if(delCount <= DEF_MAX_ITEMS_DISPLAY){
							// Add the Print condition name to message
							strncat(msg, strPrintConditionName, DEF_MAX_CHARS_APPENDED(msg));
							strncat(msg, "\n", DEF_MAX_CHARS_APPENDED(msg));

							strPrintConditionName = NULL;
						}
					}
				}

				if (listPrintConditionCannotDelete.size() > 0) {
					//show warning message to inform user cannot delete the print condition of printing job
					char msgWarning[1024] = {0};
					_snprintf_s(msgWarning, _TRUNCATE, "%d\n%s\n", ID_MESSAGE_ERROR + IDM_MSG_NOT_DELETE_PRINT_CONDITION, LoadResourceString(IDM_MSG_NOT_DELETE_PRINT_CONDITION, RESOURCE_MSG_STR));
					
					//append the list print conditions cannot be deleted to dialog message
					long numberItems = listPrintConditionCannotDelete.size();
					for(long index = 0; index < numberItems; ++index) {
						if (index < DEF_MAX_ITEMS_DISPLAY) {
							strncat(msgWarning, listPrintConditionCannotDelete.at(index).c_str(), DEF_MAX_CHARS_APPENDED(msgWarning));
							strncat(msgWarning, "\n", DEF_MAX_CHARS_APPENDED(msgWarning));
						} else {
							// Add the number of the items which did not display
							char temp[64] = {0};
							_snprintf_s(temp, _TRUNCATE, LoadResourceString(IDM_OTHER_ITEMS, RESOURCE_MSG_STR), numberItems - DEF_MAX_ITEMS_DISPLAY); 

							strncat(msgWarning, temp, DEF_MAX_CHARS_APPENDED(msgWarning));
							strncat(msgWarning, "\n", DEF_MAX_CHARS_APPENDED(msgWarning));
							break;
						}
					}
					ShowMessageBox(msgWarning, MBST_OK | MBST_ICONWARNING, NULL);
				}

				// In case the selection items are more than DEF_MAX_ITEMS_DISPLAY items
				if(delCount > DEF_MAX_ITEMS_DISPLAY){
					// Add the number of the items which did not display
					char temp[64] = {0};
					_snprintf(temp, sizeof(temp) - 1, LoadResourceString(IDM_OTHER_ITEMS, RESOURCE_MSG_STR), delCount - DEF_MAX_ITEMS_DISPLAY); 

					strncat(msg, temp, DEF_MAX_CHARS_APPENDED(msg));
					strncat(msg, "\n", DEF_MAX_CHARS_APPENDED(msg));
				}
				if (delCount > 0) {
					// Display message box
					if(IDYES == ShowMessageBox(msg, MBST_YESNO | MBST_ICONWARNING, NULL)){
						// Register to delete print settings
						unsigned long sizeOfListSelectionItems = listSelectionItems.size();
						for(unsigned long index = 0; index < sizeOfListSelectionItems; ++index){
							pData->SetDeletePrintCondition_DelDlg(listSelectionItems.at(index));
						}

						// delete print settings
						pData->DeletePrintConditions_DelDlg();
					}
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlPrintConditionList::OnUpdateState()
{
	if(m_ctl){
		DWORD dwState = CST_SHOW;
		for(long ctlID = 0; ctlID < CTRLID_COUNT; ++ctlID){
			dwState = CST_SHOW;

			// Delete button
			if(ctlID == CTRLID_BN_DELETE){

				long selectCount = 0;
				for(long itemID = 0; itemID < m_numOfItems; ++itemID){
					if(TRUE == (*(BOOL*)GetControlOption(m_ctl[CTRLID_LB_PRINT_CONDITION], itemID))){
						++selectCount;
						break;
					}
				}

				// Print settings list-box has selection items.
				if(0 != selectCount){
					dwState |= CST_ENABLED;
				}
				// Print settings list-box is non-selection.
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
void CCtlPrintConditionList::RegisterItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	LBITEMINFO lb = {0};
	lb.nMask = OT_TEXT | OT_PARAM;

	m_numOfItems = 0;
	const char* strPrintSettingName = NULL;				//!< Print settings name
	// Get Print setting name
	while(pData->GetDeletePrintConditionList_DelDlg(m_numOfItems, &strPrintSettingName)){
		// add a Print settings item
		lb.lpszText = (char*)strPrintSettingName;
		SetControlItem(m_ctl[CTRLID_LB_PRINT_CONDITION], m_numOfItems++, &lb);

		strPrintSettingName = NULL;
	}
}
