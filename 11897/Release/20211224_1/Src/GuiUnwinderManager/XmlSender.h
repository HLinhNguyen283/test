/**
	* @file		XmlSender.h
	* @brief	XMLログ関連モジュール ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include <map>

namespace unwinder_manager
{

/**
* @brief UWへXMLを通知するクラス
*/
class CXmlSender
{
public:
	/**
		* @brief  CXmlSenderコンストラクタ
		* @param  inXmlData : [i]通知するXMLデータ
	**/
	CXmlSender(const std::string& inXmlData);

	/**
		* @brief  CXmlSenderデストラクタ
	**/
	~CXmlSender();

	/**
		* @brief  XMLデータ通知
		* @retval 処理結果データ
		*		　正常の場合：レスポンスXMLデータ
		*		  異常の場合：[ERROR] エラー内容
	**/
	std::string Doit();

protected:
	/**
		* @brief  XMLデータ通知
	**/
	BOOL Send();

	/**
		* @brief  コード変換(UTF8 → Shift-JIS)
		* @param  InUTF8 : [i]UTF8の文字列
		* @retval Shift-JISの文字列
	**/
	std::string ConvUTF8toShiftJIS(const std::string& InUTF8);

	/**
		* @brief  結果データにエラー情報を設定する
		* @param  inErrorMessage : [i]エラー情報
	**/
	void SetErrorResultData(const std::string& inErrorMessage);
	
	/**
		* @brief  UWのIPアドレスを取得する
		* @retval UWのIPアドレス
	**/
	std::string GetUwIpAddress();
	
	/**
		* @brief  UWの受信フォルダ名を取得する
		* @retval UWの受信フォルダ名
	**/
	std::string GetObjName();
	
	/**
		* @brief  UWの受信ポート番号を取得する
		* @retval UWの受信ポート番号
	**/
	UINT GetSendPort();

	/**
		* @brief  通知時のリトライ値を取得する
		* @param  outRetryCount    : [o]リトライ回数
		* @param  outRetryInterval : [o]リトライのインターバル
	**/
	void GetRetryVal(UINT& outRetryCount, UINT& outRetryInterval);

	std::string					m_SendXmlData;		// 通知するXMLデータ
	std::string					m_ResultData;		// 処理結果
};

};	// namespace unwinder_manager