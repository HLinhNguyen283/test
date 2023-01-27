/*! \file DataJetInspection.cpp
 *  \brief data class: Jet Inspection
 *  \author GCS Tan Ho
 *  \date 2013/12/17 GCS Tan Ho created
*/


// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataJetInspection.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "DataIF.h"
#include "PrintConditionGUIDef.h"
#include "MainteJetInspection_Callbacks.h"
#include "Stub_JobManagerIF.h"
#include "Stub_PaperDBIF.h"
#include "Stub_PrinterStateIF.h"
#include "Ini_PrinterDescriptor.h"

// ==========================================================================

/**
 * constructor
 * @note General process
 * 
 *		Initialize the member variables
**/
CDataJetInspection::CDataJetInspection()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataJetInspection::~CDataJetInspection()
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
bool CDataJetInspection::Initialize(CDataSystemSetting* inSystemSetting)
{
	if (!inSystemSetting) {
		return false;
	}

	// Store system setting handle
	m_systemSetting = inSystemSetting;
	m_JM_Callbacks = m_systemSetting->GetJobManager_Callbacks();
	m_PaperDB_Callbacks = m_systemSetting->GetPaperDB_Callbacks();
	m_JI_Callbacks = m_systemSetting->GetJetInspection_Callbacks();



	return true;
}

/**
 *	Remove menber variables in the class
 *	@retval	TRUE: success
 *	@retval	FALSE: failure
**/
bool CDataJetInspection::Finalize()
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
void CDataJetInspection::ResetMember()
{
	m_systemSetting = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
	m_printConditionName = "";
	m_currJetInspection = "";
	m_selectedID = 0;
	m_jetInspectionList.clear();
	m_isExistInList = false;
	m_isNotSetCurrentJetInspection = false;
	m_JI_Callbacks = NULL;
	m_isImageStorage = false;
	m_isCurrentImageStorage = false;
}

/**
 *	Check the user change the jet inspection parameters
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataJetInspection::IsChanged()
{
	if ((IsChangedJetInspection() == true) || (IsChangedImageStorage() == true))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	Check the user change the jet inspection
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataJetInspection::IsChangedJetInspection()
{
	if (m_currJetInspection.compare(m_jetInspectionList.at(m_selectedID)) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	Check the user change the image storage
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataJetInspection::IsChangedImageStorage()
{
	if (m_isImageStorage != m_isCurrentImageStorage)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	Check the control has valid selection item
 *	@retval	true : valid
 *	@retval	false : invalid
 *	@note
 *	
 *		
**/
bool CDataJetInspection::IsHaveValidSelectedItem()
{
	if (m_jetInspectionList.size() == 0)
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
bool CDataJetInspection::Save()
{

	if ((m_systemSetting->IsStartedFromJobListScreen()) && (IsChanged()))
	{
		// The Print Condition GUI is started from Job List screen
		bool isSucceed = Stub_JobManagerIF::SetJICondition(m_printConditionName.c_str(), m_jetInspectionList.at(m_selectedID), m_isImageStorage);

		if(isSucceed)
		{
			UpdateCurrentJetInspection(m_printConditionName.c_str());
		}

		return isSucceed;
	}
	else
	{
		if (IsChangedJetInspection())
		{
			bool isSucceed = m_PaperDB_Callbacks->PDB_SetJICondition(m_printConditionName.c_str(), m_jetInspectionList.at(m_selectedID).c_str());
			if(isSucceed == false)
			{
				return false;
			}
		}

		if (IsChangedImageStorage())
		{
			bool isSucceed = Stub_PaperDBIF::SetImageStorage(m_printConditionName.c_str(), m_isImageStorage);
			if(isSucceed == false)
			{
				return false;
			}
		}

		// CurrentJetInspectionの更新はSetImageStorageの後にやらないといけない
		UpdateCurrentJetInspection(m_printConditionName.c_str());

		return true;
	}
}


/**
 *	Update the current selection Jet Inspection in case the user change the print condition name
 *	@param[in]	inKeyName : key name
 *	@note Notes
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
void CDataJetInspection::UpdateCurrentJetInspection(const char* inKeyName)
{
	if (!inKeyName)
	{
		return;
	}

	// Initialize Jet Inspection list
	InitializeJetInspectionList();

	// Backup the current Print Condition Name
	m_printConditionName = inKeyName;

	bool isSuccess = false;

	m_currJetInspection = "";
	m_isCurrentImageStorage = false;
	// The Print Condition GUI is started from the Job List Screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		isSuccess = Stub_JobManagerIF::GetJICondition(inKeyName, m_currJetInspection, m_isCurrentImageStorage);
	}
	// started from System Setting Screen
	else
	{
		isSuccess = Stub_PaperDBIF::GetJICondition(inKeyName, m_currJetInspection);
		m_isCurrentImageStorage = Stub_PaperDBIF::IsImageStorage(inKeyName);
	}

	m_isExistInList = true;
	m_selectedID = -1;
	if (isSuccess)
	{
		vector<string>::iterator it = std::find(m_jetInspectionList.begin(), m_jetInspectionList.end(), m_currJetInspection);
		if (m_jetInspectionList.end() != it)
		{
			m_selectedID = std::distance(m_jetInspectionList.begin(), it);
		}
		else
		{
			m_isExistInList = false;
			m_jetInspectionList.insert(m_jetInspectionList.begin(), m_currJetInspection);
			m_selectedID = 0;
		}

		m_isImageStorage = m_isCurrentImageStorage;
	}else {
		SetSelectedItemWhenNotSetCurrent();
	}

//	::SendMessage(GetGenSetupDialog(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
}

/**
 *	Set selected item when current inpsection is not set
 *	@note
 *	
**/
void CDataJetInspection::SetSelectedItemWhenNotSetCurrent()
{
	//Only has one item
	if(1 == m_jetInspectionList.size()) {
		m_selectedID = 0;
	}
	// > 1 item
	else {
		if(!m_isNotSetCurrentJetInspection) {
			string emptyInspection = "";
			m_jetInspectionList.push_back(emptyInspection);
			m_selectedID = m_jetInspectionList.size() - 1;
			m_isNotSetCurrentJetInspection = true;
		}else {
			m_selectedID = m_jetInspectionList.size() - 1;
		}
	}
}

/**
 *	Initialize the list of Jet Inspection
 *	@note
 *	
 *		
**/
void CDataJetInspection::InitializeJetInspectionList()
{
	m_jetInspectionList.clear();
	m_isNotSetCurrentJetInspection = false;

	// Get list of Jet Inspection
	if (TRUE == m_JI_Callbacks->MainteJetInspection_EnterInspectionParamsDBCS())
	{
		long nItemID = 0;
		char strJI[256] = {0};
		while (TRUE == m_JI_Callbacks->MainteJetInspection_GetInspectionParamsName(nItemID++, strJI))
		{
			m_jetInspectionList.push_back(strJI);
		}
		m_JI_Callbacks->MainteJetInspection_LeaveInspectionParamsDBCS();
	}
}

/**
 *	Get an item of Jet Inspection list.
 *	@param[in]	inItemID : an item ID (0-N)
 *	@param[out]	outJetInspectionName : a pointer to get a Jet Inspection
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note
 *	
 *		
**/
bool CDataJetInspection::GetJetInspectionList(long inItemID, const char** outJetInspectionName)
{
	if (inItemID < 0 || inItemID >= (long)m_jetInspectionList.size() || !outJetInspectionName)
	{
		return false;
	}

	*outJetInspectionName = m_jetInspectionList.at(inItemID).c_str();
	return true;
}

/**
 *	Check the JetInspection of current selection Print Condition is existed in the Jet Inspection list of system
 *	@retval	true : exist
 *	@retval	false : not exist
 *	@note
 *	
 *		
**/
bool CDataJetInspection::IsJIExistInJIList()
{
	return m_isExistInList;
}

/**
 *	Set a selection Jet Inspection
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataJetInspection::SetSelectJetInspection(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_jetInspectionList.size())
	{
		return;
	}

	// Store the selection name of JI
	string jIName = m_jetInspectionList.at(inItemID);

	// Check the case the JI of Print Condition is existed in the JI system list
	if (false == m_isExistInList && inItemID != 0)
	{
		m_jetInspectionList.erase(m_jetInspectionList.begin());
		m_isExistInList = true;
//		::SendMessage(GetGenSetupDialog(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		::SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	}

	vector<string>::iterator it = std::find(m_jetInspectionList.begin(), m_jetInspectionList.end(), jIName);
	if (m_jetInspectionList.end() != it)
	{
		m_selectedID = std::distance(m_jetInspectionList.begin(), it);
	}

	//Remove blank item if has
	if(m_isNotSetCurrentJetInspection && (inItemID != m_jetInspectionList.size() - 1)) {
		m_jetInspectionList.erase(m_jetInspectionList.end() - 1);
//		::SendMessage(GetGenSetupDialog(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		::SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_UPDATE_JET_INSPECTION_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentJetInspection = false;
	}
}

/**
 *	Get a selection Jet Inspection
 *	@param[out]	outItemID : an item ID (0-N)
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		
**/
bool CDataJetInspection::GetSelectJetInspection(long* outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_selectedID;

	return true;
}

/**
 *	画像保管を設定する
 *	@param[in]	inIsEnable : 有効/無効
**/
void CDataJetInspection::SetImageStorage(const bool inIsEnable)
{
	m_isImageStorage = inIsEnable;
}

/**
 *	画像保管を取得する
 *	@retval	true: 有効
 *	@retval	false: 無効
**/
bool CDataJetInspection::IsImageStorage()
{
	return m_isImageStorage;
}

bool CDataJetInspection::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Init Jet Inspection list
	UpdateCurrentJetInspection(inPrintConditionOrSectionID);

	return true;
}

bool CDataJetInspection::ExitScreen()
{
	return true;
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataJetInspection::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentJetInspection)
		WriteToLogBuf(LOG_DEBUG, "Jet inspection is judged as blank.");
	return m_isNotSetCurrentJetInspection;
}

/**
	@brief	全画像保存機能の有効無効
	@retval	true : 有効
	@retval	false : 無効
*/
bool CDataJetInspection::IsImageStorageEnable()
{

	CIni_PrinterDescriptor ini_PrinterDescriptor;
	ini_PrinterDescriptor.Initialize(TRUE);
	long JetInspection = ini_PrinterDescriptor.GetJetInspectionOption();
	bool isHaveEnableJI = ini_PrinterDescriptor.IsEnableJetInspection() ? true : false;

	if((JetInspection == 2 || JetInspection == 4) && isHaveEnableJI)
	{
		if(Stub_PrinterStateIF::IsIMSEnable())
		{
			return true;
		}
	}

	return false;
}

/**
	@brief	JIの領域情報コピーの有効無効
	@retval	true : 有効
	@retval	false : 無効
*/
bool CDataJetInspection::IsJIDuplicatingEnable()
{
	CIni_PrinterDescriptor ini_PrinterDescriptor;
	ini_PrinterDescriptor.Initialize(TRUE);
	long JetInspection = ini_PrinterDescriptor.GetJetInspectionOption();

	if((JetInspection == 2) || (JetInspection == 4 && Stub_PrinterStateIF::IsOCREnable()))
	{
		return true;
	}

	return false;

}

// 全画像保管ボタンのコントロール表示状態更新イベント
void CDataJetInspection::onView_ImageStorage(
		const bool inIsLockPrintCondtion,
		const bool inIsDisablePrintCondition,
		bool& outIsShow,
		bool& outIsEnabled)
{
	// 全画像保存ボタンの表示状態を取得
	outIsShow = IsImageStorageEnable();

	// 全画像保存ボタンの有効状態を判定
	if( (inIsLockPrintCondtion == true) || (inIsDisablePrintCondition == true) ) {
		// 下記のいずれかの条件に該当した場合は無効状態とする
		// 印刷条件のロック設定がON
		// ユーザーが一般かつユーザー制限の「印刷条件の編集」か「ジョブの印刷条件の編集」がOFF
		outIsEnabled = false;
	} else {
		// いずれにも該当しない場合は有効状態とする
		outIsEnabled = true;
	}
}
