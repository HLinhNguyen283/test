/**
	@file	PrintConditionGUIUtility.cpp
	@brief	PrintConditionGUI 用のユーティリティ
*/

#pragma once


// ==========================================================================
// Include files
#include "stdafx.h"

// ==========================================================================

// namespace を付けておく
namespace PrintConditionGUIUtility {

/**
	@brief	文字列から末尾のスペースを取り除く
	@param[in]	inTargetStr : チェック対象の文字列
	@retval スペース除去後の文字列
*/
std::string TrimLastSpace(std::string inTargetStr);

}	// namespace PrintConditionGUIUtility
