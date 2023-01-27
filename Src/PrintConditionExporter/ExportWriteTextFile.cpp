/*! \file WriteTextFile.cpp
 *  \brief WriteTextFile.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/03/11 pt.ju-tanaka �V�K�쐬
 *  \date 2022/04/27 cec.k-miyachi #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "ExportWriteTextFile.h"
#include "ProcPluginCBRProc.h"
#include "ResDef.h"
#include "Utility.h"
#include "ConvertMD5Hash.h"
#include "DataSystemSetting.h"
#include "Stub_PrinterStateIF.h"
#include "DataSystemSetting.h"

#include <sstream>
#include <fstream>
#include <iomanip>

static char* getDecodeKind(DEF_DECODE_LABEL_TYPE type, UNI_DECODE_PATTERN pat)
{
	char* ret_val;

	ret_val = "";
	switch( type )
	{
	case DEF_DECODE_OCR:
		{
			switch( pat.pat_OCR )
			{
			case DEF_OCR_NUMBER:
				ret_val = "OCR_Number";
				break;
			case DEF_OCR_TEXT:
				ret_val = "OCR_Text";
				break;
			case DEF_OCR_PRIORITY_NUMBER:
				ret_val = "OCR_PrinorityNumber";
				break;
			case DEF_OCR_PRIORITY_TEXT:
				ret_val = "OCR_PrinorityText";
				break;
			}
		}
		break;
	case DEF_DECODE_1D_BARCODE:
		{
			switch( pat.pat_1D )
			{
			case DEF_1D_CODE39:
				ret_val = "1D_CODE39";
				break;
			case DEF_1D_CODE128:
				ret_val = "1D_CODE128";
				break;
			case DEF_1D_NW7:
				ret_val = "1D_NW7";
				break;
			case DEF_1D_CODE39_CD:
				ret_val = "1D_CODE39CD";
				break;
			}
		}
		break;
	case DEF_DECODE_2D_BARCODE:
		{
			switch( pat.pat_2D )
			{
			case DEF_2D_QR_CODE:
				ret_val = "2D_QrCode";
				break;
			case DEF_2D_DATA_MATRIX:
				ret_val = "2D_DataMatrix";
				break;
			case DEF_2D_PDF417:
				ret_val = "2D_Pdf417";
				break;
			}
		}
	}
	return ret_val;
}

// ==========================================================================

/**
 * @brief	ExportWriteTextFile���g��API
**/
class ExportWriteTextFile::API
{
public:
	/**
	 * @brief	DED���ǂ���
	 * @retval	true : DED�ł���
	 * @retval	false : DED�ł͂Ȃ�
	**/
	static bool IsDED()
	{
		auto system = CDataSystemSetting::GetInstance();
		bool ded = system->IsDED();

		return ded;
	}

	/**
	 * @brief	JI�ڑ��ݒ�
	 * @retval	0 : ���ڑ��A1 : �f�R�[�h�̂݁A2 : �t���X�y�b�N
	**/
	static long getJIoption()
	{
		auto system = CDataSystemSetting::GetInstance();
		long retval = system->GetJetInspectionOption();

		return retval;
	}

	/**
	 * @brief	������[�h���j���[���̎擾
	 * @retval	true :  ���\�[�XID�L��
	 * @retval	false : ���\�[�XID����
	**/
	static bool getPrintModeMenuName(const char* inMode, std::string& outName)
	{
		auto system = CDataSystemSetting::GetInstance();
		bool retval = system->GetPrintModeMenuName(inMode, outName);

		return retval;
	}

};

//�R���X�g���N�^
ExportWriteTextFile::ExportWriteTextFile()
{
}

//�f�X�g���N�^
ExportWriteTextFile::~ExportWriteTextFile()
{
}

//�e�L�X�g�t�@�C���o�̓C�x���g
bool ExportWriteTextFile::onWriteTextFile(
	const std::string& inExportFilePath, 
	const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	std::string bufStr;
	//������ǉ��B
	auto addStr = [&](long inResID, const std::string& inValue)->void
	{
		std::stringstream outStr;
		std::string label;
		label = LoadResourceString(inResID, RESOURCE_STR);
		outStr << label << ":" << inValue << "\n";
		bufStr += outStr.str();
	};

	//������ǉ��i�����w��t�����x���Ή��j�B
	auto addStr2 = [&](long inResID, long cntr, const char* inValue)->void
	{
		std::stringstream outStr;
		std::string label, str;
		char format[256] = {0};
		char format2[256] = {0};
		label = LoadResourceString(inResID, RESOURCE_STR);
		_snprintf(format, sizeof(format), label.c_str(), cntr);
		label = format;
		_snprintf(format2, sizeof(format2), ":%s\n", inValue);
		str = format2;

		outStr << label << str;
		bufStr += outStr.str();
	};

	//������ǉ��i�����w��t�����x���Ή��j�B
	auto addStr1 = [&](long inResID, long cntr, const std::string& inValue)->void
	{
		std::stringstream outStr;
		std::string label;
		char format[256] = {0};
		label = LoadResourceString(inResID, RESOURCE_STR);
		_snprintf(format, sizeof(format), label.c_str(), cntr);
		label = format;
		outStr << label << ":" << inValue << "\n";
		bufStr += outStr.str();
	};


	//�G���[����
	auto execption = [&](const std::string& inStr)->void
	{
		long retExp = 1;
		std::stringstream ss;
		ss << "failed :" << inStr;
		WriteDebugLog(ss.str());
		throw(retExp);
	};

	//////////////////////////////////
	bool ret = false;

	//�e���ڂ��w��̏����ɕϊ����ăt�@�C���ɏ�������
		//��DED��SES�ŏo�͍��ڂ��ς��_�ɒ���
	std::string value;
	try
	{
		//�G�N�X�|�[�g�t�@�C���𒆊ԃt�H���_���ɍ쐬����
		std::ofstream file(inExportFilePath.c_str(), std::ios_base::out);
		if (nullptr == file)
		{
			//�t�@�C���o�͎��s�B
			std::stringstream ss;
			ss << "std::ofstream[" << getLastError()  << "]";
			execption(ss.str());
		}

		//�p�����
		addStr(IDS_PARAM_PAPER_TYPE, inMigrationData.PaperType);

		//����
		addStr(IDS_PARAM_PAPERTHICKNESS, changeTo02f((double)inMigrationData.Thickness * 0.001));
		
		//�p����
		addStr(IDS_PARAM_PAGEWIDTH, changeTo02f((double)inMigrationData.PaperSizeW * 0.001));

		//�y�[�W����T�C�Y
		addStr(IDS_PARAM_PAPER_FEED, getPaperFeed(inMigrationData.PaperSizeH));

		//�y�[�W����T�C�Y�̒P��
		addStr(IDS_PARAM_PAPER_FEED_UNIT, getPaperFeedUnit(inMigrationData.PaperSizeH.UNIT));

		//�i���ݒ�
		addStr(IDS_PARAM_PRINT_TIMING, inMigrationData.LevelTuneName);


		if( true == API::getPrintModeMenuName( inMigrationData.PrintMode.RESOURCE_ID, value) )
		{
			//������[�h
			addStr(IDS_PARAM_PRINT_MODE, value);
		}
		//������x
		addStr(IDS_PARAM_PRINT_SPEED, changeToD((inMigrationData.SpeedUnit == DEF_MODE_RESO_SPEED_100) ? inMigrationData.PrintSpeed/100 : inMigrationData.PrintSpeed));

		//�e���V����
		addStr(IDS_PARAM_TENSION, changeToD(inMigrationData.Tension));

		//DNS
		addStr(IDS_PARAM_DNS, getTrueOrFalse(inMigrationData.DNSStatus));
		
		//�p���`
		addStr(IDS_PARAM_PAPER_OP_PUNCH, getPaperPunch(inMigrationData.PaperOption));

		//�~�V����
		addStr(IDS_PARAM_PAPER_OP_PERFORATION, getTrueOrFalse(inMigrationData.PaperOption.IS_HOLES_PERFORATIONS));

		//�}�[�N
		addStr(IDS_PARAM_MARK, getParamMark(inMigrationData.PaperOption));

		//�}�[�N�I�t�Z�b�g
		addStr(IDS_PARAM_MARK_OFFSET, changeTo02f((double)inMigrationData.PaperOption.PRE_MARK_OFFSET * 0.001));

		//���O���
		addStr(IDS_PARAM_PRE_PRINT, getTrueOrFalse(inMigrationData.PaperOption.IS_PRE_MARK_PAPER));

		//�������x
		if (true == API::IsDED())
		{
			//DED�̏ꍇ�A�\���ݒ肷��
			//�\�ʊ������x
			addStr(IDS_PARAM_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_PART));
			//���ʊ������x
			addStr(IDS_PARAM_DRYER_TEMP_B, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_SELF));

			//�����A�V�X�g�t�@������
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(false) > 0 ) {
				//�\��
				addStr(IDS_PARAM_HEAT_FAN_TEMP_F, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_PART));
			}
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(true) > 0 ) {
				//����
				addStr(IDS_PARAM_HEAT_FAN_TEMP_B, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_SELF));
			}

			//NIR����
			if( Stub_PrinterStateIF::IsUnitOfNir(false) ) {
				//�\��
				addStr(IDS_PARAM_IR_POWER_F, changeToD(inMigrationData.DryTempPrint.NIR_POWER_PART));
			}
			if( Stub_PrinterStateIF::IsUnitOfNir(true) ) {
				//����
				addStr(IDS_PARAM_IR_POWER_B, changeToD(inMigrationData.DryTempPrint.NIR_POWER_SELF));
			}

			//Ranger����
			if( Stub_PrinterStateIF::IsUnitOfRanger() ) {
				//�\�ʃT�u�������x
				addStr(IDS_PARAM_SUB_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_PART));
				//���ʃT�u�������x
				addStr(IDS_PARAM_SUB_DRYER_TEMP_B, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_SELF));
			}
		}
		else
		{
			//�\�ʊ������x
			addStr(IDS_PARAM_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_SELF));

			//�����A�V�X�g�t�@������
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(true) > 0 ) {
				//�\��
				addStr(IDS_PARAM_HEAT_FAN_TEMP_F, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_SELF));
			}

			//NIR����
			if( Stub_PrinterStateIF::IsUnitOfNir(true) ) {
				//�\��
				addStr(IDS_PARAM_IR_POWER_F, changeToD(inMigrationData.DryTempPrint.NIR_POWER_SELF));
			}

			//Ranger����
			if( Stub_PrinterStateIF::IsUnitOfRanger() ) {
				//�\�ʃT�u�������x
				addStr(IDS_PARAM_SUB_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_SELF));
			}
		}

		//�g�[���J�[�u�i���́j
		if (true == API::IsDED())
		{
			//DED�̏ꍇ�A�\���ݒ肷��
			//�\��
			if (false == getToneCurve(inMigrationData.DotgainCurve[1], value))
			{
				//�G���[����
				execption("CConvertMD5Hash err[front].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_F, value);
			}

			//����
			if (false == getToneCurve(inMigrationData.DotgainCurve[0], value))
			{
				//�G���[����
				execption("CConvertMD5Hash err[back].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_B, value);
			}
		}
		else
		{
			//�\��
			if (false == getToneCurve(inMigrationData.DotgainCurve[0], value))
			{
				//�G���[����
				execption("CConvertMD5Hash err[front].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_F, value);
			}
		}

		//�C���t�H���[�V�����V�[�g
		addStr(IDS_PARAM_INFORMATION_SHEET, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.informationSheet ? TRUE : FALSE));

		//�t���b�V���O�p�^�[��
		addStr(IDS_PARAM_FLUSHING_PATTERN_CMYK, getFlushingPatternKind(inMigrationData.LayoutFlushingPattern.flushingPattern));

		//�t���b�V���O�p�^�[�����C�������炵��
		if (true == getFlushingPatternOffset(inMigrationData.LayoutFlushingPattern, value))
		{
			//Line�I�����̂݁A�o�́B
			addStr(IDS_PARAM_FLUSHING_PATTERN_OFFSET, value);
		}

		//�t���b�V���O�p�^�[���A���C���t���b�V���O�A�������I�t�Z�b�g
		//�t���b�V���O�p�^�[����Line*�ݒ莞�̂ݤ���ڏo�͂���
		if( true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern) )
		{
			if (true == API::IsDED())
			{
				//DED�̏ꍇ�A�\���ݒ肷��
				// ���[�ݒ�̔���
				if(TRUE == inMigrationData.LayoutVertivalAlignmentPPFLSPTN)
				{
					// ���[�ݒ�̏ꍇ�A�G�N�X�|�[�g���̃��C���t���b�V���O�̃I�t�Z�b�g�l�͕��̒l�Ƃ���
					//�\��
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0] * (-1));
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
					//����
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[1] * (-1));
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y_BACK, value);
				}
				else
				{
					// ��[�ݒ�̏ꍇ�A�G�N�X�|�[�g���̃��C���t���b�V���O�̃I�t�Z�b�g�l�͐��̒l�Ƃ���
					//�\��
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0]);
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
					//����
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[1]);
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y_BACK, value);
				}
			}
			else
			{
				//�\��
				// ���[�ݒ�̔���
				if(TRUE == inMigrationData.LayoutVertivalAlignmentPPFLSPTN)
				{
					// ���[�ݒ�̏ꍇ�A�G�N�X�|�[�g���̃��C���t���b�V���O�̃I�t�Z�b�g�l�͕��̒l�Ƃ���
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0] * (-1));
				}
				else
				{
					// ��[�ݒ�̏ꍇ�A�G�N�X�|�[�g���̃��C���t���b�V���O�̃I�t�Z�b�g�l�͐��̒l�Ƃ���
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0]);
				}

				addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
			}
		}

		if((true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern))
			&& (TRUE == inMigrationData.LayoutFlushingPatternAdd2.enableMultiLine))
		{
			//�t���b�V���O�p�^�[����Line*���A�t���b�V���O�p�^�[��2Line�I�v�V��������enableMultiLine��TRUE�̏ꍇ�A
			//���L�����ꂼ��ݒ肷��

			//���C���t���b�V���O�A�����ӏ��ɏo�͂���
			addStr(IDS_DATA_LINE_FLUSHING_OUTPUT_TO_MULTIPLE, getTrueOrFalse(TRUE));

			if (true == API::IsDED())
			{
				//���C���t���b�V���O�A�\�ʑ}���Ԋu
				std::string frontInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_INSERTION_INTERVAL, frontInsetInterval);

				//���C���t���b�V���O�A�\�ʃy�[�W���␳�I�t�Z�b�g
				std::string frontOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_OFFSET, frontOffset);

				//���C���t���b�V���O�A���ʑ}���Ԋu
				std::string backInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[1]);
				addStr(IDS_DATA_LINE_FLUSHING_BACK_INSERTION_INTERVAL, backInsetInterval);

				//���C���t���b�V���O�A���ʃy�[�W���␳�I�t�Z�b�g
				std::string backOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[1]);
				addStr(IDS_DATA_LINE_FLUSHING_BACK_OFFSET, backOffset);
			}
			else
			{
				//���C���t���b�V���O�A�\�ʑ}���Ԋu
				std::string frontInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_INSERTION_INTERVAL, frontInsetInterval);

				//���C���t���b�V���O�A�\�ʃy�[�W���␳�I�t�Z�b�g
				std::string frontOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_OFFSET, frontOffset);
			}
		}

		if( true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern) )
		{
			if( DEF_LINE_OPTION2_CROSS_STITCH == inMigrationData.LayoutFlushingPatternAdd.lineOption2 )
			{
				//�t���b�V���O�p�^�[����Line*���A�t���b�V���O�p�^�[��Line�I�v�V�������2��DEF_LINE_OPTION2_CROSS_STITCH�̏ꍇ�A
				//���C���t���b�V���O�A���C����ʂ�"�N���X�X�e�b�`"�ɐݒ肷��
				addStr(IDS_DATA_LINE_FLUSHING_LINE_TYPE, LoadResourceString(IDS_DATA_CROSS_STICH, RESOURCE_STR));
			}
			addStr(IDS_PARAM_FLUSHING_LINE_OPTIONS, getFlushingPatternOption(inMigrationData.LayoutFlushingPatternAdd));
		}

		if (DEF_FLUSHING_PAGE_NONE == inMigrationData.JobBaseFlushingPage.flushingPageType)
		{
			//�t���b�V���O�y�[�W�u�o�͂��Ȃ��v�ݒ莞
			addStr(IDS_PARAM_FLUSHING_PAGE, LoadResourceString(IDS_PARAM_FLUSHING_PAGE_NAME_NONE, RESOURCE_STR));
		} else {
			//�t���b�V���O�y�[�W����̂Ƃ��̂݁A�ڍ׍��ڂ��o�͂���B

			//�o�̓p�^�[��
			value = inMigrationData.JobBaseFlushingPage.flushingPage;
			addStr(IDS_PARAM_FLUSHING_PAGE, value);

			//�y�[�W���̊Ԋu
			value = changeToD(inMigrationData.JobBaseFlushingPage.interval);
			addStr(IDS_PARAM_FLUSHING_PAGE_INTERVAL, value);

			//�y�[�W���̃W���u�O
			addStr(IDS_PARAM_BEFOREJOB, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.beforeJob ? TRUE : FALSE));

			//�y�[�W���̃W���u�O��
			value = changeToD(inMigrationData.JobBaseFlushingPage.printCountOfBeforeJob);
			addStr(IDS_PARAM_COUNT_BEFOREJOB, value);

			//�y�[�W���̃W���u��
			addStr(IDS_PARAM_AFTERJOB, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.afterJob ? TRUE : FALSE));

			//�\�ʃI�t�Z�b�g
			if (true == API::IsDED())
			{
				//DED�̏ꍇ�A�\���ݒ肷��
				//�\��
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[0]);
				addStr(IDS_PARAM_OFFSET_F, value);
				//����
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[1]);
				addStr(IDS_PARAM_OFFSET_B, value);
			}
			else
			{
				//�\��
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[0]);
				addStr(IDS_PARAM_OFFSET_F, value);
			}
		}

		//��/�������摜���C�A�E�g
		if (true == API::IsDED())
		{
			//DED�̏ꍇ�A�\���ݒ肷��
			//�\��
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_Y, value);

			//����
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[1].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_BACK_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[1].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_BACK_Y, value);
		}
		else
		{
			//�\��
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_Y, value);
		}

		//�T�C�h�����}�[�N
		addStr(IDS_PARAM_SIDE_CHECK_MARK, getTrueOrFalse(inMigrationData.LayoutVerifyMark.enableVerify));

		//�T�C�h�����}�[�N���
		addStr(IDS_PARAM_SIDE_CHECK_MARK_KIND, getSideCheckMarkKind(inMigrationData.LayoutVerifyMark));

		//�G���[�A���y�[�W��
		addStr(IDS_PARAM_ERR_CONS_SHEET, changeToD(inMigrationData.LayoutVerifyMark.errorSheets));

		//�呖���T�C�h�����}�[�N�I�t�Z�b�g
		if (true == API::IsDED())
		{
			//DED�̏ꍇ�A�\���ݒ肷��
			//�\��
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[0]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_FRONT_X, value);

			//����
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[1]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_BACK_X, value);
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetY);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_BACK_Y, value);
		}
		else
		{
			//�\��
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[0]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_FRONT_X, value);
		}

		//�㏈���}�[�N
		addStr(IDS_PARAM_POST_MARK_KIND, getPostMarkKind(inMigrationData.LayoutPostProcess.side));

		//�㏈���}�[�N�T�C�Y�i���j
		addStr(IDS_PARAM_POST_MARK_SIZE_X, changeTo02f(inMigrationData.LayoutPostProcess.size.cx));

		//�㏈���}�[�N�T�C�Y�i�����j
		addStr(IDS_PARAM_POST_MARK_SIZE_Y, changeTo02f(inMigrationData.LayoutPostProcess.size.cy));

		//�㏈���}�[�N�\�ʃI�t�Z�b�g
		addStr(IDS_PARAM_POST_MARK_OFFSET_F, changeTo02f(inMigrationData.LayoutPostProcess.offsetX));

		//�㏈���}�[�N���ʃI�t�Z�b�g
		if (true == API::IsDED())
		{
			addStr(IDS_PARAM_POST_MARK_OFFSET_B, changeTo02f(inMigrationData.LayoutPostProcess.offsetX_back));
		}

		//�㏈���}�[�N�����V�[�g��
		addStr(IDS_PARAM_POST_READY_SHEETS, changeToD(inMigrationData.LayoutPostProcess.sheets));

		// ADD --> #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
		addStr(IDS_PARAM_ADJUST_SHEETSIZE, getTrueOrFalse(inMigrationData.LayoutPostProcessAdd.adjustmentSheetSizeEnabled ? TRUE : FALSE));
		addStr(IDS_PARAM_ADJUST_SHEETLENGTH, changeTo02f(inMigrationData.LayoutPostProcessAdd.adjustmentSheetLength));
		if ( inMigrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob == SET_SHEET_COUNT )
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_SET, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET, label);
		}
		else if ( inMigrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob == NONE )
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_NOSET, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET , label);
		}
		else
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_STOP, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET , label);
		}
		addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET_COUNT, changeToD(inMigrationData.LayoutPostProcessAdd.afterJobSheets));
		// ADD <-- #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j

		//JetInspection�A�t���X�y�b�N��
		if( API::getJIoption() == 2 ) {
			//����������
			addStr(IDS_PARAM_INSPECTION_PARAMATER, inMigrationData.JICondition);

			//�ꎞ�}�X�N�t�@�C����
			//V3.40JD014T5 2019/09/19 DB�L�ڃt�@�C�������݂��Ȃ��ꍇ�͋󕶎���ŏo��
			std::string TempMaskFile = inMigrationData.TemporalMaskFile;
			if( !TempMaskFile.empty() )
			{
				std::string TempMaskFullPath = GetModuleDirectory() + TempMaskFile;
				if( !PathFileExists(TempMaskFullPath.c_str()) )	TempMaskFile.clear();
			}
			addStr(IDS_PARAM_TEMP_MASK_FILEPATH, TempMaskFile);
			
			if (true == API::IsDED())
			{
				//�\�ʔ񌟍��G���A���[
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_F, changeTo02f(inMigrationData.NonInspectionLeft[1]));

				//�\�ʔ񌟍��G���A�E�[
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_F, changeTo02f(inMigrationData.NonInspectionRight[1]));

				//���ʔ񌟍��G���A���[
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_B, changeTo02f(inMigrationData.NonInspectionLeft[0]));

				//���ʔ񌟍��G���A�E�[
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_B, changeTo02f(inMigrationData.NonInspectionRight[0]));

				if( !inMigrationData.NonInspectionArea.empty() ) {
					size_t area_cnt, f_cnt;
					if( inMigrationData.NonInspectionArea.size() > 1 ) {
						//�\�ʔ񌟍���`�G���A
						area_cnt = inMigrationData.NonInspectionArea[1].size();
						for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
							const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[1][f_cnt];
							addStr1(IDS_PARAM_NONINSPECT_RECT_F, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
						}
					}
					//���ʔ񌟍���`�G���A
					area_cnt = inMigrationData.NonInspectionArea[0].size();
					for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
						const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[0][f_cnt];
						addStr1(IDS_PARAM_NONINSPECT_RECT_B, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
					}
				}
			}
			else
			{
				//�\�ʔ񌟍��G���A���[
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_F, changeTo02f(inMigrationData.NonInspectionLeft[0]));

				//�\�ʔ񌟍��G���A�E�[
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_F, changeTo02f(inMigrationData.NonInspectionRight[0]));

				//�\�ʔ񌟍���`�G���A
				if( !inMigrationData.NonInspectionArea.empty() ) {
					size_t area_cnt, f_cnt;
					area_cnt = inMigrationData.NonInspectionArea[0].size();
					for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
						const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[0][f_cnt];
						addStr1(IDS_PARAM_NONINSPECT_RECT_F, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
					}
				}
			}
			
			
		}

		//JetInspection �f�R�[�h�L����
		if( API::getJIoption() == 2 || Stub_PrinterStateIF::IsOCRandDECEnable() ) {
			//�f�R�[�h����
			addStr(IDS_PARAM_DECODE, getTrueOrFalse(inMigrationData.Decode.enableDecode));

			//�f�R�[�h�����A�G���[��~�A���V�[�g��
			addStr(IDS_PARAM_DECODE_ERR_CONS_SHEET, changeToD(inMigrationData.Decode.errorSheets));
			
			size_t d_cnt;
			static char strAngle[4][16];
			strcpy(strAngle[0],"0");
			strcpy(strAngle[1],"90");
			strcpy(strAngle[2],"180");
			strcpy(strAngle[3],"270");

			//�\�ʁA�f�R�[�h�ڍ׏��
			auto itr = inMigrationData.Decode.decodeDetailF.begin();
			for( d_cnt = 0; itr != inMigrationData.Decode.decodeDetailF.end(); itr++, d_cnt++ ) {

				//�f�R�[�h�ݒ褕\��%02d�A���x����
				addStr2(IDS_PARAM_DECODE_LABEL_NAME_F, d_cnt, itr->name);

				//�f�R�[�h�ݒ褕\��%02d�A���
				addStr2(IDS_PARAM_DECODE_TYPE_F, d_cnt, getDecodeKind(itr->labelType,itr->pattern));

				//�f�R�[�h�ݒ褕\��%02d�A�ʒu
				addStr1(IDS_PARAM_DECODE_POS_F, d_cnt, changeTo02f(itr->offset.x, itr->offset.y));

				//�f�R�[�h�ݒ褕\��%02d�A�T�C�Y
				addStr1(IDS_PARAM_DECODE_SIZE_F, d_cnt, changeTo02f(itr->size.cx, itr->size.cy));

				if(itr->labelType == DEF_DECODE_OCR )
				{
					//�f�R�[�h�ݒ褕\��%02d�A�p�x
					addStr2(IDS_PARAM_DECODE_ANGLE_F, d_cnt, strAngle[itr->ocrAngle]);
				}

				//�f�R�[�h�ݒ褕\��%02d�A�f�R�[�h���̃G���[����
				addStr1(IDS_PARAM_DECODE_ERROR_PROC_F, d_cnt, getTrueOrFalse(itr->checkError));
			}

			if (true == API::IsDED())
			{
				//���ʁA�f�R�[�h�ڍ׏��
				auto itr_b = inMigrationData.Decode.decodeDetailB.begin();
				for( d_cnt = 0; itr_b != inMigrationData.Decode.decodeDetailB.end(); itr_b++, d_cnt++ ) {

					//�f�R�[�h�ݒ褕\��%02d�A���x����
					addStr2(IDS_PARAM_DECODE_LABEL_NAME_B, d_cnt, itr_b->name);

					//�f�R�[�h�ݒ褕\��%02d�A���
					addStr2(IDS_PARAM_DECODE_TYPE_B, d_cnt, getDecodeKind(itr_b->labelType,itr_b->pattern));

					//�f�R�[�h�ݒ褕\��%02d�A�ʒu
					addStr1(IDS_PARAM_DECODE_POS_B, d_cnt, changeTo02f(itr_b->offset.x, itr_b->offset.y));

					//�f�R�[�h�ݒ褕\��%02d�A�T�C�Y
					addStr1(IDS_PARAM_DECODE_SIZE_B, d_cnt, changeTo02f(itr_b->size.cx, itr_b->size.cy));

					if(itr_b->labelType == DEF_DECODE_OCR )
					{
						//�f�R�[�h�ݒ褕\��%02d�A�p�x
						addStr2(IDS_PARAM_DECODE_ANGLE_B, d_cnt, strAngle[itr_b->ocrAngle]);
					}

					//�f�R�[�h�ݒ褕\��%02d�A�f�R�[�h���̃G���[����
					addStr1(IDS_PARAM_DECODE_ERROR_PROC_B, d_cnt, getTrueOrFalse(itr_b->checkError));
				}
				
			}
		}

		// �ϔ{�@�\�g�p�̊�
		if(CDataSystemSetting::GetInstance()->IsMagnificationAdjustment())
		{
			// �\�������ϔ{
			addStr(
				IDS_PARAM_RIP_AUTO_MANIFICATION, 
				TRUE == inMigrationData.RipMagnification.IS_ENABLE ? LoadResourceString(IDS_DATA_TRUE, RESOURCE_STR) : LoadResourceString(IDS_DATA_FALSE, RESOURCE_STR));

			// �����ϔ{���
			addStr(
				IDS_PARAM_RIFERENCE_FACE, 
				TRUE == inMigrationData.RipMagnification.IS_DATUM_PLANE_FRONT ? LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR) : LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR));

			// �\�ʎ呖�������ϔ{�l
			addStr(
				IDS_PARAM_MANIFICATION_FRONT_X, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.FRONT_X) / 100000, 5));

			// �\�ʕ����������ϔ{�l
			addStr(
				IDS_PARAM_MANIFICATION_FRONT_Y, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.FRONT_Y) / 100000, 5));

			// ���ʎ呖�������ϔ{�l
			addStr(
				IDS_PARAM_MANIFICATION_BACK_X, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.BACK_X) / 100000, 5));

			// ���ʕ����������ϔ{�l
			addStr(
				IDS_PARAM_MANIFICATION_BACK_Y, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.BACK_Y) / 100000, 5));
		}

		// �t���b�V���O�y�[�W=�L���
		// �m�Y�������`�F�b�N�@�\���L�� ���� �m�Y�������`�F�b�N���\�ȃt���b�V���O�y�[�W���ݒ肳��Ă���ꍇ
		std::string flushingPage(inMigrationData.JobBaseFlushingPage.flushingPage);
		if (inMigrationData.JobBaseFlushingPage.flushingPageType == DEF_FLUSHING_PAGE_USER &&
			CDataSystemSetting::GetInstance()->isNozzleCheckEnabled(flushingPage))
		{
			// �W���u�O�̃N���[�j���O���背�x��
			addStr(
				IDS_PARAM_CLEANING_JUDGMENT_LEVEL_BEFORE_JOB,
				changeToD(inMigrationData.JobBaseCleaningSetting.CleaningJudgmentLevelBeforeJob));

			// �W���u��̃N���[�j���O���背�x��
			addStr(
				IDS_PARAM_CLEANING_JUDGMENT_LEVEL_AFTER_JOB,
				changeToD(inMigrationData.JobBaseCleaningSetting.CleaningJudgmentLevelAfterJob));

			// �����~�ݒ�
			addStr(
				IDS_PARAM_PRINT_STOP_SETTING,
				getPrintStopSettingKind(inMigrationData.JobBaseCleaningSetting.PrintStopSetting));
		}

		//�t�@�C���ɏ������ށB
		try
		{
			file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
			file << bufStr << std::flush;
		}
		catch (const std::ofstream::failure& /*e*/)
		{
			//�G���[����
			execption("WriteFile err[failure].");
		}
		catch (...)
		{
			//�G���[����
			execption("WriteFile err[bad].");
		}
		ret = true;
	}
	catch (long /*exp*/) {
		ret = false;
	}

	return ret;
}

//GetLastError�̕�����擾
std::string ExportWriteTextFile::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

//���l���u"%0.2f"�v�����̕�����ŕԂ�
std::string ExportWriteTextFile::changeTo02f(double inVal)
{
	std::string value;
	char format[32] = {0};

	_snprintf(format, sizeof(format), "%0.2f", roundD(inVal, 2));
	value = format;
	return value;
}

//���l���u"%0.2f"�v�����̕�����ŕԂ�
std::string ExportWriteTextFile::changeTo02f(double sx, double sy, double ex, double ey)
{
	std::string value;
	char format[128] = {0};

	_snprintf(format, sizeof(format), "%0.2f, %0.2f, %0.2f, %0.2f", roundD(sx, 2), roundD(sy, 2), roundD(ex, 2), roundD(ey, 2));
	value = format;
	return value;
}

//���l���u"%0.2f"�v�����̕�����ŕԂ�
std::string ExportWriteTextFile::changeTo02f(double x, double y)
{
	std::string value;
	char format[64] = {0};

	_snprintf(format, sizeof(format), "%0.2f, %0.2f", roundD(x, 2), roundD(y, 2));
	value = format;
	return value;
}

//���l->������ϊ�(string)���ĕԂ�
std::string ExportWriteTextFile::convertNumToStringPrecision(double inValue, UINT inPrecision)
{
	std::stringstream streaam;
	streaam << std::fixed << std::setprecision(inPrecision) << inValue;
	return streaam.str();
}

//���l���u"%d"�v�����̕�����ŕԂ�
std::string ExportWriteTextFile::changeToD(long inVal)
{
	std::string value;
	char format[32] = {0};
	_snprintf(format, sizeof(format), "%d", static_cast<int>(inVal));
	value = format;
	return value;
}

//�y�[�W����T�C�Y������擾
std::string ExportWriteTextFile::getPaperFeed(const ST_PDB_PAPER_SIZE_H& inPaperSizeH)
{
	std::string value;
	char format[32] = {0};
	double dVal = 0.0;
	if (TRUE == inPaperSizeH.IS_SIZE_H)
	{
		dVal = inPaperSizeH.SIZE_H * 0.001;
	}
	switch (inPaperSizeH.UNIT)
	{
		case UNIT_MODE_MM:
			_snprintf(format, sizeof(format), "%0.1f", roundD(dVal, 1));
			break;
		case UNIT_MODE_INCH:
			_snprintf(format, sizeof(format), "%0.2f", roundD(dVal, 2));
			break;
		case UNIT_1PER2_INCH:
		case UNIT_1PER6_INCH:
			_snprintf(format, sizeof(format), "%0.0f", roundD(dVal, 0));
			break;
	}
	value = format;
	return value;
}

//�y�[�W����T�C�Y�̒P�ʕ�����擾
std::string ExportWriteTextFile::getPaperFeedUnit(const eUNIT_MODE inUnitMode)
{
	std::string value;
	switch (inUnitMode)
	{
		case UNIT_MODE_MM:
			value = LoadResourceString(IDS_DATA_UNIT_MM, RESOURCE_STR);
			break;
		case UNIT_MODE_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_INCH, RESOURCE_STR);
			break;
		case UNIT_1PER2_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_1P2INCH, RESOURCE_STR);
			break;
		case UNIT_1PER6_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_1P6INCH, RESOURCE_STR);
			break;
	}
	return value;
}

//�p���`������擾
std::string ExportWriteTextFile::getPaperPunch(const ST_PDB_PAPER_OPTION& inPaperOption)
{
	std::string value;
	if (TRUE == inPaperOption.IS_HOLES
		|| TRUE == inPaperOption.IS_HOLES_PERFORATIONS)
	{	//��ʏ�́u�~�V���v��ON�̏ꍇ��ON�ɂ���B�iDNP�n NX�ۑ� No.50�j
		value = getTrueOrFalse(TRUE);
	}
	else
	{
		value = getTrueOrFalse(FALSE);
	}
	return value;
}

//���C���t���b�V���O�A�I�v�V����������擾
std::string ExportWriteTextFile::getFlushingPatternOption(const ST_JOB_FLUSHING_PATTERN_ADD& inOption)
{
	std::string value;
	switch( inOption.lineOption ) {
	case DEF_LINE_OPTION_C0:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_C0_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_C0_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_NONE:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_NONE_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_NONE_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_S0:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S0_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S0_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_S2:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S2_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S2_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	}
	
	return value;
}

//�utrue�v�ufalse�v������擾
std::string ExportWriteTextFile::getTrueOrFalse(BOOL inVal)
{
	std::string value;
	if (TRUE == inVal)
	{
		value = LoadResourceString(IDS_DATA_TRUE, RESOURCE_STR);
	}
	else
	{
		value = LoadResourceString(IDS_DATA_FALSE, RESOURCE_STR);
	}
	return value;
}

//���O����}�[�N������擾
std::string ExportWriteTextFile::getParamMark(const ST_PDB_PAPER_OPTION& inPaperOption)
{
	std::string value;
	if (TRUE == inPaperOption.IS_PRE_MARK)
	{
		//���O����}�[�N�L��
		if (DEF_PRINT_BACK == inPaperOption.PRE_MARK_SIDE)
		{
			//����
			value = LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR);
		}
		else
		{
			//�\��
			value = LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR);
		}
	}
	else
	{
		//���O����}�[�N����
		value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//�g�[���J�[�u�i���́j������擾
bool ExportWriteTextFile::getToneCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve, std::string& outVal)
{
	std::stringstream sStr;
	std::string Hash;

	if (0 == strlen(inDotgainCurve.DOTGAIN_NAME))
	{
		//�g�[���J�[�u�������ݒ�i�f�t�H���g�j�̏ꍇ
		outVal = "";
		return true;
	}
	if (false == CConvertMD5Hash::CreateHash(
		(BYTE*)inDotgainCurve.ANCHOR_POINT,
		sizeof(inDotgainCurve.ANCHOR_POINT),
		Hash))
	{
			//�G���[����
			return false;
	}
	sStr << inDotgainCurve.DOTGAIN_NAME << "[" << Hash << "]";
	outVal = sStr.str();
	return true;
}

//�t���b�V���O�p�^�[��������擾
std::string ExportWriteTextFile::getFlushingPatternKind(DEF_FLUSHING_PATTERN inFlushingPatternKind)
{
	std::string value;
	switch (inFlushingPatternKind) 
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE1, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_2:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE2, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_4:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE4, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_8:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE8, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_16:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE16, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_32:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE32, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_1:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR1, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_2:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR2, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_4:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR4, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_8:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR8, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE1_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE2_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE4_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE8_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE16_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE32_STAR, RESOURCE_STR);
			break;
		default:
			value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
			break;
	}
	return value;
}

//�t���b�V���O�p�^�[�����C�������炵�ʕ�����擾
bool ExportWriteTextFile::getFlushingPatternOffset(const ST_JOB_FLUSHING_PATTERN& inFlushingPattern, std::string& outVal)
{
	switch (inFlushingPattern.flushingPattern)
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
		case DEF_FLUSHING_PATTERN_LINE_2:
		case DEF_FLUSHING_PATTERN_LINE_4:
		case DEF_FLUSHING_PATTERN_LINE_8:
		case DEF_FLUSHING_PATTERN_LINE_16:
		case DEF_FLUSHING_PATTERN_LINE_32:
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			//Line�I�����̂݁A�o�́B
			outVal = changeToD(inFlushingPattern.gap);
			return true;
	}
	return false;
}

//�T�C�h�����}�[�N���
std::string ExportWriteTextFile::getSideCheckMarkKind(const ST_JOB_VERIFY_MARK& inSideCheckMarkKind)
{
	std::string value;
	if (TRUE == inSideCheckMarkKind.verifyMark
		&& TRUE == inSideCheckMarkKind.printStartMark)
	{
		//�T�C�h�����}�[�N�ƈ󎚊J�n�}�[�N
		value = LoadResourceString(IDS_DATA_CHECK_MARK_PRINT_BARCODE, RESOURCE_STR);
	}
	else if (FALSE == inSideCheckMarkKind.verifyMark
		&& TRUE == inSideCheckMarkKind.printStartMark)
	{
		//����J�n�}�[�N
		value = LoadResourceString(IDS_DATA_CHECK_MARK_PRINT, RESOURCE_STR);
	}
	else
	{
		//�}�[�N�Ȃ�
		value = LoadResourceString(IDS_DATA_CHECK_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//�㏈���}�[�N��ޕ�����擾
std::string ExportWriteTextFile::getPostMarkKind(DEF_SELECT_SIDE inSide)
{
	std::string value;
	if (DEF_SELECT_SIDE_BOTH == inSide)
	{
		//����
		value = LoadResourceString(IDS_DATA_POST_MARK_BOTH, RESOURCE_STR);
	}
	else if (DEF_SELECT_SIDE_BACK == inSide)
	{
		//����
		value = LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR);
	}
	else if (DEF_SELECT_SIDE_FRONT == inSide)
	{
		//�\��
		value = LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR);
	}
	else
	{
		//�Ȃ�
		value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//������N�ʂ܂łɌv�Z�i������N+1�ʂ��l�̌ܓ��j���ĕԂ�
double ExportWriteTextFile::roundD(double inSrc, int inLen)
{
	double adjustedValue = 0.5;
	if(inSrc < 0.0)
	{
		// ���͒l�����̒l�̏ꍇ
		adjustedValue = (-0.5);
	}

	if (0 == inLen)
	{
		return static_cast<double>(static_cast<int>(inSrc + adjustedValue));
	}
	else
	{
		double srcValue = inSrc * pow(10.0, inLen);
		double destValue = static_cast<double>(static_cast<int>(srcValue + adjustedValue));
		return destValue * pow(10.0, -inLen);
	}
}

//�t���b�V���O�p�^�[����Line*���ǂ����擾
bool ExportWriteTextFile::isFlushingPatternLine(DEF_FLUSHING_PATTERN inFlushingPatternKind)
{
	bool isFlushingPatternLine = false;

	switch (inFlushingPatternKind)
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
		case DEF_FLUSHING_PATTERN_LINE_2:
		case DEF_FLUSHING_PATTERN_LINE_4:
		case DEF_FLUSHING_PATTERN_LINE_8:
		case DEF_FLUSHING_PATTERN_LINE_16:
		case DEF_FLUSHING_PATTERN_LINE_32:
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			isFlushingPatternLine = true;
			break;
		default:
			isFlushingPatternLine = false;
			break;
	}
	return isFlushingPatternLine;
}

// �����~�ݒ蕶����擾
std::string ExportWriteTextFile::getPrintStopSettingKind(const EPrintStopSetting& inPrintStopSettingKind)
{
	std::string value;
	switch (inPrintStopSettingKind)
	{
	case kStopIfMissingNozzle:
		value = LoadResourceString(IDS_DATA_STOP_PRINTING, RESOURCE_STR);
		break;
	case kNotStopIfMissingNozzle:
		value = LoadResourceString(IDS_DATA_KEEP_PRINTING, RESOURCE_STR);
		break;
	default:
		break;
	}
	return value;
}
