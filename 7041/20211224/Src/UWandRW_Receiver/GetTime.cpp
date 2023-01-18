/**
	* @file		GetTime.cpp
	* @brief	���ݓ����擾
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
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
	* @brief  yyyyMMddhhmmss_999 �̌`���Ō��ݓ������擾����
	* @retval ���ݓ����iyyyyMMddhhmmss_999�j
**/
CString CGetTime::GetStrTimeMM()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�
	CString cs;
	cs.Format(_T("%04d%02d%02d%02d%02d%02d_%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return cs;
}

/**
	* @brief  yyyy/MM/dd hh:mm:ss �̌`���Ō��ݓ������擾����
	* @retval ���ݓ����iyyyy/MM/dd hh:mm:ss�j
**/
CString CGetTime::GetStrTimeSlCo()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�
	CString cs;
	cs.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return cs;
}

/**
	* @brief  yyyyMMdd �̌`���Ō��ݓ��t���擾����
	* @retval ���ݓ����iyyyyMMdd�j
**/
CString CGetTime::GetStrDate()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�
	CString cs;
	cs.Format(_T("%04d%02d%02d"), (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return cs;
}
