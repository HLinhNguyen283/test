/**
 * @file  Server.cpp
 * @brief UWからの要求受信 実装ファイル
 * @author screen
 * @date  20210720 screen 新規作成
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
	* @brief  CServer コントラクション
	* @param  pParent       　: [i]親画面のウィンドウハンドル
	* @param  pWriteDebugLog  : [i]ログクラスのポインタ
**/
/* ==================================================================================================== */
CServer::CServer(CWnd* pParent, CWriteDebugLog* pWriteDebugLog)
	: m_pParent(pParent), m_pWriteLog(pWriteDebugLog)
{
}

/* ==================================================================================================== */
/**
	* @brief  CServer デストラクション
**/
/* ==================================================================================================== */
CServer::~CServer()
{
}

/* ==================================================================================================== */
/**
	* @brief  サーバー(UWからのJMFを受信する処理)
**/
/* ==================================================================================================== */
void CServer::HttpServerMain()
{
	USES_CONVERSION;				// A2W使用の場合の宣言
	m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	//HTTP API の初期化。
	wchar_t pwcURL[_MAX_PATH] = {0};

	std::wstring wstrHostName;
	wstrHostName = A2W(CGetIniFileParam::GetRecvIpAddress().GetString());
	INTERNET_PORT nServerPort = CGetIniFileParam::GetRecvPortNumber();
	_snwprintf_s(pwcURL, sizeof(pwcURL) -1, L"http://%s:%d/", wstrHostName.c_str(), nServerPort);

	// HTTP API DLL の初期化
	HTTPAPI_VERSION httpApiVersion = HTTPAPI_VERSION_2;
	ULONG ulRet = HttpInitialize(httpApiVersion, HTTP_INITIALIZE_SERVER, NULL);
	if (ulRet != NO_ERROR)
	{
		CString Msg;
		Msg.Format(_T("HttpInitialize Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// リクエストキューの作成.
	HANDLE hReqQueue = NULL;
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateRequestQueue 1st Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	//リクエストキューの破棄。
	HttpRemoveUrl(hReqQueue, pwcURL);
	if (hReqQueue) {
		CloseHandle(hReqQueue);
	}

	// リクエストキューの作成.
	ulRet = HttpCreateRequestQueue (httpApiVersion, NULL, NULL, 0, &hReqQueue);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateRequestQueue 2nd Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// サーバーセッションの作成.(HTTP Version2)
	HTTP_SERVER_SESSION_ID httpServerSessionId = HTTP_NULL_ID;;
	ulRet = HttpCreateServerSession (httpApiVersion, &httpServerSessionId, 0 );
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpCreateServerSession Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URLグループの作成.(HTTP Version2)
	HTTP_URL_GROUP_ID httpUrlGroupId = HTTP_NULL_ID;
	ulRet = HttpCreateUrlGroup(httpServerSessionId, &httpUrlGroupId, 0 );
	if ( NO_ERROR != ulRet ) {
		CString Msg;
		Msg.Format(_T("HttpCreateUrlGroup Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}

	// URLグループにURLを追加.
	ulRet = HttpAddUrlToUrlGroup (httpUrlGroupId, pwcURL, 0, 0);
	if (ulRet != NO_ERROR){
		CString Msg;
		Msg.Format(_T("HttpAddUrlToUrlGroup Failed (%u)"), ulRet );
		m_pWriteLog->WriteDebuglog(Msg, CWriteDebugLog::E_LOG_LEVEL_1);
		return;
	}


	// URLグループをリクエストキューに関連付ける.(HTTP Version2)
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
		//ヘッダ取得。
		ulRet = HttpReceiveHttpRequest(hReqQueue, HttpRequestID, 0, pHttpRequest, cbHttpHeaderBuffer, &cbBytesReceived, NULL); 
		if (ulRet == NO_ERROR)
		{
			m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] ヘッダの取得に成功"), CWriteDebugLog::E_LOG_LEVEL_2);
		
			PCSTR reqPath = pHttpRequest->pRawUrl;
			switch (pHttpRequest->Verb) {
			case HttpVerbGET:
				m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] HttpVerbGET : CServer::HttpServerMain() -> SendHttpGetResponse()"), CWriteDebugLog::E_LOG_LEVEL_2);
			
				// ※GETに対する処理はなし
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

	// サーバーセッションの破棄.(HTTP Version2)
	if ( !HTTP_IS_NULL_ID ( &httpServerSessionId ) ) {
		ulRet = HttpCloseServerSession( httpServerSessionId );
		if ( NO_ERROR != ulRet ) {
		}
	}

	//リクエストキューの破棄。
	if (hReqQueue) {
		// CloseHandle(hReqQueue);
		ulRet = HttpCloseRequestQueue(hReqQueue);
		if ( NO_ERROR != ulRet ) {
		}
	}

	//HTTP APIの終了処理。
	HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);

	// ログ
	m_pWriteLog->WriteDebuglog(_T("[CServer::HttpServerMain()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);
}

/* ==================================================================================================== */
/**
	* @brief  POSTであった場合のレスポンスを送信する
	* @param  hReqQueue		: [i]CServerThreadのポインタ
	* @param  pHttpRequest	: [i]CServerThreadのポインタ
	* @retval 0(固定)
**/
/* ==================================================================================================== */
DWORD CServer::SendHttpPostResponse(HANDLE hReqQueue, PHTTP_REQUEST pHttpRequest)
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	m_pWriteLog->WriteDebuglog(_T("[CServer::SendHttpPostResponse()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	DWORD result = 0;

	// HTTP_RESPONSE構造体作成
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
		m_pWriteLog->WriteDebuglog(_T("Error：[CServer::SendHttpPostResponse()] SendHttpPostResponse pHttpBodyBuffer is NULL"), CWriteDebugLog::E_LOG_LEVEL_1);
		return result;
	}
	::ZeroMemory(pHttpBodyBuffer, nHttpBodyLength + 1);

	//ボディ部が存在するかチェック。
	if (pHttpRequest->Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS)
	{
		//ボディ部を全て読込む。
		BOOL bLoop = TRUE;
		while (bLoop)
		{
			ULONG cbBytesReceived = 0;
			//ボディ取得。
			ULONG ulRet 
				= HttpReceiveRequestEntityBody(hReqQueue, pHttpRequest->RequestId, 0, pHttpBodyBuffer, nHttpBodyLength, &cbBytesReceived, NULL); // cbHttpBodyBuffer <- データ格納変数
			switch (ulRet) {
			case NO_ERROR: // 正常に読み込んだ (しかしもっとデータがあるかもしれない)
				break;
			case ERROR_HANDLE_EOF: // 全てのデータを読み込んだ 
				bLoop = FALSE;
				break;
			default:
				bLoop = false;
			}
		}
		
		{	// 受信コマンドをログ出力する
			CString FileName(_T("ReceiveCommand_"));
			FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

			CWriteDebugLog sReceiveCommandLog;
			sReceiveCommandLog.Initialize(FileName);
			sReceiveCommandLog.WriteDebuglog(CString(pHttpBodyBuffer), CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

			// 画面出力
			CString *p = new CString();
			*p = pHttpBodyBuffer;
			PostMessage(m_pParent->m_hWnd, CCommon::WM_USER_DSP_RECV, (WPARAM)p, 0);
		}

		// Responseの作成
		CString strResJMF = MakeResponseJMF(pHttpBodyBuffer);

		// 追加ヘッダー
		std::string strContentType = CONTENT_TYPE_JMF; //= "application/vnd.cip4-jmf+xml"

		//クライアントに返事。
		DWORD bytesSent = 0;

		if ( strResJMF.IsEmpty() )
		{	// レスポンスデータがない場合
			response.Headers.KnownHeaders [HttpHeaderContentType].pRawValue = strContentType.c_str();
			response.Headers.KnownHeaders [HttpHeaderContentType].RawValueLength = strContentType.length();
			response.EntityChunkCount		= 0;
			response.pEntityChunks			= NULL;
		}
		else
		{	// レスポンスデータがある場合
			// Shift-JIS → UTF8
			CString s = CCommon::ConvShiftJIStoUTF8(strResJMF.GetBuffer());
			strResJMF = s;

			HttpDataChunk.DataChunkType  = HttpDataChunkFromMemory;
			HttpDataChunk.FromMemory.pBuffer = strResJMF.GetBuffer();			// JMFデータ <JMF..>～</JMF>
			HttpDataChunk.FromMemory.BufferLength = strResJMF.GetLength();		// JMFデータサイズ
			response.Headers.KnownHeaders [HttpHeaderContentType].pRawValue = strContentType.c_str();
			response.Headers.KnownHeaders [HttpHeaderContentType].RawValueLength = strContentType.length();
			response.EntityChunkCount		= 1;
			response.pEntityChunks			= &HttpDataChunk;
		}
		// レスポンス送信
		HttpSendHttpResponse(hReqQueue, pHttpRequest->RequestId, HTTP_SEND_RESPONSE_FLAG_DISCONNECT, &response, NULL, &bytesSent, NULL, 0, NULL, NULL);


		if ( !strResJMF.IsEmpty() )
		{	// 送信レスポンスをログ出力する
			CString FileName(_T("SendResponse_"));
			FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

			CWriteDebugLog sSendResponseLog;
			sSendResponseLog.Initialize(FileName);
			sSendResponseLog.WriteDebuglog(strResJMF, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

			// 画面出力
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
	* @brief  レスポンスのJMFを作成する
	* @param  INpHttpBodyBuffer	: [i]受信JMF
	* @retval レスポンスのJMFデータ
**/
/* ==================================================================================================== */
CString CServer::MakeResponseJMF(char* INpHttpBodyBuffer)
{	
	USES_CONVERSION;				// A2W使用の場合の宣言
	m_pWriteLog->WriteDebuglog(_T("[CServer::MakeResponseJMF()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	CString ResponseJmf;
	// 受信JMFを解析する
	CJmfAnalys sJmfAnalys(INpHttpBodyBuffer);
	if ( sJmfAnalys.Perth() == ERROR_SUCCESS )
	{	// パース成功
		int nJmfType = sJmfAnalys.GetRecvJmfType();		// 受信JMFタイプを取得
		switch(nJmfType) {
		case CCommon::E_JMF_QueryStatus:
			m_pWriteLog->WriteDebuglog(_T("状態監視を受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_QueryResource:
			m_pWriteLog->WriteDebuglog(_T("用紙情報登録を受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_StopPersChParams:
			m_pWriteLog->WriteDebuglog(_T("チャネル削除を受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_CommandResource:
			m_pWriteLog->WriteDebuglog(_T("印刷条件設定を受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_CommandWakeup:
			m_pWriteLog->WriteDebuglog(_T("READY状態への遷移要請を受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		case CCommon::E_JMF_SignalStatus:
			m_pWriteLog->WriteDebuglog(_T(" UWからのSignalStatusを受信"), CWriteDebugLog::E_LOG_LEVEL_1);
			break;
		}
		ResponseJmf = GetJmfResponseData(nJmfType);		// レスポンスデータを取得(レスポンスデータがない場合は空)
	}
	else
	{	// 受信JMF形式異常
		m_pWriteLog->WriteDebuglog(_T("Error : [CServer::MakeResponseJMF()] 受信JMFの解析に失敗しました。"), CWriteDebugLog::E_LOG_LEVEL_1);
		
		// メッセージボックス
		MessageBox(m_pParent->m_hWnd, _T("受信JMFの解析に失敗しました"), _T("Error"), MB_OK);
	}

	m_pWriteLog->WriteDebuglog(_T("[CServer::MakeResponseJMF()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return ResponseJmf;
}
