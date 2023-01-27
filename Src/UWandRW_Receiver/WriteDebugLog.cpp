/**
	* @file		WriteDebugLog.cpp
	* @brief	デバッグ用ログの出力
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
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
	* @brief CWriteDebugLog コンストラクション
**/
CWriteDebugLog::CWriteDebugLog()
	: m_Level( 0 )
{
	::InitializeCriticalSection(&m_cs);
}

/**
	* @brief CWriteDebugLog デストラクション
**/
CWriteDebugLog::~CWriteDebugLog()
{
	::DeleteCriticalSection(&m_cs);
}

/**
	* @brief 初期処理
	* @param inFileName ファイル名
**/
void CWriteDebugLog::Initialize( const CString& inFileName )
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	// iniファイルよりデバッグレベルを取得
	CIni_UwRwManager_work iniUwRwManager_work;
	iniUwRwManager_work.Initialize(TRUE);
	m_Level = iniUwRwManager_work.getDebugLogLevel();

	if ( m_Level == 0 )
	{	// ログの出力は行わない
		return;
	}

	// ログ出力日付
	m_strFileName = inFileName;

	// ログ出力ファイル
	CString OutputFolder = CCommon::GetModuleDirectory() + "\\Log";
	if ( !::PathFileExists( OutputFolder ) )
	{	// 存在しない場合はフォルダを作成する
		::SHCreateDirectory( NULL, A2W(OutputFolder.GetString()) );
	}
	// ログファイルパス
	m_strOutputPath = OutputFolder + "\\" + m_strFileName;
}

/**
	* @brief 初期処理（XML受信用）
	* @param inFileName ファイルパス名
**/
void CWriteDebugLog::InitializeXml( const CString& inFileName, const char* inDeviceName)
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	// iniファイルよりデバッグレベルを取得
	CIni_UwRwManager_work iniUwRwManager_work;
	iniUwRwManager_work.Initialize(TRUE);
	m_Level = iniUwRwManager_work.getDebugLogLevel();

	if ( m_Level == 0 )
	{	// ログの出力は行わない
		return;
	}

	// ログ出力日付
	m_strFileName = inFileName;

	// ログ出力ファイル
	CString OutputFolder = CCommon::GetModuleDirectory() + "\\Log\\" + inDeviceName + "_CONNECT\\" + CGetTime::GetStrDate() + ".xml";
	if ( !::PathFileExists( OutputFolder ) )
	{	// 存在しない場合はフォルダを作成する
		::SHCreateDirectory( NULL, A2W(OutputFolder.GetString()) );
	}
	// ログファイルパス
	m_strOutputPath = OutputFolder + "\\" + m_strFileName;
}

/**
	* @brief デバッグ用ログを出力
	* @param inStrLogInfo ログに書き出す文字列
	* @param inDebugLevel デバッグ情報のLevel
	* @param inTime       TRUE=日時出力あり、FALSE=日時出力なし
*/ 
void CWriteDebugLog::WriteDebuglog( const CString& inStrLogInfo, E_LOG_LEVEL inDebugLevel, BOOL inTime )
{
	// ログ出力排他制御
	CCriticalSectionLocker locker(&m_cs);

	// 確認用Log出力
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
