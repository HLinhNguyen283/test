/*! \file PrintConditionExportXML.cpp
 *  \brief PrintConditionExportXML.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
 *  \date 2022/04/27 cec.k-miyachi #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "PrintConditionExportXML.h"
#include "CommonCBRProc.h"
#include "DataSystemSetting.h"
#include "CommonCBRProc_OP.h"

#include <sstream>
// ==========================================================================

namespace export_xml
{
 //�R���X�g���N�^
PrintCondition::PrintCondition(const SPrintConditionData& inPrintConditionData)
	:m_PrintConditionData(inPrintConditionData)
{
}

// �f�X�g���N�^
PrintCondition::~PrintCondition()
{
}

// ����������̎擾
std::string PrintCondition::GetPrintConditionName() const
{
	return m_PrintConditionData.printConditionName;
}

// �p����ނ̎擾
std::string PrintCondition::GetPaperType() const
{
	return m_PrintConditionData.migrationData.PaperType;
}

// �i���␳���̎擾
std::string PrintCondition::GetLevelTuneName() const
{
	return m_PrintConditionData.migrationData.LevelTuneName;
}

// �p���ݒ���̎擾
bool PrintCondition::GetPaperSetting(SPaperSetting& outPaperSetting) const
{
	auto getUnitModeData = [](const eUNIT_MODE inUnitMode, std::string& outPageFeedSideUnit)
	{
		std::map<eUNIT_MODE, std::string> unitModeList;
		unitModeList.insert(std::make_pair(UNIT_MODE_MM, "mm"));
		unitModeList.insert(std::make_pair(UNIT_MODE_INCH, "inch"));
		unitModeList.insert(std::make_pair(UNIT_1PER2_INCH, "1/2inch"));
		unitModeList.insert(std::make_pair(UNIT_1PER6_INCH, "1/6inch"));

		auto target = unitModeList.find(inUnitMode);
		if(target == unitModeList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible PageFeedSize (Input : " << inUnitMode << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outPageFeedSideUnit = target->second;
	};

	auto getMarkSide = [](const DEF_PRINT_SIDE inMarkSide, std::string& outMarkSide)
	{
		std::map<DEF_PRINT_SIDE, std::string> markSideList;
		markSideList.insert(std::make_pair(DEF_PRINT_FRONT, "Front"));
		markSideList.insert(std::make_pair(DEF_PRINT_BACK, "Back"));
		auto target = markSideList.find(inMarkSide);

		if(target == markSideList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible PageFeedSize (Input : " << inMarkSide << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outMarkSide = target->second;
	};

	try
	{
		SPaperSetting paperSetting;

		// �p����
		paperSetting.m_width = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperSizeW), DEF_LENGTH_MM, DEF_LENGTH_POINT) / 1000.0;

		// �p������
		paperSetting.m_thickness = m_PrintConditionData.migrationData.Thickness;

		// DNS�ݒ�
		paperSetting.m_DNSStatus = m_PrintConditionData.migrationData.DNSStatus;

		// �e���V����
		paperSetting.m_tension = m_PrintConditionData.migrationData.Tension;

		// �p���`���p���̎�
		if(TRUE == m_PrintConditionData.migrationData.PaperOption.IS_HOLES)
		{
			paperSetting.m_punchHole = true;
		}
		// �p���`�� +�~�V���ڗL��p���̎�
		else if(TRUE == m_PrintConditionData.migrationData.PaperOption.IS_HOLES_PERFORATIONS)
		{
			paperSetting.m_punchHole = true;
			paperSetting.m_sewing = true;
		}

		// ���O�}�[�N
		paperSetting.m_marker = (m_PrintConditionData.migrationData.PaperOption.IS_PRE_MARK == TRUE? true : false);

		// ���O���
		paperSetting.m_prePrinted = (m_PrintConditionData.migrationData.PaperOption.IS_PRE_MARK_PAPER == TRUE? true : false);

		// �}�[�N�I�t�Z�b�g(�呖��)
		paperSetting.m_markerOffset = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperOption.PRE_MARK_OFFSET), DEF_LENGTH_MM, DEF_LENGTH_POINT) / 1000.0;

		// �y�[�W����T�C�Y�̒P�ʂ̎擾
		std::string pageFeedSideUnit;
		getUnitModeData(m_PrintConditionData.migrationData.PaperSizeH.UNIT, pageFeedSideUnit);

		// �y�[�W����T�C�Y
		if (TRUE == m_PrintConditionData.migrationData.PaperSizeH.IS_SIZE_H)
		{
			paperSetting.m_pageFeedSize = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperSizeH.SIZE_H), m_PrintConditionData.migrationData.PaperSizeH.UNIT, UNIT_MODE_POINT) / 1000.0;
		}
		else
		{
			paperSetting.m_pageFeedSize = 0.0;
		}

		// �y�[�W����T�C�Y�̒P��
		paperSetting.m_pageFeedSizeUnit = pageFeedSideUnit;

		// �}�[�N�̈����
		std::string markSide;
		getMarkSide(m_PrintConditionData.migrationData.PaperOption.PRE_MARK_SIDE, markSide);
		paperSetting.m_markSide = markSide;
		
		//�~�V���ڃp�^�[���̌J��Ԃ���
		paperSetting.m_repeatSheets = m_PrintConditionData.migrationData.PaperOption.REP_PATER_CNT;

		outPaperSetting = paperSetting;
		return true;
	}
	catch(std::exception& e)
	{
		writeErrorMessage(e.what());
		return false;
	}
}

//������[�h�̎擾
bool PrintCondition::GetPrintMode(std::string& outPrintMode) const
{
	try
	{
		auto target = m_PrintConditionData.printModeDataList.find(m_PrintConditionData.migrationData.PrintMode.RESOURCE_ID);
		if(target == m_PrintConditionData.printModeDataList.end())
		{
			throw std::runtime_error("Not compatible printMode");
		}
		outPrintMode = target->second;
		return true;
	}
	catch(std::exception& e)
	{
		// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
		writeErrorMessage(e.what());
		return false;
	}
}

//������x�̎擾
double PrintCondition::GetPrintSpeed() const
{
	return m_PrintConditionData.migrationData.PrintSpeed;
}

// �������x���
void PrintCondition::GetDryerTemperature(SDryerTemperture& outDryerTemperture) const
{
	SDryerTemperture dryerTemperture;

	// �q�[�g���[���[�������x�i���@�j
	dryerTemperture.m_heatRollerSelf = m_PrintConditionData.migrationData.DryTempPrint.HEAT_TEMP_SELF;

	// �q�[�g���[���[�������x�i���@�j	
	dryerTemperture.m_heatRollerPart = m_PrintConditionData.migrationData.DryTempPrint.HEAT_TEMP_PART;

	// �����A�V�X�g�������x�i���@�j
	dryerTemperture.m_heatFanSelf = m_PrintConditionData.migrationData.DryTempPrint.FAN_TEMP_SELF;

	// �����A�V�X�g�������x�i���@�j
	dryerTemperture.m_heatFanPart = m_PrintConditionData.migrationData.DryTempPrint.FAN_TEMP_PART;

	// ��h�芣�����x1
	dryerTemperture.m_precoater1 =  m_PrintConditionData.migrationData.DryTempPrint.PRE_TEMP_SELF;

	// ��h�芣�����x2
	dryerTemperture.m_precoater2 =  m_PrintConditionData.migrationData.DryTempPrint.PRE_TEMP_PART;

	// NIR�ݒ�i���@�j
	dryerTemperture.m_NirPowerSelf = m_PrintConditionData.migrationData.DryTempPrint.NIR_POWER_SELF;

	// NIR�ݒ�i���@�j
	dryerTemperture.m_NirPowerPart = m_PrintConditionData.migrationData.DryTempPrint.NIR_POWER_PART;

	// �T�u�q�[�g���[���[�������x�i���@�j
	dryerTemperture.m_SubHeatRollerSelf = m_PrintConditionData.migrationData.DryTempPrint.SUB_HEAT_TEMP_SELF;

	// �T�u�q�[�g���[���[�������x�i���@�j	
	dryerTemperture.m_SubHeatRollerPart = m_PrintConditionData.migrationData.DryTempPrint.SUB_HEAT_TEMP_PART;

	outDryerTemperture = dryerTemperture;

}

// ICC�v���Z�b�g���̎擾
void PrintCondition::GetICCPreset(SICCPreset& outICCPreset) const
{
	SICCPreset iccPreset;
	
	// ICC�v���Z�b�g��
	iccPreset.m_descriptiveName = m_PrintConditionData.migrationData.ICCPreset;

	// ICC�v���Z�b�gID
	iccPreset.m_presetID = m_PrintConditionData.migrationData.ICCPresetID;

	outICCPreset = iccPreset;
}

// �W���u��{�ݒ���̎擾
void PrintCondition::GetJobBase(SJobBase& outJobBase) const
{

	SJobBase jobBase;

	// �C���t�H���[�V�����V�[�g���o�͂��邩�ǂ���
	jobBase.m_informationSheet = m_PrintConditionData.migrationData.JobBaseFlushingPage.informationSheet;

	// �t���b�V���O�y�[�W��}�����邩�ǂ���
	jobBase.m_flushingPage = (m_PrintConditionData.migrationData.JobBaseFlushingPage.flushingPageType == DEF_FLUSHING_PAGE_USER? true : false);

	// �t���b�V���O�y�[�W�̃p�^�[����
	jobBase.m_patternName = m_PrintConditionData.migrationData.JobBaseFlushingPage.flushingPage;

	// �t���b�V���O�y�[�W�̑}���Ԋu
	jobBase.m_interval = m_PrintConditionData.migrationData.JobBaseFlushingPage.interval;

	// �W���u�O�ɑ}�����邩�ǂ���
	jobBase.m_preJob = m_PrintConditionData.migrationData.JobBaseFlushingPage.beforeJob;

	// �W���u�O�ɑ}�������
	jobBase.m_preJobCount = m_PrintConditionData.migrationData.JobBaseFlushingPage.printCountOfBeforeJob;

	// �W���u��ɑ}�����邩�ǂ���
	jobBase.m_postJob = m_PrintConditionData.migrationData.JobBaseFlushingPage.afterJob;

	// �t���b�V���O�y�[�W�̃I�t�Z�b�g�i�\�ʁE�������j
	jobBase.m_offsetFront = static_cast<long>(m_PrintConditionData.migrationData.JobBaseFlushingPage.offset[0] * 1000);

	// �t���b�V���O�y�[�W�̃I�t�Z�b�g�i���ʁE�������j
	jobBase.m_offsetBack = static_cast<long>(m_PrintConditionData.migrationData.JobBaseFlushingPage.offset[1] * 1000);

	// �W���u�O�̃N���[�j���O���背�x��
	jobBase.m_cleaningJudgmentLevelBeforeJob = static_cast<long>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.CleaningJudgmentLevelBeforeJob);
	
	// �W���u��̃N���[�j���O���背�x��
	jobBase.m_cleaningJudgmentLevelAfterJob = static_cast<long>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.CleaningJudgmentLevelAfterJob);
	
	// �����~�ݒ�
	jobBase.m_printStopSetting = static_cast<EPrintStopSetting>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.PrintStopSetting);

	outJobBase = jobBase;

}

// ���C�A�E�g���̎擾
bool PrintCondition::GetLayout(SLayout& outLayout) const
{
	auto getCutterMarkAlign = [](const DEF_POSTPROCESS_ALIGNMENT inAliment, long& outCutterMarkAlign)
	{
		switch(inAliment)
		{
		case DEF_POSTPROCESS_LEFT:
			 outCutterMarkAlign = -1;
			 break;

		case DEF_POSTPROCESS_NONE:
			 outCutterMarkAlign = 0;
			 break;

		case DEF_POSTPROCESS_RIGHT:
			 outCutterMarkAlign = 1;
			 break;

		// ��L�ȊO�̏ꍇ
		default:
			throw std::runtime_error("Not compatible CutterMarkerOffset");
		}
	};

	auto getPatternType = [](const DEF_FLUSHING_PATTERN inPattern, std::string& outStr)
	{
		//�L���ȃt���b�V���O�p�^�[����ނ̈ꗗ���쐬����
		std::map<DEF_FLUSHING_PATTERN, std::string> patternTypeList;
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_NONE, "None"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_1, "Line1"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_2, "Line2"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_4, "Line4"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_8, "Line8"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_16, "Line16"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_1, "Star1"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_2, "Star2"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_4, "Star4"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_8, "Star8"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_1_STAR, "Line1+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_2_STAR, "Line2+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_4_STAR, "Line4+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_8_STAR, "Line8+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_16_STAR, "Line16+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_32, "Line32"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_32_STAR, "Line32+Star"));

		auto target = patternTypeList.find(inPattern);
		if(target == patternTypeList.end())
		{
			//���݂��Ȃ��t���b�V���O�p�^�[���̏ꍇ�̓G�N�X�|�[�g�ł��Ȃ�
			std::stringstream errorMessage;
			errorMessage << "Not compatible FlusingPage (Input : " << inPattern << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outStr = target->second;
	};

	auto getLineOptions = [](const ST_JOB_FLUSHING_PATTERN_ADD& inLayoutFlushingPatternAdd, std::string& outStr)
	{
		// �I�v�V����
		std::string optionStr;
		switch(inLayoutFlushingPatternAdd.lineOption)
		{
		case DEF_LINE_OPTION_C0:
			optionStr = "C0-";
			break;
		case DEF_LINE_OPTION_NONE:
			optionStr = "NONE-";
			break;
		case DEF_LINE_OPTION_S0:
			optionStr = "S0-";
			break;
		case DEF_LINE_OPTION_S2:
			optionStr = "S2-";
			break;
		default:
			//���݂��Ȃ��I�v�V�����̏ꍇ�̓G�N�X�|�[�g�ł��Ȃ�
			std::stringstream errorMessage;
			errorMessage << "Not compatible LineOptions (Option : " << inLayoutFlushingPatternAdd.lineOption << ")";
			throw std::runtime_error(errorMessage.str());
		}

		// �I�v�V����2
		std::string optionStr2;
		switch(inLayoutFlushingPatternAdd.lineOption2)
		{
		case DEF_LINE_OPTION2_STANDARD:
			optionStr2 = "STANDARD";
			break;
		case DEF_LINE_OPTION2_CROSS_STITCH:
			optionStr2 = "CROSS_STITCH";
			break;
		default:
			//���݂��Ȃ��I�v�V����2�̏ꍇ�̓G�N�X�|�[�g�ł��Ȃ�
			std::stringstream errorMessage;
			errorMessage << "Not compatible LineOptions (Option2 : " << inLayoutFlushingPatternAdd.lineOption2 << ")";
			throw std::runtime_error(errorMessage.str());
		}

		outStr = optionStr + optionStr2;
	};

	try
	{
		SLayout layout;

		//�摜�̃I�t�Z�b�g�i�\�ʁE�啛�j
		layout.m_offsetFront.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[0].x * 1000);
		layout.m_offsetFront.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[0].y * 1000);

		//�摜�̃I�t�Z�b�g�i���ʁE�啛�j
		layout.m_offsetBack.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[1].x * 1000);
		layout.m_offsetBack.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[1].y * 1000);

		//�T�C�h���������邩�ǂ���
		layout.m_verification = (m_PrintConditionData.migrationData.LayoutVerifyMark.enableVerify == TRUE? true : false);

		//�G���[�ɂ���A���V�[�g��
		layout.m_continuousPageCount = m_PrintConditionData.migrationData.LayoutVerifyMark.errorSheets;

		//����J�n�}�[�N���o�͂��邩�ǂ���
		layout.m_cueMark = (m_PrintConditionData.migrationData.LayoutVerifyMark.printStartMark == TRUE? true : false);

		//�o�[�R�[�h���o�͂��邩�ǂ���
		layout.m_barcode = (m_PrintConditionData.migrationData.LayoutVerifyMark.verifyMark == TRUE? true : false);

		//�o�[�R�[�h�̃I�t�Z�b�g�i�\�ʁE�呖���j
		layout.m_barcodeOffsetFront.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetX[0] * 1000);
		layout.m_barcodeOffsetFront.second = 0; //��������0�Œ�

		//�o�[�R�[�h�̃I�t�Z�b�g�i���ʁE�啛�j
		layout.m_barcodeOffsetBack.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetX[1] * 1000);
		layout.m_barcodeOffsetBack.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetY * 1000);

		//�t���b�V���O�p�^�[��
		std::string patternType;
		getPatternType(m_PrintConditionData.migrationData.LayoutFlushingPattern.flushingPattern, patternType);
		layout.m_flushingPattern = patternType;
		layout.m_flushingPatternDef = m_PrintConditionData.migrationData.LayoutFlushingPattern.flushingPattern;

		// ���C���t���b�V���O�̂��炵��
		layout.m_gapBetweenColors = m_PrintConditionData.migrationData.LayoutFlushingPattern.gap;

		// ���C���t���b�V���O�̃I�t�Z�b�g�i�\�ʁE�������j
		layout.m_lineOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPattern.offsetY[0] * 1000);

		// ���C���t���b�V���O�̃I�t�Z�b�g�i���ʁE�������j
		layout.m_lineOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPattern.offsetY[1] * 1000);

		// �y�[�W���A���C���t���b�V���O
		layout.m_LineGap = (TRUE == m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.enableMultiLine) ? true : false;

		// �y�[�W���A���C���t���b�V���O�Ԋu�i�\�ʁE�������j
		layout.m_LineGapFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.position2ndLine[0] * 1000);

		// �y�[�W���A���C���t���b�V���O�Ԋu�i���ʁE�������j
		layout.m_LineGapBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.position2ndLine[1] * 1000);

		// �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i�\�ʁE�������j
		layout.m_LineGapOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0] * 1000);

		// �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i���ʁE�������j
		layout.m_LineGapOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[1] * 1000);

		// �y�[�W���A���C���t���b�V���O�̕\���ʒu("Top" or "Bottom")
		if(TRUE == m_PrintConditionData.migrationData.LayoutVertivalAlignmentPPFLSPTN)
		{
			layout.m_LinePosition  = "Bottom";
		}
		else
		{
			layout.m_LinePosition  = "Top";
		}		

		// �y�[�W���A���C���t���b�V���O�̃I�v�V�������
		getLineOptions(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd, layout.m_LineOptions);

		// �J�b�^�[�}�[�N�̈����
		layout.m_cutterMark = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.side);

		// �J�b�^�[�}�[�N�̏����V�[�g��
		layout.m_cutterMarkPageCount = m_PrintConditionData.migrationData.LayoutPostProcess.sheets;

		// �J�b�^�[�}�[�N�̃T�C�Y�i�啛�j
		layout.m_cutterMarkSize.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.size.cx * 1000);
		layout.m_cutterMarkSize.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.size.cy * 1000);

		// �J�b�^�[�}�[�N�̃I�t�Z�b�g�i�\�ʁE�呖���j
		layout.m_cutterMarkOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.offsetX * 1000);

		//�J�b�^�[�}�[�N�̃I�t�Z�b�g�i���ʁE�呖���j
		layout.m_cutterMarkOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.offsetX_back * 1000);

		// �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu�i�\�ʁj
		long cutterMarkAlign_Flont = 0;
		getCutterMarkAlign(m_PrintConditionData.migrationData.LayoutPostProcess.alignment, cutterMarkAlign_Flont);
		layout.m_cutterMarkAlignFront = cutterMarkAlign_Flont;

		// �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu�i���ʁj
		long cutterMarkAlign_Back = 0;
		getCutterMarkAlign(m_PrintConditionData.migrationData.LayoutPostProcess.alignment_back, cutterMarkAlign_Back);
		layout.m_cutterMarkAlignBack = cutterMarkAlign_Back;

		// ADD --> #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
		layout.m_adjustmentSheetLength = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetLength * 1000);
		layout.m_afterJobSheets = m_PrintConditionData.migrationData.LayoutPostProcessAdd.afterJobSheets;
		layout.m_adjustmentSheetAfterJob = m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob;
		layout.m_adjustmentSheetSizeEnabled = static_cast<bool>(m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetSizeEnabled);
		// ADD <-- #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j

		outLayout = layout;
		return true;
	}
	catch(std::exception& e)
	{
		// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
		writeErrorMessage(e.what());
		return false;
	}
}

// �g�[���J�[�u���擾
bool PrintCondition::GetToneCurve(SToneCurve& outToneCurve, bool inIsFront) const
{
	try
	{
		AnchorPoint anchorPoint;
		OneColorPoint oneColorPoint;
		SToneCurve toneCurve;

		// �\�ʁE���ʂ̋��
		std::string side;

		 // ����@���ǂ���
		long isPart = 0;

		getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isPart);

		toneCurve.m_side = side;

		// �g�[���J�[�u��
		toneCurve.m_descriptiveName = m_PrintConditionData.migrationData.DotgainCurve[isPart].DOTGAIN_NAME;

		// �A���J�[�|�C���g���4�F�~5�_��
		for(int color = ANCHOR_COL_BLACK; color <= ANCHOR_COL_CYAN; ++color)
		{
			std::string colorName;
			switch(color)
			{
			case ANCHOR_COL_BLACK:
				colorName = "K";
				break;
			case ANCHOR_COL_YELLOW:
				colorName = "Y";
				break;
			case ANCHOR_COL_MAGENTA:
				colorName = "M";
				break;
			case ANCHOR_COL_CYAN:
				colorName = "C";
				break;
			}

			// �A���J�[�|�C���g���
			for(int pointID = 0; pointID < AnchorPointNum; ++pointID)
			{
				// ��Z�x�l
				anchorPoint.m_beforeConPoint = static_cast<long>(m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].POSITION.x);

				// �J�[�u�f�[�^�̐��l
				anchorPoint.m_afterConPoint = static_cast<long>(m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].POSITION.y);

				// �ҏW�^�C�v
				anchorPoint.m_anchorPointEditType = (FALSE == m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].EDIT_TYPE) ? 0 : 1;

				// �A���J�[�|�C���g���(0:�\���̂�, 1:�J�[�u�`��)
				if(FALSE == m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].KIND)
				{
					anchorPoint.m_anchorPointType = 0;
				}
				else
				{
					anchorPoint.m_anchorPointType = 1;
				}

				// 1�F���̃f�[�^
				oneColorPoint.point[pointID] = anchorPoint;
			}
			toneCurve.anchorPoint.insert(std::make_pair(colorName, oneColorPoint));
		}
		outToneCurve = toneCurve;
		return true;
	}
	catch(std::exception& e)
	{
		// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
		writeErrorMessage(e.what());
		return false;
	}
}

// JetInspection���̎擾
void PrintCondition::GetJetInspection(SJetInspection& outJetInspection) const
{
	// ����������
	outJetInspection.m_ParamaterName = m_PrintConditionData.migrationData.JICondition;
	// �ꎞ�}�X�N�t�@�C���p�X
	//V3.40JD014T5 2019/09/19 DB�L�ڃt�@�C�������݂��Ȃ��ꍇ�͋󕶎���ɕϊ�
	std::string TempMaskFile = m_PrintConditionData.migrationData.TemporalMaskFile;
	if( !TempMaskFile.empty() )
	{
		std::string TempMaskFullPath = GetModuleDirectory() + TempMaskFile;
		if( !PathFileExists(TempMaskFullPath.c_str()) )	TempMaskFile.clear();
	}
	outJetInspection.m_TemporalMaskFilePath = TempMaskFile;
}

// JetInspection�G���A���̎擾
bool PrintCondition::GetJetInspectionArea(SJetInspectionArea& outJetInspectionArea, bool inIsFront) const
{
	try
	{
		std::string side;	// �\�ʁE���ʂ̋��
		long isPart = 0;	// ����@���ǂ���
		// ����ʂ��擾����
		getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isPart);

		// �����ݒ�A�Ώۖ�
		outJetInspectionArea.m_side = side;
		// �񌟍��G���A���[
		outJetInspectionArea.m_NonInspectionAreaLeft = m_PrintConditionData.migrationData.NonInspectionLeft[isPart];
		// �񌟍��G���A�E�[
		outJetInspectionArea.m_NonInspectionAreaRight = m_PrintConditionData.migrationData.NonInspectionRight[isPart];

		long index = 1;
		for(auto itID = m_PrintConditionData.migrationData.NonInspectionArea[isPart].begin(); itID != m_PrintConditionData.migrationData.NonInspectionArea[isPart].end(); ++itID)
		{
			SJetInspectionNonInspectionRect NonInspectionRect;

			// �񌟍���`�G���AID
			NonInspectionRect.m_ID = index;
			// �񌟍���`�G���A�J�n�ʒu
			NonInspectionRect.m_StartPos.first = itID->START_POS_X;
			NonInspectionRect.m_StartPos.second = itID->START_POS_Y;
			// �񌟍���`�G���A�I���ʒu
			NonInspectionRect.m_EndPos.first = itID->END_POS_X;
			NonInspectionRect.m_EndPos.second = itID->END_POS_Y;

			outJetInspectionArea.m_NonInspectionRect.push_back(NonInspectionRect);
			++index;
		}

		return true;
	}
	catch(std::exception& e)
	{
		// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
		writeErrorMessage(e.what());
		return false;
	}
}

// JetInspection�f�R�[�h���̎擾
bool PrintCondition::GetJetInspectionDecode(SJetInspectionDecode& outJetInspectionDecode, bool inIsFront) const
{
	// ���l�P�ʌn�̕�������擾����
	auto getUnitStr = [](const DEF_LENGTH_UNIT inUnit, std::string& outUnitStr)
	{
		std::map<DEF_LENGTH_UNIT, std::string> unitStrList;
		unitStrList.insert(std::make_pair(DEF_LENGTH_MM, "mm"));
		unitStrList.insert(std::make_pair(DEF_LENGTH_INCH, "inch"));
		unitStrList.insert(std::make_pair(DEF_LENGTH_POINT, " point"));

		auto target = unitStrList.find(inUnit);
		if(target == unitStrList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible JetInspection Decode Unit (Input : " << inUnit << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outUnitStr = target->second;
	};

	// ���x����ނ̕�������擾����
	auto getLabelStr = [](DEF_DECODE_LABEL_TYPE inLabelType, UNI_DECODE_PATTERN inDecodePattern, std::string& outLabelName)
	{
		switch(inLabelType)
		{
		case DEF_DECODE_OCR:
			switch(inDecodePattern.pat_OCR)
			{
			case DEF_OCR_NUMBER:
				outLabelName = "OCR_Number";
				break;
			case DEF_OCR_TEXT:
				outLabelName = "OCR_Text";
				break;
			case DEF_OCR_PRIORITY_NUMBER:
				outLabelName = "OCR_PriorityNumber";
				break;
			case DEF_OCR_PRIORITY_TEXT:
				outLabelName = "OCR_PriorityText";
				break;
			}
			break;
		case DEF_DECODE_1D_BARCODE:
			switch(inDecodePattern.pat_1D)
			{
			case DEF_1D_CODE39:
				outLabelName = "1D_CODE39";
				break;
			case DEF_1D_CODE128:
				outLabelName = "1D_CODE128";
				break;
			case DEF_1D_NW7:
				outLabelName = "1D_NW7";
				break;
			case DEF_1D_CODE39_CD:
				outLabelName = "1D_CODE39CD";
				break;
			}
			break;
		case DEF_DECODE_2D_BARCODE:
			switch(inDecodePattern.pat_2D)
			{
			case DEF_2D_QR_CODE:
				outLabelName = "2D_QrCode";
				break;
			case DEF_2D_DATA_MATRIX:
				outLabelName = "2D_DataMatrix";
				break;
			case DEF_2D_PDF417:
				outLabelName = "2D_Pdf417";
				break;
			}
			break;
		}

		if(outLabelName.empty())
		{
			throw std::runtime_error("Not compatible JetInspectionDecode LabelName");
		}
	};

	// �p�x���̕�������擾����
	auto getAngleStr = [](DEF_OCR_ANGLE inAngle, std::string& outAngbleName)
	{
		switch(inAngle)
		{
		case DEF_OCR_ANGLE_0:
			outAngbleName = "0";
			break;
		case DEF_OCR_ANGLE_90:
			outAngbleName = "90";
			break;
		case DEF_OCR_ANGLE_180:
			outAngbleName = "180";
			break;
		case DEF_OCR_ANGLE_270:
			outAngbleName = "270";
			break;
		}

		if(outAngbleName.empty())
		{
			throw std::runtime_error("Not compatible JetInspectionDecode AngleName");
		}
	};

	DEF_LENGTH_UNIT unit = UserModeGUI_GetLengthUnit();
	std::string side;	// �\�ʁE���ʂ̋��
	long isDummy = 0;
	// ����ʂ��擾����
	getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isDummy);


	// �u�f�R�[�h�v�^�u�A�G���[��~�A���V�[�g��
	outJetInspectionDecode.m_ErrorSheets = m_PrintConditionData.migrationData.Decode.errorSheets;

	// ���l�P�ʌn
	getUnitStr(unit, outJetInspectionDecode.m_Unit);

	// �f�R�[�h�����̗L���^����
	outJetInspectionDecode.m_IsEnabled = (TRUE == m_PrintConditionData.migrationData.Decode.enableDecode) ? true : false;

	try
	{
		if(inIsFront)
		{
			for(auto itFront = m_PrintConditionData.migrationData.Decode.decodeDetailF.begin(); itFront != m_PrintConditionData.migrationData.Decode.decodeDetailF.end(); ++itFront)
			{
				SJetInspectionDecodeDetails tmpFrontDecodeDetails;

				// �Ώۖʁi�\or���j
				tmpFrontDecodeDetails.m_Side = side;

				// �T�C�Y�ݒ�
				tmpFrontDecodeDetails.m_Size.first = itFront->size.cx;
				tmpFrontDecodeDetails.m_Size.second = itFront->size.cy;

				// �ʒu�ݒ�
				tmpFrontDecodeDetails.m_Offset.first = itFront->offset.x;
				tmpFrontDecodeDetails.m_Offset.second = itFront->offset.y;

				// ���x������
				tmpFrontDecodeDetails.m_Name = itFront->name;

				// ���x�����
				getLabelStr(itFront->labelType, itFront->pattern, tmpFrontDecodeDetails.m_Type);

				// �iOCR����j�p�x���
				getAngleStr(itFront->ocrAngle, tmpFrontDecodeDetails.m_Angle);

				// �f�R�[�h�G���[�������A�G���[��~����J�E���g�̑Ώۂɂ��邩
				tmpFrontDecodeDetails.m_ErrorStop = (TRUE == itFront->checkError) ? true : false;

				outJetInspectionDecode.m_DetailsList.push_back(tmpFrontDecodeDetails);
			}
		}
		else
		{
			for(auto itBack = m_PrintConditionData.migrationData.Decode.decodeDetailB.begin(); itBack != m_PrintConditionData.migrationData.Decode.decodeDetailB.end(); ++itBack)
			{
				SJetInspectionDecodeDetails tmpFrontDecodeDetails;

				// �Ώۖʁi�\or���j
				tmpFrontDecodeDetails.m_Side = side;

				// �T�C�Y�ݒ�
				tmpFrontDecodeDetails.m_Size.first = itBack->size.cx;
				tmpFrontDecodeDetails.m_Size.second = itBack->size.cy;

				// �ʒu�ݒ�
				tmpFrontDecodeDetails.m_Offset.first = itBack->offset.x;
				tmpFrontDecodeDetails.m_Offset.second = itBack->offset.y;

				// ���x������
				tmpFrontDecodeDetails.m_Name = itBack->name;

				// ���x�����
				getLabelStr(itBack->labelType, itBack->pattern, tmpFrontDecodeDetails.m_Type);

				// �iOCR����j�p�x���
				getAngleStr(itBack->ocrAngle, tmpFrontDecodeDetails.m_Angle);

				// �f�R�[�h�G���[�������A�G���[��~����J�E���g�̑Ώۂɂ��邩
				tmpFrontDecodeDetails.m_ErrorStop = (TRUE == itBack->checkError) ? true : false;

				outJetInspectionDecode.m_DetailsList.push_back(tmpFrontDecodeDetails);
			}
		}
	}
	catch(std::exception& e)
	{
		// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
		writeErrorMessage(e.what());
		return false;
	}

	return true;
}

// �ϔ{���̎擾
bool PrintCondition::GetVariableMagnification(ST_PDB_RIP_MAGNIFICATION& outVariableMagnification) const
{
	// �ϔ{�l�̗L���͈͂��`�F�b�N����
	auto checkRange = [](long inValue)->bool
	{
		if((10000 > inValue) || (1000000 <= inValue))
		{
			return false;
		}
		return true;
	};

	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.FRONT_X)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.FRONT_Y)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.BACK_X)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.BACK_Y)) return false;

	outVariableMagnification = m_PrintConditionData.migrationData.RipMagnification;
	return true;
}

// �G���[���e�̏�������
void PrintCondition::writeErrorMessage(const std::string& inError) const
{
	// �Ή����Ă��Ȃ��p�^�[���̏ꍇ
	std::stringstream log;
	log << "ERROR:" << inError << "  [DescriptiveName:" << m_PrintConditionData.printConditionName << "]";
	WriteToLogBuf(LOG_DEBUG, log.str());
}

// ����ʂ̎擾
void PrintCondition::getSide(const bool inIsDED, const bool inIsFront, std::string& outSide, long& outIsPart) const
{
	// SES�\���̎�
	if(false == inIsDED)
	{
		if(true == inIsFront)
		{
			outSide = "Front";
			outIsPart = 0;
		}else
		{
			// SES�ɗ��ʂ͑��݂��Ȃ�
			std::stringstream errorMessage;
			errorMessage << "Not compatible Side (Input : Back)";
			throw std::runtime_error(errorMessage.str());
		}
	}
	// DED�\���̎�
	else
	{
		if(true == inIsFront)
		{
			outSide = "Front";
			outIsPart = 1;

		}else
		{
			outSide = "Back";
			outIsPart = 0;
		}
	}
}

}//namespace export_xml