/**
 * @file  OutputLog.h
 * @brief ログ出力クラス ヘッダーファイル
 * @date  20190610 CEC Miyachi 新規作成
 **/

#pragma once
#include <fstream>

// ===========================================================================
//
// COutputLog クラス
//
// ===========================================================================
/**
 * @brief ログ出力クラス
*/
class COutputLog
{
public:
	COutputLog();
	~COutputLog();

//--------------------------------------
// メンバ関数
//--------------------------------------
public:
	void Start( std::string strFileName );
	void Output( const std::string& strLog, bool IsOutputTime = true );
	void Output( const std::wstring& wstrLog, bool IsOutputTime=true );
protected:
private:
	std::string GetTime();
	std::string GetThreadId();
	std::string GetModuleDirectory();

//--------------------------------------
// メンバ変数
//--------------------------------------
public:
protected:
private:
	CRITICAL_SECTION	m_cs;					//!< アプリケーケーションログ用のクリティカルセクション
	CRITICAL_SECTION	m_cs_http;				//!< httpログ用のクリティカルセクション

	std::string			m_strPath;				//!< アプリケーケーションログ用のファイルパス
	std::string			m_strFileName;			//!< アプリケーケーションログ用のファイル名称

	std::ofstream		m_ofs;					//!< アプリケーケーションログ用のファイルストリーム
};
