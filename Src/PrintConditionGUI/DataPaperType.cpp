/*! \file DataPaperType.cpp
 *  \brief data class: paper type
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
#include "DataPaperType.h"
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
CDataPaperType::CDataPaperType()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataPaperType::~CDataPaperType()
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
bool CDataPaperType::Initialize(CDataSystemSetting* inSystemSetting)
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
bool CDataPaperType::Finalize()
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
void CDataPaperType::ResetMember()
{
	m_systemSetting = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
	m_PrintProfile_Callbacks = NULL;
	m_paperTypeList.clear();
	m_selectPaperTypeID = 0;
	m_paperTypeName_DlgAdd = "";
	m_listPaperType_DlgDelete.clear();
	m_selectPaperTypeID_DlgAdd = 0;
	m_hwnd_DlgDelete = NULL;
	m_currPaperType = "";
	m_printConditionName = "";
	m_isNotSetCurrentPaperType = false;
}

/**
 *	Check the user change the paper type
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataPaperType::IsChanged()
{
	if (m_selectPaperTypeID >= 0
		&& m_selectPaperTypeID < (long)m_paperTypeList.size()
		&& m_paperTypeList.at(m_selectPaperTypeID).compare(m_currPaperType) != 0)
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
bool CDataPaperType::IsHaveValidSelectedItem()
{
	if (m_paperTypeList.size() == 0
		|| m_selectPaperTypeID < 0
		|| m_selectPaperTypeID >= (long)m_paperTypeList.size())
	{
		return false;
	}

	return true;
}

/**
 *	Notify to save paper type for Print Condition
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note
 *	
 *		
**/
bool CDataPaperType::Save()
{
	bool isSucceed = true;
	if (IsChanged())
	{
		if (m_systemSetting->IsStartedFromJobListScreen())
		{
			isSucceed = m_JM_Callbacks->JM_SetPaperType(m_printConditionName.c_str(), m_paperTypeList.at(m_selectPaperTypeID)); 
		}
		else
		{
			isSucceed = m_PaperDB_Callbacks->PDB_SetPaperType(m_printConditionName.c_str(), m_paperTypeList.at(m_selectPaperTypeID).c_str());
		}

		if(isSucceed) {
			UpdateCurrentSelectionPaperType(m_printConditionName.c_str());
		}
	}
	return isSucceed;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outPaperTypeName : 用紙種類
*/
void CDataPaperType::OnCopyJobPrintCondition(std::string &outPaperTypeName)
{
	outPaperTypeName = m_paperTypeList.at(m_selectPaperTypeID);
}

/**
 *	Get an item of paper type list
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outType : a pointer to get a paper type
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::GetPaperTypeList(long inItemID, const char** outType)
{
	if (inItemID < 0 || inItemID >= (long)m_paperTypeList.size() || !outType)
	{
		return false;
	}

	*outType = m_paperTypeList.at(inItemID).c_str();

	return true;
}

/**
 *	Set a selection paper type
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::SetSelectPaperType(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_paperTypeList.size())
	{
		return;
	}

	m_selectPaperTypeID = inItemID;

	//Remove blank item if has
	if(m_isNotSetCurrentPaperType  && (inItemID != m_paperTypeList.size() - 1)) {
		m_paperTypeList.erase(m_paperTypeList.end() - 1);
//		SendMessage(GetGenSetupDialog(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentPaperType = false;
	}

	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		if (IsChanged())
		{
			// show meesage box 
			char msg[256] = {0};
			_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_CHANGE_PAPER_TYPE_JOB_LIST_SCREEN, LoadResourceString(IDM_CHANGE_PAPER_TYPE_JOB_LIST_SCREEN, RESOURCE_MSG_STR));
			ShowMessageBox(msg, MBST_OK | MBST_ICONWARNING, NULL);
		}
	}
}

/**
 *	Notify to initialize Add paper type dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::InitAddPaperTypeDlg()
{
	// Default: Setting selected at
	m_selectPaperTypeID_DlgAdd = m_selectPaperTypeID;

	// Init paper type name for Add paper type dialog
	InitPaperTypeName_DlgAdd();

	return true;
}

/**
 *	Notify to initialize Delete paper type dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::InitDeletePaperTypeDlg()
{
	InitPaperTypeList_DlgDelete();
	return true;
}

/**
 *	Get a selection paper type
 *	@param[out]	outItemID : an item ID (0-N)
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::GetSelectPaperType(long *outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_selectPaperTypeID;

	return true;
}

/**
 *	Initialize Paper type list
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::InitPaperTypeList()
{
	// Clear paper type list
	m_paperTypeList.clear();

	int nItemID = 0;
	string strPaperType ("");

	if (false == m_PaperDB_Callbacks->PDB_EnterCommonPaperDBCS())
		return;

	// Call PaperDB to get a system paper type
	while (true == m_PaperDB_Callbacks->PDB_GetSystemPaperType(nItemID++, strPaperType))
	{
		// Store system paper type to member variable
		m_paperTypeList.push_back(strPaperType);
		strPaperType = "";
	}

	m_PaperDB_Callbacks->PDB_LeaveCommonPaperDBCS();
}

/**
 *	Update the current selection paper type in case the user change the print condition name
 *	@param[in]	inKeyName : key name
 *	@note Notes
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
void CDataPaperType::UpdateCurrentSelectionPaperType(const char* inKeyName)
{
	if (!inKeyName)
	{
		return;
	}

	// Backup the current Print Condition Name
	m_printConditionName = inKeyName;

	bool isSuccess = false;

	m_currPaperType = "";
	// The Print Condition GUI is started from the Job List Screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		isSuccess = m_JM_Callbacks->JM_GetPaperType(inKeyName, m_currPaperType);
	}
	// started from System Setting Screen
	else
	{
		string strPaperTypeName ("");
		isSuccess = m_PaperDB_Callbacks->PDB_GetPaperType(inKeyName, strPaperTypeName);
		m_currPaperType = strPaperTypeName;
	}

	m_selectPaperTypeID = -1;
	vector<string>::iterator it = std::find(m_paperTypeList.begin(), m_paperTypeList.end(), m_currPaperType);
	if (isSuccess && m_paperTypeList.end() != it)
	{
		m_selectPaperTypeID = std::distance(m_paperTypeList.begin(), it);
		//Set to delete item blank if has.
		if(m_isNotSetCurrentPaperType)
			SetSelectPaperType(m_selectPaperTypeID);
	}else {
		//Only has one item
		if(1 == m_paperTypeList.size()) {
			m_selectPaperTypeID = 0;
		}
		// > 1 item
		else {
			if(!m_isNotSetCurrentPaperType) {
				string emptyPaperType = "";
				m_paperTypeList.push_back(emptyPaperType);
				m_selectPaperTypeID = m_paperTypeList.size() - 1;
				m_isNotSetCurrentPaperType = true;
//				SendMessage(GetGenSetupDialog(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
				SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
			} else {
				m_selectPaperTypeID = m_paperTypeList.size() - 1;
			}
		}
	}
}

/**
 *	Set a paper type name to add it at Add paper type dialog
 *	@param[in]	inType : paper type name
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::SetAddPaperTypeName_AddDlg(char* inType)
{
	if (!inType || strlen(inType) == 0)
	{
		return;
	}

	bool hasProhibit = ProhibitChecker::hasProhibitChar(inType);
	if (hasProhibit) {
		std::stringstream ss;
		ss << ID_MESSAGE_ERROR +  IDM_ERR_PROHIBITCHARS_PAPERTYPE_NAME << "\n" << LoadResourceString(IDM_ERR_PROHIBITCHARS_PAPERTYPE_NAME, RESOURCE_MSG_STR) << "\n" << ProhibitChecker::prohibitString();
		ShowMessageBox(const_cast<char*>(ss.str().c_str()), MBST_OK | MBST_ICONERROR, NULL);
		return;
	}

	//!< 半角スペースを除去した文字列をセット
	m_paperTypeName_DlgAdd = PrintConditionGUIUtility::TrimLastSpace(inType);
}

/**
 *	Get a paper type name to add it at Add paper type dialog.
 *	@retval : paper type name
 *	@note Notes
 *	
 *		
**/
const char* CDataPaperType::GetAddPaperTypeName_AddDlg()
{
	return m_paperTypeName_DlgAdd.c_str();
}

/**
 *	Get an item of paper type list for Add paper type dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outType : a pointer to get a paper type
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::GetPaperTypeList_AddDlg(long inItemID, const char** outType)
{
	if (inItemID < 0 || inItemID >= (long)m_paperTypeList.size() || !outType)
	{
		return false;
	}

	*outType = m_paperTypeList.at(inItemID).c_str();
	return true;
}

/**
 *	Set a selection paper type for Add paper type dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::SetSelectPaperType_AddDlg(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_paperTypeList.size())
	{
		return;
	}

	m_selectPaperTypeID_DlgAdd = inItemID;

	// Init paper type name
	InitPaperTypeName_DlgAdd();
}

/**
 *	Get a selection paper type for Add paper type dialog
 *	@param[out]	outItemID : an item ID (0-N)
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::GetSelectPaperType_AddDlg(long* outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_selectPaperTypeID_DlgAdd;
	return true;
}

/**
 *	Save the settings of Add paper type dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::SaveAddPaperTypeDlg()
{
	// Check for duplicate names
	// The paper type name is existed
	if (m_PaperDB_Callbacks->PDB_IsSystemPaperType(m_paperTypeName_DlgAdd.c_str()))
	{
		char msg[256] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_DUPLICATE_NAME_EXISTS, LoadResourceString(IDM_DUPLICATE_NAME_EXISTS, RESOURCE_MSG_STR));
		// Show messagebox
		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
		return false;
	}

	bool isSuccess = false;

	// Case: Paper type list has items
	if (m_paperTypeList.size() > 0 && m_selectPaperTypeID_DlgAdd >= 0)
	{
		/* 2014.06.20 s.mizukami add:
			add progress bar. When there is many registration of adjustment data, processing takes time.  */
		long progressID = 0;
		SIZE sizeDialog = {600, 0};
		char lStr[256]  ={0};
		_snprintf_s(lStr, _TRUNCATE, "%s", (char*)LoadResourceString(IDS_DUPLICATING, RESOURCE_STR));

		progressID = ShowProgressDialog(sizeDialog, PROGDLG_INDEFINITE_BAR | PROGDLG_MSG_CENTER, 0, 100, 0, NULL, lStr);

		// Call PrintProfile_Duplicate
#ifdef	PRINTER_PROFILE
		if (_PrintProfile_Duplicate_OP(kPrintProfile_Kind_Shading, m_paperTypeList.at(m_selectPaperTypeID_DlgAdd).c_str(), m_paperTypeName_DlgAdd.c_str())
			&& _PrintProfile_Duplicate_OP(kPrintProfile_Kind_Linearize, m_paperTypeList.at(m_selectPaperTypeID_DlgAdd).c_str(), m_paperTypeName_DlgAdd.c_str()))
#else
		if (m_PrintProfile_Callbacks->Duplicate(kPrintProfile_Kind_Shading, m_paperTypeList.at(m_selectPaperTypeID_DlgAdd).c_str(), m_paperTypeName_DlgAdd.c_str())
			&& m_PrintProfile_Callbacks->Duplicate(kPrintProfile_Kind_Linearize, m_paperTypeList.at(m_selectPaperTypeID_DlgAdd).c_str(), m_paperTypeName_DlgAdd.c_str()))
#endif
		{
			isSuccess = true;
		}

		if(0 != progressID) {
			CloseProgressDialog(progressID);
			progressID = 0;
		}

	}
	// Case: Paper type list doesn't have any items
	else
	{
#ifdef	PRINTER_PROFILE
		if (_PrintProfile_AddPrintMode_OP(kPrintProfile_Kind_Shading, m_paperTypeName_DlgAdd.c_str())
			&& _PrintProfile_AddPrintMode_OP(kPrintProfile_Kind_Linearize, m_paperTypeName_DlgAdd.c_str()))
#else
		if (m_PrintProfile_Callbacks->AddPrintMode(kPrintProfile_Kind_Shading, m_paperTypeName_DlgAdd.c_str())
			&& m_PrintProfile_Callbacks->AddPrintMode(kPrintProfile_Kind_Linearize, m_paperTypeName_DlgAdd.c_str()))
#endif
		{
			isSuccess = true;
		}
	}

	if (isSuccess)
	{
		// Add system paper type
		if (m_PaperDB_Callbacks->PDB_AddSystemPaperType(m_paperTypeName_DlgAdd.c_str()))
		{
			//紙DBにコミットする。
			m_PaperDB_Callbacks->PDB_CommitPaperDB();
			// Init paper type list
			InitPaperTypeList();
			// Update current selection item
			UpdateSelectionPaperTypeID(m_paperTypeName_DlgAdd);
			// Notify to GUI to update the paper type list
//			::PostMessage(GetGenSetupDialog(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
			::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		}

		// Reset member of Add dialog
		CancelAddPaperTypeDlg();
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
 *	Cancel to save the settings of Add paper type dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::CancelAddPaperTypeDlg()
{
	// Reset member
	m_paperTypeName_DlgAdd = "";
	m_selectPaperTypeID_DlgAdd = 0;

	return true;
}

/**
 *	Set the dialog handle of Delete paper type dialog
 *	@param[in]	inDialog : the dialog handle
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::SetDeletePaperTypeDlg(HWND inDialog)
{
	m_hwnd_DlgDelete = inDialog;
}

/**
 *	Get a paper type for Delete paper type dialog
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outType : pointer to paper type name
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::GetDeletePaperTypeList_DelDlg(long inItemID, const char** outType)
{
	if (inItemID < 0 || inItemID >= (long)m_listPaperType_DlgDelete.size() || !outType)
	{
		return false;
	}

	*outType = m_listPaperType_DlgDelete.at(inItemID).m_paperType.c_str();

	return true;
}

/**
 *	Register a paper type to delete at Delete paper type dialog.
 *	@param[in]	inItemID : an item ID (0-N)
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::SetDeletePaperType_DelDlg(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_listPaperType_DlgDelete.size())
	{
		return false;
	}

	m_listPaperType_DlgDelete.at(inItemID).m_isDelete = true;

	return true;
}

/**
 *	Delete the registered items to delete at Delete paper type dialog.
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::DeletePaperTypes_DelDlg()
{
	vector<string> listError;

	// Backup name of current selected paper type
	string curPaperType = "";
	if (m_selectPaperTypeID >= 0 && m_selectPaperTypeID < (long)m_paperTypeList.size())
	{
		curPaperType = m_paperTypeList.at(m_selectPaperTypeID);
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

	for (vector<ST_PAPER_TYPE_DELETE_INFO>::iterator it = m_listPaperType_DlgDelete.begin(); it != m_listPaperType_DlgDelete.end(); ++it)
	{
		if (it->m_isDelete)
		{
			// IPDSモードの場合、JobManagerが存在しないため該当の関数を実行回避
			bool isExistPaperType = false;
			if (m_systemSetting->IsIpdsMode() == FALSE)
			{
				if (eJM_IsExist_None != m_JM_Callbacks->JM_ISPaperType(it->m_paperType))
				{
					isExistPaperType = true;
				}
			}

			if (m_PaperDB_Callbacks->PDB_IsSystemPaperTypeInPrintCondition(it->m_paperType.c_str())
				|| isExistPaperType == true)
			{
				listError.push_back(it->m_paperType);
			}
			else
			{
				const char* strPaperType = it->m_paperType.c_str();
#ifdef	PRINTER_PROFILE
				if (_PrintProfile_Remove_OP(kPrintProfile_Kind_Shading, strPaperType)
					&& _PrintProfile_Remove_OP(kPrintProfile_Kind_Linearize, strPaperType))
#else
				if (m_PrintProfile_Callbacks->Remove(kPrintProfile_Kind_Shading, strPaperType)
					&& m_PrintProfile_Callbacks->Remove(kPrintProfile_Kind_Linearize, strPaperType))
#endif
				{
					if (false == m_PaperDB_Callbacks->PDB_DelSystemPaperType(strPaperType))
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

	// Init paper type list
	InitPaperTypeList();
	// Update current selection paper type ID
	UpdateSelectionPaperTypeID(curPaperType);
	// Init paper type list for Delete dialog
	InitPaperTypeList_DlgDelete();
	::SendMessage(m_hwnd_DlgDelete, UWM_DELETE_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);

	UINT numOfPaperTypeError = listError.size();

	if (numOfPaperTypeError > 0)
	{
		char msg[1024] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_NOT_DELETE_PAPER_TYPE, LoadResourceString(IDM_NOT_DELETE_PAPER_TYPE, RESOURCE_MSG_STR));

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

	if (false == isSuccess)
	{
		char msg[256] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, LoadResourceString(IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, RESOURCE_MSG_STR));
		ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
	}

	return true;
}

/**
 *	Notify to exit Delete paper type dialog.
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataPaperType::ExitDeletePaperTypeDlg()
{
	// Reset member variables
	m_listPaperType_DlgDelete.clear();
	m_hwnd_DlgDelete = NULL;

//	::PostMessage(GetGenSetupDialog(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_PAPER_TYPE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	return true;
}

/**
 *	Initialize a new paper type name for Add paper type dialog
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::InitPaperTypeName_DlgAdd()
{
	m_paperTypeName_DlgAdd = "";
	if (m_paperTypeList.size() > 0 && m_selectPaperTypeID_DlgAdd != -1)
	{
		long nIndex = 0;
		char tmpPaperType[256] = {0};

		while (true)
		{
			memset(tmpPaperType, 0x00, sizeof(tmpPaperType));
			_snprintf_s(tmpPaperType, _TRUNCATE, "%s_%d", m_paperTypeList.at(m_selectPaperTypeID_DlgAdd).c_str(), ++nIndex);
			// Check the paper type name does not exist
			if (false == m_PaperDB_Callbacks->PDB_IsSystemPaperType(tmpPaperType))
			{
				m_paperTypeName_DlgAdd = tmpPaperType;
				break;
			}
		}
	}
}

/**
 *	Check and update the selection item for paper type list after add new paper type
 *	@param[in]	inPaperType : Paper type name
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::UpdateSelectionPaperTypeID(string inPaperType)
{
	if (m_paperTypeList.size() == 0
		|| inPaperType.length() == 0)
	{
		return;
	}

	m_selectPaperTypeID = -1;

	// Find the index of paper type
	vector<string>::iterator it = std::find(m_paperTypeList.begin(), m_paperTypeList.end(), inPaperType);
	// The paper type list has inPaperType item
	if (m_paperTypeList.end() != it)
	{
		m_selectPaperTypeID = std::distance(m_paperTypeList.begin(), it);
	}
	// The paper type list doesn't have inPaperType item
	else
	{
		// If inPaperType is not the current setting paper type
		if (m_currPaperType.compare(inPaperType) != 0)
		{
			// Update current selection id with the current setting paper type
			UpdateSelectionPaperTypeID(m_currPaperType);
		}
	}

	if(-1 != m_selectPaperTypeID && m_isNotSetCurrentPaperType) {
		m_isNotSetCurrentPaperType = false;
	}
}

/**
 *	Init Paper type list for Dlg Delete
 *	@note Notes
 *	
 *		
**/
void CDataPaperType::InitPaperTypeList_DlgDelete()
{
	m_listPaperType_DlgDelete.clear();
	ST_PAPER_TYPE_DELETE_INFO paperType;
	for (vector<string>::iterator it = m_paperTypeList.begin(); it != m_paperTypeList.end(); ++it)
	{
		paperType.m_paperType = *it;
		paperType.m_isDelete = false;
		m_listPaperType_DlgDelete.push_back(paperType);
	}

	// Find index of the current paper type setting
	vector<string>::iterator it_CurPaperType = std::find(m_paperTypeList.begin(), m_paperTypeList.end(), m_currPaperType);
	if (m_paperTypeList.end() != it_CurPaperType)
	{
		m_listPaperType_DlgDelete.erase(m_listPaperType_DlgDelete.begin() + std::distance(m_paperTypeList.begin(), it_CurPaperType));
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
bool CDataPaperType::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Init system paper type list
	InitPaperTypeList();

	UpdateCurrentSelectionPaperType(inPrintConditionOrSectionID);

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
bool CDataPaperType::ExitScreen()
{
	m_paperTypeList.clear();
	m_selectPaperTypeID = 0;
	m_currPaperType = "";
	m_printConditionName = "";
	return true;
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataPaperType::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentPaperType)
		WriteToLogBuf(LOG_DEBUG, "Paper type is judged as blank.");
	return m_isNotSetCurrentPaperType;
}