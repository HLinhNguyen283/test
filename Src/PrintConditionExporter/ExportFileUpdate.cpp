/*! \file ExportFileUpdate.cpp
 *  \brief ExportFileUpdate.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/20 pt.ju-tanaka �V�K�쐬
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "ExportFileUpdate.h"
#include "Utility.h"
#include "ResDef.h"
#include "Stub_PaperDBIF.h"
#include "ExportWriteTextFile.h"

#include <string>
#include <sstream>
#include <imagehlp.h>
#include <iomanip>

// ==========================================================================
const std::string st_tempFolderPath = "Temp\\PrintConditionExportText";

//�G�N�X�|�[�g��̃��[�g�p�X���L�����`�F�b�N����
EReturnCode ExportFileUpdate::IsValidExportFolder(const std::string& inDestRootFolderPath)
{
	//�����̃p�X(UNC)���L�����`�F�b�N����
	if (!PathFileExists(inDestRootFolderPath.c_str())) 
	{
		// �p�X�����݂��Ȃ�.
		return kReturnCode_NotExists;
	}

	HANDLE hFile = CreateFile(inDestRootFolderPath.c_str()
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		, NULL
		, OPEN_EXISTING
		, FILE_FLAG_BACKUP_SEMANTICS
		, NULL);
	if (hFile != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hFile);
	}
	else
	{
		// �A�N�Z�X�ł��Ȃ�.
		return kReturnCode_NotAccess;
	}

	return kReturnCode_Success;
}

//�R���X�g���N�^
ExportFileUpdate::ExportFileUpdate()
{
	std::stringstream pathName;

	//\Client\Temp\PrintConditionExportText ���ꎞ�o�͐�̃t�H���_(������΍��)
	pathName << GetModuleDirectory() << st_tempFolderPath << "\\";
	MakeSureDirectoryPathExists(pathName.str().c_str());
}

//�f�X�g���N�^
ExportFileUpdate::~ExportFileUpdate()
{
}

//�t�@�C���o�͂���
bool ExportFileUpdate::WriteFile(
	const std::string& inPrintConditionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData,
	const std::string& inLocalFileName)
{
    std::stringstream exportFilePath;
    exportFilePath << GetModuleDirectory() << st_tempFolderPath << "\\" << inLocalFileName << ".txt";

	ExportWriteTextFile exportWriteTextFile;
	// \Client\Temp\PrintConditionExportText�Ƀe�L�X�g�t�@�C��(MMDDhhmmssfff_���������.txt)���o�͂���
	bool ret = exportWriteTextFile.onWriteTextFile(exportFilePath.str(), inMigrationData);
    return ret;
}

//\Client\Temp\PrintConditionExportText�̃t�@�C�����w��p�X�Ɉړ�����
bool ExportFileUpdate::MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName)
{
	//�G���[���s����
	auto errExecution = [&](HANDLE inSrcFileHandle, HANDLE inDestFileHandle, BYTE* inDatBuf) -> void
	{
		if(NULL != inSrcFileHandle)
		{
			CloseHandle(inSrcFileHandle);
			inSrcFileHandle = NULL;
		}
		if(NULL != inDestFileHandle)
		{
			CloseHandle(inDestFileHandle);
			inDestFileHandle = NULL;
		}
		if(NULL != inDatBuf)
		{
			delete[] inDatBuf;
		}
	};

	//�ړ����̃t�@�C��������"MMDDhhmmssfff_"���폜����
	std::string eqFileName = inFileName;
	size_t pos = eqFileName.find("_");
	if(pos == std::string::npos)
	{
		return false;
	}
	eqFileName.replace(0, pos + 1, "");

	//\Client\Temp\PrintConditionExportText�t�H���_�i�ړ����j
	std::stringstream SrcPath;
	SrcPath << GetModuleDirectory() << st_tempFolderPath;
	SrcPath  << "\\" << inFileName << ".txt";

	//EQ����T�[�o��̋��L�t�H���_�i�ړ���j
	std::stringstream DestPath;
	DestPath << inDestRootFolderPath;
	//�t�H���_���Ȃ���΍쐬����
	MakeSureDirectoryPathExists(DestPath.str().c_str());
	DestPath << eqFileName << ".txt";

	DWORD FileSize = 0;
	BYTE* datBuf = NULL;
	//�ړ����̃t�@�C����r�����b�N��ԂŃI�[�v������
	HANDLE srcFileHandle = CreateFile(SrcPath.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == srcFileHandle)
	{
		DWORD error = GetLastError();
		std::stringstream errLog;
		errLog << "ExportFileUpdate::MoveFiles Failed SrcPath[" << SrcPath.str() << "]" << ", ErrCode = " << error;
		WriteToLogBuf(LOG_DEBUG, errLog.str());
		return false;
	}
	else
	{
		FileSize = GetFileSize(srcFileHandle, NULL);
		if(INVALID_FILE_SIZE == FileSize)
		{
			DWORD error = GetLastError();
			std::stringstream errLog;
			errLog << "ExportFileUpdate::MoveFiles Failed GetFileSize" << ", SrcPath[" << SrcPath.str() << "]" << ", ErrCode = " << error;
			WriteToLogBuf(LOG_DEBUG, errLog.str());
			errExecution(srcFileHandle, NULL, datBuf);
			return false;
		}

		datBuf = new BYTE[FileSize];
		DWORD readSize = 0;
		if(FALSE == ReadFile(srcFileHandle, datBuf, FileSize, &readSize, NULL))
		{
			DWORD error = GetLastError();
			std::stringstream errLog;
			errLog << "ExportFileUpdate::MoveFiles Failed ReadFile" << ", SrcPath[" << SrcPath.str() << "]" << ", ErrCode = " << error;
			WriteToLogBuf(LOG_DEBUG, errLog.str());
			errExecution(srcFileHandle, NULL, datBuf);
			return false;
		}
	}
	CloseHandle(srcFileHandle);
	srcFileHandle = NULL;

	//�ړ���̃t�@�C����r�����b�N��ԂŃI�[�v������
	HANDLE destFileHandle = CreateFile(DestPath.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if(INVALID_HANDLE_VALUE == destFileHandle)
	{
		DWORD error = GetLastError();
		std::stringstream errLog;
		errLog << "ExportFileUpdate::MoveFiles Failed DestPath[" << DestPath.str() << "]" << ", ErrCode = " << error;
		WriteToLogBuf(LOG_DEBUG, errLog.str());
		errExecution(NULL, destFileHandle, datBuf);
		return false;
	}
	else
	{
		DWORD writeSize = 0;
		if(FALSE == ::WriteFile(destFileHandle, datBuf, FileSize, &writeSize, NULL))
		{
			DWORD error = GetLastError();
			std::stringstream errLog;
			errLog << "ExportFileUpdate::MoveFiles Failed WriteFile" << ", DestPath[" << DestPath.str() << "]" << ", ErrCode = " << error;
			WriteToLogBuf(LOG_DEBUG, errLog.str());
			errExecution(NULL, destFileHandle, datBuf);				
			return false;
		}
	}

	CloseHandle(destFileHandle);
	destFileHandle = NULL;
	delete[] datBuf;
	//�ړ����̃t�@�C�����폜����
	DeleteFiles(SrcPath.str());

	return true;
 }

// \Client\Temp\PrintConditionExportText�t�H���_�̎w��t�@�C�����폜����
void ExportFileUpdate::DeleteFiles(const std::string& inFilePath)
{
	std::remove(inFilePath.c_str());
}

// EQ�T�[�o���L�t�H���_���̎w��t�@�C�����폜����
bool ExportFileUpdate::DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName)
{
	// EQ�T�[�o���L�t�H���_�p�X���L�����`�F�b�N
	if(IsValidExportFolder(inFolderPath) == kReturnCode_Success)
	{
		std::stringstream srcPath;
		srcPath << inFolderPath.c_str() << inPrintConditionName.c_str() << ".txt";
		//�w��t�@�C�����폜�w�肷��(handle���N���[�Y�����ƃt�@�C���폜����)
		HANDLE handle = CreateFile(srcPath.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
		if(INVALID_HANDLE_VALUE != handle)
		{
			CloseHandle(handle);
			handle = NULL;
			return true;
		}
		else
		{
			CloseHandle(handle);
			handle = NULL;
			std::stringstream errLog;
			errLog << "ExportFileUpdate::DeleteFileEQ Failed FilePath[" << srcPath.str().c_str() << "]" << ", handle = INVALID_HANDLE_VALUE";
			WriteToLogBuf(LOG_DEBUG, errLog.str());
			return false;
		}
	}

	std::stringstream errLog;
	errLog << "ExportFileUpdate::DeleteFileEQ Failed IsValidExportFolder FolderPath[" << inFolderPath.c_str() << "]";
	WriteToLogBuf(LOG_DEBUG, errLog.str());

	return false;
}

// EQ�T�[�o���L�t�H���_���̃t�@�C����S�č폜����
void ExportFileUpdate::DeleteAllFilesEQ(const std::string& inFolderPath)
{
	// EQ�T�[�o���L�t�H���_�p�X���L�����`�F�b�N
	if(IsValidExportFolder(inFolderPath) == kReturnCode_Success)
	{
		// EQ�T�[�o���L�t�H���_���̃t�@�C����S�č폜����
		std::stringstream comStr;
		comStr << "cmd /c del /Q \"" << inFolderPath.c_str();

		int errNum = runSystemCmd(comStr.str());
		if (0 < errNum && 2 != errNum)
		{
			//�G���[�����B
			//0:�u�����v�B2:�u�w�肳�ꂽ�t�@�C����������܂���B�v�̓G���[���珜���B
			std::stringstream strLog;
			strLog << "ExportFileUpdate::DeleteAllFilesEQ Failed del[" << errNum << "]";
			WriteToLogBuf(LOG_DEBUG, strLog.str());
			return;
		}
	}
}

//�R�}���h�v�����v�g�Ŏ��s����B
DWORD ExportFileUpdate::runSystemCmd(const std::string& inCmd)
{
	STARTUPINFO  si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hWndmain = nullptr;

	if (TRUE == CreateProcess(nullptr, const_cast<char*>(inCmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		//robocopy�̏I���R�[�h���`�F�b�N�B
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return exitCode;
	}
	else
	{
		//�G���[�����B
		std::stringstream ss;
		ss << "failed CreateProcess[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return 0;
	}
}

//GetLastError�̕�����擾�B
std::string ExportFileUpdate::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

// �t�@�C�������݂��邩���`�F�b�N
bool ExportFileUpdate::IsFileExist(std::string& inFolderPath, std::string& inFileName)
{
	WIN32_FIND_DATA findFileData = {0};
	std::string filePath = inFolderPath + inFileName + ".txt";
	HANDLE findHandle = FindFirstFile(filePath.c_str(), &findFileData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		// �t�@�C�������������ꍇ
		FindClose(findHandle);
		return true;
	}

	return false;
}

// �ꎞ�t�H���_�p�X�̎擾
std::string ExportFileUpdate::getLocalFolderPath()
{
	//\Client\Temp\PrintConditionExportText ���ꎞ�o�͐�̃t�H���_
	std::string pathName;
	pathName = GetModuleDirectory() + st_tempFolderPath + "\\";
	return pathName;
}

// �ꎞ�t�@�C�������쐬
std::string ExportFileUpdate::createLocalFileName(const std::string& inPrintConditionName)
{
	// ���ԃt�@�C�������쐬����(MMDDhhmmssfff_���������)
	// �V�X�e���������擾
	SYSTEMTIME systemTime = {0};
	GetLocalTime(&systemTime);
	std::stringstream createTime;
	createTime << std::setfill('0') << std::setw(2) << systemTime.wMonth;
	createTime << std::setfill('0') << std::setw(2) << systemTime.wDay;
	createTime << std::setfill('0') << std::setw(2) << systemTime.wHour;
	createTime << std::setfill('0') << std::setw(2) << systemTime.wMinute;
	createTime << std::setfill('0') << std::setw(2) << systemTime.wSecond;
	createTime << std::setfill('0') << std::setw(3) << systemTime.wMilliseconds;
	createTime << "_";
	std::string localFilename = createTime.str() + inPrintConditionName;
	return localFilename;
}
