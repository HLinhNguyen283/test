/**
 * @file	WaitTrigger.cpp
 * @brief	待機中トリガーの管理
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "WaitTrigger.h"
#include "CommonCBRProc.h"
#include <sstream>
// ==========================================================================
static const char* st_WaitFileName = "ImportWait.txt";				//!< インポート待機中トリガーファイル名
static const char* st_WaitTimeOutName = "ImportWaitTimeout.txt";	//!< インポートタイムアウト発生トリガーファイル名
static const char* st_FolderTemp = "TEMP";							//!< テンポラリフォルダ名
// ==========================================================================

CWaitTrigger::CWaitTrigger()
{
}

CWaitTrigger::~CWaitTrigger()
{
}

//インポート依頼受付イベント
bool CWaitTrigger::OnAddRequest(const std::string& inOutputURL)
{
	//待機中トリガーファイルを取り込み元に作成する
	try{
		CreateTriggerFile(inOutputURL, st_WaitFileName);
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "CWaitTrigger::OnAddRequest Error. " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		return false;
	}
	return true;
}

//インポート終了イベント
void CWaitTrigger::OnFinsh(const std::string& inOutputURL)
{
	//取り込み元の待機中トリガーファイルを削除する
	try{
		DeleteTriggerFile(inOutputURL, st_WaitFileName);
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "CWaitTrigger::OnFinsh Error. " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		//トリガーファイルの削除に失敗しても、既にインポートは終了しているためエラーにしない
	}
}

//インポート依頼一件分のインポートタイムアウト終了イベント
void CWaitTrigger::OnTimeoutFinsh(const std::string& inOutputURL)
{
	//取り込み元の待機中トリガーファイルを削除する
	try{
		DeleteTriggerFile(inOutputURL, st_WaitFileName);
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "CWaitTrigger::OnTimeoutFinsh Error. " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		//トリガーファイルの削除に失敗しても、終了処理を継続するためエラーにしない
	}

	//インポートタイムアウト発生トリガーファイルを取り込み元に作成する
	try{
		CreateTriggerFile(inOutputURL, st_WaitTimeOutName);
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "CWaitTrigger::OnTimeoutFinsh Error. " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		//トリガーファイルの作成に失敗しても、終了処理を継続するためエラーにしない
	}
}

//出力先にトリガーファイルを作成する
void CWaitTrigger::CreateTriggerFile(const std::string& inOutputURL, const std::string& inFileName)
{
	//ネットワーク先にファイルを作成する際に、ローカルに一時ファイルを作って移動している。(ネットワーク先のアクセスは失敗しやすいので、アクセス回数を減らすため)

	//ClientのTEMPフォルダに一時ファイルを作成する
	std::stringstream tmpPath;
	tmpPath << GetModuleDirectory() << st_FolderTemp << "\\" << inFileName;
	std::string srcPath = tmpPath.str();
	HANDLE fileHandle = CreateFile(srcPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fileHandle == INVALID_HANDLE_VALUE){
		DWORD errID = GetLastError();
		const std::string lastErr = getLastErrorString(errID);
		std::stringstream log;
		log << "Cannot create file. " << srcPath << std::endl << lastErr;
		throw std::runtime_error(log.str());
	}
	if(CloseHandle(fileHandle) == FALSE){
		DWORD errID = GetLastError();
		const std::string lastErr = getLastErrorString(errID);
		std::stringstream log;
		log << "Cannot close file. " << srcPath << std::endl << lastErr;
		throw std::runtime_error(log.str());
	}

	//ネットワーク先に移動する
	std::stringstream outputPath;
	outputPath << inOutputURL << "\\" << inFileName;
	std::string dstPath = outputPath.str();
	if(MoveFileEx(srcPath.c_str(), dstPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING) == FALSE){
		DWORD errID = GetLastError();
		const std::string lastErr = getLastErrorString(errID);
		std::stringstream log;
		log << "Cannot move file. " << std::endl << "src:" << srcPath << std::endl << "dst:" << dstPath << std::endl << lastErr;
		throw std::runtime_error(log.str());
	}
};

//出力先のトリガーファイルを削除する
void CWaitTrigger::DeleteTriggerFile(const std::string& inOutputURL, const std::string& inFileName)
{
	std::stringstream path;
	path << inOutputURL << "\\" << inFileName;
	std::string targetPath = path.str();
	if(DeleteFile(targetPath.c_str()) == FALSE){
		DWORD errID = GetLastError();
		if( errID != ERROR_FILE_NOT_FOUND ) {
			const std::string lastErr = getLastErrorString(errID);
			std::stringstream log;
			log << "Cannot delete file. " << targetPath << std::endl << lastErr;
			throw std::runtime_error(log.str());
		}
	}
}

const std::string CWaitTrigger::getLastErrorString(DWORD inErrID)
{
	LPVOID errBuf = GetLastErrorString(inErrID);
	if(errBuf == nullptr){
		return "";
	}
	const std::string errStr = static_cast<LPTSTR>(errBuf);
	FreeLastErrorString(errBuf);
	return errStr;
}
