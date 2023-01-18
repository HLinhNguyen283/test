/**
 * @file  Client.cpp
 * @brief UW��JMF�𑗐M �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
#include "WriteDebugLog.h"
#include "JmfData.h"
#include "GetTime.h"
#include "GetIniFileParam.h"
#include "JmfAnalys.h"
#include "Client.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief  CClient �R���X�g���N�V����
**/
/* ==================================================================================================== */
CClient::CClient()
{
}

/* ==================================================================================================== */
/**
	* @brief  CClient �f�X�g���N�V����
**/
/* ==================================================================================================== */
CClient::~CClient()
{
}

/* ==================================================================================================== */
/**
	* @brief JMF�𑗐M����
	* @param  pParent		: [i]�e�N���X�̃|�C���^
	* @param  nJmfType		: [i]���MJMF���
	* @param  pWriteLog		: [i]���O�N���X�|�C���^
**/
/* ==================================================================================================== */
BOOL CClient::HttpClientMain( CWnd* pParent, UINT nJmfType, CWriteDebugLog* pWriteLog )
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	HINTERNET hInet = NULL;
	HINTERNET hHttp = NULL;
	HINTERNET hReq	= NULL;

	// JMF���`�f�[�^�擾
	CString strJMF = GetJmfData(nJmfType);

	// �^�O�ɒl��ݒ�
	strJMF = TagConversion( nJmfType, strJMF );

	// JMF�̃t�H�[�}�b�g�m�F
	CJmfAnalys sJmfAnalys(strJMF);
	if ( sJmfAnalys.Perth() != ERROR_SUCCESS )
	{	// �`���G���[
		MessageBox(pParent->m_hWnd, _T("JMF�`���G���["), _T("Error"), MB_OK);
		return FALSE;
	}

	// ���M���擾
	CString hostName = CGetIniFileParam::GetSendIpAddress();
	CString ObjName = CGetIniFileParam::GetSendFolder();

	switch(nJmfType) {
	case CCommon::E_JMF_SignalStatusPaper:
		ObjName = CGetIniFileParam::GetSendStatusFolder();
		break;
	case CCommon::E_JMF_SignalStatus:
		ObjName = CGetIniFileParam::GetSendPaperFolder();
		break;
	}
	INTERNET_PORT nServerPort = CGetIniFileParam::GetSendPortNumber();

	hInet = InternetOpen(_T("HttpClient"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hInet)
	{
		pWriteLog->WriteDebuglog(_T("Error�F[CClient::HttpClientMain()] InternetOpen()�Ɏ��s���܂���"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	hHttp = InternetConnect(hInet, hostName, nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(!hHttp)
	{
		pWriteLog->WriteDebuglog(_T("Error�F[CClient::HttpClientMain()] InternetConnect()�Ɏ��s���܂���"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	hReq = HttpOpenRequest(hHttp, _T("POST"), ObjName, NULL, _T(""), NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if(!hReq)
	{
		pWriteLog->WriteDebuglog(_T("Error�F[CClient::HttpClientMain()] HttpOpenRequest()�Ɏ��s���܂���"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	{
		// Shift-JIS �� UTF8
		CString s = CCommon::ConvShiftJIStoUTF8(strJMF.GetBuffer());
		strJMF = s;
	}

	{	// ���M�R�}���h�����O�o�͂���
		CString FileName(_T("SendCommand_"));
		FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

		CWriteDebugLog sSendCommandLog;
		sSendCommandLog.Initialize(FileName);
		sSendCommandLog.WriteDebuglog(strJMF, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

		// ��ʏo��
		CString *p = new CString();
		*p = strJMF;
		PostMessage(pParent->m_hWnd, CCommon::WM_USER_DSP_SEND, (WPARAM)p, 0);
	}
	{
		// �ǉ��w�b�_�[
		CString strHeaderContentType(CONTENT_TYPE_JMF);		// "Content-type: application/vnd.cip4-jmf+xml";
//		BOOL bRet = HttpSendRequest(hReq, strHeaderContentType, strHeaderContentType.GetLength(), (LPVOID)strJMF.GetString(), strJMF.GetLength()); 
		BOOL bRet;
		bRet = HttpSendRequest(hReq, NULL, 0, (LPVOID)strJMF.GetString(), strJMF.GetLength()); 
		if (bRet)
		{
			// ���O
			pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] HttpSendRequest�ڑ��ɐ������܂����B"), CWriteDebugLog::E_LOG_LEVEL_2);
	
			TCHAR chReadBuf[1024] = {0};
			DWORD dwSize = 1024;
			// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_STATUS_CODE=�T�[�o����Ԃ��ꂽ��ԃR�[�h�j
			HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE, chReadBuf, &dwSize, 0); // chReadBuf => response.StatusCode
			// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_STATUS_TEXT=�T�[�o����Ԃ��ꂽ�⑫�̃e�L�X�g�j
			HttpQueryInfo(hReq, HTTP_QUERY_STATUS_TEXT, chReadBuf, &dwSize, 0); // chReadBuf => response.pReason

			TCHAR BufSizeText[1000];
			DWORD BufSizeTextSize = 1000;
			// HTTP���N�G�X�g�Ɋ֘A��������擾�iHTTP_QUERY_CONTENT_LENGTH=���\�[�X�̃T�C�Y�j
			HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, BufSizeText, &BufSizeTextSize, NULL); // BufSizeText => response.Headers.KnownHeaders [HttpHeaderContentLength].pRawValue

			DWORD BufSize = atol(BufSizeText) + 1;
			TCHAR* pBuf = new TCHAR[BufSize];
			memset(pBuf, 0x00, BufSize);

			DWORD ReadSize;
			/* �R���e���c�̓��e���擾�E�\�� */
			InternetReadFile(hReq, pBuf, BufSize-1, &ReadSize);

			{	// ���X�|���X�����O�o�͂���
				CString FileName(_T("ReceiveResponse_"));
				FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

				CWriteDebugLog sResponseLog;
				sResponseLog.Initialize(FileName);
				sResponseLog.WriteDebuglog(pBuf, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);
				// ��ʏo��
				CString *p = new CString();
				*p = pBuf;
				PostMessage(pParent->m_hWnd, CCommon::WM_USER_DSP_RECV, (WPARAM)p, 0);
			}

			/* �㏈�� */
			delete [] pBuf;
		}else{
			// ���O
			CString strLog;
			strLog.Format(_T("Error : [CClient::HttpClientMain()] HttpSendRequest�ڑ��Ɏ��s���܂����BGetLastError=%d"), GetLastError());
			pWriteLog->WriteDebuglog(strLog, CWriteDebugLog::E_LOG_LEVEL_1);
		
			// ���b�Z�[�W�{�b�N�X
			CString strMessage;
			strMessage.Format(_T("�ڑ��Ɏ��s���܂����B\n�G���[�ԍ��F%d"), GetLastError());
			MessageBox(pParent->m_hWnd, strMessage, _T("Error"), MB_OK);
		}
	}

LABEL_ERROR:
	if (hReq)
	{
		::InternetCloseHandle(hReq);
		hReq = NULL;

		// ���O
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hReq)�ɐ������܂����B"), CWriteDebugLog::E_LOG_LEVEL_2);
	}

	if (hHttp){
		::InternetCloseHandle(hHttp);
		hHttp = NULL;

		// ���O
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hHttp)�ɐ������܂����B"), CWriteDebugLog::E_LOG_LEVEL_2);
	}
	
	if (hInet){
		::InternetCloseHandle(hInet);
		hInet = NULL;

		// ���O
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hInet)�ɐ������܂����B"), CWriteDebugLog::E_LOG_LEVEL_2);
	}

	// ���O
	pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return TRUE;

}
	
/* ==================================================================================================== */
/**
	* @brief �^�O�̒l��ύX
	* @param  nJmfType		: [i]���MJMF���
	* @param  SourceJmf		: [i]�ύX�O��JMF�f�[�^
	* @retval �^�O��l�ɕύX����JMF�f�[�^
**/
/* ==================================================================================================== */
CString CClient::TagConversion( UINT nJmfType, const CString& SourceJmf )
{
	CString DstJmf(SourceJmf);

	// Header��DeviceID
	if ( DstJmf.Find( CONV_DEVICEID ) != -1 ) DstJmf.Replace(CONV_DEVICEID, CGetIniFileParam::GetDeviceID());

	// Subscription��Repeattime
	if ( DstJmf.Find( CONV_REPEATTIME ) != -1 ) DstJmf.Replace(CONV_REPEATTIME, CGetIniFileParam::GetSubscriptionRepeattime());
	// Subscription��URL
	if ( DstJmf.Find( CONV_URL ) != -1 ) DstJmf.Replace(CONV_URL, CGetIniFileParam::GetSubscriptionURL());
	// Subscription��ChannelMode
	if ( DstJmf.Find( CONV_CHANNELMODE ) != -1 ) DstJmf.Replace(CONV_CHANNELMODE, CGetIniFileParam::GetSubscriptionChannelMode());

	// ResourceQuParams��ResourceDetails
	if ( DstJmf.Find( CONV_RESOURCEDETAILS ) != -1 ) DstJmf.Replace(CONV_RESOURCEDETAILS, CGetIniFileParam::GetResourceQuParamsResourceDetails());
	// ResourceQuParams��ResourceName
	if ( DstJmf.Find( CONV_RESOURCENAME ) != -1 ) DstJmf.Replace(CONV_RESOURCENAME, CGetIniFileParam::GetResourceQuParamsResourceName());
	// ResourceQuParams��Scope
	if ( DstJmf.Find( CONV_SCOPE ) != -1 ) DstJmf.Replace(CONV_SCOPE, CGetIniFileParam::GetResourceQuParamsScope());

	// StopPersChParams��MessageType
	if ( DstJmf.Find( CONV_MESSAGETYPE ) != -1 ) DstJmf.Replace(CONV_MESSAGETYPE, CGetIniFileParam::GetStopPersChParamsMessageType());
	// StopPersChParams��ChannelID:Q1(�f�t�H���g�L�[�F��)
	if ( DstJmf.Find( CONV_CHANNELID ) != -1 ) DstJmf.Replace(CONV_CHANNELID, CGetIniFileParam::GetStopPersChParamsChannelID());
	// StopPersChParams��URL:http://192.168.0.40(�f�t�H���g�L�[�F��)
	if ( DstJmf.Find( CONV_STOP_URL ) != -1 ) DstJmf.Replace(CONV_STOP_URL, CGetIniFileParam::GetStopPersChParamsURL());

	// ResourceCmdParams��UpdateMethod
	if ( DstJmf.Find( CONV_UPDATEMETHOD ) != -1 ) DstJmf.Replace(CONV_UPDATEMETHOD, CGetIniFileParam::GetResourceCmdParamsUpdateMethod());

	// ResouceSet��Name
	if ( DstJmf.Find( CONV_NAME ) != -1 ) DstJmf.Replace(CONV_NAME, CGetIniFileParam::GetResouceSetName());

	// Resource��DescriptiveName
	if ( DstJmf.Find( CONV_DESCRIPTIVENAME ) != -1 ) DstJmf.Replace(CONV_DESCRIPTIVENAME, CGetIniFileParam::GetResourceDescriptiveName());
	// Resource��ExternalID
	if ( DstJmf.Find( CONV_EXTERNALID ) != -1 ) DstJmf.Replace(CONV_EXTERNALID, CGetIniFileParam::GetResourceExternalID());

	// Media��Dimension
	if ( DstJmf.Find( CONV_DIMENSION ) != -1 ) DstJmf.Replace(CONV_DIMENSION, CGetIniFileParam::GetMediaDimension());
	// Media��MediaType
	if ( DstJmf.Find( CONV_MEDIATYPE ) != -1 ) DstJmf.Replace(CONV_MEDIATYPE, CGetIniFileParam::GetMediaMediaType());
	// Media��Thickness
	if ( DstJmf.Find( CONV_THICKNESS ) != -1 ) DstJmf.Replace(CONV_THICKNESS, CGetIniFileParam::GetMediaThickness());
	// Media��scr:UWTension
	if ( DstJmf.Find( CONV_UWTENSION ) != -1 ) DstJmf.Replace(CONV_UWTENSION, CGetIniFileParam::GetMediaUWTension());
	// Media��scr:MaxRunSpeed
	if ( DstJmf.Find( CONV_MAXRUNSPEED ) != -1 ) DstJmf.Replace(CONV_MAXRUNSPEED, CGetIniFileParam::GetMediaMaxRunSpeed());
	// Media��RollDiameter
	if ( DstJmf.Find( CONV_ROLLDIAMETER ) != -1 ) DstJmf.Replace(CONV_ROLLDIAMETER, CGetIniFileParam::GetMediaRollDiameter());

	// Header��ID�ݒ�
	if ( DstJmf.Find( CONV_SENDID ) != -1 ) DstJmf.Replace(CONV_SENDID, CGetIniFileParam::GetJmfID(nJmfType, CGetTime::GetStrDate()));

	// �����ݒ�
	if ( DstJmf.Find( CONV_TIME ) != -1 ) DstJmf.Replace(CONV_TIME, CCommon::CreateTimeStamp());


	if ( DstJmf.Find( CONV_STATUS ) != -1 ) DstJmf.Replace(CONV_STATUS, CGetIniFileParam::GetUwStatus());


	return DstJmf;
}