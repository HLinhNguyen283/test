/**
	* @file		Common.h
	* @brief	共通定義 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
 **/

#pragma once

/**
* @brief 共通APIクラス
*/
class CCommon
{
public:
	enum
	{	// スレッド間要求
		WM_USER_SIGNAL_STATUS = (WM_USER+100),	// SignalStatusをPipe出力要求
	};

	/**
		* @brief  モジュールフォルダを取得する
		* @retval モジュールフォルダ
	**/
	static CString GetModuleDirectory();

	/**
		* @brief  指定ファイルをメモリに読み込む
		* @param  inFilePath	: [i]ファイルパス
		* @param  outData		: [o]ファイルの内容
		* @retval TRUE=正常、FALSE=異常
	**/
	static BOOL ReadFile( const CString& inFilePath, CString& outData );

};