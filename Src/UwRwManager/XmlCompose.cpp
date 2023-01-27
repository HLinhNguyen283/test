/**
	* @file		XmlCompose.cpp
	* @brief	XMLの作成
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <string>
#include <atlcomcli.h>
#include "XmlLogData.h"
#include "XmlCompose.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CXmlComposeコンストラクタ
	* @param  inUwXjmfDataMap : [i]XMLデータに設定する情報MAP
**/
CXmlCompose::CXmlCompose(UwRwXjmfDataMap& inUwXjmfDataMap, const char* inDeviceName)
	: m_UwXjmfDataMap(inUwXjmfDataMap),
	  m_DeviceName(inDeviceName)
{
	// COMの初期化
	::CoInitialize( NULL );
}

/**
	* @brief  CXmlComposeデストラクタ
**/
CXmlCompose::~CXmlCompose()
{
	// Comの開放
	::CoUninitialize();
}

/**
	* @brief  XMLデータ作成
	* @retval 処理結果データ
	*		　正常の場合：作成したXMLデータ
	*		  異常の場合：[ERROR] エラー内容
**/
std::string CXmlCompose::Doit()
{
	BOOL nRet = LoadXml();
	if ( nRet )
	{
		nRet = CreateXML();
		if ( nRet )
		{
			nRet = SaveXML();
		}
	}
	if ( !nRet )
	{	// 異常の場合は異常内容をログに出力する
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	return m_ResultData;
}

/**
	* @brief 雛形XMLデータのロード
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlCompose::LoadXml()
{
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// エラー
		SetErrorResultData("[CXmlCompose::LoadXml] CreateInstance ERROR");
		return FALSE;
	}

	std::wstring wstrXML(L"<XJMF></XJMF>");
	_variant_t vstr(wstrXML.c_str());
	_bstr_t bstrXML = vstr;
	VARIANT_BOOL isSuccessful;
	if ( m_pDocument->loadXML(bstrXML, &isSuccessful) != S_OK )
	{	// エラー
		SetErrorResultData("[CXmlCompose::LoadXml] loadXML ERROR");
		return FALSE;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// エラー
		SetErrorResultData("[CXmlCompose::LoadXml] get_documentElement ERROR");
		return FALSE;
	}

	return TRUE;
}

/**
	* @brief XMLドキュメント作成
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlCompose::CreateXML()
{
	BOOL nRet = FALSE;
	std::string Type = m_UwXjmfDataMap["Type"];

	if (Type.compare("QueryStatus") == 0 )
	{
		// QueryStatus
		nRet = CreateQueryStatus();
	}
	else if (Type.compare("QueryResource") == 0)
	{
		// QueryResource
		nRet = CreateQueryResource();
	}
	else if (Type.compare("StopPersChParams") == 0)
	{
		// StopPersChParams
		nRet = CreateStopPersChParams();
	}
	else if (Type.compare("CommandResource") == 0)
	{
		// CommandResource
		nRet = CreateCommandResource();
	}
	else if (Type.compare("CommandWakeup") == 0)
	{
		// CommandWakeup
		nRet = CreateCommandWakeup();
	}
	return nRet;
}

/**
	* @brief  XML文書(QueryStatus)作成
	* @retval TRUE：正常、FALSE：異常
	* ([例]XML文書)
	<XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
　		<Header DeviceID="Truepress" ID="QS_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
		<QueryStatus>
　　			<Header DeviceID="Truepress" ID="QS_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
　　			<Subscription RepeatTime="30" URL="http://192.168.0.30:1234/xjmfurl"/>
　		</QueryStatus>
	</XJMF>
**/
BOOL CXmlCompose::CreateQueryStatus()
{
	MSXML2::IXMLDOMNode* pXMLOutNewChild;
	CComVariant varNodeType = MSXML2::NODE_ELEMENT;

	//
	// xmlns
	//
	_bstr_t bstr = L"http://www.CIP4.org/JDFSchema_2_0";
	_variant_t val = bstr;
	m_pRootElement->setAttribute(L"xmlns", val);

	//
	// Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] createNode Header ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeHeader,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] appendChild Header ERROR");
		return FALSE;
	}

	//
	// Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] QueryInterface Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	std::string DeviceID = m_UwXjmfDataMap["DeviceID"];
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"DeviceID", val );

	// ID
	std::string ID = m_UwXjmfDataMap["ID"];
	bstr = ID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"ID", val );

	// Time
	std::string Time = m_UwXjmfDataMap["Time"];
	bstr = Time.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"Time", val );

	//
	// QueryStatus Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeQueryStatus;
	if ( m_pDocument->createNode( varNodeType, L"QueryStatus", L"", &pNodeQueryStatus ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] createNode QueryStatus ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeQueryStatus,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] appendChild QueryStatus ERROR");
		return FALSE;
	}

	//
	// QueryStatus Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementQueryStatus = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementQueryStatus) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] QueryInterface QueryStatus ERROR");
		return FALSE;
	}

	//
	// QueryStatus/Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader2;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader2 ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] createNode QueryStatus/Header ERROR");
		return FALSE;
	}
	if ( pElementQueryStatus->appendChild(pNodeHeader2,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] appendChild QueryStatus/Header ERROR");
		return FALSE;
	}

	//
	// QueryStatus/Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader2 = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader2) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] QueryInterface QueryStatus/Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"DeviceID", val );

	// ID
	bstr = ID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"ID", val );

	// Time
	bstr = Time.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"Time", val );
	
	
	//
	// QueryStatus/Subscription Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeSubscription;
	if ( m_pDocument->createNode( varNodeType, L"Subscription", L"", &pNodeSubscription ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] createNode QueryStatus/Subscription ERROR");
		return FALSE;
	}
	if ( pElementQueryStatus->appendChild(pNodeSubscription,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] appendChild QueryStatus/Subscription ERROR");
		return FALSE;
	}

	//
	// QueryStatus/Subscription Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementSubscription = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementSubscription) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryStatus] QueryInterface QueryStatus/Subscription ERROR");
		return FALSE;
	}

	//
	// RepeatTime
	//
	std::string RepeatTime = m_UwXjmfDataMap["RepeatTime"];
	bstr = RepeatTime.c_str();
	val = bstr;
	pElementSubscription->setAttribute( L"RepeatTime", val );

	//
	// URL
	//
	std::string URL = m_UwXjmfDataMap["URL"];
	bstr = URL.c_str();
	val = bstr;
	pElementSubscription->setAttribute( L"URL", val );

	return TRUE;
}

/**
	* @brief  XML文書(QueryResource)作成
	* @retval TRUE：正常、FALSE：異常
    * ([例]XML文書)
    <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
　      <Header DeviceID="Truepress" ID="QR_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
        <QueryResource>
　　          <Header DeviceID="Truepress" ID="QR_20210720_00004" Time="2021-07-20T10:23:58+09:00" />
　　          <Subscription RepeatTime="30" ChannelMode="FireAndForget" URL="http://192.168.0.30:1234/xjmfurl"/>
              <ResourceQuParams ResourceDetails="Full" ResourceName="Media" Scope="Present" />
　       </QueryResource>
    </XJMF>
**/
BOOL CXmlCompose::CreateQueryResource()
{
	MSXML2::IXMLDOMNode* pXMLOutNewChild;
	CComVariant varNodeType = MSXML2::NODE_ELEMENT;

	//
	// xmlns
	//
	_bstr_t bstr = L"http://www.CIP4.org/JDFSchema_2_0";
	_variant_t val = bstr;
	m_pRootElement->setAttribute(L"xmlns", val);

	//
	// Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode Header ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeHeader,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild Header ERROR");
		return FALSE;
	}

	//
	// Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] QueryInterface Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	std::string DeviceID = m_UwXjmfDataMap["DeviceID"];
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"DeviceID", val );

	// ID
	std::string ID = m_UwXjmfDataMap["ID"];
	bstr = ID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"ID", val );

	// Time
	std::string Time = m_UwXjmfDataMap["Time"];
	bstr = Time.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"Time", val );

	//
	// QueryResource Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeQueryResource;
	if ( m_pDocument->createNode( varNodeType, L"QueryResource", L"", &pNodeQueryResource ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode QueryResource ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeQueryResource,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild QueryResource ERROR");
		return FALSE;
	}

	//
	// QueryResource Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementQueryResource = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementQueryResource) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] QueryInterface QueryResource ERROR");
		return FALSE;
	}

	//
	// QueryResource/Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader2;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader2 ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode QueryResource/Header ERROR");
		return FALSE;
	}
	if ( pElementQueryResource->appendChild(pNodeHeader2,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild QueryResource/Header ERROR");
		return FALSE;
	}

	//
	// QueryResource/Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader2 = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader2) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] QueryInterface QueryResource/Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"DeviceID", val );

	// ID
	bstr = ID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"ID", val );

	// Time
	bstr = Time.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"Time", val );
	
	
	//
	// QueryResource/Subscription Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeSubscription;
	if ( m_pDocument->createNode( varNodeType, L"Subscription", L"", &pNodeSubscription ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode QueryResource/Subscription ERROR");
		return FALSE;
	}
	if ( pElementQueryResource->appendChild(pNodeSubscription,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild QueryResource/Subscription ERROR");
		return FALSE;
	}

	//
	// QueryResource/Subscription Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementSubscription = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementSubscription) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] QueryInterface QueryResource/Subscription ERROR");
		return FALSE;
	}

	//
	// RepeatTime
	//
	std::string RepeatTime = m_UwXjmfDataMap["RepeatTime"];
	bstr = RepeatTime.c_str();
	val = bstr;
	pElementSubscription->setAttribute( L"RepeatTime", val );

	//
	// URL
	//
	std::string URL = m_UwXjmfDataMap["URL"];
	bstr = URL.c_str();
	val = bstr;
	pElementSubscription->setAttribute( L"URL", val );

	//
	// ChannelMode
	//
	std::string ChannelMode = m_UwXjmfDataMap["ChannelMode"];
	bstr = ChannelMode.c_str();
	val = bstr;
	pElementSubscription->setAttribute( L"ChannelMode", val );

	//
	// QueryResource/ResourceQuParams Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeResourceQuParams;
	if ( m_pDocument->createNode( varNodeType, L"ResourceQuParams", L"", &pNodeResourceQuParams ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode QueryResource/ResourceQuParams ERROR");
		return FALSE;
	}
	if ( pElementQueryResource->appendChild(pNodeResourceQuParams,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild QueryResource/ResourceQuParams ERROR");
		return FALSE;
	}

	//
	// QueryResource/ResourceQuParams Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementResourceQuParams = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementResourceQuParams) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] QueryInterface QueryResource/ResourceQuParams ERROR");
		return FALSE;
	}

	//
	// ResourceDetails
	//
	std::string ResourceDetails = m_UwXjmfDataMap["ResourceDetails"];
	bstr = ResourceDetails.c_str();
	val = bstr;
	pElementResourceQuParams->setAttribute( L"ResourceDetails", val );

	//
	// ResourceName
	//
	std::string ResourceName = m_UwXjmfDataMap["ResourceName"];
	bstr = ResourceName.c_str();
	val = bstr;
	pElementResourceQuParams->setAttribute( L"ResourceName", val );

	//
	// Scope
	//
	std::string Scope = m_UwXjmfDataMap["Scope"];
	bstr = Scope.c_str();
	val = bstr;
	pElementResourceQuParams->setAttribute( L"Scope", val );

	return TRUE;
}

/**
	* @brief  XML文書(StopPersChParams)作成
	* @retval TRUE：正常、FALSE：異常
    * ([例]XML文書)
    <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
        <Header DeviceID="Truepress" ID="ST_20210720_00001" Time="2021-07-20T10:25:07+09:00" />
        <StopPersChParams ChannelID="Q1" MessageType="Media" URL="http://192.168.0.30:1234/xjmfurl" />
    </XJMF>
**/
BOOL CXmlCompose::CreateStopPersChParams()
{
	MSXML2::IXMLDOMNode* pXMLOutNewChild;
	CComVariant varNodeType = MSXML2::NODE_ELEMENT;

	//
	// xmlns
	//
	_bstr_t bstr = L"http://www.CIP4.org/JDFSchema_2_0";
	_variant_t val = bstr;
	m_pRootElement->setAttribute(L"xmlns", val);

	//
	// Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] createNode Header ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeHeader,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] appendChild Header ERROR");
		return FALSE;
	}

	//
	// Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] QueryInterface Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	std::string DeviceID = m_UwXjmfDataMap["DeviceID"];
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"DeviceID", val );

	// ID
	std::string ID = m_UwXjmfDataMap["ID"];
	bstr = ID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"ID", val );

	// Time
	std::string Time = m_UwXjmfDataMap["Time"];
	bstr = Time.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"Time", val );

	//
	// StopPersChParams Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeStopPersChParams;
	if ( m_pDocument->createNode( varNodeType, L"StopPersChParams", L"", &pNodeStopPersChParams ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] createNode StopPersChParams ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeStopPersChParams,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] appendChild StopPersChParams ERROR");
		return FALSE;
	}

	//
	// StopPersChParams Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementStopPersChParams = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementStopPersChParams) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateStopPersChParams] QueryInterface StopPersChParams ERROR");
		return FALSE;
	}

	//
	// ChannelID
	//
	std::string ChannelID = m_UwXjmfDataMap["ChannelID"];
	bstr = ChannelID.c_str();
	val = bstr;
	pElementStopPersChParams->setAttribute( L"ChannelID", val );

	//
	// MessageType
	//
	std::string MessageType = m_UwXjmfDataMap["MessageType"];
	bstr = MessageType.c_str();
	val = bstr;
	pElementStopPersChParams->setAttribute( L"MessageType", val );

	//
	// URL
	//
	std::string URL = m_UwXjmfDataMap["URL"];
	bstr = URL.c_str();
	val = bstr;
	pElementStopPersChParams->setAttribute( L"URL", val );

	return TRUE;
}

/**
	* @brief  XML文書(CommandResource)作成
	* @retval TRUE：正常、FALSE：異常
    * ([例]XML文書)
    <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0" xmlns:scr="http://www.screen.co.jp/JDF/JDFSchema_3_0">
        <Header DeviceID="Truepress" ID="CR_20210720_00001" Time="2021-07-20T10:25:46+09:00" />
        <CommandResource>
            <Header DeviceID="Truepress" ID="CR_20210720_00001" Time="2021-07-20T10:25:46+09:00" />
            <ResourceCmdParams UpdateMethod="CompleteSet">
                <ResourceSet Name="Media">
                    <Resource DescriptiveName="NextIJ #1" ExternalID="ID_1">
                        <Media Dimension="520.00 305.00" MediaType="NextIJ" Thickness="130" scr:UWTension="5" scr:MaxRunSpeed="120" />
                    </Resource>
                </ResourceSet>
            </ResourceCmdParams>
        </CommandResource>
    </XJMF>
**/
BOOL CXmlCompose::CreateCommandResource()
{
	MSXML2::IXMLDOMNode* pXMLOutNewChild;
	CComVariant varNodeType = MSXML2::NODE_ELEMENT;

	//
	// xmlns
	//
	_bstr_t bstr = L"http://www.CIP4.org/JDFSchema_2_0";
	_variant_t val = bstr;
	m_pRootElement->setAttribute(L"xmlns", val);

	bstr = L"http://www.screen.co.jp/JDF/JDFSchema_3_0";
	val = bstr;
	m_pRootElement->setAttribute(L"xmlns:scr", val);

	//
	// Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode Header ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeHeader,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild Header ERROR");
		return FALSE;
	}

	//
	// Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	std::string DeviceID = m_UwXjmfDataMap["DeviceID"];
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"DeviceID", val );

	// ID
	std::string ID = m_UwXjmfDataMap["ID"];
	bstr = ID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"ID", val );

	// Time
	std::string Time = m_UwXjmfDataMap["Time"];
	bstr = Time.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"Time", val );

	//
	// CommandResource Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeCommandResource;
	if ( m_pDocument->createNode( varNodeType, L"CommandResource", L"", &pNodeCommandResource ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode CommandResource ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeCommandResource,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild CommandResource ERROR");
		return FALSE;
	}

	//
	// CommandResource Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementCommandResource = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementCommandResource) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource ERROR");
		return FALSE;
	}

	//
	// CommandResource/Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader2;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader2 ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] createNode CommandResource/Header ERROR");
		return FALSE;
	}
	if ( pElementCommandResource->appendChild(pNodeHeader2,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateQueryResource] appendChild CommandResource/Header ERROR");
		return FALSE;
	}

	//
	// CommandResource/Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader2 = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader2) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource/Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"DeviceID", val );

	// ID
	bstr = ID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"ID", val );

	// Time
	bstr = Time.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"Time", val );
	
	
	//
	// CommandResource/ResourceCmdParams Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeResourceCmdParams;
	if ( m_pDocument->createNode( varNodeType, L"ResourceCmdParams", L"", &pNodeResourceCmdParams ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode CommandResource/ResourceCmdParams ERROR");
		return FALSE;
	}
	if ( pElementCommandResource->appendChild(pNodeResourceCmdParams,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild CommandResource/ResourceCmdParams ERROR");
		return FALSE;
	}

	//
	// CommandResource/ResourceCmdParams Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementResourceCmdParams = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementResourceCmdParams) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource/ResourceCmdParams ERROR");
		return FALSE;
	}

	//
	// UpdateMethod
	//
	std::string UpdateMethod = m_UwXjmfDataMap["UpdateMethod"];
	bstr = UpdateMethod.c_str();
	val = bstr;
	pElementResourceCmdParams->setAttribute( L"UpdateMethod", val );

	//
	// CommandResource/ResourceCmdParams/ResourceSet Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeResourceSet;
	if ( m_pDocument->createNode( varNodeType, L"ResourceSet", L"", &pNodeResourceSet ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode CommandResource/ResourceCmdParams/ResourceSet ERROR");
		return FALSE;
	}
	if ( pElementResourceCmdParams->appendChild(pNodeResourceSet,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild CommandResource/ResourceCmdParams/ResourceSet ERROR");
		return FALSE;
	}

	//
	// CommandResource/ResourceCmdParams/ResourceSet Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementResourceSet = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementResourceSet) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource/ResourceCmdParams/ResourceSet ERROR");
		return FALSE;
	}

	//
	// Name
	//
	std::string Name = m_UwXjmfDataMap["Name"];
	bstr = Name.c_str();
	val = bstr;
	pElementResourceSet->setAttribute( L"Name", val );

	//
	// CommandResource/ResourceCmdParams/ResourceSet/Resource Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeResource;
	if ( m_pDocument->createNode( varNodeType, L"Resource", L"", &pNodeResource ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode CommandResource/ResourceCmdParams/ResourceSet/Resource ERROR");
		return FALSE;
	}
	if ( pElementResourceSet->appendChild(pNodeResource,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild CommandResource/ResourceCmdParams/ResourceSet/Resource ERROR");
		return FALSE;
	}

	//
	// CommandResource/ResourceCmdParams/ResourceSet/Resource Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementResource = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementResource) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource/ResourceCmdParams/ResourceSet/Resource ERROR");
		return FALSE;
	}

	//
	// DescriptiveName
	//
	std::string DescriptiveName = m_UwXjmfDataMap["DescriptiveName"];
	bstr = DescriptiveName.c_str();
	val = bstr;
	pElementResource->setAttribute( L"DescriptiveName", val );

	//
	// ExternalID
	//
	std::string ExternalID = m_UwXjmfDataMap["ExternalID"];
	bstr = ExternalID.c_str();
	val = bstr;
	pElementResource->setAttribute( L"ExternalID", val );

	//
	// CommandResource/ResourceCmdParams/ResourceSet/Resource/Media Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeMedia;
	if ( m_pDocument->createNode( varNodeType, L"Media", L"", &pNodeMedia ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] createNode CommandResource/ResourceCmdParams/ResourceSet/Resource/Media ERROR");
		return FALSE;
	}
	if ( pElementResource->appendChild(pNodeMedia,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] appendChild CommandResource/ResourceCmdParams/ResourceSet/Resource/Media ERROR");
		return FALSE;
	}

	//
	// CommandResource/ResourceCmdParams/ResourceSet/Resource/Media Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementMedia = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementMedia) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandResource] QueryInterface CommandResource/ResourceCmdParams/ResourceSet/Resource/Media ERROR");
		return FALSE;
	}

	//
	// Dimension
	//
	std::string Dimension = m_UwXjmfDataMap["Dimension"];
	bstr = Dimension.c_str();
	val = bstr;
	pElementMedia->setAttribute( L"Dimension", val );

	//
	// MediaType
	//
	std::string MediaType = m_UwXjmfDataMap["MediaType"];
	bstr = MediaType.c_str();
	val = bstr;
	pElementMedia->setAttribute( L"MediaType", val );

	//
	// Weight
	//
	std::string Weight = m_UwXjmfDataMap["Weight"];
	bstr = Weight.c_str();
	val = bstr;
	pElementMedia->setAttribute(L"Weight", val);

	//
	// Thickness
	//
	std::string Thickness = m_UwXjmfDataMap["Thickness"];
	bstr = Thickness.c_str();
	val = bstr;
	pElementMedia->setAttribute( L"Thickness", val );

	//
	// UWDancerWebTension
	//
	std::string UWDancerWebTension = m_UwXjmfDataMap["UWDancerWebTension"];
	bstr = UWDancerWebTension.c_str();
	val = bstr;
	pElementMedia->setAttribute( L"scr:UWDancerWebTension", val );

	//
	// RWDancerWebTension
	//
	std::string RWDancerWebTension = m_UwXjmfDataMap["RWDancerWebTension"];
	bstr = RWDancerWebTension.c_str();
	val = bstr;
	pElementMedia->setAttribute( L"scr:RWDancerWebTension", val );

	//
	// RWWindingTension
	//
	std::string RWWindingTension = m_UwXjmfDataMap["RWWindingTension"];
	bstr = RWWindingTension.c_str();
	val = bstr;
	pElementMedia->setAttribute(L"scr:RWWindingTension", val);

	return TRUE;
}

/**
	* @brief  XML文書(CommandWakeup)作成
	* @retval TRUE：正常、FALSE：異常
    * ([例]XML文書)
    <XJMF xmlns="http://www.CIP4.org/JDFSchema_2_0">
        <Header DeviceID="Truepress" ID="CW_20210720_00001" Time="2021-07-20T10:26:11+09:00" />
        <CommandWakeup>
            <Header DeviceID="Truepress" ID="CW_20210720_00001" Time="2021-07-20T10:26:11+09:00" />
        </CommandWakeup>
    </XJMF>
**/
BOOL CXmlCompose::CreateCommandWakeup()
{
	MSXML2::IXMLDOMNode* pXMLOutNewChild;
	CComVariant varNodeType = MSXML2::NODE_ELEMENT;

	//
	// xmlns
	//
	_bstr_t bstr = L"http://www.CIP4.org/JDFSchema_2_0";
	_variant_t val = bstr;
	m_pRootElement->setAttribute(L"xmlns", val);

	//
	// Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] createNode Header ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeHeader,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] appendChild Header ERROR");
		return FALSE;
	}

	//
	// Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] QueryInterface Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	std::string DeviceID = m_UwXjmfDataMap["DeviceID"];
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"DeviceID", val );

	// ID
	std::string ID = m_UwXjmfDataMap["ID"];
	bstr = ID.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"ID", val );

	// Time
	std::string Time = m_UwXjmfDataMap["Time"];
	bstr = Time.c_str();
	val = bstr;
	pElementHeader->setAttribute( L"Time", val );

	//
	// CommandWakeup Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeCommandWakeup;
	if ( m_pDocument->createNode( varNodeType, L"CommandWakeup", L"", &pNodeCommandWakeup ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] createNode CommandWakeup ERROR");
		return FALSE;
	}
	if ( m_pRootElement->appendChild(pNodeCommandWakeup,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] appendChild CommandWakeup ERROR");
		return FALSE;
	}

	//
	// CommandWakeup Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementCommandWakeup = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementCommandWakeup) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] QueryInterface CommandWakeup ERROR");
		return FALSE;
	}

	//
	// CommandWakeup/Header Nodeを追加
	//
	MSXML2::IXMLDOMNode* pNodeHeader2;
	if ( m_pDocument->createNode( varNodeType, L"Header", L"", &pNodeHeader2 ) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] createNode CommandWakeup/Header ERROR");
		return FALSE;
	}
	if ( pElementCommandWakeup->appendChild(pNodeHeader2,&pXMLOutNewChild) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] appendChild CommandWakeup/Header ERROR");
		return FALSE;
	}

	//
	// CommandWakeup/Header Elementを追加
	//
	MSXML2::IXMLDOMElementPtr pElementHeader2 = 0;
	if ( pXMLOutNewChild->QueryInterface(IID_IXMLDOMElement, (void**)&pElementHeader2) != S_OK )
	{
		SetErrorResultData("[CXmlCompose::CreateCommandWakeup] QueryInterface CommandWakeup/Header ERROR");
		return FALSE;
	}

	//
	// DeviceID
	//
	bstr = DeviceID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"DeviceID", val );

	// ID
	bstr = ID.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"ID", val );

	// Time
	bstr = Time.c_str();
	val = bstr;
	pElementHeader2->setAttribute( L"Time", val );

	return TRUE;
}

/**
	* @brief XMLデータの設定
	* @retval TRUE:正常、FALSE:異常
**/
BOOL CXmlCompose::SaveXML()
{
	std::string strSavePath = CXmlLogData::GetSendLogFilePath(m_DeviceName);
	_variant_t vSavePath( strSavePath.c_str() );
	m_pDocument->save(vSavePath);

	if ( !CXmlLogData::ReadFile( strSavePath, m_ResultData ) )
	{
		SetErrorResultData("[CXmlCompose::SaveXML] ReadFile ERROR");
		return FALSE;
	}

	return TRUE;
}

/**
	* @brief  結果データにエラー情報を設定する
	* @param  inErrorMessage : [i]エラー情報
**/
void CXmlCompose::SetErrorResultData(const std::string& inErrorMessage)
{
	m_ResultData = "[ERROR] " + inErrorMessage;
}

};	// namespace uwrw_manager