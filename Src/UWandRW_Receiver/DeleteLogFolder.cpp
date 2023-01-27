/**
	* @file		DeleteLogFolder.cpp
	* @brief	古いログフォルダ削除
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
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
	* @brief  CDeleteLogFolder コンストラクション
**/
CDeleteLogFolder::CDeleteLogFolder(CWriteDebugLog* inWriteDebugLog)
	: m_pWriteDebugLog(inWriteDebugLog)
{
}

/**
	* @brief  CDeleteLogFolder デストラクション
**/
CDeleteLogFolder::~CDeleteLogFolder()
{
}

/**
	* @brief  保存期間を経過したログフォルダを削除する
	* @param  inFolderPath	: [i]トップフォルダパス
	* @param  inPeriodDate	: [i]保存期間
	* @retval TRUE=正常、FALSE=異常
**/
BOOL CDeleteLogFolder::Doit( const CString& inFolderPath, UINT inPeriodDate )
{
	std::vector<CString> FolderList = GetFolderList(inFolderPath, "*");

	// ログ保存の最古の日付を算出
	CTime t = CTime::GetCurrentTime() + CTimeSpan( inPeriodDate * -1, 0, 0, 0 );
	CString OldDate;
	OldDate.Format("%04d%02d%02d.xml",t.GetYear(),t.GetMonth(),t.GetDay());

	std::vector<CString> DeleteFolderList;
	for( auto ite = FolderList.begin(); ite < FolderList.end(); ite++ )
	{
		if ( *ite < OldDate )
		{	// 削除対象
			DeleteFolderList.push_back(*ite);
		}
	}

	DeleteFoldertoList(inFolderPath, DeleteFolderList);

	return TRUE;
}

/**
	* @brief  指定フォルダ下のフォルダ一覧を取得する
	* @param  inFolderPath   : [i]指定フォルダ
	* @param  inSearchFolder : [i]指定フォルダパターン（例："*"）
	* @retval フォルダ名称一覧
**/
std::vector<CString> CDeleteLogFolder::GetFolderList( const CString& inFolderPath, const CString& inSearchFolder )
{
	std::vector<CString> FolderList;

	std::string strFindSearchPath( inFolderPath + "\\" + inSearchFolder );
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;

	hFind = ::FindFirstFile( strFindSearchPath.c_str(), &fd );
	if ( hFind == INVALID_HANDLE_VALUE )
	{	// 対象フォルダがない
		return FolderList;
	}

	do {
		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{	// フォルダの場合
			if ( ::strcmp( ".", fd.cFileName ) != 0 &&
				::strcmp( "..", fd.cFileName ) != 0 )
			{	// カレントフォルダと親フォルダ以外
				FolderList.push_back( fd.cFileName );
			}
		}
	} while ( ::FindNextFile( hFind, &fd ) );

	::FindClose( hFind );

	return FolderList;
}

/**
	* @brief  フォルダリストのフォルダを削除する
	* @param  inFolderPath		: [i]トップフォルダパス
	* @param  inDeleteFolderList: [i]削除フォルダ名のリスト
**/
void CDeleteLogFolder::DeleteFoldertoList( const CString& inFolderPath, const std::vector<CString>& inDeleteFolderList )
{
	BOOL nDeleteFolder = FALSE;
	for( auto ite = inDeleteFolderList.begin(); ite < inDeleteFolderList.end(); ite++ )
	{
		CString FolderPath = inFolderPath + *ite;
		if ( DeleteFolderOnes(FolderPath.GetString()) )
		{	// 正常
			CString Logmessage = "Info. Delete Folder " + FolderPath;
			m_pWriteDebugLog->WriteDebuglog(Logmessage, CWriteDebugLog::E_LOG_LEVEL_1);
			nDeleteFolder = TRUE;
		}
		else
		{	// 異常
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
	* @brief  指定フォルダを削除する
	* @param  inFolderPath : [i]削除フォルダ
	* @retval TRUE=正常、FALSE=異常
**/
BOOL CDeleteLogFolder::DeleteFolderOnes( const std::string& inFolderPath )
{
//	USES_CONVERSION;				// A2W使用の場合の宣言

	std::string strFindSearchPath( inFolderPath + "\\" + "*" );
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;

	hFind = ::FindFirstFile( strFindSearchPath.c_str(), &fd );
	if ( hFind == INVALID_HANDLE_VALUE )
	{	// 対象フォルダがない
		return TRUE;
	}

	do {
		if ( ::strcmp( ".", fd.cFileName ) != 0 &&
			::strcmp( "..", fd.cFileName ) != 0 )
		{	// カレントフォルダと親フォルダ以外
			std::string DeletePath = inFolderPath + "\\" + fd.cFileName;
			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	// 下位フォルダ(フォルダ内のファイルとフォルダを含む)を削除
				if ( !DeleteFolderOnes( DeletePath.c_str() ) )
				{	// 削除エラー
					::FindClose( hFind );
					return FALSE;
				}
			}
			else
			{	// ファイルを削除
				if ( !DeleteFile( DeletePath.c_str()) )
				{	// 削除エラー
					::FindClose( hFind );
					return FALSE;
				}
			}
		}
	} while ( ::FindNextFile( hFind, &fd ) );

	::FindClose( hFind );

	// フォルダを削除
	return RemoveDirectory( inFolderPath.c_str() );
}

