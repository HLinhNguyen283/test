/**
 * @file	Utility.cpp
 * @brief	ユーティリティ
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Utility.h"

#include "CommonCBRProc.h"

#include <string>
#include <sstream>
// ==========================================================================
//デバッグ用ログ出力
void CUtility::WriteDebugLog_(const std::string& inFunctionName, const std::string& inText)
{
	if(inText.empty() == false)
	{
		std::stringstream log;
		log << inFunctionName << " : " << inText << std::endl;
		WriteToLogBuf(LOG_DEBUG, log.str().c_str());
	}
}


//inResultがfalseの場合に例外を投げる
void CUtility::ThrowIfFalse(bool inResult, const std::string& inErrorText)
{
	if(inResult == false)
	{
		throw std::runtime_error(inErrorText);
	}
}

