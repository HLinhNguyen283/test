/**
	* @file		XmlParse.cpp
	* @brief	XMLの解析
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <string>
#include <sstream>
#include "XmlParse.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CXmlParseコンストラクタ
	* @param  outUwXjmfDataMap : [o]XMLデータから取り出した情報
**/
CXmlParse::CXmlParse(UwRwXjmfDataMap& outUwXjmfDataMap)
	: m_UwXjmfDataMap(outUwXjmfDataMap)
{
	// COMの初期化
	::CoInitialize( NULL );
}

/**
	* @brief  CXmlParseデストラクタ
**/
CXmlParse::~CXmlParse()
{
	// Comの開放
	::CoUninitialize();
}

/**
	* @brief  XMLデータの解析
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParse::Analys()
{
	MSXML2::IXMLDOMNodePtr pNode;
	JmfType nJmfType = GetXmlType(&pNode);

	BOOL nRet = TRUE;
	switch( nJmfType ) {
	case E_JMF_ResponseStatus:			// チャネル登録のレスポンス
		nRet = ParseResponseStatus(pNode);
		break;
	case E_JMF_Response:				// チャネル削除、用紙情報登録などのレスポンス
		nRet = ParseResponse(pNode);
		break;
	case E_JMF_SignalStatus:			// UWからのSignalStatus
		nRet = ParseSignalStatus(pNode);
		break;
	default:
		// 対象外のXML
		nRet = FALSE;
		break;
	}

	return nRet;
}

/**
	* @brief  XMLの種別取得
	* @param  outNode : [o]Xmlデータのノード
	* @retval XMLの種別
**/
CXmlParse::JmfType CXmlParse::GetXmlType(MSXML2::IXMLDOMNode** outNode)
{
	JmfType nJmfType = E_JMF_Unknown;

	try
	{
		if ( m_pRootElement->selectSingleNode( L"//XJMF/ResponseStatus", outNode ) == S_OK )
		{
			nJmfType = E_JMF_ResponseStatus;
		}
		else if ( m_pRootElement->selectSingleNode( L"//XJMF/Response", outNode ) == S_OK )
		{
			nJmfType = E_JMF_Response;
		}
		else if ( m_pRootElement->selectSingleNode( L"//XJMF/SignalStatus", outNode ) == S_OK )
		{
			nJmfType = E_JMF_SignalStatus;
		}
		else
		{	// 知らないXML
			SetErrorResultData("[CParseXmlFile::GetXmlType] Unkown JmfType Error");
		}
	}
	catch(...)
	{
		SetErrorResultData("[CParseXmlFile::GetXmlType] Exception");
	}
	return nJmfType;
}

/**
	* @brief  XMLデータ(ResponseStatus)の解析
	*		  処理結果(ReturnCode)とチャネルID(refID)を取り出す
	* @param  inNode : [i]Xmlデータのノード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParse::ParseResponseStatus(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// 種別
		m_UwXjmfDataMap["Type"] = "ResponseStatus";

		MSXML2::IXMLDOMElementPtr pElement_ResponseStatus = 0;
		if ( inNode->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_ResponseStatus ) == S_OK )
		{
			pElement_ResponseStatus->getAttribute(L"ReturnCode",&val);
			if ( val.vt != VT_NULL )
			{
				bstr = val;
				m_UwXjmfDataMap["ReturnCode"] = bstr;
			}
			else
			{
				SetErrorResultData("[CXmlParse::ParseResponseStatus] not attribute to ReturnCode ERROR");
				return FALSE;
			}
		}
		else
		{
			SetErrorResultData("[CXmlParse::ParseResponseStatus] QueryInterface ResponseStatus ERROR");
			return FALSE;
		}

		// Header
		MSXML2::IXMLDOMNodePtr pNode_Header = 0;
		if ( inNode->selectSingleNode(L"Header", &pNode_Header) == S_OK )
		{
			MSXML2::IXMLDOMElementPtr pElement_Header = 0;
			if ( pNode_Header->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_Header ) == S_OK )
			{
				pElement_Header->getAttribute(L"refID",&val);
				if ( val.vt != VT_NULL )
				{
					bstr = val;
					m_UwXjmfDataMap["refID"] = bstr;
				}
				else
				{
					SetErrorResultData("[CXmlParse::ParseResponseStatus] not attribute to refID ERROR");
					return FALSE;
				}
			}
			else
			{
				SetErrorResultData("[CParseXmlFile::ParseSignalStatus] QueryInterface Header ERROR");
				return FALSE;
			}
		}
	}
	catch(...)
	{
		SetErrorResultData("[CXmlParse::ParseResponseStatus] Exception");
		return FALSE;
	}

	return TRUE;
}

/**
	* @brief  XMLデータ(Response)の解析
	*		  処理結果(ReturnCode)を取り出す
	* @param  inNode : [i]Xmlデータのノード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParse::ParseResponse(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// 種別
		m_UwXjmfDataMap["Type"] = "Response";

		MSXML2::IXMLDOMElementPtr pElement_Response = 0;
		if ( inNode->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_Response ) == S_OK )
		{
			pElement_Response->getAttribute(L"ReturnCode",&val);
			if ( val.vt != VT_NULL )
			{
				bstr = val;
				m_UwXjmfDataMap["ReturnCode"] = bstr;
			}
			else
			{
				SetErrorResultData("[CXmlParse::ParseResponse] not attribute to ReturnCode ERROR");
				return FALSE;
			}
		}
		else
		{
			SetErrorResultData("[CXmlParse::ParseResponse] QueryInterface ParseResponse ERROR");
			return FALSE;
		}
	}
	catch(...)
	{
		SetErrorResultData("[CXmlParse::ParseResponse] Exception");
		return FALSE;
	}

	return TRUE;
}

/**
	* @brief  XMLデータ(SignalStatus)の解析
	*		  Ststus通知の場合：UWの状態(Status)を取り出す
	*		  用紙情報通知の場合：DescriptiveName、Dimension、MediaType、RollDiameter、Thicknessを取り出す
	* @param  inNode : [i]Xmlデータのノード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParse::ParseSignalStatus(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// 種別
		m_UwXjmfDataMap["Type"] = "SignalStatus";

		// DeviceInfo
		MSXML2::IXMLDOMNodePtr pNode_DeviceInfo = 0;
		if ( inNode->selectSingleNode(L"DeviceInfo", &pNode_DeviceInfo) == S_OK )
		{
			MSXML2::IXMLDOMElementPtr pElement_DeviceInfo = 0;
			if ( pNode_DeviceInfo->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_DeviceInfo ) == S_OK )
			{
				m_UwXjmfDataMap["SubType"]="Status";
				pElement_DeviceInfo->getAttribute(L"Status",&val);
				if ( val.vt != VT_NULL )
				{
					bstr = val;
					m_UwXjmfDataMap["Status"] = bstr;
				}
				else
				{
					WriteToLogBuf(LOG_DEBUG, "[CParseXmlFile::ParseSignalStatus] Warning not attribute to Status" );
				}
			}
			else
			{
				SetErrorResultData("[CParseXmlFile::ParseSignalStatus] QueryInterface DeviceInfo ERROR");
				return FALSE;
			}
		}
		else
		{
			// Resource
			MSXML2::IXMLDOMNodePtr pNode_Resource = 0;
			if ( inNode->selectSingleNode(L"ResourceInfo/ResourceSet/Resource", &pNode_Resource) == S_OK )
			{
				MSXML2::IXMLDOMElementPtr pElement_Resource = 0;
				if ( pNode_Resource->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_Resource ) == S_OK )
				{
					m_UwXjmfDataMap["SubType"]="Paper";
					pElement_Resource->getAttribute(L"DescriptiveName",&val);
					if ( val.vt != VT_NULL )
					{
						bstr = val;
						m_UwXjmfDataMap["DescriptiveName"] = bstr;
					}
					else
					{
						WriteToLogBuf(LOG_DEBUG, "[CParseXmlFile::ParseSignalStatus] Warning not attribute to DescriptiveName" );
					}
					// Media
					MSXML2::IXMLDOMNodePtr pNode_Media = 0;
					if ( pNode_Resource->selectSingleNode(L"Media", &pNode_Media) == S_OK )
					{
						MSXML2::IXMLDOMElementPtr pElement_Media = 0;
						if ( pNode_Media->QueryInterface( IID_IXMLDOMElement, (void**)&pElement_Media ) == S_OK )
						{
							pElement_Media->getAttribute(L"Dimension",&val);
							if ( val.vt != VT_NULL )
							{
								bstr = val;
								m_UwXjmfDataMap["Dimension"] = bstr;
							}
							pElement_Media->getAttribute(L"MediaType",&val);
							if ( val.vt != VT_NULL )
							{
								bstr = val;
								m_UwXjmfDataMap["MediaType"] = bstr;
							}
							pElement_Media->getAttribute(L"scr:RollDiameter",&val);
							if ( val.vt != VT_NULL )
							{
								bstr = val;
								m_UwXjmfDataMap["RollDiameter"] = bstr;
							}
							pElement_Media->getAttribute(L"Thickness",&val);
							if ( val.vt != VT_NULL )
							{
								bstr = val;
								m_UwXjmfDataMap["Thickness"] = bstr;
							}
							pElement_Media->getAttribute(L"Weight", &val);
							if (val.vt != VT_NULL)
							{
								bstr = val;
								m_UwXjmfDataMap["Weight"] = bstr;
							}
							pElement_Media->getAttribute(L"scr:UWWebTension", &val);
							if (val.vt != VT_NULL)
							{
								bstr = val;
								m_UwXjmfDataMap["UWWebTension"] = bstr;
							}
							pElement_Media->getAttribute(L"scr:RWWebTension", &val);
							if (val.vt != VT_NULL)
							{
								bstr = val;
								m_UwXjmfDataMap["RWWebTension"] = bstr;
							}
							pElement_Media->getAttribute(L"scr:RWRollTension", &val);
							if (val.vt != VT_NULL)
							{
								bstr = val;
								m_UwXjmfDataMap["RWRollTension"] = bstr;
							}
							pElement_Media->getAttribute(L"scr:StopRollDiameter", &val);
							if (val.vt != VT_NULL)
							{
								bstr = val;
								m_UwXjmfDataMap["StopRollDiameter"] = bstr;
							}
						}
						else
						{
							SetErrorResultData("[CParseXmlFile::ParseSignalStatus] QueryInterface Media ERROR");
							return FALSE;
						}
					}
					else
					{
						WriteToLogBuf(LOG_DEBUG, "[CParseXmlFile::ParseSignalStatus] Warning Media not found" );
					}
				}
				else
				{
					SetErrorResultData("[CParseXmlFile::ParseSignalStatus] QueryInterface ResourceInfo/ResourceSet/Resource ERROR");
					return FALSE;
				}
			}
			else
			{
				WriteToLogBuf(LOG_DEBUG, "[CParseXmlFile::ParseSignalStatus] Warning not node to expect" );
			}
 		}
	}
	catch(...)
	{
		SetErrorResultData("[CParseXmlFile::ParseSignalStatus] Exception");
		return FALSE;
	}
	return TRUE;
}

/**
	* @brief  結果データにエラー情報を設定する
	* @param  inErrorMessage : [i]エラー情報
**/
void CXmlParse::SetErrorResultData(const std::string& inErrorMessage)
{
	m_ResultData = "[ERROR] " + inErrorMessage;
}

/**
	* @brief  結果データに正常終了情報を設定する
**/
void CXmlParse::SetSuccessResultData()
{
	std::string PickupData;
	for( auto ite=m_UwXjmfDataMap.begin(); ite != m_UwXjmfDataMap.end(); ite++ )
	{
		std::stringstream ss;
		ss << ite->first << "=" << ite->second << " ";
		PickupData.append(ss.str().c_str());
	}
	m_ResultData = "[SUCCESS] " + PickupData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
	* @brief  CXmlParseFileコンストラクタ(CXmlParseの派生クラス)
	* @param  inXmlFilePath    : [i]XMLファイルのパス
	* @param  outUwXjmfDataMap : [o]XMLファイルから取り出した情報
**/
CXmlParseFile::CXmlParseFile(const std::string& inXmlFilePath, UwRwXjmfDataMap& outUwXjmfDataMap)
	: CXmlParse(outUwXjmfDataMap)
	, m_XmlFilePath(inXmlFilePath)
{
}

/**
	* @brief  CXmlParseFileデストラクタ
**/
CXmlParseFile::~CXmlParseFile()
{
}

/**
	* @brief  XMLファイルのパース
	* @retval パース結果データ
**/
std::string CXmlParseFile::Doit()
{
	BOOL nRet = LoadXml();
	if ( nRet )
	{
		nRet = Analys();
	}
	if ( !nRet )
	{	// 異常の場合は異常内容をログに出力する
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	else
	{
		SetSuccessResultData();
	}

	return m_ResultData;
}

/**
	* @brief XMLファイルのロード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParseFile::LoadXml()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// エラー
		SetErrorResultData("[CParseXmlFile::LoadXml] CreateInstance ERROR");
		return FALSE;
	}

	//Load xml file
	VARIANT_BOOL bRet = 0;
	_variant_t path( m_XmlFilePath.c_str() );
	if ( m_pDocument->load( path, &bRet ) != S_OK )
	{
		std::stringstream ss;
		ss << "[CParseXmlFile::LoadXml] load Error XmlFilePath=" << m_XmlFilePath;
		SetErrorResultData(ss.str().c_str());
		return FALSE;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// エラー
		SetErrorResultData("[CParseXmlFile::LoadXml] get_documentElement ERROR");
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
	* @brief  CXmlParseDataコンストラクタ(CXmlParseの派生クラス)
	* @param  inXmlData        : [i]XMLデータ
	* @param  outUwXjmfDataMap : [o]XMLデータから取り出した情報
**/
CXmlParseData::CXmlParseData(const std::string& inXmlData, UwRwXjmfDataMap& outUwXjmfDataMap)
	: CXmlParse(outUwXjmfDataMap)
	, m_XmlData(inXmlData)
{
}

/**
	* @brief  CXmlParseDataデストラクタ
**/
CXmlParseData::~CXmlParseData()
{
}

/**
	* @brief  XMLデータのパース
	* @retval パース結果データ
**/
std::string CXmlParseData::Doit()
{
	BOOL nRet = LoadXml();
	if ( nRet )
	{
		nRet = Analys();
	}
	if ( !nRet )
	{	// 異常の場合は異常内容をログに出力する
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	else
	{
		SetSuccessResultData();
	}

	return m_ResultData;
}

/**
	* @brief XMLデータのロード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlParseData::LoadXml()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// エラー
		SetErrorResultData("[CParseXmlData::LoadXml] CreateInstance ERROR");
		return FALSE;
	}

	//Load xml Data
	_variant_t vstr(m_XmlData.c_str());
	_bstr_t bstrXML = vstr;
	VARIANT_BOOL isSuccessful;
	if ( m_pDocument->loadXML(bstrXML, &isSuccessful) != S_OK )
	{	// エラー
		SetErrorResultData("[CParseXmlData::LoadXml] loadXML ERROR");
		return FALSE;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// エラー
		SetErrorResultData("[CParseXmlData::LoadXml] get_documentElement ERROR");
		return FALSE;
	}

	return TRUE;
}


};	// namespace uwrw_manager