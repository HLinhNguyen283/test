/**
	* @file		XmlLogData.cpp
	* @brief	XMLログ関連モジュール
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "XmlLogData.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  XML送信ログファイルパス取得
	* @retval XML送信ログファイルパス
**/
std::string CXmlLogData::GetSendLogFilePath()
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	std::string LogFolderPath = GetLogFolderPath();
	if ( !::PathFileExists( LogFolderPath.c_str() ) )
	{	// 存在しない場合はフォルダを作成する
		::SHCreateDirectory( NULL, A2W(LogFolderPath.c_str()) );
	}
	std::string LogFilePath = GetLogFilePath("Send");
	if ( ::PathFileExists( LogFilePath.c_str() ) )
	{	// 存在する場合は１ミリ待って再度ファイルパスを作成する
		Sleep(1);
		LogFilePath = GetLogFilePath("Send");
	}
	return LogFilePath;
}

/**
	* @brief  XMLレスポンスログファイルパス取得
	* @retval XMLレスポンスログファイルパス
**/
std::string CXmlLogData::GetResponseLogFilePath()
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	std::string LogFolderPath = GetLogFolderPath();
	if ( !::PathFileExists( LogFolderPath.c_str() ) )
	{	// 存在しない場合はフォルダを作成する
		::SHCreateDirectory( NULL, A2W(LogFolderPath.c_str()) );
	}
	std::string LogFilePath = GetLogFilePath("Response");
	if ( ::PathFileExists( LogFilePath.c_str() ) )
	{	// 存在する場合は１ミリ待って再度ファイルパスを作成する
		Sleep(1);
		LogFilePath = GetLogFilePath("Send");
	}
	return LogFilePath;
}

/**
	* @brief  指定ファイルを指定メモリに読み込む
	* @param  inFilePath	: [i]ファイルパス
	* @param  outData		: [o]ファイルの内容
	* @retval TRUE=正常、FALSE=異常
**/
BOOL CXmlLogData::ReadFile( const std::string& inFilePath, std::string& outData )
{
	BOOL nRet = FALSE;
	char *pcBuffer = NULL;

	HANDLE hFile = CreateFile( inFilePath.c_str(), FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
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
				nRet = TRUE;
			}
		}
	}
	if ( pcBuffer )
	{
		outData.append(pcBuffer);
		// メモリ開放
		delete [] pcBuffer;
	}
	CloseHandle( hFile );

	return nRet;
}

/**
	* @brief  指定ファイルに指定内容を書き込む
	* @param  inFilePath	: [i]ファイルパス
	* @param  inData		: [i]書き込みデータ
	* @retval TRUE=正常、FALSE=異常
**/
BOOL CXmlLogData::WriteFile( const std::string& inFilePath, std::string& inData )
{
	BOOL nRet = FALSE;

	HANDLE hFile = CreateFile( inFilePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwWrittenSize;
		if ( ::WriteFile( hFile, inData.c_str() , (DWORD)inData.length(), &dwWrittenSize, NULL ) )
		{	// 書き込み正常
			nRet = TRUE;
		}
		CloseHandle( hFile );
	}

	return nRet;
}

/**
	* @brief  モジュールフォルダを取得する
	* @retval モジュールフォルダ
**/
std::string CXmlLogData::GetModuleDirectory()
{
	TCHAR szFullPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	std::string s(szFullPath);
	size_t pos = s.rfind("\\");

	return s.substr(0, pos);
}

/**
	* @brief  現在日のXMLログフォルダを取得する
	* @retval 現在日のXMLログフォルダ
**/
std::string CXmlLogData::GetLogFolderPath()
{
	std::string LogFolderPath = GetModuleDirectory();
	LogFolderPath.append("\\LOG\\UW_CONNECT\\");
	LogFolderPath.append(GetStrDate());
	LogFolderPath.append(".xml");
	return LogFolderPath;
}

/**
	* @brief  指定のXMLログファイルパスを取得する
	* @param  inName : [i]ファイルの接頭文字
	* @retval 指定のXMLログファイルパス
**/
std::string CXmlLogData::GetLogFilePath(const std::string& inName)
{
	std::string LogFilePath = GetLogFolderPath();
	LogFilePath.append("\\");
	LogFilePath.append(inName);
	LogFilePath.append("_");
	LogFilePath.append(GetStrDateTime());
	LogFilePath.append(".xml");
	return LogFilePath;
}

/**
	* @brief  現在日付を取得する
	* @retval 現在日付(yyyyMMdd)
**/
std::string CXmlLogData::GetStrDate()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 日付をを文字列に変換
	char buf[64];
	_snprintf_s( buf, sizeof(buf) -1 , "%04d%02d%02d", st.wYear, st.wMonth, st.wDay );

	return std::string(buf);
}

/**
	* @brief  現在日時(＋ミリ秒)を取得する
	* @retval 現在日時(yyyyMMddhhmmss_999)
**/
std::string CXmlLogData::GetStrDateTime()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 日付と時刻をを文字列に変換
	char buf[64];
	_snprintf_s( buf, sizeof(buf) -1 , "%04d%02d%02d%02d%02d%02d_%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

	return std::string(buf);
}

};	// namespace unwinder_manager