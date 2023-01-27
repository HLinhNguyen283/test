/**
	* @file		WriteDebugLog.h
	* @brief	デバッグ用ログの出力 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#define	D_DEFAULT_FOLDER_NAME	_T("LOG")

/**
 * @brief デバッグ用ログの出力クラス
*/
class CWriteDebugLog
{
public:
	enum E_LOG_LEVEL
	{ 
		E_LOG_LEVEL_1 = 1,			// 主要ログ
		E_LOG_LEVEL_2,				// 関数トレースログ
		E_LOG_LEVEL_3				// 送受信データログ
	};
public:
	/**
		* @brief CWriteDebugLog コンストラクション
	**/
	CWriteDebugLog();

	/**
		* @brief CWriteDebugLog デストラクション
	**/
	virtual ~CWriteDebugLog();
		
	/**
		* @brief 初期処理
		* @param InFileName ファイル名
	**/
	void Initialize( const CString& inFileName );

	/**
		* @brief 初期処理（XML受信用）
		* @param InFileName ファイルパス名
	**/
	void InitializeXml( const CString& inFileName, const char* inDeviceName);

	/**
		* @brief デバッグ用ログを出力
		* @param InStrLogInfo ログに書き出す文字列
		* @param InDebugLevel デバッグ情報のLevel
		* @param InTime       TRUE=日時出力あり、FALSE=日時出力なし
	*/ 
	void WriteDebuglog( const CString& InStrLogInfo, E_LOG_LEVEL inDebugLevel, BOOL inTime = TRUE );

private:
	CRITICAL_SECTION	m_cs;					//!< アプリケーケーションログ用のクリティカルセクション
	int					m_Level;				//!< ログレベル
	CString				m_strFileName;			//!< ログ出力フォルダの日付
	CString				m_strDate;				//!< ログ出力フォルダの日付
	CString				m_strOutputPath;		//!< ログ出力ファイルパス
};