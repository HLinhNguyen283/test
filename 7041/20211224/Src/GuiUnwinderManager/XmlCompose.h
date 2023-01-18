/**
	* @file		XmlCompose.h
	* @brief	XMLの作成 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;

#include <map>

namespace unwinder_manager
{

typedef std::map<std::string, std::string> UwXjmfDataMap;

/**
* @brief XMLの作成クラス
*/
class CXmlCompose
{
public:
	/**
		* @brief  CXmlComposeコンストラクタ
		* @param  inUwXjmfDataMap : [i]XMLデータに設定する情報MAP
	**/
	CXmlCompose(UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlComposeデストラクタ
	**/
	~CXmlCompose();

	/**
		* @brief  XMLデータ作成
		* @retval 処理結果データ
		*		　正常の場合：作成したXMLデータ
		*		  異常の場合：[ERROR] エラー内容
	**/
	std::string Doit();

protected:
	/**
		* @brief 雛形XMLデータのロード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL LoadXml();

	/**
		* @brief XMLドキュメント作成
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL CreateXML();

	/**
		* @brief  XML文書(QueryStatus)作成
		* @retval TRUE：正常、FALSE：異常
	**/
	BOOL CreateQueryStatus();

	/**
		* @brief  XML文書(QueryResource)作成
		* @retval TRUE：正常、FALSE：異常
	**/
	BOOL CreateQueryResource();

	/**
		* @brief  XML文書(StopPersChParams)作成
		* @retval TRUE：正常、FALSE：異常
	**/
	BOOL CreateStopPersChParams();

	/**
		* @brief  XML文書(CommandResource)作成
		* @retval TRUE：正常、FALSE：異常
	**/
	BOOL CreateCommandResource();

	/**
		* @brief  XML文書(CommandWakeup)作成
		* @retval TRUE：正常、FALSE：異常
	**/
	BOOL CreateCommandWakeup();

	/**
		* @brief XMLデータの設定
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL SaveXML();

	/**
		* @brief  結果データにエラー情報を設定する
		* @param  inErrorMessage : [i]エラー情報
	**/
	void SetErrorResultData(const std::string& inErrorMessage);

protected:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument;			// XMLドキュメント
	MSXML2::IXMLDOMElementPtr	m_pRootElement;			// XMLのルートのElement
	UwXjmfDataMap&				m_UwXjmfDataMap;		// XMLのデータMAP
	std::string					m_ResultData;			// 処理結果
};

};	// namespace unwinder_manager