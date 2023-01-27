/**
	* @file		XmlSender.cpp
	* @brief	UW��XML��ʒm����
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <http.h>
#include <wininet.h>
#include <atlconv.h>
#include "Ini_UwRwManager.h"
#include "XmlLogData.h"
#include "XmlSender.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CXmlSender�R���X�g���N�^
	* @param  inXmlData : [i]�ʒm����XML�f�[�^
**/
CXmlSender::CXmlSender(const std::string& inXmlData, const char* inDeviceName)
	: m_SendXmlData(inXmlData),
	  m_DeviceName(inDeviceName)
{
}

/**
	* @brief  CXmlSender�f�X�g���N�^
**/
CXmlSender::~CXmlSender()
{
}

/**
	* @brief  XML�f�[�^�ʒm
	* @retval �������ʃf�[�^
	*		�@����̏ꍇ�F���X�|���XXML�f�[�^
	*		  �ُ�̏ꍇ�F[ERROR] �G���[���e
**/
 BOOL CXmlSender::Doit(std::string& result)
{
	// XML�f�[�^��ʒm����
	BOOL nRet = Send();
	if ( !nRet )
	{	// �ُ�̏ꍇ�ُ͈���e�����O�ɏo�͂���
		WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
	}
	else
	{	// ����̏ꍇ
	}

	result = m_ResultData;
	return nRet;
}

/**
	* @brief  XML�f�[�^�ʒm
**/
BOOL CXmlSender::Send()
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	BOOL nRet = FALSE;

	HINTERNET hInet = NULL;			// �C���^�[�l�b�g�n���h��
	HINTERNET hHttp = NULL;			// �ڑ����ʃn���h��
	HINTERNET hReq	= NULL;			// HTTP�v�����ʃn���h��

	// ���M���擾
	std::string hostName = GetUwIpAddress();
	std::string ObjName = GetObjName();
	INTERNET_PORT nServerPort = GetSendPort();

	// �C���^�[�l�b�g�̃n���h���̍쐬
	hInet = InternetOpen("HttpClient", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hInet)
	{	// �G���[
		SetErrorResultData("[CXmlSender::Doit] InternetOpen ERROR");
		goto LABEL_ERROR;
	}

	// �C���^�[�l�b�g��̃T�[�o�ւ̐ڑ�
	hHttp = InternetConnect(hInet, hostName.c_str(), nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(!hHttp)
	{	// �G���[
		SetErrorResultData("[CXmlSender::Doit] InternetConnect ERROR");
		goto LABEL_ERROR;
	}

	// HTTP���N�G�X�g�̍쐬
	hReq = HttpOpenRequest(hHttp, "POST", ObjName.c_str(), NULL, "", NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if(!hReq)
	{
		SetErrorResultData("[CXmlSender::Doit] HttpOpenRequest ERROR");
		goto LABEL_ERROR;
	}

	// HTTP���N�G�X�g�𑗐M
	BOOL bRet = HttpSendRequest(hReq, NULL, 0, (LPVOID)m_SendXmlData.c_str(), m_SendXmlData.size()); 
	if (bRet)
	{
		// ���O
		WriteToLogBuf(LOG_DEBUG, "[CXmlSender::Doit] HttpSendRequest OK" );
	
		TCHAR chReadBuf[1024] = {0};
		DWORD dwSize = 1024;
		// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_STATUS_CODE=�T�[�o����Ԃ��ꂽ��ԃR�[�h�j
		HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE, chReadBuf, &dwSize, 0);
		// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_STATUS_TEXT=�T�[�o����Ԃ��ꂽ�⑫�̃e�L�X�g�j
		HttpQueryInfo(hReq, HTTP_QUERY_STATUS_TEXT, chReadBuf, &dwSize, 0);

		TCHAR BufSizeText[1000];
		DWORD BufSizeTextSize = 1000;
		// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_CONTENT_LENGTH=���\�[�X�̃T�C�Y�j
		HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, BufSizeText, &BufSizeTextSize, NULL);

		// HTTP���X�|���X�o�b�t�@�̊m��
		DWORD BufSize = atol(BufSizeText) + 1;
		TCHAR* pBuf = new TCHAR[BufSize];
		memset(pBuf, 0x00, BufSize);

		// HTTP���X�|���X�ǂݍ���
		DWORD ReadSize = BufSize;
		nRet = InternetReadFile(hReq, pBuf, BufSize-1, &ReadSize);
		if ( nRet )
		{
			m_ResultData = pBuf;
			WriteToLogBuf(LOG_DEBUG, "[CXmlSender::Doit] InternetReadFile OK" );

			// ���X�|���X�����O�o�͂���
			std::string strSavePath = CXmlLogData::GetResponseLogFilePath(m_DeviceName);
			CXmlLogData::WriteFile(strSavePath, m_ResultData);

			// UTF8 �� Shift-JIS
			std::string ShiftJIS = ConvUTF8toShiftJIS(m_ResultData);
			m_ResultData = ShiftJIS;
		}
		else
		{
			SetErrorResultData("[CXmlSender::Doit] InternetReadFile ERROR");
		}

		// HTTP���X�|���X�o�b�t�@�̊J��
		delete [] pBuf;
	}
	else
	{	// �ʒm�G���[
		SetErrorResultData("[CXmlSender::Doit] HttpSendRequest ERROR");
	}

LABEL_ERROR:
	if (hReq)
	{
		::InternetCloseHandle(hReq);
	}

	if (hHttp)
	{
		::InternetCloseHandle(hHttp);
	}
	
	if (hInet)
	{
		::InternetCloseHandle(hInet);
	}

	return nRet;

}

/**
	* @brief  �R�[�h�ϊ�(UTF8 �� Shift-JIS)
	* @param  InUTF8 : [i]UTF8�̕�����
	* @retval Shift-JIS�̕�����
**/
std::string CXmlSender::ConvUTF8toShiftJIS( const std::string& InUTF8)
{
	//UTF-8����UTF-16�֕ϊ�
	const int nSize = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.c_str(), -1, NULL, 0 );
	BYTE* pUtf16 = new BYTE[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.c_str(), -1, (LPWSTR)pUtf16, nSize );

	//UTF-16����Shift-JIS�֕ϊ�
	const int nSizeSJis = ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pUtf16, -1, NULL, 0, NULL, NULL );
	BYTE* pSJis = new BYTE[ nSizeSJis * 2 ];
	ZeroMemory( pSJis, nSizeSJis * 2 );
	::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pUtf16, -1, (LPSTR)pSJis, nSizeSJis, NULL, NULL );

	std::string ShiftJIS((char*)pSJis);
	delete [] pUtf16;
	delete [] pSJis;

	return ShiftJIS;
}


/**
	* @brief  ���ʃf�[�^�ɃG���[����ݒ肷��
	* @param  inErrorMessage : [i]�G���[���
**/
void CXmlSender::SetErrorResultData(const std::string& inErrorMessage)
{
	m_ResultData = "[ERROR] " + inErrorMessage;
}

/**
	* @brief  UW��IP�A�h���X���擾����
	* @retval UW��IP�A�h���X
**/
std::string CXmlSender::GetUwIpAddress()
{
	CIni_UwRwManager iniUwRwManager;
	iniUwRwManager.Initialize(TRUE);
	return iniUwRwManager.getUwRw_IpAddress(m_DeviceName);
}

/**
	* @brief  UW�̎�M�t�H���_�����擾����
	* @retval UW�̎�M�t�H���_��
**/
std::string CXmlSender::GetObjName()
{
	return "";
}

/**
	* @brief  UW�̎�M�|�[�g�ԍ����擾����
	* @retval UW�̎�M�|�[�g�ԍ�
**/
UINT CXmlSender::GetSendPort()
{
	CIni_UwRwManager iniUwRwManager;
	iniUwRwManager.Initialize(TRUE);
	return atoi(iniUwRwManager.getSender_Port(m_DeviceName).c_str());
}

};	// namespace uwrw_manager