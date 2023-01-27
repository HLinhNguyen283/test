/*! \file DataPrintMode.h
 *  \brief data print mode class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
#if !defined(_DATA_PRINT_MODE_H_)
#define _DATA_PRINT_MODE_H_

// ==========================================================================
// Include files

#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include "DataSystemSetting.h"

class CDataIF;

using namespace std;

// ==========================================================================

typedef struct _PrintMode
{
	long ID;
	char DisplayName[256];
	ST_PDB_PRINT_MODE Data;
} PrintMode;

typedef struct _PrintSpeed
{
	long ID;
	long Value;
	char DisplayName[256];
} PrintSpeed;

typedef struct _Tension
{
	long ID;
	long Value;
} Tension;

/*! \brief data interface class
*/
class CDataPrintMode
{
public:
	//method
	CDataPrintMode();
	virtual ~CDataPrintMode();
	//
	virtual BOOL Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF);
	virtual void Finalize();

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();

	PrintMode* GetPrintModeByName(const char* inPrintModeName);

	PrintMode* GetSelectPrintMode();
	PrintSpeed* GetSelectPrintSpeed();
	long GetSelectPrintSpeedValue();
	Tension* GetSelectPaperTension();
	long GetSelectPaperTensionValue();
	bool IsChanged();
	bool Save();
	void OnCopyJobPrintCondition(long &outPrintSpeedValue, eMODE_RESO_SPEED_UNIT &outPrintSpeedUnit, long &outPaperTensionValue);

	bool GetPrintModeList(long inItemID, const char** outMode);
	void SetSelectPrintMode(long inItemID);
	bool GetSelectPrintMode(long* outItemID);

	PrintSpeed* GetPrintSpeedByValue(long inValue);

	bool GetPrintSpeedList(long inItemID, const char** outSpeed);
	void SetSelectPrintSpeed(long inItemID);
	bool GetSelectPrintSpeed(long* outItemID);

	Tension* GetTensionByValue(long inValue);

	void GetPaperTensionRange(long* outMin, long* outMax);
	void SetSelectPaperTension(long inItemID);
	bool GetSelectPaperTension(long* outItemID);

	void UpdatePrintCondition(const char* inPrintConditionOrSection);
	void UpdateOutputColorMode();

protected:
	//member
	CDataSystemSetting* m_SystemSetting;
	CDataIF* m_data;
	SPaperDB_Callbacks* m_PaperDB_Callbacks;	//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;		//!< Callbacks for Job Manager

	long m_TensionDef;							//!< The default value of tension

	char m_PrintConditionOrSection[256];		//!< In case the GUI is started from Job List Screen : SectionID
												//!< In case the GUI is started from System Setting Screen : Print Condition Name

	vector<PrintMode*> m_PrintModes;			//!< List of print modes
	PrintMode* m_SelectPrintMode;				//!< The select print mode
	ST_PDB_PRINT_MODE m_CurrentPrintMode;		//!< The current print mode
	long m_SelectedID;							//!< The current print mode

	vector<PrintSpeed*> m_PrintSpeeds;			//!< List of print speeds
	PrintSpeed* m_SelectPrintSpeed;				//!< The select print speed
	long m_CurrentPrintSpeed;					//!< The current print speed

	vector<Tension*> m_Tensions;				//!< List of tensions
	Tension* m_SelectTension;					//!< The select print tension
	long m_CurrentTension;						//!< The current print tension

	bool	isSupport150mpm;
	bool	isSupport100mpm;

	bool	isSupportPrintModeForCoatedOffset;

	//method
	void InitPrintModeList();
	void ReleasePrintModeList();
	void InitSelectPrintMode();

	void InitPrintSpeedList();
	void ReleasePrintSpeedList();
	void InitSelectPrintSpeed();

	void InitTensionList();
	void ReleaseTensionList();
	void InitSelectTension();

private:

	/**
	 *	The function class for searching PrintMode
	**/
	class FindPrintMode
	{
	private:
		string m_ResourceID;
	public:
		FindPrintMode(const string& inResourceID) : m_ResourceID(inResourceID) { }
		inline bool operator()(PrintMode* inPrintMode) { return inPrintMode && m_ResourceID.compare(inPrintMode->Data.RESOURCE_ID) == 0; }
	};

	/**
	 *	The function class for searching PrintSpeed
	**/
	class FindPrintSpeed
	{
	private:
		long m_Value;
	public:
		FindPrintSpeed(long inValue) : m_Value(inValue) { }
		inline bool operator()(PrintSpeed* inPrintSpeed) { return inPrintSpeed && m_Value == inPrintSpeed->Value; }
	};

	/**
	 *	The function class for searching Tension
	**/
	class FindTension
	{
	private:
		long m_Value;
	public:
		FindTension(long inValue) : m_Value(inValue) { }
		inline bool operator()(Tension* inTension) { return inTension && m_Value == inTension->Value; }
	};
};


#endif // !defined(_DATA_PRINT_MODE_H_)
