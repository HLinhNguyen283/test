/*! \file DataPrintSettings.cpp
 *  \brief data print settings class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "DataPrintSettings.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "Ini_UserModeAccess.h"
#include "Ini_Prnv.h"
#include <algorithm>
#include "CommonUiMsg_OP.h"
#include "DataIF.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <list>
#include <tuple>

#include "ProhibitChecker.h"
#include "ResDef.h"
#include "PluginCBRProc.h"

#include "Stub_PrintConditionExporter.h"
#include "PrintConditionGUIUtility.h"
#include "UwRwManager_Callbacks.h"
#include "DataPaperSettings.h"

// ==========================================================================
// Local definitions
#define INVALID_THICKNESS_VALUE		(-1)		//!< invalid paper thickness value

/*! \brief constructor
 *	@param[in]	inParent : the parent callback object
*/
CDataPrintSettings::CDataPrintSettings(CDataIF *inParent) :
	m_DataIF(inParent),
	m_SystemSetting(NULL),
	m_PaperDB_Callbacks(NULL),
	m_JM_Callbacks(NULL),
	m_IsEditSystemPrintCondition(false),
	m_IsEditJobPrintCondition(false),
	m_CurrentJob(NULL),
	m_CurrentPrintCondition(NULL),
	m_SelectPrintCondition(NULL),
	m_CurrentPrintCondition_AddDlg(NULL),
	m_SelectPrintCondition_AddDlg(NULL),
	m_Deleted_DelDlg(false),
	m_deletePCDlg(NULL),
	m_isNotSetCurrentPrintCondition(false)
{
	memset(&m_hThreadExecute, 0, sizeof(m_hThreadExecute));
}

/*! \brief destructor
*/
CDataPrintSettings::~CDataPrintSettings()
{
}

/*! \brief initialize
 *  \retval TRUE success
 *  \retval FALSE fail
*/
BOOL CDataPrintSettings::Initialize(CDataSystemSetting* inSystemSetting)
{
	if (NULL == inSystemSetting) {
		return FALSE;
	}

	m_SystemSetting = inSystemSetting ;
	m_PaperDB_Callbacks = inSystemSetting->GetPaperDB_Callbacks();
	m_JM_Callbacks = inSystemSetting->GetJobManager_Callbacks();

//	2014.2.27 matsuda: moved to EnterScreen()
/*	CIni_UserModeAccess userModeAccess;
	if (!userModeAccess.Initialize(TRUE)) {
		return FALSE;
	}
	m_IsEditPrintSetting = userModeAccess.IsEditPrintSetting() != 0;
	userModeAccess.Finalize();
*/
//	2014.2.27

	return TRUE;
}

/*! \brief finalize
*/
void CDataPrintSettings::Finalize()
{
	ReleasePrintConditionList();
	ReleaseCurrentJob();
}

/**
 *	Notify to enter PrintCondition screen
 *	@retval	true: succeed
 *	@retval	false: failed
**/
bool CDataPrintSettings::EnterScreen()
{
//	2014.2.27 matsuda: moved from Initialize()
	CIni_UserModeAccess userModeAccess;
	if (!userModeAccess.Initialize(TRUE)) {
		return FALSE;
	}
	m_IsEditSystemPrintCondition = userModeAccess.IsEditSystemPrintCondition() != 0;
	m_IsEditJobPrintCondition = userModeAccess.IsEditJobPrintCondition() != 0;
	userModeAccess.Finalize();
//	2014.2.27

	InitCurrentJob();
	UpdateData();
	return true;
}

/*! \brief Notify to exit Print condition screen
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::ExitScreen()
{
	ReleasePrintConditionList();
	ReleaseCurrentJob();
	return true;
}

/**
 *	Initialize list of print condition
 **/
void CDataPrintSettings::InitPrintConditionList()
{
	ReleasePrintConditionList();
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		string name;
		if (m_CurrentJob && m_JM_Callbacks->JM_GetPrintCondition(m_CurrentJob->Section, name))
		{
			PrintCondition* printCondition = (PrintCondition*)IO_malloc(sizeof(PrintCondition));
			printCondition->ID = 0;
			strcpy_s(printCondition->Name, name.c_str());
			m_JM_Callbacks->JM_GetPaperOption(m_CurrentJob->Section, m_SystemSetting->GetSystemUnit(), printCondition->Option);
			m_JM_Callbacks->JM_GetLock(m_CurrentJob->Section, printCondition->Lock);
			m_PrintConditions.push_back(printCondition);
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_EnterCommonPaperDBCS())
		{
			for (long ID = 0; true; ++ID)
			{
//				char printConditionName[256] = { 0 };
				string printConditionName;
				if (!m_PaperDB_Callbacks->PDB_GetPrintConditionName(printConditionName))
				{
					break;
				}
				PrintCondition* printCondition = (PrintCondition*)IO_malloc(sizeof(PrintCondition));
				printCondition->ID = ID;
//				strcpy_s(printCondition->Name, printConditionName);
				strcpy_s(printCondition->Name, printConditionName.c_str());
				m_PaperDB_Callbacks->PDB_GetPaperOption(printCondition->Name, m_SystemSetting->GetSystemUnit(), printCondition->Option);
				m_PaperDB_Callbacks->PDB_GetLock(printCondition->Name, printCondition->Lock);
				m_PrintConditions.push_back(printCondition);
			}
			m_PaperDB_Callbacks->PDB_LeaveCommonPaperDBCS();
		}
	}
//	::PostMessage(GetGenSetupDialog(), UWM_PRINT_CONDITION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_SystemSetting->GetPostMessageWindow(), UWM_PRINT_CONDITION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
}

/**
 *	Finalize list of print condition
**/
void CDataPrintSettings::ReleasePrintConditionList()
{
	for (vector<PrintCondition*>::iterator it = m_PrintConditions.begin(); it != m_PrintConditions.end(); ++it)
	{
		if (NULL != *it)
		{
			IO_free(*it);
			*it = NULL;
		}
	}
	m_PrintConditions.clear();
}

/**
 *	Determine current print condition
**/
void CDataPrintSettings::InitCurrentPrintCondition()
{
	m_CurrentPrintCondition = NULL;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_PrintConditions.size() > 0)
		{
			m_CurrentPrintCondition = m_PrintConditions[0];
		}
	}
	else
	{
		string printConditionName ("");
		if (m_PaperDB_Callbacks->PDB_GetCurrentPrintCondition(printConditionName))
		{
			m_CurrentPrintCondition = GetPrintConditionByName(printConditionName.c_str());
		}
	}
}

/**
 *	Determine the select print condition
 *	@param[in]	inPrintConditionName : the print condition name
**/
void CDataPrintSettings::InitSelectPrintCondition(const char* inPrintConditionName)
{
	m_SelectPrintCondition = NULL;
	if (inPrintConditionName)
	{
		m_SelectPrintCondition = GetPrintConditionByName(inPrintConditionName);
	}
	
	if(!m_SelectPrintCondition)
	{
		m_SelectPrintCondition = m_CurrentPrintCondition;
	}

	if(m_isNotSetCurrentPrintCondition) {
		m_isNotSetCurrentPrintCondition = false;
	}

	if (NULL == m_SelectPrintCondition && m_PrintConditions.size() > 0)
	{
		//PrintCondition only has 1 item
		if(1 == m_PrintConditions.size()) {
			m_SelectPrintCondition = m_PrintConditions[0];
		}
		//> 1 item
		else {
			//Create blank item
			PrintCondition* emptyPrintCondition = (PrintCondition*)IO_malloc(sizeof(PrintCondition));
			emptyPrintCondition->ID = m_PrintConditions.size();
			emptyPrintCondition->Lock = true;
			//Select blank item
			m_PrintConditions.push_back(emptyPrintCondition);
			m_SelectPrintCondition = emptyPrintCondition;
			m_isNotSetCurrentPrintCondition = true;
		}
	}
}

/**
 *	Initialize current job
**/
void CDataPrintSettings::InitCurrentJob()
{
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		ReleaseCurrentJob();
		char section[256] = { 0 };
		if (m_JM_Callbacks->JM_GetSectionIDToSelectPrintCondition(section))
		{
			m_CurrentJob = (JobInfo*)IO_malloc(sizeof(JobInfo));
			strcpy_s(m_CurrentJob->Section, section);
			m_JM_Callbacks->JM_GetJobInformation_PrintCondition(section, &m_CurrentJob->Info);
		}
	}
}

/**
 *	Finalize current job
**/
void CDataPrintSettings::ReleaseCurrentJob()
{
	if (NULL != m_CurrentJob)
	{
		IO_free(m_CurrentJob);
		m_CurrentJob = NULL;
	}
}

/**
 *	Reload the print condition list
 *	@param[in]	inPrintConditionOrSection : the print condition name will be selected
**/
void CDataPrintSettings::UpdateData(const char* inSelectPrintCondition)
{
	InitPrintConditionList();
	InitCurrentPrintCondition();
	InitSelectPrintCondition(inSelectPrintCondition);
}

/**
 *	#describe the function this method provides
 *	@retval	In case the Print Condition GUI is started from:
 *			-	System Setting Screen
 *					return print condition name
 *			-	Job List Screen
 *					return section id
**/
const char* CDataPrintSettings::GetSelectPrintConditionOrSection()
{
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_CurrentJob)
		{
			return m_CurrentJob->Section;
		}
	}
	else
	{
		if (m_SelectPrintCondition)
		{
			return m_SelectPrintCondition->Name;
		}
	}
	return NULL;
	
}

/**
 *	Getting the print condition by name
 *	@param[in]	inPrintConditionName : the name of print condition
 *	@retval	the print condition
**/
PrintCondition* CDataPrintSettings::GetPrintConditionByName(const char* inPrintConditionName)
{
	vector<PrintCondition*>::iterator it = find_if(m_PrintConditions.begin(), m_PrintConditions.end(), FindPrintCondition(inPrintConditionName));
	if (it != m_PrintConditions.end())
	{
		return *it;
	}
	return NULL;
}

/**
 *	Getting the current job
 *	@retval	the current job
**/
JobInfo* CDataPrintSettings::GetCurrentJob() const
{
	return m_CurrentJob;
}

/*! \brief Get a item of print condition list.
 *  \param inItemID: an item ID(0-N)
 *  \param  outPrintCondition: a pointer to gget a item of print condition list.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetPrintConditionList(long inItemID, const char** outPrintCondition)
{
	if (outPrintCondition &&
		0 <= inItemID && inItemID < (long)m_PrintConditions.size())
	{
		*outPrintCondition = m_PrintConditions[inItemID]->Name;
		return true;
	}
	return false;
}

/*! \brief Get a paper option of a print condition
 *  \param inItemID: an item ID(0-N)
 *  \param outHole: a pointer to get a ON/OFF of tractor hole(true: ON, false: OFF)
 *  \param outHoleAndPerforation: a pointer to get a ON/OFF of both tractor holes and perforations(true: ON, false: OFF)
 *  \param outPreprintedMark: a pointer to get a ON/OFF of preprinted mark(true: ON, false: OFF)
 *  \param outPreprintedPaper: a pointer to get a ON/OFF of preprinted paper(true: ON, false: OFF)
 *  \param outMarkSide: a pointer to get a side of preprinted mark.(front, back)
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetPrintConditionListPaperOption(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide)
{
	if (outHole && outHoleAndPerforat && outPreprintedMark && outPreprintedPaper && outMarkSide &&
		0 <= inItemID && inItemID < (long)m_PrintConditions.size())
	{
		ST_PDB_PAPER_OPTION& option = m_PrintConditions[inItemID]->Option;
		*outHole			= option.IS_HOLES ? true : false;
		*outHoleAndPerforat	= option.IS_HOLES_PERFORATIONS ? true : false;
		*outPreprintedMark	= option.IS_PRE_MARK ? true : false;
		*outPreprintedPaper	= option.IS_PRE_MARK_PAPER ? true : false;
		*outMarkSide		= option.PRE_MARK_SIDE;
		return true;
	}
	return false;
}

/*! \brief Get information of a current print condition.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \param outPrintCondition : a pointer to get a current print condition name.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataPrintSettings::GetCurrentPrintCondition(bool* outSelecting, const char** outPrintCondition)
{
	if (outSelecting && outPrintCondition && m_CurrentPrintCondition)
	{
		*outPrintCondition = m_CurrentPrintCondition->Name;
		*outSelecting = m_SelectPrintCondition == m_CurrentPrintCondition;
		return true;
	}
	return false;
}

/*! \brief Compare information of a selecting and system settings current print condition.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataPrintSettings::CmpSelectingAndSystemCurrentPrintCondition(bool& outSelecting)
{
	string printConditionName;
	if (m_PaperDB_Callbacks->PDB_GetCurrentPrintCondition(printConditionName))
	{
		PrintCondition* printCondition = GetPrintConditionByName(printConditionName.c_str());
		outSelecting = m_SelectPrintCondition == printCondition;
		return true;
	}
	return false;
}

/**
 *	Getting current print condition
 *	@retval	current print condition
**/
PrintCondition* CDataPrintSettings::GetCurrentPrintCondition()
{
	return m_CurrentPrintCondition;
}

/*! \brief Check if user can add a print condition or not.
 *  \retval true: The adding can do
 *  \retval false: The adding can not do
*/
bool CDataPrintSettings::IsPermissionAddPrintCondition()
{
	return m_IsEditSystemPrintCondition;
}

/*! \brief Check if user can delete a print condition or not.
 *  \retval true: The deleting can do
 *  \retval false: The deleting can not do
*/
bool CDataPrintSettings::IsPermissionDeletePrintCondition()
{
	return m_IsEditSystemPrintCondition;
}

/*! \brief Check if user can edit job print condition or not.
 *  \retval true: allowed
 *  \retval false: not allowed
*/
bool CDataPrintSettings::IsPermissionEditJobPrintCondition()
{
	return m_IsEditJobPrintCondition;
}

/*! \brief Set a selection print condition.
 *  \param inItemID: an item ID(0-N)
*/
bool CDataPrintSettings::SetSelectPrintCondition(long inItemID)
{
	bool retval = false;
	if (-1 == inItemID)
	{
		m_SelectPrintCondition = NULL;
		retval = true;
	}
	else if (0 <= inItemID && inItemID < (long)m_PrintConditions.size())
	{
		m_SelectPrintCondition = m_PrintConditions[inItemID];
		retval = true;
	}

	//Remove blank item if has
	if(retval && m_isNotSetCurrentPrintCondition && (inItemID != m_PrintConditions.size() - 1)) {
		//release resource
		IO_free(*(m_PrintConditions.end() - 1));

		m_PrintConditions.erase(m_PrintConditions.end() - 1);
//		SendMessage(GetGenSetupDialog(), UWM_PRINT_CONDITION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		SendMessage(m_SystemSetting->GetPostMessageWindow(), UWM_PRINT_CONDITION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentPrintCondition = false;
	}

	return retval;
}

/*! \brief Get lock/unlock to edit print condition settings
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPrintSettings::GetLockPrintCondtion()
{
	if (m_SelectPrintCondition)
	{
		if(USER_ID_GENERAL == m_SystemSetting->GetSystemUserMode()){	// General mode
			if(m_SystemSetting->IsStartedFromJobListScreen()){	// started from the job
				if(m_IsEditJobPrintCondition)
					return m_SelectPrintCondition->Lock ? true : false;	// according to the print setting
				else
					return true; //	locked: editing not allowed
			} else {	// started from SystemSetting
				return m_SelectPrintCondition->Lock ? true : false;	// according to the job setting
			}
		} else {	// Expert and Service modes
			return m_SelectPrintCondition->Lock ? true : false;
		}
	}
	return false;
}

/*! \brief Set lock/unlock to edit print condition settings.
 *  \param inLock (true: lock, false: unlock)
*/
void CDataPrintSettings::SetLockPrintCondtion(bool inLock)
{
	bool retval = false;
	if (m_SelectPrintCondition)
	{
		m_SelectPrintCondition->Lock = inLock;
		if (m_SystemSetting->IsStartedFromJobListScreen())
		{
			if (m_JM_Callbacks->JM_SetLock(m_CurrentJob->Section, m_SelectPrintCondition->Lock))
			{
				retval = m_JM_Callbacks->JM_SaveJobFile(m_CurrentJob->Section) == PLUGERR_NONE;
			}
		}
		else
		{
			if (m_PaperDB_Callbacks->PDB_SetLock(m_SelectPrintCondition->Name, m_SelectPrintCondition->Lock))
			{
				retval = m_PaperDB_Callbacks->PDB_CommitPaperDB();
			}
		}
	}
	if (retval)
	{
		m_SelectPrintCondition->Lock = inLock;
	}
}

/*! \brief Get a selected print condition
 *  \param outItemID: a pointer to get an item ID(0-N) of a selected print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetSelectPrintCondition(long* outItemID)
{
	if (outItemID && m_SelectPrintCondition)
	{
		*outItemID = m_SelectPrintCondition->ID;
		return true;
	}
	return false;
}

PrintCondition* CDataPrintSettings::GetSelectPrintCondition()
{
	return m_SelectPrintCondition;
}

/*! \brief Set a current print condition.
*/
void CDataPrintSettings::SetCurrentPrintCondition()
{
	if (m_SelectPrintCondition)
	{
		// Update paper thickness value to value notified from UW
		CIni_PrinterDescriptor iniPrinterDescriptor;
		iniPrinterDescriptor.Initialize(TRUE);
		if (iniPrinterDescriptor.getUnwinderOption() == 1) {
			CDataPaperSettings * dataPaperSettings = m_DataIF->GetDataPaperSettings();
			if (dataPaperSettings) {
				//SUwRwManager_Callbacks unwinderManager_Callbacks;
				//if (UnwinderManager_GetCallbacks(&unwinderManager_Callbacks)) {
				//	long currentThickness = dataPaperSettings->GetPaperThickness();
				//	bool isReflectUWPaperThickness = false;
				//	unwinderManager_Callbacks.CheckUpdateUWPaperThicknessForCurrentPrintCondition(currentThickness, isReflectUWPaperThickness);
				//	if (isReflectUWPaperThickness) {
				//		long UWThickness = INVALID_THICKNESS_VALUE;
				//		unwinderManager_Callbacks.GetUWPaperThickness(UWThickness);
				//		if (UWThickness != INVALID_THICKNESS_VALUE) {
				//			if (m_PaperDB_Callbacks->PDB_SetPaperThickness(m_SelectPrintCondition->Name, UWThickness)) {
				//				dataPaperSettings->SetCurrentPaperThickness(UWThickness);
				//				dataPaperSettings->SetPaperThickness(UWThickness);
				//			}
				//			else {
				//				WriteToLogBuf(LOG_DEBUG, "CDataPrintSettings::SetCurrentPrintCondition() error - Cannot set paper thickness to PDB.");
				//			}
				//		}
				//		else {
				//			WriteToLogBuf(LOG_DEBUG, "CDataPrintSettings::SetCurrentPrintCondition() error - Invalid UW paper thickness value.");
				//		}
				//	}
				//}
				//else {
				//	WriteToLogBuf(LOG_DEBUG, "CDataPrintSettings::SetCurrentPrintCondition() error - Cannot get unwinder manager callback.");
				//}
			}
			else {
				WriteToLogBuf(LOG_DEBUG, "CDataPrintSettings::SetCurrentPrintCondition() error - Cannot get paper setting data.");
			}
		}
		iniPrinterDescriptor.Finalize();

		m_PaperDB_Callbacks->PDB_SetCurrentPrintCondition(m_SelectPrintCondition->Name);
		m_PaperDB_Callbacks->PDB_CommitPaperDB();

		string name = m_SelectPrintCondition->Name;
		UpdateData(name.c_str());
	}

	if (m_SystemSetting->IsStartedFromJobListScreen()) {
		// ジョブの印刷条件設定からの変更時は、画面の更新のためのメッセージを通知.
		::PostMessage(GetMainWindow(), UWM_PRINTCONDITION_CHANGE_FROM_JOB, 0, 0);
	}
}

/*! \brief Notify to initialize Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::InitAddPrintConditionDlg()
{
	{
		SetAddPrintConditionName_AddDlg("");
		if (m_SelectPrintCondition)
		{
			for (int i = 1; true; ++i)
			{
				char name[256] = { 0 };
				_snprintf_s(name, _TRUNCATE, "%s_%d", m_SelectPrintCondition->Name, i);
				if (NULL == GetPrintConditionByName(name))
				{
					SetAddPrintConditionName_AddDlg(name);
					break;
				}
			}
		}
	}
	{
		m_PrintConditions_AddDlg.clear();
		SetCurrentPrintCondition_AddDlg(m_CurrentPrintCondition);
		if (m_CurrentPrintCondition)
		{
			m_PrintConditions_AddDlg.push_back(m_CurrentPrintCondition);
		}
		for (vector<PrintCondition*>::iterator it = m_PrintConditions.begin(); it != m_PrintConditions.end(); ++it)
		{
			if (m_CurrentPrintCondition != *it)
			{
				m_PrintConditions_AddDlg.push_back(*it);
			}
		}
	}
	{
		SetSelectPrintCondition_AddDlg(-1);
		if (m_SelectPrintCondition)
		{
			vector<PrintCondition*>::iterator it = find(m_PrintConditions_AddDlg.begin(), m_PrintConditions_AddDlg.end(), m_SelectPrintCondition);
			if (it != m_PrintConditions_AddDlg.end())
			{
				SetSelectPrintCondition_AddDlg(it - m_PrintConditions_AddDlg.begin());
			}
		}
	}

	return true;
}

/*! \brief Get a print condition name to add it at Add print condition dialog.
 *  \retval a print condition name
*/
const char* CDataPrintSettings::GetAddPrintConditionName_AddDlg()
{
	return m_PrintConditionName_AddDlg.c_str();
}

/*! \brief Set a print condition name to add it at Add print condition dialog.
 *  \param inName: a print condition name
*/
void CDataPrintSettings::SetAddPrintConditionName_AddDlg(char* inName)
{
	if (inName)
	{
		bool hasProhibit = ProhibitChecker::hasProhibitChar(inName);
		if (hasProhibit) {
			std::stringstream ss;
			ss << ID_MESSAGE_ERROR +  IDM_ERR_PROHIBITCHARS_PRINTCONDITION_NAME << "\n" << LoadResourceString(IDM_ERR_PROHIBITCHARS_PRINTCONDITION_NAME, RESOURCE_MSG_STR) << "\n" << ProhibitChecker::prohibitString();
			ShowMessageBox(const_cast<char*>(ss.str().c_str()), MBST_OK | MBST_ICONERROR, NULL);
			return;
		}

		//!< 半角スペースを除去した文字列をセット
		m_PrintConditionName_AddDlg = PrintConditionGUIUtility::TrimLastSpace(inName);
	}
}

/*! \brief Set a selection print condition at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
*/
void CDataPrintSettings::SetSelectPrintCondition_AddDlg(long inItemID)
{
	if (-1 == inItemID)
	{
		m_SelectPrintCondition_AddDlg = NULL;
	}
	else if (0 <= inItemID && inItemID < (long)m_PrintConditions_AddDlg.size())
	{
		m_SelectPrintCondition_AddDlg = m_PrintConditions_AddDlg[inItemID];
	}
}

/*! \brief Get a selection print condition at Add print condition dialog.
 *  \param outItemID: a pointer to get an item ID(0-N) of a selectio nprint condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetSelectPrintCondition_AddDlg(long* outItemID)
{
	if (outItemID && m_SelectPrintCondition_AddDlg)
	{
		vector<PrintCondition*>::iterator it = find(m_PrintConditions_AddDlg.begin(), m_PrintConditions_AddDlg.end(), m_SelectPrintCondition_AddDlg);
		if (it != m_PrintConditions_AddDlg.end())
		{
			*outItemID = it - m_PrintConditions_AddDlg.begin();
			return true;
		}
	}
	return false;
}

/*! \brief Get information of a current print condition at Add print condition dialog.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \param outPrintCondition : a pointer to get a current print condition name.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataPrintSettings::GetCurrentPrintCondition_AddDlg(bool* outSelecting, const char** outPrintCondition)
{
	if (outSelecting && outPrintCondition && m_CurrentPrintCondition_AddDlg)
	{
		*outPrintCondition = m_CurrentPrintCondition_AddDlg->Name;
		*outSelecting = m_CurrentPrintCondition_AddDlg == m_SelectPrintCondition_AddDlg;
		return true;
	}
	return false;
}

/**
 *	Setting current print condition to add dialog
 *	@param[in]	inPrintCondition : the print condition
**/
void CDataPrintSettings::SetCurrentPrintCondition_AddDlg(PrintCondition* inPrintCondition)
{
	m_CurrentPrintCondition_AddDlg = inPrintCondition;
}

/*! \brief Get a item of print condition list at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outName: a pointer to get a print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetPrintConditionList_AddDlg(long inItemID, const char** outName)
{
	if (outName &&
		0 <= inItemID && inItemID < (long)m_PrintConditions_AddDlg.size())
	{
		*outName = m_PrintConditions_AddDlg[inItemID]->Name;
		return true;
	}
	return false;
}

/*! \brief Get a paper option of a print condition at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outHole: a pointer to get a ON/OFF of tractor hole(true: ON, false: OFF)
 *  \param outHoleAndPerforation: a pointer to get a ON/OFF of both tractor holes and perforations(true: ON, false: OFF)
 *  \param outPreprintedMark: a pointer to get a ON/OFF of preprinted mark(true: ON, false: OFF)
 *  \param outPreprintedPaper: a pointer to get a ON/OFF of preprinted paper(true: ON, false: OFF)
 *  \param outMarkSide: a pointer to get a side of preprinted mark.(front, back)
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetPrintConditionListPaperOption_AddDlg(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide)
{
	if (outHole && outHoleAndPerforat && outPreprintedMark && outPreprintedPaper && outMarkSide &&
		0 <= inItemID && inItemID < (long)m_PrintConditions_AddDlg.size())
	{
		ST_PDB_PAPER_OPTION& option = m_PrintConditions_AddDlg[inItemID]->Option;
		*outHole					= option.IS_HOLES ? true : false;
		*outHoleAndPerforat			= option.IS_HOLES_PERFORATIONS ? true : false;
		*outPreprintedMark			= option.IS_PRE_MARK ? true : false;
		*outPreprintedPaper			= option.IS_PRE_MARK_PAPER ? true : false;
		*outMarkSide				= option.PRE_MARK_SIDE;
		return true;
	}
	return false;
}

/*! \brief Save the settings of Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::SaveAddPrintSettingDlg()
{	
	bool retval = false;
	const char* name = GetAddPrintConditionName_AddDlg();
	if (name && strlen(name) > 0)
	{
		if (m_PaperDB_Callbacks->PDB_IsPrintConditionName(name))
		{
			char msg[256] = {0};
			_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_DUPLICATE_NAME_EXISTS, LoadResourceString(IDM_DUPLICATE_NAME_EXISTS, RESOURCE_MSG_STR));
			ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
			return false;
		}

		long itemID = -1;
		if (GetSelectPrintCondition_AddDlg(&itemID))
		{
			retval = m_PaperDB_Callbacks->PDB_AddPrintConditionName(name, m_PrintConditions_AddDlg[itemID]->Name);
		}
		else
		{
			retval = m_PaperDB_Callbacks->PDB_AddPrintConditionName(name, NULL);
		}
		if (retval)
		{
			//	remove scan image data path if set
			string fileName = "";
			if(m_PaperDB_Callbacks->PDB_GetScanFileName(name, COM_SYSTEM_TYPE_SELF, fileName)){
				if(fileName != "")
					m_PaperDB_Callbacks->PDB_DelScanFileName(name, COM_SYSTEM_TYPE_SELF);
			}
			fileName = "";
			if(m_PaperDB_Callbacks->PDB_GetScanFileName(name, COM_SYSTEM_TYPE_PART, fileName)){
				if(fileName != "")
					m_PaperDB_Callbacks->PDB_DelScanFileName(name, COM_SYSTEM_TYPE_PART);
			}

			// copy the temporal mask file.
			string temporalMaskFile = "";
			if(m_PaperDB_Callbacks->PDB_GetTemporalMaskFile(name, temporalMaskFile)){
				if(temporalMaskFile != ""){
					char drive[_MAX_DRIVE] = {0}, dir[_MAX_DIR] = {0}, fileName[_MAX_FNAME] = {0}, ext[_MAX_EXT] = {0};
					_splitpath(temporalMaskFile.c_str(), drive, dir, fileName, ext); 
	
					// Create the new file name.
					SYSTEMTIME tm = { 0 };
					GetLocalTime(&tm);
					char lCurrDateTime[30] = { 0 };
					_snprintf_s(lCurrDateTime, _TRUNCATE, "PDB_%04d%02d%02d%02d%02d%02d.xml", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
					string newTemporalMaskFile = dir;
					newTemporalMaskFile = newTemporalMaskFile + lCurrDateTime;

					// Copy the temporal mask file.
					string src = GetModuleDirectory();
					src = src + temporalMaskFile;
					string dst = GetModuleDirectory();
					dst = dst + newTemporalMaskFile;
					CopyFile(src.c_str(), dst.c_str(), FALSE);
		
					// Save the temporal mask file parh.
					m_PaperDB_Callbacks->PDB_SetTemporalMaskFile(name, newTemporalMaskFile);
				}
			}


			//ジョブ印刷時のJI撮影画像ファイルをコピー(JI画面用)
			CopyJIPhotoInfo_PDB(name);

			// JI関連の情報を削除
			DeleteInspectionInfo(name);

			m_PaperDB_Callbacks->PDB_CommitPaperDB();
			UpdateData(name);

			CancelAddPrintSettingDlg();
		}
		else
		{
			char msg[256] = {0};
			_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, LoadResourceString(IDM_FAILED_UPDATE_THE_MANAGEMENT_FILE, RESOURCE_MSG_STR));
			ShowMessageBox(msg, MBST_ICONERROR | MBST_OK, NULL);
		}
	}		
	return retval;
}

/*! \brief Cancel to save the settings of Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::CancelAddPrintSettingDlg()
{
	return true;
}

/*! \brief Register window handle of Delete print condition dialog.
 *  \param inDialog: the dialog handle.
*/
void CDataPrintSettings::SetDeletePrintConditionDlg(HWND inDialog)
{
	m_deletePCDlg = inDialog;
}

/*! \brief Notify to initialize Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::InitDeletePrintConditionDlg(bool inDeleted)
{
	m_Deleted_DelDlg = inDeleted;
	m_PrintConditions_DelDlg.clear();
	for (vector<PrintCondition*>::iterator it = m_PrintConditions.begin(); it != m_PrintConditions.end(); ++it)
	{
		PrintCondition* printCondition = *it;
		if (printCondition != m_CurrentPrintCondition && FALSE == printCondition->Lock)
		{
			m_PrintConditions_DelDlg.push_back(printCondition);
		}
	}
	m_DeleteItems_DelDlg.clear();
	return true;
}

/*! \brief Get an item of print condition list at Delete print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outName: a pointer to get a print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintSettings::GetDeletePrintConditionList_DelDlg(long inItemID, const char** outName)
{
	if (0 <= inItemID && inItemID < (long)m_PrintConditions_DelDlg.size())
	{
		*outName = m_PrintConditions_DelDlg[inItemID]->Name;
		return true;
	}
	return false;
}

/*! \brief Register items of print condition list to delete at Delete print condition dialog.
 *  \param inItemID: an item ID(0-N)
*/
void CDataPrintSettings::SetDeletePrintCondition_DelDlg(long inItemID)
{
	if (0 <= inItemID && inItemID < (long)m_PrintConditions_DelDlg.size())
	{
		m_DeleteItems_DelDlg.push_back(m_PrintConditions_DelDlg[inItemID]);
	}
}

/*! \brief Delete the registered items to delete at Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::DeletePrintConditions_DelDlg()
{
	//印刷条件削除スレッドを起動
	UINT nThreadID = 0;
	m_hThreadExecute = TM_CreateThread(NULL, 0, DeletePrintConditionsProc, (LPVOID)this, 0, &nThreadID, "PrintConditionGUI.dll", "DeletePrintConditions", NULL);
	return m_hThreadExecute.thread_handle == NULL;
}

/*! \brief Notify to exit Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintSettings::ExitDeletePrintConditionDlg()
{
	if (m_Deleted_DelDlg)
	{
		if (m_SelectPrintCondition)
		{
			string name = m_SelectPrintCondition->Name;
			UpdateData(name.c_str());
		}
		else
		{
			UpdateData(NULL);
		}
	}
	return true;
}

/*!
 *  \brief 印刷条件削除スレッド
 *  \param pParam 
 *  \return 終了結果(正常終了：0以外、異常終了：0)
*/
UINT WINAPI CDataPrintSettings::DeletePrintConditionsProc(LPVOID pParam)
{
	CDataPrintSettings* dataSetting = static_cast<CDataPrintSettings*>(pParam);
	if (dataSetting)
	{
		return dataSetting->_DeletePrintConditionsProc();
	}
	return 0;
}

/*!
 *  \brief 印刷条件削除スレッド実体
 *  \return 終了結果(正常終了：0以外、異常終了：0)
*/
UINT CDataPrintSettings::_DeletePrintConditionsProc()
{
	/*
	 * 削除進捗ダイアログを表示する
	 */
	SIZE sizeDialog = { 600, 0 };
	long progress_id = ShowProgressDialog(sizeDialog,
										  PROGDLG_INDEFINITE_BAR | PROGDLG_MSG_CENTER,
										  0, 100, 0, NULL,
										  (char*)LoadResourceString(IDM_MSG_DELETE_NOW, RESOURCE_MSG_STR));
	
	for (vector<PrintCondition*>::iterator it = m_DeleteItems_DelDlg.begin(); it != m_DeleteItems_DelDlg.end(); ++it)
	{
		vector<PrintCondition*>::iterator removeIt = find(m_PrintConditions.begin(), m_PrintConditions.end(), (*it));
		if (removeIt != m_PrintConditions.end())
		{
			char* name = (*removeIt)->Name;
			if (m_PaperDB_Callbacks->PDB_IsPrintConditionName(name))
			{
				// JIスキャン画像のパス取得(印刷条件削除前に画像パスを取得しておく)
				string fileNameSelf = "";
				BOOL bSelf = m_PaperDB_Callbacks->PDB_GetScanFileName(name, COM_SYSTEM_TYPE_SELF, fileNameSelf);
				string fileNamePart = "";
				BOOL bPart = m_PaperDB_Callbacks->PDB_GetScanFileName(name, COM_SYSTEM_TYPE_PART, fileNamePart);

				// 一時マスクファイルの取得
				string temporalMask = "";
				BOOL bMask = m_PaperDB_Callbacks->PDB_GetTemporalMaskFile(name, temporalMask);

				//ジョブ印刷時のJI撮影画像ファイル名取得
				string jiPhotoFileNameF, jiPhotoFileNameB;
				GetJIPhotoInfo_PDB(name, jiPhotoFileNameF, jiPhotoFileNameB);


				// 削除対象印刷条件名が紙DBに登録済みでかつ削除済みでない場合のみ削除.
				if (m_PaperDB_Callbacks->PDB_DelPrintConditionName(name))
				{
					// JIスキャン画像削除(印刷条件の削除に成功した場合、画像ファイルを削除する)
					if(bSelf && fileNameSelf != ""){
						// ローカルのファイル名
						char localPath[_MAX_PATH] = {0};
						_snprintf_s(localPath , _TRUNCATE, "%s%s" , GetModuleDirectory(), fileNameSelf.c_str());
						DeleteFile(localPath);
					}
					if(bPart && fileNamePart != ""){
						// ローカルのファイル名
						char localPath[_MAX_PATH] = {0};
						_snprintf_s(localPath , _TRUNCATE, "%s%s" , GetModuleDirectory(), fileNamePart.c_str());
						DeleteFile(localPath);
					}
					// JI調整印刷情報削除(JI通信プラグに依頼する)
					if(m_SystemSetting->IsSystemSupportJetInspection() && JetInspector_IsJIOnline()){	
						if(m_SystemSetting->IsDED()){
							JetInspector_DeleteAdjustPrint(name, ENUM_SYSTEM_CONFIG_FULLDUPLEX);
							Remote_JetInspector_DeleteAdjustPrint(name, ENUM_SYSTEM_CONFIG_FULLDUPLEX);
						}else{
							JetInspector_DeleteAdjustPrint(name, ENUM_SYSTEM_CONFIG_FULLSIMPLEX);
						}
					}
					// 一時マスクファイル削除
					if(bMask && temporalMask != ""){
						// ローカルのファイル名
						char localPath[_MAX_PATH] = {0};
						_snprintf_s(localPath , _TRUNCATE, "%s%s" , GetModuleDirectory(), temporalMask.c_str());
						DeleteFile(localPath);
					}

					//ジョブ印刷時のJI撮影画像ファイル削除
					DeleteJIPhotoInfo_PDB(jiPhotoFileNameF, jiPhotoFileNameB);

					//release resource
					if (m_SelectPrintCondition == *removeIt) {
						m_SelectPrintCondition = NULL;
					}
					IO_free(*removeIt);

					m_PrintConditions.erase(removeIt);
				}
			}
		}
	}

	// 紙DB書き込み
	m_PaperDB_Callbacks->PDB_CommitPaperDB();
	InitDeletePrintConditionDlg(true);

	PostMessage(m_deletePCDlg, UWM_DELETE_PRINT_CONDITION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);

	//メインウィンドウに対して更新メッセージを通知する
	PostMessage(GetMainWindow(), UWM_DELETE_PRINT_CONDITION_UPDATE, 0, 0);

	//削除進捗ダイアログを削除
	if( progress_id != 0 ) {
		CloseProgressDialog( progress_id );
	}

	//印刷条件削除スレッド終了処理
	TM_DeleteThreadForce( &m_hThreadExecute );

	return 1;
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataPrintSettings::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentPrintCondition)
		WriteToLogBuf(LOG_DEBUG, "Print condition is judged as blank.");
	return m_isNotSetCurrentPrintCondition;
}


/**
 *	ジョブ印刷時のJI撮影情報をコピーする(JI画面用)
 *	@param[in]	inPrintConditionName : 印刷条件名
 *	@retval	true: 成功
 *	@retval	false: 失敗
**/
bool CDataPrintSettings::CopyJIPhotoInfo_PDB(std::string inPrintConditionName)
{
	//＊コミットは呼び出し元で実行する

	///////////////////////////////
	// 変数準備
	///////////////////////////////
	if(!m_SystemSetting || !m_PaperDB_Callbacks || inPrintConditionName.empty()){
		return false;
	}
	BOOL isDED = m_SystemSetting->IsDED();
	auto PDB_GetJIPhotoOnJobPrint = m_PaperDB_Callbacks->PDB_GetJIPhotoOnJobPrint;
	auto PDB_SetJIPhotoOnJobPrint = m_PaperDB_Callbacks->PDB_SetJIPhotoOnJobPrint;

	///////////////////////
	// サブ関数準備
	///////////////////////
	//コピー先の画像ファイル名(コピー先)を取得する
	auto GetLocalFileName = [&](string& outName)->void{
		//ファイル名
		//・<yyyymmdd_hhMMssSSS>.jpg
		SYSTEMTIME lt = {0};
		GetLocalTime(&lt);

		stringstream fileName;
		fileName << setfill('0') << lt.wYear << setw(2) << lt.wMonth << setw(2) << lt.wDay << _T("_") << setw(2) << lt.wHour << setw(2) << lt.wMinute << setw(2) << lt.wSecond << setw(3) << lt.wMilliseconds << _T(".jpg");

		outName = fileName.str();
	};



	///////////////////////
	// 処理
	///////////////////////
	string rootDirPath = GetModuleDirectory();

	//コピー先のファイル名を取得
	string dstFileName;
	GetLocalFileName(dstFileName);

	//印刷条件に撮影情報が登録されていたら、登録されている画像ファイルをコピーする
	//DED用のターゲット
	list<pair<eComSystemType, DEF_PRINT_SIDE>> targetDED;
	targetDED.push_back(pair<eComSystemType, DEF_PRINT_SIDE>(COM_SYSTEM_TYPE_PART, DEF_PRINT_FRONT));
	targetDED.push_back(pair<eComSystemType, DEF_PRINT_SIDE>(COM_SYSTEM_TYPE_SELF, DEF_PRINT_BACK));
	//SES用のターゲット
	list<pair<eComSystemType, DEF_PRINT_SIDE>> targetSES;
	targetSES.push_back(pair<eComSystemType, DEF_PRINT_SIDE>(COM_SYSTEM_TYPE_SELF, DEF_PRINT_FRONT));
		
	//ターゲット選択
	auto& target = isDED ? targetDED : targetSES;
	for(auto i = target.begin(); i != target.end(); i++){
		string srcFileName, jobName;
		double resoX = 0;
		double resoY = 0;
		SYSTEMTIME date = {0};
		if(PDB_GetJIPhotoOnJobPrint(inPrintConditionName, i->first, srcFileName, resoX, resoY, jobName, date)){
			//->取得成功
			if(!srcFileName.empty()){
				//画像ファイルをコピーする
				string srcFilePath = rootDirPath + GetJIPhotoDir_PDB(isDED, i->second) + srcFileName;
				string dstFilePath = rootDirPath + GetJIPhotoDir_PDB(isDED, i->second) + dstFileName;
				CopyFile(srcFilePath.c_str(), dstFilePath.c_str(), FALSE);

				//紙DB上の画像ファイル名を変更する(画像ファイル名以外は同じ値)
				PDB_SetJIPhotoOnJobPrint(inPrintConditionName, i->first, dstFileName, resoX, resoY, jobName, date);
			}

		}
	}

	return true;
}

/**
 *	ジョブ印刷時のJI撮影画像ファイルの保存先取得(JI画面用)
 *	@param[in]	inDED : DED設定
 *	@param[in]	inSide : 対象面
 *	@retval	保存先(PrintNav.exeからの相対パス)
**/
std::string CDataPrintSettings::GetJIPhotoDir_PDB(BOOL inDED, DEF_PRINT_SIDE inSide)
{
	if(inDED){
		//->DED
		if(inSide == DEF_PRINT_FRONT){
			//->表面
			return _T("Preferences\\PrintMonitor\\DED\\Slave\\");
		}else{
			//->裏面
			return _T("Preferences\\PrintMonitor\\DED\\Master\\");
		}
	}else{
		//->SES
		//表面
		return _T("Preferences\\PrintMonitor\\SES\\");
	}
}

/**
 *	ジョブ印刷時のJI撮影画像ファイル名を取得する(JI画面用)
 *	@param[in]	inPrintConditionName : 印刷条件名
 *	@param[out]	outFileNameF : 画像ファイル名(表面)
 *	@param[out]	outFileNameB : 画像ファイル名(裏面)
 *	@retval	true: 成功
 *	@retval	false: 失敗
**/
bool CDataPrintSettings::GetJIPhotoInfo_PDB(std::string inPrintConditionName, std::string& outFileNameF, std::string& outFileNameB)
{
	if(!m_SystemSetting || !m_PaperDB_Callbacks || inPrintConditionName.empty()){
		return false;
	}

	//印刷条件に撮影情報が登録されていたら、登録されている画像ファイル名を取得する
	typedef tuple<eComSystemType, string&> DEF_PARAM;
	BOOL isDED = m_SystemSetting->IsDED();
	auto PDB_GetJIPhotoOnJobPrint = m_PaperDB_Callbacks->PDB_GetJIPhotoOnJobPrint;

	//DED用のターゲット
	list<DEF_PARAM> targetDED;
	targetDED.push_back(DEF_PARAM(COM_SYSTEM_TYPE_PART, outFileNameF));
	targetDED.push_back(DEF_PARAM(COM_SYSTEM_TYPE_SELF, outFileNameB));
	//SES用のターゲット
	list<DEF_PARAM> targetSES;
	targetSES.push_back(DEF_PARAM(COM_SYSTEM_TYPE_SELF, outFileNameF));
		
	//ターゲット選択
	auto& target = isDED ? targetDED : targetSES;
	for(auto i = target.begin(); i != target.end(); i++){
		string fileName, jobName;
		double resoX = 0;
		double resoY = 0;
		SYSTEMTIME date = {0};
		if(PDB_GetJIPhotoOnJobPrint(inPrintConditionName, get<0>(*i), fileName, resoX, resoY, jobName, date)){
			//->取得成功
			get<1>(*i) = fileName;
		}
	}


	return true;
}


/**
 *	ジョブ印刷時のJI撮影画像ファイルを削除する(JI画面用)
 *	@param[in]	inFileNameF : 画像ファイル名(表面)
 *	@param[in]	inFileNameB : 画像ファイル名(裏面)
 *	@retval	true: 成功
 *	@retval	false: 失敗
**/
bool CDataPrintSettings::DeleteJIPhotoInfo_PDB(std::string inFileNameF, std::string inFileNameB)
{
	if(!m_SystemSetting){
		return false;
	}

	typedef pair<DEF_PRINT_SIDE, string&> DEF_PARAM;
	string rootDirPath = GetModuleDirectory();
	BOOL isDED = m_SystemSetting->IsDED();

	//DED用のターゲット
	list<DEF_PARAM> targetDED;
	targetDED.push_back(DEF_PARAM(DEF_PRINT_FRONT, inFileNameF));
	targetDED.push_back(DEF_PARAM(DEF_PRINT_BACK, inFileNameB));
	//SES用のターゲット
	list<DEF_PARAM> targetSES;
	targetSES.push_back(DEF_PARAM(DEF_PRINT_FRONT, inFileNameF));
		
	//ターゲット選択
	auto& target = isDED ? targetDED : targetSES;
	for(auto i = target.begin(); i != target.end(); i++){
		if(!i->second.empty()){
			string filePath = rootDirPath + GetJIPhotoDir_PDB(isDED, i->first) + i->second;
			ForceToDeleteFile(filePath.c_str());	
		}
	}


	return true;
}

/**
 *	印刷条件複製後に検査関連の情報を削除するかを確認する
 *	@param[in]	inPrintConditionName : 印刷条件名
 *	@retval	true: 成功
 *	@retval	false: 失敗
**/
bool CDataPrintSettings::DeleteInspectionInfo(std::string inPrintConditionName)
{
	// 引数確認.
	if(!m_SystemSetting || !m_PaperDB_Callbacks || inPrintConditionName.empty()){
		WriteToLogBuf(LOG_DEBUG, "!!!DeleteInspectionInfo(1)");
		return false;
	}

	// チェックボックスOFF または JIオプション(OCRデコード/フルバリ検査)未搭載の場合は削除する
	if(!m_DataIF->GetDuplicateJIAreaSettings() || !m_DataIF->IsJIDuplicatingEnable()){
		DeleteInspectionInfo_PDB(inPrintConditionName);
	} else {
		// 削除しない場合は、ライン間同期ツール向けの設定エクスポートを実施
		Stub_PrintConditionExporter::onExportInspectionData("Preferences\\InspectionParams");
		Stub_PrintConditionExporter::onExportInspectionData("Preferences\\TemporalMask");
	}

	return true;
}

/**
 *	印刷条件複製後に検査関連の情報を削除する
 *	@param[in]	inPrintConditionName : 印刷条件名
 *	@retval	true: 成功
 *	@retval	false: 失敗
**/
bool CDataPrintSettings::DeleteInspectionInfo_PDB(std::string inPrintConditionName)
{
	if(!m_SystemSetting || !m_PaperDB_Callbacks || inPrintConditionName.empty()){
		return false;
	}

	// 非検査エリア(紙端)
	{
		double value = m_DataIF->GetDuplicateInitNonInspectionAreaDefault();
		//DED/SES - Front
		eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		if(!m_PaperDB_Callbacks->PDB_SetNonInspectionAreaTable((char*)inPrintConditionName.c_str(), systemType, value, value)) {
			return false;
		}
		//DED -  Back
		if(TRUE == m_SystemSetting->IsDED()) {
			systemType = m_SystemSetting->IsSelf(DEF_PRINT_BACK + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
			if(!m_PaperDB_Callbacks->PDB_SetNonInspectionAreaTable((char*)inPrintConditionName.c_str(), systemType, value, value)) {
				return false;
			}
		}
	}

	// 非検査エリア(矩形)
	{
		vector<STRUCT_NON_INSPECTION_AREA> NonInspectionList;
		NonInspectionList.clear();
		//DED/SES - Front
		eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		if(!m_PaperDB_Callbacks->PDB_SetNonInspectionAreaList((char*)inPrintConditionName.c_str(), systemType, NonInspectionList)) {
				return false;
		}
		//DED -  Back
		if(TRUE == m_SystemSetting->IsDED()) {
			eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_BACK + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
			if(!m_PaperDB_Callbacks->PDB_SetNonInspectionAreaList((char*)inPrintConditionName.c_str(), systemType, NonInspectionList)) {
					return false;
			}
		}	
	}

	// デコード
	{
		// 紙DBからレイアウト情報を取得
		DEF_LENGTH_UNIT systemUnit = DEF_LENGTH_MM;
		m_SystemSetting->GetSystemUnitLength(&systemUnit);
		ST_JOB_IMAGE_POSITION			pdb_layout_image_position;
		memset(&pdb_layout_image_position, 0, sizeof(pdb_layout_image_position));
		ST_JOB_FLUSHING_PATTERN			pdb_layout_FlushingPatternn;
		memset(&pdb_layout_FlushingPatternn, 0, sizeof(pdb_layout_FlushingPatternn));
		ST_JOB_FLUSHING_PATTERN_ADD		pdb_layout_FlushingPattern_Add;
		memset(&pdb_layout_FlushingPattern_Add, 0, sizeof(pdb_layout_FlushingPattern_Add));
		ST_JOB_VERIFY_MARK				pdb_layout_VerifyMark;
		memset(&pdb_layout_VerifyMark, 0, sizeof(pdb_layout_VerifyMark));
		ST_JOB_POSTPROCESS				pdb_layout_PostProcess;
		memset(&pdb_layout_PostProcess, 0, sizeof(pdb_layout_PostProcess));
		ST_JOB_POSTPROCESS_ADD				pdb_layout_PostProcessAdd;
		memset(&pdb_layout_PostProcessAdd, 0, sizeof(pdb_layout_PostProcessAdd));
		ST_JOB_DECODE				pdb_layout_Decode;
		ST_JOB_FLUSHING_PATTERN_ADD2		pdb_layout_FlushingPattern_Add2;
		memset(&pdb_layout_FlushingPattern_Add2, 0, sizeof(pdb_layout_FlushingPattern_Add2));
		BOOL pdb_layout_verticalAlignment_PP_FLSPTN = FALSE;
		m_PaperDB_Callbacks->PDB_GetLayoutImageParameterWithPrintConditionNameEx(inPrintConditionName.c_str(), systemUnit, 
			pdb_layout_image_position, pdb_layout_FlushingPatternn, pdb_layout_FlushingPattern_Add, pdb_layout_FlushingPattern_Add2, pdb_layout_VerifyMark, pdb_layout_PostProcess, pdb_layout_PostProcessAdd, pdb_layout_Decode, pdb_layout_verticalAlignment_PP_FLSPTN);
		// デコードのみ削除して、再設定する.
		ST_JOB_DECODE				pdb_layout_Decode_new;
		m_PaperDB_Callbacks->PDB_SetLayoutImageParameterWithPrintConditionNameEx(inPrintConditionName.c_str(), systemUnit, 
			pdb_layout_image_position, pdb_layout_FlushingPatternn, pdb_layout_FlushingPattern_Add, pdb_layout_FlushingPattern_Add2, pdb_layout_VerifyMark, pdb_layout_PostProcess, pdb_layout_PostProcessAdd, pdb_layout_Decode_new, pdb_layout_verticalAlignment_PP_FLSPTN);
	}

	// 一時マスク
	{
		// ファイルパス取得.
		string temporalMaskFile = "";
		if(m_PaperDB_Callbacks->PDB_GetTemporalMaskFile(inPrintConditionName.c_str(), temporalMaskFile)){
			if(temporalMaskFile != ""){
				// ローカルのファイル名
				char localPath[_MAX_PATH] = {0};
				_snprintf_s(localPath , _TRUNCATE, "%s%s" , GetModuleDirectory(), temporalMaskFile.c_str());
				DeleteFile(localPath);
		
				// Save the temporal mask file parh.
				m_PaperDB_Callbacks->PDB_SetTemporalMaskFile(inPrintConditionName.c_str(), "");
			}
		}
	}


	// ジョブ情報(印刷モニター画像取り込み時)
	{
		//ジョブ印刷時のJI撮影画像ファイル名取得
		string jiPhotoFileNameF, jiPhotoFileNameB;
		GetJIPhotoInfo_PDB(inPrintConditionName, jiPhotoFileNameF, jiPhotoFileNameB);
		//ジョブ印刷時のJI撮影画像ファイル削除
		DeleteJIPhotoInfo_PDB(jiPhotoFileNameF, jiPhotoFileNameB);
		SYSTEMTIME lt = {0};
		eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		m_PaperDB_Callbacks->PDB_SetJIPhotoOnJobPrint(inPrintConditionName, systemType, "", 0, 0, "", lt);
	}

	return true;
}

/*!
 *  \brief レイアウトイメージ情報を取得する./ Get the layout image information from the print condition
 *  \param inPrintConditionName [IN]印刷条件名 / Print condition
 *  \param inUnit [IN]単位 / Unit （UNIT_MODE_MM or UNIT_MODE_INCH or UNIT_MODE_POINT）
 *  \param outLayoutImagePosition		[OUT] 画像データ情報 / Image position information
 *  \param outLayoutFlushingPattern		[OUT] フラッシングパターン情報 / Flushing pattern information
 *  \param outLayoutFlushingPatternAdd	[OUT] フラッシングパターンLineオプション情報 / Flushing pattern line option information
 *  \param outLayoutFlushingPatternAdd2	[OUT] Flushing pattern 2nd line information
 *  \param outLayoutVerifyMark			[OUT] 検査マーク情報 / Verify mark information
 *  \param outLayoutPostProcess			[OUT] 後処理マーク情報 / Post process information
 *  \param outEnableAlignBottomPPFLSPTN	[OUT] The status to enable bottom alignment of PostProcess and Flushing Pattern
 *  \return true：成功 / Success、false：失敗 / Failure.
 */
bool CDataPrintSettings::GetFlushingPattern(const std::string& inPrintConditionName, ST_JOB_FLUSHING_PATTERN& outFlushingPattern, ST_JOB_FLUSHING_PATTERN_ADD& outFlushingPatternAdd, BOOL& outVerticalAlignment_PP_FLSPTN)
{
	DEF_LENGTH_UNIT systemUnit = DEF_LENGTH_MM;
	m_SystemSetting->GetSystemUnitLength(&systemUnit);
	ST_JOB_IMAGE_POSITION			pdb_layout_image_position;
	memset(&pdb_layout_image_position, 0, sizeof(pdb_layout_image_position));
	memset(&outFlushingPattern, 0, sizeof(outFlushingPattern));
	memset(&outFlushingPatternAdd, 0, sizeof(outFlushingPatternAdd));
	ST_JOB_VERIFY_MARK				pdb_layout_VerifyMark;
	memset(&pdb_layout_VerifyMark, 0, sizeof(pdb_layout_VerifyMark));
	ST_JOB_POSTPROCESS				pdb_layout_PostProcess;
	memset(&pdb_layout_PostProcess, 0, sizeof(pdb_layout_PostProcess));
	ST_JOB_POSTPROCESS_ADD				pdb_layout_PostProcessAdd;
	memset(&pdb_layout_PostProcessAdd, 0, sizeof(pdb_layout_PostProcessAdd));
	ST_JOB_DECODE				pdb_layout_Decode;
	ST_JOB_FLUSHING_PATTERN_ADD2		pdb_layout_FlushingPattern_Add2;
	memset(&pdb_layout_FlushingPattern_Add2, 0, sizeof(pdb_layout_FlushingPattern_Add2));
	outVerticalAlignment_PP_FLSPTN = FALSE;

	return m_PaperDB_Callbacks->PDB_GetLayoutImageParameterWithPrintConditionName(
		inPrintConditionName.c_str(),
		systemUnit, 
		pdb_layout_image_position,
		outFlushingPattern,
		outFlushingPatternAdd,
		pdb_layout_FlushingPattern_Add2,
		pdb_layout_VerifyMark,
		pdb_layout_PostProcess,
		pdb_layout_PostProcessAdd,
		outVerticalAlignment_PP_FLSPTN);
}

/**
	モノクロ印刷設定を取得する(表)
	@retval	true : モノクロ印刷
	@retval	false : 多色印刷
*/
bool CDataPrintSettings::IsMonochromePrint_F()
{
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_CurrentJob)
		{
			return m_CurrentJob->Info.isMonochromePrint_F;
		}
	}
	
	return nullptr;
}