/*! \file DataPrintMode.cpp
 *  \brief data print mode class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "DataPrintMode.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "Ini_PrinterParameter.h"
#include "Ini_PrintConditionGUI.h"
#include "Ini_LayoutImageGUI.h"
#include <algorithm>
#include "CommonUiMsg_OP.h"
#include "DataIF.h"
#include <sstream>

// ==========================================================================

/*! \brief constructor
*/
CDataPrintMode::CDataPrintMode() : 
	m_SystemSetting(NULL),
	m_PaperDB_Callbacks(NULL),
	m_JM_Callbacks(NULL),
	m_PrintModes(),
	m_PrintSpeeds(),
	m_Tensions(),
	m_SelectPrintMode(NULL),
	m_SelectPrintSpeed(NULL),
	m_CurrentPrintSpeed(0),
	m_SelectTension(NULL),
	m_CurrentTension(0),
	m_TensionDef(0)
{
	memset(&m_PrintConditionOrSection, 0, sizeof(m_PrintConditionOrSection));
	memset(&m_CurrentPrintMode, 0, sizeof(ST_PDB_PRINT_MODE));

	m_data = NULL;
	isSupport150mpm = true;
	isSupport100mpm = true;
	isSupportPrintModeForCoatedOffset = true;
	m_SelectedID = 0;
}

/*! \brief destructor
*/
CDataPrintMode::~CDataPrintMode()
{
}

/*! \brief initialize
 *  \retval TRUE success
 *  \retval FALSE fail
*/
BOOL CDataPrintMode::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
{
	if (!inSystemSetting)
		return FALSE;
	if (!inDataIF)
		return FALSE;
	m_SystemSetting = inSystemSetting ;
	m_data = inDataIF;
	m_PaperDB_Callbacks = inSystemSetting->GetPaperDB_Callbacks();
	m_JM_Callbacks = inSystemSetting->GetJobManager_Callbacks();

	CIni_PrintConditionGUI iniPrintConditionGUI;
	if (iniPrintConditionGUI.Initialize(TRUE))
	{
		m_TensionDef = iniPrintConditionGUI.GetTensionDefault();
		iniPrintConditionGUI.Finalize();
	}

	InitTensionList();

	return TRUE;
}

/*! \brief finalize
*/
void CDataPrintMode::Finalize()
{
	ReleasePrintSpeedList();
	ReleaseTensionList();
}

/**
 *	Notify to enter PrintCondition screen
 *	@param[in]	inPrintConditionOrSectionID : the print condition name or section
 *	@retval	true: succeed
 *	@retval	false: failed
**/
bool CDataPrintMode::EnterScreen(const char* inPrintConditionOrSectionID)
{
	InitPrintModeList();

	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		m_JM_Callbacks->JM_GetSectionIDToSelectPrintCondition(m_PrintConditionOrSection);
	}
	else
	{
		string	printConditionOrSection ("");
		m_PaperDB_Callbacks->PDB_GetCurrentPrintCondition(printConditionOrSection);
		strcpy(m_PrintConditionOrSection, printConditionOrSection.c_str());
	}
	UpdatePrintCondition(m_PrintConditionOrSection);
	return true;
}

/*! \brief Notify to exit Print condition screen
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPrintMode::ExitScreen()
{
	ReleasePrintSpeedList();
	ReleasePrintModeList();
	return true;
}

/**
 *	Getting the select print mode
 *	@retval	the select print mode
**/
PrintMode* CDataPrintMode::GetSelectPrintMode()
{
	return m_SelectPrintMode;
}

/**
 *	Getting the select print speed
 *	@retval	the select print speed
**/
PrintSpeed* CDataPrintMode::GetSelectPrintSpeed()
{
	return m_SelectPrintSpeed;
}

/**
 *	Getting the select print speed value
 *	@retval	the select print speed value
**/
long CDataPrintMode::GetSelectPrintSpeedValue()
{
	if(m_SelectPrintSpeed) {
		return m_SelectPrintSpeed->Value;
	}
	return 0;
}

/**
 *	Getting the select paper tension
 *	@retval	the select paper tension
**/
Tension* CDataPrintMode::GetSelectPaperTension()
{
	return m_SelectTension;
}

/**
 *	Getting the select paper tension value
 *	@retval	the select paper tension value
**/
long CDataPrintMode::GetSelectPaperTensionValue()
{
	if(m_SelectTension) {
		return m_SelectTension->Value;
	}
	return 0;
}

/**
 *	Check the user change the print mode or not
 *	@retval	true : change
 *	@retval	false : not change
**/
bool CDataPrintMode::IsChanged()
{
	if (!m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (NULL == m_SelectPrintMode)
			return true;
		if (strcmp(m_SelectPrintMode->Data.RESOURCE_ID, m_CurrentPrintMode.RESOURCE_ID) != 0)
			return true;
		//if (strcmp(m_SelectPrintMode->Data.SCREENING_FAMILY, m_CurrentPrintMode.SCREENING_FAMILY) != 0)
		//	return true;
		//if (!m_SelectPrintMode->Data.CMYK_PRN_RESO_X != m_CurrentPrintMode.CMYK_PRN_RESO_X)
		//	return true;
		//if (!m_SelectPrintMode->Data.CMYK_PRN_RESO_Y != m_CurrentPrintMode.CMYK_PRN_RESO_Y)
		//	return true;
		//if (!m_SelectPrintMode->Data.CMYK_RIP_RESO_X != m_CurrentPrintMode.CMYK_RIP_RESO_X)
		//	return true;
		//if (!m_SelectPrintMode->Data.CMYK_RIP_RESO_Y != m_CurrentPrintMode.CMYK_RIP_RESO_Y)
		//	return true;
		//if (!m_SelectPrintMode->Data.ELS_PRN_RESO_X != m_CurrentPrintMode.ELS_PRN_RESO_X)
		//	return true;
		//if (!m_SelectPrintMode->Data.ELS_PRN_RESO_Y != m_CurrentPrintMode.ELS_PRN_RESO_Y)
		//	return true;
		//if (!m_SelectPrintMode->Data.ELS_RIP_RESO_X != m_CurrentPrintMode.ELS_RIP_RESO_X)
		//	return true;
		//if (!m_SelectPrintMode->Data.ELS_RIP_RESO_Y != m_CurrentPrintMode.ELS_RIP_RESO_Y)
		//	return true;
	}

	if (NULL == m_SelectPrintSpeed)
		return true;
	if (m_SelectPrintSpeed->Value != m_CurrentPrintSpeed)
		return true;

	if (NULL == m_SelectTension)
		return true;
	if (m_SelectTension->Value != m_CurrentTension)
		return true;

	return false;
}

/**
 *	Notify to save printmode for select Print Condition or current section
 *	@retval	true : succeed
 *	@retval	false : failed
**/
bool CDataPrintMode::Save()
{
	auto isFlushingPatternConversion = [&]()-> bool
	{
		bool	ret = false;
		// check if Flushing pattern conversion to be acquired from LayoutImageGUI.ini is enbaled.
		CIni_LayoutImageGUI iniLayoutImageGUI(UserModeGUI_GetLengthUnit());
		iniLayoutImageGUI.Initialize(TRUE);
		ret = iniLayoutImageGUI.GetFlushingPatternConversion();
		iniLayoutImageGUI.Finalize();
		return ret;
	};
	auto isPrintModeHighSpeed = [&](const ST_PDB_PRINT_MODE inPrintMode)-> bool
	{
		std::string	printModeName = inPrintMode.RESOURCE_ID;
		// At the timing of this implementation, HighSpeed is PRINT_MODE_NAME_4 and HighSpeed2 is PRINT_MODE_NAME_6.
		if ((printModeName.compare("PRINT_MODE_NAME_4") == 0) || (printModeName.compare("PRINT_MODE_NAME_6") == 0)) {
			return true;
		}
		return false;
	};
	auto isUpdateFlushingPattern = [&](const ST_JOB_FLUSHING_PATTERN inStJobFlusingPattern, DEF_FLUSHING_PATTERN& outFlushingPattern)-> bool
	{
		bool	ret = false;
		switch(inStJobFlusingPattern.flushingPattern){
		case DEF_FLUSHING_PATTERN_LINE_1:
			outFlushingPattern = DEF_FLUSHING_PATTERN_LINE_2;
			ret = true;
			break;
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
			outFlushingPattern = DEF_FLUSHING_PATTERN_LINE_2_STAR;
			ret = true;
			break;
		}
		return ret;
	};

	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (NULL == m_SelectPrintSpeed || NULL == m_SelectTension)
			return false;
//		if (!m_JM_Callbacks->JM_SetModeResoSpeed(m_PrintConditionOrSection, DEF_MODE_RESO_SPEED_M, m_SelectPrintSpeed->Value))
		if (!m_JM_Callbacks->JM_SetModeResoSpeed(m_PrintConditionOrSection, DEF_MODE_RESO_SPEED_100, m_SelectPrintSpeed->Value))
			return false;
		if (!m_JM_Callbacks->JM_SetPaperTension(m_PrintConditionOrSection, m_SelectTension->Value))
			return false;
	}
	else
	{
		if (NULL == m_SelectPrintMode || NULL == m_SelectPrintSpeed || NULL == m_SelectTension)
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetPrintMode(m_PrintConditionOrSection, m_SelectPrintMode->Data))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetModeResoSpeed(m_PrintConditionOrSection,
														m_SelectPrintMode->Data.CMYK_PRN_RESO_X, m_SelectPrintMode->Data.CMYK_PRN_RESO_Y,
//														DEF_MODE_RESO_SPEED_M, m_SelectPrintSpeed->Value))
														DEF_MODE_RESO_SPEED_100, m_SelectPrintSpeed->Value))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetPaperTension(m_PrintConditionOrSection, m_SelectTension->Value))
			return false;

		// Change flushing pattern type from 'Line1' to 'Line2' when the conditions match.
		// Reason:
		// - The flushing pattern that the conditions match have been replace by Printer(Firmware).
		// - If it can be not set, the result of PrintSimulator does not match the result of Printer.

		// Check the flushing pattern conversion to be acquired from LayoutImageGUI.ini is disabled and the condition if print mode is HighSpeed or HighSpeed2.
		if (isFlushingPatternConversion() && isPrintModeHighSpeed(m_SelectPrintMode->Data)) {

			DEF_LENGTH_UNIT systemUnit = DEF_LENGTH_MM;
			m_SystemSetting->GetSystemUnitLength(&systemUnit);

			// Get the layout image information from the print condition
			ST_JOB_IMAGE_POSITION			layoutImagePosition;
			ST_JOB_FLUSHING_PATTERN			layoutFlushingPattern;
			ST_JOB_FLUSHING_PATTERN_ADD		layoutFlushingPatternAdd;
			ST_JOB_FLUSHING_PATTERN_ADD2	layoutFlushingPatternAdd2;
			ST_JOB_VERIFY_MARK				layoutVerifyMark;
			ST_JOB_POSTPROCESS				layoutPostProcess;
			ST_JOB_POSTPROCESS_ADD			layoutPostProcessAdd;
			BOOL							layoutVerticalAlignmentPPFLSPTN;
			m_PaperDB_Callbacks->PDB_GetLayoutImageParameterWithPrintConditionName(
				m_PrintConditionOrSection,
				systemUnit,
				layoutImagePosition,
				layoutFlushingPattern,
				layoutFlushingPatternAdd,
				layoutFlushingPatternAdd2,
				layoutVerifyMark,
				layoutPostProcess,
				layoutPostProcessAdd,
				layoutVerticalAlignmentPPFLSPTN);

			// Check the condition if flushing pattern type is 'Line1' or 'Line1+Star'.
			DEF_FLUSHING_PATTERN	outFlushingPattern = layoutFlushingPattern.flushingPattern;
			if (isUpdateFlushingPattern(layoutFlushingPattern, outFlushingPattern)) {

				// Change flushing pattern type from 'Line1' to 'Line2' when the conditions match.
				layoutFlushingPattern.flushingPattern = outFlushingPattern;

				// Set the layout image information to the print condition
				m_PaperDB_Callbacks->PDB_SetLayoutImageParameterWithPrintConditionName(
					m_PrintConditionOrSection,
					systemUnit,
					layoutImagePosition,
					layoutFlushingPattern,
					layoutFlushingPatternAdd,
					layoutFlushingPatternAdd2,
					layoutVerifyMark,
					layoutPostProcess,
					layoutPostProcessAdd,
					layoutVerticalAlignmentPPFLSPTN);
			}

		}
	}
	UpdatePrintCondition(m_PrintConditionOrSection);
	return true;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outPrintSpeedValue : 印刷速度
	@param[out]	outPrintSpeedUnit  : 印刷速度単位
	@param[out]	outPaperTensionValue : テンション
*/
void CDataPrintMode::OnCopyJobPrintCondition(long &outPrintSpeedValue, eMODE_RESO_SPEED_UNIT &outPrintSpeedUnit, long &outPaperTensionValue)
{
	// 印刷速度
	outPrintSpeedValue = GetSelectPrintSpeedValue();

	// 印刷速度単位 (固定: 0.01m/min)
	outPrintSpeedUnit = DEF_MODE_RESO_SPEED_100;

	// テンション
	outPaperTensionValue = GetSelectPaperTensionValue();
}

/**
 *	Initialize the print mode list
**/
void CDataPrintMode::InitPrintModeList()
{
	ReleasePrintModeList();

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long	mOutpugColorMode = 0;
	pData->GetSelectOutputColor(&mOutpugColorMode);

	CIni_PrinterParameter printerParameter;
	if (printerParameter.Initialize(TRUE))
	{
		for (long i = 0; true; ++i)
		{
			string name;
			if (!printerParameter.getPrintModeName(i, name))
			{
				break;
			}
			long printSpeedID;
			printerParameter.GetPrintSpeedID(name.c_str(), printSpeedID);

			double resolutionX = 0, resolutionY = 0;
			printerParameter.GetPrintModePrintReso(i, &resolutionX, &resolutionY);

			if ((printerParameter.getMaxPrintSpeed(static_cast<long>(resolutionX), static_cast<long>(resolutionY), printSpeedID) == 15000) && (pData->IsSupport150mpm() == false)) {
				isSupport150mpm = false;
				continue;
			}
			// 100mpmオプション無効時は"標準2"はマスクする
			if ((printerParameter.getMaxPrintSpeed(static_cast<long>(resolutionX), static_cast<long>(resolutionY), printSpeedID) == 10000) && (pData->IsSupport100mpm() == false)) {
				isSupport100mpm = false;
				continue;
			}
			// コート紙用印刷モード搭載無効の時は「標準 （オフセットコート紙）」を省く.
			// 「標準 （オフセットコート紙）」の印刷モードは印字階調（動作パラメータ：200, 値：0x24）で判断する
			long	printStep = 0;
			printerParameter.GetPrintStep(name, printStep);
			if ((printStep == 36) && (pData->IsSupportPrintModeForCoatedOffset() == false)) {
				isSupportPrintModeForCoatedOffset = false;
				continue;
			}

			// CMYK + Muliti color
			if (DEF_OUTPUT_COLOR_MODE_CMYKRO == (DEF_OUTPUT_COLOR_MODE)mOutpugColorMode)
			{
				// 追加色無しの印刷モードはマスクする
				if (!printerParameter.IsSupportAdditionalColor(i))
				{
					continue;
				}
			}
			// CMYK color
			else
			{
				// 追加色有りの印刷モードはマスクする
				if (printerParameter.IsSupportAdditionalColor(i))
				{
					continue;
				}
			}

			PrintMode* printmode = (PrintMode*)IO_malloc(sizeof(PrintMode));
			printmode->ID = i;
			printerParameter.GetPrintModeMenuName(i, printmode->DisplayName, sizeof(printmode->DisplayName));
			strcpy_s(printmode->Data.RESOURCE_ID, name.c_str());
			
			printmode->Data.CMYK_PRN_RESO_X = (long)resolutionX;
			printmode->Data.CMYK_PRN_RESO_Y = (long)resolutionY;

			printerParameter.GetPrintModePrintResoOP(i, &resolutionX, &resolutionY);
			printmode->Data.ELS_PRN_RESO_X = (long)resolutionX;
			printmode->Data.ELS_PRN_RESO_Y = (long)resolutionY;

			printerParameter.GetPrintModeRIPReso(i, &resolutionX, &resolutionY);
			printmode->Data.CMYK_RIP_RESO_X = (long)resolutionX;
			printmode->Data.CMYK_RIP_RESO_Y = (long)resolutionY;

			printerParameter.GetPrintModeRIPResoOP(i, &resolutionX, &resolutionY);
			printmode->Data.ELS_RIP_RESO_X = (long)resolutionX;
			printmode->Data.ELS_RIP_RESO_Y = (long)resolutionY;

			string screening;
			printerParameter.getPrintModeScreening(i, screening);
			strcpy_s(printmode->Data.SCREENING_FAMILY, screening.c_str());

			m_PrintModes.push_back(printmode);
		}
		printerParameter.Finalize();
	}

	::PostMessage(m_SystemSetting->GetPostMessageWindow(), UWM_PRINT_MODE_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
}

/**
 *	Finalize the print mode list
**/
void CDataPrintMode::ReleasePrintModeList()
{
	for (vector<PrintMode*>::iterator it = m_PrintModes.begin(); it != m_PrintModes.end(); ++it)
	{
		IO_free(*it);
	}
	m_PrintModes.clear();
}

/**
 *	Determize the current printmode
**/
void CDataPrintMode::InitSelectPrintMode()
{
	m_SelectPrintMode = NULL;
	memset(&m_CurrentPrintMode, 0, sizeof(m_CurrentPrintMode));
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		bool isSetupEditingData = false;	//Don't refer to the data of the job for setup editing in this plugin.
		if (m_JM_Callbacks->JM_GetPrintMode(m_PrintConditionOrSection, m_CurrentPrintMode, isSetupEditingData))
		{
			m_SelectPrintMode = GetPrintModeByName(m_CurrentPrintMode.RESOURCE_ID);
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPrintMode(m_PrintConditionOrSection, m_CurrentPrintMode))
		{
			m_SelectPrintMode = GetPrintModeByName(m_CurrentPrintMode.RESOURCE_ID);
		}
	}
}

/**
 *	Initialize the print speed list
**/
void CDataPrintMode::InitPrintSpeedList()
{
	ReleasePrintSpeedList();

	if (m_SelectPrintMode)
	{
		CIni_PrinterParameter printerParameter;
		if (printerParameter.Initialize(TRUE))
		{
			for (long i = 0; true; ++i)
			{
				// PrinterParameter.iniから選択印刷モードに紐づく印刷速度一覧のIDを取得する
				long printSpeedID;
				if (printerParameter.GetPrintSpeedID(m_SelectPrintMode->Data.RESOURCE_ID, printSpeedID) == false) {
					std::stringstream errLog;
					errLog << "CDataPrintMode::InitPrintSpeedList :Failed to GetPrintSpeedID [" << m_SelectPrintMode->Data.RESOURCE_ID << "]";
					WriteToLogBuf(LOG_DEBUG, errLog.str());
					break;
				}

				long speed;
				if (!printerParameter.getPrintSpeed(i, m_SelectPrintMode->Data.CMYK_PRN_RESO_X, m_SelectPrintMode->Data.CMYK_PRN_RESO_Y, printSpeedID, speed))
				{
					break;
				}
				PrintSpeed* printSpeed = (PrintSpeed*)IO_malloc(sizeof(PrintSpeed));
				printSpeed->ID = i;
				printSpeed->Value = speed;
				_snprintf_s(printSpeed->DisplayName, _TRUNCATE, "%ld", printSpeed->Value / 100);
				m_PrintSpeeds.push_back(printSpeed);
			}
			printerParameter.Finalize();
		}
	}
//	::PostMessage(GetGenSetupDialog(), UWM_PRINT_SPEED_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
	::PostMessage(m_SystemSetting->GetPostMessageWindow(), UWM_PRINT_SPEED_LIST, CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM, 0);
}

/**
 *	Finalize the print speed list
**/
void CDataPrintMode::ReleasePrintSpeedList()
{
	for (vector<PrintSpeed*>::iterator it = m_PrintSpeeds.begin(); it != m_PrintSpeeds.end(); ++it)
	{
		IO_free(*it);
	}
	m_PrintSpeeds.clear();
}

/**
 *	Determine current print speed
**/
void CDataPrintMode::InitSelectPrintSpeed()
{
	m_SelectPrintSpeed = NULL;
	m_CurrentPrintSpeed = 0;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
//		if (m_JM_Callbacks->JM_GetModeResoSpeed(m_PrintConditionOrSection, DEF_MODE_RESO_SPEED_M, m_CurrentPrintSpeed))
		if (m_JM_Callbacks->JM_GetModeResoSpeed(m_PrintConditionOrSection, DEF_MODE_RESO_SPEED_100, m_CurrentPrintSpeed))
		{
			m_SelectPrintSpeed = GetPrintSpeedByValue(m_CurrentPrintSpeed);
		}
	}
	else
	{
		if (m_SelectPrintMode)
		{
			if (m_PaperDB_Callbacks->PDB_GetModeResoSpeed(m_PrintConditionOrSection,
															m_SelectPrintMode->Data.CMYK_PRN_RESO_X,
															m_SelectPrintMode->Data.CMYK_PRN_RESO_Y,
//															DEF_MODE_RESO_SPEED_M,
															DEF_MODE_RESO_SPEED_100,
															m_CurrentPrintSpeed))
			{
				m_SelectPrintSpeed = GetPrintSpeedByValue(m_CurrentPrintSpeed);
			}
		}
	}
}

/**
 *	Initialize the tension list
**/
void CDataPrintMode::InitTensionList()
{
	ReleaseTensionList();

	CIni_PrintConditionGUI iniPrintConditionGUI;
	if (iniPrintConditionGUI.Initialize(TRUE))
	{
		long min = iniPrintConditionGUI.GetTensionMin();
		long max = iniPrintConditionGUI.GetTensionMax();
		for (long id = 0, value = min; value <= max; ++value, ++id)
		{
			Tension* tension = (Tension*)IO_malloc(sizeof(Tension));
			tension->ID = id;
			tension->Value =  value;
			m_Tensions.push_back(tension);
		}
		iniPrintConditionGUI.Finalize();
	}
}

/**
 *	Finalize the tension list
**/
void CDataPrintMode::ReleaseTensionList()
{
	for (vector<Tension*>::iterator it = m_Tensions.begin(); it != m_Tensions.end(); ++it)
	{
		IO_free(*it);
	}
	m_Tensions.clear();
}

/**
 *	Determine the current tension
**/
void CDataPrintMode::InitSelectTension()
{
	m_SelectTension = GetTensionByValue(m_TensionDef);
	m_CurrentTension = 0;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetPaperTension(m_PrintConditionOrSection, m_CurrentTension))
		{
			m_SelectTension = GetTensionByValue(m_CurrentTension);
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPaperTension(m_PrintConditionOrSection, m_CurrentTension))
		{
			m_SelectTension = GetTensionByValue(m_CurrentTension);
		}
	}
}

/**
 *	Getting the print mode by print mode name
 *	@param[in]	inPrintModeName : print mode name
 *	@retval	the print mode
**/
PrintMode* CDataPrintMode::GetPrintModeByName(const char* inPrintModeName)
{
	vector<PrintMode*>::iterator it = find_if(m_PrintModes.begin(), m_PrintModes.end(), FindPrintMode(inPrintModeName));
	if (m_PrintModes.end() != it)
	{
		m_SelectedID = distance(m_PrintModes.begin(), it);
		return *it;
	}
	return NULL;
}

/*! \brief Get an item of print mode list.
 *  \param inItemID: an item ID(0-N)
 *  \param outMode: a pointer to get a print mode.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintMode::GetPrintModeList(long inItemID, const char** outMode)
{
	if(0 <= inItemID && inItemID < (long)m_PrintModes.size())
	{
		*outMode = m_PrintModes[inItemID]->DisplayName;
		return true;
	}
	return false;
}

/*! \brief Set a selection print mode.
 *  \param inItemID: an item ID(0-N)
*/
void CDataPrintMode::SetSelectPrintMode(long inItemID)
{
	if(0 <= inItemID && inItemID < (long)m_PrintModes.size())
	{
		m_SelectedID = inItemID;
		m_SelectPrintMode = m_PrintModes[inItemID];
		InitPrintSpeedList();
		InitSelectPrintSpeed();
	}
}

/*! \brief Get a selection print mode
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection print mode,
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintMode::GetSelectPrintMode(long* outItemID)
{
	if (m_SelectPrintMode)
	{
		// 2017.04.06
		// 変更前：m_SelectPrintMode->IDを返す
		// 変更後：m_PrintModes登録リストのインデックスを返す
		// 理由　："高速2"と"標準2"はオプションのため、非表示（未登録）の可能性がある。
		//         非表示（未登録）の場合、m_SelectPrintMode->IDだとオーバーフローになる。
		//
		vector<PrintMode*>::iterator it = find(m_PrintModes.begin(), m_PrintModes.end(), m_SelectPrintMode);
		if (m_PrintModes.end() != it)
		{
			*outItemID = it - m_PrintModes.begin();
			return true;
		}
	}
	return false;
}

PrintSpeed* CDataPrintMode::GetPrintSpeedByValue(long inValue)
{
	if (m_PrintSpeeds.size() == 0)	return NULL;
	vector<PrintSpeed*>::iterator it = find_if(m_PrintSpeeds.begin(), m_PrintSpeeds.end(), FindPrintSpeed(inValue));
	if (m_PrintSpeeds.end() != it)
	{
		return *it;
	}
	return *m_PrintSpeeds.begin();
}

/*! \brief Get an item of print speed list
 *  \param inItemID: an item ID(0-N)
 *  \param outSpeed: a pointer to get a print speed
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintMode::GetPrintSpeedList(long inItemID, const char** outSpeed)
{
	if (outSpeed &&
		0 <= inItemID && inItemID < (long)m_PrintSpeeds.size())
	{
		*outSpeed = m_PrintSpeeds[inItemID]->DisplayName;
		return true;
	}
	return false;
}

/*! \brief Set a selection print speed
 *  \param inItemID: an item ID(0-N)
*/
void CDataPrintMode::SetSelectPrintSpeed(long inItemID)
{
	if (0 <= inItemID && inItemID < (long)m_PrintSpeeds.size())
	{
		m_SelectPrintSpeed = m_PrintSpeeds[inItemID];
	}
}

/*! \brief Get a selection print speed
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection print speed.
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintMode::GetSelectPrintSpeed(long* outItemID)
{
	if (outItemID && m_SelectPrintSpeed)
	{
		*outItemID = m_SelectPrintSpeed->ID;
		return true;
	}
	return false;
}

Tension* CDataPrintMode::GetTensionByValue(long inValue)
{
	vector<Tension*>::iterator it = find_if(m_Tensions.begin(), m_Tensions.end(), FindTension(inValue));
	if (m_Tensions.end() != it)
	{
		return *it;
	}
	return NULL;
}

/*! \brief Get tension ranges
 *  \param outMin: a pointer to get the minimum of range
 *  \param outMax: a pointer to get the maximum of range
*/
void CDataPrintMode::GetPaperTensionRange(long* outMin, long* outMax)
{
	if (outMin && outMax && m_Tensions.size() > 0)
	{
		*outMin = m_Tensions.front()->Value;
		*outMax = m_Tensions.back()->Value;
	}
}

/*! \brief Set a selection tension
 *  \param inItemID: an item ID(0-N)
*/
void CDataPrintMode::SetSelectPaperTension(long inItemID)
{
	if (0 <= inItemID && inItemID < (long)m_Tensions.size())
	{
		m_SelectTension = m_Tensions[inItemID];
	}
}

/*! \brief Get a selection tension
 *  \param outItemID: a pointer to get the item ID(0-N) of a selection tension
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPrintMode::GetSelectPaperTension(long* outItemID)
{
	if (outItemID && m_SelectTension)
	{
		*outItemID = m_SelectTension->ID;
		return true;
	}
	return false;
}

/**
 *	Update the current print mode, print speed and tension in case the user change the print condition name
 *	@param[in]	inPrintConditionOrSection : the print condition name or section id
 *	@note Notes
 *		In case the Print Condition GUI is started from:
 *			-	System Setting Screen
 *					inPrintConditionOrSection = print condition name
 *			-	Job List Screen
 *					inPrintConditionOrSection = section id
**/
void CDataPrintMode::UpdatePrintCondition(const char* inPrintConditionOrSection)
{
	if (inPrintConditionOrSection)
	{
		strcpy_s(m_PrintConditionOrSection, inPrintConditionOrSection);
		InitPrintModeList();
		InitSelectPrintMode();
		InitPrintSpeedList();
		InitSelectPrintSpeed();
		InitSelectTension();
	}
}

/**
 *	Update the current print mode, print speed and tension in case the user change the output color mode
**/
void CDataPrintMode::UpdateOutputColorMode()
{
	InitPrintModeList();
	InitSelectPrintMode();
	if (m_SelectPrintMode == NULL)
	{
		// 出力色モードの選択肢変更時は前回選択のインデックスをもとに印刷モードを選択状態に設定
		m_SelectPrintMode = m_PrintModes[m_SelectedID];
	}
}
