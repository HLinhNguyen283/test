/**
 * @file	OperationImportProgress.h
 * @brief	ImportProgress.txtの操作(作成・削除等)クラス
**/

//=======================================================================
// Include files
#include "stdafx.h"
#include "OperationImportProgress.h"
#include "CommonCBRProc.h"
#include <sstream>
#include<iostream>
#include<fstream>
//=======================================================================

// constructor
COperationImportProgress::COperationImportProgress(const std::string& inOutputURL)
	: m_OutputFilePath((inOutputURL + "\\ImportProgress.txt"))
{
}

// destructor
COperationImportProgress::~COperationImportProgress()
{
}

// 成功時の取り込み結果を出力する
void COperationImportProgress::WriteSuccess(const std::string& inPrintConditionName, E_END_RESULT inResult)
{
	auto getResultString = [](std::pair<std::string, E_END_RESULT>& inData)->std::string
	{
		std::stringstream result;
		result << "\"" << inData.first << "\",";

		switch(inData.second){
		case COperationImportProgress::END_RESULT_SUCCESS:
			result << "Success";
			break;
		case COperationImportProgress::END_RESULT_SKIP:
			result << "Skip";
			break;
		case COperationImportProgress::END_RESULT_ERROR:
			result << "Error";
			break;
		}
		result << std::endl;
		return result.str();
	};

	try{

		std::pair<std::string, E_END_RESULT> data(inPrintConditionName, inResult);
		writeFile(getResultString(data));

	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "COperationImportProgress::WriteSuccess() Error." << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
	}
}

// ファイル作成
void COperationImportProgress::createFile()
{
	HANDLE fileHandle = CreateFile(m_OutputFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(fileHandle == INVALID_HANDLE_VALUE){
		DWORD errorID = GetLastError();
		const std::string lastErr = getLastErrorString(errorID);
		std::stringstream log;
		log << "COperationImportProgress::createFile : Cannot create file. " << m_OutputFilePath << " " << lastErr;
		WriteToLogBuf(LOG_DEBUG, log.str());
		return;
	}
	closeFile(fileHandle);
};


/**
	@brief	ImportProgress.txtファイル削除
*/
bool COperationImportProgress::deleteImportProggressFile()
{	
	try{
		deleteFile();
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "COperationImportProgress::deleteImportProggressFile Error. " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		return false;
	}
	return true;
}

// ファイルを閉じる
void COperationImportProgress::closeFile(HANDLE inFileHandle)
{
	if(inFileHandle == INVALID_HANDLE_VALUE){
		return;
	}

	if(CloseHandle(inFileHandle) == 0){
		DWORD errorID = GetLastError();
		const std::string lastErr = getLastErrorString(errorID);
		std::stringstream log;
		log << "CEndTriggerCreator::closeFile : Cannot close file. " << m_OutputFilePath << " " << lastErr;
		WriteToLogBuf(LOG_DEBUG, log.str());
	}
}
// ファイルに書く
void COperationImportProgress::writeFile(const std::string& inString)
{
	auto handleError = [&](DWORD inErrorID)
	{
		m_FaileList.push_back(inString);
		const std::string lastErr = getLastErrorString(inErrorID);
		std::stringstream log;
		log << m_OutputFilePath << " " << lastErr;
		throw std::runtime_error(log.str());
	};


	const char *fileName = m_OutputFilePath.c_str();

	std::ofstream file(fileName, std::ios::app);
	if (!file)
	{
		DWORD errorID = GetLastError();
		handleError(errorID);
	}

	std::stringstream ss;
	
	for(auto itr = m_FaileList.begin(); itr != m_FaileList.end(); ++itr)
	{
		ss << *itr;
	}
	file << ss.str() << inString.c_str() ;
	
	if (!file)
	{
		DWORD errorID = GetLastError();
		handleError(errorID);
	}

	m_FaileList.clear();

}

// ファイル削除
void COperationImportProgress::deleteFile()
{
	if(DeleteFile(m_OutputFilePath.c_str()) == FALSE){
		DWORD errID = GetLastError();
		if( errID != ERROR_FILE_NOT_FOUND ) {
			const std::string lastErr = getLastErrorString(errID);
			std::stringstream log;
			log << m_OutputFilePath << " " << lastErr;
			throw std::runtime_error(log.str());
		}
	}
}

// エラーコードの文字列を取得する
const std::string COperationImportProgress::getLastErrorString(DWORD inErrID)
{
	LPVOID errBuf = GetLastErrorString(inErrID);
	if(errBuf == nullptr){
		return "";
	}

	const std::string errStr = static_cast<LPTSTR>(errBuf);
	FreeLastErrorString(errBuf);
	return errStr;
}


