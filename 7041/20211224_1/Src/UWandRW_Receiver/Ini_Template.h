/*! \file Ini_Template.h
 *  \brief Iniファイル操作共通クラス
 *  \author TGL Hamaguchi
 *  \date 2015/05/18 TGL Hamaguchi 新規作成
*/

#if !defined(__INI_TEMPLATE_H_INCLUDE__)
#define __INI_TEMPLATE_H_INCLUDE__

// ==========================================================================
// Include files
#include <windows.h>
#include <TCHAR.h>
#include <string>
// ==========================================================================

/*!
	*  \brief Iniファイル操作共通クラス
*/
class CIni_Template
{
public:
	//定数

	//関数
	CIni_Template();
	virtual ~CIni_Template();
		
	//初期化・解放
	bool Initialize(bool isSelf);

	//ゲッター＆セッター
	bool IsFile();

protected:
	//関数
	virtual const char* GetMutexName() = 0;
	virtual void SetFilePath(bool isSelf) = 0;
	//イベント

	//部品
	void EnterMutex();
	void LeaveMutex();
	UINT GetValueInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault);
	double GetValueDouble(LPCTSTR lpAppName, LPCTSTR lpKeyName, double nDefault);
	DWORD GetValueString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize);
	DWORD GetValueSection(LPCTSTR lpAppName, LPTSTR lpReturnedString, DWORD nSize);
	BOOL WriteValueSection(LPCTSTR lpAppName, LPCTSTR lpString);
	BOOL WriteValueString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	LPTSTR GetModuleDirectory();

	//変数
	char m_filePath[_MAX_PATH];			//!< INIファイルパス.
	HANDLE m_mutex;						//!< 排他制御オブジェクト.
	TCHAR m_moduleDirectory[_MAX_DIR];	//!< モジュールディレクトリ.
private:
	//変数
	
};


#endif // !defined(__INI_TEMPLATE_H_INCLUDE__)
