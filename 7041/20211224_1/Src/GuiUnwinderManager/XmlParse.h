/**
	* @file		XmlParse.h
	* @brief	XMLの解析 ヘッダーファイル
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
* @brief XMLの解析 Baseクラス
*/
class CXmlParse
{
	enum JmfType
	{	// レスポンスJMF種別
		E_JMF_Unknown,						// 不明
		E_JMF_ResponseStatus,				// チャネル登録のレスポンス
		E_JMF_Response,						// チャネル削除、用紙情報登録などのレスポンス
		E_JMF_SignalStatus,					// UWからのSignalStatus
	};

public:
	/**
		* @brief  CXmlParseコンストラクタ
		* @param  outUwXjmfDataMap : [o]XMLデータから取り出した情報
	**/
	CXmlParse(UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParseデストラクタ
	**/
	virtual ~CXmlParse();

protected:
	/**
		* @brief  XMLデータの解析
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL Analys();

	/**
		* @brief  XMLの種別取得
		* @param  outNode : [o]Xmlデータのノード
		* @retval XMLの種別
	**/
	JmfType GetXmlType(MSXML2::IXMLDOMNode** outNode);

	/**
		* @brief  XMLデータ(ResponseStatus)の解析
		*		  処理結果(ReturnCode)とチャネルID(refID)を取り出す
		* @param  inNode : [i]Xmlデータのノード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL ParseResponseStatus(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  XMLデータ(Response)の解析
		*		  処理結果(ReturnCode)を取り出す
		* @param  inNode : [i]Xmlデータのノード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL ParseResponse(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  XMLデータ(SignalStatus)の解析
		*		  Ststus通知の場合：UWの状態(Status)を取り出す
		*		  用紙情報通知の場合：DescriptiveName、Dimension、MediaType、RollDiameter、Thicknessを取り出す
		* @param  inNode : [i]Xmlデータのノード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL ParseSignalStatus(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  結果データにエラー情報を設定する
		* @param  inErrorMessage : [i]エラー情報
	**/
	void SetErrorResultData(const std::string& inErrorMessage);

	/**
		* @brief  結果データに正常終了情報を設定する
	**/
	void SetSuccessResultData();

protected:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument;			// XMLドキュメント
	MSXML2::IXMLDOMElementPtr	m_pRootElement;			// XMLのルートのElement
	UwXjmfDataMap&				m_UwXjmfDataMap;		// XMLのデータMAP
	std::string					m_ResultData;			// 処理結果
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief XMLファイルの解析 クラス
*/
class CXmlParseFile : public CXmlParse
{
public:
	/**
		* @brief  CXmlParseFileコンストラクタ(CXmlParseの派生クラス)
		* @param  inXmlFilePath    : [i]XMLファイルのパス
		* @param  outUwXjmfDataMap : [o]XMLファイルから取り出した情報
	**/
	CXmlParseFile(const std::string& inXmlFilePath, UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParseFileデストラクタ
	**/
	virtual ~CXmlParseFile();

	/**
		* @brief  XMLファイルのパース
		* @retval パース結果データ
	**/
	std::string Doit();

protected:
	/**
		* @brief XMLファイルのロード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL LoadXml();

private:
	std::string					m_XmlFilePath;			// XMLファイルのパス
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief XMLデータの解析 クラス
*/
class CXmlParseData : public CXmlParse
{
public:
	/**
		* @brief  CXmlParseDataコンストラクタ(CXmlParseの派生クラス)
		* @param  inXmlData        : [i]XMLデータ
		* @param  outUwXjmfDataMap : [o]XMLデータから取り出した情報
	**/
	CXmlParseData(const std::string& inXmlData, UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParseDataデストラクタ
	**/
	virtual ~CXmlParseData();

	/**
		* @brief  XMLデータのパース
		* @retval パース結果データ
	**/
	std::string Doit();

protected:
	/**
		* @brief XMLデータのロード
		* @retval TRUE:正常、FALSE:異常
	**/
	BOOL LoadXml();

private:
	std::string					m_XmlData;				// XMLデータ
};

};	// namespace unwinder_manager