/*! \file	ExportXML.h
 *  \brief	ExportXML.h�̏���
 *  \author	pt.ju-tanaka
 *  \date	2019/06/26 pt.ju-tanaka �V�K�쐬
*/
#pragma once

#include "XMLElementControl.h"
#include "PrintConditionExportXML.h"
#include "PrintConditionExportXMLDef.h"

namespace export_xml
{

/**
	@brief	�G�N�X�|�[�g�t�@�C��(XML)
**/
class ExportFile
{
	public:
		/**
			@brief	Constructor
		*/
		ExportFile();

		/**
			@brief	Destructor
		*/
		~ExportFile();

		/**
			@brief	�G�N�X�|�[�g�˗�
			@param[in]	inOutputPath		 : XML�̏o�̓t�@�C���p�X
			@param[in]	inPrintConditionList : XML�o�͗p��������ꗗ
			@retval	true : ����
			@retval	false : ���s
		*/
		bool onRequestExportXML(
			const text::tstring& inOutputPath,
			const export_xml::PrintCondition& inPrintConditionList);

	private:
		/**
			@brief	����������̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetPrintConditionName(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�p����ނ̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetPaperType(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�i���␳���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetAlignment(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	������[�h�̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetPrintMode(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	������x�̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetPrintSpeed(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�p���ݒ���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetPaperSetting(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�������x���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetDryerTemperature(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);
			
		/**
			@brief	ICC�v���Z�b�g���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetICCPreset(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�g�[���J�[�u���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetToneCurve(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�W���u��{�ݒ���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetJobBase(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);
			
		/**
			@brief	���C�A�E�g���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetLayout(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JI���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetJI(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JI�G���A���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetJIArea(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JI�f�R�[�h���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetJIDecode(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	�ϔ{���̐ݒ�
			@param[in]	inPrintCondition : XML�o�͗p�������
			@param[in]	inElementPath	 : XML�G�������g�̃p�X
			@param[out]	outElement		 : �ǉ������XML�f�[�^
		*/
		void SetVariableMagnification(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	XML���o�͂���
			@param[in]	inOutputPath : �o�͐�p�X
			@param[in]	inElement	 : �o�͂���XML�f�[�^
		*/
		void outputXML(
			const text::tstring& inOutputPath,
			const std::unique_ptr<xml_utility::XMLElementControl>& inElement);

		/**
			@brief	�w��XML�p�X��XY�p�����[�^��ݒ肷��
			@param[out]	outElement : �ǉ������XML�f�[�^
			@param[in]	inElementPath : XML�G�������g�̃p�X
			@param[in]	inParamX : �p�����[�^(X)
			@param[in]	inParamY : �p�����[�^(Y)
		*/
		template <class T>
		void setXYpairParameter(
			std::unique_ptr<xml_utility::XMLElementControl>& outElement,
			const text::tstring& inElementPath,
			const T inParamX,
			const T inParamY);

		/**
			@brief	���C���t���b�V���O�ݒ�L�����擾����
			@param[in]	inFlushingPattern : �t���b�V���O�p�^�[��
			@retval	true : ���C���t���b�V���O�ݒ�L��
			@retval	false : ���C���t���b�V���O�ݒ薳��
		*/
		bool isLineFlushing(DEF_FLUSHING_PATTERN inFlushingPattern);
};

}//namespace export_xml