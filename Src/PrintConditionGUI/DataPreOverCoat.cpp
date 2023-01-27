/*! \file DataPreOverCoat.cpp
 *  \brief data class: Precoat and Overcoat data
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/


// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataPreOverCoat.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg_OP.h"
#include "DataIF.h"
#include "PrintConditionGUIDef.h"

// ==========================================================================

/**
 * constructor
 * @note General process
 * 
 *		Initialize the member variables
**/
CDataPreOverCoat::CDataPreOverCoat()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataPreOverCoat::~CDataPreOverCoat()
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
bool CDataPreOverCoat::Initialize(CDataSystemSetting* inSystemSetting)
{
	if (!inSystemSetting) {
		return false;
	}

	// Store system setting handle
	m_systemSetting = inSystemSetting;
	m_JM_Callbacks = m_systemSetting->GetJobManager_Callbacks();
	m_PaperDB_Callbacks = m_systemSetting->GetPaperDB_Callbacks();

	return true;
}

/**
 *	Remove menber variables in the class
 *	@retval	TRUE: success
 *	@retval	FALSE: failure
**/
bool CDataPreOverCoat::Finalize()
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
void CDataPreOverCoat::ResetMember()
{
	m_systemSetting = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
	memset(&m_preOverCoat, 0, sizeof(m_preOverCoat));
	m_printConditionName = "";
	m_overcoatMin = 0;
	m_overcoatMax = 0;
	m_precoatMin = 0;
	m_precoatMax = 0;
	m_selectedOvercoatID = 0;
	m_selectedPrecoatID = 0;
}

/**
 *	Check the user change the precoat or overcoat
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataPreOverCoat::IsChanged()
{
	if (m_currPreOverCoat.OVR_COAT_PART != m_preOverCoat.OVR_COAT_PART
		|| m_currPreOverCoat.OVR_COAT_SELF != m_preOverCoat.OVR_COAT_SELF
		|| m_currPreOverCoat.PRE_COAT_PART != m_preOverCoat.PRE_COAT_PART
		|| m_currPreOverCoat.PRE_COAT_SELF != m_preOverCoat.PRE_COAT_SELF)
	{
		return true;
	}
	return false;
}

/**
 *	Notify to save head alignment for Print Condition
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note
 *	
 *		
**/
bool CDataPreOverCoat::Save()
{
	bool isSucceed = true;
	if (IsChanged())
	{
		if (m_systemSetting->IsStartedFromJobListScreen())
		{
			isSucceed = m_JM_Callbacks->JM_SetPreOverCoat(m_printConditionName.c_str(), m_preOverCoat); 
		}
		else
		{
			isSucceed = m_PaperDB_Callbacks->PDB_SetPreOverCoat(m_printConditionName.c_str(), m_preOverCoat);
		}

		if(isSucceed) {
			UpdateCurrentPreOverCoat(m_printConditionName.c_str());
		}
	}
	return isSucceed;
}

/**
 *	Get a range of precoat settings
 *	@param[out]	outMin : a pointer to get the minimum of range
 *	@param[out]	outMax : a pointer to get the maximum of range
 *	@note
 *	
 *		
**/
void CDataPreOverCoat::GetPrecoatRange(long* outMin, long* outMax)
{
	if (!outMin || !outMax)
	{
		return;
	}

	*outMin = DEF_PRECOAT_MIN;
	*outMax = DEF_PRECOAT_MAX;
}

/**
 *	Set a selection precoat
 *	@param[in]	inPrinterID : a printer ID(1-2)
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note
 *	
 *		
**/
void CDataPreOverCoat::SetSelectPrecoat(long inPrinterID, long inItemID)
{
	if (inPrinterID < 1 || inPrinterID > 2
		|| inItemID < DEF_PRECOAT_MIN || inItemID > DEF_PRECOAT_MAX)
	{
		return;
	}

	bool isShowMsg = false;

	bool isSelf = m_systemSetting->IsSelf(inPrinterID);
	if (isSelf)
	{
		if (inItemID != m_currPreOverCoat.PRE_COAT_SELF)
		{
			isShowMsg = true;
		}
		m_preOverCoat.PRE_COAT_SELF = inItemID;
	}
	else
	{
		if (inItemID != m_currPreOverCoat.PRE_COAT_PART)
		{
			isShowMsg = true;
		}
		m_preOverCoat.PRE_COAT_PART = inItemID;
	}

	if (m_systemSetting->IsStartedFromJobListScreen() && isShowMsg)
	{
		// create message
		char msg[256] = {0};
		_snprintf(msg, 255, "%d\n%s", ID_MESSAGE_ERROR + IDM_CHANGE_PRECOAT_JOB_LIST_SCREEN, LoadResourceString(IDM_CHANGE_PRECOAT_JOB_LIST_SCREEN, RESOURCE_MSG_STR));
		ShowMessageBox(msg, MBST_ICONWARNING | MBST_OK, NULL);
	}
}

/**
 *	Get a selection precoat
 *	@param[in]	inPrinterID : a printer ID(1-2)
 *	@param[out]	outItemID : a pointer to get an item ID(0-N) of a selection precoat
 *	@retval	true : getting succeed
 *	@retval	false : getting failed
 *	@note
 *	
 *		
**/
bool CDataPreOverCoat::GetSelectPrecoat(long inPrinterID, long* outItemID)
{
	if (inPrinterID < 1 || inPrinterID > 2 || !outItemID)
	{
		return false;
	}

	bool isSelf = m_systemSetting->IsSelf(inPrinterID);
	if (isSelf)
	{
		*outItemID = m_preOverCoat.PRE_COAT_SELF;
	}
	else
	{
		*outItemID = m_preOverCoat.PRE_COAT_PART;
	}

	return true;
}

/**
 *	Get a range of overcoat settings
 *	@param[out]	outMin : a pointer to get the minimum of range
 *	@param[out]	outMax : a pointer to get the maximum of range
 *	@note
 *	
 *		
**/
void CDataPreOverCoat::GetOvercoatRange(long* outMin, long* outMax)
{
	if (!outMin || !outMax)
	{
		return;
	}

	*outMin = DEF_OVERCOAT_MIN;
	*outMax = DEF_OVERCOAT_MAX;
}

/**
 *	Set a selection overcoat
 *	@param[in]	inPrinterID : a printer ID(1-2)
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note
 *	
 *		
**/
void CDataPreOverCoat::SetSelectOvercoat(long inPrinterID, long inItemID)
{
	if (inPrinterID < 1 || inPrinterID > 2
		|| inItemID < DEF_OVERCOAT_MIN || inItemID > DEF_OVERCOAT_MAX)
	{
		return;
	}

	bool isSelf = m_systemSetting->IsSelf(inPrinterID);
	if (isSelf)
	{
		m_preOverCoat.OVR_COAT_SELF = inItemID;
	}
	else
	{
		m_preOverCoat.OVR_COAT_PART = inItemID;
	}
}

/**
 *	Get a selection overcoat
 *	@param[in]	inPrinterID : a printer ID(1-2)
 *	@param[out]	outItemID : a pointer to get an item ID(0-N) of a selection overcoat
 *	@retval	true : getting succeed
 *	@retval	false : getting failed
 *	@note
 *	
 *		
**/
bool CDataPreOverCoat::GetSelectOvercoat(long inPrinterID, long* outItemID)
{
	if (inPrinterID < 1 || inPrinterID > 2 || !outItemID)
	{
		return false;
	}

	bool isSelf = m_systemSetting->IsSelf(inPrinterID);
	if (isSelf)
	{
		*outItemID = m_preOverCoat.OVR_COAT_SELF;
	}
	else
	{
		*outItemID = m_preOverCoat.OVR_COAT_PART;
	}

	return true;
}

/**
 *	Update the current setting value of Precoat and overcoat
 *	@param[in]	inKey : key name
 *	@note
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
void CDataPreOverCoat::UpdateCurrentPreOverCoat(const char* inKey)
{
	if (!inKey || strlen(inKey) == 0)
	{
		return;
	}

	m_printConditionName = inKey;

	// The Print Condition GUI is started from the Job List Screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		m_JM_Callbacks->JM_GetPreOverCoat(inKey, m_currPreOverCoat);
	}
	else
	{
		m_PaperDB_Callbacks->PDB_GetPreOverCoat(inKey, m_currPreOverCoat);
	}

	m_preOverCoat = m_currPreOverCoat;
}

/**
 *	Is used to initialize data when the Print Condition GUI start.
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note Notes
 *	
 *		
**/
bool CDataPreOverCoat::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Get the current pre-over coat of Print Condition Name
	UpdateCurrentPreOverCoat(inPrintConditionOrSectionID);

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
bool CDataPreOverCoat::ExitScreen()
{
	return true;
}