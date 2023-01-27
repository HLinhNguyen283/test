/*! \file DataPrintHeadAlignment.h
 *  \brief data class: Print head alingment
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

#if !defined(_DATA_PRINT_HEAD_ALIGNMENT_H_)
#define _DATA_PRINT_HEAD_ALIGNMENT_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#ifdef PRINTER_PROFILE
#include "Stub_PrintProfileIF.h"
#endif
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// ==========================================================================
typedef struct _ST_HEAD_ALIGNMENT_DELETE_INFO
{
	string m_headAlignment;
	bool m_isDelete;
}ST_HEAD_ALIGNMENT_DELETE_INFO;

/*! \brief data class: Print head alignment
*/
class CDataPrintHeadAlignment
{
public:
	//method
	CDataPrintHeadAlignment();
	virtual ~CDataPrintHeadAlignment();
	bool Initialize(CDataSystemSetting* inSystemSetting);
	bool Finalize();

	bool IsChanged();
	bool IsHaveValidSelectedItem();
	bool Save();
	void OnCopyJobPrintCondition(std::string &outPrintHeadAlignmentName);

	void UpdateCurrentSelectionPrintHeadAlignment(const char* inPrintConditionName);

	bool GetPrintHeadAlignmentList(long inItemID, const char** outAlignment);
	void SetSelectPrintHeadAlignment(long inItemID);
	bool InitAddPrintHeadAlignmentDlg();
	bool InitDeletePrintHeadAlignmentDlg();
	bool GetSelectPrintHeadAlignment(long* outItemID);
	void SetAddPrintHeadAlignmentName_AddDlg(char* inAlignment);
	const char* GetAddPrintHeadAlignmentName_AddDlg();
	bool GetPrintHeadAlignmentList_AddDlg(long inItemID, const char** outAlignment);
	void SetSelectPrintHeadAlignment_AddDlg(long inItemID);
	bool GetSelectPrintHeadAlignment_AddDlg(long* outItemID);
	bool SaveAddPrintHeadAlignmentDlg();
	bool CancelAddPrintHeadAlignmentDlg();
	void SetDeletePrintHeadAlignmentDlg(HWND inDialog);
	bool GetDeletePrintHeadAlignmentList_DelDlg(long inItemID, const char** outAlignment);
	bool SetDeletePrintHeadAlignment_DelDlg(long inItemID);
	bool DeletePrintHeadAlignments_DelDlg();
	bool ExitDeletePrintHeadAlignmentDlg();

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();

	bool IsBlankNotAllowed();

protected:
	//method
	void ResetMember();
	void InitPrintHeadAlignmentList();
	void InitPrintHeadAlignmentList_DlgAdd();
	void UpdateSelectionPrintHeadAlignmentID(string inPrintHeadAglignment);
	void InitPrintHeadAlignmentList_DlgDelete();

	//event

	//parts

	//member
	SPaperDB_Callbacks* m_PaperDB_Callbacks;		//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;			//!< Callbacks for Job Manager
#ifndef PRINTER_PROFILE
	SPrintProfile_Callbacks* m_PrintProfile_Callbacks;		//!< Callbacks for Print Profile
#endif

	CDataSystemSetting* m_systemSetting;			//!< Data System setting handle

	string m_printConditionName;					//!< In case the GUI is started from Job List Screen : SectionID
													//!< In case the GUI is started from System Setting Screen : Print Condition Name

	vector<string> m_printHeadAlignmentList;		//!< List of system print head alignment
	string m_currHeadAlignmentName;					//!< The current setting head alignment
	long m_selectedID;								//!< The selected print head alignment ID

	long m_selectedID_DlgAdd;							//!< The selected print head alignment ID for Add print head alignment dialog
	string m_printHeadName_DlgAdd;					//!< Print head alignment for add dialog

	vector<ST_HEAD_ALIGNMENT_DELETE_INFO> m_listPrintHead_DlgDelete;		//!< List of print head alignment for Delete paper type dialog
	HWND m_hwnd_DlgDelete;							//!< Handle of the Delete paper type dialog
	bool m_isNotSetCurrentAlignment;				//!< Check if Current alignment is set or not

private:
	//member


};


#endif // !defined(_DATA_PRINT_HEAD_ALIGNMENT_H_)
