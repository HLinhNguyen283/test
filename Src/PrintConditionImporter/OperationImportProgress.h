/**
 * @file	OparationImportProgress.h
 * @brief	ImportProgress.txtの操作(作成・削除等)クラス
**/

#pragma once
//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include "Common.h"
#include <list>

/**
	@brief	ImportProgress.txtの操作(作成・削除等)を行う
*/

class COperationImportProgress
{
public:
	//! 取り込み結果
	enum E_END_RESULT
	{
		END_RESULT_SUCCESS,	//!< 成功
		END_RESULT_SKIP,	//!< スキップ
		END_RESULT_ERROR	//!< 失敗
	};

	/**
	 * @brief	constructor
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	**/
	COperationImportProgress(const std::string& inOutputURL) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~COperationImportProgress() throw();

	/**
	 * @brief	成功時の取り込み結果を出力する
	**/
	void WriteSuccess(const std::string& inPrintConditionName, E_END_RESULT inResult) throw();

	/**
	 * @brief	ファイルを作成する
	 * @note	エラー発生時は例外を投げる
	 **/
	void createFile() throw(std::runtime_error);

	/**
		@brief	ファイル削除
	*/
	bool deleteImportProggressFile();


private:

	/**
	 * @brief	ファイルを閉じる
	 * @param[in]	inFileHandle : ファイルハンドル
	**/
	void closeFile(HANDLE inFileHandle) throw();

	/**
	 * @brief	ファイルに書く
	 * @param[in]	inString : 文字列
	 * @note	エラー発生時は例外を投げる
	**/
	void writeFile(const std::string& inString) throw(std::runtime_error);

	/**
		@brief	ファイル削除
	*/
	void deleteFile();

	/**
	 * @brief	エラーコードの文字列を取得する
	 * @param[in]	inErrID : エラーコード
	 * @retval	エラーコードの文字列
	**/
	const std::string getLastErrorString(DWORD inErrID) throw();


	std::string m_OutputFilePath;	//!< 出力ファイルのフルパス
	std::list<std::string> m_FaileList;	//!< 結果一覧(名前+取り込み結果)

};

