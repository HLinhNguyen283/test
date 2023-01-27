/*! \file ExportFileUpdate.cpp
 *  \brief ExportFileUpdate.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/20 pt.ju-tanaka 新規作成
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

//エクスポート先のルートパスが有効かチェックする
EReturnCode ExportFileUpdate::IsValidExportFolder(const std::string& inDestRootFolderPath)
{
	//引数のパス(UNC)が有効かチェックする
	if (!PathFileExists(inDestRootFolderPath.c_str())) 
	{
		// パスが存在しない.
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
		// アクセスできない.
		return kReturnCode_NotAccess;
	}

	return kReturnCode_Success;
}

//コンストラクタ
ExportFileUpdate::ExportFileUpdate()
{
	std::stringstream pathName;

	//\Client\Temp\PrintConditionExportText が一時出力先のフォルダ(無ければ作る)
	pathName << GetModuleDirectory() << st_tempFolderPath << "\\";
	MakeSureDirectoryPathExists(pathName.str().c_str());
}

//デストラクタ
ExportFileUpdate::~ExportFileUpdate()
{
}

//ファイル出力する
bool ExportFileUpdate::WriteFile(
	const std::string& inPrintConditionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData,
	const std::string& inLocalFileName)
{
    std::stringstream exportFilePath;
    exportFilePath << GetModuleDirectory() << st_tempFolderPath << "\\" << inLocalFileName << ".txt";

	ExportWriteTextFile exportWriteTextFile;
	// \Client\Temp\PrintConditionExportTextにテキストファイル(MMDDhhmmssfff_印刷条件名.txt)を出力する
	bool ret = exportWriteTextFile.onWriteTextFile(exportFilePath.str(), inMigrationData);
    return ret;
}

//\Client\Temp\PrintConditionExportTextのファイルを指定パスに移動する
bool ExportFileUpdate::MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName)
{
	//エラー実行処理
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

	//移動元のファイル名から"MMDDhhmmssfff_"を削除する
	std::string eqFileName = inFileName;
	size_t pos = eqFileName.find("_");
	if(pos == std::string::npos)
	{
		return false;
	}
	eqFileName.replace(0, pos + 1, "");

	//\Client\Temp\PrintConditionExportTextフォルダ（移動元）
	std::stringstream SrcPath;
	SrcPath << GetModuleDirectory() << st_tempFolderPath;
	SrcPath  << "\\" << inFileName << ".txt";

	//EQ制御サーバ上の共有フォルダ（移動先）
	std::stringstream DestPath;
	DestPath << inDestRootFolderPath;
	//フォルダがなければ作成する
	MakeSureDirectoryPathExists(DestPath.str().c_str());
	DestPath << eqFileName << ".txt";

	DWORD FileSize = 0;
	BYTE* datBuf = NULL;
	//移動元のファイルを排他ロック状態でオープンする
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

	//移動先のファイルを排他ロック状態でオープンする
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
	//移動元のファイルを削除する
	DeleteFiles(SrcPath.str());

	return true;
 }

// \Client\Temp\PrintConditionExportTextフォルダの指定ファイルを削除する
void ExportFileUpdate::DeleteFiles(const std::string& inFilePath)
{
	std::remove(inFilePath.c_str());
}

// EQサーバ共有フォルダ内の指定ファイルを削除する
bool ExportFileUpdate::DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName)
{
	// EQサーバ共有フォルダパスが有効かチェック
	if(IsValidExportFolder(inFolderPath) == kReturnCode_Success)
	{
		std::stringstream srcPath;
		srcPath << inFolderPath.c_str() << inPrintConditionName.c_str() << ".txt";
		//指定ファイルを削除指定する(handleがクローズされるとファイル削除する)
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

// EQサーバ共有フォルダ内のファイルを全て削除する
void ExportFileUpdate::DeleteAllFilesEQ(const std::string& inFolderPath)
{
	// EQサーバ共有フォルダパスが有効かチェック
	if(IsValidExportFolder(inFolderPath) == kReturnCode_Success)
	{
		// EQサーバ共有フォルダ内のファイルを全て削除する
		std::stringstream comStr;
		comStr << "cmd /c del /Q \"" << inFolderPath.c_str();

		int errNum = runSystemCmd(comStr.str());
		if (0 < errNum && 2 != errNum)
		{
			//エラー発生。
			//0:「成功」。2:「指定されたファイルが見つかりません。」はエラーから除く。
			std::stringstream strLog;
			strLog << "ExportFileUpdate::DeleteAllFilesEQ Failed del[" << errNum << "]";
			WriteToLogBuf(LOG_DEBUG, strLog.str());
			return;
		}
	}
}

//コマンドプロンプトで実行する。
DWORD ExportFileUpdate::runSystemCmd(const std::string& inCmd)
{
	STARTUPINFO  si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hWndmain = nullptr;

	if (TRUE == CreateProcess(nullptr, const_cast<char*>(inCmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		//robocopyの終了コードをチェック。
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return exitCode;
	}
	else
	{
		//エラー発生。
		std::stringstream ss;
		ss << "failed CreateProcess[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return 0;
	}
}

//GetLastErrorの文字列取得。
std::string ExportFileUpdate::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

// ファイルが存在するかをチェック
bool ExportFileUpdate::IsFileExist(std::string& inFolderPath, std::string& inFileName)
{
	WIN32_FIND_DATA findFileData = {0};
	std::string filePath = inFolderPath + inFileName + ".txt";
	HANDLE findHandle = FindFirstFile(filePath.c_str(), &findFileData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		// ファイルが見つかった場合
		FindClose(findHandle);
		return true;
	}

	return false;
}

// 一時フォルダパスの取得
std::string ExportFileUpdate::getLocalFolderPath()
{
	//\Client\Temp\PrintConditionExportText が一時出力先のフォルダ
	std::string pathName;
	pathName = GetModuleDirectory() + st_tempFolderPath + "\\";
	return pathName;
}

// 一時ファイル名を作成
std::string ExportFileUpdate::createLocalFileName(const std::string& inPrintConditionName)
{
	// 中間ファイル名を作成する(MMDDhhmmssfff_印刷条件名)
	// システム時刻を取得
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
