/*! \file DataDryerSetting.cpp
 *  \brief data class: dryer setting
 *  \author GCS Y Tran
 *  \date 2013/12/12 GCS Y Tran created
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataDryerSetting.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "PrintConditionGUIDef.h"
#include "CommonUiMsg_OP.h"
#include "DataIF.h"

// ==========================================================================

/**
 * constructor
 * @note General process
 * 
 *		Initialize the member variables
**/
CDataDryerSetting::CDataDryerSetting()
{
	ResetMember();
}

/**
 * destructor
 * @note General process
 * 
 *		Remove objects which created with dynamic.
**/
CDataDryerSetting::~CDataDryerSetting()
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
bool CDataDryerSetting::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
{
	if (!inSystemSetting || !inDataIF || FALSE == m_iniPrintConditionGUI.Initialize(TRUE)) {
		return false;
	}

	// Store system setting handle and callback handles
	m_systemSetting = inSystemSetting;
	m_data = inDataIF;
	m_JM_Callbacks = m_systemSetting->GetJobManager_Callbacks();
	m_PaperDB_Callbacks = m_systemSetting->GetPaperDB_Callbacks();

	//Don't initialize dryer setting data, Print setting pull-down will notify to intialize
	return true;
}

/**
 *	Remove member variables in the class
 *	@retval	TRUE: success
 *	@retval	FALSE: failure
**/
bool CDataDryerSetting::Finalize()
{
	m_iniPrintConditionGUI.Finalize();
	return true;
}

/**
 *	Reset member variables in the class
 *	@note General process
 *	
 *		Reset the member variables in the class
 *		Remove objects which created with dynamic.
**/
void CDataDryerSetting::ResetMember()
{
	m_systemSetting = NULL;
	m_data = NULL;
	m_JM_Callbacks = NULL;
	m_PaperDB_Callbacks = NULL;
	m_dryerSettingList.clear();
	m_displayedDryerSettingList.clear();
	memset(&m_currDryerSetting, 0x00, sizeof(m_currDryerSetting));
	m_selectDryerSettingID = -1;
	m_isNotSetCurrentDryerSetting = false;
	memset(m_sectionID_printCondtionName, 0x00, _countof(m_sectionID_printCondtionName));

	ResetMember_AddDlg();
	ResetMember_DelDlg();
}

/**
 *	Reset member variables of Add dryer setting dialog
 *	@note General process
**/
void CDataDryerSetting::ResetMember_AddDlg()
{
	memset(&m_addDlg_dryerSetting, 0x00, sizeof(m_addDlg_dryerSetting));
	memset(&m_addDlg_initDryerSetting, 0x00, sizeof(m_addDlg_initDryerSetting));
}

/**
 *	Reset member variables of Delete dryer setting dialog
 *	@note General process
**/
void CDataDryerSetting::ResetMember_DelDlg()
{
	m_delDlg_listDryerSetting.clear();
	m_delDlg_hwnd = NULL;
}

/**
 *	Obtain the current dryer setting in case the user change the print condition name
 *	@retval	true: getting suceed
 *	@retval	false: getting fail
 *	@note General process
 *	
 *		In case the Print Condition GUI is started from:
 *			Job List Screen
 *				- call JM_GetDryTempPrint
 *			System Setting Screen
 *				- call PDB_GetDryTempPrint
 *
*	@note Note
 *		Call CDataDryerSetting::SetSelectPrintDryerSetting after that(m_currDryerSetting) to select current dryer setting
**/
bool CDataDryerSetting::ObtainCurrentDryerSetting(const char* inKeyName)
{
	//Reserve Section ID or Print condition name
	_snprintf_s(m_sectionID_printCondtionName, _TRUNCATE, "%s", inKeyName);
	//Reset m_currDryerSetting
	memset(&m_currDryerSetting, 0x00, sizeof(m_currDryerSetting));

	// The Print condition GUI is started from Job List screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		return m_JM_Callbacks->JM_GetDryTempPrint(inKeyName, m_currDryerSetting);
	}
	else
	{
		return m_PaperDB_Callbacks->PDB_GetDryTempPrint(inKeyName, m_currDryerSetting);
	}

	return false;
}

/**
 *	Set select for a dryer setting
 *	@param[in]	inDryerSetting : dryer setting to be set selected
 *	@note Note
 *	
**/
void CDataDryerSetting::SetSelectPrintDryerSetting(ST_PDB_DRY_TEMP_PRINT inDryerSetting)
{
	UINT numOfDryerSetting = m_dryerSettingList.size();

	if(numOfDryerSetting <= 0){
		return;
	}

	//Reset selected index
	m_selectDryerSettingID = -1;

	//Loop to find inDryerSetting
	UINT index = 0;
	for (; index < numOfDryerSetting; ++index)
	{
		if (IsSameDryerSetting(inDryerSetting, m_dryerSettingList.at(index)))
		{
			m_selectDryerSettingID = index;
			break;
		}
	}

	// The dryer setting list doesn't contain inDryerSetting (it has just deleted) --> set selected to current dryer setting
	if (index == numOfDryerSetting){
		if(IsSameDryerSetting(inDryerSetting, m_currDryerSetting)) {
			//Only has one item
			if(1 == m_displayedDryerSettingList.size()) {
				m_selectDryerSettingID = 0;
			}
			// > 1 item
			else {
				string emptyDryer = "";
				m_displayedDryerSettingList.push_back(emptyDryer);
				m_selectDryerSettingID = m_displayedDryerSettingList.size() - 1;
				m_isNotSetCurrentDryerSetting = true;
			}
		}
		//---the following comparison is to prevent infinite loop in case the previous code can not find m_currDryerSetting
		else {
			// Update current selection id with the current dryer setting
			SetSelectPrintDryerSetting(m_currDryerSetting);
		}
	}
//	SendMessage(GetGenSetupDialog(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
}

/**
 *	Initialize Dryer setting list
 *	@retval	true: getting suceed
 *	@retval	false: getting fail
**/
bool CDataDryerSetting::InitDryerSettingList()
{
	//Clear dryer setting list
	m_dryerSettingList.clear();
	m_displayedDryerSettingList.clear();
	m_isNotSetCurrentDryerSetting = false;

	int nItemID = 0;
	ST_PDB_DRY_TEMP_PRINT dryerSetting = {0};

	if (false == m_PaperDB_Callbacks->PDB_EnterCommonPaperDBCS()){
		return false;
	}

	// Call PaperDB to get a system dryer setting
	while (true == m_PaperDB_Callbacks->PDB_GetSystemDryTempPrint(nItemID++, dryerSetting))
	{
		// Store system dryer setting to member variable
		m_dryerSettingList.push_back(dryerSetting);
		memset(&dryerSetting, 0x00, sizeof(dryerSetting));
	}

	m_PaperDB_Callbacks->PDB_LeaveCommonPaperDBCS();

	return true;
}

/**
 *	Compare if two dryer settings are same to each other
 *	@param[in]	inSetting1 : dryer setting 1
 *	@param[in]	inSetting2 : dryer setting 2
 *	@retval	true: two settings are equal
 *	@retval	false: two settings are different
 *	@note General process
 *	
 *		Compare each member of struct ST_PDB_DRY_TEMP_PRINT
 *			If exist a member has different value from setting 1 to 2
 *				return false
 *			Else
 *				return true
**/
bool CDataDryerSetting::IsSameDryerSetting(const ST_PDB_DRY_TEMP_PRINT &inSetting1, const ST_PDB_DRY_TEMP_PRINT &inSetting2)
{
	if(inSetting1.FAN_TEMP_PART != inSetting2.FAN_TEMP_PART
		|| inSetting1.FAN_TEMP_SELF != inSetting2.FAN_TEMP_SELF
		|| inSetting1.HEAT_TEMP_PART != inSetting2.HEAT_TEMP_PART
		|| inSetting1.HEAT_TEMP_SELF != inSetting2.HEAT_TEMP_SELF
		|| inSetting1.PRE_TEMP_PART != inSetting2.PRE_TEMP_PART
		|| inSetting1.PRE_TEMP_SELF != inSetting2.PRE_TEMP_SELF
		|| inSetting1.NIR_POWER_PART != inSetting2.NIR_POWER_PART
		|| inSetting1.NIR_POWER_SELF != inSetting2.NIR_POWER_SELF
		|| inSetting1.SUB_HEAT_TEMP_SELF != inSetting2.SUB_HEAT_TEMP_SELF
		|| inSetting1.SUB_HEAT_TEMP_PART != inSetting2.SUB_HEAT_TEMP_PART){
			return false;
	}

	return true;
}

/**
 *	Create displayed name of dryer setting list
 *	@note General process
 *	
 *		Each items shall include heater temperatures 
 *			JA)  ローラー(60,60)  温風(60,60)  プレコート(60,60)
 *			EN) Roller(60,60) Fan(60,60) Precoat(60,60)
 *		Temperatures are shown inside parenthesis：
 *			Roller / Fan (Printer2, Printer1)
 *			Precoat (Back, Front)
**/
void CDataDryerSetting::InitDisplayedDryerSettingName()
{
	const char* strRoller = LoadResourceString(IDS_ROLLER, RESOURCE_STR);
	const char* strFan = LoadResourceString(IDS_FAN, RESOURCE_STR);
	const char* strPrecoat = LoadResourceString(IDS_PRECOAT, RESOURCE_STR);
	const char* strNir = LoadResourceString(IDS_N_IR, RESOURCE_STR);
	const char* strHeatRanger = LoadResourceString(IDS_SUB_HEAT_ROLLER, RESOURCE_STR);
	char dispName[256] = {0};

	UINT numOfDryerSetting = m_dryerSettingList.size();
	for (UINT index = 0; index < numOfDryerSetting; ++index){
		long pDryTemp[10] = {
			//pDryTemp[0,1,2,3,4] --> self
			m_dryerSettingList.at(index).HEAT_TEMP_SELF,
			m_dryerSettingList.at(index).FAN_TEMP_SELF,
			m_dryerSettingList.at(index).PRE_TEMP_SELF,
			m_dryerSettingList.at(index).NIR_POWER_SELF,
			m_dryerSettingList.at(index).SUB_HEAT_TEMP_SELF,
			//pDryTemp[5,6,7,8,9] --> partner
			m_dryerSettingList.at(index).HEAT_TEMP_PART,
			m_dryerSettingList.at(index).FAN_TEMP_PART,
			m_dryerSettingList.at(index).PRE_TEMP_PART,
			m_dryerSettingList.at(index).NIR_POWER_PART,
			m_dryerSettingList.at(index).SUB_HEAT_TEMP_PART
		};

		char strFormat[256] = {0};
		char strRollerDisplay[256] = {0};
		char strFanDisplay[256] = {0};
		char strPrecoatDisplay[256] = {0};
		char strNirDisplay[256] = {0};
		char strHeatRangerDisplay[256] = {0};
		bool isDED = m_systemSetting->IsDED();

		//**************Roller string**************
		bool bExistHeatRollerPrinter1 = m_systemSetting->IsExistHeatRoller(PRINTER1 + 1);
		bool bExistHeatRollerPrinter2 = m_systemSetting->IsExistHeatRoller(PRINTER2 + 1);

		if(isDED && bExistHeatRollerPrinter1 && bExistHeatRollerPrinter2) {

			_snprintf_s(strFormat, _TRUNCATE, "%s", "%s(%ld,%ld)");

			//if printer 2 is self PC
			if(m_systemSetting->IsSelf(PRINTER2 + 1)){
				_snprintf_s(strRollerDisplay, _TRUNCATE,
					strFormat, strRoller, pDryTemp[0], pDryTemp[5]);
			}
			//if printer 1 is self PC
			else{
				_snprintf_s(strRollerDisplay, _TRUNCATE,
					strFormat, strRoller, pDryTemp[5], pDryTemp[0]);
			}
		} else if (!isDED && bExistHeatRollerPrinter1) {
			_snprintf_s(strFormat, _TRUNCATE, "%s", "%s(%ld)");
			_snprintf_s(strRollerDisplay, _TRUNCATE, strFormat, strRoller, pDryTemp[0]);
		}

		//****************Fan string**************
		bool bExistDryerPrinter1 = m_systemSetting->IsExistDryerUnit(PRINTER1 + 1);
		bool bExistDryerPrinter2 = m_systemSetting->IsExistDryerUnit(PRINTER2 + 1);

		if(isDED) {
			//String format
			if(bExistDryerPrinter2) {
				// strFormat = %s(%ld,
				_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(%ld,");
			}else {
				// strFormat = %s(--,
				_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(--,");
			}

			if(bExistDryerPrinter1) {
				// strFormat = %s(%ld, %ld) or %s(--, %ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s%s", strFormat, "%ld)");
			}else {
				if(bExistDryerPrinter2) {
					// strFormat = %s(%ld, --)
					_snprintf_s(strFormat, _TRUNCATE, "%s%s", strFormat, "--)");
				}else {
					// strFormat = ""
					_snprintf_s(strFormat, _TRUNCATE, "%s", "");
				}
			}

			//if printer 2 is self PC
			if(m_systemSetting->IsSelf(PRINTER2 + 1)){
				if(bExistDryerPrinter1 && bExistDryerPrinter2) {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[1], pDryTemp[6]);
				}else if(bExistDryerPrinter2 && !bExistDryerPrinter1) {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[1]);
				}else {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[6]);
				}
			}
			//if printer 1 is self PC
			else{
				if(bExistDryerPrinter1 && bExistDryerPrinter2) {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[6], pDryTemp[1]);
				}else if(bExistDryerPrinter2 && !bExistDryerPrinter1) {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[6]);
				}else {
					_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[1]);
				}
			}
		} else {
			//String format
			if(bExistDryerPrinter1) {
				// strFormat = %s(%ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s", "%s(%ld)");
				_snprintf_s(strFanDisplay, _TRUNCATE, strFormat, strFan, pDryTemp[1]);
			} else {
				// strFormat = ""
				_snprintf_s(strFormat, _TRUNCATE, "%s", "");
			}
		}

		//****************Sub Heat Roller string**************
		bool existHeaRanger = m_systemSetting->IsExistHeatRangerUnit();

		if(isDED) {
			//String format
			if(existHeaRanger) {
				// strFormat = %s(%ld, %ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(%ld, %ld)");
			}
			else {
				// strFormat = ""
				_snprintf_s(strFormat, _TRUNCATE, "%s", "");
			}

			//if printer 2 is self PC
			if(m_systemSetting->IsSelf(PRINTER2 + 1)) {
				if(existHeaRanger) {
					_snprintf_s(strHeatRangerDisplay, _TRUNCATE, strFormat, strHeatRanger, pDryTemp[4], pDryTemp[9]);
				}
			}
			//if printer 1 is self PC
			else {
				if(existHeaRanger) {
					_snprintf_s(strHeatRangerDisplay, _TRUNCATE, strFormat, strHeatRanger, pDryTemp[9], pDryTemp[4]);
				}
			}
		} 
		else {
			//String format
			if(existHeaRanger) {
				// strFormat = %s(%ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s", "%s(%ld)");
				_snprintf_s(strHeatRangerDisplay, _TRUNCATE, strFormat, strHeatRanger, pDryTemp[4]);
			} 
			else {
				// strFormat = ""
				_snprintf_s(strFormat, _TRUNCATE, "%s", "");
			}
		}

		// 2018.01.25
		// Precoater is not supported because it's non-product functiona and nir unit is supported.
#if 0

		//********************Precoat string**************
		if(m_systemSetting->IsExistPrecoater()) {
//			_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(%ld,%ld)");
			_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(%ld)");	// dryer2 is removed from the spec - 2014.9.11

			//if printer 2 is self PC
			if(m_systemSetting->IsSelf(PRINTER2 + 1)){
//				_snprintf_s(strPrecoatDisplay, _TRUNCATE, strFormat, strPrecoat, pDryTemp[2], pDryTemp[6]);
				_snprintf_s(strPrecoatDisplay, _TRUNCATE, strFormat, strPrecoat, pDryTemp[6]);
			}
			//if printer 1 is self PC
			else {
//				_snprintf_s(strPrecoatDisplay, _TRUNCATE, strFormat, strPrecoat, pDryTemp[6], pDryTemp[2]);
				_snprintf_s(strPrecoatDisplay, _TRUNCATE, strFormat, strPrecoat, pDryTemp[2]);
			}
		}else {
			_snprintf_s(strPrecoatDisplay, _TRUNCATE, "%s", "");
		}
		_snprintf_s(dispName, _TRUNCATE, "%s%s%s", strRollerDisplay, strFanDisplay, strPrecoatDisplay);
#endif

		//********************NIR string**************
		bool bExistNirDryerPrinter1 = m_systemSetting->IsExistNirDryerUnit(PRINTER1 + 1);
		bool bExistNirDryerPrinter2 = m_systemSetting->IsExistNirDryerUnit(PRINTER2 + 1);

		if (isDED) {
			if(bExistNirDryerPrinter2) {
				// strFormat = %s(%ld,
				_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(%ld,");
			}else {
				// strFormat = %s(--,
				_snprintf_s(strFormat, _TRUNCATE, "%s", " %s(--,");
			}

			if(bExistNirDryerPrinter1) {
				// strFormat = %s(%ld, %ld) or %s(--, %ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s%s", strFormat, "%ld)");
			}else {
				if(bExistNirDryerPrinter2) {
					// strFormat = %s(%ld, --)
					_snprintf_s(strFormat, _TRUNCATE, "%s%s", strFormat, "--)");
				}else {
					// strFormat = ""
					_snprintf_s(strFormat, _TRUNCATE, "%s", "");
				}
			}

			//if printer 2 is self PC
			if(m_systemSetting->IsSelf(PRINTER2 + 1)){
				if(bExistNirDryerPrinter1 && bExistNirDryerPrinter2) {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[3], pDryTemp[8]);
				}else if(bExistNirDryerPrinter2 && !bExistNirDryerPrinter1) {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[3]);
				}else {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[8]);
				}
			}
			//if printer 1 is self PC
			else{
				if(bExistNirDryerPrinter1 && bExistNirDryerPrinter2) {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[8], pDryTemp[3]);
				}else if(bExistNirDryerPrinter2 && !bExistNirDryerPrinter1) {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[8]);
				}else {
					_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[3]);
				}
			}
		} else {
			//String format
			if(bExistNirDryerPrinter1) {
				// strFormat = %s(%ld)
				_snprintf_s(strFormat, _TRUNCATE, "%s", "%s(%ld)");
				_snprintf_s(strNirDisplay, _TRUNCATE, strFormat, strNir, pDryTemp[3]);
			} else {
				// strFormat = ""
				_snprintf_s(strFormat, _TRUNCATE, "%s", "");
			}
		}

		_snprintf_s(dispName, _TRUNCATE, "%s%s%s%s", strRollerDisplay, strFanDisplay, strHeatRangerDisplay, strNirDisplay);

		m_displayedDryerSettingList.push_back(dispName);
		memset(dispName, 0x00, _countof(dispName));
	}
}

/**
 *	Get an item of dryer settings list for printing
 *	@param[in]	inItemID: an item ID(0-N)
 *	@param[out]	outDryer: a pointer to get a dryer settings
 *	@retval	true: getting succeed
 *	@retval	false: getting failed
*/
bool CDataDryerSetting::GetPrintDryerSettingList(long inItemID, const char** outDryer)
{
	if (inItemID < 0 || inItemID >= (long)m_displayedDryerSettingList.size() || !outDryer){
		return false;
	}

	*outDryer = m_displayedDryerSettingList.at(inItemID).c_str();

	return true;
}


/**
 *	Notify to initialize Add dryer settings dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note General process
 *	
 *		Default values above are used in order of following. 
 *			① Temperature value of the print settings being edited 
 *			② Default value which defined ini file. 
**/
bool CDataDryerSetting::InitAddDryerSettingDlg()
{
	//There's print settings being edited 
	if(m_selectDryerSettingID != -1 && m_dryerSettingList.size() > 0){
		//Temperature value of the print settings being edited 
		m_addDlg_dryerSetting = m_dryerSettingList.at(m_selectDryerSettingID);
		//---reserve initial value
		m_addDlg_initDryerSetting = m_dryerSettingList.at(m_selectDryerSettingID);

	}else{
		//Default value which defined ini file.
		m_addDlg_dryerSetting.HEAT_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultHeatRollerTemp();
		m_addDlg_dryerSetting.HEAT_TEMP_PART = m_iniPrintConditionGUI.GetDefaultHeatRollerTemp();
		m_addDlg_dryerSetting.FAN_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultDryerFanTemp();
		m_addDlg_dryerSetting.FAN_TEMP_PART = m_iniPrintConditionGUI.GetDefaultDryerFanTemp();
		m_addDlg_dryerSetting.PRE_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultPrecoaterTemp();
		m_addDlg_dryerSetting.PRE_TEMP_PART = m_iniPrintConditionGUI.GetDefaultPrecoaterTemp();
		m_addDlg_dryerSetting.NIR_POWER_SELF = m_iniPrintConditionGUI.GetDefaultNirPower();
		m_addDlg_dryerSetting.NIR_POWER_PART = m_iniPrintConditionGUI.GetDefaultNirPower();
		m_addDlg_dryerSetting.SUB_HEAT_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultHeatRangerTemp();
		m_addDlg_dryerSetting.SUB_HEAT_TEMP_PART = m_iniPrintConditionGUI.GetDefaultHeatRangerTemp();

		//---reserve initial value
		m_addDlg_initDryerSetting.HEAT_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultHeatRollerTemp();
		m_addDlg_initDryerSetting.HEAT_TEMP_PART = m_iniPrintConditionGUI.GetDefaultHeatRollerTemp();
		m_addDlg_initDryerSetting.FAN_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultDryerFanTemp();
		m_addDlg_initDryerSetting.FAN_TEMP_PART = m_iniPrintConditionGUI.GetDefaultDryerFanTemp();
		m_addDlg_initDryerSetting.PRE_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultPrecoaterTemp();
		m_addDlg_initDryerSetting.PRE_TEMP_PART = m_iniPrintConditionGUI.GetDefaultPrecoaterTemp();
		m_addDlg_initDryerSetting.NIR_POWER_SELF = m_iniPrintConditionGUI.GetDefaultNirPower();
		m_addDlg_initDryerSetting.NIR_POWER_PART = m_iniPrintConditionGUI.GetDefaultNirPower();
		m_addDlg_initDryerSetting.SUB_HEAT_TEMP_SELF = m_iniPrintConditionGUI.GetDefaultHeatRangerTemp();
		m_addDlg_initDryerSetting.SUB_HEAT_TEMP_PART = m_iniPrintConditionGUI.GetDefaultHeatRangerTemp();
	}

	return true;
}


/**
 *	Notify to initialize Delete dryer settings dialog
 *	@retval	true : success
 *	@retval	false : failure
 *	@note Notes
 *	
 *		
**/
bool CDataDryerSetting::InitDeleteDryerSettingDlg()
{
	InitDryerSettingList_DelDlg();
	return true;
}

/**
 *	Set a selection dryer settings
 *	@param[in]	inItemID : an item ID (0-N)
 *	@note Notes
 *	
 *		
**/
void CDataDryerSetting::SetSelectPrintDryerSetting(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_dryerSettingList.size()){
		return;
	}

	m_selectDryerSettingID = inItemID;

	//Remove blank item if has
	if(m_isNotSetCurrentDryerSetting && (inItemID != m_displayedDryerSettingList.size() - 1)) {
		m_displayedDryerSettingList.erase(m_displayedDryerSettingList.end() - 1);
//		SendMessage(GetGenSetupDialog(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		SendMessage(m_systemSetting->GetPostMessageWindow(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		m_isNotSetCurrentDryerSetting = false;
	}
}


/**
 *	Get a selection dryer settings
 *	@param[out]	outItemID: a pointer to get an item ID(0-N) of a selection dryer settings
 *	@retval	true: getting succeed
 *	@retval	false: getting failed
 *	@note Notes
 *	
**/
bool CDataDryerSetting::GetSelectPrintDryerSetting(long* outItemID)
{
	if (!outItemID){
		return false;
	}

	*outItemID = m_selectDryerSettingID;

	return true;
}

/**
 *	Set a temperature of heat roller for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@param[in]	inTemperature: a temperature
 *	@note Notes
 *	
**/
void CDataDryerSetting::SetHeatRollerTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	if(inPrinterID < 1 || inPrinterID > 2){
		return;
	}

	bool	isSelf = m_systemSetting->IsSelf(inPrinterID);

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// 100[C]
	long	maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTemp();
	
	// 140[C]
	if(pData->IsSupport140DegC(isSelf)){
		if(pData->IsSupport120DegC(isSelf) || pData->IsSupport120DegC_V3(isSelf))	// both 140C and 120C options are ON // #2288
			maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor120DegC();
		else	// 140C option only is ON
			maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor140DegC();
	}
	// 120[C]
	else if(pData->IsSupport120DegC(isSelf) || pData->IsSupport120DegC_V3(isSelf)){	// 120C option only is ON
		maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor120DegC();
	}

	//Correct inTemperature
	long temp = atol(inTemperature);
	temp = min(maxTemp, max(m_iniPrintConditionGUI.GetMinHeatRollerTemp(), temp));

	if(isSelf){
		m_addDlg_dryerSetting.HEAT_TEMP_SELF = temp;
	}else{
		m_addDlg_dryerSetting.HEAT_TEMP_PART = temp;
	}
}


/**
 *	Set a temperature of dryer fan for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@param[in]	inTemperature: a temperature
 *	@note Notes
 *	
**/
void CDataDryerSetting::SetDryerFanTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	if(inPrinterID < 1 || inPrinterID > 2){
		return;
	}

	bool	isSelf = m_systemSetting->IsSelf(inPrinterID);

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// 100[C]
	long	maxTemp = m_iniPrintConditionGUI.GetMaxDryerFanTemp();
	
	// 140[C]
	if(pData->IsSupport140DegC(isSelf)){
		maxTemp = m_iniPrintConditionGUI.GetMaxDryerFanTempFor140DegC();
	}

	//Correct inTemperature
	long temp = atol(inTemperature);
	temp = min(maxTemp, max(m_iniPrintConditionGUI.GetMinDryerFanTemp(), temp));

	if(m_systemSetting->IsSelf(inPrinterID)){
		m_addDlg_dryerSetting.FAN_TEMP_SELF = temp;
	}else{
		m_addDlg_dryerSetting.FAN_TEMP_PART = temp;
	}
}

/**
 *	Get a temperature of heat roller for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retval	a temperature
 *	@note Notes
 *	
**/
long CDataDryerSetting::GetHeatRollerTemperature_AddDlg(long inPrinterID)
{
	bool	isSelf = m_systemSetting->IsSelf(inPrinterID);

	long retval = isSelf ? m_addDlg_dryerSetting.HEAT_TEMP_SELF : m_addDlg_dryerSetting.HEAT_TEMP_PART;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// 100[C]
	long	maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTemp();
	
	// 140[C]
	if(pData->IsSupport140DegC(isSelf)){
		if(pData->IsSupport120DegC(isSelf) || pData->IsSupport120DegC_V3(isSelf))	// both 140C and 120C options are ON // #2288
			maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor120DegC();
		else	// 140C option only is ON
			maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor140DegC();
	}
	// 120[C]
	else if(pData->IsSupport120DegC(isSelf) || pData->IsSupport120DegC_V3(isSelf)){	// 120C option only is ON
		maxTemp = m_iniPrintConditionGUI.GetMaxHeatRollerTempFor120DegC();
	}

	//Correct heat roller temperature
	retval = min(maxTemp, max(m_iniPrintConditionGUI.GetMinHeatRollerTemp(), retval));

	return retval;
}

/**
 *	Get a temperature of dryer fun for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retval	a temperature
 *	@note Notes
 *	
**/
long CDataDryerSetting::GetDryerFanTemperature_AddDlg(long inPrinterID)
{
	bool	isSelf = m_systemSetting->IsSelf(inPrinterID);

	long retval = m_systemSetting->IsSelf(inPrinterID) ? m_addDlg_dryerSetting.FAN_TEMP_SELF : m_addDlg_dryerSetting.FAN_TEMP_PART;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// 100[C]
	long	maxTemp = m_iniPrintConditionGUI.GetMaxDryerFanTemp();
	
	// 140[C]
	if(pData->IsSupport140DegC(isSelf)){
		maxTemp = m_iniPrintConditionGUI.GetMaxDryerFanTempFor140DegC();
	}

	//Correct dryer fan temperature
	retval = min(maxTemp, max(m_iniPrintConditionGUI.GetMinDryerFanTemp(), retval));

	return retval;
}

/**
 *	Set a temperature of precoat for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retvala temperature
 *	@note Notes
 *	
**/
void CDataDryerSetting::SetPrecoatTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	if(inPrinterID < 1 || inPrinterID > 2){
		return;
	}

	//Correct inTemperature
	long temp = atol(inTemperature);
	temp = min(m_iniPrintConditionGUI.GetMaxPrecoaterTemp(), max(m_iniPrintConditionGUI.GetMinPrecoaterTemp(), temp));

	if(m_systemSetting->IsSelf(inPrinterID)){
		m_addDlg_dryerSetting.PRE_TEMP_SELF = temp;
	}else{
		m_addDlg_dryerSetting.PRE_TEMP_PART = temp;
	}
}

/**
 *	Get a temperature of precoat for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retval	a temperature
 *	@note Notes
 *	
**/
long CDataDryerSetting::GetPrecoatTemperature_AddDlg(long inPrinterID)
{
	long retval = m_systemSetting->IsSelf(inPrinterID) ? m_addDlg_dryerSetting.PRE_TEMP_SELF : m_addDlg_dryerSetting.PRE_TEMP_PART;

	//Correct precoater temperature
	retval = min(m_iniPrintConditionGUI.GetMaxPrecoaterTemp(), max(m_iniPrintConditionGUI.GetMinPrecoaterTemp(), retval));

	return retval;
}

/**
 *	Check if dryer settings saved or not
 *	@retval true: it saved
 *	@retval false: it does not save
**/
bool CDataDryerSetting::IsSavedDryerSetting_AddDlg()
{
	return IsSameDryerSetting(m_addDlg_dryerSetting, m_addDlg_initDryerSetting);
}

/**
 *	Get a value of nir power for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retval	a temperature
 *	@note Notes
 *	
**/
long CDataDryerSetting::GetNirPower_AddDlg(long inPrinterID)
{
	long retval = m_systemSetting->IsSelf(inPrinterID) ? m_addDlg_dryerSetting.NIR_POWER_SELF : m_addDlg_dryerSetting.NIR_POWER_PART;

	//Correct nir power
	if (0 != retval) {
		retval = min(m_iniPrintConditionGUI.GetMaxNirPower(), max(m_iniPrintConditionGUI.GetMinNirPower(), retval));
	}

	return retval;
}

/**
 *	Set a value of nir power for a printer at Add dryer settings dialog
 *	@param[in]	inPrinterID: a printer ID(1-2)
 *	@retval: a nir power
 *	@note Notes
 *	
**/
void CDataDryerSetting::SetNirPower_AddDlg(long inPrinterID, char* inNirPower)
{
	if(inPrinterID < 1 || inPrinterID > 2){
		return;
	}

	//!< ヒーターレス機の場合は、最小値または最大値にクロップする。
	bool bExistHeatRoller = false;
	bExistHeatRoller = (m_systemSetting->IsExistHeatRoller(PRINTER1 + 1)? true:false);
	if(m_systemSetting->IsDED()) {
		bExistHeatRoller = (m_systemSetting->IsExistHeatRoller(PRINTER2 + 1)? true:bExistHeatRoller);
	}

	//Correct inTemperature
	long temp = atol(inNirPower);
	if (((0 != temp) && bExistHeatRoller) || (!bExistHeatRoller)) {
		temp = min(m_iniPrintConditionGUI.GetMaxNirPower(), max(m_iniPrintConditionGUI.GetMinNirPower(), temp));
	}

	if(m_systemSetting->IsSelf(inPrinterID)){
		m_addDlg_dryerSetting.NIR_POWER_SELF = temp;
	}else{
		m_addDlg_dryerSetting.NIR_POWER_PART = temp;
	}
}

/**
 *	Save the settings of Add dryer settings dialog
 *	@retval	-1: failed
 *	@retval	0: success (same setting is not added)
 *	@retval	1: success (new setting is added)
**/
long CDataDryerSetting::SaveAddDryerSettingDlg()
{
	long retval = -1;

	//If the same setting exists
	if(m_PaperDB_Callbacks->PDB_IsSystemDryTempPrint(m_addDlg_dryerSetting)){
		// don't add it, don't show overlap error message
		// select the same setting
		SetSelectPrintDryerSetting(m_addDlg_dryerSetting);
		retval = 0;
	}
	else if(m_PaperDB_Callbacks->PDB_AddSystemDryTempPrint(m_addDlg_dryerSetting)){
		//紙DBにコミットする。
		m_PaperDB_Callbacks->PDB_CommitPaperDB();
		// init dryer setting list
		InitDryerSettingList();
		InitDisplayedDryerSettingName();
		// select new added item
		SetSelectPrintDryerSetting(m_addDlg_dryerSetting);
		retval = 1;
	}

	//Reset member of Add dryer setting dialog
	ResetMember_AddDlg();

	return retval;
}

/**
 *	Cancel to save the settings of Add dryer settings dialog
 *	@retval true: succeed
 *	@retval false: failed
**/
bool CDataDryerSetting::CancelAddDryerSettingDlg()
{
	ResetMember_AddDlg();
	return true;
}

/**
 *	Set the dialog handle of Delete dryer temperature dialog
 *	@param[in] inDialog: the dialog handle
**/
void CDataDryerSetting::SetDeleteDryerSettingDlg(HWND inDialog)
{
	m_delDlg_hwnd = inDialog;
}


/**
 *	Get an item of dryer settings list to delete at Delete dryer temperature dialog
 *	@param[in] inItemID: an item ID(0-N)
 *	@param[out] outDryer: a pointer to get a dryer settings
 *	@retval true: getting succeed
 *	@retval false: getting failed
**/
bool CDataDryerSetting::GetDeleteDryerSettingList_DelDlg(long inItemID, const char** outDryer)
{
	if (inItemID < 0 || inItemID >= (long)m_delDlg_listDryerSetting.size() || !outDryer)
	{
		return false;
	}

	*outDryer = m_delDlg_listDryerSetting.at(inItemID).dispName.c_str();

	return true;
}


/**
 *	Register a drayer settings list to delete at Delete dryer temperature dialog
 *	@param[in] inItemID: an item ID(0-N)
**/
void CDataDryerSetting::SetDeleteDryerSetting_DelDlg(long inItemID)
{
	if (inItemID < 0 || inItemID >= (long)m_delDlg_listDryerSetting.size())
	{
		return;
	}

	m_delDlg_listDryerSetting.at(inItemID).isSelectedToDel = true;
}


/**
 *	Delete the registered items to delete at Delete dryer temperature dialog.
 *	@retval true: succeed
 *	@retval false: failed
*/
bool CDataDryerSetting::DeleteDryerSettings_DelDlg()
{
	UINT delDlg_numOfDryerSett = m_delDlg_listDryerSetting.size();
	vector<string> listError;
	bool isCommit = false;

	//start deleting process
	//---backup seleted dryer setting
	ST_PDB_DRY_TEMP_PRINT selDryerSett = {0};
	if (m_selectDryerSettingID >= 0
		&& m_selectDryerSettingID < (long)m_dryerSettingList.size())
	{
		selDryerSett = m_dryerSettingList.at(m_selectDryerSettingID);
	}

	for (UINT id = 0; id < delDlg_numOfDryerSett; ++id) {
		if (m_delDlg_listDryerSetting.at(id).isSelectedToDel) {

			// IPDSモードの場合、JobManagerが存在しないため該当の関数を実行回避
			bool isExistDryTempUsedInOtherJobs = false;
			if (m_systemSetting->IsIpdsMode() == FALSE) {
				if(m_JM_Callbacks->JM_IsDryTempUsedInOtherJobs(m_delDlg_listDryerSetting.at(id).setting)) {
					isExistDryTempUsedInOtherJobs = true;
				}
			}

			if (m_PaperDB_Callbacks->PDB_IsSystemDryTempPrintInPrintCondition(m_delDlg_listDryerSetting.at(id).setting)
				|| isExistDryTempUsedInOtherJobs == true)
			{
				listError.push_back(m_delDlg_listDryerSetting.at(id).dispName);
			}
			//---delete dryer setting from Paper DB
			else if(m_PaperDB_Callbacks->PDB_DelSystemDryTempPrint(m_delDlg_listDryerSetting.at(id).setting)){
				isCommit = true;
			}
		}
	}
	//紙DBが更新されている場合、コミットする。
	if (isCommit) {
		m_PaperDB_Callbacks->PDB_CommitPaperDB();
	}

	// Init dryer setting list
	InitDryerSettingList();
	InitDisplayedDryerSettingName();
	//update selection
	SetSelectPrintDryerSetting(selDryerSett);
	//re-init dryer setting list for Delete dialog
	InitDryerSettingList_DelDlg();
	//notify GUI to update list-box of Delete dialog
	::SendMessage(m_delDlg_hwnd, UWM_DELETE_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);

	UINT numOfPaperTypeError = listError.size();

	if (numOfPaperTypeError > 0)
	{
		char msg[1024] = {0};
		_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_ERR_DRYER_SETT_IS_USED_IN_OTHERS, LoadResourceString(IDM_ERR_DRYER_SETT_IS_USED_IN_OTHERS, RESOURCE_MSG_STR));

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

	return true;
}


/**
 *	Notify to exit Delete dryer temperature dialog
 *	@retval true: succeed
 *	@retval false: failed
*/
bool CDataDryerSetting::ExitDeleteDryerSettingDlg()
{
	ResetMember_DelDlg();
//	::PostMessage(GetGenSetupDialog(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_systemSetting->GetPostMessageWindow(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	return true;
}

/**
 *	Init Dryer setting list for Dlg Delete
 *	@note Notes
 *	
 *		
**/
void CDataDryerSetting::InitDryerSettingList_DelDlg()
{
	m_delDlg_listDryerSetting.clear();
	UINT numOfDryerSett = m_dryerSettingList.size();

	for (UINT i = 0; i < numOfDryerSett; ++i){
		//if the item is not current dryer setting
		if (false == IsSameDryerSetting(m_dryerSettingList.at(i), m_currDryerSetting)){
			ST_DRYER_SETTING_DELETE_INFO dryerSetting;
			//add the item to the able-to-delete list
			dryerSetting.dispName = m_displayedDryerSettingList.at(i);
			dryerSetting.setting = m_dryerSettingList.at(i);
			dryerSetting.isSelectedToDel = false;

			m_delDlg_listDryerSetting.push_back(dryerSetting);
		}
		//otherwise, don't add it to the able-to-delete list (current setting can not be deleted)
	}
}

/**
 *	Initialize Dryer setting data (to call every Print setting pull-down change selection)
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CDataDryerSetting::EnterScreen(const char* inKeyName)
{
	return UpdatePrintCondition(inKeyName);
}

bool CDataDryerSetting::ExitScreen()
{
	return true;
}

bool CDataDryerSetting::UpdatePrintCondition(const char* inKeyName)
{
	bool retval = true;


	// Init system dryer setting list
	retval = retval && InitDryerSettingList();
	// Create displayed name for dryer setting list
	InitDisplayedDryerSettingName();

	// Init current dryer setting
	retval = retval && ObtainCurrentDryerSetting(inKeyName);
	//Init index of selection dryer setting
	SetSelectPrintDryerSetting(m_currDryerSetting);

	return retval;
}

/**
 *	Check if Dryer setting is changed or not
 *	@return true : changed, false: not changed
**/
bool CDataDryerSetting::IsDryerSettingChanged()
{
	if (m_selectDryerSettingID < 0 || m_selectDryerSettingID >= (long)m_dryerSettingList.size())
	{
		return false;
	}

	return !IsSameDryerSetting(m_currDryerSetting, m_dryerSettingList.at(m_selectDryerSettingID));
}

/**
 *	Check the control has valid selection item
 *	@retval	true : valid
 *	@retval	false : invalid
 *	@note
 *	
 *		
**/
bool CDataDryerSetting::IsHaveValidSelectedItem()
{
	if (m_dryerSettingList.size() == 0)
	{
		return false;
	}

	return true;
}

/**
 *	Save all of values are changed
 *	@retval	TRUE: success (including no changes to be saved)
 *	@retval	FALSE: failure (saving process failed)
**/
bool CDataDryerSetting::SaveValues()
{
	if(!IsDryerSettingChanged()){
		//There's no change to be saved
		return true;
	}

	bool isSaveSuccess = false;
	// The Print condition GUI is started from Job List screen
	if (m_systemSetting->IsStartedFromJobListScreen())
	{
		isSaveSuccess = m_JM_Callbacks->JM_SetDryTempPrint(m_sectionID_printCondtionName, m_dryerSettingList.at(m_selectDryerSettingID));
	}
	else
	{
		isSaveSuccess = m_PaperDB_Callbacks->PDB_SetDryTempPrint(m_sectionID_printCondtionName, m_dryerSettingList.at(m_selectDryerSettingID));
	}

	if(isSaveSuccess) {
		UpdatePrintCondition(m_sectionID_printCondtionName);
	}

	return isSaveSuccess;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outDryerSetting : 乾燥温度(構造体)
*/
void CDataDryerSetting::OnCopyJobPrintCondition(ST_PDB_DRY_TEMP_PRINT &outDryerSetting)
{
	outDryerSetting = m_dryerSettingList.at(m_selectDryerSettingID);
}


/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataDryerSetting::IsBlankNotAllowed()
{
	if(m_isNotSetCurrentDryerSetting)
		WriteToLogBuf(LOG_DEBUG, "Dryer setting is judged as blank.");
	return m_isNotSetCurrentDryerSetting;
}

/**
	@brief	目標乾燥温度の設定
	@param[in]	inPrinterID : PrinterID
	@param[in]	inTemperature : 目標乾燥温度
*/
void CDataDryerSetting::SetDryerTemp_HeatRanger(long inPrinterID, char* inTemperature)
{
	// 最大値と最小値を超えていないか
	long minHeatRangerTemp = m_iniPrintConditionGUI.GetMinHeatRangerTemp();
	long maxHeatRangerTemp = m_iniPrintConditionGUI.GetMaxHeatRangerTemp();

	long temp = atol(inTemperature);

	if(temp >= maxHeatRangerTemp)
	{
		temp = maxHeatRangerTemp;
	}
	if(temp <= minHeatRangerTemp)
	{
		temp = minHeatRangerTemp;
	}

	if(m_systemSetting->IsSelf(inPrinterID)){
		m_addDlg_dryerSetting.SUB_HEAT_TEMP_SELF = temp;
	}else{
		m_addDlg_dryerSetting.SUB_HEAT_TEMP_PART = temp;
	}
}

/**
	@brief	目標乾燥温度の取得
	@param[in]	inPrinterID : PrinterID
	@retval	目標乾燥温度
*/
long CDataDryerSetting::GetDryerTemp_HeatRanger(long inPrinterID)
{
	bool isSelf = m_systemSetting->IsSelf(inPrinterID);

	long retval = m_systemSetting->IsSelf(inPrinterID) ? m_addDlg_dryerSetting.SUB_HEAT_TEMP_SELF : m_addDlg_dryerSetting.SUB_HEAT_TEMP_PART;

	return retval;
}