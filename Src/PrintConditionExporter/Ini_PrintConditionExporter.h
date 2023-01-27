/**
 * @file	Ini_PrintConditionExporter.h
 * @brief	PrintConditionExporter.ini ファイル操作クラス
 * @author	pt.ju-tanaka
 * @date	2019/02/12 pt.ju-tanaka 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "Ini_Template.h"
#include <string>
// ==========================================================================

/**
 * @brief	PrintConditionExporter.ini ファイル操作クラス
**/
class CIni_PrintConditionExporter : public CIni_Template
{
public:
	/**
	 * @brief	コンストラクタ
	**/
	CIni_PrintConditionExporter();

	/**
	 * @brief	デストラクタ
	**/
	virtual ~CIni_PrintConditionExporter();

	/**
	 * @brief	Log.htmlに出力するかどうか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsDebugLog();

	/**
	 * @brief	Log.htmlにXMLエクスポート処理のログを出力するかどうか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsDebugXmlExportLog();

	/**
	 * @brief	EQ制御サーバー上にあるテキスト出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にあるテキスト出力先のルートフォルダ名
	**/
	void GetOutputRootFolder(std::string& outPath);

	/**
	 * @brief	EQ制御サーバー上にあるXML出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にあるXML出力先のルートフォルダ名
	**/
	void GetOutputRootFolderXML(std::string& outPath);

	/**
	 * @brief	EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔
	 * @retval	リトライ間隔(単位：秒)
	**/
	long GetAutoUpdatePrintConditionRetryInterval();

	/**
	 * @brief	EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔
	 * @retval	リトライ間隔(単位：秒)
	**/
	long GetAutoUpdatePrintConditionRetryIntervalXML();

	/**
	 * @brief	EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能の有効状態
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdatePrintConditionText();

	/**
	 * @brief	EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能の有効状態
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdatePrintConditionXML();

	/**
	 * @brief	XML/TEXT出力先の<IPAddress>-<構成名>フォルダを192.168.0.30-<構成名>で処理するか？
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdateFixIP();

	/**
	 * @brief	EQ制御サーバー上にあるJI関連情報出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にあるJI関連情報出力先のルートフォルダ名
	**/
	void GetOutputRootFolderInspectionData(std::string& outPath);

protected:
	//関数

	/**
	 * @brief	Mutexの名前を取得する
	 * @retval	Mutexの名前
	**/
	virtual const char* GetMutexName();

	/**
	 * @brief	INIファイルパスを設定する
	 * @param	bSelf : 自機のINIファイルかどうか
	**/
	virtual void SetFilePath(BOOL bSelf);

	//イベント

	//部品

	//変数

private:
	//変数
	
};
