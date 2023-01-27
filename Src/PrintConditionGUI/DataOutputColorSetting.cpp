/*! \file DataOutputColorSetting.cpp
 *  \brief DataOutputColorSetting.cpp‚Ìˆ—
 *  \author SCREEN
 *  \date 2017/11/20 SCREEN V‹Kì¬
*/


// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataOutputColorSetting.h"
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
CDataOutputColorSetting::CDataOutputColorSetting()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataOutputColorSetting::~CDataOutputColorSetting()
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
bool CDataOutputColorSetting::Initialize(CDataSystemSetting* inSystemSetting)
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
bool CDataOutputColorSetting::Finalize()
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
void CDataOutputColorSetting::ResetMember()
{
	m_systemSetting = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
	m_printConditionName = "";
	m_preOutputColor = DEF_OUTPUT_COLOR_MODE_CMYK;
	m_curOutputColor = DEF_OUTPUT_COLOR_MODE_CMYK;
}

/**
 *	Check the user change the output color
 *	@retval	true : change
 *	@retval	false : not change
 *	@note
 *	
 *		
**/
bool CDataOutputColorSetting::IsChanged()
{
	if (m_preOutputColor != m_curOutputColor)
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
bool CDataOutputColorSetting::Save()
{
	bool isSucceed = true;
	if (IsChanged())
	{
		if (m_systemSetting->IsStartedFromJobListScreen())
		{
//			isSucceed = m_JM_Callbacks->JM_SetOutputColorMode(m_printConditionName.c_str(), m_preOutputColor); 
		}
		else
		{
			isSucceed = m_PaperDB_Callbacks->PDB_SetOutputColorMode(m_printConditionName.c_str(), m_preOutputColor);
		}

		if(isSucceed) {
			UpdateCurrentOutputColor(m_printConditionName.c_str());
		}
	}
	return isSucceed;
}

/**
 *	Set a selection output color
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note
 *	
 *		
**/
void CDataOutputColorSetting::SetSelectOutputColor(long inItemID)
{
	m_preOutputColor = (DEF_OUTPUT_COLOR_MODE)inItemID;
}

/**
 *	Get a selection output color
 *	@param[out]	outItemID : a pointer to get an item ID(0-N) of a selection output color
 *	@retval	true : getting succeed
 *	@retval	false : getting failed
 *	@note
 *	
 *		
**/
bool CDataOutputColorSetting::GetSelectOutputColor(long* outItemID)
{
	if (!outItemID)
	{
		return false;
	}

	*outItemID = m_preOutputColor;

	return true;
}

/**
 *	Update the current setting value of output color
 *	@param[in]	inKey : key name
 *	@note
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
void CDataOutputColorSetting::UpdateCurrentOutputColor(const char* inKey)
{
	if (!inKey || strlen(inKey) == 0)
	{
		return;
	}

	m_printConditionName = inKey;
	m_curOutputColor = DEF_OUTPUT_COLOR_MODE_CMYK;

	// The Print Condition GUI is started from the Job List Screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		m_JM_Callbacks->JM_GetOutputColorMode(inKey, m_curOutputColor);
	}
	else
	{
		m_PaperDB_Callbacks->PDB_GetOutputColorMode(inKey, m_curOutputColor);
	}

	m_preOutputColor = m_curOutputColor;
}

/**
 *	Is used to initialize data when the Print Condition GUI start.
 *	@retval	true : succeed
 *	@retval	false : failed
 *	@note Notes
 *	
 *		
**/
bool CDataOutputColorSetting::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Get the current output color of Print Condition Name
	UpdateCurrentOutputColor(inPrintConditionOrSectionID);

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
bool CDataOutputColorSetting::ExitScreen()
{
	return true;
}
