/*! \file DataPrintHeadAlignment.cpp
 *  \brief data class: Print head alingment
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include <sstream>

#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataPrintHeadAlignment.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "DataIF.h"

#include "ProhibitChecker.h"
#include "PrintConditionGUIUtility.h"

// ==========================================================================

/**
 * constructor
 * @note General process
 * 
 *		Initialize the member variables
**/
CDataPrintHeadAlignment::CDataPrintHeadAlignment()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataPrintHeadAlignment::~CDataPrintHeadAlignment()
{
	ResetMember();
}

/**
 *	Initialzie menber variables in the class
 *	@param[in]	inSystemSetting : system setting data
 *	@retval	TRUE: success
 *	@retval	FALSE: failure
 *	@note Notes
 *	
 *		The methods to initialize each member variables are written at each getter methods.
**/
bool CDataPrintHeadAlignment::Initialize(CDataSystemSetting* inSystemSetting)
{
	if (!inSystemSetting) {
		return false;
	}

	// Store system setting handle
	m_systemSetting = inSystemSetting;
	m_JM_Callbacks = m_systemSetting->GetJobManager_Callbacks();
	m_PaperDB_Callbacks = m_systemSetting->GetPaperDB_Callbacks();
#ifndef PRINTER_PROFILE
	m_PrintProfile_Callbacks = m_systemSetting->GetPrintProfile_Callbacks();
#endif
	return true;
}

/**
 *	Remove menber variables in the class
 *	@retval	TRUE: success
 *	@retval	FALSE: failure
**/
bool CDataPrintHeadAlignment::Finalize()
{
	return true;
}

/**
 *	Reset menber variables in the class
 *	@note General process
 *	
 *		Reset the member variables in the class
 *		Remove objects which created with dynamic.
**/
void CDataPrintHeadAlignment::ResetMember()
{
	m_systemSetting = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
#ifndef PRINTER_PROFILE
	m_PrintProfile_Callbacks = NULL;
#endif
	m_printHeadAlignmentList.clear();
	m_selectedID = 0;
	m_selectedID_DlgAdd = 0;
	m_printHeadName_DlgAdd = "";
	m_listPrintHead_DlgDelete.clear();
	m_hwnd_DlgDelete = NULL;
	m_currHeadAlignmentName = "";
	m_printConditionName = "";
	m_isNotSetCurrentAlignment = false;
}

/**
 *	Check the user change the head alignment
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataPrintHeadAlignment::IsChanged()
{
	if (m_selectedID >= 0
		&& m_selectedID < (long)m_printHeadAlignmentList.size()
		&& m_printHeadAlignmentList.at(m_selectedID).compare(m_currHeadAlignmentName) != 0)
	{
		return true;
	}

	return false;
}

/**
 *	Check the control has valid selection item
 *	@retval	true : valid
 *	@retval	false : invalid
 *	@note
 *	
 *		
**/
bool CDataPrintHeadAlignment::IsHaveValidSelectedItem()
{
	if (m_printHeadAlignmentList.size() == 0
		|| m_selectedID < 0
		|| m_selectedID >= (long)m_printHeadAlignmentList.size())
	{
		return false;
	}

	return true;
}

/**
 *	Notify to save head alignment for Print Condition
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note
 *	
 *		
**/
bool CDataPrintHeadAlignment::Save()
{
	bool isSucceed = true;
	if (IsChanged())
	{
		if (m_systemSetting->IsStartedFromJobListScreen())
		{
			isSucceed = m_JM_Callbacks->JM_SetLevelTune(m_printConditionName.c_str(), m_printHeadAlignmentList.at(m_selectedID)); 
		}
		else
		{
			isSucceed = m_PaperDB_Callbacks->PDB_SetLevelTuneName(m_printConditionName.c_str(), m_printHeadAlignmentList.at(m_selectedID).c_str());
		}

		if(isSucceed) {
			UpdateCurrentSelectionPrintHeadAlignment(m_printConditionName.c_str());
		}
	}
	return isSucceed;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outPrintHeadAlignmentName : 段差補正名
*/
void CDataPrintHeadAlignment::OnCopyJobPrintCondition(std::string &outPrintHeadAlignmentName)
{
	outPrintHeadAlignmentName = m_printHeadAlignmentList.at(m_selectedID);
}

/**
 *	Get an item of head alignment list
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outAlignment : a pointer to get a head alignment
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::GetPrintHeadAlignmentList(long inItemID, const char** outAlignment)
{
	if (inItemID < 0 || inItemID >= (long)m_printHeadAlignmentList.size() || !outAlignment)
	{
		return false;
	}

	*outAlignment = m_printHeadAlignmentList.at(inItemID).c_str();

	return true;
}

/**
 *	Set a selection head alignment
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::SetSelectPrintHeadAlignment(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_printHeadAlignmentList.size())
	{
		return;
	}

	m_selectedID = inItemID;
	//Remove blank item if has
	if(m_isNotSetCurrentAlignment && (inItemID != m_printHeadAlignmentList.size() - 1)) {
		m_printHeadAlignmentList.erase(m_printHeadAlignmentList.end() - 1);
//		SendMessage(GetGenSetupDialog(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentAlignment = false;
	}
}

/**
 *	Notify to initialize Add head alignment dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::InitAddPrintHeadAlignmentDlg()
{
	// Default: Setting selected at
	m_selectedID_DlgAdd = m_selectedID;

	// Init head alignment list for Add dialog
	InitPrintHeadAlignmentList_DlgAdd();

	return true;
}

/**
 *	Notify to initialize Delete head alignmet dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::InitDeletePrintHeadAlignmentDlg()
{
	InitPrintHeadAlignmentList_DlgDelete();
	return true;
}

/**
 *	Get a selection head alignment
 *	@param[out]	outItemID : an item ID (0-N)
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::GetSelectPrintHeadAlignment(long *outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_selectedID;

	return true;
}

/**
 *	Initialize Print head alignment list
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::InitPrintHeadAlignmentList()
{
	// Clear print head alignment list
	m_printHeadAlignmentList.clear();

	int nItemID = 0;
	string strPrintHead ("");

	if (false == m_PaperDB_Callbacks->PDB_EnterCommonPaperDBCS())
		return;

	// Call PaperDB to get a system print head alignment
	while (true == m_PaperDB_Callbacks->PDB_GetSystemLevelTuneName(nItemID++, strPrintHead))
	{
		// Store system print head alignment to member variable
		m_printHeadAlignmentList.push_back(strPrintHead);
		strPrintHead = "";
	}

	m_PaperDB_Callbacks->PDB_LeaveCommonPaperDBCS();
}

/**
 *	Update the current selection head alignment in case the user change the print condition name
 *	@param[in]	inKeyName : key name
 *	@note Notes
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
void CDataPrintHeadAlignment::UpdateCurrentSelectionPrintHeadAlignment(const char* inKeyName)
{
	if (!inKeyName)
	{
		return;
	}

	m_printConditionName = inKeyName;

	bool isSuccess = false;

	m_currHeadAlignmentName = "";
	// The Print Condition GUI is started from the Job List Screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		isSuccess = m_JM_Callbacks->JM_GetLevelTune(inKeyName, m_currHeadAlignmentName);
	}
	// started from System Setting Screen
	else
	{
		isSuccess = m_PaperDB_Callbacks->PDB_GetLevelTuneName(inKeyName, m_currHeadAlignmentName);
	}

	m_selectedID = -1;
	vector<string>::iterator it = std::find(m_printHeadAlignmentList.begin(), m_printHeadAlignmentList.end(), m_currHeadAlignmentName);
	if (isSuccess && m_printHeadAlignmentList.end() != it)
	{
		m_selectedID = std::distance(m_printHeadAlignmentList.begin(), it);
		//Set to delete item blank if has.
		if(m_isNotSetCurrentAlignment)
			SetSelectPrintHeadAlignment(m_selectedID);
	}else {
		//Only has one item
		if(1 == m_printHeadAlignmentList.size()) {
			m_selectedID = 0;
		}
		// > 1 item
		else {
			if(!m_isNotSetCurrentAlignment) {
				string emptyPaperType = "";
				m_printHeadAlignmentList.push_back(emptyPaperType);
				m_selectedID = m_printHeadAlignmentList.size() - 1;
				m_isNotSetCurrentAlignment = true;
//				SendMessage(GetGenSetupDialog(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
				SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
			}else {
				m_selectedID = m_printHeadAlignmentList.size() - 1;
			}
		}
	}
}

/**
 *	Set a head alignment name to add it at Add head alignment dialog
 *	@param[in]	inAlignment : head alignment name
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::SetAddPrintHeadAlignmentName_AddDlg(char* inAlignment)
{
	if (!inAlignment || strlen(inAlignment) == 0)
	{
		return;
	}

#if 0
// 禁止文字チェックは不要になったが、実装は残す
	bool hasProhibit = ProhibitChecker::hasProhibitChar(inAlignment);
	if (hasProhibit) {
		std::stringstream ss;
		ss << ID_MESSAGE_ERROR +  IDM_ERR_PROHIBITCHARS_LEVELTUNE_NAME << "\n" << LoadResourceString(IDM_ERR_PROHIBITCHARS_LEVELTUNE_NAME, RESOURCE_MSG_STR) << "\n" << ProhibitChecker::prohibitString();
		ShowMessageBox(const_cast<char*>(ss.str().c_str()), MBST_OK | MBST_ICONERROR, NULL);
		return;
	}
#endif

	//!< 半角スペースを除去した文字列をセット
	m_printHeadName_DlgAdd = PrintConditionGUIUtility::TrimLastSpace(inAlignment);
}

/**
 *	Get a head alignment name to add it at Add head alignment dialog.
 *	@retval : head alignment name
 *	@note Notes
 *	
 *		
**/
const char* CDataPrintHeadAlignment::GetAddPrintHeadAlignmentName_AddDlg()
{
	return m_printHeadName_DlgAdd.c_str();
}

/**
 *	Get an item of head alignment list for Add head alignment dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outAlignment : a pointer to get a head alignment
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::GetPrintHeadAlignmentList_AddDlg(long inItemID, const char** outAlignment)
{
	if (inItemID < 0 || inItemID >= (long)m_printHeadAlignmentList.size() || !outAlignment)
	{
		return false;
	}

	*outAlignment = m_printHeadAlignmentList.at(inItemID).c_str();
	return true;
}

/**
 *	Set a selection head alignment for Add head alignment dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::SetSelectPrintHeadAlignment_AddDlg(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_printHeadAlignmentList.size())
	{
		return;
	}

	m_selectedID_DlgAdd = inItemID;

	// Init paper type name
	InitPrintHeadAlignmentList_DlgAdd();
}

/**
 *	Get a selection head alignment for Add head alignment dialog
 *	@param[out]	outItemID : an item ID (0-N)
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::GetSelectPrintHeadAlignment_AddDlg(long* outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_selectedID_DlgAdd;
	return true;
}

/**
 *	Save the settings of Add head alignment dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::SaveAddPrintHeadAlignmentDlg()
{
	// Check for duplicate names
	// The head alignment name is existed
	if (m_PaperDB_Callbacks->PDB_IsSystemLevelTuneName(m_printHeadName_DlgAdd.c_str()))
	{
		char msg[256] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_DUPLICATE_NAME_EXISTS, LoadResourceString(IDM_DUPLICATE_NAME_EXISTS, RESOURCE_MSG_STR));
		// Show messagebox
		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
		return false;
	}

	bool isSuccess = false;

	// Case: Print head alignment list has items
	if (m_selectedID_DlgAdd >= 0 && m_selectedID_DlgAdd < (long)m_printHeadAlignmentList.size())
	{
		/* 2014.06.20 s.mizukami add:
			add progress bar. When there is many registration of adjustment data, processing takes time.  */
		long progressID = 0;
		SIZE sizeDialog = {600, 0};
		char lStr[256]  ={0};
		_snprintf_s(lStr, _TRUNCATE, "%s", (char*)LoadResourceString(IDS_DUPLICATING, RESOURCE_STR));

		progressID = ShowProgressDialog(sizeDialog, PROGDLG_INDEFINITE_BAR | PROGDLG_MSG_CENTER, 0, 100, 0, NULL, lStr);

		// Call PrintProfile_Duplicate
#ifdef PRINTER_PROFILE
		if (_PrintProfile_Duplicate_OP(kPrintProfile_Kind_HeadAlignment, m_printHeadAlignmentList.at(m_selectedID_DlgAdd).c_str(), m_printHeadName_DlgAdd.c_str())
			&& _PrintProfile_Duplicate_OP(kPrintProfile_Kind_RegisterSetting, m_printHeadAlignmentList.at(m_selectedID_DlgAdd).c_str(), m_printHeadName_DlgAdd.c_str()))
#else
		if (m_PrintProfile_Callbacks->Duplicate(kPrintProfile_Kind_HeadAlignment, m_printHeadAlignmentList.at(m_selectedID_DlgAdd).c_str(), m_printHeadName_DlgAdd.c_str())
			&& m_PrintProfile_Callbacks->Duplicate(kPrintProfile_Kind_RegisterSetting, m_printHeadAlignmentList.at(m_selectedID_DlgAdd).c_str(), m_printHeadName_DlgAdd.c_str()))
#endif
		{
			isSuccess = true;
		}

		if(0 != progressID) {
			CloseProgressDialog(progressID);
			progressID = 0;
		}
	}
	// Case: Print head alignment list doesn't have any items
	else
	{
#ifdef PRINTER_PROFILE
		if (_PrintProfile_AddPrintMode_OP(kPrintProfile_Kind_HeadAlignment, m_printHeadName_DlgAdd.c_str())
			&& _PrintProfile_AddPrintMode_OP(kPrintProfile_Kind_RegisterSetting, m_printHeadName_DlgAdd.c_str()))
#else
		if (m_PrintProfile_Callbacks->AddPrintMode(kPrintProfile_Kind_HeadAlignment, m_printHeadName_DlgAdd.c_str())
			&& m_PrintProfile_Callbacks->AddPrintMode(kPrintProfile_Kind_RegisterSetting, m_printHeadName_DlgAdd.c_str()))
#endif
		{
			isSuccess = true;
		}
	}

	if (isSuccess)
	{
		// Add system head alignment
		if (m_PaperDB_Callbacks->PDB_AddSystemLevelTuneName(m_printHeadName_DlgAdd.c_str()))
		{
			//紙DBにコミットする。
			m_PaperDB_Callbacks->PDB_CommitPaperDB();
			// Init print head alignment list
			InitPrintHeadAlignmentList();
			// Update current selection item
			UpdateSelectionPrintHeadAlignmentID(m_printHeadName_DlgAdd);
			// Notify to GUI to update the print head alignment list
//			::PostMessage(GetGenSetupDialog(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
			::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		}

		// Reset member of Add dialog
		CancelAddPrintHeadAlignmentDlg();
	}
	else
	{
		char msg[256] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, LoadResourceString(IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, RESOURCE_MSG_STR));
		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
	}

	return isSuccess;
}

/**
 *	Cancel to save the settings of Add head alignment dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::CancelAddPrintHeadAlignmentDlg()
{
	// Reset member
	m_printHeadName_DlgAdd = "";
	m_selectedID_DlgAdd = 0;

	return true;
}

/**
 *	Set the dialog handle of Delete head alignment dialog
 *	@param[in]	inDialog : the dialog handle
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::SetDeletePrintHeadAlignmentDlg(HWND inDialog)
{
	m_hwnd_DlgDelete = inDialog;
}

/**
 *	Get a paper type for Delete paper type dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outAlignment : pointer to head alignment name
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::GetDeletePrintHeadAlignmentList_DelDlg(long inItemID, const char** outAlignment)
{
	if (inItemID < 0 || inItemID >= (long)m_listPrintHead_DlgDelete.size() || !outAlignment)
	{
		return false;
	}

	*outAlignment = m_listPrintHead_DlgDelete.at(inItemID).m_headAlignment.c_str();

	return true;
}

/**
 *	Register a head alignment to delete at Delete head alignment dialog.
 *	@param[in]	inItemID : an item ID (0-N)
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::SetDeletePrintHeadAlignment_DelDlg(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_listPrintHead_DlgDelete.size())
	{
		return false;
	}

	m_listPrintHead_DlgDelete.at(inItemID).m_isDelete = true;

	return true;
}

/**
 *	Delete the registered items to delete at Delete head alignment dialog.
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::DeletePrintHeadAlignments_DelDlg()
{
	vector<string> listError;

	// Backup name of current selected head alignment
	string curPrintHead = "";

	if (m_selectedID >= 0 && m_selectedID < (long)m_printHeadAlignmentList.size())
	{
		curPrintHead = m_printHeadAlignmentList.at(m_selectedID);
	}

	/* 2014.06.20 s.mizukami add:
		add progress bar. When there is many registration of adjustment data, processing takes time.  */
	long progressID = 0;
	SIZE sizeDialog = {600, 0};
	char lStr[256]  ={0};
	_snprintf_s(lStr, _TRUNCATE, "%s", (char*)LoadResourceString(IDS_DELETING, RESOURCE_STR));

	progressID = ShowProgressDialog(sizeDialog, PROGDLG_INDEFINITE_BAR | PROGDLG_MSG_CENTER, 0, 100, 0, NULL, lStr);

	bool isSuccess = true;
	bool isCommit = false;

	for (vector<ST_HEAD_ALIGNMENT_DELETE_INFO>::iterator it = m_listPrintHead_DlgDelete.begin(); it != m_listPrintHead_DlgDelete.end(); ++it)
	{
		if (it->m_isDelete)
		{
			// IPDSモードの場合、JobManagerが存在しないため該当の関数を実行回避
			bool isExistLevelTune = false;
			if (m_systemSetting->IsIpdsMode() == FALSE)
			{
				if(eJM_IsExist_None != m_JM_Callbacks->JM_ISLevelTune(it->m_headAlignment))
				{
					isExistLevelTune = true;
				}
			}

			if (m_PaperDB_Callbacks->PDB_IsSystemLevelTuneNameInPrintCondition(it->m_headAlignment.c_str())
				|| isExistLevelTune == true)
			{
				listError.push_back(it->m_headAlignment);
			}
			else
			{
				const char* strHeadAlignment = it->m_headAlignment.c_str();
#ifdef PRINTER_PROFILE
				if (_PrintProfile_Remove_OP(kPrintProfile_Kind_HeadAlignment, strHeadAlignment)
					&& _PrintProfile_Remove_OP(kPrintProfile_Kind_RegisterSetting, strHeadAlignment))
#else
				if (m_PrintProfile_Callbacks->Remove(kPrintProfile_Kind_HeadAlignment, strHeadAlignment)
					&& m_PrintProfile_Callbacks->Remove(kPrintProfile_Kind_RegisterSetting, strHeadAlignment))
#endif
				{
					if (false == m_PaperDB_Callbacks->PDB_DelSystemLevelTuneName(strHeadAlignment))
					{
						isSuccess = false;
					} else {
						isCommit = true;
					}
				}
			}
		}
	}

	//紙DBが更新されている場合、コミットする。
	if (isCommit) {
		m_PaperDB_Callbacks->PDB_CommitPaperDB();
	}

	if(0 != progressID) {
		CloseProgressDialog(progressID);
		progressID = 0;
	}

	UINT numOfPaperTypeError = listError.size();

	if (numOfPaperTypeError > 0)
	{
		char msg[1024] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_NOT_DELETE_HEAD_ALIGNMENT, LoadResourceString(IDM_NOT_DELETE_HEAD_ALIGNMENT, RESOURCE_MSG_STR));

		UINT numOfDisplayItems = numOfPaperTypeError > 10 ? 10 : numOfPaperTypeError;
		for (UINT errorID = 0; errorID < numOfDisplayItems; ++errorID)
		{
			strcat_s(msg, _TRUNCATE, "\n");
			strcat_s(msg, _TRUNCATE, listError.at(errorID).c_str());
		}

		if (numOfPaperTypeError > 10)
		{
			char tmp[64] = {0};
			_snprintf_s(tmp, _TRUNCATE, LoadResourceString(IDM_OTHER_ITEMS, RESOURCE_MSG_STR), numOfPaperTypeError - 10);
			strcat_s(msg, _TRUNCATE, "\n");
			strcat_s(msg, _TRUNCATE, tmp);
		}

		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
	}

	// Init Print Head alignment list
	InitPrintHeadAlignmentList();
	// Update current selection head alignment ID
	UpdateSelectionPrintHeadAlignmentID(curPrintHead);
	// Init print head alignment list for Delete dialog
	InitPrintHeadAlignmentList_DlgDelete();
	::SendMessage(m_hwnd_DlgDelete, UWM_DELETE_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);

	if (false == isSuccess)
	{
		char msg[256] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, LoadResourceString(IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, RESOURCE_MSG_STR));
		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
	}

	return true;
}

/**
 *	Notify to exit Delete head alignment dialog.
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::ExitDeletePrintHeadAlignmentDlg()
{
	// Reset member variables
	m_listPrintHead_DlgDelete.clear();
	m_hwnd_DlgDelete = NULL;

//	::PostMessage(GetGenSetupDialog(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_PRINT_HEAD_ALIGNMENT_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	return true;
}

/**
 *	Initialize a new print head alignment name for Add head alignment dialog
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::InitPrintHeadAlignmentList_DlgAdd()
{
	m_printHeadName_DlgAdd = "";
	if (m_printHeadAlignmentList.size() > 0 && m_selectedID_DlgAdd != -1)
	{
		long nIndex = 0;
		char tmpHeadAlignment[256] = {0};

		while (true)
		{
			memset(tmpHeadAlignment, 0x00, sizeof(tmpHeadAlignment));
			_snprintf_s(tmpHeadAlignment, _TRUNCATE, "%s_%d", m_printHeadAlignmentList.at(m_selectedID_DlgAdd).c_str(), ++nIndex);
			// Check the head alignment name does not exist
			if (false == m_PaperDB_Callbacks->PDB_IsSystemLevelTuneName(tmpHeadAlignment))
			{
				m_printHeadName_DlgAdd = tmpHeadAlignment;
				break;
			}
		}
	}
}

/**
 *	Check and update the selection item for head alignment list after add new head alignment
 *	@param[in]	inPrintHeadAglignment : head alignment name
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::UpdateSelectionPrintHeadAlignmentID(string inPrintHeadAglignment)
{
	if (m_printHeadAlignmentList.size() == 0
		|| inPrintHeadAglignment.length() == 0)
	{
		return;
	}

	m_selectedID = -1;

	// Find the index of head alignment
	vector<string>::iterator it = std::find(m_printHeadAlignmentList.begin(), m_printHeadAlignmentList.end(), inPrintHeadAglignment);
	// The print head alignment list has inPrintHeadAglignment item
	if (m_printHeadAlignmentList.end() != it)
	{
		m_selectedID = std::distance(m_printHeadAlignmentList.begin(), it);
	}
	// The print head alignment list doesn't have inPrintHeadAglignment item
	else
	{
		// If inPrintHeadAglignment is not the current setting head alignment name
		if (m_currHeadAlignmentName.compare(inPrintHeadAglignment) != 0)
		{
			// Update current selection id with the current setting paper type
			UpdateSelectionPrintHeadAlignmentID(m_currHeadAlignmentName);
		}
	}

	if(-1 != m_selectedID && m_isNotSetCurrentAlignment) {
		m_isNotSetCurrentAlignment = false;
	}
}

/**
 *	Init Head alignment list for Dlg Delete
 *	@note Notes
 *	
 *		
**/
void CDataPrintHeadAlignment::InitPrintHeadAlignmentList_DlgDelete()
{
	m_listPrintHead_DlgDelete.clear();
	ST_HEAD_ALIGNMENT_DELETE_INFO paperType;
	for (vector<string>::iterator it = m_printHeadAlignmentList.begin(); it != m_printHeadAlignmentList.end(); ++it)
	{
		paperType.m_headAlignment = *it;
		paperType.m_isDelete = false;
		m_listPrintHead_DlgDelete.push_back(paperType);
	}

	// Find index of the current setting of print head alignment
	vector<string>::iterator it_CurHeadAlignment = std::find(m_printHeadAlignmentList.begin(), m_printHeadAlignmentList.end(), m_currHeadAlignmentName);
	if (m_printHeadAlignmentList.end() != it_CurHeadAlignment)
	{
		m_listPrintHead_DlgDelete.erase(m_listPrintHead_DlgDelete.begin() + std::distance(m_printHeadAlignmentList.begin(), it_CurHeadAlignment));
	}
};

/**
 *	Is used to initialize data when the Print Condition GUI start.
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Init system paper head alignment list
	InitPrintHeadAlignmentList();

	// Init current selection head alignment
	UpdateCurrentSelectionPrintHeadAlignment(inPrintConditionOrSectionID);

	return true;
}

/**
 *	Release data when the Print Condition GUI is exited.
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note Notes
 *	
 *		
**/
bool CDataPrintHeadAlignment::ExitScreen()
{
	m_printHeadAlignmentList.clear();
	m_selectedID = 0;
	m_currHeadAlignmentName = "";
	m_printConditionName = "";
	return true;
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataPrintHeadAlignment::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentAlignment)
		WriteToLogBuf(LOG_DEBUG, "Printhead alignment is judged as blank.");
	return m_isNotSetCurrentAlignment;
}