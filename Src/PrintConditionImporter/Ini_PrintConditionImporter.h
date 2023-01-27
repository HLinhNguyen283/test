/**
 * @file	Ini_PrintConditionImporter.h
 * @brief	PrintConditionImporter.ini ファイル操作クラス
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "Ini_Template.h"
#include <string>
// ==========================================================================

/**
 * @brief	PrintConditionImporter.ini ファイル操作クラス
**/
class CIni_PrintConditionImporter : public CIni_Template
{
public:
	/**
	 * @brief	コンストラクタ
	**/
	CIni_PrintConditionImporter();

	/**
	 * @brief	デストラクタ
	**/
	virtual ~CIni_PrintConditionImporter();
		
	/**
	 * @brief	インポート時のベースとなる印刷条件名を取得する
	 * @param[out]	outName : インポート時のベースとなる印刷条件名
	 * @note	取得できなかった場合は空文字になる
	**/
	void GetBasePrintConditionName(std::string& outName);

	/**
	 * @brief	JI系の情報の登録が必要かどうかを取得する
	 * @retval	true : 必要
	 * @retval	false : 不要
	**/
	bool IsRegisterJetInspection();

	/**
	 * @brief	インポートのタイムアウト時間を取得する
	 * @retval	インポートのタイムアウト時間[単位:秒]
	**/
	DWORD GetTimeoutImport();

	/**
	 * @brief	デバッグ用のファイルを出力するか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsDebugLog();

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
