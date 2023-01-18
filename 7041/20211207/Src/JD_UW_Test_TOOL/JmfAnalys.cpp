/**
 * @file  JmfAnalys.cpp
 * @brief JMFを解析する
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "JmfAnalys.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief CJmfAnalys コンストラクション
	* @param  JmfData : [i]XMLデータ
**/
/* ==================================================================================================== */
CJmfAnalys::CJmfAnalys( const CString& JmfData )
	: m_JmfData(JmfData)
{
}

/* ==================================================================================================== */
/**
	* @brief CJmfAnalys デストラクション
**/
/* ==================================================================================================== */
CJmfAnalys::~CJmfAnalys()
{
}

/* ==================================================================================================== */
/**
	* @brief XMLデータのパース
	* @retval ERROR_SUCCESS(正常[形式正常])、その他(異常)
**/
/* ==================================================================================================== */
LONG CJmfAnalys::Perth()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// エラー
		return -1;
	}

	_variant_t vstr(m_JmfData);
	_bstr_t bstrXML = vstr;
	VARIANT_BOOL isSuccessful;
	if ( m_pDocument->loadXML(bstrXML, &isSuccessful) != S_OK )
	{	// エラー
		return -2;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// エラー
		return -3;
	}
	return ERROR_SUCCESS;
}

/* ==================================================================================================== */
/**
	* @brief XMLデータのパース
	* @param  OutPath : [i]出力パス
	* @retval ERROR_SUCCESS(正常)、その他(異常)
**/
/* ==================================================================================================== */
LONG CJmfAnalys::Save(const CString& OutPath)
{
	_variant_t savepath( OutPath );
	if ( m_pDocument->save( savepath ) != S_OK)
	{	// 保存失敗
		return -1;
	}
	return ERROR_SUCCESS;
}

/* ==================================================================================================== */
/**
	* @brief 受信JMF種別取得
	* @retval JMF種別
**/
/* ==================================================================================================== */
int CJmfAnalys::GetRecvJmfType()
{
	int nJmfType = -1;
	// 

	MSXML2::IXMLDOMNodePtr pNode;
	if ( m_pRootElement->selectSingleNode( L"//XJMF/QueryStatus", &pNode ) == S_OK )
	{	// 
		nJmfType = CCommon::E_JMF_QueryStatus;
	}
	else if ( m_pRootElement->selectSingleNode( L"//XJMF/QueryResource", &pNode ) == S_OK )
	{
		nJmfType = CCommon::E_JMF_QueryResource;
	}
	else if ( m_pRootElement->selectSingleNode( L"//XJMF/StopPersChParams", &pNode ) == S_OK )
	{
		nJmfType = CCommon::E_JMF_StopPersChParams;
	}
	else if ( m_pRootElement->selectSingleNode( L"//XJMF/CommandResource", &pNode ) == S_OK )
	{
		nJmfType = CCommon::E_JMF_CommandResource;
	}
	else if ( m_pRootElement->selectSingleNode( L"//XJMF/CommandWakeup", &pNode ) == S_OK )
	{
		nJmfType = CCommon::E_JMF_CommandWakeup;
	}
	else if ( m_pRootElement->selectSingleNode( L"//XJMF/SignalStatus", &pNode ) == S_OK )
	{
		nJmfType = CCommon::E_JMF_SignalStatus;
	}

	return nJmfType;
}