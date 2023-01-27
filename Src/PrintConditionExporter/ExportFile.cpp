/**
 * @file	ExportFile.cpp
 * @brief	エクスポートファイル(テキスト)
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "ExportFile.h"
#include "CommonCBRProc.h"
#include "Utility.h"
#include "ResDef.h"
#include "ExportWriteTextFile.h"

#include <string>
#include <memory>
#include <sstream>
#include <imagehlp.h>

// ==========================================================================
const std::string CExportFile::st_tempFolderPath = "Temp\\PrintConditionExport";
const std::string CExportFile::st_paperDataFolder = "PaperData";


//エクスポート先のルートパスが有効かチェックする
bool CExportFile::IsValidExportFolder(const std::string& inDestRootFolderPath)
{
	//引数のパス(UNC)が有効かチェックする
	if (!PathFileExists(inDestRootFolderPath.c_str())) {
		// パスが存在しない.
		return false;
	}

	HANDLE hFile = CreateFile(inDestRootFolderPath.c_str()
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		, NULL
		, OPEN_EXISTING
		, FILE_FLAG_BACKUP_SEMANTICS
		, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
	} else {
		// アクセスできない.
		return false;
	}

	return true;
}


//コンストラクタ
CExportFile::CExportFile()
{
	std::stringstream pathName;

	//\Client\Temp\PrintConditionExport が一時出力先のフォルダ(無ければ作る)
	pathName << GetModuleDirectory() << st_tempFolderPath << "\\";
	MakeSureDirectoryPathExists(pathName.str().c_str());
}

//デストラクタ
CExportFile::~CExportFile()
{
	//一時出力先のフォルダの中身を空にする
	DeleteAllFiles();
}

//ファイル出力する
bool CExportFile::WriteFile(const CPrintCondition& inPdbData)
{
    std::stringstream exportFilePath;
    exportFilePath << GetModuleDirectory() << st_tempFolderPath << "\\" << inPdbData.m_printConditionName << ".txt";

	ExportWriteTextFile exportWriteTextFile;
	// \Client\Temp\PrintConditionExportにテキストファイル(<印刷条件名>.txt)を出力する
	bool ret = exportWriteTextFile.onWriteTextFile(exportFilePath.str(), inPdbData.m_migrationData);
	return ret;
}

//\Client\Temp\PrintConditionExportの全ファイルを指定パスに移動する
bool CExportFile::MoveAllFiles(const std::string& inDestRootFolderPath)
{
	//inDestRootFolderPath\PaperDataに\Client\Temp\PrintConditionExportの全ファイルを移動する
	std::stringstream DestPath;
	std::stringstream SrcPath;

	//\Client\Temp\PrintConditionExportフォルダ（移動元）
	SrcPath << GetModuleDirectory() << st_tempFolderPath;

	//inDestRootFolderPath\PaperDataフォルダ（移動先）
	DestPath << inDestRootFolderPath << "\\" << st_paperDataFolder;
	//PaperDataフォルダがなければ作成する
	MakeSureDirectoryPathExists(DestPath.str().c_str());

	//ファイルを移動させる。
	if (false == moveFile(SrcPath.str(), DestPath.str())) {
		//エラー発生。
		return false;
	}

	return true;
}

//\Client\Temp\PrintConditionExportのファイルを全て削除する
void CExportFile::DeleteAllFiles()
{
	//\Client\Temp\PrintConditionExportのファイルを全て削除する
	std::stringstream comStr;
	std::stringstream SrcPath;
	SrcPath << GetModuleDirectory() << st_tempFolderPath;
	comStr << "cmd /c rmdir /Q /S \"" << SrcPath.str() << "\\\"";

	int errNum = runSystemCmd(comStr.str());
	if (0 < errNum && 2 != errNum) {
		//エラー発生。
		//0:「成功」。2:「指定されたファイルが見つかりません。」はエラーから除く。
		std::stringstream ss;
		ss << "failed rm[" << errNum << "]";
		WriteDebugLog(ss.str());
		return;
	}
}

//コマンドプロンプトで実行する。
DWORD CExportFile::runSystemCmd(const std::string& inCmd)
{
	STARTUPINFO  si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hWndmain = nullptr;

	if (TRUE == CreateProcess(nullptr, const_cast<char*>(inCmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		//robocopyの終了コードをチェック。
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return exitCode;
	} else {
		//エラー発生。
		std::stringstream ss;
		ss << "failed CreateProcess[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return 0;
	}
}

//GetLastErrorの文字列取得。
std::string CExportFile::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

//ファイルを移動させる。
bool CExportFile::moveFile(const std::string& inSrcPath, const std::string& inDestPath)
{
	//CopyFile(SrcPath.str().c_str(), DestPath.str().c_str(), TRUE);
	std::stringstream comStr;
	comStr << "cmd /c robocopy /COPYALL /MOV \"" << inSrcPath << "\" \"" << inDestPath << "\"";
	/* 参考～robocopyの終了コード（8以上がエラー）。
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
		//エラー発生。
		std::stringstream ss;
		ss << "failed robocopy[" << errNum << "]";
		WriteDebugLog(ss.str());
		return false;
	}

	return true;
}
