/*! \file Stub_PaperDBIF.h
 *  \brief Stub_PaperDBIF.h�̏���
 *  \author omori
 *  \date 2019/03/04 omori �V�K�쐬
*/


#pragma once
#include "Stub_PaperDBIF.h"
/**
	@brief	stub of jobmanager plugin
*/
class Stub_PaperDBIF
{
private:
	/**
		@brief	constructor
	*/
	Stub_PaperDBIF();
public:
	/**
		@brief	destructor
	*/
	~Stub_PaperDBIF();

	/**
	@brief	JI�����������擾����
	@param[in]	inPrintConditionName : ���������
	@param[out]	outJICondition : JI�������
	@retval	true : ����
	@retval	false : ���s
	*/
	static bool GetJICondition(const char* inPrintConditionName, std::string& outJICondition);

	/**
	@brief	�摜�ۊǂ�ݒ肷��
	@param[in]	inPrintConditionName : ���������
	@param[in]	inIsImageStorage : �摜�ۊǂ̗L������(true:On�Afalse:Off)
	@retval	true : ����
	@retval	false : ���s
	*/
	static bool SetImageStorage(const std::string& inPrintConditionName, const bool inIsImageStorage);


	/**
	@brief	�摜�ۊǂ��擾����
	@param[in]	inPrintConditionName : ���������
	@retval	true : On
	@retval	false : Off
	*/
	static bool IsImageStorage(const std::string& inPrintConditionName);

	/**
	 * @brief	�V�X�e���̈����������ϔ{�p�����[�^���擾����
	 * @param[in]	inPrintConditionName : ���������
	 * @param[out]	outRipMagnificationData : �ϔ{�p�����[�^�̃f�[�^
	 * @return	�����̐���(��DB����擾�ł������ǂ���)��Ԃ�
	 * @retval	true : ����
	 * @retval	false : ���s
	**/
	static bool GetRipMagnification(const std::string& inPrintConditionName, ST_PDB_RIP_MAGNIFICATION& outRipMagnificationData);

	/**
	 * @brief	�V�X�e���̈�������ɕϔ{�p�����[�^��ݒ肷��
	 * @param[in]	inPrintConditionName : ���������
	 * @param[in]	inRipMagnificationData : �ݒ肷��ϔ{�p�����[�^�̃f�[�^
	 * @return	�����̐���(��DB�ɕۑ��ł������ǂ���)��Ԃ�
	 * @retval	true : ����
	 * @retval	false : ���s
	**/
	static bool SetRipMagnification(const std::string& inPrintConditionName, const ST_PDB_RIP_MAGNIFICATION& inRipMagnificationData);

	/**
	 * @brief	��DB���R�~�b�g����
	 * @retval	true : ����
	 * @retval	false : ���s
	**/
	static bool CommitPaperDB();

	/**
		@brief	�V�X�e���̈�������Ƀv���C�}�[��ON/OFF��ݒ肷��
		@param[in]	inPrintConditionName : ���������
		@param[in]	inIsPrimerSetting : �v���C�}�[��ON/OFF�ݒ�(true:ON�Afalse:OFF)
		@retval	true : ����
		@retval	false : ���s
	**/
	static bool SetPrimerStatus(const std::string& inPrintConditionName, const bool& inIsPrimerSetting);

	/**
		@brief	�V�X�e���̈����������v���C�}�[��ON/OFF���擾����
		@param[in]	inPrintConditionName : ���������
		@param[out]	outIsPrimerSetting : �v���C�}�[��ON/OFF�ݒ�(true:ON�Afalse:OFF)
		@retval	true : ����
		@retval	false : ���s
	**/
	static bool GetPrimerStatus(const std::string& inPrintConditionName, bool& outIsPrimerSetting);
};
