/**
 * @file  OutputLog.cpp
 * @brief ログ出力クラス 実装ファイル
 * @date  20190610 CEC Miyachi 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <shlwapi.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "CriticalSection.h"
#include "OutputLog.h"
// ===========================================================================

/* ==================================================================================================== */
/**
	* @brief COutputLog コンストラクション
**/
/* ==================================================================================================== */
COutputLog::COutputLog()
{
	::InitializeCriticalSection(&m_cs);
	::InitializeCriticalSection(&m_cs_http);
}

/* ==================================================================================================== */
/**
	* @brief COutputLog デストラクション
**/
/* ==================================================================================================== */
COutputLog::~COutputLog()
{
	::DeleteCriticalSection(&m_cs);
	::DeleteCriticalSection(&m_cs_http);
}

/* ==================================================================================================== */
/**
	* @brief  ログ出力情報の初期設定を行う
	* @details (ログ出力ファイル名称を決定する)
	* @param  strFileName    : [i]ログファイルの先頭文字
**/
/* ==================================================================================================== */
void COutputLog::Start( std::string strFileName )
{
	USES_CONVERSION;

	SYSTEMTIME st;
	GetLocalTime(&st);

	char buf[512];
	_snprintf_s( buf, sizeof(buf) -1 , "%s_%04d%02d%02d_%02d%02d%02d.log", strFileName.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	m_strPath = GetModuleDirectory() + "\\" + buf;
}

/* ==================================================================================================== */
/**
	* @brief  ログ出力関数(std::string)
	* @param  strLog       : [i]ログ出力文言
	* @param  IsOutputTime : [i]時刻出力有無(true：あり、false：なし)
**/
/* ==================================================================================================== */
void COutputLog::Output( const std::string& strLog, bool IsOutputTime/*=true*/ )
{
	USES_CONVERSION;

	CCriticalSectionLocker locker(&m_cs);
	try {
		std::ofstream ofs( m_strPath.c_str(), std::ios::app );
		if ( IsOutputTime ) ofs << "[" << GetTime() << "] ";
		if ( IsOutputTime ) ofs << GetThreadId() << " ";
		ofs << strLog.c_str() << std::endl;
		ofs.close();
	}
	catch(...)
	{
		return;
	}
}

/* ==================================================================================================== */
/**
	* @brief  ログ出力関数(std::wstring)
	* @param  wstrLog      : [i]ログ出力文言
	* @param  IsOutputTime : [i]時刻出力有無(true：あり、false：なし)
**/
/* ==================================================================================================== */
void COutputLog::Output( const std::wstring& wstrLog, bool IsOutputTime/*=true*/ )
{
	USES_CONVERSION;

	Output( W2A(wstrLog.c_str()), IsOutputTime );
}

/* ==================================================================================================== */
/**
	* @brief  現在時刻編集(年月日時分秒＋ミリ秒)
	* @retval 編集済み現在時刻
**/
/* ==================================================================================================== */
std::string COutputLog::GetTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::ostringstream oss;
	oss << std::setw(4) << st.wYear << "/"
		<< std::setw(2) << std::setfill('0') << st.wMonth << "/"
		<< std::setw(2) << std::setfill('0') << st.wDay << " "
		<< std::setw(2) << std::setfill('0') << st.wHour << ":"
		<< std::setw(2) << std::setfill('0') << st.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << st.wSecond << "."
		<< std::setw(3) << std::setfill('0') << st.wMilliseconds;
	std::string strTime(oss.str());
	return strTime;
}

/* ==================================================================================================== */
/**
	* @brief  スレッドＩＤ編集
	* @retval 編集済みスレッドＩＤ
**/
/* ==================================================================================================== */
std::string COutputLog::GetThreadId()
{
	std::ostringstream oss;
	oss << std::setw(5) << std::setfill('0') << GetCurrentThreadId();
	std::string strThreadId(oss.str());
	return strThreadId;
}	

std::string COutputLog::GetModuleDirectory()
{
	TCHAR szFullPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	std::string s(szFullPath);
	size_t pos = s.rfind("\\");

	return s.substr(0, pos);
}