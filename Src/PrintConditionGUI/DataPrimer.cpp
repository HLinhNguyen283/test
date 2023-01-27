/**
 * @file	DataPrimer.cpp
 * @brief	data class: �v���C�}�[
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataPrimer.h"
#include "Stub_JobManagerIF.h"
#include "Stub_PaperDBIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

// �R���X�g���N�^
DataPrimer::DataPrimer() : 
	m_SystemSetting(NULL),
	m_DataIF(NULL),
	m_IsPrimer(false),
	m_IsPrePrimer(false)
{
}

// �f�X�g���N�^
DataPrimer::~DataPrimer()
{
}

// ������
bool DataPrimer::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
{
	if((!inSystemSetting) || (!inDataIF))
	{
		return false;
	}

	m_SystemSetting = inSystemSetting;
	m_DataIF = inDataIF;

	return true;
}

// ���������ʕ\��
bool DataPrimer::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// �v���C�}�[��ON/OFF�{�^���̏�����
	initializePrimer(inPrintConditionOrSectionID);

	return true;
}

// �W���u��������ݒ�R�s�[�C�x���g
void DataPrimer::onCopyJobPrintCondition(bool& outIsPrimer)
{
	outIsPrimer = m_IsPrimer;
}

// �v���C�}�[��ON/OFF�{�^�����ύX���ꂽ���ǂ���
bool DataPrimer::IsChanged()
{
	if(m_IsPrePrimer != m_IsPrimer)
	{
		return true;
	}
	
	return false;
}

// ��������̕ۑ�
bool DataPrimer::Save()
{
	// �W���u�̈�������ݒ��ʂ̏ꍇ
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		Stub_JobManagerIF::SetPrimerStatus(m_PrintConditionOrSectionID, m_IsPrimer);
	}
	// �V�X�e���̈�������ݒ��ʂ̏ꍇ
	else
	{
		Stub_PaperDBIF::SetPrimerStatus(m_PrintConditionOrSectionID, m_IsPrimer);
	}

	// �v���C�}�[��ON/OFF�{�^���̏�����
	initializePrimer(m_PrintConditionOrSectionID.c_str());

	return true;
}

// �v���C�}�[��ON/OFF�{�^���̃R���g���[���\����ԍX�V�C�x���g
void DataPrimer::onView_Primer(bool& outIsShow, bool& outIsEnabled)
{
	m_SystemSetting->IsConnectedPrimer() ? outIsShow = true : outIsShow = false;

	bool isTempEnable = true;
	if(true == m_DataIF->GetLockPrintCondtion() || true == m_DataIF->IsDisablePrintCondition())
	{
		// ��������̃��b�N�ݒ肪ON�A���[�U�[����ʂ����[�U�[�����́u��������̕ҏW�v�܂��́u�W���u�̈�������̕ҏW�v��OFF�̏ꍇ
		isTempEnable = false;
	}

	// �W���u�̈�������ݒ��ʂ̏ꍇ
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		bool reservePrintFlg = Stub_JobManagerIF::GetReservePrintMode(m_PrintConditionOrSectionID.c_str(), false);
		ENUM_PRE_RIP_PROGRESS preRIPStatus = DEF_PRE_RIP_PROGRESS_NONE;
		Stub_JobManagerIF::GetPreRIPStatus(m_PrintConditionOrSectionID.c_str(), preRIPStatus, false);
		// ����\��W���u�����ORIP���������Ă��Ȃ��ꍇ
		if(!(reservePrintFlg && !(DEF_PRE_RIP_PROGRESS_COMP == preRIPStatus)) && isTempEnable)
		{
			isTempEnable = true;
		}
		else
		{
			isTempEnable = false;
		}

		outIsEnabled = isTempEnable;
	}
	// �V�X�e���̈�������ݒ��ʂ̏ꍇ
	else
	{
		outIsEnabled = isTempEnable;
	}
}

// ��������ύX��̍X�V
void DataPrimer::UpdatePrintCondition(const char* inPrintConditionOrSection)
{
	// �v���C�}�[��ON/OFF�{�^���̏�����
	initializePrimer(inPrintConditionOrSection);
}

// �v���C�}�[��ON/OFF�{�^���̃{�^�������C�x���g
void DataPrimer::onPush_Primer(bool inState)
{
	m_IsPrimer = inState;
}

// �v���C�}�[��ON/OFF�{�^���̃R���g���[���\�����e�X�V�C�x���g
void DataPrimer::onDraw_Primer(bool& outState)
{
	outState = m_IsPrimer;
}

// �v���C�}�[��ON/OFF�{�^���̏�����
void DataPrimer::initializePrimer(const char* inPrintConditionOrSectionID)
{
	m_PrintConditionOrSectionID = inPrintConditionOrSectionID;
	bool isTempPrimer = false;

	// �W���u�̈�������ݒ��ʂ̏ꍇ
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		if(!Stub_JobManagerIF::GetPrimerStatus(m_PrintConditionOrSectionID, isTempPrimer))
		{
			isTempPrimer = false;
		}
	}
	// �V�X�e���̈�������ݒ��ʂ̏ꍇ
	else
	{
		if(!Stub_PaperDBIF::GetPrimerStatus(m_PrintConditionOrSectionID, isTempPrimer))
		{
			isTempPrimer = false;
		}
	}

	m_IsPrimer = isTempPrimer;
	m_IsPrePrimer = m_IsPrimer;
}