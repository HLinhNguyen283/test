/**
 * @file  WriteDebugLog.cpp
 * @brief �f�o�b�O�p���O�̏o��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "GetTime.h"
#include "GetIniFileParam.h"
#include "CriticalSection.h"
#include "WriteDebugLog.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief CWriteDebugLog �R���X�g���N�V����
**/
/* ==================================================================================================== */
CWriteDebugLog::CWriteDebugLog()
	: m_Level( 0 )
{
	::InitializeCriticalSection(&m_cs);
}

/* ==================================================================================================== */
/**
	* @brief CWriteDebugLog �f�X�g���N�V����
**/
/* ==================================================================================================== */
CWriteDebugLog::~CWriteDebugLog()
{
	::DeleteCriticalSection(&m_cs);
}

/* ==================================================================================================== */
/**
	* @brief ��������
	* @param InFileName �t�@�C���p�X
**/
/* ==================================================================================================== */
void CWriteDebugLog::Initialize( const CString& InFileName )
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	// ini�t�@�C�����f�o�b�O���x�����擾
	m_Level = CGetIniFileParam::GetDebugLogLevel();
	if ( m_Level == 0 )
	{	// ���O�̏o�͍͂s��Ȃ�
		return;
	}
	// ini�t�@�C����胍�O�̏o�͐�t�H���_�̎擾
	std::wstring wstrOutputPath;

	wstrOutputPath = A2W(CGetIniFileParam::GetOutputLogPath().GetString());
	if ( wstrOutputPath.empty() )
	{	// ��̏ꍇ�̓J�����g�z���̃f�t�H���g�t�H���_�ɂ���
		wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
		wstrOutputPath.append(L"\\");
		wstrOutputPath.append(A2W(D_DEFAULT_FOLDER_NAME));
	}
	if ( !::PathFileExists( W2A(wstrOutputPath.c_str()) ) )
	{	// ���݂��Ȃ��ꍇ�̓t�H���_���쐬����
		int nRet = ::SHCreateDirectory( NULL, wstrOutputPath.c_str() );
		if ( nRet != ERROR_SUCCESS )
		{	// �쐬�ł��Ȃ��ꍇ�̓J�����g�z���̃f�t�H���g�t�H���_�ɐ؂�ւ���
			wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
			wstrOutputPath.append(L"\\");
			wstrOutputPath.append(A2W(D_DEFAULT_FOLDER_NAME));
			nRet = ::SHCreateDirectory( NULL, wstrOutputPath.c_str() );
			if ( nRet != ERROR_SUCCESS )
			{	// �쐬�ł��Ȃ��ꍇ�̓J�����g�ɐ؂�ւ���
				wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
			}
		}
	}
	// �ۑ���@�p�X�A�t�@�C����
	wstrOutputPath.append(L"\\");
	wstrOutputPath.append(A2W(InFileName.GetString()));

	m_strOutputPath = W2A(wstrOutputPath.c_str());
}

/* ==================================================================================================== */
/**
	* @brief �f�o�b�O�p���O���o��
	* @param InStrLogInfo ���O�ɏ����o��������
	* @param InDebugLevel �f�o�b�O����Level
	* @param InTime       TRUE=�����o�͂���AFALSE=�����o�͂Ȃ�
*/ 
/* ==================================================================================================== */
void CWriteDebugLog::WriteDebuglog( const CString& InStrLogInfo, E_LOG_LEVEL InDebugLevel, BOOL InTime )
{
	// ���O�o�͔r������
	CCriticalSectionLocker locker(&m_cs);

	// �m�F�pLog�o��
	if(m_Level > 0 && InDebugLevel <= m_Level){
		FILE* fpTest = NULL;
		fopen_s(&fpTest, m_strOutputPath, _T("a"));
		if ( fpTest )
		{
			if ( InTime )
			{	// 
				fprintf(fpTest, _T("%s %s\n"), CGetTime::GetStrTimeSlCo(), InStrLogInfo);
			}
			else
			{
				fprintf(fpTest, _T("%s\n"), InStrLogInfo);
			}
			fclose(fpTest);
		}
	}
}