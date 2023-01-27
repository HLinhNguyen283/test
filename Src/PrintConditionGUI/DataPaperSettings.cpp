/*! \file DataPaperSettings.cpp
 *  \brief data paper settings class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
// ==========================================================================
// Include files

#include "stdafx.h"
#include "DataPaperSettings.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "Ini_PrinterParameter.h"
#include "Ini_PrintConditionGUI.h"
#include "Ini_MainteJetInspection.h"
#include "CommonConvertUnit.h"
#include <algorithm>
#include "Jpeg_io.h"

// ==========================================================================

const char* CDataPaperSettings::st_unit_mm = "mm";
const char* CDataPaperSettings::st_unit_inch = "inch";
const char* CDataPaperSettings::st_unit_1_2_inch = "1/2inch";
const char* CDataPaperSettings::st_unit_1_6_inch = "1/6inch";

static const double		PaperWeightMin = 0.0;			//!< 坪量最小値
static const double		PaperWeightMax = 300.0;			//!< 坪量最大値

/*! \brief constructor
*/
CDataPaperSettings::CDataPaperSettings() :
	m_SystemSetting(NULL),
	m_PaperDB_Callbacks(NULL),
	m_JM_Callbacks(NULL),
	m_PaperLengthUnits(),
	m_PaperWidth(0),
	m_PaperThickness(0),
	m_CurrentPaperWidth(0),
	m_CurrentPaperThickness(0),
	m_PaperWidthMin(0),
	m_PaperWidthMax(0),
	m_PaperWidthDef(0),
	m_PaperLengthMin(0),
	m_PaperLengthMax(0),
	m_PaperLengthDef(0),
	m_PaperThicknessMin(0),
	m_PaperThicknessMax(0),
	m_PaperThicknessDef(0),
	m_PatternIntervalMin(0),
	m_PatternIntervalMax(0),
	m_PatternIntervalDef(0),
	m_PrePrintMarkOffset(0),
	m_PrintSideSelection(DEF_PRINT_FRONT),
	m_DynamicNozzleShift(0),
	m_CurrentDynamicNozzleShift(0),
	m_nonInspectionFrontLeft(0),
	m_nonInspectionFrontRight(0),
	m_nonInspectionBackLeft(0),
	m_nonInspectionBackRight(0),
	m_RipHeight(0),
	m_currentPaperWeight(0),
	m_paperWeight(0)
{
	memset(m_PrintConditionOrSection, 0, sizeof(m_PrintConditionOrSection));
	memset(&m_PaperLength, 0, sizeof(ST_PDB_PAPER_SIZE_H));
	memset(&m_PaperOption, 0, sizeof(ST_PDB_PAPER_OPTION));
	memset(&m_CurrentPaperLength, 0, sizeof(ST_PDB_PAPER_SIZE_H));
	memset(&m_CurrentPaperOption, 0, sizeof(ST_PDB_PAPER_OPTION));
	memset(&m_BitmapPaperReview, 0, sizeof(m_BitmapPaperReview));
	m_NonInspectionList[DEF_PRINT_FRONT].clear();
	m_NonInspectionList[DEF_PRINT_BACK].clear();
	memset(&m_jobVerifyMark, 0x00, sizeof(m_jobVerifyMark));
	memset(m_PaperResoX, 0x00, sizeof(m_PaperResoX));
	memset(m_PaperResoY, 0x00, sizeof(m_PaperResoY));

	m_DuplicateJIAreaSettings = FALSE;
	m_NonInspectionAreaDef = 0.0;
}

/*! \brief destructor
*/
CDataPaperSettings::~CDataPaperSettings()
{
}

/*! \brief initialize
 *  \retval TRUE success
 *  \retval FALSE fail
*/
BOOL CDataPaperSettings::Initialize(CDataSystemSetting* inSystemSetting)
{
	if (!inSystemSetting)
		return FALSE;
	m_SystemSetting = inSystemSetting ;
	m_PaperDB_Callbacks = inSystemSetting->GetPaperDB_Callbacks();
	m_JM_Callbacks = inSystemSetting->GetJobManager_Callbacks();

	m_PaperLengthUnits.push_back(UNIT_MODE_MM);
	m_PaperLengthUnits.push_back(UNIT_MODE_INCH);
	m_PaperLengthUnits.push_back(UNIT_1PER2_INCH);
	m_PaperLengthUnits.push_back(UNIT_1PER6_INCH);

//	2014.3.5 matsuda: move to EnterScreen
/*	CIni_PrintConditionGUI ini_PrintConditionGUI;
	if (ini_PrintConditionGUI.Initialize(TRUE))
	{
		m_PaperWidthMin = ini_PrintConditionGUI.GetPaperWidthMin();
		m_PaperWidthMin = (long)(ConvertUnit((double)m_PaperWidthMin / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperWidthMax = ini_PrintConditionGUI.GetPaperWidthMax();
		m_PaperWidthMax = (long)(ConvertUnit((double)m_PaperWidthMax / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperWidthDef = ini_PrintConditionGUI.GetPaperWidthDefault();
		m_PaperWidthDef = (long)(ConvertUnit((double)m_PaperWidthDef / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);

		m_PaperLengthMin = ini_PrintConditionGUI.GetPaperLengthMin();
		m_PaperLengthMax = ini_PrintConditionGUI.GetPaperLengthMax();
		m_PaperLengthDef = ini_PrintConditionGUI.GetPaperLengthDefault();

		m_PaperThicknessMin = ini_PrintConditionGUI.GetPaperThicknessMin();
		m_PaperThicknessMin = (long)(ConvertUnit((double)m_PaperThicknessMin / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperThicknessMax = ini_PrintConditionGUI.GetPaperThicknessMax();
		m_PaperThicknessMax = (long)(ConvertUnit((double)m_PaperThicknessMax / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperThicknessDef = ini_PrintConditionGUI.GetPaperThicknessDefault();
		m_PaperThicknessDef = (long)(ConvertUnit((double)m_PaperThicknessDef / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);

		m_PatternIntervalMin = ini_PrintConditionGUI.GetPatternIntervalMin();
		m_PatternIntervalMax = ini_PrintConditionGUI.GetPatternIntervalMax();
		m_PatternIntervalDef = ini_PrintConditionGUI.GetPatternIntervalDefault();

		m_PrePrintMarkOffset = ini_PrintConditionGUI.GetPrePrintMarkOffset();
		m_PrePrintMarkOffset = (long)(ConvertUnit((double)m_PrePrintMarkOffset / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		
		ini_PrintConditionGUI.Finalize();
	}
*/
//	2014.3.5

	return TRUE;
}

/*! \brief finalize
*/
void CDataPaperSettings::Finalize()
{
	ReleasePaperPreview();
}

/**
 *	Check the user change the paper settings or not
 *	@retval	true : change
 *	@retval	false : not change
**/
bool CDataPaperSettings::IsChanged()
{
	if (m_CurrentPaperWidth != m_PaperWidth)
		return true;
	if (memcmp(&m_CurrentPaperLength, &m_PaperLength, sizeof(ST_PDB_PAPER_SIZE_H)) != 0)
		return true;
	if (m_CurrentPaperThickness != m_PaperThickness)
		return true;
	if (memcmp(&m_CurrentPaperOption, &m_PaperOption, sizeof(ST_PDB_PAPER_OPTION)) != 0)
		return true;
	if (m_CurrentDynamicNozzleShift != m_DynamicNozzleShift)
		return true;

	if (m_currentPaperWeight != m_paperWeight)
	{
		return true;
	}

	return false;
}

/**
 *	Check the user change the paper size or not
 *	@retval	true : change
 *	@retval	false : not change
**/
bool CDataPaperSettings::IsPaperSizeChanged()
{
	//paper width is changed
	if (m_CurrentPaperWidth != m_PaperWidth)
		return true;
	//paper height (page feed length) is changed
	//or paper height (page feed length) specified state(ON->OFF or OFF->ON)
	if (memcmp(&m_CurrentPaperLength, &m_PaperLength, sizeof(ST_PDB_PAPER_SIZE_H)) != 0)
		return true;
	return false;
}

/**
 *	Notify to save paper settings for select Print Condition or current section
 *	@retval	true : succeed
 *	@retval	false : failed
**/
bool CDataPaperSettings::Save()
{
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (!m_JM_Callbacks->JM_SetPaperSizeW(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_PaperWidth))
			return false;
		if (!m_JM_Callbacks->JM_SetPaperSizeH_PrintConditionGUI(m_PrintConditionOrSection, m_PaperLength))
			return false;
		if (!m_JM_Callbacks->JM_SetPaperThickness(m_PrintConditionOrSection, m_PaperThickness))
			return false;
		if (!m_JM_Callbacks->JM_SetPaperOption(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_PaperOption))
			return false;
		m_JM_Callbacks->JM_SetDNSStatus(m_PrintConditionOrSection, m_DynamicNozzleShift);
	}
	else
	{
		if (!m_PaperDB_Callbacks->PDB_SetPaperSizeW(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_PaperWidth))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetPaperSizeH(m_PrintConditionOrSection, m_PaperLength))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetPaperThickness(m_PrintConditionOrSection, m_PaperThickness))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetPaperOption(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_PaperOption))
			return false;
		if (!m_PaperDB_Callbacks->PDB_SetDNSStatus(m_PrintConditionOrSection, m_DynamicNozzleShift))
			return false;
	}

	// 坪量の設定
	m_PaperDB_Callbacks->PDB_SetPaperWeight(m_PrintConditionOrSection, m_paperWeight);

	UpdatePrintCondition(m_PrintConditionOrSection);
	return true;
}

/**
	@brief	ジョブ印刷条件設定コピーイベント
	@param[out]	outPaperSettings : コピー対象の用紙設定各種 (構造体)
*/
void CDataPaperSettings::OnCopyJobPrintCondition(PrintConditionGUI::ST_COPY_TARGET_PAPER_SETTINGS &outPaperSettings)
{
	// ※現在選択中の値を取得
	// 用紙幅
	outPaperSettings.paperWidth = m_PaperWidth;
	outPaperSettings.paperWidthUnit = m_SystemSetting->GetSystemUnit();

	// 紙送り設定
	outPaperSettings.paperLength = m_PaperLength;

	// 紙厚
	outPaperSettings.paperThickness = m_PaperThickness;

	// 紙オプション
	outPaperSettings.paperOption = m_PaperOption;

	// DNS
	outPaperSettings.dns = m_DynamicNozzleShift;
}

/**
 *	Update the current selection paper type in case the user change the print condition name
 *	@param[in]	inPrintConditionOrSection : the print condition name or section id
 *	@note Notes
 *	
 *		In case the Print Condition GUI is started from:
 *			-	System Setting Screen
 *					inPrintConditionOrSection = print condition name
 *			-	Job List Screen
 *					inPrintConditionOrSection = section id
**/
void CDataPaperSettings::UpdatePrintCondition(const char* inPrintConditionOrSection)
{
	if (inPrintConditionOrSection)
	{
		strcpy_s(m_PrintConditionOrSection, inPrintConditionOrSection);
		InitPaperWidth();
		InitPaperLength();
		InitPaperThickness();
		InitPaperOption();
		InitPaperPreview();
		InitDynamicNozzleShift();
		InitNonInspectionAreaList();
		InitNonInspectionAreaInfo();
		InitVerifyMark();
		InitPaperReso();
		InitRIPHeight();
		InitPaperWeight();
	}
}

/**
 *	Initialize current paper width
**/
void CDataPaperSettings::InitPaperWidth()
{
	m_CurrentPaperWidth = 0;
	m_PaperWidth = m_PaperWidthDef;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetPaperSizeW(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_CurrentPaperWidth))
		{
			m_PaperWidth = m_CurrentPaperWidth;
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPaperSizeW(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_CurrentPaperWidth))
		{
			m_PaperWidth = m_CurrentPaperWidth;
		}
	}
}

/**
 *	Initialize current paper length
**/
void CDataPaperSettings::InitPaperLength()
{
	memset(&m_CurrentPaperLength, 0, sizeof(ST_PDB_PAPER_SIZE_H));
	m_PaperLength.IS_SIZE_H = FALSE;
	m_PaperLength.UNIT = UNIT_MODE_MM;
	m_PaperLength.SIZE_H = m_PaperLengthDef;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetPaperSizeH(m_PrintConditionOrSection, m_CurrentPaperLength))
		{
			memcpy(&m_PaperLength, &m_CurrentPaperLength, sizeof(ST_PDB_PAPER_SIZE_H));
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPaperSizeH(m_PrintConditionOrSection, m_CurrentPaperLength))
		{
			memcpy(&m_PaperLength, &m_CurrentPaperLength, sizeof(ST_PDB_PAPER_SIZE_H));
		}
	}
}

/**
 *	Initialize current paper thickness
**/
void CDataPaperSettings::InitPaperThickness()
{
	m_CurrentPaperThickness = 0;
	m_PaperThickness = m_PaperThicknessDef;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetPaperThickness(m_PrintConditionOrSection, m_CurrentPaperThickness))
		{
			m_PaperThickness = m_CurrentPaperThickness;
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPaperThickness(m_PrintConditionOrSection, m_CurrentPaperThickness))
		{
			m_PaperThickness = m_CurrentPaperThickness;
		}
	}
}

/**
 *	Initialize current paper option
**/
void CDataPaperSettings::InitPaperOption()
{
	memset(&m_CurrentPaperOption, 0, sizeof(ST_PDB_PAPER_OPTION));
	memset(&m_PaperOption, 0, sizeof(ST_PDB_PAPER_OPTION));
	m_PaperOption.REP_PATER_CNT = m_PatternIntervalDef;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetPaperOption(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_CurrentPaperOption))
		{
			memcpy(&m_PaperOption, &m_CurrentPaperOption, sizeof(ST_PDB_PAPER_OPTION));
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetPaperOption(m_PrintConditionOrSection, m_SystemSetting->GetSystemUnit(), m_CurrentPaperOption))
		{
			memcpy(&m_PaperOption, &m_CurrentPaperOption, sizeof(ST_PDB_PAPER_OPTION));
		}
	}
}

/**
 *	Initialize paper preview
**/
void CDataPaperSettings::InitPaperPreview()
{
	ReleasePaperPreview();
	m_BitmapPaperReview[DEF_PRINT_FRONT] = LoadJpegImage(DEF_PRINT_FRONT);
	m_BitmapPaperReview[DEF_PRINT_BACK] = LoadJpegImage(DEF_PRINT_BACK);
}

/**
 *	Finalize paper preview
**/
void CDataPaperSettings::ReleasePaperPreview()
{
	if (m_BitmapPaperReview[DEF_PRINT_FRONT])
	{
		DeleteObject(m_BitmapPaperReview[DEF_PRINT_FRONT]);
		m_BitmapPaperReview[DEF_PRINT_FRONT] = NULL;
	}
	if (m_BitmapPaperReview[DEF_PRINT_BACK])
	{
		DeleteObject(m_BitmapPaperReview[DEF_PRINT_BACK]);
		m_BitmapPaperReview[DEF_PRINT_BACK] = NULL;
	}
}

/**
 *	Initialize current dynamic nozzle shift
**/
void CDataPaperSettings::InitDynamicNozzleShift()
{
	m_CurrentDynamicNozzleShift = false;
	m_DynamicNozzleShift = false;
	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		if (m_JM_Callbacks->JM_GetDNSStatus(m_PrintConditionOrSection, m_CurrentDynamicNozzleShift))
		{
			m_DynamicNozzleShift = m_CurrentDynamicNozzleShift;
		}
	}
	else
	{
		if (m_PaperDB_Callbacks->PDB_GetDNSStatus(m_PrintConditionOrSection, m_CurrentDynamicNozzleShift))
		{
			m_DynamicNozzleShift = m_CurrentDynamicNozzleShift;
		}
	}
}

/*! \brief Get a paper width
 *  \retval a paper width
*/
long CDataPaperSettings::GetPaperWidth()
{
	return m_PaperWidth;
}

/*! \brief Set a paper width
 *  \param inWidth: a paper width
*/
void CDataPaperSettings::SetPaperWidth(long inWidth)
{
	if (inWidth < m_PaperWidthMin)
		inWidth = m_PaperWidthMin;
	if (inWidth > m_PaperWidthMax)
		inWidth = m_PaperWidthMax;
	m_PaperWidth = inWidth;
}

/*! \brief Get if the feeding length of paper set or not.
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPaperSettings::GetUsePaperFeedLengthSetting()
{
	return m_PaperLength.IS_SIZE_H ? true : false;
}

/*! \brief Set if the feeding length of paper set or not.
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetUsePaperFeedLengthSetting(bool inUse)
{
	m_PaperLength.IS_SIZE_H = inUse;
}

/*! \brief Get a feeding length of paper
 *  \retval a feeding length of paper
*/
long CDataPaperSettings::GetPaperFeedLength()
{
	return m_PaperLength.SIZE_H;
}

/*! \brief Set a feeding length of paper
 *  \param inLength: a feeding length of paper
*/
void CDataPaperSettings::SetPaperFeedLength(long inLength)
{
	long min = (long)(ConvertUnit((double)m_PaperLengthMin / 1000, UNIT_MODE_MM, m_PaperLength.UNIT) * 1000);
	long max = (long)(ConvertUnit((double)m_PaperLengthMax / 1000, UNIT_MODE_MM, m_PaperLength.UNIT) * 1000);

	//Page length range： 0, min value ~ max value
	if(0 != inLength) {
		if (inLength < min)
			inLength = min;
		if (inLength > max)
			inLength = max;
	}

	m_PaperLength.SIZE_H = inLength;
}

/*! \brief Get a paper thickness
 *  \retval a paper thickness
*/
long CDataPaperSettings::GetPaperThickness()
{
	return m_PaperThickness;
}

/*! \brief Set a paper thickness
 *  \param inThickness: a paper thickness
*/
void CDataPaperSettings::SetPaperThickness(long inThickness)
{
	if (inThickness < m_PaperThicknessMin)
		inThickness = m_PaperThicknessMin;
	if (inThickness > m_PaperThicknessMax)
		inThickness = m_PaperThicknessMax;
	m_PaperThickness = inThickness;
}

/**
 *	Set a current paper thickness
 *	@param[in] inThickness: a paper thickness
**/
void CDataPaperSettings::SetCurrentPaperThickness(long inThickness)
{
	if (inThickness < m_PaperThicknessMin)
		inThickness = m_PaperThicknessMin;
	if (inThickness > m_PaperThicknessMax)
		inThickness = m_PaperThicknessMax;
	m_CurrentPaperThickness = inThickness;
}

/*! \brief Get an item of unit list for feeding paper
 *  \param inItemID: an item ID(0-N)
 *  \param outUnit: a pointer to get an unit for feeding
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPaperSettings::GetPaperFeedUnitList(long inItemID, const char** outUnit)
{
	if (0 <= inItemID && inItemID < (long)m_PaperLengthUnits.size())
	{
		switch(m_PaperLengthUnits[inItemID])
		{
		case UNIT_MODE_MM:		*outUnit = st_unit_mm;			break;
		case UNIT_MODE_INCH:	*outUnit = st_unit_inch;		break;
		case UNIT_1PER2_INCH:	*outUnit = st_unit_1_2_inch;	break;
		case UNIT_1PER6_INCH:	*outUnit = st_unit_1_6_inch;	break;
		}
		return true;
	}
	return false;
}

/*! \brief Get a selection unit for feeding paper
 *  \param outItemID: a pointer to g et an item ID(0-N) of unit for feeding length
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPaperSettings::GetSelectPaperFeedUnit(long* outItemID)
{
	if (outItemID)
	{
		vector<eUNIT_MODE>::iterator it = find(m_PaperLengthUnits.begin(), m_PaperLengthUnits.end(), m_PaperLength.UNIT);
		if (m_PaperLengthUnits.end() != it)
		{
			*outItemID = it - m_PaperLengthUnits.begin();
			return true;
		}
	}
	return false;
}

/*! \brief Set a selection unit for feeding paper
 *  \param inItemID: an item ID(0-N)
*/
void CDataPaperSettings::SetSelectPaperFeedUnit(long inItemID)
{
	if (0 <= inItemID && inItemID < (long)m_PaperLengthUnits.size())
	{
		m_PaperLength.SIZE_H = (long)(ConvertUnit((double)m_PaperLength.SIZE_H / 1000, m_PaperLength.UNIT, m_PaperLengthUnits[inItemID]) * 1000);
		m_PaperLength.UNIT = m_PaperLengthUnits[inItemID];
	}
}

/*! \brief Get ON/OFF of a tractor hole
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPaperSettings::GetOptionTractorHole()
{
	return m_PaperOption.IS_HOLES ? true : false;
}

/*! \brief Set ON/OFF of a tractor hole
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetOptionTractorHole(bool inUse)
{
	m_PaperOption.IS_HOLES = inUse;
	if (inUse)
	{
		SetOptionTractorholesAndPerforations(false);
	}
}

/*! \brief Get ON/OFF of a tractor hole and a perforations
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPaperSettings::GetOptionTractorHolesAndPerforations()
{
	return m_PaperOption.IS_HOLES_PERFORATIONS ? true: false;
}

/*! \brief Set ON/OFF of a tractor hole and a perforations
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetOptionTractorholesAndPerforations(bool inUse)
{
	m_PaperOption.IS_HOLES_PERFORATIONS = inUse;
	if (inUse)
	{
		SetOptionTractorHole(false);
	}
}

/*! \brief Get a interval of perforation
 *  \retval a interval of perforation
*/
long CDataPaperSettings::GetOptionPerforationInterval()
{
	return m_PaperOption.REP_PATER_CNT;
}

/*! \brief Set a interval of perforation
 *  \param inInterval: a interval of perforation
*/
void CDataPaperSettings::SetOptionPerforationInterval(long inInterval)
{
	if (inInterval < m_PatternIntervalMin)
		inInterval = m_PatternIntervalMin;
	if (inInterval > m_PatternIntervalMax)
		inInterval = m_PatternIntervalMax;
	m_PaperOption.REP_PATER_CNT = inInterval;
}

/*! \brief Get ON/OFF of a preprinted mark
 *  \param outMarkSide * a pointer to get a side of the pre-printed mark.
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPaperSettings::GetOptionPreprintedMark(DEF_PRINT_SIDE* outMarkSide)
{
	if (outMarkSide)
	{
		*outMarkSide = m_PaperOption.PRE_MARK_SIDE;
	}
	return m_PaperOption.IS_PRE_MARK ? true : false;
}

/*! \brief Set ON/OFF of a preprinted mark
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetOptionPreprintedMark(bool inUse)
{
	m_PaperOption.IS_PRE_MARK = inUse;
}

/*! \brief Get an offset of preprinted mark
 *  \retval an offset of preprinted mark
*/
long CDataPaperSettings::GetOptionPreprintedMarkOffset()
{
	return m_PaperOption.PRE_MARK_OFFSET;
}

/*! \brief Set an offset of preprinted mark
 *  \param inOffset: an offset of preprinted mark
*/
void CDataPaperSettings::SetOptionPreprintedMarkOffset(long inOffset)
{
	if (inOffset < 0)
		inOffset = 0;
	if (inOffset > m_PaperWidth - m_PrePrintMarkOffset)
		inOffset = m_PaperWidth - m_PrePrintMarkOffset;
	m_PaperOption.PRE_MARK_OFFSET = inOffset;
}

/*! \brief Get ON/OFF of a preprinted paper
 *  \retval true: ON
 *  \retval false: OFF
*/
bool CDataPaperSettings::GetOptionPreprintedPaper()
{
	return m_PaperOption.IS_PRE_MARK_PAPER ? true : false;
}

/*! \brief Set ON/OFF of a preprinted paper
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetOptionPreprintedPaper(bool inUse)
{
	m_PaperOption.IS_PRE_MARK_PAPER = inUse;
}

/*! \brief Notify to initialize Select preprinted mark side dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPaperSettings::InitSelectPreprintedMarkSideDlg()
{
	return true;
}

/*! \brief Set a selection side of preprinted mark
 *  \param inSide: a print side(front, back)
*/
void CDataPaperSettings::SetSelectPreprintedMarkSide_SelectDlg(DEF_PRINT_SIDE inSide)
{
	m_PaperOption.PRE_MARK_SIDE = inSide;
}


/*! \brief Notify to exit Select preprinted mark side dialog
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPaperSettings::ExitSelectPreprintedMarkSideDlg()
{
	return true;
}

/*! \brief Set if dynamic nozzle shifting is ON or not.
 *  \param inUse: ON/OFF(true: ON, false: OFF)
*/
void CDataPaperSettings::SetDynamicNozzleShift(bool inUse)
{
	m_DynamicNozzleShift = inUse;
}

/*! \brief Get dynamic nozzle shifting setting
 *  \retval dynamic nozzle shifting setting (true: ON, false: OFF)
*/
bool CDataPaperSettings::GetDynamicNozzleShift()
{
	return m_DynamicNozzleShift;
}

/**
 *	Notify to enter PrintCondition screen
 *	@retval	true: succeed
 *	@retval	false: failed
**/
bool CDataPaperSettings::EnterScreen(const char* inPrintConditionOrSectionID)
{
//	2014.3.5 matsuda: moved from Initialize
	CIni_PrintConditionGUI ini_PrintConditionGUI;
	if (ini_PrintConditionGUI.Initialize(TRUE))
	{
		m_PaperWidthMin = ini_PrintConditionGUI.GetPaperWidthMin();
		m_PaperWidthMin = (long)(ConvertUnit((double)m_PaperWidthMin / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperWidthMax = ini_PrintConditionGUI.GetPaperWidthMax();
		m_PaperWidthMax = (long)(ConvertUnit((double)m_PaperWidthMax / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperWidthDef = ini_PrintConditionGUI.GetPaperWidthDefault();
		m_PaperWidthDef = (long)(ConvertUnit((double)m_PaperWidthDef / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);

		m_PaperLengthMin = ini_PrintConditionGUI.GetPaperLengthMin();
		m_PaperLengthMax = ini_PrintConditionGUI.GetPaperLengthMax();
		m_PaperLengthDef = ini_PrintConditionGUI.GetPaperLengthDefault();

		m_PaperThicknessMin = ini_PrintConditionGUI.GetPaperThicknessMin();
		//m_PaperThicknessMin = (long)(ConvertUnit((double)m_PaperThicknessMin / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperThicknessMax = ini_PrintConditionGUI.GetPaperThicknessMax();
		//m_PaperThicknessMax = (long)(ConvertUnit((double)m_PaperThicknessMax / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		m_PaperThicknessDef = ini_PrintConditionGUI.GetPaperThicknessDefault();
		//m_PaperThicknessDef = (long)(ConvertUnit((double)m_PaperThicknessDef / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);

		m_PatternIntervalMin = ini_PrintConditionGUI.GetPatternIntervalMin();
		m_PatternIntervalMax = ini_PrintConditionGUI.GetPatternIntervalMax();
		m_PatternIntervalDef = ini_PrintConditionGUI.GetPatternIntervalDefault();

		m_PrePrintMarkOffset = ini_PrintConditionGUI.GetPrePrintMarkOffset();
		m_PrePrintMarkOffset = (long)(ConvertUnit((double)m_PrePrintMarkOffset / 1000, DEF_LENGTH_MM, m_SystemSetting->GetSystemUnit()) * 1000);
		
		ini_PrintConditionGUI.Finalize();
	}
//	2014.3.5

	if (m_SystemSetting->IsStartedFromJobListScreen())
	{
		m_JM_Callbacks->JM_GetSectionIDToSelectPrintCondition(m_PrintConditionOrSection);
	}
	else
	{
		string printConditionOrSection ("");
		m_PaperDB_Callbacks->PDB_GetCurrentPrintCondition(printConditionOrSection);
		strcpy(m_PrintConditionOrSection, printConditionOrSection.c_str());
	}
	UpdatePrintCondition(m_PrintConditionOrSection);

	m_NonInspectionAreaDef = ini_PrintConditionGUI.GetDuplicateInitNonInspectionAreaDefault();

	return true;
}

/*! \brief Notify to exit Print condition screen
 *  \retval true: succeed
 *  \retval false: failed
*/
bool CDataPaperSettings::ExitScreen()
{
	ReleasePaperPreview();
	return true;
}

/*! \brief Set a selection side to display a preview image that both a paper and paper options are drawn.
 *  \param inSide: a selection side(front, back)
*/
void CDataPaperSettings::SetPaperPreviewSide(DEF_PRINT_SIDE inSide)
{
	m_PrintSideSelection = inSide;
}


/*! \brief Get a selection side to display a preview image that both a paper and paper options are drawn.
 *  \retval a selection side(front, back)
*/
DEF_PRINT_SIDE CDataPaperSettings::GetPaperPreviewSide()
{
	return m_PrintSideSelection;
}

/*! \brief Get a preview image that both a paper and paper options are drawn.
 *  \param inSide: a side to get a imgae(front/back)
 *  \param outImage: a pointer to get a preview image object
 *  \retval true: getting succeed
 *  \retval false: getting failed
*/
bool CDataPaperSettings::GetPaperPreview(DEF_PRINT_SIDE inSide, HBITMAP* outImage)
{
	if(!outImage || DEF_PRINT_BOTH == inSide) {
		return false;
	}

	*outImage = m_BitmapPaperReview[inSide];
	return true;
}

/*! \fn HBITMAP LoadJpegImage(char *file)
 *  \brief JPEG	JPEG file
 *  \param file	file path
 *  \return hbitmap file
*/
HBITMAP CDataPaperSettings::LoadJpegImage(DEF_PRINT_SIDE inSide)
{
	//Get file path
	UINT lPrinter = (DEF_PRINT_BACK == inSide) ? PRINTER2 : PRINTER1;
	eComSystemType eSystemType = m_SystemSetting->IsSelf(lPrinter + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
	char scanFileName[256];
	char localPath[256] = {0};
	string fileName = "";

	if(!m_SystemSetting->IsStartedFromJobListScreen()) {
		//Get file name
//		if(!m_PaperDB_Callbacks->PDB_GetScanFileName(m_PrintConditionOrSection, eSystemType, scanFileName)) {
		if(!m_PaperDB_Callbacks->PDB_GetScanFileName(m_PrintConditionOrSection, eSystemType, fileName)) {
			return false;
		}
	}else {
		string printConditionName = "";
		if(!m_JM_Callbacks->JM_GetPrintCondition(m_PrintConditionOrSection, printConditionName)) {
			return false;
		}
		//Get file name
		if(!m_PaperDB_Callbacks->PDB_GetScanFileName(printConditionName.c_str(), eSystemType, fileName)) {
			return false;
		}
	}
	strcpy_s(scanFileName, fileName.c_str());

	_snprintf_s(localPath , sizeof(localPath), _TRUNCATE, "%s%s" , GetModuleDirectory() , scanFileName);
	//~

	HBITMAP hBmp;
	ImageData image;
	PBYTE buf, src, dst;
	long x, y;
	
	image.data = NULL;
	image.width = 0;
	JPEG_Input(localPath, &image);
	if (!image.width) return NULL;

	image.data = malloc(image.width * image.height * 3);
	if (!image.data) return NULL;

	JPEG_Input(localPath, &image);

	buf = (PBYTE)malloc(image.width * image.height * 4);
	if (!buf) {
		free(image.data);
		return NULL;
	}
	memset(buf, 0x00, image.width * image.height * 4);


	src = (PBYTE)image.data;
	dst = buf;
	for (y=0; y<image.height; y++) {
		for (x=0; x<image.width; x++, src+=3) {
			*(dst++) = src[2];
			*(dst++) = src[1];
			*(dst++) = src[0];
			*(dst++) = 0;
		}
	}

	hBmp = CreateBitmap(image.width, image.height, 4
		, 8, buf);

	free(image.data);
	free(buf);

	return hBmp;
}

/*! \brief Initialize non spection list
*/
void CDataPaperSettings::InitNonInspectionAreaList()
{
	//reset info
	m_NonInspectionList[DEF_PRINT_FRONT].clear();
	bool isDed = m_SystemSetting->IsDED();
	if(isDed) {
		m_NonInspectionList[DEF_PRINT_BACK].clear();
	}

	string printConditionName = m_PrintConditionOrSection;
	if(m_SystemSetting->IsStartedFromJobListScreen()) {
		if(!m_JM_Callbacks->JM_GetPrintCondition(m_PrintConditionOrSection, printConditionName)) {
			return;
		}
	}

	//DED/SES - Front
	eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
	if(!m_PaperDB_Callbacks->PDB_GetNonInspectionAreaList(printConditionName.c_str(), systemType, m_NonInspectionList[DEF_PRINT_FRONT])) {
		WriteToLogBuf(LOG_DEBUG, "Failed  InitNonInspectionAreaList.PDB_GetNonInspectionAreaList error.(FRONT)");
	}

	//DED - Back
	if(isDed) {
		systemType = m_SystemSetting->IsSelf(DEF_PRINT_BACK + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		if(!m_PaperDB_Callbacks->PDB_GetNonInspectionAreaList(printConditionName.c_str(), systemType, m_NonInspectionList[DEF_PRINT_BACK])) {
			WriteToLogBuf(LOG_DEBUG, "Failed  InitNonInspectionAreaList.PDB_GetNonInspectionAreaList error.(BACK)");
		}
	}
}

/*! \brief Initialize non inspection information
*/
void CDataPaperSettings::InitNonInspectionAreaInfo()
{
	string printConditionName = m_PrintConditionOrSection;
	if(m_SystemSetting->IsStartedFromJobListScreen()) {
		if(!m_JM_Callbacks->JM_GetPrintCondition(m_PrintConditionOrSection, printConditionName)) {
			return;
		}
	}

	CIni_MainteJetInspection iniMainteJetInspection;
	if(FALSE == iniMainteJetInspection.Initialize(TRUE)) {
		return;
	}

	//DED/SES - Front
	eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
	if(!m_PaperDB_Callbacks->PDB_GetNonInspectionAreaTable(printConditionName.c_str(), systemType, m_nonInspectionFrontLeft, m_nonInspectionFrontRight)) {
		double value = 0.0f;
		if(TRUE == m_PaperOption.IS_HOLES || TRUE == m_PaperOption.IS_HOLES_PERFORATIONS) {
			value = iniMainteJetInspection.GetDefNonInspectionArea_Punch();
		} else {
			value = iniMainteJetInspection.GetDefNonInspectionArea_NoPunch();
		}
		value = (double)value / 100;
		m_nonInspectionFrontLeft = value;
		m_nonInspectionFrontRight = value;
	}
	

	//DED -  Back
	if(m_SystemSetting->IsDED()) {
		systemType = m_SystemSetting->IsSelf(DEF_PRINT_BACK + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		if(!m_PaperDB_Callbacks->PDB_GetNonInspectionAreaTable(printConditionName.c_str(), systemType, m_nonInspectionBackLeft, m_nonInspectionBackRight)) {

			double backValue = 0.0f;
			if(TRUE == m_PaperOption.IS_HOLES || TRUE == m_PaperOption.IS_HOLES_PERFORATIONS) {
				backValue = iniMainteJetInspection.GetDefNonInspectionArea_Punch();
			} else {
				backValue = iniMainteJetInspection.GetDefNonInspectionArea_NoPunch();
			}
			backValue = (double)backValue / 100;
			m_nonInspectionBackLeft = backValue;
			m_nonInspectionBackRight = backValue;
		}
	}

	// Release
	iniMainteJetInspection.Finalize();
}

/*! \brief Get rect of non inspection area
 *	\ param[IN] inItemID: item id in the list of mask
 *	\ param[IN] inSide: The printer side (front/Back)
 *  \ param[OUT] outRect: The rect of non inspection area
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataPaperSettings::GetNonInspectionRect(long inItemID, DEF_PRINT_SIDE inSide, RECT& outRect)
{
	if(!m_SystemSetting->IsDED() && DEF_PRINT_BACK == inSide || inItemID < 0) {
		return FALSE;
	}

	if (inItemID < (long)m_NonInspectionList[inSide].size()) {
		STRUCT_NON_INSPECTION_AREA nonInspectionArea = m_NonInspectionList[inSide].at(inItemID);
		LONG left = (LONG)(nonInspectionArea.START_POS_X + 0.5);
		LONG top = (LONG)(nonInspectionArea.START_POS_Y + 0.5);
		LONG right = (LONG)(nonInspectionArea.END_POS_X + 0.5);
		LONG bottom = (LONG)(nonInspectionArea.END_POS_Y + 0.5);

		SetRect(&outRect, left, top, right, bottom);
		return TRUE;
	}
	return FALSE;
}

/*! \brief Get info of a eliminated area of  a printer side
 *  \ param[in] inSide : The printer side
 *  \ param[in] inIsLeft : Check if non inspection area is left or right
 *  \ param[out] outLeft : The left pos of eliminated rect
 *  \ param[out] outRight : The right pos of eliminated rect
*/
void CDataPaperSettings::GetEliminatedInfo(DEF_PRINT_SIDE inSide, bool inIsLeft, double& outLeft, double& outRight)
{
	//Get system length unit
	DEF_LENGTH_UNIT systemUnit = DEF_LENGTH_MM;
	m_SystemSetting->GetSystemUnitLength(&systemUnit);
	//~

	//Calculate paper width
	double lPaperWidth = (double)GetPaperWidth() / 1000;
	if(DEF_LENGTH_MM != systemUnit) {
		lPaperWidth = ConvertUnit(lPaperWidth, systemUnit, DEF_LENGTH_MM);
	}
	lPaperWidth = min(max(lPaperWidth, 0), m_PaperWidthMax);
	//~

	//Calculate left and right position
	if (DEF_PRINT_FRONT == inSide){
		if (inIsLeft) {
			outRight = m_nonInspectionFrontLeft;
		} else {
			outLeft = lPaperWidth - m_nonInspectionFrontRight;
			outRight = lPaperWidth;
		}
	} else {

		if (inIsLeft) {
			outLeft = lPaperWidth;
			outRight = lPaperWidth - m_nonInspectionBackLeft;
		} else {
			outLeft = m_nonInspectionBackRight;
		}
		// Update position to top-left for display on GUI
		double width = abs(outRight - outLeft);
		outLeft = lPaperWidth - outLeft;
		outRight = outLeft + width;
	}
	//~
}

/*! \brief Get maximum and minimum value of paper size.
 *	\ param[OUT] outMaxWidth: Maximum width
 *	\ param[OUT] outMinWidth: Minimum width
 *  \ param[OUT] outMaxHeight: Maximum height
 *  \ param[OUT] outMinHeight: Minimum height
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataPaperSettings::GetMaxMinPaperSize(long& outMaxWidth, long& outMinWidth, long& outMaxHeight, long& outMinHeight)
{
	//Width
	outMaxWidth = m_PaperWidthMax;
	outMinWidth = m_PaperWidthMin;

	//Height
	outMaxHeight = m_PaperLengthMax;
	outMinHeight = m_PaperLengthMin;
	return TRUE;
}

/*! \brief Initialize verify mark
*/
void CDataPaperSettings::InitVerifyMark()
{
	//reset info
	memset(&m_jobVerifyMark, 0x00, sizeof(m_jobVerifyMark));

	if(m_SystemSetting->IsStartedFromJobListScreen()) {
		// Get system length unit
		DEF_LENGTH_UNIT systemUnit = DEF_LENGTH_MM;
		m_SystemSetting->GetSystemUnitLength(&systemUnit);

		// Get Job Verify mark information
		bool isSetupEditingData = false;	//Don't refer to the data of the job for setup editing in this plugin.
		if(!m_JM_Callbacks->JM_GetVerifyMark_LayoutImage(m_PrintConditionOrSection, systemUnit, &m_jobVerifyMark, isSetupEditingData)) {
			WriteToLogBuf(LOG_DEBUG, "Failed  InitVerifyMark.JM_GetVerifyMark_LayoutImage error.");
		}
	}
}

/*! \brief Get prining start mark information
 *	\ param[IN] inSide: The printer side
 *  \ param[OUT] outOffsetX: Offset X
 *  \ param[OUT] outOffsetY: Offset Y
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataPaperSettings::GetVerifyMarkOffset(DEF_PRINT_SIDE inSide, double& outOffsetX, double& outOffsetY)
{
	if(!m_SystemSetting->IsDED() && DEF_PRINT_BACK == inSide) {
		return FALSE;
	}

	//Get offset
	switch(inSide){
	case DEF_PRINT_FRONT:
		{
			outOffsetX = m_jobVerifyMark.offsetX[0];
			outOffsetY = 0.0;
		}
		break;
	case DEF_PRINT_BACK:
		{
			outOffsetX = m_jobVerifyMark.offsetX[1];
			outOffsetY = m_jobVerifyMark.offsetY;
		}
		break;
	}
	return TRUE;
}

/*! \brief Check if printing start mark is enabled or not
 *  \ retVal TRUE: Enable
 *  \ retVal FALSE: Disable
*/
BOOL CDataPaperSettings::GetEnablePrintStartMark()
{
	return m_jobVerifyMark.printStartMark;
}

/**
 *	Get selection targets to verify mark
 *	\ note Process to initialize the member variable
 *	
 *		JM_GetVerifyMark_LayoutImage
 * \ retVal status of verifying mark(TRUE:ON, FALSE:OFF)
**/
BOOL CDataPaperSettings::GetVerifyMarkTarget()
{
	return m_jobVerifyMark.verifyMark;
}

/*! \brief Get the paper length unit
 *  \ retVal the unit of peper length
*/
eUNIT_MODE CDataPaperSettings::GetPaperLengthUnit()
{
	return m_PaperLength.UNIT;
}

/**
 *	Get duplicate JetInspection area settings.
 *	@retval	false:Not duplicate, true:Duplicate
**/
bool CDataPaperSettings::GetDuplicateJIAreaSettings()
{
	return m_DuplicateJIAreaSettings;
}

/*! \brief Set duplicate JetInspection area settings.
 *  \param inState State of duplicate JetInspection area settings. (false:Not duplicate, true:Duplicate).
*/
void CDataPaperSettings::SetDuplicateJIAreaSettings(bool inState)
{
	m_DuplicateJIAreaSettings = inState;
}

/**
 *	Get duplicate JetInspection area settings.
 *	@retval	false:Not duplicate, true:Duplicate
**/
double CDataPaperSettings::GetDuplicateInitNonInspectionAreaDefault()
{
	return m_NonInspectionAreaDef;
}

/*! \brief Initialize paper resolution
*/
void CDataPaperSettings::InitPaperReso()
{
	string printConditionName = m_PrintConditionOrSection;
	if(m_SystemSetting->IsStartedFromJobListScreen()) {
		//Get printcondition name if starting from job screen
		if(!m_JM_Callbacks->JM_GetPrintCondition(m_PrintConditionOrSection, printConditionName)) {
			return;
		}
	}

	eComSystemType systemType = m_SystemSetting->IsSelf(DEF_PRINT_FRONT + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
	//Get paper resolution of FRONT side
	if(!m_PaperDB_Callbacks->PDB_GetScanFileReso(printConditionName.c_str(), systemType, m_PaperResoX[DEF_PRINT_FRONT], m_PaperResoY[DEF_PRINT_FRONT])) {
		return;
	}

	if(m_SystemSetting->IsDED()) {
		systemType = m_SystemSetting->IsSelf(DEF_PRINT_BACK + 1) ? COM_SYSTEM_TYPE_SELF : COM_SYSTEM_TYPE_PART;
		//Get paper resolution of BACK side
		if(!m_PaperDB_Callbacks->PDB_GetScanFileReso(printConditionName.c_str(), systemType, m_PaperResoX[DEF_PRINT_BACK], m_PaperResoY[DEF_PRINT_BACK])) {
			return;
		}
	}
}

/*! \brief Get the paper resolution
 *	\ param[IN] inSide: The printer side
 *  \ param[OUT] outResoX: the resolution follow X coordinate
 *  \ param[OUT] outResoY: the resolution follow Y coordinate
 *  \ retVal TRUE: success
 *  \ retVal FALSE: failure
*/
BOOL CDataPaperSettings::GetPaperReso(DEF_PRINT_SIDE inSide, double& outResoX, double& outResoY)
{
	if(!m_SystemSetting->IsDED() && DEF_PRINT_BACK == inSide) {
		return FALSE;
	}

	outResoX = m_PaperResoX[inSide];
	outResoY = m_PaperResoY[inSide];
	return TRUE;
}

/*! \brief Initialize the RIP height of specified job
*/
void CDataPaperSettings::InitRIPHeight()
{
	if(m_SystemSetting->IsStartedFromJobListScreen()) {
		m_JM_Callbacks->JM_GetRipSizeH(m_PrintConditionOrSection, m_RipHeight);
	}
}

/*! \brief Get the RIP height of specified job
 *  \ param[OUT] outRipHeight: the RIP height of specified job
*/
void CDataPaperSettings::GetRipSizeH(double& outRipHeight)
{
	//From ini file
	outRipHeight = (m_PaperLengthDef > 0 ? m_PaperLengthDef : m_PaperLengthMax) / 1000;

	if(m_SystemSetting->IsStartedFromJobListScreen() && m_RipHeight > 0) {
		outRipHeight = (double) ((m_RipHeight / 1000) + 0.5);
	}
}

// 坪量の初期化
void CDataPaperSettings::InitPaperWeight()
{
#if 0
		setPaperWeight(100.0);
#endif
	m_currentPaperWeight = 0;
	m_PaperDB_Callbacks->PDB_GetPaperWeight(m_PrintConditionOrSection, m_currentPaperWeight);
	m_paperWeight = m_currentPaperWeight;
}

// 坪量の取得
double CDataPaperSettings::getPaperWeight()
{
	return m_paperWeight;
}

// 坪量の設定
void CDataPaperSettings::setPaperWeight(const double inPaperWeight)
{
	long convert = static_cast<long>(inPaperWeight * 10);
	double paperWeight = static_cast<double>(convert) / 10;

	if (inPaperWeight < PaperWeightMin)
	{
		paperWeight = PaperWeightMin;
	}
	if (inPaperWeight > PaperWeightMax)
	{
		paperWeight = PaperWeightMax;
	}
	m_paperWeight = paperWeight;
}