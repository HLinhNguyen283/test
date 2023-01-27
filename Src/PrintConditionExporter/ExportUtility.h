/*! \file ExportUtility.h
 *  \brief ExportUtility.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
*/

#pragma once

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PaperDB_MigrationDef.h"

#include <list>
// ==========================================================================

const std::string st_ExtensionTxt = ".txt";
const std::string st_ExtensionXML = ".xml";

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
 * @brief 依頼リストデータ構造体
**/
struct SRequestConditionData
{
	std::string				conditionName;		//!< 印刷条件名
	ST_PDB_MIGRATION_DATA	migrationData;		//!< 業務移行用構造体
	bool					isUpdate;			//!< 更新 or 削除指定
	std::string				localFileName;		//!< 一時ファイル名

	SRequestConditionData()
	{
		isUpdate = false;
	}
};

/**
 * ユーティリティ
**/
class ExportUtility
{
public:

	/**
	 * 印刷条件情報を取得する
	 * @param[in]	outPDBDataList : 依頼リストデータ構造体
	**/
	static void getPDBGetPrintConditionInfo(std::list<SRequestConditionData>& outPDBDataList);

	/**
	 * エクスポート先のルートパスが有効かチェックする
	 * @param[in]	inDestRootFolderPath : エクスポート先のルートパス(UNC)
	 * @retval	kReturnCode_Success：エラー無し(0)
	 * @retval	kReturnCode_NotExist：フォルダパスが存在しない(1)
	 * @retval	kReturnCode_NotAccess：フォルダにアクセスできない(2)
	**/
	static EReturnCode isValidExportFolder(const std::string& inDestRootFolderPath);

	/**
	 * @brief	文字列を置換する
	 * @param[in]	inSrcStr : 変換対象の文字列
	 * @param[in]	inDstStr : 変換後の文字列
	 * @param[in,out]	ioStr : 文字列
	 * @param[in]	inPos : 開始位置(省略時は先頭から)
	**/
	static void replaceString(const std::string& inSrcStr, const std::string& inDstStr, std::string& ioStr, const size_t inPos = 0);

	/**
	 * ファイルを指定パスに移動する
	 * @param[in]	inDestRootFolderPath : 移動先のルートパス(UNC)
	 * @param[in]	inFileName : ファイル名
	 * @param[in]	inExtensionStr : 拡張子文字列
	 * @param[in]	inTempFolderPath : 中間フォルダパス
	 * @retval	true：ファイル移動成功
	 * @retval	false：失敗
	**/
	static bool MoveFiles(const std::string& inDestRootFolderPath, const std::string& inFileName, const std::string& inExtensionStr, const std::string& inTempFolderPath);

	/**
	 * 指定されたファイルを削除する
	 * @param[in]	inFilePath : 削除対象のファイル
	**/
	static void DeleteFiles(const std::string& inFilePath);

	/**
	 * EQサーバ共有フォルダ内の指定ファイルを削除する
	 * @param[in]	inFolderPath : EQサーバ共有フォルダのパス。
	 * @param[in]	inPrintConditionName : 削除対象の印刷条件名
	 * @param[in]	inExtensionStr : 拡張子文字列
	**/
	static bool DeleteFileEQ(const std::string& inFolderPath, const std::string& inPrintConditionName, const std::string& inExtensionStr);
	
	/**
	 * EQサーバ共有フォルダ内のファイルを全て削除する
	 * @param[in]	inFolderPath : EQサーバ共有フォルダのパス。
	**/
	static void DeleteAllFilesEQ(const std::string& inFolderPath);

	/**
	 * @brief	ファイルが存在するかのチェック
	 * @param[in]	inFolderPath : フォルダパス
	 * @param[in]	inFileName : ファイル名
	 * @param[in]	inExtensionStr : 拡張子文字列
	**/
	static bool IsFileExist(std::string& inFolderPath, std::string& inFileName, const std::string& inExtensionStr);

	/**
	 * @brief	中間フォルダパスの取得
	 * @param[in]	inTempFolderPath : 中間フォルダパス
	 * @retval	中間フォルダパス
	**/
	static std::string getLocalFolderPath(const std::string& inTempFolderPath);

	/**
	 * @brief	一時ファイル名の作成
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @retval	一時ファイル名
	**/
	static std::string createLocalFileName(const std::string& inPrintConditionName);

private:
	/**
	 * コマンドプロンプトで実行する。
	 * @param[in]	inCmd : 実行するコマンド文字列。
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	static DWORD runSystemCmd(const std::string& inCmd);

	/**
	 * GetLastErrorの文字列を取得する。
	 * @retval	GetLastErrorの文字列
	**/
	static std::string getLastError();
};
