/**
	* @file		GetTime.cpp
	* @brief	現在日時取得
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <time.h>
#include "GetTime.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief  yyyyMMddhhmmss_999 の形式で現在日時を取得する
	* @retval 現在日時（yyyyMMddhhmmss_999）
**/
CString CGetTime::GetStrTimeMM()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換
	CString cs;
	cs.Format(_T("%04d%02d%02d%02d%02d%02d_%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return cs;
}

/**
	* @brief  yyyy/MM/dd hh:mm:ss の形式で現在日時を取得する
	* @retval 現在日時（yyyy/MM/dd hh:mm:ss）
**/
CString CGetTime::GetStrTimeSlCo()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換
	CString cs;
	cs.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

/**
	* @brief  yyyyMMdd の形式で現在日付を取得する
	* @retval 現在日時（yyyyMMdd）
**/
CString CGetTime::GetStrDate()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換
	CString cs;
	cs.Format(_T("%04d%02d%02d"), (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return cs;
}
