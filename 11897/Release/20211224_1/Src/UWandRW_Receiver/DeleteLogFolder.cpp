/**
	* @file		DeleteLogFolder.cpp
	* @brief	�Â����O�t�H���_�폜
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <vector>
#include <sstream>
#include "WriteDebugLog.h"
#include "DeleteLogFolder.h"
// ===========================================================================

/**
	* @brief  CDeleteLogFolder �R���X�g���N�V����
**/
CDeleteLogFolder::CDeleteLogFolder(CWriteDebugLog* inWriteDebugLog)
	: m_pWriteDebugLog(inWriteDebugLog)
{
}

/**
	* @brief  CDeleteLogFolder �f�X�g���N�V����
**/
CDeleteLogFolder::~CDeleteLogFolder()
{
}

/**
	* @brief  �ۑ����Ԃ��o�߂������O�t�H���_���폜����
	* @param  inFolderPath	: [i]�g�b�v�t�H���_�p�X
	* @param  inPeriodDate	: [i]�ۑ�����
	* @retval TRUE=����AFALSE=�ُ�
**/
BOOL CDeleteLogFolder::Doit( const CString& inFolderPath, UINT inPeriodDate )
{
	std::vector<CString> FolderList = GetFolderList(inFolderPath, "*");

	// ���O�ۑ��̍ŌÂ̓��t���Z�o
	CTime t = CTime::GetCurrentTime() + CTimeSpan( inPeriodDate * -1, 0, 0, 0 );
	CString OldDate;
	OldDate.Format("%04d%02d%02d.xml",t.GetYear(),t.GetMonth(),t.GetDay());

	std::vector<CString> DeleteFolderList;
	for( auto ite = FolderList.begin(); ite < FolderList.end(); ite++ )
	{
		if ( *ite < OldDate )
		{	// �폜�Ώ�
			DeleteFolderList.push_back(*ite);
		}
	}

	DeleteFoldertoList(inFolderPath, DeleteFolderList);

	return TRUE;
}

/**
	* @brief  �w��t�H���_���̃t�H���_�ꗗ���擾����
	* @param  inFolderPath   : [i]�w��t�H���_
	* @param  inSearchFolder : [i]�w��t�H���_�p�^�[���i��F"*"�j
	* @retval �t�H���_���̈ꗗ
**/
std::vector<CString> CDeleteLogFolder::GetFolderList( const CString& inFolderPath, const CString& inSearchFolder )
{
	std::vector<CString> FolderList;

	std::string strFindSearchPath( inFolderPath + "\\" + inSearchFolder );
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;

	hFind = ::FindFirstFile( strFindSearchPath.c_str(), &fd );
	if ( hFind == INVALID_HANDLE_VALUE )
	{	// �Ώۃt�H���_���Ȃ�
		return FolderList;
	}

	do {
		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{	// �t�H���_�̏ꍇ
			if ( ::strcmp( ".", fd.cFileName ) != 0 &&
				::strcmp( "..", fd.cFileName ) != 0 )
			{	// �J�����g�t�H���_�Ɛe�t�H���_�ȊO
				FolderList.push_back( fd.cFileName );
			}
		}
	} while ( ::FindNextFile( hFind, &fd ) );

	::FindClose( hFind );

	return FolderList;
}

/**
	* @brief  �t�H���_���X�g�̃t�H���_���폜����
	* @param  inFolderPath		: [i]�g�b�v�t�H���_�p�X
	* @param  inDeleteFolderList: [i]�폜�t�H���_���̃��X�g
**/
void CDeleteLogFolder::DeleteFoldertoList( const CString& inFolderPath, const std::vector<CString>& inDeleteFolderList )
{
	BOOL nDeleteFolder = FALSE;
	for( auto ite = inDeleteFolderList.begin(); ite < inDeleteFolderList.end(); ite++ )
	{
		CString FolderPath = inFolderPath + *ite;
		if ( DeleteFolderOnes(FolderPath.GetString()) )
		{	// ����
			CString Logmessage = "Info. Delete Folder " + FolderPath;
			m_pWriteDebugLog->WriteDebuglog(Logmessage, CWriteDebugLog::E_LOG_LEVEL_1);
			nDeleteFolder = TRUE;
		}
		else
		{	// �ُ�
			CString Logmessage = "ERROR Delete Folder " + FolderPath;
			m_pWriteDebugLog->WriteDebuglog(Logmessage, CWriteDebugLog::E_LOG_LEVEL_1);
			nDeleteFolder = TRUE;
		}
	}
	if ( !nDeleteFolder )
	{
		m_pWriteDebugLog->WriteDebuglog("Info. Delete Folder no there", CWriteDebugLog::E_LOG_LEVEL_1);
	}
}

/**
	* @brief  �w��t�H���_���폜����
	* @param  inFolderPath : [i]�폜�t�H���_
	* @retval TRUE=����AFALSE=�ُ�
**/
BOOL CDeleteLogFolder::DeleteFolderOnes( const std::string& inFolderPath )
{
//	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	std::string strFindSearchPath( inFolderPath + "\\" + "*" );
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;

	hFind = ::FindFirstFile( strFindSearchPath.c_str(), &fd );
	if ( hFind == INVALID_HANDLE_VALUE )
	{	// �Ώۃt�H���_���Ȃ�
		return TRUE;
	}

	do {
		if ( ::strcmp( ".", fd.cFileName ) != 0 &&
			::strcmp( "..", fd.cFileName ) != 0 )
		{	// �J�����g�t�H���_�Ɛe�t�H���_�ȊO
			std::string DeletePath = inFolderPath + "\\" + fd.cFileName;
			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	// ���ʃt�H���_(�t�H���_���̃t�@�C���ƃt�H���_���܂�)���폜
				if ( !DeleteFolderOnes( DeletePath.c_str() ) )
				{	// �폜�G���[
					::FindClose( hFind );
					return FALSE;
				}
			}
			else
			{	// �t�@�C�����폜
				if ( !DeleteFile( DeletePath.c_str()) )
				{	// �폜�G���[
					::FindClose( hFind );
					return FALSE;
				}
			}
		}
	} while ( ::FindNextFile( hFind, &fd ) );

	::FindClose( hFind );

	// �t�H���_���폜
	return RemoveDirectory( inFolderPath.c_str() );
}

