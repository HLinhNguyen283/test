/**
	* @file		Server.cpp
	* @brief	UW����̗v����M �����t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Common.h"
#include "GetTime.h"
#include "WriteDebugLog.h"
#include "Ini_UnwinderManager.h"
#include "Ini_PrinterDescriptor.h"
#include "Server.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief  CServer �R���g���N�V����
	* @param  InSendThreadID  : [i]���M�X���b�h�̂h�c
	* @param  InWriteDebugLog : [i]���O�N���X�̃|�C���^
**/
CServer::CServer(UINT InSendThreadID, CWriteDebugLog* InWriteDebugLog)
	: m_nSendThreadID(InSendThreadID), m_pWriteLog(InWriteDebugLog)
{
}

/**
	* @brief  CServer �f�X�g���N�V����
**/
CServer::~CServer()
{
}

/**
	* @brief  �T�[�o�[(UW�����JMF����M���鏈��)
**/
void CServer::HttpServerMain()
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾
	m_pWriteLog->WriteDebuglog("Start CServer::HttpServerMain", CWriteDebugLog::E_LOG_LEVEL_1);

	CString StatusURL = GetRecvStatusURL();
	{
		std::stringstream ss;
		ss << "Info. CServer::HttpServerMain StatusURL=" << StatusURL;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
	}
	CString PaperURL = GetRecvPaperURL();
	{
		std::stringstream ss;
		ss << "Info. CServer::HttpServerMain PaperURL=" << PaperURL;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
	}

	std::wstring strwStatusURL = A2W(StatusURL.GetString());
	std::wstring strwPaperURL = A2W(PaperURL.GetString());

	// HTTP API DLL �̏�����
	HTTPAPI_VERSION httpApiVersion = HTTPAPI_VERSION_2;
	ULONG ulRet = HttpInitialize(httpApiVersion, HTTP_INITIALIZE_SERVER, NULL);
	if (ulRet != NO_ERROR)
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpInitialize ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// ���N�G�X�g�L���[�̍쐬.
	HANDLE hReqQueue = NULL;
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR)
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpCreateRequestQueue 1st ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	//���N�G�X�g�L���[�̔j���B
	ulRet = HttpRemoveUrl(hReqQueue, strwStatusURL.c_str());
	ulRet = HttpRemoveUrl(hReqQueue, strwPaperURL.c_str());

	if (hReqQueue)
	{
		CloseHandle(hReqQueue);
	}

	// ���N�G�X�g�L���[�̍쐬.
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR)
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpCreateRequestQueue 2nd ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// �T�[�o�[�Z�b�V�����̍쐬.(HTTP Version2)
	HTTP_SERVER_SESSION_ID httpServerSessionId = HTTP_NULL_ID;;
	ulRet = HttpCreateServerSession (httpApiVersion, &httpServerSessionId, 0 );
	if (ulRet != NO_ERROR)
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpCreateServerSession ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URL�O���[�v�̍쐬.(HTTP Version2)
	HTTP_URL_GROUP_ID httpUrlGroupId = HTTP_NULL_ID;
	ulRet = HttpCreateUrlGroup(httpServerSessionId, &httpUrlGroupId, 0 );
	if ( NO_ERROR != ulRet )
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpCreateUrlGroup ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URL�O���[�v��StatusURL��ǉ�.
	ulRet = HttpAddUrlToUrlGroup (httpUrlGroupId, strwStatusURL.c_str(), 0, 0);
	if (ulRet != NO_ERROR)
	{	// [����]�Ǘ��Ҍ������Ȃ���ԂŋN�������ulRet=5�i�A�N�Z�X���ہj�̃G���[�ƂȂ�܂�
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpAddUrlToUrlGroup(StatusURL) ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	if ( strwStatusURL != strwPaperURL )
	{
		// URL�O���[�v��PaperURL��ǉ�.
		ulRet = HttpAddUrlToUrlGroup (httpUrlGroupId, strwPaperURL.c_str(), 0, 0);
		if (ulRet != NO_ERROR){
			std::stringstream ss;
			ss << "ERROR CServer::HttpServerMain HttpAddUrlToUrlGroup(PaperURL) ulRet=" << ulRet;
			m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
			return;
		}
	}

	// URL�O���[�v�����N�G�X�g�L���[�Ɋ֘A�t����.(HTTP Version2)
	HTTP_BINDING_INFO httpBindingInfo;
	HTTP_PROPERTY_FLAGS httpPropFlags;
	httpPropFlags.Present = 1;
	httpBindingInfo.Flags = httpPropFlags;
	httpBindingInfo.RequestQueueHandle = hReqQueue;
	ulRet = HttpSetUrlGroupProperty (httpUrlGroupId, HttpServerBindingProperty, &httpBindingInfo, sizeof( HTTP_BINDING_INFO ));
	if ( NO_ERROR != ulRet )
	{
		std::stringstream ss;
		ss << "ERROR CServer::HttpServerMain HttpSetUrlGroupProperty ulRet=" << ulRet;
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	BOOL Loop = TRUE;
	while (Loop)
	{
		HTTP_REQUEST_ID HttpRequestID = HTTP_NULL_ID;
		BYTE pHttpHeaderBuffer [2048] = {0};
		PHTTP_REQUEST pHttpRequest = (PHTTP_REQUEST)pHttpHeaderBuffer;
		ULONG cbHttpHeaderBuffer = 2048, cbBytesReceived = 0;
		//�w�b�_�擾�B
		ulRet = HttpReceiveHttpRequest(hReqQueue, HttpRequestID, 0, pHttpRequest, cbHttpHeaderBuffer, &cbBytesReceived, NULL); 
		if (ulRet == NO_ERROR)
		{
			m_pWriteLog->WriteDebuglog("Info. CServer::HttpServerMain HttpReceiveHttpRequest Success", CWriteDebugLog::E_LOG_LEVEL_2);
		
			PCSTR reqPath = pHttpRequest->pRawUrl;
			switch (pHttpRequest->Verb) {
			case HttpVerbGET:
				// ��GET�ɑ΂��鏈���͂Ȃ�
				// SendHttpGetResponse(hReqQueue, pHttpRequest);
				break;
			case HttpVerbPOST:
				Loop = HttpPostReceive(hReqQueue, pHttpRequest);
				break;
			default:
				break;
			}
		}
		else {
			std::stringstream ss;
			ss << "ERROR CServer::HttpServerMain HttpReceiveHttpRequest ulRet=" << ulRet;
			m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
			Loop = FALSE;
		}
	}

//exit:

	ulRet = HttpRemoveUrl(hReqQueue, strwStatusURL.c_str());
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
	m_pWriteLog->WriteDebuglog("End.. CServer::HttpServerMain", CWriteDebugLog::E_LOG_LEVEL_1);
}

/**
	* @brief  POST��M����
	* @param  hReqQueue		: [i]HttpCreateRequestQueue�̃|�C���^
	* @param  pHttpRequest	: [i]PHTTP_REQUEST�̃|�C���^
	* @retval 0(�Œ�)
**/
DWORD CServer::HttpPostReceive(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest)
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	m_pWriteLog->WriteDebuglog("Start CServer::HttpPostReceive", CWriteDebugLog::E_LOG_LEVEL_1);

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
		m_pWriteLog->WriteDebuglog("ERROR CServer::HttpPostReceive pHttpBodyBuffer is NULL", CWriteDebugLog::E_LOG_LEVEL_1);
		return FALSE;
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
		m_pWriteLog->WriteDebuglog("Info. CServer::HttpPostReceive Receive Data", CWriteDebugLog::E_LOG_LEVEL_1);

		{	// ��M�R�}���h�����O�o�͂���
			CString FileName(_T("Receive_"));
			FileName.Append(CGetTime::GetStrTimeMM() + _T(".xml"));

			CWriteDebugLog sReceiveCommandLog;
			sReceiveCommandLog.InitializeXml(FileName);
			sReceiveCommandLog.WriteDebuglog(CString(pHttpBodyBuffer), CWriteDebugLog::E_LOG_LEVEL_1, FALSE);
		}

		{	// �N���C�A���g�ɕԎ�
			DWORD bytesSent = 0;
			// �ǉ��w�b�_�[
			std::string strContentType = "Content-type: application/vnd.cip4-jmf+xml"; //= "application/vnd.cip4-jmf+xml"
			// ���X�|���X�f�[�^���Ȃ��ꍇ
			response.Headers.KnownHeaders [HttpHeaderContentType].pRawValue = strContentType.c_str();
			response.Headers.KnownHeaders [HttpHeaderContentType].RawValueLength = strContentType.length();
			response.EntityChunkCount		= 0;
			response.pEntityChunks			= NULL;
			// ���X�|���X���M
			HttpSendHttpResponse(hReqQueue, pHttpRequest->RequestId, HTTP_SEND_RESPONSE_FLAG_DISCONNECT, &response, NULL, &bytesSent, NULL, 0, NULL, NULL);
		}
		m_pWriteLog->WriteDebuglog("Info. CServer::HttpPostReceive Send Response Data", CWriteDebugLog::E_LOG_LEVEL_1);

		{	// ��M�f�[�^��ʒm

			// UTF8 �� Shift-JIS
			std::string ShiftJIS = ConvUTF8toShiftJIS(pHttpBodyBuffer);

			CString *p = new CString();
			p->Append(ShiftJIS.c_str());
			PostThreadMessage( m_nSendThreadID, CCommon::WM_USER_SIGNAL_STATUS, (WPARAM)p, (LPARAM)0 );
		}
	}

	if ( pHttpBodyBuffer )
	{
		delete [] pHttpBodyBuffer;
		pHttpBodyBuffer = NULL;
	}

	m_pWriteLog->WriteDebuglog("End.. CServer::HttpPostReceive", CWriteDebugLog::E_LOG_LEVEL_1);

	return TRUE;
}

/**
	* @brief  �R�[�h�ϊ�(UTF8 �� Shift-JIS)
	* @param  InUTF8 : [i]UTF8�̕�����
	* @retval Shift-JIS�̕�����
**/
std::string CServer::ConvUTF8toShiftJIS(const std::string& InUTF8)
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
	* @brief  SignalStatus(��ԊĎ�)��URL�擾
	* @retval SignalStatus(��ԊĎ�)��URL
**/
CString CServer::GetRecvStatusURL()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	std::string IpAddress  = iniPrinterDescriptor.getMyIpAddress();

	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	std::string ReceiverPort = iniUnwinderManager.getReceiver_Port();
	std::string FolderName = iniUnwinderManager.getStatus_Folder();

	std::string URL = "http://" + IpAddress + ":" + ReceiverPort;
	if ( !FolderName.empty() )
	{
		URL.append("/");
		URL.append(FolderName);
	}

	return URL.c_str();
}

/**
	* @brief  SignalStatus(�p������)��URL�擾
	* @retval SignalStatus(�p������)��URL
**/
CString CServer::GetRecvPaperURL()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	std::string IpAddress  = iniPrinterDescriptor.getMyIpAddress();

	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	std::string ReceiverPort = iniUnwinderManager.getReceiver_Port();
	std::string FolderName = iniUnwinderManager.getResource_Folder();

	std::string URL = "http://" + IpAddress + ":" + ReceiverPort;
	if ( !FolderName.empty() )
	{
		URL.append("/");
		URL.append(FolderName);
	}

	return URL.c_str();
}
