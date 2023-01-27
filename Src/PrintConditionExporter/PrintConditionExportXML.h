/*! \file PrintConditionExportXML.h
 *  \brief PrintConditionExportXML.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
*/
#pragma once

// ==========================================================================
// Include files

#include "CommonConvertUnit.h"
#include "PaperDB_MigrationDef.h"
#include "PrintConditionExportXMLDef.h"
#include "Ini_PrinterParameter.h"
// ==========================================================================

namespace export_xml
{

//! ����������
struct SPrintConditionData
{
	std::string printConditionName;							//!< ���������
	ST_PDB_MIGRATION_DATA migrationData;					//!< �Ɩ��ڍs�p�\����
	std::map<std::string, std::string> printModeDataList;	//!< ������[�h�̈ꗗ <������[�h��, EQUIOS�ł̈�����[�h��>
};

/**
	@brief	XML�o�͗p�������
*/
class PrintCondition
{
	private:
		
		SPrintConditionData m_PrintConditionData;	//!< ����������

		/**
			@brief	constructor(�� �g�p�֎~)
		*/
		PrintCondition();

	public:

		/**
			@brief	constructor
			@param[in]	inPrintConditionData : ��������f�[�^
		*/
		PrintCondition(const SPrintConditionData& inPrintConditionData);

		/**
			@brief	destructor
		*/
		~PrintCondition();
			
		/**
			@brief	����������̎擾
			@retval	���������
		*/
		std::string GetPrintConditionName() const;

		/**
			@brief	�p����ނ̎擾
			@retval	�p�����
		*/
		std::string GetPaperType() const;

		/**
			@brief	�i���␳���̎擾
			@retval	�i���␳��
		*/
		std::string GetLevelTuneName() const;

		/**
			@brief	�p���ݒ���̎擾
			@param[out]	outPaperSetting : �p���ݒ���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetPaperSetting(SPaperSetting& outPaperSetting) const;

		/**
			@brief	������[�h�̎擾
			@param[out]	outPrintMode : ������[�h
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetPrintMode(std::string& outPrintMode) const;

		/**
			@brief	������x�̎擾
			@retval	������x
		*/
		double GetPrintSpeed() const;

		/**
			@brief	�������x���̎擾
			@param[out]	outDryerTemperture : �������x���
		*/
		void GetDryerTemperature(SDryerTemperture& outDryerTemperture) const;

			
		/**
			@brief	ICC�v���Z�b�g���̎擾
			@param[out]	outICCPreset : ICC�v���Z�b�g���
		*/
		void GetICCPreset(SICCPreset& outICCPreset) const;


		/**
			@brief	�W���u��{�ݒ���̎擾
			@param[out]	outJobBase : �W���u��{�ݒ���
		*/
		void GetJobBase(SJobBase& outJobBase) const;
			
		/**
			@brief	���C�A�E�g���̎擾
			@param[out]	outLayout : ���C�A�E�g���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetLayout(SLayout& outLayout) const;

		/**
			@brief	�g�[���J�[�u���̎擾
			@param[out]	outToneCurve :�g�[���J�[�u���
			@param[in]	inIsFront : �\�ʂ��ǂ���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetToneCurve(SToneCurve& outToneCurve, bool inIsFront) const;

		/**
			@brief	JetInspection���̎擾
			@param[out]	outJetInspection : JetInspection���
		*/
		void GetJetInspection(SJetInspection& outJetInspection) const;

		/**
			@brief	JetInspection�G���A���̎擾
			@param[out]	outJetInspectionArea : JetInspection�G���A���
			@param[in]	inIsFront : �\�ʂ��ǂ���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetJetInspectionArea(SJetInspectionArea& outJetInspectionArea, bool inIsFront) const;

		/**
			@brief	JetInspection�f�R�[�h���̎擾
			@param[out]	outJetInspectionDecode : JetInspection�f�R�[�h���
			@param[in]	inIsFront : �\�ʂ��ǂ���
			@param[in]	inIsFront : �\�ʂ��ǂ���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetJetInspectionDecode(SJetInspectionDecode& outJetInspectionDecode, bool inIsFront) const;

		/**
			@brief	�ϔ{���̎擾
			@param[out]	outVariableMagnification : �ϔ{���
			@retval	true : ����
			@retval	false : ���s
		*/
		bool GetVariableMagnification(ST_PDB_RIP_MAGNIFICATION& outVariableMagnification) const;

	private:
		/**
			@brief	�G���[���e�̏�������
			@param[in]	inError : �G���\���
		*/
		void writeErrorMessage(const std::string& inError) const;

		/**
			@brief	����ʂ̎擾
			@param[in]	inIsDED : DED���ǂ���
			@param[in]	inIsFront : �\�ʂ��ǂ���
			@param[out]	outSide : �����
			@param[out]	outIsPart : ���@or����@(0:���@�A1:����@)
		*/
		void getSide(const bool inIsDED, const bool inIsFront, std::string& outSide, long& outIsPart) const;
};
}//namespace export_xml