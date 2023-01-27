/**
 * @file	Utility.h
 * @brief	ユーティリティ
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include <string>

#define WriteDebugLog(x) CUtility::WriteDebugLog_(__FUNCTION__, x)

/**
 * ユーティリティ
**/
class CUtility
{
public:
	/**
	 * デバッグ用ログ出力
	 * @param[in]	inFunctionName : 関数名
	 * @param[in]	inText：文字列
	**/
	static void WriteDebugLog_(const std::string& inFunctionName, const std::string& inText);



	/**
	 *	inSrcがnullptrの場合に例外を投げる
	 *	@param[in]	inSrc : 何らかのポインタ
	 *	@param[in]	inErrorText : エラー時のメッセージ用文字列
	 *	@retval	inSrc
	**/
	template <class T1> static T1* ThrowIfNull(T1* inSrc, const std::string& inErrorText)
	{
		if(inSrc == nullptr)
		{
			throw std::invalid_argument(inErrorText);
		}
		else
		{
			return inSrc;
		}
	}

	/**
	 *	inResultがfalseの場合に例外を投げる
	 *	@param[in]	inSrc : 何らかのポインタ
	 *	@param[in]	inErrorText : エラー時のメッセージ用文字列
	**/
	static void ThrowIfFalse(bool inResult, const std::string& inErrorText);

};


