/**
	* @file		WriteDebugLog.cpp
	* @brief	�f�o�b�O�p���O�̏o��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "GetTime.h"
#include "Ini_UwRwManager_work.h"
#include "CriticalSection.h"
#include "WriteDebugLog.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief CWriteDebugLog �R���X�g���N�V����
**/
CWriteDebugLog::CWriteDebugLog()
	: m_Level( 0 )
{
	::InitializeCriticalSection(&m_cs);
}

/**
	* @brief CWriteDebugLog �f�X�g���N�V����
**/
CWriteDebugLog::~CWriteDebugLog()
{
	::DeleteCriticalSection(&m_cs);
}

/**
	* @brief ��������
	* @param inFileName �t�@�C����
**/
void CWriteDebugLog::Initialize( const CString& inFileName )
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	// ini�t�@�C�����f�o�b�O���x�����擾
	CIni_UwRwManager_work iniUwRwManager_work;
	iniUwRwManager_work.Initialize(TRUE);
	m_Level = iniUwRwManager_work.getDebugLogLevel();

	if ( m_Level == 0 )
	{	// ���O�̏o�͍͂s��Ȃ�
		return;
	}

	// ���O�o�͓��t
	m_strFileName = inFileName;

	// ���O�o�̓t�@�C��
	CString OutputFolder = CCommon::GetModuleDirectory() + "\\Log";
	if ( !::PathFileExists( OutputFolder ) )
	{	// ���݂��Ȃ��ꍇ�̓t�H���_���쐬����
		::SHCreateDirectory( NULL, A2W(OutputFolder.GetString()) );
	}
	// ���O�t�@�C���p�X
	m_strOutputPath = OutputFolder + "\\" + m_strFileName;
}

/**
	* @brief ���������iXML��M�p�j
	* @param inFileName �t�@�C���p�X��
**/
void CWriteDebugLog::InitializeXml( const CString& inFileName, const char* inDeviceName)
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	// ini�t�@�C�����f�o�b�O���x�����擾
	CIni_UwRwManager_work iniUwRwManager_work;
	iniUwRwManager_work.Initialize(TRUE);
	m_Level = iniUwRwManager_work.getDebugLogLevel();

	if ( m_Level == 0 )
	{	// ���O�̏o�͍͂s��Ȃ�
		return;
	}

	// ���O�o�͓��t
	m_strFileName = inFileName;

	// ���O�o�̓t�@�C��
	CString OutputFolder = CCommon::GetModuleDirectory() + "\\Log\\" + inDeviceName + "_CONNECT\\" + CGetTime::GetStrDate() + ".xml";
	if ( !::PathFileExists( OutputFolder ) )
	{	// ���݂��Ȃ��ꍇ�̓t�H���_���쐬����
		::SHCreateDirectory( NULL, A2W(OutputFolder.GetString()) );
	}
	// ���O�t�@�C���p�X
	m_strOutputPath = OutputFolder + "\\" + m_strFileName;
}

/**
	* @brief �f�o�b�O�p���O���o��
	* @param inStrLogInfo ���O�ɏ����o��������
	* @param inDebugLevel �f�o�b�O����Level
	* @param inTime       TRUE=�����o�͂���AFALSE=�����o�͂Ȃ�
*/ 
void CWriteDebugLog::WriteDebuglog( const CString& inStrLogInfo, E_LOG_LEVEL inDebugLevel, BOOL inTime )
{
	// ���O�o�͔r������
	CCriticalSectionLocker locker(&m_cs);

	// �m�F�pLog�o��
	if(m_Level > 0 && inDebugLevel <= m_Level)
	{
		FILE* fpTest = NULL;
		fopen_s(&fpTest, m_strOutputPath, _T("a"));
		if ( fpTest )
		{
			if ( inTime )
			{	// 
				fprintf(fpTest, _T("%s %s\n"), (LPCTSTR)CGetTime::GetStrTimeSlCo(), (LPCTSTR)inStrLogInfo);
			}
			else
			{
				fprintf(fpTest, _T("%s\n"), (LPCTSTR)inStrLogInfo);
			}
			fclose(fpTest);
		}
	}
}
