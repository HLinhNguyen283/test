/*! \file DataICCPresetSetting.h
 *  \brief data class: ICC preset setting
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

#if !defined(_DATA_ICC_PRESET_SETTING_H_)
#define _DATA_ICC_PRESET_SETTING_H_

// ==========================================================================
// Include files

#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "DataSystemSetting.h"
#include "DataPrintSettings.h"
#include "DataPrintMode.h"
#include "PrintConditionGUIDef.h"

// ==========================================================================
typedef struct _SELECTED_ICC_PRESET {
	long selected_index;			//!< Selected index
	string icc_preset_name;			//!< ICC Preset name
}SELECTED_ICC_PRESET;

/*! \brief data class: ICC Preset
*/
class CDataICCPresetSetting
{
public:
	//method
	CDataICCPresetSetting();
	virtual ~CDataICCPresetSetting();
	bool Initialize(CDataSystemSetting* inSystemSetting, CDataPrintMode* inPrintMode);
	bool Finalize();

	bool GetICCPresetList(long inItemId, const char** outICCPreset);
	void SetSelectICCPreset(long inItemId, bool inIsInit = false);
	bool GetSelectICCPreset(long* outItemId);
	bool IsSelectedICCPresetChanged();
	bool SaveValues();
	bool AddSystemICCPreset();
	void SetAddICCPresetNameDlg(char* inICCPreset);
	const char* GetAddICCPresetNameDlg();
	void CancelAddICCPresetNameDlg();
	bool IsAddICCPresetNameExist();
	bool IsExistName();
	void RemoveICCPresetNameNotExist();
	bool IsICCProfileNotAssignedToICCPreset();
	bool InitICCPresetList();
	void CheckICCPresetUse();

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();
	bool UpdatePrintCondition(const char* inKeyName);
	bool IsHaveValidSelectedItem();

	bool IsBlankNotAllowed();

protected:
	//method
	void AppendICCPresetNameIndex(char inOriICCPresetName[256]);
	void ResetMember();
	bool ObtainCurrentICCPreset(const char* inKeyName);
	void SetSelectedItemWhenNotSetCurrent();

	//event

	//parts

	//member
	CDataSystemSetting* m_systemSetting;				//!< Data System setting handle
	CDataPrintMode* m_printMode;						//!< Data Print mode hanlde
	SELECTED_ICC_PRESET m_selectedICCPreset;			//!< Selected ICC preset struct
	string m_initSelectedICCPresetName;					//!< Initial selected ICC Preset name.
	vector<string> m_iccPresetList;						//!< List of ICC Presets
	char m_addICCPresetName[256];						//!< ICC Preset Name is added
	bool m_isExistName;									//!< ICC preser name is exist in the list of ICC Presets or not
	char m_PrintConditionOrSectionID[256];					//!< Print condition or section ID
	char m_curICCPreset[256];							//!< Current ICC preset
	//bool m_ICCPresetListInitialized;					//!< Whether ICC preset list is initialized or not
	string m_currICC_systemSetting;						//!< Name of current ICC preset (saved by MAT)
	string m_ICCName_systemSetting;						//!< Name of current ICC preset (used on GUI)
	bool m_isNotSetCurrentIccPreset;					//!< Check if Current icc preset is set or not

private:
	//member

};
#endif // !defined(_DATA_ICC_PRESET_SETTING_H_)