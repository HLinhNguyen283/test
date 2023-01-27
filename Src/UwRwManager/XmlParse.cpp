/**
	* @file		XmlParse.cpp
	* @brief	XML�̉��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
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
	* @brief  CXmlParse�R���X�g���N�^
	* @param  outUwXjmfDataMap : [o]XML�f�[�^������o�������
**/
CXmlParse::CXmlParse(UwRwXjmfDataMap& outUwXjmfDataMap)
	: m_UwXjmfDataMap(outUwXjmfDataMap)
{
	// COM�̏�����
	::CoInitialize( NULL );
}

/**
	* @brief  CXmlParse�f�X�g���N�^
**/
CXmlParse::~CXmlParse()
{
	// Com�̊J��
	::CoUninitialize();
}

/**
	* @brief  XML�f�[�^�̉��
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParse::Analys()
{
	MSXML2::IXMLDOMNodePtr pNode;
	JmfType nJmfType = GetXmlType(&pNode);

	BOOL nRet = TRUE;
	switch( nJmfType ) {
	case E_JMF_ResponseStatus:			// �`���l���o�^�̃��X�|���X
		nRet = ParseResponseStatus(pNode);
		break;
	case E_JMF_Response:				// �`���l���폜�A�p�����o�^�Ȃǂ̃��X�|���X
		nRet = ParseResponse(pNode);
		break;
	case E_JMF_SignalStatus:			// UW�����SignalStatus
		nRet = ParseSignalStatus(pNode);
		break;
	default:
		// �ΏۊO��XML
		nRet = FALSE;
		break;
	}

	return nRet;
}

/**
	* @brief  XML�̎�ʎ擾
	* @param  outNode : [o]Xml�f�[�^�̃m�[�h
	* @retval XML�̎��
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
		{	// �m��Ȃ�XML
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
	* @brief  XML�f�[�^(ResponseStatus)�̉��
	*		  ��������(ReturnCode)�ƃ`���l��ID(refID)�����o��
	* @param  inNode : [i]Xml�f�[�^�̃m�[�h
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParse::ParseResponseStatus(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// ���
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
	* @brief  XML�f�[�^(Response)�̉��
	*		  ��������(ReturnCode)�����o��
	* @param  inNode : [i]Xml�f�[�^�̃m�[�h
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParse::ParseResponse(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// ���
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
	* @brief  XML�f�[�^(SignalStatus)�̉��
	*		  Ststus�ʒm�̏ꍇ�FUW�̏��(Status)�����o��
	*		  �p�����ʒm�̏ꍇ�FDescriptiveName�ADimension�AMediaType�ARollDiameter�AThickness�����o��
	* @param  inNode : [i]Xml�f�[�^�̃m�[�h
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParse::ParseSignalStatus(MSXML2::IXMLDOMNodePtr inNode)
{
	_variant_t	val;
	_bstr_t		bstr;

	try
	{
		// ���
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
	* @brief  ���ʃf�[�^�ɃG���[����ݒ肷��
	* @param  inErrorMessage : [i]�G���[���
**/
void CXmlParse::SetErrorResultData(const std::string& inErrorMessage)
{
	m_ResultData = "[ERROR] " + inErrorMessage;
}

/**
	* @brief  ���ʃf�[�^�ɐ���I������ݒ肷��
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
	* @brief  CXmlParseFile�R���X�g���N�^(CXmlParse�̔h���N���X)
	* @param  inXmlFilePath    : [i]XML�t�@�C���̃p�X
	* @param  outUwXjmfDataMap : [o]XML�t�@�C��������o�������
**/
CXmlParseFile::CXmlParseFile(const std::string& inXmlFilePath, UwRwXjmfDataMap& outUwXjmfDataMap)
	: CXmlParse(outUwXjmfDataMap)
	, m_XmlFilePath(inXmlFilePath)
{
}

/**
	* @brief  CXmlParseFile�f�X�g���N�^
**/
CXmlParseFile::~CXmlParseFile()
{
}

/**
	* @brief  XML�t�@�C���̃p�[�X
	* @retval �p�[�X���ʃf�[�^
**/
std::string CXmlParseFile::Doit()
{
	BOOL nRet = LoadXml();
	if ( nRet )
	{
		nRet = Analys();
	}
	if ( !nRet )
	{	// �ُ�̏ꍇ�ُ͈���e�����O�ɏo�͂���
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	else
	{
		SetSuccessResultData();
	}

	return m_ResultData;
}

/**
	* @brief XML�t�@�C���̃��[�h
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParseFile::LoadXml()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// �G���[
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
	{	// �G���[
		SetErrorResultData("[CParseXmlFile::LoadXml] get_documentElement ERROR");
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
	* @brief  CXmlParseData�R���X�g���N�^(CXmlParse�̔h���N���X)
	* @param  inXmlData        : [i]XML�f�[�^
	* @param  outUwXjmfDataMap : [o]XML�f�[�^������o�������
**/
CXmlParseData::CXmlParseData(const std::string& inXmlData, UwRwXjmfDataMap& outUwXjmfDataMap)
	: CXmlParse(outUwXjmfDataMap)
	, m_XmlData(inXmlData)
{
}

/**
	* @brief  CXmlParseData�f�X�g���N�^
**/
CXmlParseData::~CXmlParseData()
{
}

/**
	* @brief  XML�f�[�^�̃p�[�X
	* @retval �p�[�X���ʃf�[�^
**/
std::string CXmlParseData::Doit()
{
	BOOL nRet = LoadXml();
	if ( nRet )
	{
		nRet = Analys();
	}
	if ( !nRet )
	{	// �ُ�̏ꍇ�ُ͈���e�����O�ɏo�͂���
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	else
	{
		SetSuccessResultData();
	}

	return m_ResultData;
}

/**
	* @brief XML�f�[�^�̃��[�h
	* @retval TRUE:����AFALSE:�ُ�
**/
BOOL CXmlParseData::LoadXml()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// �G���[
		SetErrorResultData("[CParseXmlData::LoadXml] CreateInstance ERROR");
		return FALSE;
	}

	//Load xml Data
	_variant_t vstr(m_XmlData.c_str());
	_bstr_t bstrXML = vstr;
	VARIANT_BOOL isSuccessful;
	if ( m_pDocument->loadXML(bstrXML, &isSuccessful) != S_OK )
	{	// �G���[
		SetErrorResultData("[CParseXmlData::LoadXml] loadXML ERROR");
		return FALSE;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// �G���[
		SetErrorResultData("[CParseXmlData::LoadXml] get_documentElement ERROR");
		return FALSE;
	}

	return TRUE;
}


};	// namespace uwrw_manager