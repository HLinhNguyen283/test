/*! \file DataDryerSetting.h
 *  \brief data class: dryer setting
 *  \author GCS Y Tran
 *  \date 2013/12/12 GCS Y Tran created
*/

#if !defined(_DATA_DRYER_SETTING_H_)
#define _DATA_DRYER_SETTING_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#include <string>
#include <vector>
#include "Ini_PrintConditionGUI.h"

class CDataIF;

using namespace std;

// ==========================================================================
typedef struct _ST_DRYER_SETTING_DELETE_INFO
{
	ST_PDB_DRY_TEMP_PRINT setting;
	string dispName;
	bool isSelectedToDel;
}ST_DRYER_SETTING_DELETE_INFO;

/*! \brief data class: dryer setting
*/
class CDataDryerSetting
{
public:
	//method
	CDataDryerSetting();
	virtual ~CDataDryerSetting();
	bool Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF);
	bool Finalize();

	bool GetPrintDryerSettingList(long inItemID, const char** outDryer);
	bool InitAddDryerSettingDlg();
	bool InitDeleteDryerSettingDlg();
	void SetSelectPrintDryerSetting(long inItemID);
	bool GetSelectPrintDryerSetting(long* outItemID);

	void SetHeatRollerTemperature_AddDlg(long inPrinterID, char* inTemperature);
	void SetDryerFanTemperature_AddDlg(long inPrinterID, char* inTemperature);
	long GetHeatRollerTemperature_AddDlg(long inPrinterID);
	long GetDryerFanTemperature_AddDlg(long inPrinterID);
	void SetPrecoatTemperature_AddDlg(long inPrinterID, char* inTemperature);
	long GetPrecoatTemperature_AddDlg(long inPrinterID);
	bool IsSavedDryerSetting_AddDlg();
	long GetNirPower_AddDlg(long inPrinterID);
	void SetNirPower_AddDlg(long inPrinterID, char* inNirPower);

	long SaveAddDryerSettingDlg();
	bool CancelAddDryerSettingDlg();
	void SetDeleteDryerSettingDlg(HWND inDialog);
	bool GetDeleteDryerSettingList_DelDlg(long inItemID, const char** outDryer);
	void SetDeleteDryerSetting_DelDlg(long inItemID);
	bool DeleteDryerSettings_DelDlg();
	bool ExitDeleteDryerSettingDlg();
	void SetSelectPrintDryerSetting(ST_PDB_DRY_TEMP_PRINT inDryerSetting);

	bool EnterScreen(const char* inKeyName);
	bool ExitScreen();
	bool UpdatePrintCondition(const char* inKeyName);
	bool IsHaveValidSelectedItem();
	bool IsDryerSettingChanged();
	bool SaveValues();
	void OnCopyJobPrintCondition(ST_PDB_DRY_TEMP_PRINT &outDryerSetting);
	bool IsBlankNotAllowed();
	void SetDryerTemp_HeatRanger(long inPrinterID, char* inTemperature);
	long GetDryerTemp_HeatRanger(long inPrinterID);

protected:
	//method
	void ResetMember();
	void ResetMember_AddDlg();
	void ResetMember_DelDlg();

	bool InitDryerSettingList();
	bool IsSameDryerSetting(const ST_PDB_DRY_TEMP_PRINT &inSetting1, const ST_PDB_DRY_TEMP_PRINT &inSetting2);
	void InitDisplayedDryerSettingName();
	bool ObtainCurrentDryerSetting(const char* inKeyName);
	void InitDryerSettingList_DelDlg();

	//event

	//parts

	//member
	SPaperDB_Callbacks* m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;				//!< Callbacks for Job Manager

	CDataSystemSetting* m_systemSetting;				//!< Data System setting handle
	CDataIF* m_data;

	char m_sectionID_printCondtionName[256];			//!< Section ID (in case start from job setting), Print condition name (in case start from System setting)
	vector<ST_PDB_DRY_TEMP_PRINT> m_dryerSettingList;	//!< List of system dryer setting
	vector<string> m_displayedDryerSettingList;			//!< List of system dryer setting that displayed on the pull-down
	ST_PDB_DRY_TEMP_PRINT m_currDryerSetting;			//!< The current dryer setting (get from PDB_)
	long m_selectDryerSettingID;						//!< The selected dryer setting ID

	//Add dryer setting dialog
	ST_PDB_DRY_TEMP_PRINT m_addDlg_dryerSetting;		//!< The dryer setting displayed on Add dryer setting dialog
	ST_PDB_DRY_TEMP_PRINT m_addDlg_initDryerSetting;	//!< The initial values of dryer setting displayed on Add dryer setting dialog

	//Delete dryer setting dialog
	vector<ST_DRYER_SETTING_DELETE_INFO> m_delDlg_listDryerSetting;		//!< List of dryer setting on Delete dryer setting dialog
	HWND m_delDlg_hwnd;									//!< Handle of Delete dryer setting dialog;
	CIni_PrintConditionGUI m_iniPrintConditionGUI;		//!< Ini class of Print Condition GUI.
	bool m_isNotSetCurrentDryerSetting;					//!< Check if Current dryer setting is set or not

private:
	//member


};


#endif // !defined(_DATA_DRYER_SETTING_H_)
