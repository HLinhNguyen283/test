/*! \file	ExportXML.cpp
 *  \brief	ExportXML.cpp�̏���
 *  \author	pt.ju-tanaka
 *  \date	2019/06/26 pt.ju-tanaka �V�K�쐬
 *  \date	2022/04/27 cec.k-miyachi #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
*/

#include "stdafx.h"
#include "CommonCBRProc.h"
#include "fileText.h"
#include "DataSystemSetting.h"
#include "ExportXML.h"
#include "Stub_PrinterStateIF.h"

namespace export_xml
{
 //�R���X�g���N�^
ExportFile::ExportFile()
{
}

// �f�X�g���N�^
ExportFile::~ExportFile()
{
}

// XML�`���ł̈�������G�N�X�|�[�g�˗�
bool ExportFile::onRequestExportXML(
	const text::tstring& inOutputPath,
	const export_xml::PrintCondition& inPrintConditionList)
{
	bool isXmlExportLog = CDataSystemSetting::GetInstance()->IsDebugXmlExportLog();
	std::unique_ptr<xml_utility::XMLElementControl> elementXML;
	elementXML.reset(new xml_utility::XMLElementControl(_T("<PrintConditions>")));

	try
	{
		std::unique_ptr<xml_utility::XMLElementControl> elementXMLSub;
		text::tstring elementPath = _T("PrintCondition");
		elementXMLSub.reset(new xml_utility::XMLElementControl(_T("<") + elementPath + _T(">")));

		// ����������̐ݒ�
		SetPrintConditionName(inPrintConditionList, elementPath, elementXMLSub);

		// �p����ނ̐ݒ�
		SetPaperType(inPrintConditionList, elementPath, elementXMLSub);

		// �i���␳���̐ݒ�
		SetAlignment(inPrintConditionList, elementPath, elementXMLSub);

		// ������[�h�̐ݒ�
		SetPrintMode(inPrintConditionList, elementPath, elementXMLSub);

		// ������x�̐ݒ�
		SetPrintSpeed(inPrintConditionList, elementPath, elementXMLSub);

		// �p���ݒ���̐ݒ�
		SetPaperSetting(inPrintConditionList, elementPath, elementXMLSub);

		// �������x���
		SetDryerTemperature(inPrintConditionList, elementPath, elementXMLSub);

		// ICC�v���Z�b�g���̐ݒ�
		SetICCPreset(inPrintConditionList, elementPath, elementXMLSub);

		// �g�[���J�[�u���̐ݒ�
		SetToneCurve(inPrintConditionList, elementPath, elementXMLSub);

		// �W���u��{�ݒ�����̐ݒ�
		SetJobBase(inPrintConditionList, elementPath, elementXMLSub);

		// ���C�A�E�g���̐ݒ�
		SetLayout(inPrintConditionList, elementPath, elementXMLSub);

		if(2 == CDataSystemSetting::GetInstance()->GetJetInspectionOption())
		{
			// JI�֘A���̐ݒ�(JetInspector�Ƃ̐ڑ�����Ă���΃G�������g�o�͑ΏۂƂ���)
			SetJI(inPrintConditionList, elementPath, elementXMLSub);

			// JI�G���A���̐ݒ�
			SetJIArea(inPrintConditionList, elementPath, elementXMLSub);
		}

		if((Stub_PrinterStateIF::IsOCRandDECEnable()) || (2 == CDataSystemSetting::GetInstance()->GetJetInspectionOption()))
		{
			// JI�f�R�[�h���̐ݒ�
			SetJIDecode(inPrintConditionList, elementPath, elementXMLSub);
		}

		if(CDataSystemSetting::GetInstance()->IsMagnificationAdjustment())
		{
			// �ϔ{���̐ݒ�
			SetVariableMagnification(inPrintConditionList, elementPath, elementXMLSub);
		}

		elementXML->addElement(elementXMLSub);

		if(isXmlExportLog)
		{
			// XML�G�N�X�|�[�g���O�o�͂��L����
			std::stringstream exportlog;
			exportlog << "XML Exportable" << " Name:[" << inPrintConditionList.GetPrintConditionName() << "]";
			WriteToLogBuf(LOG_DEBUG, exportlog.str());
		}
	}
    catch(std::exception& e)
    {
        std::stringstream errorlog;
		errorlog << "XML Export failure Name:[" << inPrintConditionList.GetPrintConditionName() << "] Method:[" << e.what() << "]";
		WriteToLogBuf(LOG_DEBUG, errorlog.str());

		return false;
    }

	// XML�t�@�C���֏o��
	outputXML(inOutputPath, elementXML);

	return true;
}

// ����������̐ݒ�
void ExportFile::SetPrintConditionName(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ������������擾��XML�f�[�^�ɐݒ�
	outElement->setValue(_T(inElementPath + "/DescriptiveName"), inPrintCondition.GetPrintConditionName());
}

// �p����ނ̐ݒ�
void ExportFile::SetPaperType(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �p����ނ��擾��XML�f�[�^�ɐݒ�
	outElement->setValue(_T(inElementPath + "/PaperType"), inPrintCondition.GetPaperType());
}

// �i���␳���̐ݒ�
void ExportFile::SetAlignment(
	const export_xml::PrintCondition& inPrintCondition, 
	const text::tstring& inElementPath, 
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �i���␳�����擾��XML�f�[�^�ɐݒ�
	outElement->setValue(inElementPath + _T("/Alignment"), inPrintCondition.GetLevelTuneName());
}

// ������[�h�̐ݒ�
void ExportFile::SetPrintMode(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ������[�h�̎擾
	std::string printMode;
	if(!inPrintCondition.GetPrintMode(printMode))
	{
		// ������[�h�擾���s���͏����I��
		throw std::runtime_error("SetPrintMode");
	}

	// ������[�h��XML�f�[�^�ɐݒ�
	outElement->setValue(inElementPath + _T("/PrintMode"), printMode);
}

// ������x�̐ݒ�
void ExportFile::SetPrintSpeed(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ������x���擾��XML�f�[�^�ɐݒ�
	outElement->setValue(inElementPath + _T("/PrintSpeed"), text::convertNumToString<double>(inPrintCondition.GetPrintSpeed()));
}

// �p���ݒ���̐ݒ�
void ExportFile::SetPaperSetting(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �p���ݒ�����擾
	SPaperSetting paperSetting;
	if(!inPrintCondition.GetPaperSetting(paperSetting))
	{
		// �p���ݒ���擾���s���͏����I��
		throw std::runtime_error("SetPaperSetting");
	}

	text::tstring paperSettingPath = inElementPath + _T("/PaperSetting");

	// �p������XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/Width"), text::convertNumToStringPrecision<double>(paperSetting.m_width,4));

	// �p�����݂�XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/Thickness"), text::convertNumToString<long>(paperSetting.m_thickness));

	// �e���V������XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/Tension"), text::convertNumToString<long>(paperSetting.m_tension));

	// DNS��XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/DNS"), true == paperSetting.m_DNSStatus ? _T("true") : _T("false"));

	// �p���`���p����XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/PunchHole"), true == paperSetting.m_punchHole ? _T("true") : _T("false"));

	// �~�V���ڗp����XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/Sewing"), true == paperSetting.m_sewing ? _T("true") : _T("false"));

	// ���O�}�[�N��XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/Marker"), true == paperSetting.m_marker ? _T("true") : _T("false"));

	// ���O�����XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/PrePrinted"), true == paperSetting.m_prePrinted ? _T("true") : _T("false"));

	if(paperSetting.m_marker)
	{
		// �}�[�N�I�t�Z�b�g(�呖��)��XML�f�[�^�ɐݒ�(���O�}�[�N��true�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(paperSettingPath + _T("/MarkerOffset"), text::convertNumToStringPrecision<double>(paperSetting.m_markerOffset,4));
	}

	// �y�[�W����T�C�Y��XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/PageFeedSize"), text::convertNumToStringPrecision<double>(paperSetting.m_pageFeedSize,4));

	// �y�[�W����T�C�Y�̒P�ʂ�XML�f�[�^�ɐݒ�
	outElement->setValue(paperSettingPath + _T("/PageFeedSizeUnit"), paperSetting.m_pageFeedSizeUnit);

	if(paperSetting.m_marker)
	{
		// �}�[�N�̈���ʂ�XML�f�[�^�ɐݒ�(���O�}�[�N��true�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(paperSettingPath + _T("/MarkSide"), paperSetting.m_markSide);
	}
}

// �������x���̐ݒ�
void ExportFile::SetDryerTemperature(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �������x�����擾
	SDryerTemperture dryerTemperture;
	inPrintCondition.GetDryerTemperature(dryerTemperture);
	text::tstring dryerTemperaturePath = inElementPath + _T("/DryerTemperature");

	// �q�[�g���[���[�������x(���@)��XML�f�[�^�ɐݒ�
	outElement->setValue(dryerTemperaturePath + _T("/HeatRollerSelf"), text::convertNumToString<long>(dryerTemperture.m_heatRollerSelf));

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// �q�[�g���[���[�������x(����@)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(dryerTemperaturePath + _T("/HeatRollerPart"), text::convertNumToString<long>(dryerTemperture.m_heatRollerPart));
	}

	long heaterFanSelf = Stub_PrinterStateIF::IsLoadingHeaterFan(true);
	long heaterFanPart = Stub_PrinterStateIF::IsLoadingHeaterFan(false);
	if((0 != heaterFanSelf) || (0 != heaterFanPart))
	{
		// �����A�V�X�g�������x(���@)��XML�f�[�^�ɐݒ�(���@or����@�̂ǂ��炩1�ł������t�@�����ڂł���΃G�������g�o�͂���))
		outElement->setValue(dryerTemperaturePath + _T("/HeatFanSelf"), text::convertNumToString<long>(dryerTemperture.m_heatFanSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// �����A�V�X�g�������x(����@)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�G�������g�o�͂���)
			outElement->setValue(dryerTemperaturePath + _T("/HeatFanPart"), text::convertNumToString<long>(dryerTemperture.m_heatFanPart));
		}
	}

	bool isUnitOfNirSelf = Stub_PrinterStateIF::IsUnitOfNir(true);
	bool isUnitOfNirPart = Stub_PrinterStateIF::IsUnitOfNir(false);
	if((isUnitOfNirSelf) || (isUnitOfNirPart))
	{
		// NIR�ݒ�(���@)��XML�f�[�^�ɐݒ�(���@or����@�̂ǂ��炩1�ł�NIR���ڂł���΃G�������g�o�͂���))
		outElement->setValue(dryerTemperaturePath + _T("/NIRPowerSelf"), text::convertNumToString<long>(dryerTemperture.m_NirPowerSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// NIR�ݒ�(����@)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�G�������g�o�͂���)
			outElement->setValue(dryerTemperaturePath + _T("/NIRPowerPart"), text::convertNumToString<long>(dryerTemperture.m_NirPowerPart));
		}
	}

	bool isPreCoat = Stub_PrinterStateIF::IsLoadingPreCoat();
	if(isPreCoat)
	{
		// ��h�芣�����x�P��XML�f�[�^�ɐݒ�(�v���R�[�g���ڂł���΃G�������g�o�͂���))
		outElement->setValue(dryerTemperaturePath + _T("/Precoater1"), text::convertNumToString<long>(dryerTemperture.m_precoater1));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// NIR�ݒ�(����@)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�G�������g�o�͂���)
			outElement->setValue(dryerTemperaturePath + _T("/Precoater2"), text::convertNumToString<long>(dryerTemperture.m_precoater2));
		}
	}

	bool isUnitOfRanger = Stub_PrinterStateIF::IsUnitOfRanger();
	if(isUnitOfRanger)
	{
		// �T�u�q�[�g���[���[�������x(���@)��XML�f�[�^�ɐݒ�
		outElement->setValue(dryerTemperaturePath + _T("/SubHeatRollerSelf"), text::convertNumToString<long>(dryerTemperture.m_SubHeatRollerSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// �T�u�q�[�g���[���[�������x(����@)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�̂݃G�������g�o�͂���)
			outElement->setValue(dryerTemperaturePath + _T("/SubHeatRollerPart"), text::convertNumToString<long>(dryerTemperture.m_SubHeatRollerPart));
		}
	}
}

// ICC�v���Z�b�g���̐ݒ�
void ExportFile::SetICCPreset(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ICC�v���Z�b�g�����擾
	SICCPreset iccPreset;
	inPrintCondition.GetICCPreset(iccPreset);
	text::tstring iccPresetPath = inElementPath + _T("/ICCPreset");

	// ICC�v���Z�b�g����XML�f�[�^�ɐݒ�
	outElement->setValue(iccPresetPath + _T("/DescriptiveName"), iccPreset.m_descriptiveName);

	// ICC�v���Z�b�gID��XML�f�[�^�ɐݒ�
	outElement->setValue(iccPresetPath + _T("/ID"), iccPreset.m_presetID);
}

// �g�[���J�[�u���̐ݒ�
void ExportFile::SetToneCurve(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �A���J�[�|�C���g���ݒ菈��
	auto setAnchorPoint = [](
		std::unique_ptr<xml_utility::XMLElementControl>& outElement,
		const export_xml::PrintCondition& inPrintCondition,
		const text::tstring& inElementPath,
		const SToneCurve& inToneCurve,
		const text::tstring& inToneCurvePath)
	{
		text::tstring colorRootPath = inToneCurvePath + _T("/AnchorPoint");
		// PartIDKeys��XML�f�[�^�ɐݒ�
		outElement->setValue(colorRootPath + _T("/PartIDKeys"), _T("Color PointID"));

		// �e�F�̃A���J�[�|�C���g����ݒ�
		for(auto itr = inToneCurve.anchorPoint.begin(); itr != inToneCurve.anchorPoint.end(); ++itr)
		{
			size_t index = std::distance(inToneCurve.anchorPoint.begin(), itr);
			text::tstring colorPath = inToneCurvePath + _T("/AnchorPoint/AnchorPoint<") + text::convertNumToTString<size_t>(index) + _T(">");

			// �F����XML�f�[�^�ɐݒ�
			outElement->setValue(colorPath + _T("/Color"), itr->first);

			// �A���J�[�|�C���g����ݒ�
			for(int cnt = 0; cnt < AnchorPointNum; ++cnt)
			{
				text::tstring anchorPointPath = colorPath + _T("/AnchorPoint<") + text::convertNumToTString<size_t>(cnt) + _T(">");

				// Point ID��XML�f�[�^�ɐݒ�
				outElement->setValue(_T(anchorPointPath + "/PointID"), text::convertNumToString<int>(cnt));

				// ��Z�x�l��XML�f�[�^�ɐݒ�
				outElement->setValue(_T(anchorPointPath + "/BeforeConPoint"), text::convertNumToString<long>(itr->second.point[cnt].m_beforeConPoint));

				// �J�[�u�f�[�^�̐��l��XML�f�[�^�ɐݒ�
				outElement->setValue(_T(anchorPointPath + "/AfterConPoint"), text::convertNumToString<long>(itr->second.point[cnt].m_afterConPoint));

				// �ҏW�^�C�v��XML�f�[�^�ɐݒ�
				outElement->setValue(_T(anchorPointPath + "/AnchorPointEditType"), text::convertNumToString<long>(itr->second.point[cnt].m_anchorPointEditType));

				// �J�[�u�f�[�^�̕�����XML�f�[�^�ɐݒ�
				outElement->setValue(_T(anchorPointPath + "/AnchorPointType"), text::convertNumToString<long>(itr->second.point[cnt].m_anchorPointType));
			}
		}
	};

	//	���C������

	// �\�ʂ̃g�[���J�[�u�����擾
	SToneCurve toneCurveFront;
	if(!inPrintCondition.GetToneCurve(toneCurveFront, true))
	{
		// �g�[���J�[�u���̎擾���s��
		return;
	}

	text::tstring toneCurveFrontRootPath = inElementPath + _T("/ToneCurve");
	// PartIDKeys��XML�f�[�^�ɐݒ�
	outElement->setValue(toneCurveFrontRootPath + _T("/PartIDKeys"), _T("Side"));

	text::tstring toneCurveFrontPath = inElementPath + _T("/ToneCurve/ToneCurve<") + text::convertNumToTString<int>(0) + _T(">");

	// �\�ʁE���ʂ̋�ʂ�XML�f�[�^�ɐݒ�
	outElement->setValue(toneCurveFrontPath + _T("/Side"), toneCurveFront.m_side);
	// �\�ʂ̃g�[���J�[�u����XML�f�[�^�ɐݒ�
	outElement->setValue(toneCurveFrontPath + _T("/DescriptiveName"), toneCurveFront.m_descriptiveName);
	// �\�ʂ̊e�F�̃A���J�[�|�C���g����ݒ�
	setAnchorPoint(outElement, inPrintCondition, inElementPath, toneCurveFront, toneCurveFrontPath);


	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// ���ʂ̃g�[���J�[�u�����擾
		SToneCurve toneCurveBack;
		if(!inPrintCondition.GetToneCurve(toneCurveBack, false))
		{
			// �g�[���J�[�u���̎擾���s��
			return;
		}
		text::tstring toneCurveBackPath = inElementPath + _T("/ToneCurve/ToneCurve<") + text::convertNumToTString<int>(1) + _T(">");

		// �\�ʁE���ʂ̋�ʂ�XML�f�[�^�ɐݒ�
		outElement->setValue(toneCurveBackPath + _T("/Side"), toneCurveBack.m_side);
		// ���ʂ̃g�[���J�[�u����XML�f�[�^�ɐݒ�
		outElement->setValue(toneCurveBackPath + _T("/DescriptiveName"), toneCurveBack.m_descriptiveName);
		// ���ʂ̊e�F�̃A���J�[�|�C���g����ݒ�
		setAnchorPoint(outElement, inPrintCondition, inElementPath, toneCurveBack, toneCurveBackPath);
	}
}

// �W���u��{�ݒ���̐ݒ�
void ExportFile::SetJobBase(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �W���u��{�ݒ�����擾
	SJobBase jobBase;
	inPrintCondition.GetJobBase(jobBase);
	text::tstring jobBasePath = inElementPath + _T("/JobBase");

	// �C���t�H���[�V�����V�[�g�̏o�͗L����XML�f�[�^�ɐݒ�
	outElement->setValue(jobBasePath + _T("/InformationSheet"), true == jobBase.m_informationSheet ? _T("true") : _T("false"));

	// �t���b�V���O�y�[�W�̑}���L����XML�f�[�^�ɐݒ�
	outElement->setValue(jobBasePath + _T("/FlushingPage"), true == jobBase.m_flushingPage ? _T("true") : _T("false"));

	if(jobBase.m_flushingPage)
	{
		// �t���b�V���O�y�[�W�̃p�^�[������XML�f�[�^�ɐݒ�(�t���b�V���O�y�[�W�̑}���L�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(jobBasePath + _T("/PatternName"), jobBase.m_patternName);

		// �t���b�V���O�y�[�W�̑}���Ԋu��XML�f�[�^�ɐݒ�(�t���b�V���O�y�[�W�̑}���L�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(jobBasePath + _T("/Interval"), text::convertNumToString<long>(jobBase.m_interval));
	}

	// �W���u�O�̑}���L����XML�f�[�^�ɐݒ�
	outElement->setValue(jobBasePath + _T("/PreJob"), true == jobBase.m_preJob ? _T("true") : _T("false"));

	if(jobBase.m_preJob)
	{
		// �W���u�O�ɑ}������񐔂�XML�f�[�^�ɐݒ�(�W���u�O�ɑ}������ݒ�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(jobBasePath + _T("/PreJobCount"), text::convertNumToString<long>(jobBase.m_preJobCount));
	}

	// �W���u��̑}���L����XML�f�[�^�ɐݒ�
	outElement->setValue(jobBasePath + _T("/PostJob"), true == jobBase.m_postJob ? _T("true") : _T("false"));

	if(jobBase.m_flushingPage)
	{
		// �t���b�V���O�y�[�W�̃I�t�Z�b�g(�\�ʁE������)��XML�f�[�^�ɐݒ�(�t���b�V���O�y�[�W�̑}���L�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(jobBasePath + _T("/OffsetFront"), text::convertNumToString<long>(jobBase.m_offsetFront));
	}

	if((jobBase.m_flushingPage) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// �t���b�V���O�y�[�W�̃I�t�Z�b�g(���ʁE������)��XML�f�[�^�ɐݒ�(�t���b�V���O�y�[�W�̑}���L���ADED�\���̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(jobBasePath + _T("/OffsetBack"), text::convertNumToString<long>(jobBase.m_offsetBack));
	}

	// �t���b�V���O�y�[�W=�L���
	// �m�Y�������`�F�b�N�@�\���L�� ���� �m�Y�������`�F�b�N���\�ȃt���b�V���O�y�[�W���ݒ肳��Ă���ꍇ
	if (jobBase.m_flushingPage &&
		CDataSystemSetting::GetInstance()->isNozzleCheckEnabled(jobBase.m_patternName))
	{
		// �W���u�O�̃N���[�j���O���背�x��
		outElement->setValue(jobBasePath + _T("/CleaningJudgmentLevelBeforeJob"), text::convertNumToString<long>(jobBase.m_cleaningJudgmentLevelBeforeJob));
		// �W���u��̃N���[�j���O���背�x��
		outElement->setValue(jobBasePath + _T("/CleaningJudgmentLevelAfterJob"), text::convertNumToString<long>(jobBase.m_cleaningJudgmentLevelAfterJob));
		// �����~�ݒ�
		outElement->setValue(jobBasePath + _T("/PrintStopSetting"), jobBase.m_printStopSetting == kStopIfMissingNozzle ? _T("Stop") : _T("NotStop"));
	}
}

// ���C�A�E�g���̐ݒ�
void ExportFile::SetLayout(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ���C�A�E�g�����擾
	SLayout layout;
	if(!inPrintCondition.GetLayout(layout))
	{
		// ���C�A�E�g���擾���s���͏����I��
		throw std::runtime_error("SetLayout");
	}

	text::tstring layoutPath = inElementPath + _T("/Layout");

	// �摜�̃I�t�Z�b�g(�\�ʁE�啛)��XML�f�[�^�ɐݒ�
	setXYpairParameter(outElement, layoutPath + _T("/OffsetFront"), layout.m_offsetFront.first, layout.m_offsetFront.second);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// �摜�̃I�t�Z�b�g(���ʁE�啛)��XML�f�[�^�ɐݒ�(DED�\���̏ꍇ�̂݃G�������g�o�͂���)
		setXYpairParameter(outElement, layoutPath + _T("/OffsetBack"), layout.m_offsetBack.first, layout.m_offsetBack.second);
	}

	// �T�C�h�����̗L����XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/Verification"), true == layout.m_verification ? _T("true") : _T("false"));

	if(layout.m_verification)
	{
		// �G���[�ɂ���A���V�[�g����XML�f�[�^�ɐݒ�(�T�C�h����������ݒ�̏ꍇ�̂݃G�������g�o�͂���)
		outElement->setValue(layoutPath + _T("/ContinuousPageCount"), text::convertNumToString<long>(layout.m_continuousPageCount));
	}

	// ����J�n�}�[�N�̏o�͗L����XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/CueMark"), true == layout.m_cueMark ? _T("true") : _T("false"));

	// �o�[�R�[�h�̏o�͗L����XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/Barcode"), true == layout.m_barcode ? _T("true") : _T("false"));

	if(layout.m_barcode)
	{
		// �o�[�R�[�h�̃I�t�Z�b�g(�\�ʁE�呖���B��������0 �Œ�)��XML�f�[�^�ɐݒ�(�o�[�R�[�h�o�͂���ݒ�̏ꍇ�̂݃G�������g�o�͂���)
		setXYpairParameter(outElement, layoutPath + _T("/BarcodeOffsetFront"), layout.m_barcodeOffsetFront.first, layout.m_barcodeOffsetFront.second);
	}

	if((layout.m_barcode) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// �o�[�R�[�h�̃I�t�Z�b�g(���ʁE�啛)��XML�f�[�^�ɐݒ�(�o�[�R�[�h�o�͂���ݒ肩�ADED�\���̏ꍇ�̂݃G�������g�o�͂���)
		setXYpairParameter(outElement, layoutPath + _T("/BarcodeOffsetBack"), layout.m_barcodeOffsetBack.first, layout.m_barcodeOffsetBack.second);
	}

	// �t���b�V���O�p�^�[����XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/FlushingPattern"), layout.m_flushingPattern);

	// ���C���t���b�V���O���ݒ肳��Ă��邩���f����
	if(isLineFlushing(layout.m_flushingPatternDef))
	{
		// ���C���t���b�V���O�ݒ莞�̏ꍇ�Ɋe�G�������g�o�͂���

		// ���C���t���b�V���O�̂��炵�ʂ�XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/GapBetweenColors"), text::convertNumToString<long>(layout.m_gapBetweenColors));

		// ���C���t���b�V���O�̃I�t�Z�b�g(�\�ʁE������)��XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/LineOffsetFront"), text::convertNumToString<long>(layout.m_lineOffsetFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// ���C���t���b�V���O�̃I�t�Z�b�g(���ʁE������)��XML�f�[�^�ɐݒ�
			outElement->setValue(layoutPath + _T("/LineOffsetBack"), text::convertNumToString<long>(layout.m_lineOffsetBack));
		}

		// �y�[�W���A���C���t���b�V���O��XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/LineGap"), true == layout.m_LineGap ? _T("true") : _T("false"));

		// �y�[�W���A���C���t���b�V���O�Ԋu�i�\�ʁE�������j��XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/LineGapFront"), text::convertNumToString<long>(layout.m_LineGapFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// �y�[�W���A���C���t���b�V���O�Ԋu�i���ʁE�������j��XML�f�[�^�ɐݒ�
			outElement->setValue(layoutPath + _T("/LineGapBack"), text::convertNumToString<long>(layout.m_LineGapBack));
		}

		// �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i�\�ʁE�������j��XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/LineGapOffsetFront"), text::convertNumToString<long>(layout.m_LineGapOffsetFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// �y�[�W���A���C���t���b�V���O�̕␳�I�t�Z�b�g�i���ʁE�������j��XML�f�[�^�ɐݒ�
			outElement->setValue(layoutPath + _T("/LineGapOffsetBack"), text::convertNumToString<long>(layout.m_LineGapOffsetBack));
		}

		// �y�[�W���A���C���t���b�V���O�̃I�v�V��������XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/LineOptions"), layout.m_LineOptions);
	}

	// �y�[�W���A���C���t���b�V���O�̕\���ʒu��XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/LinePosition"), layout.m_LinePosition);

	// �J�b�^�[�}�[�N�̈���ʂ�XML�f�[�^�ɐݒ�
	outElement->setValue(layoutPath + _T("/CutterMark"), text::convertNumToString<long>(layout.m_cutterMark));

	if(0 != layout.m_cutterMark)
	{
		// CutterMark��off(0)�ȊO�̏ꍇ�Ɋe�G�������g�o�͂���

		// �J�b�^�[�}�[�N�̏����V�[�g����XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/CutterMarkPageCount"), text::convertNumToString<long>(layout.m_cutterMarkPageCount));

		// �J�b�^�[�}�[�N�̃T�C�Y(�啛)��XML�f�[�^�ɐݒ�
		setXYpairParameter(outElement, layoutPath + _T("/CutterMarkSize"), layout.m_cutterMarkSize.first, layout.m_cutterMarkSize.second);

		// �J�b�^�[�}�[�N�̃I�t�Z�b�g(�\�ʁE�呖��)��XML�f�[�^�ɐݒ�
		outElement->setValue(layoutPath + _T("/CutterMarkOffsetFront"), text::convertNumToString<long>(layout.m_cutterMarkOffsetFront));
	}

	if((0 != layout.m_cutterMark) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// �J�b�^�[�}�[�N�̃I�t�Z�b�g(���ʁE�呖��)��XML�f�[�^�ɐݒ�(CutterMark��off(0)�ȊO���ADED�\���̏ꍇ�ɃG�������g�o�͂���)
		outElement->setValue(layoutPath + _T("/CutterMarkOffsetBack"), text::convertNumToString<long>(layout.m_cutterMarkOffsetBack));
	}

	if(0 != layout.m_cutterMark)
	{
		// �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu(�\��)��XML�f�[�^�ɐݒ�(CutterMark��off(0)�ȊO�̏ꍇ�ɃG�������g�o�͂���)
		outElement->setValue(layoutPath + _T("/CutterMarkAlignFront"), text::convertNumToString<long>(layout.m_cutterMarkAlignFront));
	}

	if((0 != layout.m_cutterMark) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// �J�b�^�[�}�[�N�I�t�Z�b�g�̊�ʒu(����)��XML�f�[�^�ɐݒ�(CutterMark��off(0)�ȊO���ADED�\���̏ꍇ�ɃG�������g�o�͂���)
		outElement->setValue(layoutPath + _T("/CutterMarkAlignBack"), text::convertNumToString<long>(layout.m_cutterMarkAlignBack));
	}

	// ADD --> #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
	if(0 != layout.m_cutterMark) {

		// �u�㏈���}�[�N�A�����V�[�g�}�[�N�T�C�Y�v�L��(TRUE)/����(FALSE)���o�͂���
		outElement->setValue(layoutPath + _T("/AdjustmentSheetSizeEnabled"), true == layout.m_adjustmentSheetSizeEnabled ? _T("true") : _T("false"));
		if ( layout.m_adjustmentSheetSizeEnabled )
		{	// �u�����}�[�N�A�����V�[�g�}�[�N�T�C�Y�v���L���̏ꍇ�Ɂu�㏈���}�[�N�A�����V�[�g�}�[�N�����v���o�͂���
			outElement->setValue(layoutPath + _T("/AdjustmentSheetLength"), text::convertNumToString<long>(layout.m_adjustmentSheetLength));
		}
		// �u�W���u��̃V�[�g�����w�肷����@��I������I�v�V�����v���o�͂���
		outElement->setValue(layoutPath + _T("/AdjustmentSheetAfterJob"), text::convertNumToString<long>(layout.m_adjustmentSheetAfterJob));
		if ( layout.m_adjustmentSheetAfterJob == SET_SHEET_COUNT )
		{	// �u�W���u��̃V�[�g�����w�肷����@��I������I�v�V�����v���u�V�[�g���ݒ�v�̏ꍇ�Ɂu�㏈���}�[�N�A�W���u��V�[�g���v���o�͂���
			outElement->setValue(layoutPath + _T("/AfterJobSheets"), text::convertNumToString<long>(layout.m_afterJobSheets));
		}
		// ADD <-- #9545: ��������̃C���|�[�g�^�G�N�X�|�[�g�i�����_�C�o�[�g�ݒ�j
	}
}

// JI���̐ݒ�
void ExportFile::SetJI(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// JI�����擾
	SJetInspection jetInspection;
	inPrintCondition.GetJetInspection(jetInspection);

	text::tstring jetInspectionPath = inElementPath + _T("/Inspection");
	// ������������XML�f�[�^�ɐݒ�
	outElement->setValue(jetInspectionPath + _T("/ParamaterName"), jetInspection.m_ParamaterName);
	// �ꎞ�}�X�N�t�@�C���p�X��XML�f�[�^�ɐݒ�
	outElement->setValue(jetInspectionPath + _T("/TemporalMaskFilePath"), jetInspection.m_TemporalMaskFilePath);
}

// JI�G���A���̐ݒ�
void ExportFile::SetJIArea(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	auto setJIArea = [&](bool inIsFront, text::tstring& inJetInspectionAreaFrontPath)
	{
		// JI�G���A�����擾
		SJetInspectionArea jetInspectionArea;
		if(!inPrintCondition.GetJetInspectionArea(jetInspectionArea, inIsFront))
		{
			std::stringstream errorlog;
			errorlog << "ExportFile::SetJIArea failure inIsFront = " << inIsFront;
			WriteToLogBuf(LOG_DEBUG, errorlog.str());
			return;
		}

		// �����ݒ�A�Ώۖʂ�XML�f�[�^�ɐݒ�
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/Side"), jetInspectionArea.m_side);
		// �񌟍��G���A���[
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/NonInspectionAreaLeft"), text::convertNumToTStringPrecision<double>(jetInspectionArea.m_NonInspectionAreaLeft, 2));
		// �񌟍��G���A�E�[
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/NonInspectionAreaRight"), text::convertNumToTStringPrecision<double>(jetInspectionArea.m_NonInspectionAreaRight, 2));

		text::tstring nonInspectionRootPath = inJetInspectionAreaFrontPath + _T("/NonInspectionRect");
		// PartIDKeys��XML�f�[�^�ɐݒ�
		outElement->setValue(nonInspectionRootPath + _T("/PartIDKeys"), _T("ID"));

		for(auto it = jetInspectionArea.m_NonInspectionRect.begin(); it != jetInspectionArea.m_NonInspectionRect.end(); ++it)
		{
			size_t index = std::distance(jetInspectionArea.m_NonInspectionRect.begin(), it);
			text::tstring nonInspectionPath = inJetInspectionAreaFrontPath + _T("/NonInspectionRect/NonInspectionRect<") + text::convertNumToTString<size_t>(index) + _T(">");

			// ID��XML�f�[�^�ɐݒ�
			outElement->setValue(nonInspectionPath + _T("/ID"), text::convertNumToTString<long>(it->m_ID));
			// �񌟍���`�G���A�J�n�ʒu��XML�f�[�^�ɐݒ�
			outElement->setValue(
				nonInspectionPath + _T("/StartPos"), 
				text::convertNumToTStringPrecision<double>(it->m_StartPos.first, 2) + _T(" ") + text::convertNumToTStringPrecision<double>(it->m_StartPos.second, 2));
			// �񌟍���`�G���A�I���ʒu��XML�f�[�^�ɐݒ�
			outElement->setValue(
				nonInspectionPath + _T("/EndPos"), 
				text::convertNumToTStringPrecision<double>(it->m_EndPos.first, 2) + _T(" ") + text::convertNumToTStringPrecision<double>(it->m_EndPos.second, 2));
		}		
	};

	text::tstring jetInspectionAreaRootPath = inElementPath + _T("/InspectionArea");
	// PartIDKeys��XML�f�[�^�ɐݒ�
	outElement->setValue(jetInspectionAreaRootPath + _T("/PartIDKeys"), _T("Side"));

	// �\�ʂ�JI�G���A����ݒ�
	text::tstring jetInspectionAreaFrontPath = inElementPath + _T("/InspectionArea/InspectionArea<")+ text::convertNumToTString<int>(0) + _T(">");
	setJIArea(true, jetInspectionAreaFrontPath);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// ���ʂ�JI�G���A����ݒ�
		text::tstring jetInspectionAreaBackPath = inElementPath + _T("/InspectionArea/InspectionArea<")+ text::convertNumToTString<int>(1) + _T(">");
		setJIArea(false, jetInspectionAreaBackPath);
	}
}

// JI�f�R�[�h���̐ݒ�
void ExportFile::SetJIDecode(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	auto setJIDecodeDetails = [&](SJetInspectionDecode& inJetInspectionDecode, text::tstring& ioJetInspectionDecodePath, long& ioIndex)
	{
		for(auto it = inJetInspectionDecode.m_DetailsList.begin(); it != inJetInspectionDecode.m_DetailsList.end(); ++it)
		{
			text::tstring decodeDetailsPath = ioJetInspectionDecodePath + _T("/DecodeDetails/DecodeDetails<") + text::convertNumToTString<size_t>(ioIndex++) + _T(">");

			// �\�ʁE���ʂ̋�ʂ�XML�f�[�^�ɐݒ�
			outElement->setValue(decodeDetailsPath + _T("/Side"), it->m_Side);
			// �T�C�Y�ݒ��XML�f�[�^�ɐݒ�
			outElement->setValue(
				decodeDetailsPath + _T("/Size"), 
				text::convertNumToTStringPrecision<double>(it->m_Size.first, 1)+ _T(" ") + text::convertNumToTStringPrecision<double>(it->m_Size.second, 1));
			// �ʒu�ݒ��XML�f�[�^�ɐݒ�
			outElement->setValue(
				decodeDetailsPath + _T("/Offset"),
				text::convertNumToTStringPrecision<double>(it->m_Offset.first, 1)+ _T(" ") + text::convertNumToTStringPrecision<double>(it->m_Offset.second, 1));
			// ���x�����̂�XML�f�[�^�ɐݒ�
			outElement->setValue(decodeDetailsPath + _T("/Name"), it->m_Name);
			// ���x����ނ�XML�f�[�^�ɐݒ�
			outElement->setValue(decodeDetailsPath + _T("/Type"), it->m_Type);
			// �iOCR����j�p�x����XML�f�[�^�ɐݒ�
			outElement->setValue(decodeDetailsPath + _T("/Angle"), it->m_Angle);
			// �f�R�[�h�G���[�������A�G���[��~����J�E���g�̑Ώۂɂ��邩��XML�f�[�^�ɐݒ�
			outElement->setValue(decodeDetailsPath + _T("/ErrorStop"), true == it->m_ErrorStop ? _T("true") : _T("false"));
		}
	};

	// JI�f�R�[�h�����擾
	SJetInspectionDecode jetInspectionDecode;
	if(!inPrintCondition.GetJetInspectionDecode(jetInspectionDecode, true))
	{
		std::string errorlog = "ExportFile::SetJIDecode failure Front";
		WriteToLogBuf(LOG_DEBUG, errorlog);
		return;
	}

	long indexDecodeDetails = 0;
	text::tstring jetInspectionDecodePath = inElementPath + _T("/Decode");
	// �u�f�R�[�h�v�^�u�A�G���[��~�A���V�[�g����XML�f�[�^�ɐݒ�
	outElement->setValue(jetInspectionDecodePath + _T("/ErrorSheets"), text::convertNumToTString<long>(jetInspectionDecode.m_ErrorSheets));
	// �u���l�P�ʌn��XML�f�[�^�ɐݒ�
	outElement->setValue(jetInspectionDecodePath + _T("/Unit"), jetInspectionDecode.m_Unit);

	// �f�R�[�h�����̗L���^����
	outElement->setValue(jetInspectionDecodePath + _T("/IsEnabled"), true == jetInspectionDecode.m_IsEnabled ? _T("true") : _T("false"));

	text::tstring decodeDetailsFullPath = jetInspectionDecodePath + _T("/DecodeDetails");
	// PartIDKeys��XML�f�[�^�ɐݒ�
	outElement->setValue(decodeDetailsFullPath + _T("/PartIDKeys"), _T("Name"));

	// �\�ʂ�JI�f�R�[�h�ڍ׏���ݒ�
	setJIDecodeDetails(jetInspectionDecode, jetInspectionDecodePath, indexDecodeDetails);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		SJetInspectionDecode jetInspectionDecodeBack;
		if(!inPrintCondition.GetJetInspectionDecode(jetInspectionDecodeBack, false))
		{
			std::string errorlog = "ExportFile::SetJIDecode failure Back";
			WriteToLogBuf(LOG_DEBUG, errorlog);
			return;
		}

		// ���ʂ�JI�f�R�[�h�ڍ׏���ݒ�
		setJIDecodeDetails(jetInspectionDecodeBack, jetInspectionDecodePath, indexDecodeDetails);
	}
}

// �ϔ{���̐ݒ�
void ExportFile::SetVariableMagnification(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// �ϔ{�����擾
	ST_PDB_RIP_MAGNIFICATION ripMagnification;
	if(!inPrintCondition.GetVariableMagnification(ripMagnification))
	{
		// �ϔ{���擾���s���͏����I��
		throw std::runtime_error("SetVariableMagnification");
	}

	text::tstring scalingPath = inElementPath + _T("/Scaling");

	// �ϔ{�@�\��ON/OFF��XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/ScalingValue"), TRUE == ripMagnification.IS_ENABLE ? _T("true") : _T("false"));

	// ��ʂ�XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/BaseSide"), TRUE == ripMagnification.IS_DATUM_PLANE_FRONT ? _T("Front ") : _T("Back"));

	// �ϔ{�l(�\�E��)��XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/ScalingValueFrontW"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.FRONT_X) / 1000, 3));

	// �ϔ{�l(�\�E�c)��XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/ScalingValueFrontH"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.FRONT_Y) / 1000, 3));

	// �ϔ{�l(���E��)��XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/ScalingValueBackW"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.BACK_X) / 1000, 3));

	// �ϔ{�l(���E�c)��XML�f�[�^�ɐݒ�
	outElement->setValue(scalingPath + _T("/ScalingValueBackH"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.BACK_Y) / 1000, 3));
}

// XML���o�͂���
void ExportFile::outputXML(
	const text::tstring& inOutputPath,
	const std::unique_ptr<xml_utility::XMLElementControl>& inElement)
{
	// �o�͂���XML�f�[�^�𕶎��f�[�^�ɕϊ�����
	text::tstring outXMLData = inElement->getTString();

	// XML�t�@�C�����o�͂��� (UTF-8 Bom����)
	file_utility::OutTextFileStream outTextFileStream(inOutputPath, false, false);
	outTextFileStream.writeLine(outXMLData);
}

// �w��XML�p�X��XY�p�����[�^��ݒ肷��
template <class T>
void ExportFile::setXYpairParameter(
	std::unique_ptr<xml_utility::XMLElementControl>& outElement,
	const text::tstring& inElementPath,
	const T inParamX,
	const T inParamY)
{
	// ������ɕϊ�
	text::tstring paramX = text::convertNumToTString<T>(inParamX);
	text::tstring paramY = text::convertNumToTString<T>(inParamY);
	text::tstring param = paramX + _T(" ") + paramY;

	// XML�f�[�^�ɐݒ�
	outElement->setValue(inElementPath, param);
}

// ���C���t���b�V���O�ݒ�L�����擾����
bool ExportFile::isLineFlushing(DEF_FLUSHING_PATTERN inFlushingPattern)
{
	bool ret = true;

	switch(inFlushingPattern)
	{
	case DEF_FLUSHING_PATTERN_NONE:
	case DEF_FLUSHING_PATTERN_STAR_1:
	case DEF_FLUSHING_PATTERN_STAR_2:
	case DEF_FLUSHING_PATTERN_STAR_4:
	case DEF_FLUSHING_PATTERN_STAR_8:
		ret = false;
		break;
	}

	return ret;
}

}//namespace export_xml