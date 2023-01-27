/*! \file ExportFileUpdate.h
 *  \brief ExportFileUpdate.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/20 pt.ju-tanaka 新規作成
*/

#pragma once

#include "Common.h"
#include "PaperDB_MigrationDef.h"

#include <string>

/**
 * エクスポート先フォルダへのアクセス結果定義
**/
enum EReturnCode 
{
	kReturnCode_Success = 0,	//!< エラー無し
	kReturnCode_NotExists,		//!< フォルダパスが存在しない
	kReturnCode_NotAccess,		//!< フォルダにアクセスできない
};


/**
 * エクスポートファイル(テキスト)
**/
class ExportFileUpdate
{
public:

	/**
	 * エクスポート先のルートパスが有効かチェックする
	 * @param[in]	inDestRootFolderPath : エクスポート先のルートパス(UNC)
	 * @retval	kReturnCode_Success：エラー無し(0)
	 * @retval	kReturnCode_NotExist：フォルダパスが存在しない(1)
	 * @retval	kReturnCode_NotAccess：フォルダにアクセスできない(2)
	**/
	EReturnCode IsValidExportFolder(const std::string& inDestRootFolderPath);

	/**
	 * コンストラクタ
	**/
	ExportFileUpdate();

	/**
	 * デストラクタ
	**/
	virtual ~ExportFileUpdate();

	/**
	 * ファイル出力する
	 * @param[in]	inPrintConditionName : ファイル出力対象の印刷条件名
	 * @param[in]	inMigrationData : ファイル出力対象の業務移行用構造体
	 * @param[in]	inLocalFileName : 出力する一時ファイル名
	 * @retval	true：\Client\Temp\PrintConditionExportTextへのファイル出力成功
	 * @retval	false：失敗
	**/
	bool WriteFile(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData, const std::string& inLocalFileName);

	/**
	 * \Client\Temp\PrintConditionExportのファイルを指定パスに移動する
	 * @param[in]	inDestRootFolderPath : 移動先のルートパス(UNC)
	 * @param[in]	inFileName : ファイル名
	 * @retval	true：ファイル移動成功
	 * @retval	false：失敗
	**/
	bool MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName);

	/**
	 * 指定されたファイルを削除する
	 * @param[in]	inFilePath : 削除対象のファイル
	**/
	void DeleteFiles(const std::string& inFilePath);

	/**
	 * EQサーバ共有フォルダ内の指定ファイルを削除する
	 * @param[in]	inFolderPath : EQサーバ共有フォルダのパス。
	 * @param[in]	inPrintConditionName : 削除対象の印刷条件名
	**/
	bool DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName);
	
	/**
	 * EQサーバ共有フォルダ内のファイルを全て削除する
	 * @param[in]	inFolderPath : EQサーバ共有フォルダのパス。
	**/
	void DeleteAllFilesEQ(const std::string& inFolderPath);

	/**
	 * @brief	ファイルが存在するかのチェック
	 * @param[in]	inFolderPath : フォルダパス
	 * @param[in]	inFileName : ファイル名
	**/
	bool IsFileExist(std::string& inFolderPath, std::string& inFileName);

	/**
	 * @brief	一時フォルダパスの取得
	 * @retval	一時フォルダパス
	**/
	std::string getLocalFolderPath();

	/**
	 * @brief	一時ファイル名の作成
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @retval	一時ファイル名
	**/
	std::string createLocalFileName(const std::string& inPrintConditionName);

private:
	/**
	 * コマンドプロンプトで実行する。
	 * @param[in]	inCmd : 実行するコマンド文字列。
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	DWORD runSystemCmd(const std::string& inCmd);

	/**
	 * GetLastErrorの文字列を取得する。
	 * @retval	GetLastErrorの文字列
	**/
	std::string getLastError();

};
