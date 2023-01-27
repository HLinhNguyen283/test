/**
 * @file	EndTriggerCreator.cpp
 * @brief	取り込み先に完了トリガーを作成する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "EndTriggerCreator.h"
#include "CommonCBRProc.h"
#include <sstream>
// ==========================================================================

CEndTriggerCreator::CEndTriggerCreator(const std::string& inOutputURL)
	: m_OutputFilePath(inOutputURL)
{
	m_TempFilePath = GetModuleDirectory();
	m_TempFilePath += "TEMP";
	m_FileName = "";
}

CEndTriggerCreator::~CEndTriggerCreator()
{
}

void CEndTriggerCreator::AddResult(const std::string& inPrintConditionName, E_END_RESULT inResult)
{
	//一覧に追加
	std::pair<std::string, E_END_RESULT> data(inPrintConditionName, inResult);
	m_List.push_back(data);
}

void CEndTriggerCreator::WriteFailed()
{
	try{
		//Failedファイル作成(中身は空)
		m_FileName = "\\ImportFailed.txt";
		HANDLE fileHandle = createFile();
		closeFile(fileHandle);

		//ローカル(TEMP)に作ったので出力先(URLパス(UNC))に移動
		moveFile();
	}
	catch(std::runtime_error& error){
		std::stringstream log;
		log << "CEndTriggerCreator::WriteFailed() Error.\nsrc:" << (m_TempFilePath + m_FileName) << "\ndst:"<< (m_OutputFilePath + m_FileName) << "\n" << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
	}
}

void CEndTriggerCreator::WriteSuccess()
{
	auto getResultString = [](std::pair<std::string, E_END_RESULT>& inData)->std::string
	{
		std::stringstream result;
		result << "\"" << inData.first << "\",";

		switch(inData.second){
		case CEndTriggerCreator::END_RESULT_SUCCESS:
			result << "Success";
			break;
		case CEndTriggerCreator::END_RESULT_SKIP:
			result << "Skip";
			break;
		case CEndTriggerCreator::END_RESULT_ERROR:
			result << "Error";
			break;
		}
		result << std::endl;
		return result.str();
	};

	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	try{
		m_FileName  += "\\ImportSuccess.txt";
		fileHandle = createFile();
		for(auto data = m_List.begin(); data != m_List.end(); ++data){
			writeFile(fileHandle, getResultString(*data));
		}
		closeFile(fileHandle);
		fileHandle = INVALID_HANDLE_VALUE;

		//ローカル(TEMP)に作ったので出力先(URLパス(UNC))に移動
		moveFile();
	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "CEndTriggerCreator::WriteSuccess() Error.\nsrc:" << (m_TempFilePath + m_FileName) << "\ndst:"<< (m_OutputFilePath + m_FileName) << "\n" << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		closeFile(fileHandle);
	}
}

HANDLE CEndTriggerCreator::createFile()
{
	HANDLE fileHandle = CreateFile((m_TempFilePath + "\\" + m_FileName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(fileHandle == INVALID_HANDLE_VALUE){
		const std::string lastErr = getLastErrorString(GetLastError());
		std::stringstream log;
		log << "Cannot create file. " << lastErr;
		throw std::runtime_error(log.str());
	}

	return fileHandle;
};

void CEndTriggerCreator::closeFile(HANDLE inFileHandle)
{
	if(inFileHandle == INVALID_HANDLE_VALUE){
		return;
	}

	if(CloseHandle(inFileHandle) == 0){
		const std::string lastErr = getLastErrorString(GetLastError());
		std::stringstream log;
		log << "CEndTriggerCreator::closeFile : Cannot close file, " << (m_TempFilePath + "\\" + m_FileName) << " " << lastErr;
		WriteToLogBuf(LOG_DEBUG, log.str());
	}
}

void CEndTriggerCreator::writeFile(HANDLE inFileHandle, const std::string& inString)
{
	DWORD writeSize = 0;
	if(WriteFile(inFileHandle, inString.c_str(), inString.size(), &writeSize, NULL) == 0){
		const std::string lastErr = getLastErrorString(GetLastError());
		std::stringstream log;
		log << "Cannot write file. " << lastErr;
		throw std::runtime_error(log.str());
	}

	if(inString.size() > writeSize){
		std::stringstream log;
		log << "Cannot write file. write size = " << writeSize;
		throw std::runtime_error(log.str());
	}
}

void CEndTriggerCreator::moveFile()
{
	if(MoveFileEx((m_TempFilePath + "\\" + m_FileName).c_str(), (m_OutputFilePath + "\\" + m_FileName).c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING) == FALSE){
		const std::string lastErr = getLastErrorString(GetLastError());
		std::stringstream log;
		log << "Cannot move file. " << lastErr;
		throw std::runtime_error(log.str());
	}
}

const std::string CEndTriggerCreator::getLastErrorString(DWORD inErrID)
{
	LPVOID errBuf = GetLastErrorString(inErrID);
	if(errBuf == nullptr){
		return "";
	}

	const std::string errStr = static_cast<LPTSTR>(errBuf);
	FreeLastErrorString(errBuf);
	return errStr;
}
