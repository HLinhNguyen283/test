/**
	* @file		GetTime.h
	* @brief	現在日時取得 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/
#pragma once

/**
 * @brief 現在日時取得クラス
*/
class CGetTime
{
	public:
		/**
		* @brief 2009:12:21:04:45:47 の形式で現在の日時を取得する
		* @return 「2009:12:21:04:45:47」形式の文字列
		*/
		static CString GetStrTimeCo();

		/**
		* @brief 20091221044547 の形式で現在の日時を取得する
		* @return 「20091221044547」形式の文字列
		*/
		static CString GetStrTime();

		/**
		* @brief 20091221044547_999 の形式で現在の日時を取得する
		* @return 「20091221044547_999」形式の文字列
		*/
		static CString GetStrTimeMM();

		/**
		* @brief 20091221044547 の形式で現在の日時を取得する
		* @return 「20091221044547」形式のint型の数値
		*/
		static int GetIntTime();
		
		/**
		* @brief 2009/12/21/04/45/47 の形式で現在の日時を取得する
		* @return 「2009/12/21/04/45/47」形式の文字列
		*/
		static CString GetStrTimeSla();

		/**
		* @brief 2009/12/21 04:45:47 の形式で現在の日時を取得する
		* @return 「2009/12/21 04:45:47」形式の文字列
		*/
		static CString GetStrTimeSlCo();

		/**
		* @brief 2009:12:21 の形式で現在の日付を取得する
		* @return 「2009:12:21」形式の文字列
		*/
		static CString GetStrDateCo();

		/**
		* @brief 20091221 の形式で現在の日付を取得する
		* @return 「20091221」形式の文字列
		*/
		static CString GetStrDate();

		/**
		* @brief 20091221 の形式で現在の日付を取得する
		* @return 「20091221」形式のint型の数値
		*/
		static int GetIntDate();
		
		/**
		* @brief 2009/12/21 の形式で現在の日付を取得する
		* @return 「2009/12/21」形式の文字列
		*/
		static CString GetStrDateSla();
};