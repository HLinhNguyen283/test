/**
 * @file  GetTime.cpp
 * @brief ���ݓ����擾
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
// 2009:12:21:34 �̌`�œ�����CString�^�Ŏ擾����
CString CGetTime::GetStrTimeCo()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>2009:07:27:11:30:12
	CString cs;
	cs.Format(_T("%04d:%02d:%02d:%02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrTime
// --------------------------------------------------------------------------
// 2009122134 �̌`�œ�����CString�^�Ŏ擾����
CString CGetTime::GetStrTime()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>20090727113012
	CString cs;
	cs.Format(_T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		int GetIntTime
// --------------------------------------------------------------------------
// 2009122134 �̌`�œ�����int�^�Ŏ擾����
int CGetTime::GetIntTime()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>20090727113012
	CString Today;
	Today.Format(_T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	int time = atoi(Today.GetString());

	return time;
}

// --------------------------------------------------------------------------
//		CString GetStrTimeSla
// --------------------------------------------------------------------------
// 2009/12/21/34/45/47 �̌`�œ������擾����
CString CGetTime::GetStrTimeSla()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>2009/07/27/11/30/12
	CString cs;
	cs.Format(_T("%04d/%02d/%02d/%02d/%02d/%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrTimeSlCo
// --------------------------------------------------------------------------
// CString : 2009/12/21 04:45:47 �̌`�œ������擾����
CString CGetTime::GetStrTimeSlCo()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>2009:07:27 11:30:12
	CString cs;
	cs.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrDateCo
// --------------------------------------------------------------------------
// 2009:12:21 �̌`�œ��t��CString�^�Ŏ擾����
CString CGetTime::GetStrDateCo()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>2009:07:27
	CString cs;
	cs.Format(_T("%04d:%02d:%02d"), st.wYear, st.wMonth, st.wDay);

	return cs;
}

// --------------------------------------------------------------------------
//		CString GetStrDate
// --------------------------------------------------------------------------
// 20091221 �̌`�œ��t��CString�^�Ŏ擾����
CString CGetTime::GetStrDate()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>20090727
	CString cs;
	cs.Format(_T("%04d%02d%02d"), (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return cs;
}

// --------------------------------------------------------------------------
//		int GetIntDate
// --------------------------------------------------------------------------
// 20091221 �̌`�œ��t��int�^�Ŏ擾����
int CGetTime::GetIntDate()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>20090727113012
	CString Today;
	Today.Format(_T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
	int time = atoi(Today.GetString());

	return time;
}

// --------------------------------------------------------------------------
//		CString GetStrDateSla
// --------------------------------------------------------------------------
// 2009/12/21 �̌`�œ��t���擾����
CString CGetTime::GetStrDateSla()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>2009/07/27
	CString cs;
	cs.Format(_T("%04d/%02d/%02d"), st.wYear, st.wMonth, st.wDay);

	return cs;
}