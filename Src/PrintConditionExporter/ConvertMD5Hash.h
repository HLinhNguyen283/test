/**
 * @file	ConvertMD5Hash.h
 * @brief	MD5ハッシュ値変換クラス
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include <string>

/**
 * @brief	MD5ハッシュ値変換クラス
**/
class CConvertMD5Hash
{
private:

	/**
	 * @brief	constructor
	**/
	CConvertMD5Hash();

public:
	/**
	 * @brief	destructor
	**/
	~CConvertMD5Hash();

	/**
	 * @brief	デバッグファイル出力を行うかどうか設定する
	 * @param[in]	inIsOutput : デバッグファイル出力をするかどうか(true:する, false:しない)
	**/
	static void SetOutputDebugFile(bool inIsOutput);

	/**
	 * @brief	MD5ハッシュ値作成.
	 * @param[in]	inData : 入力データ
	 * @param[in]	inDataSize : 入力データサイズ
	 * @param[out]	outHashedData : MD5ハッシュ値
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool CreateHash(const BYTE* inData, DWORD inDataSize, std::string& outHashedData);

protected:

	/**
	 * @brief	CreateHash関数失敗のデバッグログ出力をする
	 * @param[in]	inMsg : 付加するメッセージ
	 * @param[in]	inErrNo : GetLastErrorのエラー番号(0の場合は、無視する)
	**/
	static void OutputCreateHashFailedLog(const std::string& inMsg, DWORD inErrNo);

	/**
	 * @brief	デバッグログ出力をする
	 * @param[in]	inMsg : 出力するメッセージ
	**/
	static void OutputDebugLog(const std::string& inMsg);

	/**
	 * @brief	デバッグファイル出力をする
	 * @param[in]	inData : データ
	 * @param[in]	inDataSize : データサイズ
	**/
	static void OutputDebugFile(const BYTE* inData, DWORD inDataSize);

private:
	static bool m_isOutputDebugFile;	//!< デバッグファイル出力をするかどうか(true:する, false:しない)
};
 