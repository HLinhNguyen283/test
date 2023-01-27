/**
	* @file		XmlLogData.h
	* @brief	UWへXMLを通知する ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include <map>

namespace uwrw_manager
{


/**
* @brief UWへXMLを通知するクラス
*/
class CXmlLogData
{
public:
	/**
		* @brief  XML送信ログファイルパス取得
		* @retval XML送信ログファイルパス
	**/
	static std::string GetSendLogFilePath(const char* inDeviceName);

	/**
		* @brief  XMLレスポンスログファイルパス取得
		* @retval XMLレスポンスログファイルパス
	**/
	static std::string GetResponseLogFilePath(const char* inDeviceName);

	/**
		* @brief  指定ファイルを指定メモリに読み込む
		* @param  inFilePath	: [i]ファイルパス
		* @param  outData		: [o]ファイルの内容
		* @retval TRUE=正常、FALSE=異常
	**/
	static BOOL ReadFile( const std::string& inFilePath, std::string& outData );

	/**
		* @brief  指定ファイルに指定内容を書き込む
		* @param  inFilePath	: [i]ファイルパス
		* @param  inData		: [i]書き込みデータ
		* @retval TRUE=正常、FALSE=異常
	**/
	static BOOL WriteFile( const std::string& inFilePath, std::string& inData );

	/**
		* @brief  現在日付を取得する
		* @retval 現在日付(yyyyMMdd)
	**/
	static std::string CXmlLogData::GetStrDate();

protected:
	/**
		* @brief  モジュールフォルダを取得する
		* @retval モジュールフォルダ
	**/
	static std::string GetModuleDirectory();

	/**
		* @brief  現在日のXMLログフォルダを取得する
		* @retval 現在日のXMLログフォルダ
	**/
	static std::string GetLogFolderPath(const char* inDeviceName);

	/**
		* @brief  指定のXMLログファイルパスを取得する
		* @param  inName : [i]ファイルの接頭文字
		* @retval 指定のXMLログファイルパス
	**/
	static std::string GetLogFilePath(const std::string& inName, const char* inDeviceName);

	/**
		* @brief  現在日時(＋ミリ秒)を取得する
		* @retval 現在日時(yyyyMMddhhmmss_999)
	**/
	static std::string CXmlLogData::GetStrDateTime();
};

};	// namespace uwrw_manager