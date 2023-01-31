/**
 * @file  JmfAnalys.cpp
 * @brief JMF����͂���
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
	* @brief CJmfAnalys �R���X�g���N�V����
	* @param  JmfData : [i]XML�f�[�^
**/
/* ==================================================================================================== */
CJmfAnalys::CJmfAnalys( const CString& JmfData )
	: m_JmfData(JmfData)
{
}

/* ==================================================================================================== */
/**
	* @brief CJmfAnalys �f�X�g���N�V����
**/
/* ==================================================================================================== */
CJmfAnalys::~CJmfAnalys()
{
}

/* ==================================================================================================== */
/**
	* @brief XML�f�[�^�̃p�[�X
	* @retval ERROR_SUCCESS(����[�`������])�A���̑�(�ُ�)
**/
/* ==================================================================================================== */
LONG CJmfAnalys::Perth()
{
	//Create document
	if ( m_pDocument.CreateInstance( CLSID_DOMDocument ) != S_OK )
	{	// �G���[
		return -1;
	}

	_variant_t vstr(m_JmfData);
	_bstr_t bstrXML = vstr;
	VARIANT_BOOL isSuccessful;
	if ( m_pDocument->loadXML(bstrXML, &isSuccessful) != S_OK )
	{	// �G���[
		return -2;
	}

	//Get root Node 
	if (  m_pDocument->get_documentElement( &m_pRootElement ) != S_OK )
	{	// �G���[
		return -3;
	}
	return ERROR_SUCCESS;
}

/* ==================================================================================================== */
/**
	* @brief XML�f�[�^�̃p�[�X
	* @param  OutPath : [i]�o�̓p�X
	* @retval ERROR_SUCCESS(����)�A���̑�(�ُ�)
**/
/* ==================================================================================================== */
LONG CJmfAnalys::Save(const CString& OutPath)
{
	_variant_t savepath( OutPath );
	if ( m_pDocument->save( savepath ) != S_OK)
	{	// �ۑ����s
		return -1;
	}
	return ERROR_SUCCESS;
}

/* ==================================================================================================== */
/**
	* @brief ��MJMF��ʎ擾
	* @retval JMF���
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