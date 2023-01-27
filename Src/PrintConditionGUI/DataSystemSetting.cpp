/*! \file DataSystemSetting.cpp
 *  \brief data class: system settings
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataSystemSetting.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_Prnv.h"
#include "Stub_PrinterStateIF.h"
#include "Ini_FlushingPageMagnification.h"

// ==========================================================================
// Local definition
#define DEF_MAX_COLOR_NUMBER		8					//!< Max number of color
#define STR_EMPTY					_T("")				//!< Empty string
#define DEF_COLOR_BLACK				RGB(0, 0, 0)		//!< BLACK color

const char* CDataSystemSetting::st_unit_mm = "mm";
const char* CDataSystemSetting::st_unit_inch = "inch";
const char* CDataSystemSetting::st_unit_pt = "pt";
const char* CDataSystemSetting::st_unit_m = "m";
const char* CDataSystemSetting::st_unit_feet = "feet";

/**
 * constructor
 * @note Normal process
 * 
 *		Initialize the member variables
 *	@note 
 *  \param[IN] inIsStartForMaintenance The print condition GUI is start for maintenance
 *		
**/
CDataSystemSetting::CDataSystemSetting(bool inIsStartForMaintenance)
{
	m_isMasterPc = false;
	m_userMode = USER_ID_GENERAL;
	m_isDedSystem = false;
	m_isExistPrecoater = false;
	m_isExistOvercoater_Slave = false;
	m_isExistOvercoater_Master = false;
	m_isHaveJetInspection = false;
	m_isHaveEnableJI = false;
	memset(m_isExistDryerUnit, false, sizeof(m_isExistDryerUnit));
	memset(m_isExistNirDryerUnit, false, sizeof(m_isExistNirDryerUnit));
	memset(m_isExistMutliColor, false, sizeof(m_isExistMutliColor));
	memset(m_isExistHeatRoller, false, sizeof(m_isExistHeatRoller));
	m_isExistHeatRangerUnit = false;
	
	m_lengthUnit = DEF_LENGTH_MM;
	m_lengthUnitString = NULL;
	m_largeLengthUnitString = NULL;
	m_isStartedFromJobList = false;

	m_isStartForMaintenace = inIsStartForMaintenance;
	memset(m_isSelf, 0, sizeof(m_isSelf));
	m_isEditablePrintCondition = true;

	m_hWnd = NULL;

	m_isIpdsMode = FALSE;

	m_displayColorInfosList[DEF_PRINT_FRONT] = NULL;
	m_displayColorInfosList[DEF_PRINT_BACK] = NULL;
	memset(m_displayColorSize, 0x00, sizeof(m_displayColorSize));
	m_isUnitOfDNS = false;
	m_isMonochromePrinter = false;

	m_isMagnifigationAdustment = false;
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
CDataSystemSetting::~CDataSystemSetting()
{

}


/**
 * Initialzie menber variables in the class
 * @retval	TRUE: success
 * @retval	FALSE: failure
 * @note Notes
 * 
 *		The methods to initialize each member variables are written at each getter methods.
 *	@note 
 *	
 *		
**/
bool CDataSystemSetting::Initialize()
{
	// Initialize PaperDB, JobManager and PrinterState callbacks
	if (!PaperDB_GetCallbacks(&m_PaperDB_Callbacks))
		return false;

	// IPDS Mode：JobManagerが存在しないため実行回避
	CIni_PrinterDescriptor iniPrinterDescriptor;
	if (FALSE == iniPrinterDescriptor.Initialize(TRUE)) {
		return false;
	}
	if (iniPrinterDescriptor.IsIpdsMode() == FALSE) {
		if (!JM_GetCallbacks(&m_JM_Callbacks)) { return false; }
	}
	if (!MainteJetInspection_GetCallbacks(&m_JI_Callbacks)) { return false; }	
	iniPrinterDescriptor.Finalize();

	if (!PrinterState_GetCallbacks(&m_PS_Callbacks)){
		return false;
	}
	if (!PrintProfile_GetCallbacks(&m_PrintProfile_Callbacks))
		return false;

	// 表裏倍率調整ON/OFF
	Ini_FlushingPageMagnification iniFlushingPageMagnification;
	iniFlushingPageMagnification.Initialize(TRUE);
	m_isMagnifigationAdustment = iniFlushingPageMagnification.IsMagnificationAdjustment();

//	2014.2.27 matsuda: moved to EnterScreen()
/*	// Ini management
	CIni_PrinterDescriptor iniPrinterDescriptor;
	CIni_Prnv iniPrnv;

	if (FALSE == iniPrinterDescriptor.Initialize(TRUE)
		|| FALSE == iniPrnv.Initialize(TRUE))
	{
		return false;
	}

	//Initialize for m_isMasterPc variable
	BOOL isDedSystem = iniPrinterDescriptor.IsDedSystem();
	BOOL isDedMaster = iniPrinterDescriptor.IsDedMaster();

	//---the system is running with DED and the system setting in DED is master-pc
	if(TRUE == isDedSystem && TRUE == isDedMaster) {
		//the system is master-pc
		m_isMasterPc = true;
	}
	//---the system is running with DED and the system setting in DED is slave-pc
	else if(TRUE == isDedSystem && FALSE == isDedMaster) {
		//the system is slave-pc.
		m_isMasterPc = false;
	}else {
		//---the system is master-pc because it is running with SES
		m_isMasterPc = true;
	}

	//Initialize for m_userMode variable
	m_userMode = (DEF_USER_MODE_ID)iniPrnv.GetUserMode();

	//Initialize for m_isDedSystem variable
	m_isDedSystem = TRUE == isDedSystem;

	//Initialize for m_isExistPrecoater variable
	m_isExistPrecoater = m_PS_Callbacks.PrinterState_IsLoadingPreCoat();

	//Initialize for m_isSelf variable
	//In DED and Master-PC
	if(TRUE == IsDED() && TRUE == IsMaster()) {
		m_isSelf[PRINTER1] = false;				// PrinterID:1 = Self:false
		m_isSelf[PRINTER2] = true;				// PrinterID:2 = Self:true
	}
	//In DED and Slave-PC
	else if(TRUE == IsDED() && FALSE == IsMaster()) {
		m_isSelf[PRINTER1] = true;				// PrinterID:1 = Self:true
		m_isSelf[PRINTER2] = false;				// PrinterID:2 = Self:false
	}
	//In SES
	else if(FALSE == IsDED()) {
		m_isSelf[PRINTER1] = true;				// PrinterID:1 = Self:true
	}

	//Initialize for m_isExitDryerUnit[]
	m_isExistDryerUnit[PRINTER1] = 0 != m_PS_Callbacks.PrinterState_IsLoadingHeaterFan(IsSelf(PRINTER1+1));
	m_isExistDryerUnit[PRINTER2] = 0 != m_PS_Callbacks.PrinterState_IsLoadingHeaterFan(IsSelf(PRINTER2+1));

	// Initialize for m_lengthUnitString, m_largeLengthUnitString and m_lengthUnit variables
	m_lengthUnit = iniPrnv.GetLengthUnit();
	switch(m_lengthUnit){
	case DEF_LENGTH_MM:
		m_lengthUnitString = st_unit_mm;
		m_largeLengthUnitString = st_unit_m;
		break;
	case DEF_LENGTH_INCH:
		m_lengthUnitString = st_unit_inch;
		m_largeLengthUnitString = st_unit_feet;
		break;
	case DEF_LENGTH_POINT:
		m_lengthUnitString = st_unit_pt;
		m_largeLengthUnitString = st_unit_feet;
		break;
	}

	// Init overcoat
	m_isExistOvercoater_Slave = iniPrinterDescriptor.IsExistCoating(true);
	m_isExistOvercoater_Master = iniPrinterDescriptor.IsExistCoating(false);

	// Jet Inspection
	m_isHaveJetInspection = iniPrinterDescriptor.GetJetInspectionOption() == 0 ? FALSE : TRUE;
	m_isHaveEnableJI = iniPrinterDescriptor.IsEnableJetInspection() ? true : false;

	// Finalize ini classes
	iniPrinterDescriptor.Finalize();
	iniPrnv.Finalize();
*/
//	2014.2.27

	return true;
}

bool CDataSystemSetting::EnterScreen()
{
//	2014.2.27 matsuda: moved from Initialize()
	// Ini management
	CIni_PrinterDescriptor iniPrinterDescriptor;
	CIni_Prnv iniPrnv;

	if (FALSE == iniPrinterDescriptor.Initialize(TRUE)
		|| FALSE == iniPrnv.Initialize(TRUE))
	{
		return false;
	}

	//Initialize for m_isMasterPc variable
	BOOL isDedSystem = iniPrinterDescriptor.IsDedSystem();
	BOOL isDedMaster = iniPrinterDescriptor.IsDedMaster();

	//---the system is running with DED and the system setting in DED is master-pc
	if(TRUE == isDedSystem && TRUE == isDedMaster) {
		//the system is master-pc
		m_isMasterPc = true;
	}
	//---the system is running with DED and the system setting in DED is slave-pc
	else if(TRUE == isDedSystem && FALSE == isDedMaster) {
		//the system is slave-pc.
		m_isMasterPc = false;
	}else {
		//---the system is master-pc because it is running with SES
		m_isMasterPc = true;
	}

	//Initialize for m_userMode variable
	if(iniPrnv.IsServiceMode())
		m_userMode = USER_ID_SERVICE;
	else
		m_userMode = (DEF_USER_MODE_ID)iniPrnv.GetUserMode();

	//Initialize for m_isDedSystem variable
	m_isDedSystem = TRUE == isDedSystem;

	//Initialize for m_isExistPrecoater variable
	m_isExistPrecoater = m_PS_Callbacks.PrinterState_IsLoadingPreCoat();

	//Initialize for m_isSelf variable
	//In DED and Master-PC
	if(TRUE == IsDED() && TRUE == IsMaster()) {
		m_isSelf[PRINTER1] = false;				// PrinterID:1 = Self:false
		m_isSelf[PRINTER2] = true;				// PrinterID:2 = Self:true
	}
	//In DED and Slave-PC
	else if(TRUE == IsDED() && FALSE == IsMaster()) {
		m_isSelf[PRINTER1] = true;				// PrinterID:1 = Self:true
		m_isSelf[PRINTER2] = false;				// PrinterID:2 = Self:false
	}
	//In SES
	else if(FALSE == IsDED()) {
		m_isSelf[PRINTER1] = true;				// PrinterID:1 = Self:true
	}

	//Initialize for m_isExistHeatRollerUnit[]
	m_isExistHeatRoller[PRINTER1] = (0 == m_PS_Callbacks.GetDryUnitType(IsSelf(PRINTER1+1))? false:true);
	m_isExistHeatRoller[PRINTER2] = ((!IsDED()) || (0 == m_PS_Callbacks.GetDryUnitType(IsSelf(PRINTER2+1)))? false:true);

	//Initialize for m_isExitDryerUnit[]
	m_isExistDryerUnit[PRINTER1] = 0 != m_PS_Callbacks.PrinterState_IsLoadingHeaterFan(IsSelf(PRINTER1+1));
	m_isExistDryerUnit[PRINTER2] = 0 != m_PS_Callbacks.PrinterState_IsLoadingHeaterFan(IsSelf(PRINTER2+1));

	//Initialize for m_isExitNirDryerUnit[]
	m_isExistNirDryerUnit[PRINTER1] = 0 != m_PS_Callbacks.PrinterState_IsUnitOfNir(IsSelf(PRINTER1+1));
	m_isExistNirDryerUnit[PRINTER2] = 0 != m_PS_Callbacks.PrinterState_IsUnitOfNir(IsSelf(PRINTER2+1));

	//Initialize for m_isExistMutliColor[]
	m_isExistMutliColor[PRINTER1] = 0 != m_PS_Callbacks.PrinterState_IsUnitOfSixColors(IsSelf(PRINTER1+1));
	m_isExistMutliColor[PRINTER2] = 0 != m_PS_Callbacks.PrinterState_IsUnitOfSixColors(IsSelf(PRINTER2+1));

	// Check existence of DNS
	m_isUnitOfDNS = m_PS_Callbacks.PrinterState_IsUnitOfDns(true);
	if (TRUE == IsDED())
	{
		 m_isUnitOfDNS = m_isUnitOfDNS && m_PS_Callbacks.PrinterState_IsUnitOfDns(false);
	}

	// Check monochrome printer or not
	m_isMonochromePrinter = m_PS_Callbacks.IsMonochromePrinter();

	//Initialize for m_isExistHeatRangerUnit
	m_isExistHeatRangerUnit = 0 != IsUnitOfRanger();

	// Initialize for m_lengthUnitString, m_largeLengthUnitString and m_lengthUnit variables
	m_lengthUnit = iniPrnv.GetLengthUnit();
	switch(m_lengthUnit){
	case DEF_LENGTH_MM:
		m_lengthUnitString = st_unit_mm;
		m_largeLengthUnitString = st_unit_m;
		break;
	case DEF_LENGTH_INCH:
		m_lengthUnitString = st_unit_inch;
		m_largeLengthUnitString = st_unit_feet;
		break;
	case DEF_LENGTH_POINT:
		m_lengthUnitString = st_unit_pt;
		m_largeLengthUnitString = st_unit_feet;
		break;
	}

	// Init overcoat
	m_isExistOvercoater_Slave = iniPrinterDescriptor.IsExistCoating(true);
	m_isExistOvercoater_Master = iniPrinterDescriptor.IsExistCoating(false);

	// Jet Inspection
	m_isHaveJetInspection = iniPrinterDescriptor.GetJetInspectionOption() != JETINSPECTION_FULLSPEC ? FALSE : TRUE;
	m_isHaveEnableJI = iniPrinterDescriptor.IsEnableJetInspection() ? true : false;

	// IPDS Mode
	m_isIpdsMode = iniPrinterDescriptor.IsIpdsMode();

	// Finalize ini classes
	iniPrinterDescriptor.Finalize();
	iniPrnv.Finalize();
//	2014.2.27

	m_isStartedFromJobList = false;

	//ignore this check if print condition GUI for maintenance is opened in
	//other way (not include open it on maintenance screen), eg: auto open when feeding process finished
	if (!m_isStartForMaintenace) {
		// IPDSモードの場合、JobManagerが存在しないため該当の関数を実行回避
		if (IsIpdsMode() == FALSE) {
			// Check the Print Condition GUI is started from the Job List Screen
			char tmpSection[256] = {0};
			if (m_JM_Callbacks.JM_GetSectionIDToSelectPrintCondition(tmpSection))
			{
				if (strlen(tmpSection) > 0)
				{
					m_isStartedFromJobList = true;
				}
			}
		}
	}

	CreateDisplayColorInfos(DEF_PRINT_FRONT);
	if(IsDED()) {
		CreateDisplayColorInfos(DEF_PRINT_BACK);
	}
	return true;
}

bool CDataSystemSetting::ExitScreen()
{
	if(m_isStartedFromJobList){
		if(IsIpdsMode() == FALSE) {
			m_JM_Callbacks.JM_SetSectionIDToSelectPrintCondition("");
		}
	}
	DeleteDisplayColorInfos(DEF_PRINT_FRONT);
	DeleteDisplayColorInfos(DEF_PRINT_BACK);
	return true;
}

/**
 *	Get the status if the system runs with DED or SES.
 *	@retval	true: DED
 *	@retval	false: SES
 *	@note Process to initialize the member variable
 *	
 *		CIni_PrinterDescriptor::IsDedSystem
 *	@note General process
 *	
 *		Return the member variable.
 *	@note Notes
 *	
 *		
**/
bool CDataSystemSetting::IsDED()
{
	return m_isDedSystem;
}

/**
 *	Check if the printer has a precoater unit or not.
 *	@retval	true: it has
 *	@retval	false: it does not have
 *	@note Process to initialize the member variable
 *	
 *		PrinterState_GetRPPSPreCoater
 *	@note General process
 *	
 *		Return the member variable.
**/
bool CDataSystemSetting::IsExistPrecoater()
{
	return m_isExistPrecoater;
}

/*! \brief Check if a printer has a heat roller or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataSystemSetting::IsExistHeatRoller(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return false;
	}

	return m_isExistHeatRoller[inPrinterID-1];
}

/*! \brief Check if a printer has a dryer unit or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataSystemSetting::IsExistDryerUnit(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return false;
	}

	return m_isExistDryerUnit[inPrinterID-1];
}

/*! \brief Check if a printer has a nir dryer unit or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataSystemSetting::IsExistNirDryerUnit(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return false;
	}

	return m_isExistNirDryerUnit[inPrinterID-1];
}

/**
 *	Check if the printer has a overcoater unit or not.
 *	@param[in] inPrinterID: a printer ID(1-2)
 *	@retval	true: it has
 *	@retval	false: it does not have
 *	@note Process to initialize the member variable
 *	
**/
bool CDataSystemSetting::IsExistOvercoater(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2)
	{
		return false;
	}

	return (m_isDedSystem && (inPrinterID == 1)) ? m_isExistOvercoater_Slave : m_isExistOvercoater_Master;
}

/*! \brief Check if a printer has a Sub Heat Roller dryer unit or not.
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataSystemSetting::IsExistHeatRangerUnit()
{
	return m_isExistHeatRangerUnit;
}

/**
 *	Get an user mode of the system.
 *	@retval	an user mode(general, expert, service)
 *	@note Process to initialize the member variable
 *	
 *		CIni_Prnv::GetUserMode
 *	@note General process
 *	
 *		Return the member variable.
**/
DEF_USER_MODE_ID CDataSystemSetting::GetSystemUserMode()
{
	return m_userMode;
}

/**
 *	Check if the printer support Jet Inspection or not
 *	@retval	true: it has
 *	@retval	false: it does not have
 *	@note Process to initialize the member variable
 *	
**/
bool CDataSystemSetting::IsSystemSupportJetInspection()
{
	return m_isHaveJetInspection && m_isHaveEnableJI;
}

/**
 *	Check if the printer support Jet Inspection or not
 *	@retval	true: it has
 *	@retval	false: it does not have
 *	@note Process to initialize the member variable
 *	
**/
bool CDataSystemSetting::IsHaveJetInspection()
{
	return m_isHaveJetInspection;
}

/**
 *	Check if the printer is the master-pc or not.
 *	@retval	true: master-pc
 *	@retval	false: slave-pc
 *	@note Process to initialize the member variable
 *	
 *		If the system is running with DED and the system setting in DED is master-pc,
 *		- the system is master-pc.
 *			- CIni_PrinterDescriptor::IsDedSystem
 *			- CIni_PrinterDescriptor::IsDedMaster
 *		Else the system is running with DED and the system setting in DED is slave-pc,
 *		- the system is slave-pc.
 *		Else,
 *		- the system is master-pc because it is running with SES.
 *	@note General process
 *	
 *		Return the member variable.
**/
bool CDataSystemSetting::IsMaster()
{
	return m_isMasterPc;
}

/**
 *	Check if the printer is self PC or partner PC
 *	@param[in]	inPrinterID : printer ID.(1: printer-1(front), 2:printer-2(back))
 *	@retval	the number of colors
 *	@note Process to initialize the member variable
 *	
 *		TBD
 *	@note General process
 *	
 *		Return the member variable.
**/
bool CDataSystemSetting::IsSelf(UINT inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > PRINTER_COUNT){
		return false;
	}
	return m_isSelf[inPrinterID-1];
}

/**
 *	Get an unit of length.
 *	@param[out]	outUnit : a pointer to get an unit of length.(mm/inch/point)
 *	@retval	an unit of length(mm/inch/point)
 *	@note Notes
 *	
 *		
**/
const char* CDataSystemSetting::GetSystemUnitLength(DEF_LENGTH_UNIT* outUnit)
{
	if (!outUnit) {
		return "";
	}

	*outUnit = m_lengthUnit;
	return m_lengthUnitString;
}

DEF_LENGTH_UNIT CDataSystemSetting::GetSystemUnit()
{
	return m_lengthUnit;
}

/**
 *	Get a large unit of length.
 *	@param[out]	outUnit : a pointer to get a large unit of length.(mm/inch/point)
 *	@retval	a unit of length(m/feet)
 *	@note Notes
 *	
 *		
**/
const char* CDataSystemSetting::GetSystemUnitLargeLength(DEF_LENGTH_UNIT* outUnit)
{
	if (!outUnit) {
		return "";
	}

	*outUnit = m_lengthUnit;
	return m_largeLengthUnitString;
}

/**
 *	Check the Print Condition GUI is started from the Job List Screen or not
 *	@retval	true : Job List Screen
 *	@retval	false : System Setting Screen
 *	@note Notes
 *	
 *		
**/
bool CDataSystemSetting::IsStartedFromJobListScreen()
{
	return m_isStartedFromJobList;
}

/**
 *	Get Job Manager callbacks handle
 *	@retval	: Pointer to Job manager callbacks
 *	@note Notes
 *	
 *		
**/
SJobManager_Callbacks* CDataSystemSetting::GetJobManager_Callbacks()
{
	return &m_JM_Callbacks;
}

/**
 *	Get PaperDB callbacks handle
 *	@retval	: Pointer to PaperDB callbacks
 *	@note Notes
 *	
 *		
**/
SPaperDB_Callbacks* CDataSystemSetting::GetPaperDB_Callbacks()
{
	return &m_PaperDB_Callbacks;
}

/**
 *	Get Printer State callbacks handle
 *	@retval	: Pointer to Printer State callbacks
 **/
SPrinterState_Callbacks* CDataSystemSetting::GetPrinterState_Callbacks()
{
	return &m_PS_Callbacks;
}

/**
 *	Get Print Profile callbacks handle
 *	@retval	: Pointer to Print Profile callbacks
 **/
SPrintProfile_Callbacks* CDataSystemSetting::GetPrintProfile_Callbacks()
{
	return &m_PrintProfile_Callbacks;
}

/**
 *	Get JetInspection callbacks handle
 *	@retval	: Pointer to JetInspection callbacks
 *	@note Notes
 *	
 *		
**/
SMainteJetInspection_Callbacks* CDataSystemSetting::GetJetInspection_Callbacks()
{
	return &m_JI_Callbacks;
}

/*! \brief Check if a job is running for printing or not.
 *  \retval true: a job is running
 *  \retval false: a job is not running
*/
bool CDataSystemSetting::IsJobRunning()
{
	if (IsIpdsMode() == TRUE) {
		return false;
	}

	return m_JM_Callbacks.JM_IsRunJob();
}

/**
 *	Check the Print Condition of the job is edited from the original setting or not
 *	@retval	true : edited
 *	@retval	false : not edited
 *	@note Notes
 *	
 *		
**/
bool CDataSystemSetting::IsPrintConditionEdited()
{
	if (IsIpdsMode() == TRUE) {
		return false;
	}

	char sectionID[256] = {0};

	if(m_JM_Callbacks.JM_GetSectionIDToSelectPrintCondition(sectionID)){
		BOOL bEdited;
		if(m_JM_Callbacks.JM_GetPrintConditionEditFlag(sectionID, bEdited))
			return TRUE == bEdited;
	}
	return false;
}
/**
 *	Check the the JI option is enable or not
 *	@retval	true : enable
 *	@retval	false : disable
 *	
**/
bool CDataSystemSetting::IsJIOptionEnable()
{
	bool isEnable = false;
	CIni_PrinterDescriptor iniPrinterDescriptor;
	if (TRUE == iniPrinterDescriptor.Initialize(TRUE)){
		//0 :JI option disable
		//1,2 :JI option enable
		if (JETINSPECTION_FULLSPEC == iniPrinterDescriptor.GetJetInspectionOption())
		{
			isEnable = true;
		} else {
			isEnable = false;
		}
		// Finalize ini classes
		iniPrinterDescriptor.Finalize();
	}
	return isEnable;
}

/**
 *	Refesh Print Condition GUI state
 *	@param[in] inJobID: JobID
 *	@retval	true : Print condition GUI need to refesh
 *	@retval	false : Print condition GUI doesn't need to refesh
**/
bool CDataSystemSetting::RefreshPrintConditionGUIState(const char* inJobID)
{
	if (IsIpdsMode() == TRUE) {
		return false;
	}

	bool isSetupEditingData = false;	//Don't refer to the data of the job for setup editing in this plugin.

	// get reserve print flag
	bool reservePrintFlg = m_JM_Callbacks.JM_GetReservePrintMode(inJobID, isSetupEditingData);
	//get ripped status
	ENUM_PRE_RIP_PROGRESS preRipStatus = DEF_PRE_RIP_PROGRESS_NONE;
	if (!m_JM_Callbacks.JM_GetPreRIPStatus(inJobID, preRipStatus, isSetupEditingData)) {
		return false;
	}
	bool rippedJob = (preRipStatus == DEF_PRE_RIP_PROGRESS_COMP);

	//items in print condition screen can not be edited when 
	//jobs with print reservation
	//jobs with RIPed RIP and Hold job
	//
	// 2018.09.19
	// Jobs with RIPed RIP and Hold job allows editing because it does not depend on RIP.
	bool isEditable = !(reservePrintFlg && !rippedJob);

	if (m_isEditablePrintCondition != isEditable) {
		m_isEditablePrintCondition = isEditable;
		return true;
	}
	return false;
}

/**
 *	Get editable state for print condition GUI
 *	@retval	true : Items in print condition GUI can be edited
 *	@retval	false : Items in print condition GUI cannot be edited
**/
bool CDataSystemSetting::IsEditablePrintConditionGUI()
{
	if(!IsStartedFromJobListScreen()) {
		return true;
	}
	return m_isEditablePrintCondition;
}

/**
 *	Get print condition names of running job
 *	@param[out] outPrintCondition: Print condition names
 *	@retval	true :	 success
 *	@retval	false : fail
**/
bool CDataSystemSetting::GetPrintConditionNamesOfRunningJob(std::vector<std::string>& outPrintConditions)
{
	if (IsIpdsMode() == TRUE) {
		return false;
	}

	outPrintConditions.clear();
	char runningJobID[DEF_MAX_JOB_ID_SIZE] = {0};
	long index = 0;

	while (m_JM_Callbacks.JM_GetJobRefID_Run(index++, runningJobID)) {
		//get print condition name of running job
		std::string printConditionOfRunningJob;
		if (!m_JM_Callbacks.JM_GetPrintCondition(runningJobID, printConditionOfRunningJob)) {
			continue;
		}

		if (printConditionOfRunningJob.length() > 0) {
			outPrintConditions.push_back(printConditionOfRunningJob);
		}
	}

	return outPrintConditions.size() > 0 ? true : false;
}

/*! \brief Set a window handle.
 *  \param inWnd: window handle
*/
void CDataSystemSetting::SetPostMessageWindow(HWND inWnd)
{
	m_hWnd = inWnd;
}

/*! \brief Get a window handle.
 *  \ retVal window handle
*/
HWND CDataSystemSetting::GetPostMessageWindow()
{
	return m_hWnd;
}

/**
 *	Check if the system is running with IPDS mode
 *	@retval	TRUE: IPDS mode
 *	@retval	FALSE: normal mode
**/
BOOL CDataSystemSetting::IsIpdsMode()
{
	return m_isIpdsMode;
}

/**
 *	Create list of color info
 *	@param[in]	inSide : target side
**/
void CDataSystemSetting::CreateDisplayColorInfos(DEF_PRINT_SIDE inSide)
{
	if(DEF_PRINT_BOTH == inSide) {
		return;
	}

	auto getColorRGB = [](long inC, long inM, long inY, long inK) -> COLORREF {
		unsigned char conv_cmyk[4] = { 0 };
		unsigned char conv_rgb[4] = { 0 };
		double density = 0.8;
		conv_cmyk[0] = (unsigned char)((double)inC * density / 100.0 * 255.0 );
		conv_cmyk[1] = (unsigned char)((double)inM * density / 100.0 * 255.0);
		conv_cmyk[2] = (unsigned char)((double)inY * density / 100.0 * 255.0);
		conv_cmyk[3] = (unsigned char)((double)inK * density / 100.0 * 255.0);
		ConvertColorPixel(PQNS_CMYK, conv_cmyk, PQNS_DISPLAYRGB, conv_rgb);
		return RGB(conv_rgb[2], conv_rgb[1], conv_rgb[0]);
	};

	DeleteDisplayColorInfos(inSide);

	bool isSelf = (IsDED() && DEF_PRINT_FRONT == inSide)? false : true;

	COLORINFO tmp[DEF_MAX_COLOR_NUMBER] = { 0 };
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	UINT colorSize = iniPrinterDescriptor.GetRipServerInkCount(isSelf);
	UINT colorCount = 0;
	for (UINT colorID = 0; colorID < colorSize && colorID < DEF_MAX_COLOR_NUMBER; ++colorID)
	{
		const char* bufName = iniPrinterDescriptor.GetRipServerCBufColorName(isSelf, colorID);

		if (stricmp(bufName, "C") == 0 ||
			stricmp(bufName, "M") == 0 ||
			stricmp(bufName, "Y") == 0 ||
			stricmp(bufName, "K") == 0 ||
			stricmp(bufName, "R") == 0 ||
			stricmp(bufName, "O") == 0||
			stricmp(bufName, "S1") == 0||
			stricmp(bufName, "S2") == 0 )
		{
			tmp[colorCount].CORLOR_NUM = colorID;
			_snprintf_s(tmp[colorCount].CBUF_NAME, _TRUNCATE, "%s",iniPrinterDescriptor.GetRipServerCBufColorName(isSelf, colorID));
			// Get display color
			const char* CMYK_Color = iniPrinterDescriptor.GetRipServerInkColor(isSelf, colorID);
			long cyan = 0, magenta = 0, yellow = 0, black = 0;
			sscanf_s(CMYK_Color, "%ld,%ld,%ld,%ld", &cyan, &magenta, &yellow, &black);
			tmp[colorCount].COLOR = getColorRGB(cyan, magenta, yellow, black);

			++colorCount;
		}
	}

	m_displayColorSize[inSide] = colorCount;

	iniPrinterDescriptor.Finalize();

	if (colorCount)
	{
		COLORINFOLIST& colorInfos = m_displayColorInfosList[inSide];
		size_t size = sizeof(COLORINFO) * colorCount;
		colorInfos = (COLORINFOLIST)IO_malloc(size);
		memcpy_s(colorInfos, size, tmp, size);
	}
}

/**
 *	Delete list of color info
 *	@param[in]	inSide : Target side
**/
void CDataSystemSetting::DeleteDisplayColorInfos(DEF_PRINT_SIDE inSide)
{
	if (m_displayColorInfosList[inSide] && DEF_PRINT_BOTH != inSide)
	{
		IO_free(m_displayColorInfosList[inSide]);
		m_displayColorInfosList[inSide] = NULL;
	}
}

/**
 *	Get color by color name
 *	@param[in]	inSide : a print side(front/back)
 *	@param[in]	inColorName : the color name
 *	@param[out]	outColor : Color value of color name
 *	@retval	true: The color exist
 *	@retval	false: The color doesn't exist
**/
bool CDataSystemSetting::GetDisplayColor(DEF_PRINT_SIDE inSide, const char* inColorName, COLORREF& outColor)
{
	outColor = DEF_COLOR_BLACK;
	if(!inColorName || DEF_PRINT_BOTH == inSide) {
		return false;
	}

	for(UINT indexColor = 0; indexColor < m_displayColorSize[inSide]; ++indexColor) {
		if(stricmp(m_displayColorInfosList[inSide][indexColor].CBUF_NAME, inColorName) == 0) {
			outColor = m_displayColorInfosList[inSide][indexColor].COLOR;
			return true;
		}
	}

	return false;
}

/**
 *	Get Cbuf color name
 *	@param[in]	inColorID : number of color
 *	@param[in]	inSide : printer side
 *	@retval	Cbuf color name
 *	@note NOTE
**/
const char* CDataSystemSetting::GetCBufColorName(UINT inColorID, DEF_PRINT_SIDE inSide)
{
	if(DEF_PRINT_BOTH == inSide) {
		return STR_EMPTY;
	}
	if (inColorID < m_displayColorSize[inSide]){
		return m_displayColorInfosList[inSide][inColorID].CBUF_NAME;
	}
	
	return STR_EMPTY;
}

/**
 *	Get colors size
 *	@param[in]	inSide : Target side
 *	@retval	: number of colors
**/
UINT CDataSystemSetting::GetDisplayColorSize(DEF_PRINT_SIDE inSide) const
{
	if(DEF_PRINT_BOTH == inSide) {
		return 0;
	}
	return m_displayColorSize[inSide];
}

/*! \brief Check if a printer has multi color option or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataSystemSetting::IsExistMultiColor(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return false;
	}

	return m_isExistMutliColor[inPrinterID-1];
}

bool CDataSystemSetting::IsUnitOfRanger()
{
	return Stub_PrinterStateIF::IsUnitOfRanger();
}

/**
 *	Check existence of DNS
 *	@retval	: true: exist
**/
bool CDataSystemSetting::IsExistDNS()
{
	return m_isUnitOfDNS;
}

/**
 *	Check Monochrome Printer or Not
 *	@retval	: true: monochrome printer
**/
bool CDataSystemSetting::IsMonochromePrinter()
{
	return m_isMonochromePrinter;
}

// 表裏倍率調整用のフラッシングページを出力するかどうか
bool CDataSystemSetting::IsMagnificationAdjustment()
{
	return m_isMagnifigationAdustment;
}

// プライマー接続の有無を取得する
bool CDataSystemSetting::IsConnectedPrimer()
{
	return Stub_PrinterStateIF::IsConnectedPrimer();
}
