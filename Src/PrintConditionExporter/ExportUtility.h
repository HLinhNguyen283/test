/*! \file ExportUtility.h
 *  \brief ExportUtility.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
*/

#pragma once

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PaperDB_MigrationDef.h"

#include <list>
// ==========================================================================

const std::string st_ExtensionTxt = ".txt";
const std::string st_ExtensionXML = ".xml";

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
 * @brief �˗����X�g�f�[�^�\����
**/
struct SRequestConditionData
{
	std::string				conditionName;		//!< ���������
	ST_PDB_MIGRATION_DATA	migrationData;		//!< �Ɩ��ڍs�p�\����
	bool					isUpdate;			//!< �X�V or �폜�w��
	std::string				localFileName;		//!< �ꎞ�t�@�C����

	SRequestConditionData()
	{
		isUpdate = false;
	}
};

/**
 * ���[�e�B���e�B
**/
class ExportUtility
{
public:

	/**
	 * ������������擾����
	 * @param[in]	outPDBDataList : �˗����X�g�f�[�^�\����
	**/
	static void getPDBGetPrintConditionInfo(std::list<SRequestConditionData>& outPDBDataList);

	/**
	 * �G�N�X�|�[�g��̃��[�g�p�X���L�����`�F�b�N����
	 * @param[in]	inDestRootFolderPath : �G�N�X�|�[�g��̃��[�g�p�X(UNC)
	 * @retval	kReturnCode_Success�F�G���[����(0)
	 * @retval	kReturnCode_NotExist�F�t�H���_�p�X�����݂��Ȃ�(1)
	 * @retval	kReturnCode_NotAccess�F�t�H���_�ɃA�N�Z�X�ł��Ȃ�(2)
	**/
	static EReturnCode isValidExportFolder(const std::string& inDestRootFolderPath);

	/**
	 * @brief	�������u������
	 * @param[in]	inSrcStr : �ϊ��Ώۂ̕�����
	 * @param[in]	inDstStr : �ϊ���̕�����
	 * @param[in,out]	ioStr : ������
	 * @param[in]	inPos : �J�n�ʒu(�ȗ����͐擪����)
	**/
	static void replaceString(const std::string& inSrcStr, const std::string& inDstStr, std::string& ioStr, const size_t inPos = 0);

	/**
	 * �t�@�C�����w��p�X�Ɉړ�����
	 * @param[in]	inDestRootFolderPath : �ړ���̃��[�g�p�X(UNC)
	 * @param[in]	inFileName : �t�@�C����
	 * @param[in]	inExtensionStr : �g���q������
	 * @param[in]	inTempFolderPath : ���ԃt�H���_�p�X
	 * @retval	true�F�t�@�C���ړ�����
	 * @retval	false�F���s
	**/
	static bool MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName, const std::string& inExtensionStr, const std::string& inTempFolderPath);

	/**
	 * �w�肳�ꂽ�t�@�C�����폜����
	 * @param[in]	inFilePath : �폜�Ώۂ̃t�@�C��
	**/
	static void DeleteFiles(const std::string& inFilePath);

	/**
	 * EQ�T�[�o���L�t�H���_���̎w��t�@�C�����폜����
	 * @param[in]	inFolderPath : EQ�T�[�o���L�t�H���_�̃p�X�B
	 * @param[in]	inPrintConditionName : �폜�Ώۂ̈��������
	 * @param[in]	inExtensionStr : �g���q������
	**/
	static bool DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName, const std::string& inExtensionStr);
	
	/**
	 * EQ�T�[�o���L�t�H���_���̃t�@�C����S�č폜����
	 * @param[in]	inFolderPath : EQ�T�[�o���L�t�H���_�̃p�X�B
	**/
	static void DeleteAllFilesEQ(const std::string& inFolderPath);

	/**
	 * @brief	�t�@�C�������݂��邩�̃`�F�b�N
	 * @param[in]	inFolderPath : �t�H���_�p�X
	 * @param[in]	inFileName : �t�@�C����
	 * @param[in]	inExtensionStr : �g���q������
	**/
	static bool IsFileExist(std::string& inFolderPath, std::string& inFileName, const std::string& inExtensionStr);

	/**
	 * @brief	���ԃt�H���_�p�X�̎擾
	 * @param[in]	inTempFolderPath : ���ԃt�H���_�p�X
	 * @retval	���ԃt�H���_�p�X
	**/
	static std::string getLocalFolderPath(const std::string& inTempFolderPath);

	/**
	 * @brief	�ꎞ�t�@�C�����̍쐬
	 * @param[in]	inPrintConditionName : ���������
	 * @retval	�ꎞ�t�@�C����
	**/
	static std::string createLocalFileName(const std::string& inPrintConditionName);

private:
	/**
	 * �R�}���h�v�����v�g�Ŏ��s����B
	 * @param[in]	inCmd : ���s����R�}���h������B
	 * @retval	true�F����
	 * @retval	false�F���s
	**/
	static DWORD runSystemCmd(const std::string& inCmd);

	/**
	 * GetLastError�̕�������擾����B
	 * @retval	GetLastError�̕�����
	**/
	static std::string getLastError();
};
