/*! \file DlgPageMain.cpp
*  \brief Main page dialog
*  \author GCS
*  \date 2014/11/13 GCS
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgPageMain.h"

#include "CtlGpMain.h"
#include "CtlPrintSettingsSelection.h"
#include "CtlCurrentPrintCondition.h"
#include "CtlPrintModeSelection.h"
#include "CtlPrintingSpeedSelection.h"
#include "CtlTensionSelection.h"
#include "CtlPrintHeadAlignSelection.h"
#include "CtlPaperTypeSelection.h"
#include "CtlPrecoatSelection.h"
#include "CtlOvercoatSelection.h"
#include "CtlDryerSettingsSelection.h"
#include "CtlPageSize.h"
#include "CtlPaperThickness.h"
#include "CtlPaperOption.h"
#include "CtlPaperOptionPreview.h"
#include "CtlToneCurveSettings.h"
#include "CtlPrintConditionCommitting.h"
#include "CtlLockSettings.h"
#include "CtlCloseScreen.h"
#include "CtlPrintSettingsName.h"
#include "CtlInspectionSettingSelection.h"
#include "CtlICCPresetSetting.h"
#include "CtlDNS.h"
#include "CtlJobSetting.h"
#include "CtlOutputColorSelection.h"
#include "CtlImageStorage.h"
#include "CtlVariableMagnification.h"
#include "CtlPrimer.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		1245//!< Width of dialog
#define DEF_DLG_H		875//!< Heigh of dialog

enum{
	CTLGP_GP_MAIN = 0,								//!< Main group-box control
	CTLGP_PRINT_SETTINGS_SELECTION,					//!< Print settings selection control
	CTLGP_CURRENT_PRINT_SETTINGS,					//!< Current print settings control
	CTLGP_PRINT_MODE_SELECTION,						//!< Print mode selection control
	CTLGP_PRINT_SPEED_SELECTION,					//!< Print speed selection control
	CTLGP_TENSION_SELECTION,						//!< Paper tension selection control
	CTLGP_PRINT_HEAD_ALIGNMENT_SELECTION,			//!< Print head alignment selection control
	CTLGP_PAPER_TYPE_SELECTION,						//!< Paper type selection control
	CTLGP_ICC_PRESET_SELECTION,						//!< ICC preset selection control
	CTLGP_PRECOAT_SELECTION,						//!< Precoat selection control
	CTLGP_OVERCOAT_SELECTION,						//!< Overcoat selection control
	CTLGP_DRYER_SETTINGS_SELECTION,					//!< Dryer settings selection control
	CTLGP_PAGE_SIZE,								//!< Page size control
	CTLGP_PAPER_THICKNESS,							//!< Paper thickness control
	CTLGP_PAPER_OPTION,							//!< Paper option control
	CTLGP_PAPER_OPTION_PREVIEW,					//!< Paper option preview control
	CTLGP_INSPECTION_SETTING_SELECTION,				//!< Inspection setting selection control
	CTLGP_TONE_CURVE_SETTINGS,						//!< Tone curve settings control
	CTLGP_PRINT_CONDITION_COMMITTING,				//!< Print condition committing control
	CTLGP_LOCK_SETTINGS,							//!< Lock settings control
	CTLGP_CLOSE_SCREEN,								//!< Close screen control
	CTLGP_PRINT_SETTING_NAME,						//!< print settings name
	CTLGP_DNS,									//!< dynamic nozzle shift
	CTLGP_START_JOB_SETTING,						//!< basic setup button control
	CTLGP_OUTPUTCOLOR_SELECTION,					//!< Output color selection control
	CTLGP_IMAGE_STORAGE,							//!< ImageStorage selection control
	CTLGP_VARIABLE_MAGNIFICATION,					//!< 変倍コントロール
	CTLGP_PRIMER,									//!< Primer control

	CTLGP_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CDlgPageMain::CDlgPageMain()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \brief destructor
*/
CDlgPageMain::~CDlgPageMain()
{
}

/*! \brief Create main dialog
*  \param inData DataIF
*  \param inPrinter Printer
*  \return TRUE/FALSE : Success/Fail
*/
BOOL CDlgPageMain::Create(CBaseDataIF* inData, CBasePrinterIF* inPrinter)
{
	CDataIF* pData = dynamic_cast<CDataIF*>(inData);
	if (pData->EnterScreen())
		return CBaseDlg::Create(inData, inPrinter);
	return FALSE;
}

/*! \brief Create main dialog
*  \param inData DataIF
*  \param inPrinter Printer
*  \return TRUE/FALSE : Success/Fail
*/
void CDlgPageMain::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	pData->SetPostMessageWindow(m_hWnd);
}

/*! \brief Set attribute for main dialog
*  \return None
*/
void CDlgPageMain::OnSetAttribute()
{
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	int Width = jobSelect_ClientRect.right - jobSelect_ClientRect.left + 1;
	int Height = jobSelect_ClientRect.bottom - jobSelect_ClientRect.top + 1;

	RECT DlgRect =
	{
		(Width -  DEF_DLG_W) / 2,
		(Height - DEF_DLG_H) / 2,
		((Width - DEF_DLG_W) / 2) + DEF_DLG_W,
		((Height - DEF_DLG_H) / 2) + DEF_DLG_H
	};

	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__));


	// main group-box control
	CCtlGpMain* gpMain = new CCtlGpMain();
	m_controls[CTLGP_GP_MAIN] = (CBaseCtl*)gpMain;

	// print settings selection control
	CCtlPrintSettingsSelection* ctlPrintSettingsSelection = new CCtlPrintSettingsSelection();
	m_controls[CTLGP_PRINT_SETTINGS_SELECTION] = (CBaseCtl*)ctlPrintSettingsSelection;

	// current print settings control
	CCtlCurrentPrintCondition* ctlCurPrintCondition = new CCtlCurrentPrintCondition();
	m_controls[CTLGP_CURRENT_PRINT_SETTINGS] = (CBaseCtl*)ctlCurPrintCondition;

	// print mode selection control
	CCtlPrintModeSelection* ctlPrintModeSelection = new CCtlPrintModeSelection();
	ctlPrintModeSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRINT_MODE_SELECTION] = (CBaseCtl*)ctlPrintModeSelection;

	// print speed selection control
	CCtlPrintingSpeedSelection* ctlPrintSpeedSelection = new CCtlPrintingSpeedSelection();
	ctlPrintSpeedSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRINT_SPEED_SELECTION] = (CBaseCtl*)ctlPrintSpeedSelection;

	// Paper tension selection control
	CCtlTensionSelection* ctlTensioSelection = new CCtlTensionSelection();
	ctlTensioSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_TENSION_SELECTION] = (CBaseCtl*)ctlTensioSelection;

	// Print head alignment selection control
	CCtlPrintHeadAlignSelection* ctlPrintHeadAlignment = new CCtlPrintHeadAlignSelection();
	ctlPrintHeadAlignment->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRINT_HEAD_ALIGNMENT_SELECTION] = (CBaseCtl*)ctlPrintHeadAlignment;

	// Paper type selection control
	CCtlPaperTypeSelection* ctlPaperType = new CCtlPaperTypeSelection();
	ctlPaperType->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PAPER_TYPE_SELECTION] = (CBaseCtl*)ctlPaperType;

	// Paper type selection control
	CCtlICCPresetSetting* ctlICC = new CCtlICCPresetSetting();
	ctlICC->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_ICC_PRESET_SELECTION] = (CBaseCtl*)ctlICC;

	// Precoat selection control
	CCtlPrecoatSelection* ctlPrecoatSelection = new CCtlPrecoatSelection();
	ctlPrecoatSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRECOAT_SELECTION] = (CBaseCtl*)ctlPrecoatSelection;

	// Overcoat selection control
	CCtlOvercoatSelection* ctlOvercoatSelection = new CCtlOvercoatSelection();
	ctlOvercoatSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_OVERCOAT_SELECTION] = (CBaseCtl*)ctlOvercoatSelection;

	// Dryer settings selection control
	CCtlDryerSettingsSelection* ctlDryerSettings = new CCtlDryerSettingsSelection();
	ctlDryerSettings->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_DRYER_SETTINGS_SELECTION] = (CBaseCtl*)ctlDryerSettings;

	// Page size control
	CCtlPageSize* ctlPageSize = new CCtlPageSize();
	ctlPageSize->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PAGE_SIZE] = (CBaseCtl*)ctlPageSize;

	// Paper thickness control
	CCtlPaperThickness* ctlPaperThickness = new CCtlPaperThickness();
	ctlPaperThickness->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PAPER_THICKNESS] = (CBaseCtl*)ctlPaperThickness;

	// Paper option control
	CCtlPaperOption* ctlPaperOption = new CCtlPaperOption();
	ctlPaperOption->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PAPER_OPTION] = (CBaseCtl*)ctlPaperOption;

	// Paper option preview control
	CCtlPaperOptionPreview* ctlPaperOptionPreview = new CCtlPaperOptionPreview();
	ctlPaperOptionPreview->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PAPER_OPTION_PREVIEW] = (CBaseCtl*)ctlPaperOptionPreview;

	// Inspection setting selection control
	CCtlInspectionSettingSelection* ctlInspectionSelection = new CCtlInspectionSettingSelection();
	ctlInspectionSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_INSPECTION_SETTING_SELECTION] = (CBaseCtl*)ctlInspectionSelection;

	// Tone curve settings control
	CCtlToneCurveSettings* ctlToneCurveSettings = new CCtlToneCurveSettings();
	ctlToneCurveSettings->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_TONE_CURVE_SETTINGS] = (CBaseCtl*)ctlToneCurveSettings;

	// Print condition committing control
	CCtlPrintConditionCommitting* ctlPrintConditionCommitting = new CCtlPrintConditionCommitting();
	ctlPrintConditionCommitting->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRINT_CONDITION_COMMITTING] = (CBaseCtl*)ctlPrintConditionCommitting;

	// lock settings control
	CCtlLockSettings* ctlLockSettings = new CCtlLockSettings();
	m_controls[CTLGP_LOCK_SETTINGS] = (CBaseCtl*)ctlLockSettings;

	// close screen control
	CCtlCloseScreen* ctlCloseScreen = new CCtlCloseScreen();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;

	// print setting name
	CCtlPrintSettingsName* ctlPrintSettingName = new CCtlPrintSettingsName();
	m_controls[CTLGP_PRINT_SETTING_NAME] = (CBaseCtl*)ctlPrintSettingName;

	// Dynamic nozzle shift control
	CCtlDNS* ctlDNS = new CCtlDNS();
	ctlDNS->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_DNS] = (CBaseCtl*)ctlDNS;

	// Output color selection control
	CCtlOutputColorSelection* ctlOutputColorSelection = new CCtlOutputColorSelection();
	ctlOutputColorSelection->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_OUTPUTCOLOR_SELECTION] = (CBaseCtl*)ctlOutputColorSelection;

	// Basic setup
	CCtlJobSetting* ctlJobSetting = new CCtlJobSetting();
	ctlJobSetting->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_START_JOB_SETTING] = (CBaseCtl*)ctlJobSetting;

	// ImageStorage selection control
	CCtlImageStorage* ctlImageStorage = new CCtlImageStorage();
	ctlImageStorage->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_IMAGE_STORAGE] = (CBaseCtl*)ctlImageStorage;

	// 変倍コントロール
	CCtlVariableMagnification* ctlVariableMagnification = new CCtlVariableMagnification();
	ctlVariableMagnification->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_VARIABLE_MAGNIFICATION] = (CBaseCtl*)ctlVariableMagnification;
	
	// Primer control
	CCtlPrimer* ctlPrimer = new CCtlPrimer();
	ctlPrimer->SetOwnerControl(CTLGP_GP_MAIN, CTRLID_GB_MAIN);
	m_controls[CTLGP_PRIMER] = (CBaseCtl*)ctlPrimer;
}
