/**
	* @file  Utility.h
	* @brief 汎用モジュール ヘッダファイル
	* @author cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

#pragma once

// ===========================================================================
// Include files
#include <string>
#include <vector>
// ===========================================================================

namespace uwrw_manager
{

/**
* @brief 汎用モジュールクラス
*/
class CUtility
{
public:
	/* ==================================================================================================== */
	/**
		* @brief  文字列置換
		* @param  IoStrData : [io]対象文字列
		* @param  InSrc     : [i]変換前
		* @param  InDst     : [i]変換前
		* @retval 文字列中の文字を全て置換する
	**/
	/* ==================================================================================================== */
	static void replaceString( std::string& IoStrData, const std::string InSrc, const std::string InDst );

	/* ==================================================================================================== */
	/**
		* @brief  文字列を特定文字で区切る
		* @param  InStrData : [i]対象文字列
		* @param  InDelim   : [i]区切り文字
		* @retval 文字列のリスト
	**/
	/* ==================================================================================================== */
	static std::vector<std::string> splitString( const std::string& InStrData, char InDelim );

	/* ==================================================================================================== */
	/**
		* @brief  YYYYMMDDの形式で現在日を取得する
		* @retval 現在日
	**/
	/* ==================================================================================================== */
	static std::string GetStrDate();

};

};	// namespace ink_code_manager