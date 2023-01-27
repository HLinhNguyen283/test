/*! \file DataPrintSettings.h
 *  \brief data print settings class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
#if !defined(_DATA_PRINT_SETTINGS_H_)
#define _DATA_PRINT_SETTINGS_H_

// ==========================================================================
// Include files

#include <windows.h>
#include <vector>
#include <string>
#include "DataSystemSetting.h"
#include "Common.h"
using namespace std;

// ==========================================================================


typedef struct _PrintCondition
{
	long ID;
	char Name[256];
	ST_PDB_PAPER_OPTION Option;
	BOOL Lock;
} PrintCondition;

typedef struct _JobInfo
{
	char Section[256];
	PrintConditionGUI::ST_JOB_INFORMATION Info;
} JobInfo;

class CDataIF;

/*! \brief data interface class
*/
class CDataPrintSettings
{
public:
	//method
	CDataPrintSettings(CDataIF *inParent);
	virtual ~CDataPrintSettings();
	//
	virtual BOOL Initialize(CDataSystemSetting* inSystemSetting);
	virtual void Finalize();

	bool EnterScreen();
	bool ExitScreen();

	JobInfo* GetCurrentJob() const;

	bool GetPrintConditionList(long inItemID, const char** outPrintCondition);
	bool GetPrintConditionListPaperOption(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide);
	bool GetCurrentPrintCondition(bool* outSelecting, const char** outPrintCondition);
	void SetCurrentPrintCondition();
	bool IsPermissionAddPrintCondition();
	bool IsPermissionDeletePrintCondition();
	bool IsPermissionEditJobPrintCondition();
	bool GetSelectPrintCondition(long* outItemID);
	bool SetSelectPrintCondition(long inItemID);
	bool CmpSelectingAndSystemCurrentPrintCondition(bool& outSelecting);

	bool GetLockPrintCondtion();
	void SetLockPrintCondtion(bool inLock);

	// Add Dialog
	bool InitAddPrintConditionDlg();
	const char* GetAddPrintConditionName_AddDlg();
	void SetAddPrintConditionName_AddDlg(char* inName);
	bool GetSelectPrintCondition_AddDlg(long* outItemID);
	void SetSelectPrintCondition_AddDlg(long inItemID);
	bool GetCurrentPrintCondition_AddDlg(bool* outSelecting, const char** outPrintCondition);
	void SetCurrentPrintCondition_AddDlg(PrintCondition* inPrintCondition);
	bool GetPrintConditionList_AddDlg(long inItemID, const char** outName);
	bool GetPrintConditionListPaperOption_AddDlg(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide);
	bool SaveAddPrintSettingDlg();
	bool CancelAddPrintSettingDlg();

	// Delete Dialog
	void SetDeletePrintConditionDlg(HWND inDialog);
	bool InitDeletePrintConditionDlg(bool inDeleted = false);
	bool GetDeletePrintConditionList_DelDlg(long inItemID, const char** outName);
	void SetDeletePrintCondition_DelDlg(long inItemID);
	bool DeletePrintConditions_DelDlg();
	bool ExitDeletePrintConditionDlg();

	void UpdateData(const char* inSelectPrintCondition = NULL);

	const char* GetSelectPrintConditionOrSection();
	bool IsBlankNotAllowed();

	bool GetFlushingPattern(const std::string& inPrintConditionName, ST_JOB_FLUSHING_PATTERN& outFlushingPattern, ST_JOB_FLUSHING_PATTERN_ADD& outFlushingPatternAdd, BOOL& outVerticalAlignment_PP_FLSPTN);

	bool IsMonochromePrint_F();

protected:
	//member
	CDataIF *m_DataIF;									//!< Callbacks for DataIF
	CDataSystemSetting* m_SystemSetting;
	SPaperDB_Callbacks* m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;				//!< Callbacks for Job Manager

	ST_THREAD_INFO m_hThreadExecute;					//!< 印刷条件削除スレッド

	bool m_IsEditSystemPrintCondition;					//!< User can modify the print condition (system)
	bool m_IsEditJobPrintCondition;						//!< User can modify the print condition (job)

	vector<PrintCondition*> m_PrintConditions;			//!< List of print condition

	PrintCondition* m_CurrentPrintCondition;			//!< Current print condition
	PrintCondition* m_SelectPrintCondition;				//!< Select print condition

	JobInfo* m_CurrentJob;								//!< Current Job

	string m_PrintConditionName_AddDlg;					//!< Print condition name in add dialog
	vector<PrintCondition*> m_PrintConditions_AddDlg;	//!< List of print condition in add dialog
	PrintCondition* m_CurrentPrintCondition_AddDlg;		//!< Current print condition in add dialog
	PrintCondition* m_SelectPrintCondition_AddDlg;		//!< Select print condition in add dialog

	HWND m_deletePCDlg;									//!< The handle of delete dialog
	bool m_Deleted_DelDlg;								//!< Deleted any print condition or not
	vector<PrintCondition*> m_PrintConditions_DelDlg;	//!< List of print condition in delete dialog
	vector<PrintCondition*> m_DeleteItems_DelDlg;		//!< List of print condition will be deleted in delete dialog
	bool m_isNotSetCurrentPrintCondition;				//!< Check if Current printcondition is set or not

	//method
	PrintCondition* GetPrintConditionByName(const char* inPrintConditionName);
	PrintCondition* GetSelectPrintCondition();
	PrintCondition* GetCurrentPrintCondition();

	void InitPrintConditionList();
	void ReleasePrintConditionList();
	void InitCurrentPrintCondition();
	void InitSelectPrintCondition(const char* inPrintConditionName = NULL);

	void InitCurrentJob();
	void ReleaseCurrentJob();

	static UINT WINAPI DeletePrintConditionsProc(LPVOID pParam);

	bool CopyJIPhotoInfo_PDB(std::string inPrintConditionName);
	bool GetJIPhotoInfo_PDB(std::string inPrintConditionName, std::string& outFileNameF, std::string& outFileNameB);
	bool DeleteJIPhotoInfo_PDB(std::string inFileNameF, std::string inFileNameB);
	std::string GetJIPhotoDir_PDB(BOOL inDED, DEF_PRINT_SIDE inSide);

	bool DeleteInspectionInfo(std::string inPrintConditionName);
	bool DeleteInspectionInfo_PDB(std::string inPrintConditionName);

private:
	/**
	 *	The function class for searching Print condition
	**/
	class FindPrintCondition
	{
	private:
		string m_Name;
	public:
		FindPrintCondition(const string& inName) : m_Name(inName) { }
		inline bool operator()(PrintCondition* inPrintCondition) { return inPrintCondition && m_Name.compare(inPrintCondition->Name) == 0; }
	};

	UINT _DeletePrintConditionsProc();
};



#endif // !defined(_DATA_PRINT_SETTINGS_H_)
