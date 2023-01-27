/*! \file DataICCPresetSetting.cpp
 *  \brief data class: ICC preset setting
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include <sstream>

#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "ResDef.h"
#include "DataICCPresetSetting.h"
#include "DataIF.h"

#include "ProhibitChecker.h"

// ==========================================================================

/**
 * constructor
 * @note Normal process
 * 
 *		Initialize the member variables
 *	@note 
 *	
 *		
**/
CDataICCPresetSetting::CDataICCPresetSetting()
{
	ResetMember();
}

/**
 * destructor
 * @note Normal process
 * 
 *		Remove objects which created with dynamic.
 *	@note 
 *	
 *		
**/
CDataICCPresetSetting::~CDataICCPresetSetting()
{
	ResetMember();
}

/**
 * Initialize member variables in the class
 * @retval	true: success
 * @retval	false: failure
 * @note Notes
 * 
 *		The methods to initialize each member variables are written at each getter methods.
 *	@note 
 *	
 *		
**/
bool CDataICCPresetSetting::Initialize(CDataSystemSetting* inSystemSetting
	, CDataPrintMode* inPrintMode)
{
	if(!inSystemSetting || !inPrintMode) {
		return false;
	}

	m_systemSetting = inSystemSetting;
	m_printMode = inPrintMode;
	return true;
}

/**
 *	Remove menber variables in the class
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CDataICCPresetSetting::Finalize()
{
	return true;
}

/**
 *	Initialize ICC Preset list
 *	@note Notes
 *	
 *		
**/
bool CDataICCPresetSetting::InitICCPresetList()
{
	//if(m_ICCPresetListInitialized)	//	initialize once only when the screen is opened
	//	return true;

	m_iccPresetList.clear();
	m_isNotSetCurrentIccPreset = false;

	int nItemID = 0;
//	char strICCPreset[256] = {0};
	string strICCPreset;

	if (false == m_systemSetting->GetPaperDB_Callbacks()->PDB_EnterCommonPaperDBCS())
		return false;

	// Call PaperDB to get a system ICC preset
	while (m_systemSetting->GetPaperDB_Callbacks()->PDB_GetSystemICCPreset(nItemID++, strICCPreset))
	{
		// Store system ICC preset to member variable
		m_iccPresetList.push_back(strICCPreset);
		strICCPreset = "";
	}

	m_systemSetting->GetPaperDB_Callbacks()->PDB_LeaveCommonPaperDBCS();

	//m_ICCPresetListInitialized = true;

	return true;
}

/*! \brief Get an item of ICC preset list.
 *  \param inItemID: an item ID(0-N)
 *  \param outICCPreset: a pointer to get a ICC preset.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataICCPresetSetting::GetICCPresetList(long inItemId, const char** outICCPreset)
{
	if(inItemId < 0 || inItemId >= (long)m_iccPresetList.size() || !outICCPreset)
		return false;

	*outICCPreset = m_iccPresetList.at(inItemId).c_str();
	return true;
}

/*! \brief Set a selection ICC preset.
 *  \param inItemID: an item ID(0-N)
*/
void CDataICCPresetSetting::SetSelectICCPreset(long inItemID, bool inIsInit)
{
	if (m_systemSetting->IsIpdsMode()) {
		return;	// nothing to do
	}
	if(inItemID < 0 || inItemID >= (long)m_iccPresetList.size()) {
		return;
	}

	m_selectedICCPreset.selected_index = inItemID;
	m_selectedICCPreset.icc_preset_name = m_iccPresetList.at(inItemID);

	//Remove blank item if has
	if(!inIsInit && m_isNotSetCurrentIccPreset && (inItemID != m_iccPresetList.size() - 1)) {
		m_iccPresetList.erase(m_iccPresetList.end() - 1);
//		SendMessage(GetGenSetupDialog(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentIccPreset = false;
	}
}

/*! \brief Get a selection ICC preset
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection ICC preset,
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataICCPresetSetting::GetSelectICCPreset(long* outItemID)
{
	if (m_systemSetting->IsIpdsMode()) {
		return false; // nothing to do
	}
	if(!outItemID) {
		return false;
	}

	*outItemID = m_selectedICCPreset.selected_index;
	return true;
}

/*! \brief Check if Current ICC preset is changed or not.
 *  \retval true: changed
 *  \retval false: not changed
*/
bool CDataICCPresetSetting::IsSelectedICCPresetChanged()
{
	if (m_systemSetting->IsIpdsMode()) {
		return false; // nothing to do
	}
	return 0 != m_selectedICCPreset.icc_preset_name.compare(m_initSelectedICCPresetName);
}

/*!
 *  \brief Set the ICC preset to the print condition
 *  \return true：Success, false：Failure.
 */
bool CDataICCPresetSetting::SaveValues()
{
	if (m_systemSetting->IsIpdsMode()) {
		return true; // nothing to do
	}
	if (m_systemSetting->IsStartedFromJobListScreen()){
		//	no need to save (this item is disabled)
		return true;
	}
	if(IsSelectedICCPresetChanged() || NULL != m_PrintConditionOrSectionID) {
		string iccPresetName("");
		if (0 == m_selectedICCPreset.icc_preset_name.compare(m_ICCName_systemSetting)) {
			iccPresetName = m_currICC_systemSetting;
		} else {
			iccPresetName = m_selectedICCPreset.icc_preset_name;
		}

		bool retval = m_systemSetting->GetPaperDB_Callbacks()->PDB_SetICCPreset(m_PrintConditionOrSectionID, iccPresetName.c_str());
		if (retval) {
			m_initSelectedICCPresetName = iccPresetName;
		}

		return retval;
	}
	return true;
}

/*!
 *  \brief Add the ICC preset (to the list in the system)
 *  \param inICCPreset	[IN]ICC preset to be added
 *  \return true：Success, false：Failure.
 */
bool CDataICCPresetSetting::AddSystemICCPreset()
{
	if(!m_systemSetting->GetPaperDB_Callbacks()->PDB_AddSystemICCPreset(m_addICCPresetName))
	{
		return false;
	}
	//紙DBにコミットする。
	m_systemSetting->GetPaperDB_Callbacks()->PDB_CommitPaperDB();

	//Remove blank item if has
	if(m_isNotSetCurrentIccPreset) {
		m_iccPresetList.erase(m_iccPresetList.end() - 1);
		m_isNotSetCurrentIccPreset = false;
	}

	m_iccPresetList.push_back(m_addICCPresetName);

	if(!IsExistName()) {
		RemoveICCPresetNameNotExist();
	}

	//Sort
	sort(m_iccPresetList.begin(), m_iccPresetList.end());

	//Reset selected ICC preset
	for(long index = 0; index < (long)m_iccPresetList.size(); ++index) {
		if(0 == strcmp(m_addICCPresetName, m_iccPresetList.at(index).c_str())) {
			m_selectedICCPreset.selected_index =  index;
			m_selectedICCPreset.icc_preset_name = m_addICCPresetName;
			break;
		}
	}

//	::PostMessage(GetGenSetupDialog(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);

	//Create default string for ICC Preset name
	CancelAddICCPresetNameDlg();
	return true;
}

/*! \brief Set a addition name at Add ICC Preset dialog.
 *  \param inICCPreset: a name of ICC preset
*/
void CDataICCPresetSetting::SetAddICCPresetNameDlg(char* inICCPreset)
{
	if (!inICCPreset || 0 == strlen(inICCPreset))
	{
		return;
	}

#if 0
// 禁止文字チェックは不要になったが、実装は残しておく
	bool hasProhibit = ProhibitChecker::hasProhibitChar(inICCPreset);
	if (hasProhibit) {
		std::stringstream ss;
		ss << ID_MESSAGE_ERROR +  IDM_ERR_PROHIBITCHARS_ICCPRESET_NAME << "\n" << LoadResourceString(IDM_ERR_PROHIBITCHARS_ICCPRESET_NAME, RESOURCE_MSG_STR) << "\n" << ProhibitChecker::prohibitString();
		ShowMessageBox(const_cast<char*>(ss.str().c_str()), MBST_OK | MBST_ICONERROR, NULL);
		return;
	}
#endif
	_snprintf_s(m_addICCPresetName, sizeof(m_addICCPresetName), _TRUNCATE, "%s", inICCPreset);
}

/*! \brief Get a addition name at Add ICC Preset dialog.
 *  \retval a name of ICC Preset
*/
const char* CDataICCPresetSetting::GetAddICCPresetNameDlg()
{
	return m_addICCPresetName;
}

/*! \brief Cancel to save the settings of Add ICC Preset dialog.
*/
void CDataICCPresetSetting::CancelAddICCPresetNameDlg()
{
	//Reset ICC Preset to display on Add dialog
	AppendICCPresetNameIndex((char*)LoadResourceString(IDS_NEW_ICC_PRESET, RESOURCE_STR));
}

/*! \brief Check if the new name already exists or not
 *  \return true：exist, false：not exist.
*/
bool CDataICCPresetSetting::IsAddICCPresetNameExist()
{
	if(m_iccPresetList.size() > 0) {
		bool isExist = false;

		for(long index = 0; index < (long)m_iccPresetList.size(); ++index) {
			if(0 == strcmp(m_addICCPresetName, m_iccPresetList.at(index).c_str()))
			{
				isExist = true;
				break;
			}
		}

		return isExist;
	}else {
		//Always no exist
		return false;
	}
}

/*! \brief Check if ICC preser name is exist in the list of ICC Presets or not
 *  \return true：exist, false：not exist.
*/
bool CDataICCPresetSetting::IsExistName()
{
	return m_isExistName;
}

/*! \brief Remove ICC preset name does not exist
*/
void CDataICCPresetSetting::RemoveICCPresetNameNotExist()
{
	m_iccPresetList.erase(m_iccPresetList.begin());
	m_isExistName = true;
}

/*! \brief Append index for ICC Preset Name
 *  \param inICCPresetName: a name of ICC preset
*/
void CDataICCPresetSetting::AppendICCPresetNameIndex(char inOriICCPresetName[256])
{
	if (m_iccPresetList.size() > 0)
	{
		long nIndex = 0;
		char tmpICCPreset[256] = {0};
		bool isExist = false;

		while (true)
		{
			isExist = false;
			memset(tmpICCPreset, 0x00, sizeof(tmpICCPreset));

			if(0 == nIndex) {
				_snprintf_s(tmpICCPreset, _TRUNCATE, "%s", inOriICCPresetName);
				++nIndex;
			}else {
				_snprintf_s(tmpICCPreset, _TRUNCATE, "%s(%d)", inOriICCPresetName, nIndex++);
			}

			// Check the ICC Preset name does not exist
			for(long index = 0; index < (long)m_iccPresetList.size(); ++index) {
				if(0 == strcmp(tmpICCPreset, m_iccPresetList.at(index).c_str()))
				{
					isExist = true;
					break;
				}
			}

			if(!isExist) {
				_snprintf_s(m_addICCPresetName, sizeof(m_addICCPresetName), _TRUNCATE, "%s", tmpICCPreset);
				break;
			}
		}
	}
	else
	{
		_snprintf_s(m_addICCPresetName, _TRUNCATE, "%s", inOriICCPresetName);
	}
}

/*! \brief get state of ICC profile is assigned to ICCPreset or not.
 *  \retval true: not assigned
 *  \retval false: assigned
*/
bool CDataICCPresetSetting::IsICCProfileNotAssignedToICCPreset()
{
	if (m_systemSetting->IsIpdsMode()) {
		return false; // nothing to do
	}

	//!< モノクロ機の場合は、ICC プリセット設定状況のチェックを行わない (モノクロ機では、ICC プリセットへのICC設定は必須ではないため)
	if (m_systemSetting->IsMonochromePrinter()) {
		return false;	// Assignment not required
	}

	PrintMode* printMode = m_printMode->GetSelectPrintMode();
	return !m_systemSetting->GetPaperDB_Callbacks()->PDB_CheckICCPresetRegistered(m_selectedICCPreset.icc_preset_name.c_str(), printMode->Data.RESOURCE_ID);
}

/*!
 *  \brief Check if the newly added ICC preset is used in print condition.
 *    If not used, the dialog box with the warning message that the unused ICC preset will be discarded will be displayed.
 */
void CDataICCPresetSetting::CheckICCPresetUse()
{
	m_systemSetting->GetPaperDB_Callbacks()->PDB_CheckICCPresetUse();
}

/**
 *	Reset menber variables in the class
 *	@note General process
 *	
 *		Reset the member variables in the class
 *		Remove objects which created with dynamic.
**/
void CDataICCPresetSetting::ResetMember()
{
	m_systemSetting = NULL;
	m_printMode = NULL;
	m_selectedICCPreset.selected_index = 0;
	m_selectedICCPreset.icc_preset_name = "";
	m_iccPresetList.clear();
	m_isExistName = true;
	memset(m_addICCPresetName, 0x00, sizeof(m_addICCPresetName));
	m_initSelectedICCPresetName = "";
//	m_PrintConditionOrSectionID = NULL;
	memset(m_PrintConditionOrSectionID, 0x00, sizeof(m_PrintConditionOrSectionID));
	m_currICC_systemSetting = "";
	m_ICCName_systemSetting = "";
	m_isNotSetCurrentIccPreset = false;
	memset(m_curICCPreset, 0x00, sizeof(m_curICCPreset));
}

bool CDataICCPresetSetting::EnterScreen(const char* inPrintConditionOrSectionID)
{
	//m_ICCPresetListInitialized = false;
	return UpdatePrintCondition(inPrintConditionOrSectionID);
}

bool CDataICCPresetSetting::ExitScreen()
{
	return true;
}

/**
 *	Obtain the current ICC preset setting in case the user change the print condition name
 *	@retval	true: getting succeed
 *	@retval	false: getting fail
 *	@note General process
 *	
 *		In case the Print Condition GUI is started from:
 *			Job List Screen
 *				- call JM_GetICCPreset
 *			System Setting Screen
 *				- call >PDB_GetICCPreset
 *
 *	@note Note
 *		Call CDataICCPresetSetting::SetSelectICCPreset after that(m_curICCPreset) to select current ICC preset
**/
bool CDataICCPresetSetting::ObtainCurrentICCPreset(const char* inKeyName)
{
	//Reserve Section ID or Print condition name
	_snprintf_s(m_PrintConditionOrSectionID, _TRUNCATE, "%s", inKeyName);
	// Get the current ICC preset item
	if(m_systemSetting->IsStartedFromJobListScreen()) {
		string strICC ("");
		//Get the current ICC preset string
		if(!m_systemSetting->GetJobManager_Callbacks()->JM_GetICCPreset(inKeyName, strICC)) {
			return false;
		}
		long iccListSize = (long)m_iccPresetList.size();
		long index = 0;

		for(index = 0; index < iccListSize; ++index) {
			if(0 == strICC.compare(m_iccPresetList.at(index).c_str())) {
				m_selectedICCPreset.selected_index =  index;
				m_selectedICCPreset.icc_preset_name = strICC;
				break;
			}
		}

		if(index == iccListSize) {
			m_iccPresetList.push_back(strICC);
			m_selectedICCPreset.selected_index =  index;
			m_selectedICCPreset.icc_preset_name = strICC;
		}
		//return true;
	}
	//System setting
	else {
		string strICC ("");
		string strSavedICC ("");
		bool isExistName = false;

		//Get the current ICC preset string
		if(!m_systemSetting->GetPaperDB_Callbacks()->PDB_GetICCPreset(inKeyName, strICC, isExistName, strSavedICC)) {
			SetSelectedItemWhenNotSetCurrent();
			return true;
		}

		m_currICC_systemSetting = strICC;
		if (0 != strICC.compare(strSavedICC)) {
			m_ICCName_systemSetting = strICC + " (" + strSavedICC + ")";
		} else {
			m_ICCName_systemSetting = strICC;
		}

		m_isExistName = isExistName;

		if(m_isExistName) {
			for(long index = 0; index < (long)m_iccPresetList.size(); ++index) {
				if(0 == strICC.compare(m_iccPresetList.at(index).c_str())) {
					m_selectedICCPreset.selected_index =  index;
					m_iccPresetList.at(index) = m_ICCName_systemSetting;
					break;
				}
			}
		}else {
			//Insert to top of list
			m_iccPresetList.insert(m_iccPresetList.begin(), m_ICCName_systemSetting);
			m_selectedICCPreset.selected_index = 0;
		}

		m_selectedICCPreset.icc_preset_name = m_ICCName_systemSetting;
	}

	//Initialize for m_initSelectedICCPresetName member
	m_initSelectedICCPresetName = m_selectedICCPreset.icc_preset_name;

	//Create default string for ICC Preset name
	CancelAddICCPresetNameDlg();

//	::PostMessage(GetGenSetupDialog(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_ICC_PRESET_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	return true;
}

/**
 *	Set selected item when current icc preset is not set
 *	@note
 *	
**/
void CDataICCPresetSetting::SetSelectedItemWhenNotSetCurrent()
{
	//Only has one item
	if(1 == m_iccPresetList.size()) {
		m_selectedICCPreset.selected_index =  0;
		m_selectedICCPreset.icc_preset_name = m_iccPresetList.at(0).c_str();
	}
	// > 1 item
	else {
		if(!m_isNotSetCurrentIccPreset) {
			string emptyDryer = "";
			m_iccPresetList.push_back(emptyDryer);
			m_selectedICCPreset.selected_index = m_iccPresetList.size() - 1;
			m_selectedICCPreset.icc_preset_name = m_iccPresetList.at(m_iccPresetList.size() - 1).c_str();
			m_isNotSetCurrentIccPreset = true;
		}
	}
}

bool CDataICCPresetSetting::UpdatePrintCondition(const char* inKeyName)
{
	bool retval = true;
	if (m_systemSetting->IsIpdsMode()) {
		return retval;	// nothing to do
	}
	// Init ICC Preset list
	retval = retval && InitICCPresetList();
	// Obtain ICC preset of the current print condition
	retval = retval && ObtainCurrentICCPreset(inKeyName);
	//Init index of selection dryer setting
	SetSelectICCPreset(m_selectedICCPreset.selected_index, true);

	return retval;
}

/**
 *	Check the control has valid selection item
 *	@retval	true : valid
 *	@retval	false : invalid
 *	@note
 *
**/
bool CDataICCPresetSetting::IsHaveValidSelectedItem()
{
	if (m_systemSetting->IsIpdsMode()) {
		return true; // nothing to do
	}
	return (m_iccPresetList.size() != 0);
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataICCPresetSetting::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentIccPreset)
		WriteToLogBuf(LOG_DEBUG, "ICC preset is judged as blank.");
	return m_isNotSetCurrentIccPreset;
}