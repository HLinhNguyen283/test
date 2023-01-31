/**
 * @file  GetTime.cpp
 * @brief 現在日時取得
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "GetTime.h"
#include <time.h>
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------
//		CString GetStrTimeCo
// --------------------------------------------------------------------------
// 2009:12:21:34 の形で日時をCString型で取得する
CString CGetTime::GetStrTimeCo()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>2009:07:27:11:30:12
	CString cs;
	cs.Format(_T("%04d:%02d:%02d:%02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrTime
// --------------------------------------------------------------------------
// 2009122134 の形で日時をCString型で取得する
CString CGetTime::GetStrTime()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>20090727113012
	CString cs;
	cs.Format(_T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		int GetIntTime
// --------------------------------------------------------------------------
// 2009122134 の形で日時をint型で取得する
int CGetTime::GetIntTime()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>20090727113012
	CString Today;
	Today.Format(_T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	int time = atoi(Today.GetString());

	return time;
}

// --------------------------------------------------------------------------
//		CString GetStrTimeSla
// --------------------------------------------------------------------------
// 2009/12/21/34/45/47 の形で日時を取得する
CString CGetTime::GetStrTimeSla()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>2009/07/27/11/30/12
	CString cs;
	cs.Format(_T("%04d/%02d/%02d/%02d/%02d/%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrTimeSlCo
// --------------------------------------------------------------------------
// CString : 2009/12/21 04:45:47 の形で日時を取得する
CString CGetTime::GetStrTimeSlCo()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>2009:07:27 11:30:12
	CString cs;
	cs.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrDateCo
// --------------------------------------------------------------------------
// 2009:12:21 の形で日付をCString型で取得する
CString CGetTime::GetStrDateCo()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>2009:07:27
	CString cs;
	cs.Format(_T("%04d:%02d:%02d"), st.wYear, st.wMonth, st.wDay);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrDate
// --------------------------------------------------------------------------
// 20091221 の形で日付をCString型で取得する
CString CGetTime::GetStrDate()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>20090727
	CString cs;
	cs.Format(_T("%04d%02d%02d"), (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return cs;
}

// --------------------------------------------------------------------------
//		int GetIntDate
// --------------------------------------------------------------------------
// 20091221 の形で日付をint型で取得する
int CGetTime::GetIntDate()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>20090727113012
	CString Today;
	Today.Format(_T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
	int time = atoi(Today.GetString());

	return time;
}

// --------------------------------------------------------------------------
//		CString GetStrDateSla
// --------------------------------------------------------------------------
// 2009/12/21 の形で日付を取得する
CString CGetTime::GetStrDateSla()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>2009/07/27
	CString cs;
	cs.Format(_T("%04d/%02d/%02d"), st.wYear, st.wMonth, st.wDay);

	return cs;
}