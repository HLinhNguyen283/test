/**
 * @file	DataPrimer.h
 * @brief	data class: �v���C�}�[
**/

#pragma once

// ==========================================================================
// Include files
#include "DataIF.h"
#include "DataSystemSetting.h"

// ==========================================================================

/**
 * @brief	data class: �v���C�}�[
**/
class DataPrimer
{
public:
	//�֐�
	/**
		@brief	�R���X�g���N�^
	**/
	DataPrimer();

	/**
		@brief	�f�X�g���N�^
	**/
	virtual ~DataPrimer();

	/**
		@brief	������
		@param[in]	inSystemSetting : CDataSystemSetting�N���X�C���X�^���X
		@param[in]	inDataIF : CDataIF�N���X�C���X�^���X
		@retval	true : ����
		@retval	false : ���s
	*/
	bool Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF);

	/**
		@brief	���������ʕ\��
		@param[in]	inPrintConditionOrSectionID : ���������or�Z�N�V����ID
		@retval	true : ����
		@retval	false : ���s
	*/
	bool EnterScreen(const char* inPrintConditionOrSectionID);

	/**
		@brief	�W���u��������ݒ�R�s�[�C�x���g
		@param[out]	outIsPrimer : �v���C�}�[
	*/
	void onCopyJobPrintCondition(bool& outIsPrimer);

	/**
		@brief	�v���C�}�[��ON/OFF�{�^�����ύX���ꂽ���ǂ���
		@retval	true : �ύX�L��
		@retval	false : �ύX����
	*/
	bool IsChanged();

	/**
		@brief	��������̕ۑ�
		@retval	true : ����
		@retval	false : ���s
	*/
	bool Save();

	/**
		@brief	�v���C�}�[��ON/OFF�{�^���̃R���g���[���\����ԍX�V�C�x���g
		@param[out]	outIsShow : �v���C�}�[��ON/OFF�{�^���̕\�����
		@param[out]	outIsEnabled : �v���C�}�[��ON/OFF�{�^���̗L�����
	*/
	void onView_Primer(bool& outIsShow, bool& outIsEnabled);

	/**
		@brief	��������ύX��̍X�V
		@param[in]	inPrintConditionOrSection : �v���C�}�[��ON/OFF�{�^���̕\�����
	*/
	void UpdatePrintCondition(const char* inPrintConditionOrSection);

	/**
		@brief	�v���C�}�[��ON/OFF�{�^���̃{�^�������C�x���g
		@param[in]	inState : �v���C�}�[��ON/OFF�{�^���̏��
	*/
	void onPush_Primer(bool inState);

	/**
		@brief	�v���C�}�[��ON/OFF�{�^���̃R���g���[���\�����e�X�V�C�x���g
		@param[in]	outState : �v���C�}�[��ON/OFF�{�^���̏��
	*/
	void onDraw_Primer(bool& outState);

private:
	//�֐�
	/**
		@brief	�v���C�}�[��ON/OFF�{�^���̏�����
		@param[in]	inPrintConditionOrSectionID : ���������or�Z�N�V����ID
	*/
	void initializePrimer(const char* inPrintConditionOrSectionID);

	//�ϐ�
	CDataSystemSetting* m_SystemSetting;	//!< CDataSystemSetting�N���X
	CDataIF* m_DataIF;						//!< CDataIF�N���X
	string m_PrintConditionOrSectionID;		//!< ���������or�Z�N�V����ID

	bool m_IsPrimer;						//!< �v���C�}�[��ON/OFF(true:ON, false:OFF)
	bool m_IsPrePrimer;						//!< �����ݒ莞�̃v���C�}�[��ON/OFF(true:ON, false:OFF)
};
