/**
 * @file  WriteDebugLog.cpp
 * @brief デバッグ用ログの出力
 * @author screen
 * @date  20210720 screen 新規作成
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
	* @brief CWriteDebugLog コンストラクション
**/
/* ==================================================================================================== */
CWriteDebugLog::CWriteDebugLog()
	: m_Level( 0 )
{
	::InitializeCriticalSection(&m_cs);
}

/* ==================================================================================================== */
/**
	* @brief CWriteDebugLog デストラクション
**/
/* ==================================================================================================== */
CWriteDebugLog::~CWriteDebugLog()
{
	::DeleteCriticalSection(&m_cs);
}

/* ==================================================================================================== */
/**
	* @brief 初期処理
	* @param InFileName ファイルパス
**/
/* ==================================================================================================== */
void CWriteDebugLog::Initialize( const CString& InFileName )
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	// iniファイルよりデバッグレベルを取得
	m_Level = CGetIniFileParam::GetDebugLogLevel();
	if ( m_Level == 0 )
	{	// ログの出力は行わない
		return;
	}
	// iniファイルよりログの出力先フォルダの取得
	std::wstring wstrOutputPath;

	wstrOutputPath = A2W(CGetIniFileParam::GetOutputLogPath().GetString());
	if ( wstrOutputPath.empty() )
	{	// 空の場合はカレント配下のデフォルトフォルダにする
		wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
		wstrOutputPath.append(L"\\");
		wstrOutputPath.append(A2W(D_DEFAULT_FOLDER_NAME));
	}
	if ( !::PathFileExists( W2A(wstrOutputPath.c_str()) ) )
	{	// 存在しない場合はフォルダを作成する
		int nRet = ::SHCreateDirectory( NULL, wstrOutputPath.c_str() );
		if ( nRet != ERROR_SUCCESS )
		{	// 作成できない場合はカレント配下のデフォルトフォルダに切り替える
			wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
			wstrOutputPath.append(L"\\");
			wstrOutputPath.append(A2W(D_DEFAULT_FOLDER_NAME));
			nRet = ::SHCreateDirectory( NULL, wstrOutputPath.c_str() );
			if ( nRet != ERROR_SUCCESS )
			{	// 作成できない場合はカレントに切り替える
				wstrOutputPath = A2W(CCommon::GetCurrentDirectory().GetString());
			}
		}
	}
	// 保存先　パス、ファイル名
	wstrOutputPath.append(L"\\");
	wstrOutputPath.append(A2W(InFileName.GetString()));

	m_strOutputPath = W2A(wstrOutputPath.c_str());
}

/* ==================================================================================================== */
/**
	* @brief デバッグ用ログを出力
	* @param InStrLogInfo ログに書き出す文字列
	* @param InDebugLevel デバッグ情報のLevel
	* @param InTime       TRUE=日時出力あり、FALSE=日時出力なし
*/ 
/* ==================================================================================================== */
void CWriteDebugLog::WriteDebuglog( const CString& InStrLogInfo, E_LOG_LEVEL InDebugLevel, BOOL InTime )
{
	// ログ出力排他制御
	CCriticalSectionLocker locker(&m_cs);

	// 確認用Log出力
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