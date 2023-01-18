/*! \file Ini_Template.cpp
 *  \brief Iniファイル操作共通クラス
 *  \author TGL Hamaguchi
 *  \date 2015/05/18 TGL Hamaguchi 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Ini_Template.h"
#include <stdio.h>
#include <windows.h>
#include "Crtdbg.h"
// ==========================================================================
// Local definitions

// ==========================================================================

/*! \brief コンストラクタ
*/
CIni_Template::CIni_Template()
{
	memset(m_filePath, 0x00, sizeof(m_filePath));
	m_mutex = NULL;
	memset(m_moduleDirectory, 0x00, sizeof(m_moduleDirectory));
}

/*! \brief デストラクタ
*/
CIni_Template::~CIni_Template()
{
	if(m_mutex){
		CloseHandle(m_mutex);
		m_mutex = NULL;
	}
}

/**
 * @brief	初期化
 * @retval	true : 成功
 * @retval	false : 失敗
**/
bool CIni_Template::Initialize(bool isSelf)
{
	if( m_mutex != NULL ){
		_ASSERTE(m_mutex == NULL);
		return false;
	}	
	//
	const char* mutexName = GetMutexName();
	m_mutex = OpenMutex(MUTEX_ALL_ACCESS, NULL, mutexName);
	if(!m_mutex){
		m_mutex = CreateMutex(NULL, FALSE, mutexName);
	}
	while(!m_mutex){
		m_mutex = OpenMutex(MUTEX_ALL_ACCESS, NULL, mutexName);
		Sleep(10);
	}
	//
	SetFilePath(isSelf);

	return true;
}

/*! \brief ファイルの有無チェック
 *  \return ファイルの有無
*/
bool CIni_Template::IsFile()
{
	EnterMutex();
	DWORD ret = GetFileAttributes(m_filePath);
	LeaveMutex();
	if ((long)ret == -1) return false;
	if (ret & FILE_ATTRIBUTE_DIRECTORY) return false;
	return true;
}

/*! \brief ファイルアクセス用排他制御(取得)
*/
void CIni_Template::EnterMutex()
{
	WaitForSingleObject(m_mutex, INFINITE);
}

/*! \brief ファイルアクセス用排他制御(解放)
*/
void CIni_Template::LeaveMutex()
{
	ReleaseMutex(m_mutex);
}

/*! \brief INIファイルから整数値取得
 *  \param lpAppName セクション名
 *  \param lpKeyName キー名
 *  \param nDefault キー名が見つからなかった場合に返すべき値
 *  \return 処理結果
*/
UINT CIni_Template::GetValueInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault)
{
	EnterMutex();
	UINT res = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_filePath);
	LeaveMutex();

	return res;
}

/*! \brief INIファイルから浮動小数値取得
 *  \param lpAppName セクション名
 *  \param lpKeyName キー名
 *  \param nDefault キー名が見つからなかった場合に返すべき値
 *  \return 処理結果
*/
double CIni_Template::GetValueDouble(LPCTSTR lpAppName, LPCTSTR lpKeyName, double nDefault)
{
	char keyBuf[256] = {0};
	char lpDefault[256] = {0};
	_snprintf_s( lpDefault, 255, _TRUNCATE, "%0.12f", nDefault );

	EnterMutex();
	GetPrivateProfileString( lpAppName, lpKeyName, lpDefault, keyBuf, 255, m_filePath );
	LeaveMutex();

	return atof( keyBuf );
}

/*! \brief INIファイルから文字列取得
 *  \param lpAppName セクション名
 *  \param lpKeyName キー名
 *  \param lpDefault 既定の文字列
 *  \param lpReturnedString 情報が格納されるバッファ
 *  \param nSize 情報バッファのサイズ
 *  \return 処理結果
*/
DWORD CIni_Template::GetValueString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	EnterMutex();
	DWORD res = GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, m_filePath);
	LeaveMutex();

	return res;
}

  /*! \brief INIファイルから指定されたセクション内のすべてのキーと値を取得
 *  \param lpAppName セクション名
 *  \param lpReturnedString 情報が格納されるバッファ
 *  \param nSize 情報バッファのサイズ
 *  \return 処理結果
*/
DWORD CIni_Template::GetValueSection(LPCTSTR lpAppName, LPTSTR lpReturnedString, DWORD nSize)
{
	EnterMutex();
	DWORD res = GetPrivateProfileSection(lpAppName, lpReturnedString, nSize, m_filePath);
	LeaveMutex();

	return res;
}

 /*! \brief 指定されたセクション内の既存のキー名と値を削除し、指定されたキーとその値のペアを書き込みます。
 *  \param lpAppName セクション名
 *  \param lpString 書き込むべきデータ
 *  \return 処理結果
*/
BOOL CIni_Template::WriteValueSection(LPCTSTR lpAppName, LPCTSTR lpString)
{
	EnterMutex();
	BOOL res = WritePrivateProfileSection(lpAppName, lpString, m_filePath);
	LeaveMutex();

	return res;
}


/*! \brief INIファイルに文字列設定
 *  \param pAppName セクション名
 *  \param lpKeyName キー名
 *  \param lpString 追加するべき文字列
 *  \return 処理結果
*/
BOOL CIni_Template::WriteValueString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	EnterMutex();
	BOOL res = WritePrivateProfileString(lpAppName, lpKeyName, lpString, m_filePath);
	LeaveMutex();

	return res;
}


/*! \fn LPCTSTR GetModuleDirectory()
 *  \brief モジュールのディレクトリへのパスを取得する。
 *  \return モジュールのディレクトリへのパスが格納された NULL で終わる文字列へのポインタが返る。
*/
LPTSTR CIni_Template::GetModuleDirectory()
{
	if (_tcslen(m_moduleDirectory) == 0){
		TCHAR szFullPath[_MAX_PATH] = {0};
		TCHAR szDir[_MAX_DIR] = {0};
		TCHAR szDrive[_MAX_DRIVE] = {0};

		::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

		_tsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
		_sntprintf_s(m_moduleDirectory, sizeof(m_moduleDirectory) -1, _TRUNCATE, _T("%s%s"), szDrive, szDir);
	}

	return m_moduleDirectory;
}