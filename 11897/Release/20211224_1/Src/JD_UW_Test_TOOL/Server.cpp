/**
 * @file  Server.cpp
 * @brief UW����̗v����M �����t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "GetTime.h"
#include "GetIniFileParam.h"
#include "WriteDebugLog.h"
#include "JmfData.h"
#include "JmfAnalys.h"
#include "Server.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief  CServer �R���g���N�V����
	* @param  pParent       �@: [i]�e��ʂ̃E�B���h�E�n���h��
	* @param  pWriteDebugLog  : [i]���O�N���X�̃|�C���^
**/
/* ==================================================================================================== */
CServer::CServer(CWnd* pParent, CWriteDebugLog* pWriteDebugLog)
	: m_pParent(pParent), m_pWriteLog(pWriteDebugLog)
{
}

/* ==================================================================================================== */
/**
	* @brief  CServer �f�X�g���N�V����
**/
/* ==================================================================================================== */
CServer::~CServer()
{
}

/* ==================================================================================================== */
/**
	* @brief  �T�[�o�[(UW�����JMF����M���鏈��)
**/
/* ==================================================================================================== */
void CServer::HttpServerMain()
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾
	m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	//HTTP API �̏������B
	wchar_t pwcURL[_MAX_PATH] = {0};

	std::wstring wstrHostName;
	wstrHostName = A2W(CGetIniFileParam::GetRecvIpAddress().GetString());
	INTERNET_PORT nServerPort = CGetIniFileParam::GetRecvPortNumber();
	_snwprintf_s(pwcURL, sizeof(pwcURL) -1, L"http://%s:%d/", wstrHostName.c_str(), nServerPort);

	// HTTP API DLL �̏�����
	HTTPAPI_VERSION httpApiVersion = HTTPAPI_VERSION_2;
	ULONG ulRet = HttpInitialize(httpApiVersion, HTTP_INITIALIZE_SERVER, NULL);
	if (ulRet != NO_ERROR)
	{
		CString Msg;
		Msg.Format(_T("HttpInitialize Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// ���N�G�X�g�L���[�̍쐬.
	HANDLE hReqQueue = NULL;
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateRequestQueue 1st Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	//���N�G�X�g�L���[�̔j���B
	HttpRemoveUrl(hReqQueue, pwcURL);
	if (hReqQueue) {
		CloseHandle(hReqQueue);
	}

	// ���N�G�X�g�L���[�̍쐬.
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateRequestQueue 2nd Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// �T�[�o�[�Z�b�V�����̍쐬.(HTTP Version2)
	HTTP_SERVER_SESSION_ID httpServerSessionId = HTTP_NULL_ID;;
	ulRet = HttpCreateServerSession (httpApiVersion, &httpServerSessionId, 0 );
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateServerSession Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URL�O���[�v�̍쐬.(HTTP Version2)
	HTTP_URL_GROUP_ID httpUrlGroupId = HTTP_NULL_ID;
	ulRet = HttpCreateUrlGroup(httpServerSessionId, &httpUrlGroupId, 0 );
	if ( NO_ERROR != ulRet ) {
		CString Msg;
		Msg.Format(_T("HttpCreateUrlGroup Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URL�O���[�v��URL��ǉ�.
	ulRet = HttpAddUrlToUrlGroup (httpUrlGroupId, pwcURL, 0, 0);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpAddUrlToUrlGroup Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}


	// URL�O���[�v�����N�G�X�g�L���[�Ɋ֘A�t����.(HTTP Version2)
	HTTP_BINDING_INFO httpBindingInfo;
	HTTP_PROPERTY_FLAGS httpPropFlags;
	httpPropFlags.Present = 1;
	httpBindingInfo.Flags = httpPropFlags;
	httpBindingInfo.RequestQueueHandle = hReqQueue;
	ulRet = HttpSetUrlGroupProperty (httpUrlGroupId, HttpServerBindingProperty, &httpBindingInfo, sizeof( HTTP_BINDING_INFO ));
	if ( NO_ERROR != ulRet ) {
		CString Msg;
		Msg.Format(_T("HttpSetUrlGroupProperty Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	while (1)
	{
		HTTP_REQUEST_ID HttpRequestID = HTTP_NULL_ID;
		BYTE pHttpHeaderBuffer [2048] = {0};
		PHTTP_REQUEST pHttpRequest = (PHTTP_REQUEST)pHttpHeaderBuffer;
		ULONG cbHttpHeaderBuffer = 2048, cbBytesReceived = 0;
		//�w�b�_�擾�B
		ulRet = HttpReceiveHttpRequest(hReqQueue, HttpRequestID, 0, pHttpRequest, cbHttpHeaderBuffer, &cbBytesReceived, NULL); 
		if (ulRet == NO_ERROR)
		{
			m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] �w�b�_�̎擾�ɐ���"), CWriteDebugLog::E_LOG_LEVEL_2);
		
			PCSTR reqPath = pHttpRequest->pRawUrl;
			switch (pHttpRequest->Verb) {
			case HttpVerbGET:
				m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] HttpVerbGET : CServer::HttpServerMain() -> SendHttpGetResponse()"), CWriteDebugLog::E_LOG_LEVEL_2);
			
				// ��GET�ɑ΂��鏈���͂Ȃ�
				// SendHttpGetResponse(hReqQueue, pHttpRequest);

				m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] HttpVerbGET : CServer::HttpServerMain() <- SendHttpGetResponse()"), CWriteDebugLog::E_LOG_LEVEL_2);
				break;
			case HttpVerbPOST:
				m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] HttpVerbPOST : CServer::HttpServerMain() -> SendHttpPostResponse()"), CWriteDebugLog::E_LOG_LEVEL_2);
			
				SendHttpPostResponse(hReqQueue, pHttpRequest);

				m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] HttpVerbPOST : CServer::HttpServerMain() <- SendHttpPostResponse()"), CWriteDebugLog::E_LOG_LEVEL_2);
				break;
			default:
				break;
			}
		}
		else {
			CString Msg;
			Msg.Format(_T("HttpReceiveHttpRequest Failed (%u)"), ulRet );
			m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		}
	}

//exit:

	ulRet = HttpRemoveUrl(hReqQueue, pwcURL);
	if ( NO_ERROR != ulRet ) {
	}

	// �T�[�o�[�Z�b�V�����̔j��.(HTTP Version2)
	if ( !HTTP_IS_NULL_ID ( &httpServerSessionId ) ) {
		ulRet = HttpCloseServerSession( httpServerSessionId );
		if ( NO_ERROR != ulRet ) {
		}
	}

	//���N�G�X�g�L���[�̔j���B
	if (hReqQueue) {
		// CloseHandle(hReqQueue);
		ulRet = HttpCloseRequestQueue(hReqQueue);
		if ( NO_ERROR != ulRet ) {
		}
	}

	//HTTP API�̏I�������B
	HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);

	// ���O
	m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
}

/* ==================================================================================================== */
/**
	* @brief  POST�ł������ꍇ�̃��X�|���X�𑗐M����
	* @param  hReqQueue		: [i]CServerThread�̃|�C���^
	* @param  pHttpRequest	: [i]CServerThread�̃|�C���^
	* @retval 0(�Œ�)
**/
/* ==================================================================================================== */
DWORD CServer::SendHttpPostResponse(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest)
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	m_pWriteLog->WriteDebuglog(_T("[CServer::SendHttpPostResponse()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	DWORD result = 0;

	// HTTP_RESPONSE�\���̍쐬
	HTTP_RESPONSE response;
	::ZeroMemory ( &response, sizeof (HTTP_RESPONSE) );
	response.StatusCode = 200;
	response.pReason = "OK";
	response.ReasonLength = (USHORT)strlen("OK");
	HTTP_DATA_CHUNK					HttpDataChunk;
	::ZeroMemory ( &HttpDataChunk, sizeof (HTTP_DATA_CHUNK) );

	ULONG nHttpBodyLength = 16;
	if(pHttpRequest->Headers.KnownHeaders[HttpHeaderContentLength].pRawValue)
	{
		nHttpBodyLength = atol(pHttpRequest->Headers.KnownHeaders[HttpHeaderContentLength].pRawValue);
	}
	
	char* pHttpBodyBuffer = new char[nHttpBodyLength + 1];
	if(!pHttpBodyBuffer)
	{
		m_pWriteLog->WriteDebuglog(_T("Error�F[CServer::SendHttpPostResponse()] SendHttpPostResponse pHttpBodyBuffer is NULL"), CWriteDebugLog::E_LOG_LEVEL_1);
		return result;
	}
	::ZeroMemory(pHttpBodyBuffer, nHttpBodyLength + 1);

	//�{�f�B�������݂��邩�`�F�b�N�B
	if (pHttpRequest->Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS)
	{
		//�{�f�B����S�ēǍ��ށB
		BOOL bLoop = TRUE;
		while (bLoop)
		{
			ULONG cbBytesReceived = 0;
			//�{�f�B�擾�B
			ULONG ulRet 
				= HttpReceiveRequestEntityBody(hReqQueue, pHttpRequest->RequestId, 0, pHttpBodyBuffer, nHttpBodyLength, &cbBytesReceived, NULL); // cbHttpBodyBuffer <- �f�[�^�i�[�ϐ�
			switch (ulRet) {
			case NO_ERROR: // ����ɓǂݍ��� (�����������ƃf�[�^�����邩������Ȃ�)
				break;
			case ERROR_HANDLE_EOF: // �S�Ẵf�[�^��ǂݍ��� 
				bLoop = FALSE;
				break;
			default:
				bLoop = false;
			}
		}
		
		{	// ��M�R�}���h�����O�o�͂���
			CString FileName(_T("ReceiveCommand_"));
			FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

			CWriteDebugLog sReceiveCommandLog;
			sReceiveCommandLog.Initialize(FileName);
			sReceiveCommandLog.WriteDebuglog(CString(pHttpBodyBuffer), CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

			// ��ʏo��
			CString *p = new CString();
			*p = pHttpBodyBuffer;
			PostMessage(m_pParent->m_hWnd, CCommon::WM_USER_DSP_RECV, (WPARAM)p, 0);
		}

		// Response�̍쐬
		CString strResJMF = MakeResponseJMF(pHttpBodyBuffer);

		// �ǉ��w�b�_�[
		std::string strContentType = CONTENT_TYPE_JMF; //= "application/vnd.cip4-jmf+xml"

		//�N���C�A���g�ɕԎ��B
		DWORD bytesSent = 0;

		if ( strResJMF.IsEmpty() )
		{	// ���X�|���X�f�[�^���Ȃ��ꍇ
			response.Headers.KnownHeaders [HttpHeaderContentType].pRawValue = strContentType.c_str();
			response.Headers.KnownHeaders [HttpHeaderContentType].RawValueLength = strContentType.length();
			response.EntityChunkCount		= 0;
			response.pEntityChunks			= NULL;
		}
		else
		{	// ���X�|���X�f�[�^������ꍇ
			// Shift-JIS �� UTF8
			CString s = CCommon::ConvShiftJIStoUTF8(strResJMF.GetBuffer());
			strResJMF = s;

			HttpDataChunk.DataChunkType  = HttpDataChunkFromMemory;
			HttpDataChunk.FromMemory.pBuffer = strResJMF.GetBuffer();			// JMF�f�[�^ <JMF..>�`</JMF>
			HttpDataChunk.FromMemory.BufferLength = strResJMF.GetLength();		// JMF�f�[�^�T�C�Y
			response.Headers.KnownHeaders [HttpHeaderContentType].pRawValue = strContentType.c_str();
			response.Headers.KnownHeaders [HttpHeaderContentType].RawValueLength = strContentType.length();
			response.EntityChunkCount		= 1;
			response.pEntityChunks			= &HttpDataChunk;
		}
		// ���X�|���X���M
		HttpSendHttpResponse(hReqQueue, pHttpRequest->RequestId, HTTP_SEND_RESPONSE_FLAG_DISCONNECT, &response, NULL, &bytesSent, NULL, 0, NULL, NULL);


		if ( !strResJMF.IsEmpty() )
		{	// ���M���X�|���X�����O�o�͂���
			CString FileName(_T("SendResponse_"));
			FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

			CWriteDebugLog sSendResponseLog;
			sSendResponseLog.Initialize(FileName);
			sSendResponseLog.WriteDebuglog(strResJMF, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

			// ��ʏo��
			CString *p = new CString();
			*p = strResJMF;
			PostMessage(m_pParent->m_hWnd, CCommon::WM_USER_DSP_SEND, (WPARAM)p, 0);
		}
	}

	if ( pHttpBodyBuffer )
	{
		delete [] pHttpBodyBuffer;
		pHttpBodyBuffer = NULL;
	}

	m_pWriteLog->WriteDebuglog(_T("[CServer::SendHttpPostResponse()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return result;
}

/* ==================================================================================================== */
/**
	* @brief  ���X�|���X��JMF���쐬����
	* @param  INpHttpBodyBuffer	: [i]��MJMF
	* @retval ���X�|���X��JMF�f�[�^
**/
/* ==================================================================================================== */
CString CServer::MakeResponseJMF(char* INpHttpBodyBuffer)
{	
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾
	m_pWriteLog->WriteDebuglog(_T("[CServer::MakeResponseJMF()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	CString ResponseJmf;
	// ��MJMF����͂���
	CJmfAnalys sJmfAnalys(INpHttpBodyBuffer);
	if ( sJmfAnalys.Perth() == ERROR_SUCCESS )
	{	// �p�[�X����
		int nJmfType = sJmfAnalys.GetRecvJmfType();		// ��MJMF�^�C�v���擾
		switch(nJmfType) {
		case CCommon::E_JMF_QueryStatus:
			m_pWriteLog->WriteDebuglog(_T("��ԊĎ�����M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_QueryResource:
			m_pWriteLog->WriteDebuglog(_T("�p�����o�^����M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_StopPersChParams:
			m_pWriteLog->WriteDebuglog(_T("�`���l���폜����M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_CommandResource:
			m_pWriteLog->WriteDebuglog(_T("��������ݒ����M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_CommandWakeup:
			m_pWriteLog->WriteDebuglog(_T("READY��Ԃւ̑J�ڗv������M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_SignalStatus:
			m_pWriteLog->WriteDebuglog(_T(" UW�����SignalStatus����M"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		}
		ResponseJmf = GetJmfResponseData(nJmfType);		// ���X�|���X�f�[�^���擾(���X�|���X�f�[�^���Ȃ��ꍇ�͋�)
	}
	else
	{	// ��MJMF�`���ُ�
		m_pWriteLog->WriteDebuglog(_T("Error : [CServer::MakeResponseJMF()] ��MJMF�̉�͂Ɏ��s���܂����B"), CWriteDebugLog::E_LOG_LEVEL_1);
		
		// ���b�Z�[�W�{�b�N�X
		MessageBox(m_pParent->m_hWnd, _T("��MJMF�̉�͂Ɏ��s���܂���"), _T("Error"), MB_OK);
	}

	m_pWriteLog->WriteDebuglog(_T("[CServer::MakeResponseJMF()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return ResponseJmf;
}
