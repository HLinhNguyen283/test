/*! \file DataRequestExportXML.cpp
 *  \brief DataRequestExportXML.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka �V�K�쐬
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestExportXML.h"
#include "ProcPluginCBRProc.h"
#include "ThreadSafe.h"
#include "DataSystemSetting.h"
#include "ThreadSafeShareLock.h"
#include "ResDef.h"
#include "PrintConditionExportXML.h"
#include "ExportXML.h"
#include "Utility.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <imagehlp.h>

// ==========================================================================
// Local Definition
const std::string st_TempFolderPath = "Temp\\PrintConditionExportXml";
const std::string st_EQFolderXMLPath = "#work_ex\\ExportJDPaper_xml";
std::string st_ExportFolderPath = "";

// ==========================================================================

// �R���X�g���N�^
DataRequestExportXML::DataRequestExportXML() : 
	m_RequestsXML(MOD_PRINT_CONDITION_EXPORTER, "CRequestQueueXML", 1000)
{
}

// �f�X�g���N�^
DataRequestExportXML::~DataRequestExportXML()
{
}

// �������C�x���g
bool DataRequestExportXML::onInitialize()
{
	if( !initEQFolder() )
	{
		// EQ����T�[�o��ւ̋��L�t�H���_�쐬���s�A�܂��̓A�N�Z�X�s��
		return false;
	}

	std::list<SRequestConditionData> pdbDataList;
	// ��DB���������������擾����
	ExportUtility::getPDBGetPrintConditionInfo(pdbDataList);

	// ����������ƈ�������f�[�^���X�^�b�N(m_RequestsXML)�ɓo�^����
	for(auto itrPDBList = pdbDataList.begin(); itrPDBList != pdbDataList.end(); ++itrPDBList)
	{
		SRequestConditionData requestConditionData;
		requestConditionData.conditionName = itrPDBList->conditionName;
		requestConditionData.migrationData = itrPDBList->migrationData;
		requestConditionData.isUpdate = true;
		requestXMLPrintCondionExport(requestConditionData);
	}

	std::string outputFolder = GetModuleDirectory() + st_TempFolderPath;
	// ���ԃt�H���_�̍쐬
	if(FALSE == CreateDirectory(outputFolder.c_str(), NULL))
	{
		return false;
	}

	return true;
}

// �G�N�X�|�[�g�X�V�C�x���g
void DataRequestExportXML::onUpdateXMLExport(ST_THREAD_INFO* inUpdateThread)
{
	std::string fileName;

	// ����̈˗����e���|�����̈ꗗ�ɃR�s�[���Am_RequestsXML�ɂ���˗�����ɂ���.���g���C���K�v�Ȃ��̂͏�����ɐ擪�ɒǉ�����
	//  UniqueLock���Ă�����Ԓ��́A��DB����̍X�V�˗���ǉ��ł��Ȃ��B��DB����̍X�V�˗����ɗ͎~�܂�ʂ悤�G�N�X�|�[�g�̎��������̓��b�N���Ȃ�
	std::list<SRequestConditionData> tmpRequestsList;
	{
		auto requests = m_RequestsXML.UniqueLock();
		std::copy(requests->begin(), requests->end(), std::back_inserter(tmpRequestsList));
		requests->clear();
	}

	// �d���������������폜
	deleteDuplicateCondition(tmpRequestsList);

	auto itrRequest = tmpRequestsList.begin();
	while( itrRequest!= tmpRequestsList.end() )
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
			std::string tmpFolderPath = ExportUtility::getLocalFolderPath(st_TempFolderPath);
			if( ExportUtility::IsFileExist(tmpFolderPath, itrRequest->localFileName, st_ExtensionXML) )
			{
				// ���ԃt�H���_�ɊY���t�@�C��������ꍇ�AEQ����T�[�o��̋��L�t�H���_��XML�t�@�C�����ړ�����
				if( ExportUtility::MoveFiles(st_ExportFolderPath, itrRequest->localFileName, st_ExtensionXML, st_TempFolderPath) )
				{
					// EQ����T�[�o��̋��L�t�H���_��XML�t�@�C�����ړ��ł����ꍇ�A
					// m_RequestsXML����{�˗����폜�ł���
					itrRequest = tmpRequestsList.erase(itrRequest);
					continue;
				}
			}
			else
			{
				export_xml::SPrintConditionData printConditionData;
				printConditionData.printConditionName = fileName;
				printConditionData.migrationData = itrRequest->migrationData;
				CDataSystemSetting::GetInstance()->onExportXML(printConditionData.printModeDataList);

				export_xml::PrintCondition printCondition(printConditionData);
				std::string outputXMLPath = GetModuleDirectory() + st_TempFolderPath + "\\" + itrRequest->localFileName + st_ExtensionXML;
				export_xml::ExportFile exportFile;

				// ���ԃt�H���_�ɊY���t�@�C���������ꍇ�AXML�t�@�C���𒆊ԃt�H���_�ɍ쐬����
				if(exportFile.onRequestExportXML(outputXMLPath, printConditionData))
				{
					// EQ����T�[�o��̋��L�t�H���_��XML�t�@�C�����ړ�����
					if( ExportUtility::MoveFiles(st_ExportFolderPath, itrRequest->localFileName, st_ExtensionXML, st_TempFolderPath) )
					{
						// XML�t�@�C�������ԃt�@�C���ɍ쐬�ł��A����EQ����T�[�o��̋��L�t�H���_��XML�t�@�C�����ړ��ł����ꍇ�A
						// m_RequestsXML����{�˗����폜�ł���
						itrRequest = tmpRequestsList.erase(itrRequest);
						continue;
					}
				}
			}

			// XML�t�@�C�������ԃt�@�C���ɍ쐬�ł��Ȃ����� or EQ����T�[�o��̋��L�t�H���_�Ƀt�@�C�����ړ��ł��Ȃ������ꍇ�A
			// ���g���C�ɂčēx�����������Ȃ����߁Am_RequestsXML����{�˗����폜���Ă͂Ȃ�Ȃ�
			++itrRequest;
		}
		else
		{
			// �폜�w��̏ꍇ�AEQ����T�[�o��̋��L�t�H���_�ɂ���XML�t�@�C�����폜����
			if( ExportUtility::DeleteFileEQ(st_ExportFolderPath, fileName, st_ExtensionXML) )
			{
					// EQ����T�[�o��̋��L�t�H���_�̃t�@�C���폜����Ă���ꍇ�Am_RequestsXML����{�˗����폜����
					itrRequest = tmpRequestsList.erase(itrRequest);
					continue;
			}

			// EQ����T�[�o��̋��L�t�H���_�̃t�@�C�����폜����Ă��Ȃ��ꍇ�A
			// ���g���C�ɂčēx�����������Ȃ����߁Am_RequestsXML����{�˗����폜���Ȃ�
			++itrRequest;
		}
	}

	//�e���|�����̈ꗗ�Ɏc�������̂��Am_RequestsXML�̐擪�ɒǉ����āA���g���C�\�Ȃ悤�ɂ���
	{
		auto requests = m_RequestsXML.UniqueLock();
		requests->insert(requests->begin(), tmpRequestsList.begin(), tmpRequestsList.end());
	}
}

// �V�X�e���̈�������X�V��ʒm����
void DataRequestExportXML::onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	SRequestConditionData requestConditionData;
	requestConditionData.conditionName = inPrintConditionName;
	requestConditionData.migrationData = inMigrationData;
	requestConditionData.isUpdate = true;

	requestXMLPrintCondionExport(requestConditionData);
}

// �V�X�e���̈�������폜��ʒm����
void DataRequestExportXML::onDeletePrintCondition(const std::string& inPrintConditionName)
{
	ST_PDB_MIGRATION_DATA dummyMigrationData;
	SRequestConditionData requestConditionData;
	requestConditionData.conditionName = inPrintConditionName;
	// ��������폜�̏ꍇST_PDB_MIGRATION_DATA�̏��͋�œo�^����
	requestConditionData.migrationData = dummyMigrationData;
	requestConditionData.isUpdate = false;

	requestXMLPrintCondionExport(requestConditionData);
}

// ��������̃G�N�X�|�[�g�˗�
void DataRequestExportXML::requestXMLPrintCondionExport(SRequestConditionData& inRequestConditionData)
{
	//�����o����p�X���L�����`�F�b�N����
	EReturnCode returnCode = ExportUtility::isValidExportFolder(st_ExportFolderPath);
	if(returnCode == kReturnCode_Success)
	{
		auto requests = m_RequestsXML.UniqueLock();

		inRequestConditionData.localFileName = ExportUtility::createLocalFileName(inRequestConditionData.conditionName);

		//�G�N�X�|�[�g�˗������X�g�ɓo�^����
		requests->push_back(inRequestConditionData);
	}
}

// EQ����T�[�o��̋��L�t�H���_�̏�����
bool DataRequestExportXML::initEQFolder()
{
	std::string pcNameEQ;
	CDataSystemSetting::GetInstance()->GetEqCenterPcName(pcNameEQ);
	std::string outputRootFolderXML;
	CDataSystemSetting::GetInstance()->GetOutputRootFolderXML(outputRootFolderXML);
	if(outputRootFolderXML.empty())
	{
		outputRootFolderXML = st_EQFolderXMLPath;
	}

	std::string exportFolderPath = "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\";

	std::string formMsg;
	long resourceID = 0;

	try
	{
		// EQ�T�[�o�[��̋��L�t�H���_�ɃA�N�Z�X�ł��邩���`�F�b�N("\\<EQ��PC��IP�A�h���X>\#work_ex\ExportJDPaper_xml\")
		EReturnCode returnCode = ExportUtility::isValidExportFolder(exportFolderPath.c_str());
		if(returnCode == kReturnCode_NotAccess)
		{
			// ���L�t�H���_�ɃA�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK_XML;
			throw 1;
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// EQ�T�[�o�[��̋��L�t�H���_�p�X����Ō��"\"���폜����
			int pos = exportFolderPath.find_last_of("\\");
			std::string tmpExportFolder = exportFolderPath.substr(0, pos);
			char* tmpRootFolder = const_cast<char*>(tmpExportFolder.c_str());
			// EQ�T�[�o�[���root�t�H���_�p�X�̎擾("\\<EQ��PC��IP�A�h���X>\#work_ex")
			PathRemoveFileSpec(tmpRootFolder);

			// EQ�T�[�o�[���Root�t�H���_�ɃA�N�Z�X�ł��邩���`�F�b�N
			std::string rootFolder = tmpRootFolder;
			returnCode = ExportUtility::isValidExportFolder(rootFolder);
			if(returnCode != kReturnCode_Success)
			{
				// EQ�T�[�o�[���Root�t�H���_�����݂��Ȃ� or �A�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
				resourceID = IDM_ERR_NETWORK_XML;
				throw 1;
			}

			// ���L�t�H���_�����݂��Ȃ��ꍇ�A���L�t�H���_���쐬
			if( MakeSureDirectoryPathExists(exportFolderPath.c_str()) != TRUE )
			{
				// �t�H���_�쐬�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
				ExportUtility::replaceString("%s", exportFolderPath, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
				throw 1;
			}
		}
		else
		{
			// EQ�T�[�o�[��̋��L�t�H���_�ɃA�N�Z�X�ł���ꍇ�͏����Ȃ�	
		}

		bool isFixIP = CDataSystemSetting::GetInstance()->IsEnableAutoUpdateFixIP();
		bool isDED = CDataSystemSetting::GetInstance()->IsDED();

		std::string ipAddressSelf;
		if( isFixIP )
		{
			if( isDED ) ipAddressSelf = "192.168.0.30";
		} else {
			CDataSystemSetting::GetInstance()->GetClientPcIPAddrSelf(ipAddressSelf);
		}
		if(ipAddressSelf.empty())
		{
			// ���@��IP�A�h���X���L�ڂ���Ă��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
			ExportUtility::replaceString("%s", exportFolderPath, formMsg);
			resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
			throw 1;
		}

		if(isDED)
		{
			// DED�\�����̃t�@�C���p�X
			exportFolderPath += ipAddressSelf + "-DED"+ "\\";
		}
		else
		{
			// SES�\�����̃t�@�C���p�X
			exportFolderPath += ipAddressSelf + "-SES"+ "\\";
		}

		// �w��t�H���_�̑��݂��`�F�b�N
		returnCode = ExportUtility::isValidExportFolder(exportFolderPath.c_str());
		if(returnCode == kReturnCode_Success)
		{
			// �t�H���_�����݂���ꍇ�A�t�H���_���̃t�@�C�����폜
			ExportUtility::DeleteAllFilesEQ(exportFolderPath);
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// �t�H���_�����݂��Ȃ��ꍇ�A�t�H���_���쐬
			if( MakeSureDirectoryPathExists(exportFolderPath.c_str()) != TRUE )
			{
				// �t�H���_�쐬�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
				ExportUtility::replaceString("%s", exportFolderPath, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
				throw 1;
			}
		}
		else
		{
			// �t�H���_�ɃA�N�Z�X�ł��Ȃ��ꍇ�A�G���[���b�Z�[�W�̃p�����[�^���Z�b�g����
			formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK_XML;
			throw 1;
		}

		st_ExportFolderPath = exportFolderPath;

		bool isDualSimplex = CDataSystemSetting::GetInstance()->IsDualSimplex();
		// DualSimplex���ݒ�L����DED�\���̏ꍇ
		if( (isDualSimplex) && (isDED) )
		{
			std::string filePathSelf;
			filePathSelf += "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\" + ipAddressSelf + "-SES"+ "\\";
			// SES�t�H���_(���@)���̑S�t�@�C�����폜
			ExportUtility::DeleteAllFilesEQ(filePathSelf);

			std::string ipAddressPart;
			CDataSystemSetting::GetInstance()->GetClientPcIPAddrPart(ipAddressPart);
			std::string filePathPart;
			filePathPart += "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\" + ipAddressPart + "-SES"+ "\\";
			// SES�t�H���_���̑S�t�@�C�����폜
			ExportUtility::DeleteAllFilesEQ(filePathPart);
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
void DataRequestExportXML::deleteDuplicateCondition(std::list<SRequestConditionData>& ioRequests)
{
	auto itrRequest = ioRequests.begin();
	std::list<SRequestConditionData> collectiedList;

	while( itrRequest != ioRequests.end() )
	{
		auto itrCollected = collectiedList.begin();
		while( itrCollected != collectiedList.end() )
		{
			// �d���폜�ς݈˗����X�g�ɏd�������������������邩�`�F�b�N
			if( itrRequest->conditionName.compare(itrCollected->conditionName) == 0 )
			{
				std::string tmpFolderPath = ExportUtility::getLocalFolderPath(st_TempFolderPath);
				// ���ԃt�H���_�ɏd�������������̈ꎞ�t�@�C�������邩�`�F�b�N
				if( ExportUtility::IsFileExist(tmpFolderPath, itrCollected->localFileName, st_ExtensionXML) )
				{
					// �d�������������̈ꎞ�t�@�C�����폜
					std::string tmpFilePath = tmpFolderPath + itrCollected->localFileName + ".txt";
					ExportUtility::DeleteFiles(tmpFilePath);
				}

				// �˗����X�g����d���������������폜����
				itrCollected = collectiedList.erase(itrCollected);
				continue;
			}

			// ������������d�����Ă��Ȃ���Ύ��̈��������
			++itrCollected;
		}

		// �d���폜�ς݈˗����X�g�Ɉ��������o�^����
		collectiedList.push_back(*itrRequest);
		++itrRequest;
	}

	ioRequests.clear();
	// �d���폜�ς݈˗����X�g�̓��e���˗����X�g�ɐݒ�
	for(auto itr = collectiedList.begin(); itr != collectiedList.end(); ++itr) 
	{
		ioRequests.push_back(*itr);
	}
}


//JI�A�����֘A�t�H���_�̃~���[�����O�R�s�[
bool DataRequestExportXML::MirroringInspectionData(const std::string& inJIDataFolder)
{
	if( !(CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML()) )	return	true;
	
	std::string pcNameEQ;
	CDataSystemSetting::GetInstance()->GetEqCenterPcName(pcNameEQ);
	std::string outputRootFolderJIData;
	CDataSystemSetting::GetInstance()->GetOutputRootFolderInspectionData(outputRootFolderJIData);
	if(outputRootFolderJIData.empty())	return true;

	std::string srcPath = GetModuleDirectory();
	srcPath += inJIDataFolder;
	std::string dstPath = "\\\\" + pcNameEQ + "\\" + outputRootFolderJIData + "\\" + inJIDataFolder;
	if( srcPath.back() == '\\' )	srcPath.pop_back();
	if( dstPath.back() == '\\' )	dstPath.pop_back();

	return	mirrorFolder(static_cast<const std::string&>(srcPath), static_cast<const std::string&>(dstPath));
}

//GetLastError�̕�����擾�B
std::string DataRequestExportXML::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

//�R�}���h�v�����v�g�Ŏ��s����B
DWORD DataRequestExportXML::runSystemCmd(const std::string& inCmd)
{
	STARTUPINFO  si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hWndmain = nullptr;

	if (TRUE == CreateProcess(nullptr, const_cast<char*>(inCmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		//robocopy�̏I���R�[�h���`�F�b�N�B
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return exitCode;
	} else {
		//�G���[�����B
		std::stringstream ss;
		ss << "failed CreateProcess[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return 0;
	}
}

//�t�H���_�̃~���[�����O�B
bool DataRequestExportXML::mirrorFolder(const std::string& inSrcPath, const std::string& inDestPath)
{
	//CopyFile(SrcPath.str().c_str(), DestPath.str().c_str(), TRUE);

	if( CDataSystemSetting::GetInstance()->IsDebugLog() )
	{
		WriteDebugLog(inSrcPath);
		WriteDebugLog(inDestPath);
	}
	
	//---generate logfile name
	SYSTEMTIME tm = { 0 };
	GetLocalTime(&tm);
	char logname[64] = { 0 };
	_snprintf_s(logname, _TRUNCATE, "MirrorFailed%04d%02d%02d%02d%02d%02d%03d.txt",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	std::stringstream logStr;
	logStr << GetModuleDirectory() <<"\\Log\\" << std::string(logname);
	
	std::stringstream comStr;
	comStr << "cmd /c robocopy \"" << inSrcPath << "\" \"" << inDestPath << "\" /MIR /R:1 /W:1 /LOG:" << logStr.str();
	/* �Q�l�`robocopy�̏I���R�[�h�i8�ȏオ�G���[�j�B
		if errorlevel 16  echo  ***FATAL ERROR***  & goto end
		if errorlevel 15  echo FAIL MISM XTRA COPY & goto end
		if errorlevel 14  echo FAIL MISM XTRA      & goto end
		if errorlevel 13  echo FAIL MISM      COPY & goto end
		if errorlevel 12  echo FAIL MISM           & goto end
		if errorlevel 11  echo FAIL      XTRA COPY & goto end
		if errorlevel 10  echo FAIL      XTRA      & goto end
		if errorlevel  9  echo FAIL           COPY & goto end
		if errorlevel  8  echo FAIL                & goto end
		if errorlevel  7  echo      MISM XTRA COPY & goto end
		if errorlevel  6  echo      MISM XTRA      & goto end
		if errorlevel  5  echo      MISM      COPY & goto end
		if errorlevel  4  echo      MISM           & goto end
		if errorlevel  3  echo           XTRA COPY & goto end
		if errorlevel  2  echo           XTRA      & goto end
		if errorlevel  1  echo                COPY & goto end
		if errorlevel  0  echo    --no change--    & goto end
	*/
	int errNum = runSystemCmd(comStr.str());
	if (7 < errNum) {
		//�G���[�����B
		std::stringstream ss;
		ss << "failed robocopy[" << errNum << "]";
		WriteDebugLog(ss.str());
		return false;
	} else {
		std::remove((logStr.str()).c_str());
	}

	return true;
}
