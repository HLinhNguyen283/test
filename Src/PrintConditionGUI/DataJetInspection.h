/*! \file CDataJetInspection.h
 *  \brief data class: Jet Inspection
 *  \author GCS Tan Ho
 *  \date 2013/12/17 GCS Tan Ho created
*/

#if !defined(_DATA_JET_INSPECTION_H_)
#define _DATA_JET_INSPECTION_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// ==========================================================================

/*! \brief data class: Jet Inspection
*/
class CDataJetInspection
{
public:
	//method
	CDataJetInspection();
	virtual ~CDataJetInspection();
	bool Initialize(CDataSystemSetting* inSystemSetting);
	bool Finalize();

	bool IsChanged();
	bool IsHaveValidSelectedItem();
	bool Save();
	void UpdateCurrentJetInspection(const char* inKeyName);

	bool GetJetInspectionList(long inIndex, const char** outJetInspectionName);
	bool IsJIExistInJIList();
	void SetSelectJetInspection(long inItemID);
	bool GetSelectJetInspection(long* outItemID);
	void SetImageStorage(const bool inIsEnable);
	bool IsImageStorage();

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();

	bool IsBlankNotAllowed();

	bool IsImageStorageEnable();
	bool IsJIDuplicatingEnable();

	/**
		@brief	全画像保管ボタンのコントロール表示状態更新イベント
		@param[in]	inIsLockPrintCondtion : 印刷条件設定のロック状態
		@param[in]	inIsDisablePrintCondition : 操作制限の印刷条件の状態
		@param[out]	outIsShow : 全画像保管ボタンの表示状態
		@param[out]	outIsEnabled : 全画像保管ボタンの有効状態
	*/
	void onView_ImageStorage(
		const bool inIsLockPrintCondtion,
		const bool inIsDisablePrintCondition,
		bool& outIsShow,
		bool& outIsEnabled);

protected:
	//method
	void ResetMember();
	void InitializeJetInspectionList();
	string GetCurrentSettingJetInspection();

	void SetSelectedItemWhenNotSetCurrent();

	bool IsChangedJetInspection();
	bool IsChangedImageStorage();

	//event

	//parts

	//member
	SPaperDB_Callbacks* m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;			//!< Callbacks for Job Manager
	SMainteJetInspection_Callbacks* m_JI_Callbacks;			//!< Callbacks for Jet Inspection

	CDataSystemSetting* m_systemSetting;			//!< Data System setting handle

	string m_printConditionName;					//!< In case the GUI is started from Job List Screen : SectionID
													//!< In case the GUI is started from System Setting Screen : Print Condition Name

	string m_currJetInspection;						//!< The current setting Jet Inspection

	long m_selectedID;								//!< Selected item ID

	vector<string> m_jetInspectionList;				//!< List of Jet Inspection
	bool m_isExistInList;							//!< true : the current setting Jet Inspection of Print Condition Name is existed in the system Jet Inspection list

	bool m_isNotSetCurrentJetInspection;			//!< Check if inspection setting is set or not

	bool m_isImageStorage;							//!< 画像保管(true:On、false:Off)
	bool m_isCurrentImageStorage;					//!< 画像保管の現在値

private:
	//member


};


#endif // !defined(_DATA_JET_INSPECTION_H_)
