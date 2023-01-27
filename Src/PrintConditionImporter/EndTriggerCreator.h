/**
 * @file	EndTriggerCreator.h
 * @brief	取り込み先に完了トリガーを作成する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include <list>
#include <string>

/**
 * @brief 取り込み先に完了トリガーを作成する
**/
class CEndTriggerCreator
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
	CEndTriggerCreator(const std::string& inOutputURL) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CEndTriggerCreator() throw();

	/**
	 * @brief	取り込み結果を追加する
	 * @param[in]	inName : 印刷条件名
	 * @param[in]	inResult : 取り込み結果
	**/
	void AddResult(const std::string& inPrintConditionName, E_END_RESULT inResult) throw();

	/**
	 * @brief	失敗時の取り込み結果を出力する
	**/
	void WriteFailed() throw();

	/**
	 * @brief	成功時の取り込み結果を出力する
	**/
	void WriteSuccess() throw();

protected:
	/**
	 * @brief	ファイルを作成して開く(m_TempFilePathに作成)
	 * @retval	ファイルハンドル
	 * @note	エラー発生時は例外を投げる
	 **/
	HANDLE createFile() throw(std::runtime_error);

	/**
	 * @brief	ファイルを閉じる
	 * @param[in]	inFileHandle : ファイルハンドル
	**/
	void closeFile(HANDLE inFileHandle) throw();

	/**
	 * @brief	ファイルに書く
	 * @param[in]	inFileHandle : ファイルハンドル
	 * @param[in]	inString : 文字列
	 * @note	エラー発生時は例外を投げる
	**/
	void writeFile(HANDLE inFileHandle, const std::string& inString) throw(std::runtime_error);

	/**
	 * @brief	ファイルを移動(m_TempFilePath→m_OutputFilePath)
	 * @note	エラー発生時は例外を投げる
	**/
	void moveFile() throw(std::runtime_error);

	/**
	 * @brief	エラーコードの文字列を取得する
	 * @param[in]	inErrID : エラーコード
	 * @retval	エラーコードの文字列
	**/
	const std::string getLastErrorString(DWORD inErrID) throw();


	std::string m_OutputFilePath;	//!< 出力ファイルのフルパス
	std::string m_TempFilePath;		//!< 出力ファイルの一時ファイルのフルパス
	std::string m_FileName;			//!< 出力ファイル名
	std::list<std::pair<std::string, E_END_RESULT>> m_List;	//!< 結果一覧(名前+取り込み結果)
};

