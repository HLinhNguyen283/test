/**
 * @file	WaitTrigger.h
 * @brief	待機中トリガーの管理
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include <string>

/**
 * @brief	待機中トリガーの管理
**/
class CWaitTrigger
{
private:
	/**
	 * @brief	constructor
	**/
	CWaitTrigger();

public:

	/**
	 * @brief	destructor
	**/
	virtual ~CWaitTrigger();

	/**
	 * @brief	インポート依頼受付イベント
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @retval	true : 成功
	 * @retval	false : 失敗
	 * @note	待機中トリガーを作成する。
	**/
	static bool OnAddRequest(const std::string& inOutputURL);

	/**
	 * @brief	インポート依頼一件分のインポート終了イベント
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @note	待機中トリガーを削除する
	**/
	static void OnFinsh(const std::string& inOutputURL);

	/**
	 * @brief	インポート依頼一件分のインポートタイムアウト終了イベント
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @note	待機中トリガー削除とタイムアウトトリガー作成
	**/
	static void OnTimeoutFinsh(const std::string& inOutputURL);

protected:
	/**
	 * @brief	トリガーファイルを作成する
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @param[in]	inFileName : ファイル名
	 * @note	エラー発生時は例外を投げる
	 **/
	static void CreateTriggerFile(const std::string& inOutputURL, const std::string& inFileName) throw(std::runtime_error);

	/**
	 * @brief	トリガーファイルを削除する
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @param[in]	inFileName : ファイル名
	 * @note	エラー発生時は例外を投げる
	**/
	static void DeleteTriggerFile(const std::string& inOutputURL, const std::string& inFileName) throw(std::runtime_error);

	/**
	 * @brief	エラーコードの文字列を取得する
	 * @param[in]	inErrID : エラーコード
	 * @retval	エラーコードの文字列
	**/
	static const std::string getLastErrorString(DWORD inErrID);

private:

};

