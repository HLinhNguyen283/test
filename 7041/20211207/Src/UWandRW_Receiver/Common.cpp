/**
	* @file		Common.cpp
	* @brief	共通処理
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
 **/

// ===========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
// ===========================================================================

/**
	* @brief  モジュールフォルダを取得する
	* @retval モジュールフォルダ
**/
CString CCommon::GetModuleDirectory()
{
	TCHAR szFullPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	std::string s(szFullPath);
	size_t pos = s.rfind("\\");

	return s.substr(0, pos).c_str();
}

/**
	* @brief  指定ファイルをメモリに読み込む
	* @param  inFilePath	: [i]ファイルパス
	* @param  outData		: [o]ファイルの内容
	* @retval TRUE=正常、FALSE=異常
**/
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
