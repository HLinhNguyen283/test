/**
 * @file	FinishFile.cpp
 * @brief	完了トリガーファイル
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "FinishFile.h"
#include "PrintCondition.h"
#include "CommonCBRProc.h"
#include "Utility.h"
#include "DataSystemSetting.h"

#include <string>
#include <sstream>
#include <fstream>

// ==========================================================================


//コンストラクタ
CFinishFile::CFinishFile()
{
}

//デストラクタ
CFinishFile::~CFinishFile()
{
}

//ファイル作成する
bool CFinishFile::CreateFile(const std::string& inDestRootFolderPath, bool inSuccess)
{
	//GetLastErrorの文字列取得。
	auto getLastError = [&]()->std::string {
		DWORD errID = GetLastError();
		LPVOID errBuf = GetLastErrorString(errID);
		std::string errMsg = (LPTSTR)errBuf;
		FreeLastErrorString(errBuf);
		return errMsg;
	};

	std::stringstream inFilePath;
	bool isDebugLog = CDataSystemSetting::GetInstance()->IsDebugLog();

	//inDestRootFolderPathにファイル作成する
	if (true == inSuccess) {
		//成功指定ならExportSuccess.txtを作成する(中身は空)
		inFilePath << inDestRootFolderPath << "\\ExportSuccess.txt";
		if(isDebugLog)
		{
			std::stringstream log;
			log << "request Export sucess";
			WriteToLogBuf(LOG_DEBUG, log.str());
		}
	} else {
		//失敗指定ならExportFailed.txtを作成する(中身は空)
		inFilePath << inDestRootFolderPath << "\\ExportFailed.txt";
		if(isDebugLog)
		{
			std::stringstream log;
			log << "request Export failed";
			WriteToLogBuf(LOG_DEBUG, log.str());
		}
	}
	std::ofstream file(inFilePath.str(), std::ios_base::out);
	if (nullptr == file) {
		//ファイル出力失敗。
		std::stringstream ss;
		ss << "failed std::ofstream[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return false;
	}
	return true;
}
