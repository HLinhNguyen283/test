/*! \file DataDotgainCurve.h
 *  \brief data class: Dot gain curve
 *  \author GCS Dieu Dinh
 *  \date 2013/12/12 GCS Dieu Dinh created
*/

#if !defined(_DATA_DOTGAIN_CURVE_H_)
#define _DATA_DOTGAIN_CURVE_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#include "PrintConditionGUIDef.h"
#include "DataIF.h"
#include "Ini_PrintConditionGUI.h"
// ==========================================================================

/*! \brief data class: Dotgain curve
*/
class CDataDotgainCurve
{
public:
	//method
	CDataDotgainCurve();
	virtual ~CDataDotgainCurve();
	bool Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF);
	bool Finalize();

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();
	bool InitDotgainCurve(const char* inKeyName);
	bool IsDotgainCurveChanged();
	bool UpdatePrintCondition(const char* inPrintConditionOrSectionID);
	bool Save();

	// For drawing
	long GetToneFrameWidth_ForDrawing();
	COLORREF GetToneFrameColor_ForDrawing();
	bool GetDotgainCurve_ForDrawing(ST_PDB_DOTGAIN_CURVE& outDotgainCurve);
	void GetLinearDotGainData_ForDrawing(ST_PDB_ANCHOR_POINT* outAnchor);
	bool IsDotgainCurveDefault_ForDrawing();

	// For ToneSettingGUI
	void SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);
	bool GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve);


protected:
	//method
	void ResetMember();
	bool CompareTwoDotgainCurve(ST_PDB_DOTGAIN_CURVE inDotgainCurve1, ST_PDB_DOTGAIN_CURVE inDotgainCurve2);
	bool isToneCurveFile(TCHAR *inFilePath, long inFilePathSize, const char *inTargetColorName);
	bool SaveDotGainCurveToJobOrSystem();

	//event

	//parts

	//member
	CDataSystemSetting* m_systemSetting;				//!< Data System setting handle
	CDataIF* m_dataIF;									//!< Data IF handle
	ST_PDB_DOTGAIN_CURVE m_dotgainCurve[2];				//!< Dotgain curve value, [0]->COM_SYSTEM_TYPE_SELF, [1]-> COM_SYSTEM_TYPE_PART
	ST_PDB_DOTGAIN_CURVE m_initDotgainCurve[2];			//!< Initial dotgain curve
	bool m_isInitSuccess;								//!< true : dotgain curve can be obtained from PaperDB or JobManager
	char m_printConditionName[256];						//!< Print condition is selected
	CIni_PrintConditionGUI m_iniPrintConditionGUI;		//!< Ini class of Print Condition GUI.
private:
	//member

};
#endif // !defined(_DATA_DOTGAIN_CURVE_H_)