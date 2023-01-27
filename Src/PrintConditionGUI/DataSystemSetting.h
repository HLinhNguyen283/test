/*! \file DataSystemSetting.h
 *  \brief data class: system settings
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

#if !defined(_DATA_SYSTEM_SETTING_H_)
#define _DATA_SYSTEM_SETTING_H_


// ==========================================================================
// Include files

#include <windows.h>
#include "PrintProfile_Callbacks.h"
#include "JobManager_Callbacks.h"
#include "PaperDB_Callbacks.h"
#include "PrinterState_Callbacks.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_Prnv.h"
#include "MainteJetInspection_Callbacks.h"
#include "PrintConditionGUIDef.h"
#include <vector>
// ==========================================================================
enum
{
	PRINTER1 = 0,		//!< Printer 1
	PRINTER2,			//!< Printer 2

	PRINTER_COUNT		//!< Number of printers
};

/*! \brief data class: system settings
*/
class CDataSystemSetting
{
public:
	//method
	CDataSystemSetting(bool inIsStartForMaintenance);
	virtual ~CDataSystemSetting();
	bool Initialize();

	bool EnterScreen();
	bool ExitScreen();

	//From CDataIF
	bool IsExistPrecoater();
	bool IsDED();
	bool IsExistHeatRoller(long inPrinterID);
	bool IsExistDryerUnit(long inPrinterID);
	bool IsExistOvercoater(long inPrinterID);
	bool IsExistHeatRangerUnit();

	DEF_USER_MODE_ID GetSystemUserMode();

	//
	const char* GetSystemUnitLength(DEF_LENGTH_UNIT* outUnit);
	const char* GetSystemUnitLargeLength(DEF_LENGTH_UNIT* outUnit);
	DEF_LENGTH_UNIT GetSystemUnit();

	SJobManager_Callbacks* GetJobManager_Callbacks();
	SPaperDB_Callbacks* GetPaperDB_Callbacks();
	SPrinterState_Callbacks* GetPrinterState_Callbacks();
	SPrintProfile_Callbacks* GetPrintProfile_Callbacks();
	SMainteJetInspection_Callbacks* GetJetInspection_Callbacks();

	bool IsStartedFromJobListScreen();
	bool IsSystemSupportJetInspection();
	bool IsHaveJetInspection();

	bool IsMaster();
	bool IsSelf(UINT inPrinterID);

	bool IsJobRunning();

	bool IsPrintConditionEdited();
	bool IsJIOptionEnable();
	bool RefreshPrintConditionGUIState(const char* inJobID);
	bool IsEditablePrintConditionGUI();
	bool GetPrintConditionNamesOfRunningJob(std::vector<std::string>& outPrintConditions);

	void SetPostMessageWindow(HWND inWnd);
	HWND GetPostMessageWindow();

	BOOL IsIpdsMode();

	UINT GetDisplayColorSize(DEF_PRINT_SIDE inSide) const;
	bool GetDisplayColor(DEF_PRINT_SIDE inSide, const char* inColorName, COLORREF& outColor);
	const char* GetCBufColorName(UINT inColorID, DEF_PRINT_SIDE inSide);

	bool IsExistMultiColor(long inPrinterID);

	bool IsExistNirDryerUnit(long inPrinterID);

	bool IsUnitOfRanger();
	bool IsExistDNS();
	bool IsMonochromePrinter();

	/**
	 * @brief	表裏倍率調整用のフラッシングページを出力するかどうか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsMagnificationAdjustment();

	/**
		@brief	プライマー接続の有無を取得する
		@retval	true : プライマー接続有り
		@retval	false : プライマー接続無し
	*/
	bool IsConnectedPrimer();

protected:
	//method

	//event

	//parts
	void CreateDisplayColorInfos(DEF_PRINT_SIDE inSide);
	void DeleteDisplayColorInfos(DEF_PRINT_SIDE inSide);
	//member
	bool m_isMasterPc;								//!< true: master-pc, false: slave-pc
	bool m_isSelf[PRINTER_COUNT];					//!< true: is self pc, false: is partner pc, [0]: printer 1, [1]: printer 2
	DEF_USER_MODE_ID m_userMode;					//!< User mode.
	bool m_isDedSystem;								//!< true: is DED, false: is SES
	bool m_isExistPrecoater;						//!< true: it has, false: it does not have
	bool m_isExistOvercoater_Slave;					//!< true: it has, false: it does not have
	bool m_isExistOvercoater_Master;				//!< true: it has, false: it does not have
	bool m_isHaveJetInspection;						//!< true: it has, false: it does not have
	bool m_isHaveEnableJI;							//!< true: enable, false: disable
	bool m_isExistHeatRoller[PRINTER_COUNT];		//!< true: the printer has it, false: the printer does not have it.([0]: printer1, [1]: printer 2)
	bool m_isExistDryerUnit[PRINTER_COUNT];			//!< true: the printer has it, false: the printer does not have it.([0]: printer1, [1]: printer 2)
	bool m_isExistNirDryerUnit[PRINTER_COUNT];		//!< true: the printer has it, false: the printer does not have it.([0]: printer1, [1]: printer 2)
	bool m_isExistMutliColor[PRINTER_COUNT];		//!< true: it has, false: it does not have
	bool m_isExistHeatRangerUnit;					//!< true: the printer has it, false: the printer does not have it.

	bool m_isStartedFromJobList;					//!< true: started from Job List, false : started from System Setting
	bool m_isStartForMaintenace ;					//!< true: print condition GUI is started for maintenance by function call, false: Print condition GUI is started by mouse click

	DEF_LENGTH_UNIT m_lengthUnit;					//!< Unit of length of the system
	const char* m_lengthUnitString;					//!< String of unit of length(mm/inch/pt)
	const char* m_largeLengthUnitString;			//!< String of large unit of length(m/feet)

	static const char* st_unit_mm;					//!< unit of length(mm)
	static const char* st_unit_inch;				//!< unit of length(inch)
	static const char* st_unit_pt;					//!< unit of length(pt)
	static const char* st_unit_m;					//!< unit of length(m)
	static const char* st_unit_feet;				//!< unit of length(feet)

	bool m_isUnitOfDNS;								//!< DNS support
	bool m_isMonochromePrinter;						//!< monochrome printer

	SPaperDB_Callbacks m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks m_JM_Callbacks;			//!< Callbacks for Job Manager
	SPrinterState_Callbacks m_PS_Callbacks;			//!< Call back for Printer State
	SPrintProfile_Callbacks m_PrintProfile_Callbacks;		//!< Call back for Print Profile
	SMainteJetInspection_Callbacks m_JI_Callbacks;			//!< Callbacks for Jet Inspection

	bool m_isEditablePrintCondition;			//!< editable state for print condition GUI

	HWND m_hWnd;								//!< Dialog handle

	BOOL m_isIpdsMode;							//!< IPDS Mode
	COLORINFOLIST m_displayColorInfosList[DEF_PRINT_BOTH];		//!< List of display color id (2 sides)
	UINT m_displayColorSize[DEF_PRINT_BOTH];					//!< Size of color ID list (2 sides)
private:
	//member

	bool m_isMagnifigationAdustment;			//!< 表裏倍率調整用のフラッシングページを出力するかどうか(true:する、 false:しない)
	
};



#endif // !defined(_DATA_SYSTEM_SETTING_H_)
