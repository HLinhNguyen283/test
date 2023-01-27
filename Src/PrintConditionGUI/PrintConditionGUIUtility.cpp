/**
	@file	PrintConditionGUIUtility.cpp
	@brief	PrintConditionGUI 用のユーティリティ
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "HDMonoUtility.h"


// ==========================================================================
// Local definitions

// ==========================================================================

// namespace を付けておく
namespace PrintConditionGUIUtility {


/**
	@brief	文字列から末尾のスペースを取り除く
	@param[in]	inTargetStr : チェック対象の文字列
	@retval スペース除去後の文字列
*/
std::string TrimLastSpace(std::string inTargetStr)
{
	std::string	rtStr = inTargetStr;

	// trimするとき削除する文字(半角スペース、全角スペース)
	std::string trimCharacters = " 　";

	// 左側からトリムする文字以外が見つかる位置を検索します。
	std::string::size_type found = rtStr.find_last_not_of(trimCharacters);
	
	if (found != std::string::npos) {
		// 対象文字列を削除
		rtStr.erase(found + 1);
	} 

	return rtStr;
};

}	// namespace PrintConditionGUIUtility
