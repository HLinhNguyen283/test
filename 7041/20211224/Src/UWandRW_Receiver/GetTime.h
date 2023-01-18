/**
	* @file		GetTime.h
	* @brief	現在日時取得 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/
#pragma once

/**
 * @brief 現在日時取得クラス
*/
class CGetTime
{
public:
	/**
		* @brief  yyyyMMddhhmmss_999 の形式で現在日時を取得する
		* @retval 現在日時（yyyyMMddhhmmss_999）
	**/
	static CString GetStrTimeMM();

	/**
		* @brief  yyyy/MM/dd hh:mm:ss の形式で現在日時を取得する
		* @retval 現在日時（yyyy/MM/dd hh:mm:ss）
	**/
	static CString GetStrTimeSlCo();

	/**
		* @brief  yyyyMMdd の形式で現在日付を取得する
		* @retval 現在日時（yyyyMMdd）
	**/
	static CString GetStrDate();
};