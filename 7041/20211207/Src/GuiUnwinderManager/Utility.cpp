/**
	* @file  Utility.cpp
	* @brief 汎用モジュール
	* @author cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include "Utility.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  文字列置換
	* @param  IoStrData : [io]対象文字列
	* @param  InSrc     : [i]変換前
	* @param  InDst     : [i]変換前
	* @retval 文字列中の文字を全て置換する
**/
void CUtility::replaceString( std::string& IoStrData, const std::string InSrc, const std::string InDst )
{

	bool Loop = true;
	std::string::size_type n = 0;
	while( Loop )
	{
		n = IoStrData.find(InSrc, n);
		if (n != IoStrData.npos)
		{
			//置換文字列検出
			IoStrData.replace( n, InSrc.size(), InDst) ;
			n+=2;
		} else {
			Loop = false;
		}
	}
}

/**
	* @brief  文字列を特定文字で区切る
	* @param  InStrData : [i]対象文字列
	* @param  InDelim   : [i]区切り文字
	* @retval 文字列のリスト
**/
std::vector<std::string> CUtility::splitString( const std::string& InStrData, char InDelim )
{
    std::istringstream iss(InStrData);
    std::string StrTemp;
    std::vector<std::string> strList;
    while (std::getline(iss, StrTemp, InDelim))
	{
		strList.push_back(StrTemp);
	}
    return strList;
}

/**
	* @brief  YYYYMMDDの形式で現在日を取得する
	* @retval 現在日
**/
std::string CUtility::GetStrDate()
{
	// 現在の時刻を取得
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 時刻をを文字列に変換=>20090727
	char buf[64];
	_snprintf_s( buf, sizeof(buf) - 1, "%04d%02d%02d", (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return std::string(buf);
}


};	// namespace ink_code_manager