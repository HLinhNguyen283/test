/*! \file ExportFileUpdate.h
 *  \brief ExportFileUpdate.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/20 pt.ju-tanaka �V�K�쐬
*/

#pragma once

#include "Common.h"
#include "PaperDB_MigrationDef.h"

#include <string>

/**
 * �G�N�X�|�[�g��t�H���_�ւ̃A�N�Z�X���ʒ�`
**/
enum EReturnCode 
{
	kReturnCode_Success = 0,	//!< �G���[����
	kReturnCode_NotExists,		//!< �t�H���_�p�X�����݂��Ȃ�
	kReturnCode_NotAccess,		//!< �t�H���_�ɃA�N�Z�X�ł��Ȃ�
};


/**
 * �G�N�X�|�[�g�t�@�C��(�e�L�X�g)
**/
class ExportFileUpdate
{
public:

	/**
	 * �G�N�X�|�[�g��̃��[�g�p�X���L�����`�F�b�N����
	 * @param[in]	inDestRootFolderPath : �G�N�X�|�[�g��̃��[�g�p�X(UNC)
	 * @retval	kReturnCode_Success�F�G���[����(0)
	 * @retval	kReturnCode_NotExist�F�t�H���_�p�X�����݂��Ȃ�(1)
	 * @retval	kReturnCode_NotAccess�F�t�H���_�ɃA�N�Z�X�ł��Ȃ�(2)
	**/
	EReturnCode IsValidExportFolder(const std::string& inDestRootFolderPath);

	/**
	 * �R���X�g���N�^
	**/
	ExportFileUpdate();

	/**
	 * �f�X�g���N�^
	**/
	virtual ~ExportFileUpdate();

	/**
	 * �t�@�C���o�͂���
	 * @param[in]	inPrintConditionName : �t�@�C���o�͑Ώۂ̈��������
	 * @param[in]	inMigrationData : �t�@�C���o�͑Ώۂ̋Ɩ��ڍs�p�\����
	 * @param[in]	inLocalFileName : �o�͂���ꎞ�t�@�C����
	 * @retval	true�F\Client\Temp\PrintConditionExportText�ւ̃t�@�C���o�͐���
	 * @retval	false�F���s
	**/
	bool WriteFile(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData, const std::string& inLocalFileName);

	/**
	 * \Client\Temp\PrintConditionExport�̃t�@�C�����w��p�X�Ɉړ�����
	 * @param[in]	inDestRootFolderPath : �ړ���̃��[�g�p�X(UNC)
	 * @param[in]	inFileName : �t�@�C����
	 * @retval	true�F�t�@�C���ړ�����
	 * @retval	false�F���s
	**/
	bool MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName);

	/**
	 * �w�肳�ꂽ�t�@�C�����폜����
	 * @param[in]	inFilePath : �폜�Ώۂ̃t�@�C��
	**/
	void DeleteFiles(const std::string& inFilePath);

	/**
	 * EQ�T�[�o���L�t�H���_���̎w��t�@�C�����폜����
	 * @param[in]	inFolderPath : EQ�T�[�o���L�t�H���_�̃p�X�B
	 * @param[in]	inPrintConditionName : �폜�Ώۂ̈��������
	**/
	bool DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName);
	
	/**
	 * EQ�T�[�o���L�t�H���_���̃t�@�C����S�č폜����
	 * @param[in]	inFolderPath : EQ�T�[�o���L�t�H���_�̃p�X�B
	**/
	void DeleteAllFilesEQ(const std::string& inFolderPath);

	/**
	 * @brief	�t�@�C�������݂��邩�̃`�F�b�N
	 * @param[in]	inFolderPath : �t�H���_�p�X
	 * @param[in]	inFileName : �t�@�C����
	**/
	bool IsFileExist(std::string& inFolderPath, std::string& inFileName);

	/**
	 * @brief	�ꎞ�t�H���_�p�X�̎擾
	 * @retval	�ꎞ�t�H���_�p�X
	**/
	std::string getLocalFolderPath();

	/**
	 * @brief	�ꎞ�t�@�C�����̍쐬
	 * @param[in]	inPrintConditionName : ���������
	 * @retval	�ꎞ�t�@�C����
	**/
	std::string createLocalFileName(const std::string& inPrintConditionName);

private:
	/**
	 * �R�}���h�v�����v�g�Ŏ��s����B
	 * @param[in]	inCmd : ���s����R�}���h������B
	 * @retval	true�F����
	 * @retval	false�F���s
	**/
	DWORD runSystemCmd(const std::string& inCmd);

	/**
	 * GetLastError�̕�������擾����B
	 * @retval	GetLastError�̕�����
	**/
	std::string getLastError();

};
