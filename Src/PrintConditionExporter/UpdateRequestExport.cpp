/*! \file UpdateRequestExport.cpp
 *  \brief UpdateRequestExport.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/19 pt.ju-tanaka �V�K�쐬
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestExport.h"
#include "ProcPluginCBRProc.h"
#include "ThreadSafe.h"
#include "UpdateRequestExport.h"
#include "Ini_PrinterDescriptor.h"
#include "DataSystemSetting.h"
#include "ThreadSafeShareLock.h"
#include "Stub_PaperDBIF.h"
#include "ExportFileUpdate.h"

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include <imagehlp.h>
#include "ResDef.h"

#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>
// ==========================================================================

const std::string st_tempEQFolderPath = "#work_ex\\ExportJDPaper_text";


// �R���X�g���N�^
UpdateRequestExport::UpdateRequestExport() : 
	m_Requests(MOD_PRINT_CONDITION_EXPORTER, "CUpdateRequestQueue", 1000)
{
	m_IsDualSimplex= false;
	m_IsDED = false;
}

// �f�X�g���N�^
UpdateRequestExport::~UpdateRequestExport()
{
}

// �������C�x���g
bool UpdateRequestExport::onInitialize(ST_THREAD_INFO* inUpdateThread)
{
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	m_PCNameEQ = iniPrinterDescriptor.GetEqCenterPcName();								// EQ�Ǘ��T�[�o�[��PC��
	m_IsDualSimplex = iniPrinterDescriptor.IsEnableExchangePrinterCompositionOption();	// DualSimplex���ݒ肪�L�����ǂ���
	m_IsDED = iniPrinterDescriptor.IsDedSystem();										// DED�\�����ǂ���
	m_IpAddress = iniPrinterDescriptor.GetClientPcIPAddr(true);							// ���@��IP�A�h���X
	m_IpAddressPart = iniPrinterDescriptor.GetClientPcIPAddr(false);					// ����@��IP�A�h���X

	CDataSystemSetting::GetInstance()->GetOutputRootFolder(m_OutputRootFolder);			// EQ����T�[�o�[��ɂ���o�͐�̃��[�g�t�H���_
	if(m_OutputRootFolder.empty())
	{
		m_OutputRootFolder = st_tempEQFolderPath;
	}

	if( !initEQFolder() )
	{
		// EQ����T�[�o��ւ̋��L�t�H���_�쐬���s�A�܂��̓A�N�Z�X�s��
		return false;
	}

	SRequestData pdbData;
	std::list<SRequestData> pdbDataList;
	{
		NS_SRWLock::CUniqueTransaction<bool> lockObj;
		// �C���|�[�g�E�G�N�X�|�[�g�p�̔r�������i�ǂݏ����֎~�j���J�n����
		CStub_PaperDBIF::PDB_UniqueLock_forImportExport(lockObj);
		CStub_PaperDBIF::PDB_EnterCommonPaperDBCS();		

		std::string name;
		// �S������������擾����
		while(CStub_PaperDBIF::PDB_GetPrintConditionName(name) == true)
		{
			ST_PDB_MIGRATION_DATA migrationData;
			// ��DB����Y�������������f�[�^���擾
			if(!CStub_PaperDBIF::PDB_GetPrintConditionMigration(name, UserModeGUI_GetLengthUnit(), DEF_MODE_RESO_SPEED_M, migrationData))
			{
				// ��DB����̈�������f�[�^�擾���s�����ꍇ�A���̈�������̏������s��
				std::stringstream strLog;
				strLog << "UpdateRequestExport::onInitialize GetPrintConditionMigration failed [ConditionName: " << name << "]";
				WriteToLogBuf(LOG_DEBUG, strLog.str());
				continue;
			}

			//SRequestData�\���̃����o��isUpdate��localFileName��initialize���ɂ͎g�p���Ȃ����߁A�ݒ�s�v�Ƃ���
			pdbData.conditionName = name;
			pdbData.migrationData = migrationData;
			pdbDataList.push_back(pdbData);
		}	

		CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS();
		// �C���|�[�g�E�G�N�X�|�[�g�p�̔r�������̏I���͂��̃X�R�[�v���I���^�C�~���O�ŏI���ƂȂ�
	}

	// ����������ƈ�������f�[�^���X�^�b�N(m_Requests)�ɓo�^����
	for(auto itrPDBList = pdbDataList.begin(); itrPDBList != pdbDataList.end(); ++itrPDBList)
	{
		requestPrintCondionExport(itrPDBList->conditionName, itrPDBList->migrationData, true);
	}

	return true;
}

// �G�N�X�|�[�g�X�V�C�x���g
void UpdateRequestExport::onUpdateExport(ST_THREAD_INFO* inUpdateThread)
{
	ExportFileUpdate exportFileUpdate;
	std::string fileName;

	// ����̈˗����e���|�����̈ꗗ�ɃR�s�[���Am_Requests�ɂ���˗�����ɂ���.���g���C���K�v�Ȃ��̂͏�����ɐ擪�ɒǉ�����
	//  UniqueLock ���Ă�����Ԓ��́A��DB����̍X�V�˗���ǉ��ł��Ȃ��B��DB����̍X�V�˗����ɗ͎~�܂�ʂ悤�G�N�X�|�[�g�̎��������̓��b�N���Ȃ�
	std::list<SRequestData> tmpRequests;
	{
		auto requests = m_Requests.UniqueLock();
		std::copy(requests->begin(), requests->end(), std::back_inserter(tmpRequests));
		requests->clear();
	}

	// �d���������������폜
	deleteDuplicateCondition(tmpRequests);

	auto itrRequest = tmpRequests.begin();
	while( itrRequest!= tmpRequests.end() )
	{
		// �X�V�X���b�h���I�����Ă��邩�`�F�b�N
		if(inUpdateThread->thread_handle == nullptr)
		{
			// �X�V�X���b�h���I�����Ă���ꍇ�A�����I��
			return;
		}

		// �˗����X�g�Ɉ˗����L��ꍇ
		fileName.clear();
		fileName = itrRequest->conditionName;
		if(itrRequest->isUpdate == true)
		{
			// �X�V�w��̏ꍇ
			std::string tmpFolderPath = exportFileUpdate.getLocalFolderPath();
			if( exportFileUpdate.IsFileExist(tmpFolderPath, itrRequest->localFileName) )
			{
				// ���ԃt�H���_�ɊY���t�@�C��������ꍇ�AEQ����T�[�o��̋��L�t�H���_�Ƀe�L�X�g�t�@�C�����ړ�����
				if( exportFileUpdate.MoveFiles(m_ExportFolder, itrRequest->localFileName) )
				{
					// EQ����T�[�o��̋��L�t�H���_�Ƀe�L�X�g�t�@�C�����ړ��ł����ꍇ�A
					// m_Requests����{�˗����폜�ł���
					itrRequest = tmpRequests.erase(itrRequest);
					continue;
				}
			}
			else
			{
				// ���ԃt�H���_�ɊY���t�@�C���������ꍇ�A�e�L�X�g�t�@�C���𒆊ԃt�H���_�ɍ쐬����
				if( exportFileUpdate.WriteFile(fileName, itrRequest->migrationData, itrRequest->localFileName) )
				{
					// EQ����T�[�o��̋��L�t�H���_�Ƀe�L�X�g�t�@�C�����ړ�����
					if( exportFileUpdate.MoveFiles(m_ExportFolder, itrRequest->localFileName) )
					{
						// �e�L�X�g�t�@�C�������ԃt�@�C���ɍ쐬�ł��A����EQ����T�[�o��̋��L�t�H���_�Ƀe�L�X�g�t�@�C�����ړ��ł����ꍇ�A
						// m_Requests����{�˗����폜�ł���
						itrRequest = tmpRequests.erase(itrRequest);
						continue;
					}
				}
			}

			// �e�L�X�g�t�@�C�������ԃt�@�C���ɍ쐬�ł��Ȃ����� or EQ����T�[�o��̋��L�t�H���_�Ƀt�@�C�����ړ��ł��Ȃ������ꍇ�A
			// ���g���C�ɂčēx�����������Ȃ����߁Am_Requests����{�˗����폜���Ă͂Ȃ�Ȃ�
			++itrRequest;
		}
		else
		{
			// �폜�w��̏ꍇ�AEQ����T�[�o��̋��L�t�H���_�ɂ���e�L�X�g�t�@�C�����폜����
			if( exportFileUpdate.DeleteFileEQ(m_ExportFolder, fileName) )
			{
					// EQ����T�[�o��̋��L�t�H���_�̃t�@�C���폜����Ă���ꍇ�Am_Requests����{�˗����폜����
					itrRequest = tmpRequests.erase(itrRequest);
					continue;
			}

			// EQ����T�[�o��̋��L�t�H���_�̃t�@�C�����폜����Ă��Ȃ��ꍇ�A
			// ���g���C�ɂčēx�����������Ȃ����߁Am_Requests����{�˗����폜���Ȃ�
			++itrRequest;
		}
	}

	//�e���|�����̈ꗗ�Ɏc�������̂��Am_Requests�̐擪�ɒǉ����āA���g���C�\�Ȃ悤�ɂ���
	{
		auto requests = m_Requests.UniqueLock();
		requests->insert(requests->begin(), tmpRequests.begin(), tmpRequests.end());
	}
}

// �V�X�e���̈�������X�V��ʒm����
void UpdateRequestExport::onUpdatePrintCondition(
	const std::string& inPrintConditionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData, 
	ST_THREAD_INFO* inUpdateThread)
{
	requestPrintCondionExport(inPrintConditionName, inMigrationData, true);
}

// �V�X�e���̈�������폜��ʒm����
void UpdateRequestExport::onDeletePrintCondition(const std::string& inPrintConditionName, ST_THREAD_INFO* inUpdateThread)
{
	// ST_PDB_MIGRATION_DATA�Ɨp����ނ̏��͋�œo�^����
	ST_PDB_MIGRATION_DATA dummyMigrationData;
	requestPrintCondionExport(inPrintConditionName, dummyMigrationData, false);
}

// ����������G�N�X�|�[�g�˗��ɓo�^����
void UpdateRequestExport::requestPrintCondionExport(
	const std::string& inCondiotionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData, 
	const bool inIsUpdate)
{
	ExportFileUpdate exportFileUpdate;
	EReturnCode returnCode;
	//�����o����p�X���L�����`�F�b�N����
	returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder);
	if(returnCode == kReturnCode_Success)
	{
		auto requests = m_Requests.UniqueLock();

		std::string localFileName = exportFileUpdate.createLocalFileName(inCondiotionName);

		//�G�N�X�|�[�g�˗������X�g�ɓo�^����
		SRequestData requestData;
		requestData.conditionName = inCondiotionName;
		requestData.migrationData = inMigrationData;
		requestData.isUpdate = inIsUpdate;
		requestData.localFileName = localFileName;
		requests->push_back(requestData);
	}
}

// EQ����T�[�o��̋��L�t�H���_��������
bool UpdateRequestExport::initEQFolder()
{
	m_ExportFolder += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\";

	std::string formMsg;
	long resourceID = 0;
	ExportFileUpdate exportFileUpdate;

	try
	{
		// EQ�T�[�o�[��̋��L�t�H���_�ɃA�N�Z�X�ł��邩���`�F�b�N("\\<EQ��PC��IP�A�h���X>\#work_exExportJDPaper_text\")
		EReturnCode returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder.c_str());
		if(returnCode == kReturnCode_NotAccess)
		{
			// ���L�t�H���_�ɃA�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK;
			throw 1;
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// EQ�T�[�o�[��̋��L�t�H���_�p�X����Ō��"\"���폜����
			int pos = m_ExportFolder.find_last_of("\\");
			std::string tmpExportFolder = m_ExportFolder.substr(0, pos);
			char* tmpRootFolder = const_cast<char*>(tmpExportFolder.c_str());
			// EQ�T�[�o�[���root�t�H���_�p�X�̎擾("\\<EQ��PC��IP�A�h���X>\#work_ex")
			PathRemoveFileSpec(tmpRootFolder);

			// EQ�T�[�o�[���Root�t�H���_�ɃA�N�Z�X�ł��邩���`�F�b�N
			std::string rootFolder = tmpRootFolder;
			returnCode = exportFileUpdate.IsValidExportFolder(rootFolder);
			if(returnCode != kReturnCode_Success)
			{
				// EQ�T�[�o�[���Root�t�H���_�����݂��Ȃ� or �A�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
				resourceID = IDM_ERR_NETWORK;
				throw 1;
			}

			// ���L�t�H���_�����݂��Ȃ��ꍇ�A���L�t�H���_���쐬
			if( MakeSureDirectoryPathExists(m_ExportFolder.c_str()) != TRUE )
			{
				// �t�H���_�쐬�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			// EQ�T�[�o�[��̋��L�t�H���_�ɃA�N�Z�X�ł���ꍇ�͏����Ȃ�	
		}

		if(m_IpAddress.empty())
		{
			// ���@��IP�A�h���X���L�ڂ���Ă��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
			replaceString("%s", m_ExportFolder, formMsg);
			resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
			throw 1;
		}

		std::string ipAddressSelf;
		if( CDataSystemSetting::GetInstance()->IsEnableAutoUpdateFixIP() )
		{
			if( m_IsDED )
			{
				ipAddressSelf = "192.168.0.30";
			}
			else
			{
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			ipAddressSelf = m_IpAddress;
		}

		if(m_IsDED)
		{
			// DED�\�����̃t�@�C���p�X
			m_ExportFolder += ipAddressSelf + "-DED"+ "\\";
		}
		else
		{
			// SES�\�����̃t�@�C���p�X
			m_ExportFolder += ipAddressSelf + "-SES"+ "\\";
		}

		// �w��t�H���_�̑��݂��`�F�b�N
		returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder.c_str());
		if(returnCode == kReturnCode_Success)
		{
			// �t�H���_�����݂���ꍇ�A�t�H���_���̃t�@�C�����폜
			exportFileUpdate.DeleteAllFilesEQ(m_ExportFolder);
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// �t�H���_�����݂��Ȃ��ꍇ�A�t�H���_���쐬
			if( MakeSureDirectoryPathExists(m_ExportFolder.c_str()) != TRUE )
			{
				// �t�H���_�쐬�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			// �t�H���_�ɃA�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK;
			throw 1;
		}

		// DualSimplex���ݒ�L����DED�\���̏ꍇ
		if( (m_IsDualSimplex) && (m_IsDED) )
		{
			std::string filePathSelf;
			filePathSelf += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\" + m_IpAddress + "-SES"+ "\\";
			// SES�t�H���_(���@)���̑S�t�@�C�����폜
			exportFileUpdate.DeleteAllFilesEQ(filePathSelf);

			std::string filePathPart;
			filePathPart += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\" + m_IpAddressPart + "-SES"+ "\\";
			// SES�t�H���_���̑S�t�@�C�����폜
			exportFileUpdate.DeleteAllFilesEQ(filePathPart);
		}
	}
	catch(int)
	{
		// �G���[���b�Z�[�W��\������
		ShowMessageBox(ERR_PRINT_CONDITION_EXPORTER, resourceID, formMsg, MBST_OK | MBST_ICONERROR | MBST_MODELESS, NULL);
		WriteToLogBuf(LOG_STATUS, formMsg);
		return false;
	}

	return true;
}

// �˗����X�g����d���������������폜����
void UpdateRequestExport::deleteDuplicateCondition(std::list<SRequestData>& ioRequests)
{
	ExportFileUpdate exportFileUpdate;
	auto itrRequest = ioRequests.begin();
	std::list<SRequestData> collectiedList;

	while( itrRequest != ioRequests.end() )
	{
		auto itrCollected = collectiedList.begin();
		while( itrCollected != collectiedList.end() )
		{
			// �d���폜�ς݈˗����X�g�ɏd�������������������邩�`�F�b�N
			if( itrRequest->conditionName.compare(itrCollected->conditionName) == 0 )
			{
				std::string tmpFolderPath = exportFileUpdate.getLocalFolderPath();
				// ���ԃt�H���_�ɏd�������������̈ꎞ�t�@�C�������邩�`�F�b�N
				if( exportFileUpdate.IsFileExist(tmpFolderPath, itrCollected->localFileName) )
				{
					// �d�������������̈ꎞ�t�@�C�����폜
					std::string tmpFilePath = tmpFolderPath + itrCollected->localFileName + ".txt";
					exportFileUpdate.DeleteFiles(tmpFilePath);
				}

				// �˗����X�g����d���������������폜����
				itrCollected = collectiedList.erase(itrCollected);
				continue;
			}

			// ������������d�����Ă��Ȃ���Ύ��̈��������
			++itrCollected;
		}

		// �d���폜�ς݈˗����X�g�Ɉ��������o�^����
		SRequestData collectiedData;
		collectiedData.conditionName = itrRequest->conditionName;
		collectiedData.migrationData = itrRequest->migrationData;
		collectiedData.isUpdate = itrRequest->isUpdate;
		collectiedData.localFileName = itrRequest->localFileName;
		collectiedList.push_back(collectiedData);
		++itrRequest;
	}

	ioRequests.clear();
	// �d���폜�ς݈˗����X�g�̓��e���˗����X�g�ɐݒ�
	for(auto itr = collectiedList.begin(); itr != collectiedList.end(); ++itr) 
	{
		SRequestData requestData;
		requestData.conditionName = itr->conditionName;
		requestData.migrationData = itr->migrationData;
		requestData.isUpdate = itr->isUpdate;
		requestData.localFileName = itr->localFileName;
		ioRequests.push_back(requestData);
	}
}

// �������u������
void UpdateRequestExport::replaceString(const std::string& inSrc, const std::string& inDst, std::string& ioLog, const size_t inPos) const
{
	size_t pos = ioLog.find(inSrc, inPos);
	if (pos != std::string::npos)
	{
		ioLog.replace(pos, 2, inDst);
	}
}
