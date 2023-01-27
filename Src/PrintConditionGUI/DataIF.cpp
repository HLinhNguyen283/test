/*! \file DataIF.cpp
 *  \brief data interface class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "DataIF.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataPrintSettings.h"
#include "DataPrintMode.h"
#include "DataPaperType.h"
#include "DataSystemSetting.h"
#include "DataPrintHeadAlignment.h"
#include "DataPreOverCoat.h"
#include "DataDotgainCurve.h"
#include "DataPaperSettings.h"
#include "DataICCPresetSetting.h"
#include "DataDryerSetting.h"
#include "CommonUiMsg_OP.h"
#include "DataJetInspection.h"
#include "PrintConditionGUI_OP.h"
#include "DataPrintConditionModeJobSetting.h"
#include "IFSPKDef.h"
#include "DataOutputColorSetting.h"
#include "Stub_JobManagerIF.h"
#include "JobDef_PrintCondition_OP.h"
#include "Stub_PrintConditionImporter.h"
#include "UwRwManager_Callbacks.h"


// ==========================================================================
// Local definitions
#define INVALID_THICKNESS_VALUE		(-1)		//!< invalid paper thickness value

/*! \brief constructor
 *  \param[IN] inParent the PrintConditionGUI object
 *  \param[IN] inIsStartForMaintenance True: print condition GUI is started for maintenance by function call, false: Print condition GUI is started by mouse click
*/
CDataIF::CDataIF(CPrintConditionGUI_OP* inParent , bool inIsStartForMaintenance) : m_IsUpdated(false), m_IsSimplexJob(false)
{
	m_Parent = inParent;

	m_systemSetting = new CDataSystemSetting(inIsStartForMaintenance);
	m_PrintSettings = new CDataPrintSettings(this);
	m_PrintMode = new CDataPrintMode();
	m_paperType = new CDataPaperType();
	m_dotgainCurve = new CDataDotgainCurve();
	m_printHeadAlignment = new CDataPrintHeadAlignment();
	m_dryerSetting = new CDataDryerSetting();
	m_jetInspection = new CDataJetInspection();
	m_preOverCoat = new CDataPreOverCoat();
	m_iccPresetSetting = new CDataICCPresetSetting();
	m_PaperSettings = new CDataPaperSettings();
	m_PrintConditionModeJobSetting = new CDataPrintConditionModeJobSetting();
	m_OutputColorSetting = new CDataOutputColorSetting();
	m_DataVariableMagnification = std::unique_ptr<DataVariableMagnification>(new DataVariableMagnification());
	m_DataPrimer = std::unique_ptr<DataPrimer>(new DataPrimer());

	m_IsPushCurrentConditionBtnOpen = false;
	m_IsPushCurrentConditionBtn = false;

	memset(m_heatRollerTemp, 0x00, _countof(m_heatRollerTemp));
	memset(m_dryerFanTemp, 0x00, _countof(m_dryerFanTemp));
	memset(m_precoaterTemp, 0x00, _countof(m_precoaterTemp));
}

/*! \brief destructor
*/
CDataIF::~CDataIF()
{
	if (m_systemSetting)
	{
		delete m_systemSetting;
		m_systemSetting = NULL;
	}

	if (m_PrintSettings)
	{
		delete m_PrintSettings;
		m_PrintSettings = NULL;
	}

	if (m_PrintMode)
	{
		delete m_PrintMode;
		m_PrintMode = NULL;
	}

	if (m_paperType)
	{
		delete m_paperType;
		m_paperType = NULL;
	}

	if (m_printHeadAlignment)
	{
		delete m_printHeadAlignment;
		m_printHeadAlignment = NULL;
	}

	if (m_dotgainCurve)
	{
		delete m_dotgainCurve;
		m_dotgainCurve = NULL;
	}

	if (m_preOverCoat)
	{
		delete m_preOverCoat;
		m_preOverCoat = NULL;
	}

	if (m_iccPresetSetting)
	{
		delete m_iccPresetSetting;
		m_iccPresetSetting = NULL;
	}

	if (m_dryerSetting)
	{
		delete m_dryerSetting;
		m_dryerSetting = NULL;
	}

	if (m_jetInspection)
	{
		delete m_jetInspection;
		m_jetInspection = NULL;
	}

	if (m_PaperSettings)
	{
		delete m_PaperSettings;
		m_PaperSettings = NULL;
	}
	
	if (m_PrintConditionModeJobSetting) {
		delete m_PrintConditionModeJobSetting;
		m_PrintConditionModeJobSetting = NULL;
	}

	if (m_OutputColorSetting) {
		delete m_OutputColorSetting;
		m_OutputColorSetting = NULL;
	}

	if (m_DataVariableMagnification) {
		m_DataVariableMagnification = nullptr;
	}
	
	if (m_DataPrimer) {
		m_DataPrimer = nullptr;
	}
}

/*! \brief initialize
 *  \retval TRUE success
 *  \retval FALSE fail
*/
BOOL CDataIF::Initialize()
{
	if (NULL == m_systemSetting || false == m_systemSetting->Initialize())
		return FALSE;
	if (NULL == m_PrintSettings || FALSE == m_PrintSettings->Initialize(m_systemSetting))
		return FALSE;
	if (NULL == m_PrintMode || FALSE == m_PrintMode->Initialize(m_systemSetting, this))
		return FALSE;
	if (NULL == m_paperType || false == m_paperType->Initialize(m_systemSetting))
		return FALSE;
	if (NULL == m_printHeadAlignment || false == m_printHeadAlignment->Initialize(m_systemSetting))
		return FALSE;
	if (NULL == m_dotgainCurve || false == m_dotgainCurve->Initialize(m_systemSetting, this))
		return FALSE;
	if (NULL == m_preOverCoat || false == m_preOverCoat->Initialize(m_systemSetting))
		return FALSE;
	if (NULL == m_iccPresetSetting || false == m_iccPresetSetting->Initialize(m_systemSetting, m_PrintMode))
		return FALSE;
	if (NULL == m_PaperSettings || FALSE == m_PaperSettings->Initialize(m_systemSetting))
		return FALSE;
	if(NULL == m_dryerSetting || false == m_dryerSetting->Initialize(m_systemSetting, this))
		return FALSE;
	if (NULL == m_jetInspection || false == m_jetInspection->Initialize(m_systemSetting))
		return FALSE;
	if (NULL == m_PrintConditionModeJobSetting || false == m_PrintConditionModeJobSetting->Initialize(this, m_systemSetting))
		return FALSE;
	if (NULL == m_OutputColorSetting || false == m_OutputColorSetting->Initialize(m_systemSetting))
		return FALSE;
	if (nullptr == m_DataVariableMagnification || false == m_DataVariableMagnification->Initialize(m_systemSetting, this))
		return FALSE;
	if (nullptr == m_DataPrimer || false == m_DataPrimer->Initialize(m_systemSetting, this))
		return FALSE;

	return TRUE;
}

/*! \brief finalize
*/
void CDataIF::Finalize()
{
	if (m_PrintSettings)
		m_PrintSettings->Finalize();

	if (m_PrintMode)
		m_PrintMode->Finalize();

	if (m_paperType)
		m_paperType->Finalize();

	if (m_printHeadAlignment)
		m_printHeadAlignment->Finalize();

	if (m_dotgainCurve)
		m_dotgainCurve->Finalize();

	if (m_preOverCoat)
		m_preOverCoat->Finalize();

	if (m_iccPresetSetting)
		m_iccPresetSetting->Finalize();

	if (m_PaperSettings)
		m_PaperSettings->Finalize();

	if (m_dryerSetting)
		m_dryerSetting->Finalize();

	if (m_jetInspection)
		m_jetInspection->Finalize();

	if (m_OutputColorSetting)
		m_OutputColorSetting->Finalize();
}

/*! \brief UI_Initialize時に行う初期化処理.
 *  \return TRUE 成功
 *  \return FALSE 失敗
*/
BOOL CDataIF::UI_Initialize()
{
	return TRUE;
}

/*! \brief UI_Finalize時に行う処理.
*/
void CDataIF::UI_Finalize()
{
}

/*! \brief ウィンドウプロシージャ
 *  \param hWnd ウィンドウハンドル
 *  \param Message メッセージ番号
 *  \param wParam パラメータ
 *  \param lParam パラメータ
 *  \retval DEF_NONE 通知なし
 *  \retval DEF_EXIT_PAGE 画面を閉じる
 *  \retval DEF_UPDATE_STATE ステータス更新を通知する
 *  \retval DEF_UPDATE_VALUE 表示値更新を通知する
*/
long CDataIF::UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case UWM_DELETE_PRINT_CONDITION_UPDATE:	// 印刷条件削除画面の更新
		SetChangeParam();
		break;
	}

	if (m_systemSetting->IsStartedFromJobListScreen()) {
		//refresh control state of PrintConditionGUI items
		if (m_systemSetting->RefreshPrintConditionGUIState(m_PrintSettings->GetSelectPrintConditionOrSection())) {
			SetChangeParam();
		}
	}
	return 0;
}

/*! \brief Get a item of print condition list.
 *  \param inItemID: an item ID(0-N)
 *  \param  outPrintCondition: a pointer to gget a item of print condition list.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintConditionList(long inItemID, const char** outPrintCondition)
{
	return m_PrintSettings->GetPrintConditionList(inItemID, outPrintCondition);
}

/*! \brief Get a paper option of a print condition
 *  \param inItemID: an item ID(0-N)
 *  \param outHole: a pointer to get a ON/OFF of tractor hole(true: ON, false: OFF)
 *  \param outHoleAndPerforation: a pointer to get a ON/OFF of both tractor holes and perforations(true: ON, false: OFF)
 *  \param outPreprintedMark: a pointer to get a ON/OFF of preprinted mark(true: ON, false: OFF)
 *  \param outPreprintedPaper: a pointer to get a ON/OFF of preprinted paper(true: ON, false: OFF)
 *  \param outMarkSide: a pointer to get a side of preprinted mark.(front, back)
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintConditionListPaperOption(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide)
{
	return m_PrintSettings->GetPrintConditionListPaperOption(inItemID, outHole, outHoleAndPerforat, outPreprintedMark, outPreprintedPaper, outMarkSide);
}


/*! \brief Get information of a current print condition.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \param outPrintCondition : a pointer to get a current print condition name.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataIF::GetCurrentPrintCondition(bool* outSelecting, const char** outPrintCondition)
{
	return m_PrintSettings->GetCurrentPrintCondition(outSelecting, outPrintCondition);
}


/*! \brief Compare information of a selecting and system settings current print condition.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataIF::CmpSelectingAndSystemCurrentPrintCondition(bool& outSelecting)
{
	return m_PrintSettings->CmpSelectingAndSystemCurrentPrintCondition(outSelecting);
}


/*! \brief Check if user can add a print condition or not.
 *  \retval true: The adding can do
 *  \retval false: The adding can not do
*/
bool CDataIF::IsPermissionAddPrintCondition()
{
	return m_PrintSettings->IsPermissionAddPrintCondition();
}


/*! \brief Check if user can delete a print condition or not.
 *  \retval true: The deleting can do
 *  \retval false: The deleting can not do
*/
bool CDataIF::IsPermissionDeletePrintCondition()
{
	return m_PrintSettings->IsPermissionDeletePrintCondition();
}


/*! \brief Check if controls in print condition screen should be disabled or not.
 *  \retval true: disabled
 *  \retval false: enabled
*/
bool CDataIF::IsDisablePrintCondition()
{
	bool isDisable = true;
	if(m_systemSetting->GetSystemUserMode() != USER_ID_GENERAL) {
		isDisable = false;
	} else {
		if (m_systemSetting->IsStartedFromJobListScreen()) {
			if (IsPermissionEditJobPrintCondition()) {
				isDisable = false;
			}
		} else {
			if(m_PrintSettings->IsPermissionAddPrintCondition()
				|| m_PrintSettings->IsPermissionDeletePrintCondition()) {
				isDisable = false;
			}
		}
	}
	return !(isDisable == false && m_systemSetting->IsEditablePrintConditionGUI());
}


/*! \brief Check if user can edit job print condition or not.
 *  \retval true: allowed
 *  \retval false: not allowed
*/
bool CDataIF::IsPermissionEditJobPrintCondition()
{
	return m_PrintSettings->IsPermissionEditJobPrintCondition();
}


/*! \brief Set a selection print condition.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintCondition(long inItemID)
{
	if (m_PrintSettings->SetSelectPrintCondition(inItemID))
	{
		UpdateCurrent();
		SetChangeParam();
	}
}


/*! \brief Notify to initialize Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitAddPrintConditionDlg()
{
	return m_PrintSettings->InitAddPrintConditionDlg();
}

/*! \brief Notify to initialize Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitDeletePrintConditionDlg()
{
	return m_PrintSettings->InitDeletePrintConditionDlg();
}


/*! \brief Get a selected print condition
 *  \param outItemID: a pointer to get an item ID(0-N) of a selected print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintCondition(long* outItemID)
{
	return m_PrintSettings->GetSelectPrintCondition(outItemID);
}


/*! \brief Set a current print condition.
*/
void CDataIF::SetCurrentPrintCondition()
{
	m_PrintSettings->SetCurrentPrintCondition();
	SetChangeParam();
	long itemID;
	if (m_PrintSettings->GetSelectPrintCondition(&itemID))
	{
		// #11897: temporarily comment out
		//SUwRwManager_Callbacks cb;
		//if (UnwinderManager_GetCallbacks(&cb))
		//{
		//	cb.OnSetCurrentPrintCondition();
		//}
		//else
		//{
		//	WriteToLogBuf(LOG_DEBUG, "CDataIF::SetCurrentPrintCondition : Failed : UnwinderManager_GetCallbacks");
		//}
	}
}

/*! \brief Set a current print condition.
 *  \param inSetChangeParam: true:call SetChangeParam()/ false: not call
*/
void CDataIF::SetCurrentPrintCondition(bool inSetChangeParam)
{
	m_PrintSettings->SetCurrentPrintCondition();
	if(inSetChangeParam) {
		SetChangeParam();
	}
}

/*! \brief Get ON/OFF of a tractor hole and a perforations
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetOptionTractorHolesAndPerforations()
{
	return m_PaperSettings->GetOptionTractorHolesAndPerforations();
}

/*! \brief Get ON/OFF of a tractor hole
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetOptionTractorHole()
{
	return m_PaperSettings->GetOptionTractorHole();
}

/*! \brief Get ON/OFF of a preprinted mark
 *  \param outMarkSide * a pointer to get a side of the pre-printed mark.
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetOptionPreprintedMark(DEF_PRINT_SIDE* outMarkSide)
{
	return m_PaperSettings->GetOptionPreprintedMark(outMarkSide);
}


/*! \brief Check if a job is running for printing or not.
 *  \retval true: a job is running
 *  \retval false: a job is not running
*/
bool CDataIF::IsJobRunning()
{
	return m_systemSetting->IsJobRunning();
}

/*! \brief Get an item of print mode list.
 *  \param inItemID: an item ID(0-N)
 *  \param outMode: a pointer to get a print mode.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintModeList(long inItemID, const char** outMode)
{
	return m_PrintMode->GetPrintModeList(inItemID, outMode);
}


/*! \brief Set a selection print mode.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintMode(long inItemID)
{
	m_PrintMode->SetSelectPrintMode(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection print mode
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection print mode,
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintMode(long* outItemID)
{
	return m_PrintMode->GetSelectPrintMode(outItemID);
}


/*! \brief Get an item of print speed list
 *  \param inItemID: an item ID(0-N)
 *  \param outSpeed: a pointer to get a print speed
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintSpeedList(long inItemID, const char** outSpeed)
{
	return m_PrintMode->GetPrintSpeedList(inItemID, outSpeed);
}


/*! \brief Set a selection print speed
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintSpeed(long inItemID)
{
	m_PrintMode->SetSelectPrintSpeed(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection print speed
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection print speed.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintSpeed(long* outItemID)
{
	return m_PrintMode->GetSelectPrintSpeed(outItemID);
}

/**
 *	Getting the select print speed value
 *	@retval	the select print speed value
**/
long CDataIF::GetSelectPrintSpeedValue()
{
	return m_PrintMode->GetSelectPrintSpeedValue();
}


/*! \brief Get tension ranges
 *  \param outMin: a pointer to get the minimum of range
 *  \param outMax: a pointer to get the maximum of range
*/
void CDataIF::GetPaperTensionRange(long* outMin, long* outMax)
{
	m_PrintMode->GetPaperTensionRange(outMin, outMax);
}


/*! \brief Set a selection tension
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPaperTension(long inItemID)
{
	m_PrintMode->SetSelectPaperTension(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection tension
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection tension
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPaperTension(long* outItemID)
{
	return m_PrintMode->GetSelectPaperTension(outItemID);
}


/**
 *	Getting the select tension value
 *	@retval	the select tension value
**/
long CDataIF::GetSelectPaperTensionValue()
{
	return m_PrintMode->GetSelectPaperTensionValue();
}


/*! \brief head alignment list
 *  \param inItemID: an item ID(0-N)
 *  \param outAlignment: a pointer to get a head alignment
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintHeadAlignmentList(long inItemID, const char** outAlignment)
{
	return m_printHeadAlignment->GetPrintHeadAlignmentList(inItemID, outAlignment);
}


/*! \brief Set a selection head alignment
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintHeadAlignment(long inItemID)
{
	m_printHeadAlignment->SetSelectPrintHeadAlignment(inItemID);
	SetChangeParam();
}


/*! \brief Notify to initialize Add head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitAddPrintHeadAlignmentDlg()
{
	return m_printHeadAlignment->InitAddPrintHeadAlignmentDlg();
}


/*! \brief Notify to initialize Delete head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitDeletePrintHeadAlignmentDlg()
{
	return m_printHeadAlignment->InitDeletePrintHeadAlignmentDlg();
}


/*! \brief Get a selection head alignment
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection head alignment
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintHeadAlignment(long* outItemID)
{
	return m_printHeadAlignment->GetSelectPrintHeadAlignment(outItemID);
}


/*! \brief Get an item of paper type list
 *  \param inItemID: an item ID(0-N)
 *  \param outType: a pointer to get a paper type
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPaperTypeList(long inItemID, const char** outType)
{
	return m_paperType->GetPaperTypeList(inItemID, outType);
}


/*! \brief Set a selection paper type
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPaperType(long inItemID)
{
	m_paperType->SetSelectPaperType(inItemID);
	SetChangeParam();
}


/*! \brief Notify to initialize Add paper type dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitAddPaperTypeDlg()
{
	return m_paperType->InitAddPaperTypeDlg();
}


/*! \brief Notify to initialize Delete paper type dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitDeletePaperTypeDlg()
{
	return m_paperType->InitDeletePaperTypeDlg();
}


/*! \brief Get a selection paper type
 *  \param outItemID: a pointer to get the item ID(0-N) of a paper type
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPaperType(long *outItemID)
{
	return m_paperType->GetSelectPaperType(outItemID);
}


/*! \brief Get a range of precoat settings
 *  \param  outMin: a pointer to get the minimum of range
 *  \param  outMax: a pointer to get the maximum of range
*/
void CDataIF::GetPrecoatRange(long* outMin, long* outMax)
{
	m_preOverCoat->GetPrecoatRange(outMin, outMax);
}


/*! \brief Set a selection precoat
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrecoat(long inPrinterID, long inItemID)
{
	m_preOverCoat->SetSelectPrecoat(inPrinterID, inItemID);
	SetChangeParam();
}


/*! \brief Check if the printer has a precoater unit or not.
 *  \retval true: the printer has it
 *  \retval false: the printer has not it
*/
bool CDataIF::IsExistPrecoater()
{
	return m_systemSetting->IsExistPrecoater();
}


/*! \brief Check if the system is running with DED
 *  \retval true: DED
 *  \retval false: SES
*/
bool CDataIF::IsDED()
{
	return m_systemSetting->IsDED();
}


/*! \brief Get a selection precoat
 *  \param inPrinterID: a printer ID(1-2)
 *  \param outItemID: a pointer to get an item ID(0-N) of a selection precoat
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrecoat(long inPrinterID, long* outItemID)
{
	return m_preOverCoat->GetSelectPrecoat(inPrinterID, outItemID);
}


/*! \brief Get ranges of overcoat
 *  \param  outMin: a pointer to get the minimum of range
 *  \param  outMax: a pointer to get the maximum of range
*/
void CDataIF::GetOvercoatRange(long* outMin, long* outMax)
{
	m_preOverCoat->GetOvercoatRange(outMin, outMax);
}


/*! \brief Set a selection overcoat
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectOvercoat(long inPrinterID, long inItemID)
{
	m_preOverCoat->SetSelectOvercoat(inPrinterID, inItemID);
	SetChangeParam();
}


/*! \brief Check if the printer has an overcoater unit or not
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it
 *  \retval false: the printer has not it
*/
bool CDataIF::IsExistOvercoater(long inPrinterID)
{
	return m_systemSetting->IsExistOvercoater(inPrinterID);
}


/*! \brief Get a selection overcoat
 *  \param inPrinterID: a printer ID(1-2)
 *  \param outItemID: a pointer to get an item ID(0-N) of a selection overcoat
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectOvercoat(long inPrinterID, long* outItemID)
{
	return m_preOverCoat->GetSelectOvercoat(inPrinterID, outItemID);
}


/*! \brief Get an item of dryer settings list for printing
 *  \param inItemID: an item ID(0-N)
 *  \param outDryer: a pointer to get a dryer settings
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintDryerSettingList(long inItemID, const char** outDryer)
{
	return m_dryerSetting->GetPrintDryerSettingList(inItemID, outDryer);
}


/*! \brief Notify to initialize Add dryer settings dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitAddDryerSettingDlg()
{
	return m_dryerSetting->InitAddDryerSettingDlg();
}


/*! \brief Notify to initialize Delete dryer settings dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitDeleteDryerSettingDlg()
{
	return m_dryerSetting->InitDeleteDryerSettingDlg();
}


/*! \brief Set a selection dryer settings
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintDryerSetting(long inItemID)
{
	m_dryerSetting->SetSelectPrintDryerSetting(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection dryer settings
 *  \param outItemID: a pointer to get an item ID(0-N) of a selection dryer settings
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintDryerSetting(long* outItemID)
{
	return m_dryerSetting->GetSelectPrintDryerSetting(outItemID);
}


/*! \brief Get an item of unit list for feeding paper
 *  \param inItemID: an item ID(0-N)
 *  \param outUnit: a pointer to get an unit for feeding
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPaperFeedUnitList(long inItemID, const char** outUnit)
{
	return m_PaperSettings->GetPaperFeedUnitList(inItemID, outUnit);
}


/*! \brief Get a unit of length that the system uses.
 *  \param outUnit: a pointer to get an unit ID(mm,inch,point)
 *  \retval a text of an unit (mm,inch,point)
*/
const char* CDataIF::GetSystemUnitLength(DEF_LENGTH_UNIT* outUnit)
{
	return m_systemSetting->GetSystemUnitLength(outUnit);
}


/*! \brief Set a paper width
 *  \param inWidth: a paper width
*/
void CDataIF::SetPaperWidth(long inWidth)
{
	m_PaperSettings->SetPaperWidth(inWidth);
	SetChangeParam();
}

/*! \brief Set if the feeding length of paper set or not.
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetUsePaperFeedLengthSetting(bool inUse)
{
	m_PaperSettings->SetUsePaperFeedLengthSetting(inUse);
	SetChangeParam();
}


/*! \brief Set a feeding length of paper
 *  \param inLength: a feeding length of paper
*/
void CDataIF::SetPaperFeedLength(long inLength)
{
	m_PaperSettings->SetPaperFeedLength(inLength);
	SetChangeParam();
}


/*! \brief Set a selection unit for feeding paper
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPaperFeedUnit(long inItemID)
{
	m_PaperSettings->SetSelectPaperFeedUnit(inItemID);
	SetChangeParam();
}


/*! \brief Get if the feeding length of paper set or not.
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetUsePaperFeedLengthSetting()
{
	return m_PaperSettings->GetUsePaperFeedLengthSetting();
}


/*! \brief Get a paper width
 *  \retval a paper width
*/
long CDataIF::GetPaperWidth()
{
	return m_PaperSettings->GetPaperWidth();
}


/*! \brief Get a feeding length of paper
 *  \retval a feeding length of paper
*/
long CDataIF::GetPaperFeedLength()
{
	return m_PaperSettings->GetPaperFeedLength();
}


/*! \brief Get a selection unit for feeding paper
 *  \param outItemID: a pointer to g et an item ID(0-N) of unit for feeding length
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPaperFeedUnit(long* outItemID)
{
	return m_PaperSettings->GetSelectPaperFeedUnit(outItemID);
}


/*! \brief Set a paper thickness
 *  \param inThickness: a paper thickness
*/
void CDataIF::SetPaperThickness(long inThickness)
{
	m_PaperSettings->SetPaperThickness(inThickness);
	SetChangeParam();
}


/*! \brief Get a paper thickness
 *  \retval a paper thickness
*/
long CDataIF::GetPaperThickness()
{
	return m_PaperSettings->GetPaperThickness();
}


/*! \brief Set ON/OFF of a preprinted mark
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetOptionPreprintedMark(bool inUse)
{
	m_PaperSettings->SetOptionPreprintedMark(inUse);
	SetChangeParam();
}


/*! \brief Set ON/OFF of a tractor hole
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetOptionTractorHole(bool inUse)
{
	m_PaperSettings->SetOptionTractorHole(inUse);
	SetChangeParam();
}


/*! \brief Set ON/OFF of a tractor hole and a perforations
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetOptionTractorholesAndPerforations(bool inUse)
{
	m_PaperSettings->SetOptionTractorholesAndPerforations(inUse);
	SetChangeParam();
}


/*! \brief Set a interval of perforation
 *  \param inInterval: a interval of perforation
*/
void CDataIF::SetOptionPerforationInterval(long inInterval)
{
	m_PaperSettings->SetOptionPerforationInterval(inInterval);
	SetChangeParam();
}


/*! \brief Notify to initialize Select preprinted mark side dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitSelectPreprintedMarkSideDlg()
{
	return m_PaperSettings->InitSelectPreprintedMarkSideDlg();
}


/*! \brief Set an offset of preprinted mark
 *  \param inOffset: an offset of preprinted mark
*/
void CDataIF::SetOptionPreprintedMarkOffset(long inOffset)
{
	m_PaperSettings->SetOptionPreprintedMarkOffset(inOffset);
	SetChangeParam();
}

/*! \brief Set ON/OFF of a preprinted paper
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetOptionPreprintedPaper(bool inUse)
{
	m_PaperSettings->SetOptionPreprintedPaper(inUse);
	SetChangeParam();
}


/*! \brief Get a interval of perforation
 *  \retval a interval of perforation
*/
long CDataIF::GetOptionPerforationInterval()
{
	return m_PaperSettings->GetOptionPerforationInterval();
}


/*! \brief Get an offset of preprinted mark
 *  \retval an offset of preprinted mark
*/
long CDataIF::GetOptionPreprintedMarkOffset()
{
	return m_PaperSettings->GetOptionPreprintedMarkOffset();
}


/*! \brief Get ON/OFF of a preprinted paper
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetOptionPreprintedPaper()
{
	return m_PaperSettings->GetOptionPreprintedPaper();
}

/*! \brief Set a selection side to display a preview image that both a paper and paper options are drawn.
 *  \param inSide: a selection side(front, back)
*/
void CDataIF::SetPaperPreviewSide(DEF_PRINT_SIDE inSide)
{
	m_PaperSettings->SetPaperPreviewSide(inSide);
}


/*! \brief Get a selection side to display a preview image that both a paper and paper options are drawn.
 *  \retval a selection side(front, back)
*/
DEF_PRINT_SIDE CDataIF::GetPaperPreviewSide()
{
	return m_PaperSettings->GetPaperPreviewSide();
}


/*! \brief Get a preview image that both a paper and paper options are drawn.
 *  \param inSide: a side to get a imgae(front/back)
 *  \param outImage: a pointer to get a preview image object
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPaperPreview(DEF_PRINT_SIDE inSide, HBITMAP* outImage)
{
	return m_PaperSettings->GetPaperPreview(inSide, outImage);
}


/*! \brief Notify to initialize Tone curve settings dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::InitToneSettingDlg()
{
//	return ToneSettings_ShowDialog((char*)m_PrintSettings->GetSelectPrintConditionOrSection(), GetGenSetupDialog()) == TRUE ? true : false;
	return ToneSettings_ShowDialog((char*)m_PrintSettings->GetSelectPrintConditionOrSection(), GetPostMessageWindow()) == TRUE ? true : false;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outIsShowCopyMessage : コピー確認ダイアログ表示有無(true: 表示された, false: 表示されていない)
*/
void CDataIF::OnCopyJobPrintCondition(bool &outIsShowCopyMessage)
{
	//システムの印刷条件を表示しているかチェックする
	if (IsStartFromSystemSetting()) {
		// システムの印刷条件の場合、処理しない
		outIsShowCopyMessage = false;
		return;
	}

	//各データクラスからコピー対象のデータを収集する
	PrintConditionGUI::ST_COPY_TARGET_JOB_PRINT_CONDITION copyTargetJobPrintCondition = {0};

	// 印刷速度, 印刷速度単位, テンション
	m_PrintMode->OnCopyJobPrintCondition(copyTargetJobPrintCondition.printSpeedValue, copyTargetJobPrintCondition.printSpeedUnit, copyTargetJobPrintCondition.paperTensionValue);

	// 段差補正名
	m_printHeadAlignment->OnCopyJobPrintCondition(copyTargetJobPrintCondition.printHeadAlignmentName);

	// 用紙種類
	m_paperType->OnCopyJobPrintCondition(copyTargetJobPrintCondition.paperTypeName);

	// 乾燥温度
	m_dryerSetting->OnCopyJobPrintCondition(copyTargetJobPrintCondition.dryerSettings);

	// 用紙設定各種
	// -> 用紙幅, 紙送り, 紙厚
	// -> 紙オプション (パンチ有無, パンチ＋ミシン有無, 事前印刷マーク有無, 事前印刷マーク印字面, 事前印刷マークオフセット, 事前印刷用紙有無)
	// -> DNS
	m_PaperSettings->OnCopyJobPrintCondition(copyTargetJobPrintCondition.paperSettings);

	// プライマー
	m_DataPrimer->onCopyJobPrintCondition(copyTargetJobPrintCondition.isPrimer);

	//ジョブ管理にジョブ印刷条件設定コピー依頼を行う
	Stub_JobManagerIF::OnBeforeSaveJobPrintCondition(copyTargetJobPrintCondition, outIsShowCopyMessage);

}

/*! \brief Save print condition settings
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::SavePrintCondition()
{
	bool selecting = false;
	const char* printConditionName = NULL;
	bool hasCurrentPrintCondition = m_PrintSettings->GetCurrentPrintCondition(&selecting, &printConditionName);
	// Update paper thickness to value notified from UW
	if (IsStartFromSystemSetting()) {
		CIni_PrinterDescriptor iniPrinterDescriptor;
		iniPrinterDescriptor.Initialize(TRUE);
		if (iniPrinterDescriptor.getUnwinderOption() == 1) {
			if (selecting) {
				// #11897: temporarily comment out
				//SUwRwManager_Callbacks unwinderManager_Callbacks;
				//if (UnwinderManager_GetCallbacks(&unwinderManager_Callbacks)) {
				//	bool isReflectUWThickness = false;
				//	long currentThickness = m_PaperSettings->GetPaperThickness();
				//	unwinderManager_Callbacks.CheckUpdateUWPaperThicknessForCurrentPrintCondition(currentThickness, isReflectUWThickness);
				//	if (isReflectUWThickness) {
				//		long UWThickness = INVALID_THICKNESS_VALUE;
				//		unwinderManager_Callbacks.GetUWPaperThickness(UWThickness);
				//		if (UWThickness != INVALID_THICKNESS_VALUE) {
				//			m_PaperSettings->SetPaperThickness(UWThickness);
				//		}
				//		else {
				//			WriteToLogBuf(LOG_DEBUG, "CDataIF::SavePrintCondition() error - Invalid UW paper thickness value.");
				//		}
				//	}
				//}
				//else {
				//	WriteToLogBuf(LOG_DEBUG, "CDataIF::SavePrintCondition() error - cannot get unwinderManager Callbacks.");
				//}
			}
		}
		iniPrinterDescriptor.Finalize();
	}

	//Save value
	if (   !m_PrintMode->Save()					// Save value for printmode, printspeed, tension
		|| !m_paperType->Save()					// Save value for Paper type
		|| !m_preOverCoat->Save()				// Save value for Precoat and Overcoat
		|| !m_printHeadAlignment->Save()		// Save value for Head alignment
		|| !m_dryerSetting->SaveValues()		// Save value for Dryer settings
		|| !m_iccPresetSetting->SaveValues()	// Save value for ICC Preset
		|| !m_PaperSettings->Save()				// Save value for paper settings
		|| !m_jetInspection->Save()				// Save value for Jet Inspection
		|| !m_dotgainCurve->Save()				// Save value for dotgain curve
		|| !m_OutputColorSetting->Save()		// Save value for Output color mode
		|| !m_DataPrimer->Save())				// Save value for Primer
	{
		return false;
	}

	if (IsStartFromSystemSetting())
	{
		bool isSaveSuccess = m_systemSetting->GetPaperDB_Callbacks()->PDB_CommitPaperDB();
		SetChangeParam();
		if (isSaveSuccess && hasCurrentPrintCondition)
		{
			if (selecting)
			{
				//SUwRwManager_Callbacks cb;
				//if (UnwinderManager_GetCallbacks(&cb))
				//{
				//	cb.OnUpdateCurrentPrintCondition();
				//}
				//else
				//{
				//	WriteToLogBuf(LOG_DEBUG, "CDataIF::SavePrintCondition : Failed : UnwinderManager_GetCallbacks");
				//}
			}
		}
		return isSaveSuccess;
	}
	else
	{
		const char* section = m_PrintSettings->GetSelectPrintConditionOrSection();
		if (section)
		{
			m_systemSetting->GetJobManager_Callbacks()->JM_SetPrintConditionEditFlag(section, TRUE);
			if (PLUGERR_NONE == m_systemSetting->GetJobManager_Callbacks()->JM_SaveJobFile(section))
			{
				/**
					note:
					If user changed the overcoat value, JobManager will send it information to EQ Center.
					But EQ Center possible returns an error (because EQ Client may be editing the same job).
					In this case, JobManager reset the overcoat value to previous saved value, so PrintConditionGUI need apply this.
				*/

				m_preOverCoat->EnterScreen(section);

				m_IsUpdated = true;
				SetChangeParam();
				return true;
			}
		}
	}
	return false;
}


/*! \brief Check if print condition settings can save or not
 *  \retval true: it can save
 *  \retval false: it can not save
*/
bool CDataIF::IsAbleSavePrintSetting()
{
	// Check all settings are set
	if (   !m_paperType->IsHaveValidSelectedItem()
		|| !m_dryerSetting->IsHaveValidSelectedItem()
		|| !m_jetInspection->IsHaveValidSelectedItem()
		|| !m_printHeadAlignment->IsHaveValidSelectedItem()
		|| !m_iccPresetSetting->IsHaveValidSelectedItem())
	{
		return false;
	}
	return !IsSavedPrintSetting();
}

/*! \brief Set lock/unlock to edit print condition settings.
 *  \param inLock (true: lock, false: unlock)
*/
void CDataIF::SetLockPrintCondtion(bool inLock)
{
	m_PrintSettings->SetLockPrintCondtion(inLock);
	SetChangeParam();
}

/*! \brief Get an user mode of the system
 *  \retval an user mode
*/
DEF_USER_MODE_ID CDataIF::GetSystemUserMode()
{
	return m_systemSetting->GetSystemUserMode();
}


/*! \brief Get lock/unlock to edit print condition settings
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetLockPrintCondtion()
{
	return m_PrintSettings->GetLockPrintCondtion();
}


/*! \brief Check if print condition settings saved or not
 *  \retval true: it saved
 *  \retval false: it does not save
*/
bool CDataIF::IsSavedPrintSetting()
{
	//Check if changes are made or not
	if (   m_PrintMode->IsChanged()								// Print mode is changed
		|| m_paperType->IsChanged()								// Paper type is changed
		|| m_preOverCoat->IsChanged()							// Precoat and overcoat is changed
		|| m_printHeadAlignment->IsChanged()					// Printhead alignment is changed
		|| m_iccPresetSetting->IsSelectedICCPresetChanged()		// ICC Preset is changed
		|| m_dotgainCurve->IsDotgainCurveChanged() 				// Dotgain curve is changed
		|| m_PaperSettings->IsChanged()							// Paper Setting is changed
		|| m_jetInspection->IsChanged()							// Jet Inspection is changed
		|| m_dryerSetting->IsDryerSettingChanged()				// Dryer setting is changed
		|| m_OutputColorSetting->IsChanged()					// Output color mode is changed
		|| m_DataPrimer->IsChanged())							// Primer is changed
	{
		return false;
	}
	//Not changed
	return true;
}
/**
 *	Check the user change the paper size or not
 *	@retval	true : change
 *	@retval	false : not change
**/
bool CDataIF::IsPaperSizeChanged(){
	return m_PaperSettings->IsPaperSizeChanged();
}
/**
 *	Check the the JI option is enable or not
 *	@retval	true : enable
 *	@retval	false : disable
 *	
**/
bool CDataIF::IsJIOptionEnable()
{
	return m_systemSetting->IsJIOptionEnable();
}
/**
 *	Notify to enter PrintCondition screen
 *	@retval	true: succeed
 *	@retval	false: failed
**/
bool CDataIF::EnterScreen()
{
	if (!m_systemSetting->EnterScreen())
		return false;
	if (!m_PrintSettings->EnterScreen())
		return false;

	const char* printConditionOrSection = m_PrintSettings->GetSelectPrintConditionOrSection();

	if (printConditionOrSection)
	{
		m_IsSimplexJob = false;
		//Check if it starts from Job list or not
		if (m_systemSetting->IsStartedFromJobListScreen()){
			bool isSetupEditingData = false;	//Don't refer to the data of the job for setup editing in this plugin.
			DEF_PRINT_SIDE sideOfJob = DEF_PRINT_FRONT;			//Printing side of job
			if (!m_systemSetting->GetJobManager_Callbacks()->JM_GetSidePrint(printConditionOrSection, sideOfJob, isSetupEditingData)){
				return false;
			}
			//Check the current job is simplex job or not
			if (DEF_PRINT_FRONT == sideOfJob){
				m_IsSimplexJob = true;
			}
		}

		// 2017.12.04
		// 出力色モードによって印刷モードを切り替えるのでOutputColorSettingをPrintModeより先にUpdateする
		m_OutputColorSetting->EnterScreen(printConditionOrSection);
		m_PrintMode->EnterScreen(printConditionOrSection);
		m_paperType->EnterScreen(printConditionOrSection);
		m_preOverCoat->EnterScreen(printConditionOrSection);
		m_printHeadAlignment->EnterScreen(printConditionOrSection);
		m_jetInspection->EnterScreen(printConditionOrSection);
		m_dotgainCurve->EnterScreen(printConditionOrSection);
		m_dryerSetting->EnterScreen(printConditionOrSection);
		m_PaperSettings->EnterScreen(printConditionOrSection);
		m_iccPresetSetting->EnterScreen(printConditionOrSection);
		m_DataVariableMagnification->EnterScreen(printConditionOrSection);
		m_DataPrimer->EnterScreen(printConditionOrSection);
	}

	m_IsUpdated = false;

	// IBT log for open screen
	char *tmpSegment = (TRUE == m_systemSetting->IsDED())? IBT_SEG_JD2 : IBT_SEG_JD;
	if(FALSE == IBTLogManager_WriteToIBTLog(true, tmpSegment, IBT_SUBJECT_STAT, IBT_EVENT_OPEN, _T("PrintConditionGUI.dll")))
	{
		char lstr[256] = {0};
		_snprintf_s(lstr, _TRUNCATE, "WriteIBTLog - Open PrintConditionGUI.dll : Failed to log!!!");
		WriteToLogBuf(LOG_DEBUG, lstr);
	}

	return true;
}

/*! \brief Notify to exit Print condition screen
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitScreen()
{
	m_systemSetting->ExitScreen();
	m_PrintSettings->ExitScreen();

	m_PrintMode->ExitScreen();
	m_paperType->ExitScreen();
	m_preOverCoat->ExitScreen();
	m_printHeadAlignment->ExitScreen();
	m_jetInspection->ExitScreen();
	m_dotgainCurve->ExitScreen();
	m_dryerSetting->ExitScreen();
	m_PaperSettings->ExitScreen();
	m_iccPresetSetting->ExitScreen();
	m_OutputColorSetting->ExitScreen();

	// IBT log for close screen
	char *tmpSegment = (TRUE == m_systemSetting->IsDED())? IBT_SEG_JD2 : IBT_SEG_JD;
	if(FALSE == IBTLogManager_WriteToIBTLog(true, tmpSegment, IBT_SUBJECT_STAT, IBT_EVENT_CLOSE, _T("PrintConditionGUI.dll")))
	{
		char lstr[256] = {0};
		_snprintf_s(lstr, _TRUNCATE, "WriteIBTLog - Close PrintConditionGUI.dll : Failed to log!!!");
		WriteToLogBuf(LOG_DEBUG, lstr);
	}

	return true;
}

/*! \brief Set a print condition name to add it at Add print condition dialog.
 *  \param inName: a print condition name
*/
void CDataIF::SetAddPrintConditionName_AddDlg(char* inName)
{
	m_PrintSettings->SetAddPrintConditionName_AddDlg(inName);
	SetChangeParam();
}


/*! \brief Get a print condition name to add it at Add print condition dialog.
 *  \retval a print condition name
*/
const char* CDataIF::GetAddPrintConditionName_AddDlg()
{
	return m_PrintSettings->GetAddPrintConditionName_AddDlg();
}


/*! \brief Get a item of print condition list at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outName: a pointer to get a print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintConditionList_AddDlg(long inItemID, const char** outName)
{
	return m_PrintSettings->GetPrintConditionList_AddDlg(inItemID, outName);
}


/*! \brief Get a paper option of a print condition at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outHole: a pointer to get a ON/OFF of tractor hole(true: ON, false: OFF)
 *  \param outHoleAndPerforation: a pointer to get a ON/OFF of both tractor holes and perforations(true: ON, false: OFF)
 *  \param outPreprintedMark: a pointer to get a ON/OFF of preprinted mark(true: ON, false: OFF)
 *  \param outPreprintedPaper: a pointer to get a ON/OFF of preprinted paper(true: ON, false: OFF)
 *  \param outMarkSide: a pointer to get a side of preprinted mark.(front, back)
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintConditionListPaperOption_AddDlg(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide)
{
	return m_PrintSettings->GetPrintConditionListPaperOption_AddDlg(inItemID, outHole, outHoleAndPerforat, outPreprintedMark, outPreprintedPaper, outMarkSide);
}


/*! \brief Get information of a current print condition at Add print condition dialog.
 *  \param outSelecting : a pointer to get it, whether a selection print condition on the screen is the same as a current print condition of the printer.
 *  \param outPrintCondition : a pointer to get a current print condition name.
 *  \retval true : The printer has a current print condition.
 *  \retval false : The printer does not have a current print condition.
*/
bool CDataIF::GetCurrentPrintCondition_AddDlg(bool* outSelecting, const char** outPrintCondition)
{
	return m_PrintSettings->GetCurrentPrintCondition_AddDlg(outSelecting, outPrintCondition);
}


/*! \brief Set a selection print condition at Add print condition dialog.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintCondition_AddDlg(long inItemID)
{
	m_PrintSettings->SetSelectPrintCondition_AddDlg(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection print condition at Add print condition dialog.
 *  \param outItemID: a pointer to get an item ID(0-N) of a selectio nprint condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintCondition_AddDlg(long* outItemID)
{
	return m_PrintSettings->GetSelectPrintCondition_AddDlg(outItemID);
}


/*! \brief Save the settings of Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::SaveAddPrintSettingDlg()
{
	bool result = false;
	if (m_PrintSettings->SaveAddPrintSettingDlg())
	{
		UpdateCurrent();
		result = true;
	}
	SetChangeParam();
	return result;
}


/*! \brief Cancel to save the settings of Add print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::CancelAddPrintSettingDlg()
{
	bool result = m_PrintSettings->CancelAddPrintSettingDlg();
	SetChangeParam();
	return result;
}


/*! \brief Register window handle of Delete print condition dialog.
 *  \param inDialog: the dialog handle.
*/
void CDataIF::SetDeletePrintConditionDlg(HWND inDialog)
{
	m_PrintSettings->SetDeletePrintConditionDlg(inDialog);
}


/*! \brief Get an item of print condition list at Delete print condition dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outName: a pointer to get a print condition.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetDeletePrintConditionList_DelDlg(long inItemID, const char** outName)
{
	return m_PrintSettings->GetDeletePrintConditionList_DelDlg(inItemID, outName);
}


/*! \brief Register items of print condition list to delete at Delete print condition dialog.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetDeletePrintCondition_DelDlg(long inItemID)
{
	m_PrintSettings->SetDeletePrintCondition_DelDlg(inItemID);
}


/*! \brief Delete the registered items to delete at Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::DeletePrintConditions_DelDlg()
{
	return m_PrintSettings->DeletePrintConditions_DelDlg();
}


/*! \brief Notify to exit Delete print condition dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitDeletePrintConditionDlg()
{
	bool result = false;
	if (m_PrintSettings->ExitDeletePrintConditionDlg())
	{
		UpdateCurrent();
		result = true;
	}
	SetChangeParam();
	return result;
}

/**
 *	The current job setting is updated or not
 *	@retval	true: updated
 *	@retval	false: not updated
 *	@note Notes
 *		For req. 110
**/
bool CDataIF::IsUpdatedCurrentPrintSetting()
{
	return m_IsUpdated;
}

/*! \brief Set a addition name of print head alignment at Add print head alignment dialog.
 *  \param inAlignment: a name of print head alignment
*/
void CDataIF::SetAddPrintHeadAlignmentName_AddDlg(char* inAlignment)
{
	m_printHeadAlignment->SetAddPrintHeadAlignmentName_AddDlg(inAlignment);
	SetChangeParam();
}


/*! \brief Get a addition name of print head alignment at Add print head alignment dialog.
 *  \retval a name of print head alignment
*/
const char* CDataIF::GetAddPrintHeadAlignmentName_AddDlg()
{
	return m_printHeadAlignment->GetAddPrintHeadAlignmentName_AddDlg();
}


/*! \brief Get an item of print head alignment list at Add print head alignment dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outAlignment: a pointer to get a print head alignment
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPrintHeadAlignmentList_AddDlg(long inItemID, const char** outAlignment)
{
	return m_printHeadAlignment->GetPrintHeadAlignmentList_AddDlg(inItemID, outAlignment);
}

/*! \brief Set a selection print head alignment at Add print head alignment dialog.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPrintHeadAlignment_AddDlg(long inItemID)
{
	m_printHeadAlignment->SetSelectPrintHeadAlignment_AddDlg(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection print head alignment at Add print head alignment dialog.
 *  \param outItemID: a pointer to get an item ID(0-N) of print head alignment list
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPrintHeadAlignment_AddDlg(long* outItemID)
{
	return m_printHeadAlignment->GetSelectPrintHeadAlignment_AddDlg(outItemID);
}


/*! \brief Save the settings of Add print head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::SaveAddPrintHeadAlignmentDlg()
{
	bool retval = m_printHeadAlignment->SaveAddPrintHeadAlignmentDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Cancel to save the settings of Add print head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::CancelAddPrintHeadAlignmentDlg()
{
	bool retval = m_printHeadAlignment->CancelAddPrintHeadAlignmentDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Register the dialog handle of Delete print head alignment dialog
 *  \param inDialog: the dialog handle.
*/
void CDataIF::SetDeletePrintHeadAlignmentDlg(HWND inDialog)
{
	m_printHeadAlignment->SetDeletePrintHeadAlignmentDlg(inDialog);
	SetChangeParam();
}


/*! \brief Get an item of deletion list of print head alignment at Delete print head alignment dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outAlignment: a pointer to get a print head alignment
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetDeletePrintHeadAlignmentList_DelDlg(long inItemID, const char** outAlignment)
{
	return m_printHeadAlignment->GetDeletePrintHeadAlignmentList_DelDlg(inItemID, outAlignment);
}


/*! \brief Register a print head alignment list to delete at Delete print head alignment dialog.
 *  \param inItemID: an item ID(0-N)
*/
bool CDataIF::SetDeletePrintHeadAlignment_DelDlg(long inItemID)
{
	return m_printHeadAlignment->SetDeletePrintHeadAlignment_DelDlg(inItemID);
}


/*! \brief Delete the registered items to delete at Delete print head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::DeletePrintHeadAlignments_DelDlg()
{
	bool retval = m_printHeadAlignment->DeletePrintHeadAlignments_DelDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Notify to exit Delete print head alignment dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitDeletePrintHeadAlignmentDlg()
{
	return m_printHeadAlignment->ExitDeletePrintHeadAlignmentDlg();
}

/*! \brief Set a paper type name to add it at Add paper type dialog.
 *  \param inType: a paper type
*/
void CDataIF::SetAddPaperTypeName_AddDlg(char* inType)
{
	m_paperType->SetAddPaperTypeName_AddDlg(inType);
	SetChangeParam();
}


/*! \brief Get a paper type name to add it at Add paper type dialog.
 *  \retval a paper type
*/
const char* CDataIF::GetAddPaperTypeName_AddDlg()
{
	return m_paperType->GetAddPaperTypeName_AddDlg();
}


/*! \brief Get a item of paper type list at Add paper type dialog.
 *  \param inItemID: an item ID(0-N)
 *  \param outType: a pointer to g et a  paper type
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetPaperTypeList_AddDlg(long inItemID, const char** outType)
{
	return m_paperType->GetPaperTypeList_AddDlg(inItemID, outType);
}

/*! \brief Set a selection paper type
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectPaperType_AddDlg(long inItemID)
{
	m_paperType->SetSelectPaperType_AddDlg(inItemID);
	SetChangeParam();
}


/*! \brief Get a selection paper type
 *  \param outItemID: a pointer to get an item ID(0-N) of paper type list
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectPaperType_AddDlg(long* outItemID)
{
	return m_paperType->GetSelectPaperType_AddDlg(outItemID);
}


/*! \brief Save the settings of Add paper type dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::SaveAddPaperTypeDlg()
{
	bool retval = m_paperType->SaveAddPaperTypeDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Cancel to save the settings of Add paper type dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::CancelAddPaperTypeDlg()
{
	bool retval = m_paperType->CancelAddPaperTypeDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Set the dialog handle of Delete paper type dialog
 *  \param inDialog: the dialog handle 
*/
void CDataIF::SetDeletePaperTypeDlg(HWND inDialog)
{
	m_paperType->SetDeletePaperTypeDlg(inDialog);
}


/*! \brief Get an item of paper type list to delete at Delete paper type dialog
 *  \param inItemID: an item ID(0-N)
 *  \param outType: a pointer to get a paper type
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetDeletePaperTypeList_DelDlg(long inItemID, const char** outType)
{
	return m_paperType->GetDeletePaperTypeList_DelDlg(inItemID, outType);
}


/*! \brief Register a paper type to delete at Delete paper type dialog.
 *  \param inItemID: an item ID(0-N)
*/
bool CDataIF::SetDeletePaperType_DelDlg(long inItemID)
{
	return m_paperType->SetDeletePaperType_DelDlg(inItemID);
}


/*! \brief Delete the registered items to delete at Delete paper type dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::DeletePaperTypes_DelDlg()
{
	bool retval = m_paperType->DeletePaperTypes_DelDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Notify to exit Delete paper type dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitDeletePaperTypeDlg()
{
	return m_paperType->ExitDeletePaperTypeDlg();
}


/*! \brief Set a temperature of heat roller for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inTemperature: a temperature
*/
void CDataIF::SetHeatRollerTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	m_dryerSetting->SetHeatRollerTemperature_AddDlg(inPrinterID, inTemperature);
	SetChangeParam();
}


/*! \brief Set a temperature of dryer fun for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inTemperature: a temperature
*/
void CDataIF::SetDryerFunTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	m_dryerSetting->SetDryerFanTemperature_AddDlg(inPrinterID, inTemperature);
	SetChangeParam();
}


/*! \brief Check if a printer has a dryer unit or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataIF::IsExistDryerUnit(long inPrinterID)
{
	return m_systemSetting->IsExistDryerUnit(inPrinterID);
}


/*! \brief Check if a printer has a heat roller or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataIF::IsExistHeatRoller(long inPrinterID)
{
	return m_systemSetting->IsExistHeatRoller(inPrinterID);
}


/*! \brief Get a temperature of heat roller for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval a temperature
*/
const char* CDataIF::GetHeatRollerTemperature_AddDlg(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return NULL;
	}

	_snprintf_s(m_heatRollerTemp[inPrinterID-1], _TRUNCATE, "%ld", m_dryerSetting->GetHeatRollerTemperature_AddDlg(inPrinterID));
	return m_heatRollerTemp[inPrinterID-1];
}


/*! \brief Get a temperature of dryer fun for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval a temperature
*/
const char* CDataIF::GetDryerFunTemperature_AddDlg(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return NULL;
	}

	_snprintf_s(m_dryerFanTemp[inPrinterID-1], _TRUNCATE, "%ld", m_dryerSetting->GetDryerFanTemperature_AddDlg(inPrinterID));
	return m_dryerFanTemp[inPrinterID-1];
}


/*! \brief Set a temperature of precoat for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inTemperature: a temperature
*/
void CDataIF::SetPrecoatTemperature_AddDlg(long inPrinterID, char* inTemperature)
{
	m_dryerSetting->SetPrecoatTemperature_AddDlg(inPrinterID, inTemperature);
	SetChangeParam();
}


/*! \brief Get a temperature of precoat for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval a temperature
*/
const char* CDataIF::GetPrecoatTemperature_AddDlg(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return NULL;
	}

	_snprintf_s(m_precoaterTemp[inPrinterID-1], _TRUNCATE, "%ld", m_dryerSetting->GetPrecoatTemperature_AddDlg(inPrinterID));
	return m_precoaterTemp[inPrinterID-1];
}

/*! \brief Check if dryer settings saved or not
 *  \retval true: it saved
 *  \retval false: it does not save
*/
bool CDataIF::IsSavedDryerSetting_AddDlg()
{
	return m_dryerSetting->IsSavedDryerSetting_AddDlg();
}

/*! \brief Get a value of nir power for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval a temperature
*/
const char* CDataIF::GetNirPower_AddDlg(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return NULL;
	}

	_snprintf_s(m_nirPower[inPrinterID-1], _TRUNCATE, "%ld", m_dryerSetting->GetNirPower_AddDlg(inPrinterID));
	return m_nirPower[inPrinterID-1];
}

/*! \brief Set a value of nir power for a printer at Add dryer settings dialog
 *  \param inPrinterID: a printer ID(1-2)
 *  \param inNirPower: a nir power
*/
void CDataIF::SetNirPower_AddDlg(long inPrinterID, char* inNirPower)
{
	m_dryerSetting->SetNirPower_AddDlg(inPrinterID, inNirPower);
	SetChangeParam();
}

/*! \brief Save the settings of Add dryer settings dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::SaveAddDryerSettingDlg()
{
	long retval = m_dryerSetting->SaveAddDryerSettingDlg();

	switch (retval){
	case -1:			//failed
		return false;
	case 0:				//success (same setting is not added)
		break;
	case 1:				//success (new setting is added)
		//Notify GUI to update the dryer setting list
//		::PostMessage(GetGenSetupDialog(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		::PostMessage(GetPostMessageWindow(), UWM_DRYER_SETTING_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
		break;
	}

	SetChangeParam();
	return true;
}


/*! \brief Cancel to save the settings of Add dryer settings dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::CancelAddDryerSettingDlg()
{
	bool retval = m_dryerSetting->CancelAddDryerSettingDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Set the dialog handle of Delete dryer temperature dialog
 *  \param inDialog: the dialog handle
*/
void CDataIF::SetDeleteDryerSettingDlg(HWND inDialog)
{
	m_dryerSetting->SetDeleteDryerSettingDlg(inDialog);
	SetChangeParam();
}


/*! \brief Get an item of dryer settings list to delete at Delete dryer temperature dialog
 *  \param inItemID: an item ID(0-N)
 *  \param outDryer: a pointer to get a dryer settings
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetDeleteDryerSettingList_DelDlg(long inItemID, const char** outDryer)
{
	return m_dryerSetting->GetDeleteDryerSettingList_DelDlg(inItemID, outDryer);
}


/*! \brief Register a drayer settings list to delete at Delete dryer temperature dialog
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetDeleteDryerSetting_DelDlg(long inItemID)
{
	m_dryerSetting->SetDeleteDryerSetting_DelDlg(inItemID);
}


/*! \brief Delete the registered items to delete at Delete dryer temperature dialog.
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::DeleteDryerSettings_DelDlg()
{
	bool retval = m_dryerSetting->DeleteDryerSettings_DelDlg();
	if(retval){
		SetChangeParam();
	}
	return retval;
}


/*! \brief Notify to exit Delete dryer temperature dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitDeleteDryerSettingDlg()
{
	bool retval = m_dryerSetting->ExitDeleteDryerSettingDlg();
	SetChangeParam();
	return retval;
}


/*! \brief Set a selection side of preprinted mark
 *  \param inSide: a print side(front, back)
*/
void CDataIF::SetSelectPreprintedMarkSide_SelectDlg(DEF_PRINT_SIDE inSide)
{
	m_PaperSettings->SetSelectPreprintedMarkSide_SelectDlg(inSide);
	SetChangeParam();
}


/*! \brief Notify to exit Select preprinted mark side dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataIF::ExitSelectPreprintedMarkSideDlg()
{
	return m_PaperSettings->ExitSelectPreprintedMarkSideDlg();
}

/**
 *	\brief check way to display dialog print condition from case System setting.
**/
bool CDataIF::IsStartFromSystemSetting()
{
	return m_systemSetting->IsStartedFromJobListScreen() ? false : true;
}

/*! \brief get name of current job.
 *  \param outJobName : Name of curernt job.
*/
bool CDataIF::GetJobName(char ** outJobName)
{
	if (outJobName)
	{
		JobInfo* job = m_PrintSettings->GetCurrentJob();
		if (job)
		{
			*outJobName = job->Info.jobName;
			return true;
		}
	}
	return false;
}

/*! \brief get state of ICC profile is assigned to ICCPreset or not.
*/
bool CDataIF::IsICCProfileNotAssignedToICCPreset()
{
	return m_iccPresetSetting->IsICCProfileNotAssignedToICCPreset();
}

/*! \brief get name of current print mode.
*/
void CDataIF::GetCurrentPrintModeName(char ** outModeName)
{
	if (outModeName)
	{
		PrintMode* printmode = m_PrintMode->GetSelectPrintMode();
		if (printmode)
		{
			*outModeName = printmode->DisplayName;
		}
	}
}

/**
 *	Get a dot gain curve
 *	@param[out]	outDotgainCurve : a dot gain curve
 *	@return true : success, false: fail
**/
bool CDataIF::GetDotgainCurve_ForDrawing(ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	return m_dotgainCurve->GetDotgainCurve_ForDrawing(outDotgainCurve);
}

/**
 *	Get the anchor point of the linear
 *	@param[out]outAnchor	Anchor point information
**/
void CDataIF::GetLinearDotGainData_ForDrawing(ST_PDB_ANCHOR_POINT* outAnchor)
{
	m_dotgainCurve->GetLinearDotGainData_ForDrawing(outAnchor);
}

/*!
 *  \brief Check if the newly added ICC preset is used in print condition.
 *    If not used, the dialog box with the warning message that the unused ICC preset will be discarded will be displayed.
 */
void CDataIF::CheckICCPresetUse()
{
	m_iccPresetSetting->CheckICCPresetUse();
}

bool CDataIF::IsSystemSupportJetInspection()
{
	return m_systemSetting->IsSystemSupportJetInspection();
}

bool CDataIF::IsHaveJetInspection()
{
	return m_systemSetting->IsHaveJetInspection();
}

/*! \brief Get an item of Jet Inspection list.
 *  \param inItemID: an item ID(0-N)
 *  \param outJI: a pointer to get a Jet Inspection
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetJetInspectionList(long inItemID, const char** outJI)
{
	return m_jetInspection->GetJetInspectionList(inItemID, outJI);
}

/*! \brief Check the Jet Inspection of Print Condition is existed in the system list of Jet Inspection
 *  \retval true: exist
 *  \retval false: not exist
*/
bool CDataIF::IsJIExistInJIList()
{
	return m_jetInspection->IsJIExistInJIList();
}

/*! \brief Set select Jet Inspection
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectJetInspection(long inItemID)
{
	m_jetInspection->SetSelectJetInspection(inItemID);
	SetChangeParam();
}

/*! \brief Get select Jet Inspection
 *  \param outItemID: an item ID(0-N)
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectJetInspection(long* outItemID)
{
	return m_jetInspection->GetSelectJetInspection(outItemID);
}

/**
 *	画像保管を設定する
 *	@param[in]	inIsEnable : 有効/無効
**/
void CDataIF::SetImageStorage(const bool inIsEnable)
{
	m_jetInspection->SetImageStorage(inIsEnable);
	SetChangeParam();
}

/**
 *	画像保管を取得する
 *	@retval	true: 有効
 *	@retval	false: 無効
**/
bool CDataIF::IsImageStorage()
{
	return m_jetInspection->IsImageStorage();
}


/*! \brief Get an item of ICC preset list.
 *  \param inItemID: an item ID(0-N)
 *  \param outICCPreset: a pointer to get a ICC preset.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetICCPresetList(long inItemId, const char** outICCPreset)
{
	return m_iccPresetSetting->GetICCPresetList(inItemId, outICCPreset);
}

/*! \brief Set a selection ICC preset.
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectICCPreset(long inItemID)
{
	m_iccPresetSetting->SetSelectICCPreset(inItemID);
	SetChangeParam();
}

/*! \brief Get a selection ICC preset
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection ICC preset,
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectICCPreset(long* outItemID)
{
	return m_iccPresetSetting->GetSelectICCPreset(outItemID);
}

/*!
 *  \brief Add the ICC preset (to the list in the system)
 *  \return true：Success, false：Failure.
 */
bool CDataIF::AddSystemICCPreset()
{
	bool retval = m_iccPresetSetting->AddSystemICCPreset();
	SetChangeParam();
	return retval;
}

/*! \brief Set a addition name at Add ICC Preset dialog.
 *  \param inICCPreset: a name of ICC preset
*/
void CDataIF::SetAddICCPresetNameDlg(char* inICCPreset)
{
	m_iccPresetSetting->SetAddICCPresetNameDlg(inICCPreset);
	SetChangeParam();
}

/*! \brief Get a addition name at Add ICC Preset dialog.
 *  \retval a name of ICC Preset
*/
const char* CDataIF::GetAddICCPresetNameDlg()
{
	return m_iccPresetSetting->GetAddICCPresetNameDlg();
}

/*! \brief Cancel to save the settings of Add ICC Preset dialog.
*/
void CDataIF::CancelAddICCPresetNameDlg()
{
	m_iccPresetSetting->CancelAddICCPresetNameDlg();
	SetChangeParam();
}

/*! \brief Check if the new name already exists or not
 *  \return true：exist, false：not exist.
*/
bool CDataIF::IsAddICCPresetNameExist()
{
	return m_iccPresetSetting->IsAddICCPresetNameExist();
}

/*! \brief Check if ICC preser name is exist in the list of ICC Presets or not
 *  \return true：exist, false：not exist.
*/
bool CDataIF::IsExistName()
{
	return m_iccPresetSetting->IsExistName();
}

/*! \brief Remove ICC preset name does not exist
*/
void CDataIF::RemoveICCPresetNameNotExist()
{
	m_iccPresetSetting->RemoveICCPresetNameNotExist();
	SetChangeParam();
}

/*! \brief Check if print condition of the job is edited or not.
 *  \retval true: edited
 *  \retval false: not edited
*/
bool CDataIF::IsPrintConditionEdited()
{
	if(m_systemSetting->IsPrintConditionEdited())
		return true;
	else
		return false;
}

/*! \brief Set the dynamic nozzle shift setting.
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataIF::SetDynamicNozzleShift(bool inUse)
{
	m_PaperSettings->SetDynamicNozzleShift(inUse);
	SetChangeParam();
}


/*! \brief Get the dynamic nozzle shift setting.
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataIF::GetDynamicNozzleShift()
{
	return m_PaperSettings->GetDynamicNozzleShift();
}

/**
 *	Get duplicate JetInspection area settings.
 *	@retval	false:Not duplicate, true:Duplicate
**/
bool CDataIF::GetDuplicateJIAreaSettings()
{
	return m_PaperSettings->GetDuplicateJIAreaSettings();
}

/*! \brief Set duplicate JetInspection area settings.
 *  \param inState State of duplicate JetInspection area settings. (false:Not duplicate, true:Duplicate).
*/
void CDataIF::SetDuplicateJIAreaSettings(bool inState)
{
	m_PaperSettings->SetDuplicateJIAreaSettings(inState);
	SetChangeParam();
}

/**
 *	Get duplicate JetInspection area settings.
 *	@retval	false:Not duplicate, true:Duplicate
**/
double CDataIF::GetDuplicateInitNonInspectionAreaDefault()
{
	return m_PaperSettings->GetDuplicateInitNonInspectionAreaDefault();
}


/**
 *	Update current setting in case the user change the print condition name
**/
void CDataIF::UpdateCurrent()
{
	const char* printConditionOrSection = m_PrintSettings->GetSelectPrintConditionOrSection();
	// 2017.12.04
	// 出力色モードによって印刷モードを切り替えるのでOutputColorSettingをPrintModeより先にUpdateする
	m_OutputColorSetting->UpdateCurrentOutputColor(printConditionOrSection);
	m_PrintMode->UpdatePrintCondition(printConditionOrSection);
	m_paperType->UpdateCurrentSelectionPaperType(printConditionOrSection);
	m_printHeadAlignment->UpdateCurrentSelectionPrintHeadAlignment(printConditionOrSection);
	m_preOverCoat->UpdateCurrentPreOverCoat(printConditionOrSection);
	m_PaperSettings->UpdatePrintCondition(printConditionOrSection);
	m_jetInspection->UpdateCurrentJetInspection(printConditionOrSection);
	m_dryerSetting->UpdatePrintCondition(printConditionOrSection);
	m_iccPresetSetting->UpdatePrintCondition(printConditionOrSection);
	m_dotgainCurve->UpdatePrintCondition(printConditionOrSection);
	m_DataVariableMagnification->UpdatePrintCondition(printConditionOrSection);
	m_DataPrimer->UpdatePrintCondition(printConditionOrSection);
}

/*! \brief Get Host info
 *  \retval the host info
 */
PHOST CDataIF::GetHost()
{
	return m_Parent->GetHost();
}

/*! \brief Check if selection is blank or not
 *  \retval true: Not allow
 *  \retval false: Allow
*/
bool CDataIF::IsBlankNotAllowed()
{
	bool bBlank;

	bBlank = m_PrintSettings->IsBlankNotAllowed()
		|| m_paperType->IsBlankNotAllowed()
		|| m_printHeadAlignment->IsBlankNotAllowed()
		|| m_dryerSetting->IsBlankNotAllowed()
		|| m_iccPresetSetting->IsBlankNotAllowed();

	if (m_systemSetting->IsSystemSupportJetInspection())
		bBlank = bBlank || m_jetInspection->IsBlankNotAllowed();


	return bBlank;
}
/*! \brief Check if the current job is simplex job or not
 *  \retval true: Is simplex job
 *  \retval false: Not simplex job
*/
bool CDataIF::IsSimplexJob()
{
	return m_IsSimplexJob;
}

/**
 *	Check if dotgain curve is default or not
 *	@note General process
 *	
 *
 *	@return true: is default, false : otherwise
**/
bool CDataIF::IsDotgainCurveDefault_ForDrawing()
{
	return m_dotgainCurve->IsDotgainCurveDefault_ForDrawing();
}

/**
 *	Get tone frame width
 *	@retval	tone frame width
**/
long CDataIF::GetToneFrameWidth_ForDrawing()
{
	return m_dotgainCurve ->GetToneFrameWidth_ForDrawing();
}

/**
 *	Get tone frame color
 *	@retval	tone frame color (R,G,B)
**/
COLORREF CDataIF::GetToneFrameColor_ForDrawing()
{
	return m_dotgainCurve ->GetToneFrameColor_ForDrawing();
}

/*! \brief Get rect of non inspection area
 *	\ param[IN] inItemID: item id in the list of mask
 *	\ param[IN] inSide: The printer side (front/Back)
 *  \ param[OUT] outRect: The rect of non inspection area
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataIF::GetNonInspectionRect(long inItemID, DEF_PRINT_SIDE inSide, RECT& outRect)
{
	return m_PaperSettings->GetNonInspectionRect(inItemID, inSide, outRect);
}

/*! \brief Get info of a eliminated area of  a printer side
 *  \ param[in] inSide : The printer side
 *  \ param[in] inIsLeft : Check if non inspection area is left or right
 *  \ param[out] outLeft : The left pos of eliminated rect
 *  \ param[out] outRight : The right pos of eliminated rect
*/
void CDataIF::GetEliminatedInfo(DEF_PRINT_SIDE inSide, bool inIsLeft, double& outLeft, double& outRight)
{
	m_PaperSettings->GetEliminatedInfo(inSide, inIsLeft, outLeft, outRight);
}

/*! \brief Get maximum and minimum value of paper size.
 *	\ param[OUT] outMaxWidth: Maximum width
 *	\ param[OUT] outMinWidth: Minimum width
 *  \ param[OUT] outMaxHeight: Maximum height
 *  \ param[OUT] outMinHeight: Minimum height
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataIF::GetMaxMinPaperSize(long& outMaxWidth, long& outMinWidth, long& outMaxHeight, long& outMinHeight)
{
	return m_PaperSettings->GetMaxMinPaperSize(outMaxWidth, outMinWidth, outMaxHeight, outMinHeight);
}

/*! \brief Get prining start mark information
 *	\ param[IN] inSide: The printer side
 *  \ param[OUT] outOffsetX: Offset X
 *  \ param[OUT] outOffsetY: Offset Y
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataIF::GetVerifyMarkOffset(DEF_PRINT_SIDE inSide, double& outOffsetX, double& outOffsetY)
{
	return m_PaperSettings->GetVerifyMarkOffset(inSide, outOffsetX, outOffsetY);
}

/*! \brief Check if printing start mark is enabled or not
 *  \ retVal TRUE: Enable
 *  \ retVal FALSE: Disable
*/
BOOL CDataIF::GetEnablePrintStartMark()
{
	return m_PaperSettings->GetEnablePrintStartMark();
}

/**
 *	Get selection targets to verify mark
 *	\ note Process to initialize the member variable
 *	
 *		JM_GetVerifyMark_LayoutImage
 * \ retVal status of verifying mark(TRUE:ON, FALSE:OFF)
**/
BOOL CDataIF::GetVerifyMarkTarget()
{
	return m_PaperSettings->GetVerifyMarkTarget();
}

/*! \brief Get the paper length unit
 *  \ retVal the unit of peper length
*/
eUNIT_MODE CDataIF::GetPaperLengthUnit()
{
	return m_PaperSettings->GetPaperLengthUnit();
}

/*!
 * \brief ジョブ設定を開始
 * \return 処理結果
 * \note 印刷条件設定画面でジョブ設定ボタン押下時に印刷条件モードでジョブ設定を開始する
 */
bool CDataIF::StartJobSetting()
{
	if (!m_PrintConditionModeJobSetting) {
		return false;
	}
	return m_PrintConditionModeJobSetting->StartJobSetting();
}

/**
 *	Get print condition names of running job
 *	@param[out] outPrintCondition: Print condition names
 *	@retval	true :	 success
 *	@retval	false : fail
**/
bool CDataIF::GetPrintConditionNamesOfRunningJob(std::vector<std::string>& outPrintConditions)
{
	return m_systemSetting->GetPrintConditionNamesOfRunningJob(outPrintConditions);
}

/*! \brief Get the paper resolution
 *	\ param[IN] inSide: The printer side
 *  \ param[OUT] outResoX: the resolution follow X coordinate
 *  \ param[OUT] outResoY: the resolution follow Y coordinate
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataIF::GetPaperReso(DEF_PRINT_SIDE inSide, double& outResoX, double& outResoY)
{
	return m_PaperSettings->GetPaperReso(inSide, outResoX, outResoY);
}

/*! \brief Get the RIP height of specified job
 *  \ param[OUT] outRipHeight: the RIP height of specified job
*/
void CDataIF::GetRipSizeH(double& outRipHeight)
{
	m_PaperSettings->GetRipSizeH(outRipHeight);
}

/*! \brief Set a window handle.
 *  \param inWnd: window handle
*/
void CDataIF::SetPostMessageWindow(HWND inWnd)
{
	m_systemSetting->SetPostMessageWindow(inWnd);
}

/*! \brief Get a window handle.
 *  \ retVal window handle
*/
HWND CDataIF::GetPostMessageWindow()
{
	return m_systemSetting->GetPostMessageWindow();
}

/*!	\brief 150mpmに対応しているかをチェック
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupport150mpm()
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupport150mpm PrinterState_GetCallbacks failed.");
		return false;
	}

	if(m_systemSetting->IsDED()){
		// DED
		if((printerStateCb.PrinterState_GetLoadingSpeedOption(true) == eSpeedOptionType_Fast) &&
			(printerStateCb.PrinterState_GetLoadingSpeedOption(false) == eSpeedOptionType_Fast)){
			// 両機が対応している
			return true;
		}
	}
	else{
		// SES
		if(printerStateCb.PrinterState_GetLoadingSpeedOption(true) == eSpeedOptionType_Fast){
			// 自機が対応している
			return true;
		}
	}
	return false;
}

/*!	\brief 120[C]に対応しているかをチェック
 *  \param inIsSelf [IN] ：true：自機、false：相手機.
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupport120DegC(bool inIsSelf)
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupport120DegC PrinterState_GetCallbacks failed.");
		return false;
	}

	if(printerStateCb.PrinterState_IsUnitOf120DegC(inIsSelf) == true){
		return true;
	}
	return false;
}

/*!	\brief 520HD V3 120[C]に対応しているかをチェック
 *  \param inIsSelf [IN] ：true：自機、false：相手機.
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupport120DegC_V3(bool inIsSelf)
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupport120DegC_V3 PrinterState_GetCallbacks failed.");
		return false;
	}

	if(printerStateCb.PrinterState_IsUnitOf120DegC_V3(inIsSelf) == true){
		return true;
	}
	return false;
}

/*!	\brief 140[C]に対応しているかをチェック
 *  \param inIsSelf [IN] ：true：自機、false：相手機.
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupport140DegC(bool inIsSelf)
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupport140DegC PrinterState_GetCallbacks failed.");
		return false;
	}

	if(printerStateCb.PrinterState_IsUnitOf140DegC(inIsSelf) == true){
		return true;
	}
	return false;
}

/**
 *	Check if the system is running with IPDS mode
 *	@retval	TRUE: IPDS mode
 *	@retval	FALSE: normal mode
**/
BOOL CDataIF::IsIpdsMode()
{
	return m_systemSetting->IsIpdsMode();
}

/*!	\brief 100mpmに対応しているかをチェック
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupport100mpm()
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupport100mpm PrinterState_GetCallbacks failed.");
		return false;
	}

	if(m_systemSetting->IsDED()){
		// DED
		if((printerStateCb.PrinterState_GetLoadingSpeed100Option(true) == 1) &&
			(printerStateCb.PrinterState_GetLoadingSpeed100Option(false) == 1)){
			// 両機が対応している
			return true;
		}
	}
	else{
		// SES
		if(printerStateCb.PrinterState_GetLoadingSpeed100Option(true) == 1){
			// 自機が対応している
			return true;
		}
	}
	return false;
}

/*!	\brief 画面Open時のCurrentConditionボタンのOn/Off状態を設定
 *  \param inOnOff: true:On, false:Off
*/
void CDataIF::SetCurrentConditionBtnOnOffOpen(bool inOnOff)
{
	m_IsPushCurrentConditionBtnOpen = inOnOff;
}

/*!	\brief 画面Open時のCurrentConditionボタンのOn/Off状態を取得
 *	\return true : On
 *	\return false : Off
*/
bool CDataIF::GetCurrentConditionBtnOnOffOpen()
{
	return m_IsPushCurrentConditionBtnOpen;
}

/*!	\brief 現在のCurrentConditionボタンのOn/Off状態を設定
 *  \param inOnOff: true:On, false:Off
*/
void CDataIF::SetCurrentConditionBtnOnOff(bool inOnOff)
{
	m_IsPushCurrentConditionBtn = inOnOff;
}

/*!	\brief 現在のCurrentConditionボタンのOn/Off状態を取得
 *	\return true : On
 *	\return false : Off
*/
bool CDataIF::GetCurrentConditionBtnOnOff()
{
	return m_IsPushCurrentConditionBtn;
}

/**
 *	Get colors size
 *	@param[in]	inSide : a print side(front/back)
 *	@reval	: number of colors
**/
UINT CDataIF::GetDisplayColorSize(DEF_PRINT_SIDE inSide) const
{
	return m_systemSetting->GetDisplayColorSize(inSide);
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
bool CDataIF::IsMaster()
{
	return m_systemSetting->IsMaster();
}

/**
 *	Get color by color name
 *	@param[in]	inSide : a print side(front/back)
 *	@param[in]	inColorName : the color name
 *	@param[out]	outColor : Color value of color name
 *	@retval	true: The color exist
 *	@retval	false: The color doesn't exist
**/
bool CDataIF::GetDisplayColor(DEF_PRINT_SIDE inSide, const char* inColorName, COLORREF& outColor)
{
	return m_systemSetting->GetDisplayColor(inSide, inColorName, outColor);
}

/**
 *	Set current dotgain curve value to PrintConditionGUI
 *	@param[in]	inSystem : input eComSystemType
 *	@param[in]	inDotgainCurve : dotgain curve value
**/
void CDataIF::SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	return m_dotgainCurve->SetCurrentDotGainCurve(inSystem, inDotgainCurve);
}

/**
 *	Get current dotGain curve value from PrintconditionGUI
 *	@param[in]	inSystem : eComSystemType value
 *	@param[out]	outDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CDataIF::GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	return m_dotgainCurve->GetCurrentDotGainCurve(inSystem, outDotgainCurve);
}

/*! \brief Check if a printer has a nir dryer unit or not.
 *  \param inPrinterID: a printer ID(1-2)
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataIF::IsExistNirDryerUnit(long inPrinterID)
{
	return m_systemSetting->IsExistNirDryerUnit(inPrinterID);
}

/*! \brief Check if a printer has multi color option or not.
 *  \retval true: the printer has it.
 *  \retval false: the printer does not have it.
*/
bool CDataIF::IsExistMultiColor()
{
	if(m_systemSetting->IsDED()){
		// DED
		if((m_systemSetting->IsExistMultiColor(DEF_PRINTER_1)) && (m_systemSetting->IsExistMultiColor(DEF_PRINTER_2))){
			// 両機が対応している
			return true;
		}
	}
	else{
		// SES
		if(m_systemSetting->IsExistMultiColor(DEF_PRINTER_1)){
			// 自機が対応している
			return true;
		}
	}
	return false;
}

/*! \brief Set a selection output color
 *  \param inItemID: an item ID(0-N)
*/
void CDataIF::SetSelectOutputColor(long inItemID)
{
	m_OutputColorSetting->SetSelectOutputColor(inItemID);
	// 2017.12.04
	// 出力色モードによって印刷モードを切り替えるのでPrintMode(Speed,Tension)をリセットする
	m_PrintMode->UpdateOutputColorMode();
	SetChangeParam();
}

/*! \brief Get a selection output color
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection ICC preset,
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataIF::GetSelectOutputColor(long* outItemID)
{
	return m_OutputColorSetting->GetSelectOutputColor(outItemID);
}

//表面の事前印刷マークの副走査オフセットを取得する
long CDataIF::GetPreMarkOffsetFrontY()
{
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	long preMarkOffsetFrontY = iniPrinterDescriptor.GetPreMarkOffsetFrontY();
	iniPrinterDescriptor.Finalize();
	return preMarkOffsetFrontY;
}

//ジョブ管理にQueueStatusの発行を依頼する
void CDataIF::SendQueueStatusCommand()
{
	// EQのジョブリストを1回だけ取得する
	bool isShowProgressDialog = true;
	m_systemSetting->GetJobManager_Callbacks()->JM_GetEQJobListOnlyOnce(isShowProgressDialog);
}

//削除する印刷条件名を使用しているジョブがいないかのチェックする
bool CDataIF::IsPrintConditionUsedForJobs(const std::string& inPrintConditionName, std::string& outUsingJob)
{
	return m_systemSetting->GetJobManager_Callbacks()->JM_IsPrintConditionUsedForJobs(inPrintConditionName, outUsingJob);
}

//レイアウトイメージ情報を取得する./ Get the layout image information from the print condition
bool CDataIF::GetFlushingPattern(const std::string& inPrintConditionName, ST_JOB_FLUSHING_PATTERN& outFlushingPattern, ST_JOB_FLUSHING_PATTERN_ADD& outFlushingPatternAdd, BOOL& outVerticalAlignment_PP_FLSPTN)
{
	return m_PrintSettings->GetFlushingPattern(inPrintConditionName, outFlushingPattern, outFlushingPatternAdd, outVerticalAlignment_PP_FLSPTN);
}

//Getting the select print mode
void CDataIF::GetCurrentPrintMode(PrintMode& outPrintMode)
{
	PrintMode* printMode = m_PrintMode->GetSelectPrintMode();
	outPrintMode = *printMode;
}

//モノクロ印刷設定を取得する(表)
bool CDataIF::IsMonochromePrint_F()
{
	return m_PrintSettings->IsMonochromePrint_F();
}

//Getting the select nir power
long CDataIF::GetSelectNirPower(bool isSelf)
{
	// Get the select dryer temperature
	ST_PDB_DRY_TEMP_PRINT	dryTempPrint = {0};
	m_dryerSetting->OnCopyJobPrintCondition(dryTempPrint);

	long	ret = 0;
	if(isSelf){
		ret = dryTempPrint.NIR_POWER_SELF;
	} else {
		ret = dryTempPrint.NIR_POWER_PART;
	}

	return ret;
}

/*!	\brief コート紙用印刷モードに対応しているかをチェック
 *	\return true : 対応している
 *	\return false : 対応していない
*/
bool CDataIF::IsSupportPrintModeForCoatedOffset()
{
	SPrinterState_Callbacks printerStateCb;
	if(!PrinterState_GetCallbacks(&printerStateCb)){
		WriteToLogBuf(LOG_DEBUG, "CDataIF::IsSupportPrintModeForCoatedOffset PrinterState_GetCallbacks failed.");
		return false;
	}

	if(m_systemSetting->IsDED()){
		// DED
		if((printerStateCb.PrinterState_IsUnitOfPrintModeForCoatedOffset(true) == true) &&
			(printerStateCb.PrinterState_IsUnitOfPrintModeForCoatedOffset(false) == true)){
			// 両機が対応している
			return true;
		}
	}
	else{
		// SES
		if(printerStateCb.PrinterState_IsUnitOfPrintModeForCoatedOffset(true) == true){
			// 自機が対応している
			return true;
		}
	}
	return false;
}

//インポート時ベースの印刷条件名を削除確認後、キャンセルしたかのチェック。
bool CDataIF::IsCancelDeleteBasePrintCondition(const std::string& inPrintConditionName)
{
	if (IsBasePrintCondition(inPrintConditionName)) {
		//対象が業務移行時の基準印刷条件の場合。
		std::string formMsg = LoadResourceString(IDM_DELETE_BASE_CODITION, RESOURCE_MSG_STR);
		size_t pos = formMsg.find("%s");
		if (pos != std::string::npos) {
			formMsg.replace(pos, 2, inPrintConditionName);
		}

		std::stringstream delMsg;
		delMsg << ID_MESSAGE_ERROR+IDM_DELETE_BASE_CODITION << "\n" << formMsg;
		if(IDCANCEL == ShowMessageBox(const_cast<char*>(delMsg.str().c_str()), MBST_OK | MBST_CANCEL | MBST_ICONWARNING, NULL)){
			//キャンセルした。
			return true;
		}
	}
	return false;
}

/**
	@brief	全画像保存機能の有効無効
	@retval	true : 有効
	@retval	false : 無効
*/
bool CDataIF::IsImageStorageEnable()
{
	return m_jetInspection->IsImageStorageEnable();
}

/**
	@brief	JIの領域情報コピーの有効無効
	@retval	true : 有効
	@retval	false : 無効
*/
bool CDataIF::IsJIDuplicatingEnable()
{
	return m_jetInspection->IsJIDuplicatingEnable();
}


// Ranger搭載有無の取得
bool CDataIF::IsUnitOfRanger()
{
	return m_systemSetting->IsUnitOfRanger();
}

/**
	@brief	目標乾燥温度の設定
	@param[in]	inPrinterID : PrinterID
	@param[in]	inTemperature : 目標乾燥温度
*/
void CDataIF::SetDryerTemp_SubHeatRoller(long inPrinterID, char* inTemperature)
{
	m_dryerSetting->SetDryerTemp_HeatRanger(inPrinterID, inTemperature);
	SetChangeParam();
}

/**
	@brief	目標乾燥温度の取得
	@param[in]	inPrinterID : PrinterID
	@retval	目標乾燥温度
*/
const char*  CDataIF::GetDryerTemp_SubHeatRoller(long inPrinterID)
{
	if (inPrinterID < 1 || inPrinterID > 2){
		return NULL;
	}

	_snprintf_s(m_subHeatRollerTemp[inPrinterID-1], _TRUNCATE, "%ld", m_dryerSetting->GetDryerTemp_HeatRanger(inPrinterID));
	return m_subHeatRollerTemp[inPrinterID-1];

}

/**
 *	Check existence of DNS
 *	@retval	: true: exist
**/
bool CDataIF::IsExistDNS()
{
	return m_systemSetting->IsExistDNS();
}

// 変倍ボタンのコントロール表示状態更新イベント
void CDataIF::onView_VariableMagnification(bool& outIsShow, bool& outIsEnabled)
{
	m_DataVariableMagnification->onView_VariableMagnification(outIsShow, outIsEnabled);
}

// 変倍値表示フィールドのコントロール表示状態更新イベント
void CDataIF::onView_VariableMagnificationField(bool& outIsShowFront, bool& outIsShowBack)
{
	m_DataVariableMagnification->onView_VariableMagnificationField(outIsShowFront, outIsShowBack);
}

// RIP変倍値設定のコントロール表示状態更新イベント
void CDataIF::onView_RipVariableMagnification(bool& outIsShow)
{
	m_DataVariableMagnification->onView_RipVariableMagnification(outIsShow);
}

// 変倍入力のコントロール表示状態更新イベント
void CDataIF::onView_VariableMagnificationInput(bool& outIsShowFront, bool& outIsShowBack, bool& outIsEnabled)
{
	m_DataVariableMagnification->onView_VariableMagnificationInput(outIsShowFront, outIsShowBack, outIsEnabled);
}

// 基準面指定のコントロール表示状態更新イベント
void CDataIF::onView_VariableMagnificationReferencePlane(bool& outIsShow)
{
	m_DataVariableMagnification->onView_VariableMagnificationReferencePlane(outIsShow);
}

// 登録ボタンのコントロール表示状態更新イベント
void CDataIF::onView_VariableMagnificationCommit(bool& outIsEnabled)
{
	m_DataVariableMagnification->onView_VariableMagnificationCommit(outIsEnabled);
}

// 変倍値表示フィールドのコントロール表示内容更新イベント
void CDataIF::onDraw_VariableMagnificationField(std::string& outFrontX, std::string& outFrontY, std::string& outBackX, std::string& outBackY)
{
	m_DataVariableMagnification->onDraw_VariableMagnificationField(outFrontX, outFrontY, outBackX, outBackY);
}

// RIP変倍値設定のコントロール表示内容更新イベント
void CDataIF::onDraw_RipVariableMagnification(bool& outIsRipVariableMagnification)
{
	m_DataVariableMagnification->onDraw_RipVariableMagnification(outIsRipVariableMagnification);
}

// 基準面指定のコントロール表示内容更新イベント
void CDataIF::onDraw_VariableMagnificationReferencePlane(bool& outIsEditDatumPlaneFront)
{
	m_DataVariableMagnification->onDraw_VariableMagnificationReferencePlane(outIsEditDatumPlaneFront);
}

// 変倍入力のコントロール表示内容更新イベント
void CDataIF::onDraw_VariableMagnificationInput(
	std::string& outFrontX, 
	std::string& outFrontY, 
	std::string& outBackX, 
	std::string& outBackY,
	bool& outIsEditRipVariableMagnification)
{
	m_DataVariableMagnification->onDraw_VariableMagnificationInput(outFrontX, outFrontY, outBackX, outBackY, outIsEditRipVariableMagnification);
}

// 変倍ボタンのコントロール操作イベント
void CDataIF::onPush_VariableMagnification(CBaseDataIF* inData, CBasePrinterIF* inPrinter)
{
	m_DataVariableMagnification->onPush_VariableMagnification(inData, inPrinter);
}

// RIP変倍値設定のコントロール操作イベント
void CDataIF::onPush_RipVariableMagnification(bool inIsRipVariableMagnification)
{
	m_DataVariableMagnification->onPush_RipVariableMagnification(inIsRipVariableMagnification);
	SetChangeParam();
}

// 基準面設定のコントロール操作イベント
void CDataIF::onPush_VariableMagnificationReferencePlane()
{
	m_DataVariableMagnification->onPush_VariableMagnificationReferencePlane();
	SetChangeParam();
}

// 変倍入力のコントロール操作イベント
void CDataIF::onPush_VariableMagnificationInput(
	const long long& inFrontX, 
	const long long& inFrontY, 
	const long long& inBackX, 
	const long long& inBackY)
{
	m_DataVariableMagnification->onPush_VariableMagnificationInput(inFrontX, inFrontY, inBackX, inBackY);
	SetChangeParam();
}

// 変倍登録ボタンのコントロール操作イベント
void CDataIF::onPush_VariableMagnificationCommit()
{
	m_DataVariableMagnification->onPush_VariableMagnificationCommit();
	SetChangeParam();
}

// 変倍終了ボタンのコントロール操作イベント
bool CDataIF::onPush_VariableMagnificationClose()
{
	bool isChange = m_DataVariableMagnification->onPush_VariableMagnificationClose();
	SetChangeParam();
	return isChange;
}

// 変倍ダイアログを初期化する
void CDataIF::initializeDlgVariableMagnification()
{
	m_DataVariableMagnification->initializeDlgVariableMagnification();
}

// 編集中の変倍値を取得する
void CDataIF::getVariableMagnificationInput(long long& outFrontX, long long& outFrontY, long long& outBackX, long long& outBackY)
{
	m_DataVariableMagnification->getVariableMagnificationInput(outFrontX, outFrontY, outBackX, outBackY);
}
// プライマーのON/OFFボタンのコントロール表示状態更新イベント
void CDataIF::onView_Primer(bool& outIsShow, bool& outIsEnabled)
{
	m_DataPrimer->onView_Primer(outIsShow, outIsEnabled);
}

// プライマーのON/OFFボタンのボタン押下イベント
void CDataIF::onPush_Primer(bool inState)
{
	m_DataPrimer->onPush_Primer(inState);
	SetChangeParam();
}

// プライマーのON/OFFボタンのコントロール表示内容更新イベント
void CDataIF::onDraw_Primer(bool& outState)
{
	m_DataPrimer->onDraw_Primer(outState);
}

// 全画像保管ボタンのコントロール表示状態更新イベント
void CDataIF::onView_ImageStorage(bool& outIsShow, bool& outIsEnabled)
{
	bool isLockPrintCondtion = GetLockPrintCondtion();
	bool isDisablePrintCondition = IsDisablePrintCondition();
	m_jetInspection->onView_ImageStorage(isLockPrintCondtion, isDisablePrintCondition, outIsShow, outIsEnabled);
}

// 坪量の取得
double CDataIF::getPaperWeight()
{
	return m_PaperSettings->getPaperWeight();
}

// 坪量の設定
void CDataIF::setPaperWeight(const double inPaperWeight)
{
	m_PaperSettings->setPaperWeight(inPaperWeight);
	SetChangeParam();
}

/**
 *	Get CDataPaperSettings's object
 *	retval: CDataPaperSettings's object
**/
CDataPaperSettings * CDataIF::GetDataPaperSettings(void)
{
	return m_PaperSettings;
}
