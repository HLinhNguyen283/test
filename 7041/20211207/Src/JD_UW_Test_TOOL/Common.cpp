/**
 * @file  Common.cpp
 * @brief 共通処理
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
// ===========================================================================

/* ==================================================================================================== */
/**
	* @brief  モジュールのカレントフォルダを取得する
	* @retval モジュールのカレントフォルダ
**/
/* ==================================================================================================== */
CString CCommon::GetCurrentDirectory()
{
	TCHAR szDirectoryName[MAX_PATH];
	::GetCurrentDirectory( sizeof(szDirectoryName)/sizeof(szDirectoryName[0]), szDirectoryName);

	return CString( szDirectoryName );
}

/* ==================================================================================================== */
/**
	* @brief  ツールのiniファイル(JD_UW_Test_TOOL.ini)パスを取得
	* @retval ツールのiniファイルパス
**/
/* ==================================================================================================== */
CString CCommon::GetToolIniPath()
{
	CString s =  GetCurrentDirectory() + _T("\\") + TEST_TOOL_INI;
	return s;
}

/* ==================================================================================================== */
/**
	* @brief  ツールのワークiniファイル(JD_UW_Test_WORK.ini)パスを取得
	*　　　　 JD_UW_Test_WORK.iniは本ツールが作成するINIファイル
	* @retval ツールのワークiniファイルパス
**/
/* ==================================================================================================== */
CString CCommon::GetWorkIniPath()
{
	return GetCurrentDirectory() + _T("\\") + TEST_WORK_INI;
}

/* ==================================================================================================== */
/**
	* @brief  指定ファイルをメモリに読み込む
	* @param  inFilePath	: [i]ファイルパス
	* @param  outData		: [o]ファイルの内容
	* @retval TRUE=正常、FALSE=異常
**/
/* ==================================================================================================== */
BOOL CCommon::ReadFile( const CString& inFilePath, CString& outData )
{
	BOOL bRet = FALSE;
	TCHAR *pcBuffer = NULL;

	HANDLE hFile = CreateFile( inFilePath, FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwSize = GetFileSize( hFile, NULL );	//上位DWORDは考慮しない
		if ( dwSize != -1 )
		{
			//ファイルを読み込むバッファを確保
			pcBuffer = new TCHAR[dwSize + 1];
			ZeroMemory( pcBuffer, dwSize + 1 );
			//ファイル読み込み
			DWORD dwRead, dwTotal = 0;
			while ( dwTotal != dwSize )
			{
				BOOL bRet = ::ReadFile( hFile, &pcBuffer[dwTotal], dwSize, &dwRead, NULL );
				if ( bRet == 0 )
				{	// 読み込み終了
					break;
				}
				dwTotal += dwRead;	// 読み取りバイト総計を加算
			}
			if ( dwTotal == dwSize )
			{
				// 読み込み正常終了
				outData.Append( pcBuffer );
				bRet = TRUE;
			}
		}
	}
	if ( pcBuffer )
	{
		// メモリ開放
		delete [] pcBuffer;
	}
	CloseHandle( hFile );

	return bRet;
}

/* ==================================================================================================== */
/**
	* @brief  現在時刻をゾーン形式で戻す
	* @retval ゾーン形式の現在時刻
**/
/* ==================================================================================================== */
CString CCommon::CreateTimeStamp()
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);

	CString Bias, TimeStamp;
	if (tzInfo.Bias > 0 ) Bias = _T("-");
	else Bias = _T("+");
	TimeStamp.Format( _T("%04d-%02d-%02dT%02d:%02d:%02d%s%02d:%02d"),
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond,
			Bias.GetString(), abs(tzInfo.Bias) / 60, abs(tzInfo.Bias) % 60);

	return TimeStamp;
}
