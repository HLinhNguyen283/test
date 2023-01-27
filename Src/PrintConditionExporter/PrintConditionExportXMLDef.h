/*! \file PrintConditionExportXMLDef.h
 *  \brief PrintConditionExportXMLDef.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
 *  \date 2022/04/27 cec.k-miyachi #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
*/
#pragma once

// ==========================================================================
// Include files

#include <array>
#include <map>
// ==========================================================================

namespace export_xml
{
	
static const int AnchorPointColors = 4;	//!< �A���J�[�|�C���g���擾�p�̐F��
static const int AnchorPointNum = 5;	//!< �A���J�[�|�C���g��


typedef std::pair<long, long> xypair_t; //!< �Q�̏����ꏏ�ɕێ�����p�̌^��`(long�^)
typedef std::pair<double, double> xypair_double; //!< �Q�̏����ꏏ�ɕێ�����p�̌^��`(double�^)

//! �p���ݒ���
struct SPaperSetting
{
	double m_width;					//!< �p����[�P��:point]
	long m_thickness;				//!< �p������[�P��:��m]
	long m_tension;					//!< �e���V����
	bool m_DNSStatus;				//!< DNS�ݒ�
	bool m_punchHole;				//!< �p���`���p��
	bool m_sewing;					//!< �~�V���ڗp��
	bool m_marker;					//!< ���O�}�[�N
	bool m_prePrinted;				//!< ���O���
	double m_markerOffset;			//!< �}�[�N�I�t�Z�b�g(�呖��)[�P��:point]
	double m_pageFeedSize;			//!< �y�[�W����T�C�Y[�P��:point]
	std::string m_pageFeedSizeUnit;	//!< �y�[�W����T�C�Y�̒P��
	std::string m_markSide;			//!< �}�[�N�����
	long m_repeatSheets;			//!< �~�V���ڃp�^�[���̌J��Ԃ���

	SPaperSetting()
	{
		m_width = 0.0;
		m_thickness = 0;
		m_tension = 0;
		m_punchHole = false;
		m_sewing = false;
		m_marker = false;
		m_prePrinted = false;
		m_markerOffset = 0.0;
		m_pageFeedSize = 0.0;
		m_repeatSheets = 0;
	}
};

//! �������x���
struct SDryerTemperture
{
	long m_heatRollerSelf;	//!< �q�[�g���[���[�������x(���@)[�P��:��]
	long m_heatRollerPart;	//!< �q�[�g���[���[�������x(���@)[�P��:��]
	long m_heatFanSelf;		//!< �����A�V�X�g�������x(���@)[�P��:��]
	long m_heatFanPart;		//!< �����A�V�X�g�������x(���@)[�P��:��]
	long m_precoater1;		//!< ��h�芣�����x1[�P��:��]
	long m_precoater2;		//!< ��h�芣�����x2[�P��:��]
	long m_NirPowerSelf;	//!< NIR�ݒ�i���@�j[kW]
	long m_NirPowerPart;	//!< NIR�ݒ�i���@�j[kW]
	long m_SubHeatRollerSelf;	//!< �T�u�q�[�g���[���[�������x(���@)[�P��:��]
	long m_SubHeatRollerPart;	//!< �T�u�q�[�g���[���[�������x(���@)[�P��:��]

	SDryerTemperture()
	{
		m_heatRollerSelf = 0;
		m_heatRollerPart = 0;
		m_heatFanSelf = 0;
		m_heatFanPart = 0;
		m_precoater1 = 0;
		m_precoater2 = 0;
		m_NirPowerSelf = 0;
		m_NirPowerPart = 0;
		m_SubHeatRollerSelf = 0;
		m_SubHeatRollerPart = 0;
	}
};

//! ICC�v���Z�b�g���
struct SICCPreset
{
	std::string m_descriptiveName; //!< ICC�v���Z�b�g��
	std::string m_presetID; 	   //!< ICC�v���Z�b�gID

	SICCPreset()
	{
	}
};

//! �W���u��{�ݒ�
struct SJobBase
{
	bool m_informationSheet;	//!< �C���t�H���[�V�����V�[�g���o�͂��邩�ǂ���
	bool m_flushingPage;		//!< �t���b�V���O�y�[�W��}�����邩�ǂ���
	std::string m_patternName;	//!< �t���b�V���O�y�[�W�̃p�^�[����
	long m_interval;			//!< �t���b�V���O�y�[�W�̑}���Ԋu
	bool m_preJob;			//!< �W���u�O�ɑ}�����邩�ǂ���
	long m_preJobCount;		//!< �W���u�O�ɑ}�������
	bool m_postJob;			//!< �W���u��ɑ}�����邩�ǂ���
	long m_offsetFront;		//!< �t���b�V���O�y�[�W�̃I�t�Z�b�g�i�\�ʁE�������j[�P��:0.001mm]
	long m_offsetBack;		//!< �t���b�V���O�y�[�W�̃I�t�Z�b�g�i���ʁE�������j[�P��:0.001mm]
	long m_cleaningJudgmentLevelBeforeJob;		//!< �W���u�O�̃N���[�j���O���背�x��
	long m_cleaningJudgmentLevelAfterJob;		//!< �W���u��̃N���[�j���O���背�x��
	EPrintStopSetting m_printStopSetting;		//!< �����~�ݒ�

	SJobBase()
	{
		m_informationSheet = false;
		m_flushingPage = false;
		m_interval = 0;
		m_preJob = false;
		m_preJobCount = 0;
		m_postJob = false;
		m_offsetFront = 0;
		m_offsetBack = 0;
		m_cleaningJudgmentLevelBeforeJob = 0;
		m_cleaningJudgmentLevelAfterJob = 0;
		m_printStopSetting = kNotStopIfMissingNozzle;
	}
};

//! ���C�A�E�g���
struct SLayout
{
	xypair_t m_offsetFront;	//!< �摜�̃I�t�Z�b�g�i�\�ʁE�啛�j[�P��:0.001mm]
	xypair_t m_offsetBack;	//!< �摜�̃I�t�Z�b�g�i���ʁE�啛�j[�P��:0.001mm]

	bool m_verification;			//!< �T�C�h���������邩�ǂ���
	long m_continuousPageCount;		//!< �G���[�ɂ���A���V�[�g��
	bool m_cueMark;					//!< ����J�n�}�[�N���o�͂��邩�ǂ���
	bool m_barcode;					//!< �o�[�R�[�h���o�͂��邩�ǂ���
	xypair_t m_barcodeOffsetFront;	//!< �o�[�R�[�h�̃I�t�Z�b�g�i�\�ʁE�呖���j[�P��:0.001mm]
	xypair_t m_barcodeOffsetBack;	//!< �o�[�R�[�h�̃I�t�Z�b�g�i���ʁE�啛�j[�P��:0.001mm]
	std::string m_flushingPattern;	//!< �t���b�V���O�p�^�[��
	DEF_FLUSHING_PATTERN m_flushingPatternDef;	//!< �t���b�V���O�p�^�[����`
	long m_gapBetweenColors;		//!< ���C���t���b�V���O�̂��炵��
	long m_lineOffsetFront;			//!< ���C���t���b�V���O�̃I�t�Z�b�g�i�\�ʁE�������j[�P��:0.001mm]
	long m_lineOffsetBack;			//!< ���C���t���b�V���O�̃I�t�Z�b�g�i���ʁE�������j[�P��:0.001mm]
	bool m_LineGap;					//!< �y�[�W���A���C���t���b�V���O
	long m_LineGapFront;			//!< �y�[�W���A���C���t���b�V���O�Ԋu�i�\�ʁE�������j[�P��:0.001mm]
	long m_LineGapBack;				//!< �y�[�W���A���C���t���b�V���O�Ԋu�i���ʁE�������j[�P��:0.001mm]
	long m_LineGapOffsetFront;		//!< �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i�\�ʁE�������j[�P��:0.001mm]
	long m_LineGapOffsetBack;		//!< �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i���ʁE�������j[�P��:0.001mm]
	std::string m_LinePosition;		//!< �y�[�W���A���C���t���b�V���O�̕\���ʒu("Top" or "Bottom")
	std::string m_LineOptions;		//!< �y�[�W���A���C���t���b�V���O�̃I�v�V�������
	long m_cutterMark;				//!< �J�b�^�[�}�[�N�̈����
	long m_cutterMarkPageCount;		//!< �J�b�^�[�}�[�N�̏����V�[�g��
	xypair_t m_cutterMarkSize;		//!< �J�b�^�[�}�[�N�̃T�C�Y�i�啛�j[�P��:0.001mm]
	long m_cutterMarkOffsetFront;	//!< �J�b�^�[�}�[�N�̃I�t�Z�b�g�i�\�ʁE�呖���j[�P��:0.001mm]
	long m_cutterMarkOffsetBack;	//!< �J�b�^�[�}�[�N�̃I�t�Z�b�g�i���ʁE�呖���j[�P��:0.001mm]
	long m_cutterMarkAlignFront;	//!< �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu�i�\�ʁj
	long m_cutterMarkAlignBack;		//!< �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu�i���ʁj

	// ADD --> #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
	bool m_adjustmentSheetSizeEnabled;	//!< �㏈���}�[�N�̒����V�[�g�}�[�N�T�C�Y�L��(TRUE)/����(FALSE)
	long m_adjustmentSheetLength;	//!< �㏈���}�[�N�̒����V�[�g�}�[�N����[�P��:0.001mm]
	ADJUSTMENT_SHEET_AFTER_JOB_OPTION m_adjustmentSheetAfterJob;	//!< �㏈���}�[�N�̃W���u��̃V�[�g�����w�肷����@��I������I�v�V����
	long m_afterJobSheets;			//!< �㏈���}�[�N�̃W���u��V�[�g��
	// ADD <-- #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j

	SLayout()
	{
		m_offsetFront.first = 0;
		m_offsetFront.second = 0;
		m_offsetBack.first = 0;
		m_offsetBack.second = 0;
		m_verification = false;
		m_continuousPageCount = 0;
		m_cueMark = false;
		m_barcode = false;
		m_barcodeOffsetFront.first = 0;
		m_barcodeOffsetFront.second = 0;
		m_barcodeOffsetBack.first = 0;
		m_barcodeOffsetBack.second = 0;
		m_flushingPatternDef = DEF_FLUSHING_PATTERN_NONE;
		m_gapBetweenColors = 0;
		m_lineOffsetFront = 0;
		m_lineOffsetBack = 0;
		m_LineGap = false;
		m_LineGapFront = 0;
		m_LineGapBack = 0;
		m_LineGapOffsetFront = 0;
		m_LineGapOffsetBack = 0;
		m_cutterMark = 0;
		m_cutterMarkPageCount = 0;
		m_cutterMarkSize.first = 0;
		m_cutterMarkSize.second = 0;
		m_cutterMarkOffsetFront = 0;
		m_cutterMarkOffsetBack = 0;
		m_cutterMarkAlignFront = 0;
		m_cutterMarkAlignBack = 0;

		// ADD --> #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
		m_adjustmentSheetLength = 0;
		m_afterJobSheets = 0;
		m_adjustmentSheetAfterJob = NONE;
		m_adjustmentSheetSizeEnabled = false;
		// ADD <-- #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
	}
};

//! �A���J�[�|�C���g���
struct AnchorPoint
{	
	long m_beforeConPoint;		//!< ��Z�x�l
	long m_afterConPoint;		//!< �J�[�u�f�[�^�̐��l
	long m_anchorPointEditType;	//!< �ҏW�^�C�v
	long m_anchorPointType;		//!< �A���J�[�|�C���g���
	AnchorPoint()
	{	
		m_beforeConPoint = 0;
		m_afterConPoint = 0;
		m_anchorPointEditType = 0;
		m_anchorPointType = 0;
	}
};

//! 1�F���̃A���J�[�|�C���g���(5�_��)
struct OneColorPoint
{
	std::array<AnchorPoint, AnchorPointNum> point; //!< 1�F���̃A���J�[�|�C���g
	OneColorPoint()
	{
	}
};

//! �g�[���J�[�u���
struct SToneCurve
{
	std::string m_side;				//!< �\�ʁE���ʂ̋��
	std::string m_descriptiveName;	//!< �g�[���J�[�u��
	std::map<std::string, OneColorPoint> anchorPoint;	//!< �A���J�[�|�C���g���4�F�~5�_��

	SToneCurve()
	{
	}
};

//! JetInspection���
struct SJetInspection
{
	std::string m_ParamaterName;		//!< ����������
	std::string m_TemporalMaskFilePath;	//!< �ꎞ�}�X�N�t�@�C���p�X

	SJetInspection()
	{
	}
};

//! JetInspection�񌟍���`�G���A���
struct SJetInspectionNonInspectionRect
{
	long m_ID;					//!< �񌟍���`�G���AID
	xypair_double m_StartPos;	//!< �񌟍���`�G���A�J�n�ʒu
	xypair_double m_EndPos;		//!< �񌟍���`�G���A�I���ʒu

	SJetInspectionNonInspectionRect()
	{
		m_ID = 0;
		m_StartPos.first = 0.0;
		m_StartPos.second = 0;
		m_EndPos.first = 0.0;
		m_EndPos.second = 0.0;
	}
};

//! JetInspection�G���A���
struct SJetInspectionArea
{
	std::string m_side;													//!< �����ݒ�A�Ώۖ�
	double m_NonInspectionAreaLeft;										//!< �񌟍��G���A���[
	double m_NonInspectionAreaRight;									//!< �񌟍��G���A�E�[
	std::vector<SJetInspectionNonInspectionRect> m_NonInspectionRect;	//!< JetInspection�񌟍���`�G���A���
	
	SJetInspectionArea()
	{
		m_NonInspectionAreaLeft = 0.0;
		m_NonInspectionAreaRight = 0.0;
	}
};

//! JetInspection�f�R�[�h�ڍ׏��
struct SJetInspectionDecodeDetails
{
	std::string m_Side;				//!< �Ώۖʁi�\ or ���j
	xypair_double m_Size;			//!< �T�C�Y�ݒ�
	xypair_double m_Offset;			//!< �ʒu�ݒ�
	std::string m_Name;				//!< ���x������
	std::string m_Type;				//!< ���x�����
	std::string m_Angle;			//!< �iOCR����j�p�x���
	bool m_ErrorStop;				//!< �f�R�[�h�G���[�������A�G���[��~����J�E���g�̑Ώۂɂ��邩

	SJetInspectionDecodeDetails()
	{
		m_Size.first = 0.0;
		m_Size.second = 0.0;
		m_Offset.first = 0.0;
		m_Offset.second = 0.0;
		m_ErrorStop = false;
	}
};

//! JetInspection�f�R�[�h���
struct SJetInspectionDecode
{
	long m_ErrorSheets;											//!< �u�f�R�[�h�v�^�u�A�G���[��~�A���V�[�g��
	std::string m_Unit;											//!< ���l�P�ʌn
	bool m_IsEnabled;											//!< �f�R�[�h�����̗L���^����
	std::vector<SJetInspectionDecodeDetails> m_DetailsList;		//!< JetInspection�f�R�[�h�ڍ׏��

	SJetInspectionDecode()
	{
		m_ErrorSheets = 0;
		m_IsEnabled = false;
	}
};

}//namespace export_xml