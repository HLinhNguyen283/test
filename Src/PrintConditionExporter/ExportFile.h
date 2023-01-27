/**
 * @file	ExportFile.h
 * @brief	エクスポートファイル(テキスト)
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include "Common.h"
#include "PrintCondition.h"

#include <string>

/**
 * エクスポートファイル(テキスト)
**/
class CExportFile
{
private:
	static const std::string st_tempFolderPath;		//一時出力先パス
	static const std::string st_paperDataFolder;	//エクスポート先フォルダ名
public:
	/**
	 * エクスポート先のルートパスが有効かチェックする
	 * @param[in]	inDestRootFolderPath : エクスポート先のルートパス(UNC)
	 * @retval	true：有効
	 * @retval	false：無効
	**/
	static bool IsValidExportFolder(const std::string& inDestRootFolderPath);

	/**
	 * コンストラクタ
	**/
	CExportFile();

	/**
	 * デストラクタ
	**/
	virtual ~CExportFile();

	/**
	 * ファイル出力する
	 * @param[in]	inPdbData : 出力対象の印刷条件名の各レコード
	 * @retval	true：\Client\Temp\PrintConditionExportへのファイル出力成功
	 * @retval	false：失敗
	**/
	bool WriteFile(const CPrintCondition& inPdbData);

	/**
	 * \Client\Temp\PrintConditionExportの全ファイルを指定パスに移動する
	 * @param[in]	inDestRootFolderPath : 移動先のルートパス(UNC)
	 * @retval	true：全ファイル移動成功
	 * @retval	false：失敗
	**/
	bool MoveAllFiles(const std::string& inDestRootFolderPath);

	/**
	 * \Client\Temp\PrintConditionExportのファイルを全て削除する
	**/
	void DeleteAllFiles();


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

	/**
	 * ファイルを移動させる。
	 * @param[in]	inSrcPath : 移動元フォルダ。
	 * @param[in]	inDestPath : 移動先フォルダ。
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	bool moveFile(const std::string& inSrcPath, const std::string& inDestPath);
};


