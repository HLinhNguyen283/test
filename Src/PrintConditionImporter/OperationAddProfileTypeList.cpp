/**
 * @file	OperationAddProfileTypeList.h
 * @brief	AddPaperTypeList.txtおよびAddAlignmentList.txtの操作(作成・削除等)クラス
**/

//=======================================================================
// Include files
#include "stdafx.h"
#include "OperationAddProfileTypeList.h"
#include "CommonCBRProc.h"
#include <sstream>
#include<iostream>
#include<fstream>
//=======================================================================

// constructor
COperationAddProfileTypeList::COperationAddProfileTypeList(const std::string& inOutputURL, const std::string inOutputFname)
	: m_OutputFilePath((inOutputURL + "\\" + inOutputFname))
{
}

// destructor
COperationAddProfileTypeList::~COperationAddProfileTypeList()
{
}

// 成功時の取り込み結果を出力する
void COperationAddProfileTypeList::WriteSuccess(const std::string& inProfileName)
{
	auto getResultString = [](std::string inData)->std::string
	{
		std::stringstream result;
		result << inData << std::endl;
		return result.str();
	};

	try{

		writeFile(getResultString(inProfileName));

	}catch(std::runtime_error& error){
		std::stringstream log;
		log << "OperationAddProfileTypeList::WriteSuccess() Error." << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
	}
}

// ファイルに書く
void COperationAddProfileTypeList::writeFile(const std::string& inString)
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

// エラーコードの文字列を取得する
const std::string COperationAddProfileTypeList::getLastErrorString(DWORD inErrID)
{
	LPVOID errBuf = GetLastErrorString(inErrID);
	if(errBuf == nullptr){
		return "";
	}

	const std::string errStr = static_cast<LPTSTR>(errBuf);
	FreeLastErrorString(errBuf);
	return errStr;
}

COperationAddPaperTypeList::COperationAddPaperTypeList(const std::string& inOutputURL)
	: COperationAddProfileTypeList(inOutputURL, "AddPaperTypeList.txt")
{
}

COperationAddPaperTypeList::~COperationAddPaperTypeList()
{
}


COperationAddAllignmentList::COperationAddAllignmentList(const std::string& inOutputURL)
	: COperationAddProfileTypeList(inOutputURL, "AddAlignmentList.txt")
{
}

COperationAddAllignmentList::~COperationAddAllignmentList()
{
}

