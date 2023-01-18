/**
	* @file		GetIniFileParam.h
	* @brief	INIファイル値取得 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/
#pragma once

/**
* @brief INI情報取得/更新クラス
*/
class CGetIniFileParam
{
	// TP-UW_Communication.iniのファイルパス取得
	static CString GetUnwinderIniFilePath();
	// PrinterDescriptor.iniのファイルパス取得
	static CString GetPrinterDescriptorIniFilePath();
public:
	// Unwindwe機能の有効/無効
	static BOOL GetUnwinderOption();
	// UWからのSignalStatusを受信するポート
	static UINT GetRecvPortNumber();
	// UWからのSignalStatusを受信するIPアドレス
	static CString GetRecvIpAddress();
	// Status受信フォルダ名
	static CString GetRecvStatusFolder();
	// Paper受信フォルダ名
	static CString GetRecvPaperFolder();
	// Status受信URL名
	static CString GetRecvStatusURL();
	// Paper受信URL名
	static CString GetRecvPaperURL();
	// ログ保存期間
	static BOOL GetLogRetentionPeriod();
	// ログレベル
	static UINT GetDebugLogLevel();
};