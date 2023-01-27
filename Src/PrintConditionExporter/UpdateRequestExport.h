/*! \file UpdateRequestExport.h
 *  \brief UpdateRequestExport.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/19 pt.ju-tanaka �V�K�쐬
*/


#pragma once

#include "Common.h"
#include "ThreadSafe.h"
#include "PaperDB_MigrationDef.h"


/**
 * @brief �˗����X�g�f�[�^�\����
**/
typedef struct _SRequestData
{
	std::string				conditionName;		//!< ���������
	ST_PDB_MIGRATION_DATA	migrationData;		//!< �Ɩ��ڍs�p�\����
	bool					isUpdate;			//!< �X�V or �폜�w��
	std::string				localFileName;		//!< �ꎞ�t�@�C����

	_SRequestData()
	{
		isUpdate = false;
	}
}SRequestData;


/**
 * @brief �G�N�X�|�[�g�X�V�v���ɑ΂��鏈�����s��
**/
class UpdateRequestExport
{
public:
	/**
	 * @brief	constructor
	**/
	UpdateRequestExport();

	/**
	 * @brief	destructor
	**/
	virtual ~UpdateRequestExport();

	/**
	 * @brief	�������C�x���g
	**/
	bool onInitialize(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	�G�N�X�|�[�g�X�V�C�x���g
	**/
	void onUpdateExport(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	�V�X�e���̈�������X�V��ʒm����
	 * @param[in]	inPrintConditionName : ���������
	 * @param[in]	inMigrationData : �Ɩ��ڍs�p�\����
	**/
	void onUpdatePrintCondition(
		const std::string& inPrintConditionName, 
		const ST_PDB_MIGRATION_DATA& inMigrationData, 
		ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	�V�X�e���̈�������폜��ʒm����
	 * @param[in]	inPrintConditionName : ���������
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName, ST_THREAD_INFO* inUpdateThread);

private:
	/**
	 * @brief	��������̃G�N�X�|�[�g�˗�
	 * @param[in]	inCondiotionName : ���������
	 * @param[in]	inMigrationData : ��������f�[�^
	 * @param[in]	inIsUpdate : �X�V�w��(true:�X�V false:�폜)
	 * @retval	true: �G���[����
	 * @retval	false: ��荞�݌��p�X���s��
	**/
	void requestPrintCondionExport(
		const std::string& inCondiotionName, 
		const ST_PDB_MIGRATION_DATA& inMigrationData, 
		const bool inIsUpdate);

	/**
	 * @brief	EQ����T�[�o��̋��L�t�H���_�̏�����
	**/
	bool initEQFolder();
	

	/**
	 * @brief	�˗����X�g����d���������������폜����
	 * @param[in,out]	ioRequests : �˗����X�g
	**/
	void deleteDuplicateCondition(std::list<SRequestData>& ioRequests);

	/**
	 * @brief	�������u������
	 * @param[in]	inSrc : �ϊ��Ώۂ̕�����
	 * @param[in]	inDst : �ϊ���̕�����
	 * @param[in,out]	ioLog : ������
	 * @param[in]	inPos : �J�n�ʒu(�ȗ����͐擪����)
	**/
	void replaceString(const std::string& inSrc, const std::string& inDst, std::string& ioLog, const size_t inPos = 0) const;


	CThreadSafe<std::list<SRequestData>> m_Requests;	//!< �˗��ꗗ
	std::string m_PCNameEQ;			// EQ�Ǘ��T�[�o�[��PC��
	bool m_IsDualSimplex;			// DualSimplex���ݒ肪�L�����ǂ���
	BOOL m_IsDED;					// DED�\�����ǂ���
	std::string m_IpAddress;		// ���@��IP�A�h���X
	std::string m_IpAddressPart;	// ����@��IP�A�h���X
	std::string m_OutputRootFolder;	// EQ����T�[�o�[��ɂ���o�͐�̃��[�g�t�H���_
	std::string m_ExportFolder;		// EQ����T�[�o�[��ɂ���o�͐�t�H���_
};