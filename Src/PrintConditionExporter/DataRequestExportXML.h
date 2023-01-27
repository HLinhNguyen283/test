/*! \file DataRequestExportXML.h
 *  \brief DataRequestExportXML.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
*/

#pragma once

// ==========================================================================
// Include files
#include "Common.h"
#include "ThreadSafe.h"
#include "PaperDB_MigrationDef.h"
#include "ExportUtility.h"
#include <list>

// ==========================================================================

/*** @brief XML�G�N�X�|�[�g�X�V�v���ɑ΂��鏈�����s��
**/
class DataRequestExportXML
{
public:
	/**
	 * @brief	constructor
	**/
	DataRequestExportXML();

	/**
	 * @brief	destructor
	**/
	virtual ~DataRequestExportXML();

	/**
	 * @brief	�������C�x���g
	**/
	bool onInitialize();

	/**
	 * @brief	�G�N�X�|�[�g�X�V�C�x���g
	**/
	void onUpdateXMLExport(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	�V�X�e���̈�������X�V��ʒm����
	 * @param[in]	inPrintConditionName : ���������
	 * @param[in]	inPaperType : �p�����
	 * @param[in]	inMigrationData : �Ɩ��ڍs�p�\����
	**/
	void onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData);

	/**
	 * @brief	�V�X�e���̈�������폜��ʒm����
	 * @param[in]	inPrintConditionName : ���������
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName);

	/**
	 * @brief	JI�A�����֘A�t�H���_�̃~���[�����O�R�s�[
	 * @param[in]	JI�����֘A�t�H���_�iClient�t�H���_����̑��΃p�X�j
	**/
	bool MirroringInspectionData(const std::string& inJIDataFolder);

private:
	/**
	 * @brief	EQ����T�[�o��̋��L�t�H���_�̏�����
	**/
	bool initEQFolder();

	/**
	 * @brief	�˗����X�g����d���������������폜����
	 * @param[in,out]	ioRequests : �˗����X�g
	**/
	void deleteDuplicateCondition(std::list<SRequestConditionData>& ioRequests);

	/**
	 * @brief	��������̃G�N�X�|�[�g�˗�
	 * @param[in]	inRequestConditionData : �˗����X�g�f�[�^�\����
	 * @retval	true: �G���[����
	 * @retval	false: ��荞�݌��p�X���s��
	**/
	void requestXMLPrintCondionExport(SRequestConditionData& inRequestConditionData);

	DWORD runSystemCmd(const std::string& inCmd);
	bool mirrorFolder(const std::string& inSrcPath, const std::string& inDestPath);
	std::string getLastError();
	
	CThreadSafe<std::list<SRequestConditionData>> m_RequestsXML;	//!< �˗��ꗗ
};