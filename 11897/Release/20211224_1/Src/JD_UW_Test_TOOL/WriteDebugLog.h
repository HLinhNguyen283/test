/**
 * @file  WriteDebugLog.h
 * @brief デバッグ用ログの出力 ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
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
	/* ==================================================================================================== */
	/**
		* @brief CWriteDebugLog コンストラクション
	**/
	/* ==================================================================================================== */
	CWriteDebugLog();

	/* ==================================================================================================== */
	/**
		* @brief CWriteDebugLog デストラクション
	**/
	/* ==================================================================================================== */
	virtual ~CWriteDebugLog();
		
	/* ==================================================================================================== */
	/**
		* @brief 初期処理
		* @param InFileName ファイルパス
	**/
	/* ==================================================================================================== */
	void Initialize( const CString& InFileName );

	/* ==================================================================================================== */
	/**
		* @brief デバッグ用ログを出力
		* @param InStrLogInfo ログに書き出す文字列
		* @param InDebugLevel デバッグ情報のLevel
		* @param InTime       TRUE=日時出力あり、FALSE=日時出力なし
	*/ 
	/* ==================================================================================================== */
	void WriteDebuglog( const CString& InStrLogInfo, E_LOG_LEVEL InDebugLevel, BOOL InTime = TRUE );

private:
	CRITICAL_SECTION	m_cs;					//!< アプリケーケーションログ用のクリティカルセクション
	int					m_Level;				//!< ログレベル
	CString				m_strOutputPath;		//!< ログ出力ファイルパス

};