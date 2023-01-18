/**
 * @file  OutputLog.cpp
 * @brief ���O�o�̓N���X �����t�@�C��
 * @date  20190610 CEC Miyachi �V�K�쐬
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
	* @brief COutputLog �R���X�g���N�V����
**/
/* ==================================================================================================== */
COutputLog::COutputLog()
{
	::InitializeCriticalSection(&m_cs);
	::InitializeCriticalSection(&m_cs_http);
}

/* ==================================================================================================== */
/**
	* @brief COutputLog �f�X�g���N�V����
**/
/* ==================================================================================================== */
COutputLog::~COutputLog()
{
	::DeleteCriticalSection(&m_cs);
	::DeleteCriticalSection(&m_cs_http);
}

/* ==================================================================================================== */
/**
	* @brief  ���O�o�͏��̏����ݒ���s��
	* @details (���O�o�̓t�@�C�����̂����肷��)
	* @param  strFileName    : [i]���O�t�@�C���̐擪����
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
	* @brief  ���O�o�͊֐�(std::string)
	* @param  strLog       : [i]���O�o�͕���
	* @param  IsOutputTime : [i]�����o�͗L��(true�F����Afalse�F�Ȃ�)
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
	* @brief  ���O�o�͊֐�(std::wstring)
	* @param  wstrLog      : [i]���O�o�͕���
	* @param  IsOutputTime : [i]�����o�͗L��(true�F����Afalse�F�Ȃ�)
**/
/* ==================================================================================================== */
void COutputLog::Output( const std::wstring& wstrLog, bool IsOutputTime/*=true*/ )
{
	USES_CONVERSION;

	Output( W2A(wstrLog.c_str()), IsOutputTime );
}

/* ==================================================================================================== */
/**
	* @brief  ���ݎ����ҏW(�N���������b�{�~���b)
	* @retval �ҏW�ς݌��ݎ���
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
	* @brief  �X���b�h�h�c�ҏW
	* @retval �ҏW�ς݃X���b�h�h�c
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