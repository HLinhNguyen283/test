/*! \file DataPaperType.h
 *  \brief data class: paper type
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

#if !defined(_DATA_PAPER_TYPE_H_)
#define _DATA_PAPER_TYPE_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#ifdef	PRINTER_PROFILE
#include "Stub_PrintProfileIF.h"
#endif
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// ==========================================================================
typedef struct _ST_PAPER_TYPE_DELETE_INFO
{
	string m_paperType;
	bool m_isDelete;
}ST_PAPER_TYPE_DELETE_INFO;

/*! \brief data class: paper type
*/
class CDataPaperType
{
public:
	//method
	CDataPaperType();
	virtual ~CDataPaperType();
	bool Initialize(CDataSystemSetting* inSystemSetting);
	bool Finalize();

	bool IsChanged();
	bool IsHaveValidSelectedItem();
	bool Save();
	void OnCopyJobPrintCondition(std::string &outPaperTypeName);

	bool GetPaperTypeList(long inItemID, const char** outType);
	void SetSelectPaperType(long inItemID);
	bool InitAddPaperTypeDlg();
	bool InitDeletePaperTypeDlg();
	bool GetSelectPaperType(long *outItemID);
	void SetAddPaperTypeName_AddDlg(char* inType);
	const char* GetAddPaperTypeName_AddDlg();
	bool GetPaperTypeList_AddDlg(long inItemID, const char** outType);
	void SetSelectPaperType_AddDlg(long inItemID);
	bool GetSelectPaperType_AddDlg(long* outItemID);
	bool SaveAddPaperTypeDlg();
	bool CancelAddPaperTypeDlg();
	void SetDeletePaperTypeDlg(HWND inDialog);
	bool GetDeletePaperTypeList_DelDlg(long inItemID, const char** outType);
	bool SetDeletePaperType_DelDlg(long inItemID);
	bool DeletePaperTypes_DelDlg();
	bool ExitDeletePaperTypeDlg();

	void UpdateCurrentSelectionPaperType(const char* inPrintConditionName);

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();

	bool IsBlankNotAllowed();

protected:
	//method
	void ResetMember();
	void InitPaperTypeList();
	void InitPaperTypeName_DlgAdd();
	void UpdateSelectionPaperTypeID(string inPaperType);
	void InitPaperTypeList_DlgDelete();

	//event

	//parts

	//member
	SPaperDB_Callbacks* m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;			//!< Callbacks for Job Manager
#ifndef	PRINTER_PROFILE
	SPrintProfile_Callbacks* m_PrintProfile_Callbacks;		//!< Callbacks for Print Profile
#endif

	CDataSystemSetting* m_systemSetting;			//!< Data System setting handle

	string m_printConditionName;					//!< In case the GUI is started from Job List Screen : SectionID
													//!< In case the GUI is started from System Setting Screen : Print Condition Name

	vector<string> m_paperTypeList;					//!< List of system paper type
	string m_currPaperType;							//!< The current paper type
	long m_selectPaperTypeID;						//!< The selected paper type ID

	long m_selectPaperTypeID_DlgAdd;				//!< The selected paper type ID for Add paper type dialog
	string m_paperTypeName_DlgAdd;					//!< Paper type name for add dialog

	vector<ST_PAPER_TYPE_DELETE_INFO> m_listPaperType_DlgDelete;		//!< List of paper type for Delete paper type dialog
	HWND m_hwnd_DlgDelete;							//!< Handle of the Delete paper type dialog
	bool m_isNotSetCurrentPaperType;				//!< Check if Current paper type is set or not

private:
	//member


};


#endif // !defined(_DATA_PAPER_TYPE_H_)
