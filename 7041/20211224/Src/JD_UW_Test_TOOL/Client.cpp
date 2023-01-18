/**
 * @file  Client.cpp
 * @brief UWへJMFを送信 ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
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
	* @brief  CClient コンストラクション
**/
/* ==================================================================================================== */
CClient::CClient()
{
}

/* ==================================================================================================== */
/**
	* @brief  CClient デストラクション
**/
/* ==================================================================================================== */
CClient::~CClient()
{
}

/* ==================================================================================================== */
/**
	* @brief JMFを送信する
	* @param  pParent		: [i]親クラスのポインタ
	* @param  nJmfType		: [i]送信JMF種別
	* @param  pWriteLog		: [i]ログクラスポインタ
**/
/* ==================================================================================================== */
BOOL CClient::HttpClientMain( CWnd* pParent, UINT nJmfType, CWriteDebugLog* pWriteLog )
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] -> IN"), CWriteDebugLog::E_LOG_LEVEL_2);

	HINTERNET hInet = NULL;
	HINTERNET hHttp = NULL;
	HINTERNET hReq	= NULL;

	// JMF雛形データ取得
	CString strJMF = GetJmfData(nJmfType);

	// タグに値を設定
	strJMF = TagConversion( nJmfType, strJMF );

	// JMFのフォーマット確認
	CJmfAnalys sJmfAnalys(strJMF);
	if ( sJmfAnalys.Perth() != ERROR_SUCCESS )
	{	// 形式エラー
		MessageBox(pParent->m_hWnd, _T("JMF形式エラー"), _T("Error"), MB_OK);
		return FALSE;
	}

	// 送信情報取得
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
		pWriteLog->WriteDebuglog(_T("Error：[CClient::HttpClientMain()] InternetOpen()に失敗しました"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	hHttp = InternetConnect(hInet, hostName, nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(!hHttp)
	{
		pWriteLog->WriteDebuglog(_T("Error：[CClient::HttpClientMain()] InternetConnect()に失敗しました"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	hReq = HttpOpenRequest(hHttp, _T("POST"), ObjName, NULL, _T(""), NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if(!hReq)
	{
		pWriteLog->WriteDebuglog(_T("Error：[CClient::HttpClientMain()] HttpOpenRequest()に失敗しました"), CWriteDebugLog::E_LOG_LEVEL_1);
		goto LABEL_ERROR;
	}

	{
		// Shift-JIS → UTF8
		CString s = CCommon::ConvShiftJIStoUTF8(strJMF.GetBuffer());
		strJMF = s;
	}

	{	// 送信コマンドをログ出力する
		CString FileName(_T("SendCommand_"));
		FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

		CWriteDebugLog sSendCommandLog;
		sSendCommandLog.Initialize(FileName);
		sSendCommandLog.WriteDebuglog(strJMF, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);

		// 画面出力
		CString *p = new CString();
		*p = strJMF;
		PostMessage(pParent->m_hWnd, CCommon::WM_USER_DSP_SEND, (WPARAM)p, 0);
	}
	{
		// 追加ヘッダー
		CString strHeaderContentType(CONTENT_TYPE_JMF);		// "Content-type: application/vnd.cip4-jmf+xml";
//		BOOL bRet = HttpSendRequest(hReq, strHeaderContentType, strHeaderContentType.GetLength(), (LPVOID)strJMF.GetString(), strJMF.GetLength()); 
		BOOL bRet;
		bRet = HttpSendRequest(hReq, NULL, 0, (LPVOID)strJMF.GetString(), strJMF.GetLength()); 
		if (bRet)
		{
			// ログ
			pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] HttpSendRequest接続に成功しました。"), CWriteDebugLog::E_LOG_LEVEL_2);
	
			TCHAR chReadBuf[1024] = {0};
			DWORD dwSize = 1024;
			// HTTPリクエストに関連する情報を取得（HTTP_QUERY_STATUS_CODE=サーバから返された状態コード）
			HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE, chReadBuf, &dwSize, 0); // chReadBuf => response.StatusCode
			// HTTPリクエストに関連する情報を取得（HTTP_QUERY_STATUS_TEXT=サーバから返された補足のテキスト）
			HttpQueryInfo(hReq, HTTP_QUERY_STATUS_TEXT, chReadBuf, &dwSize, 0); // chReadBuf => response.pReason

			TCHAR BufSizeText[1000];
			DWORD BufSizeTextSize = 1000;
			// HTTPリクエストに関連する情報を取得（HTTP_QUERY_CONTENT_LENGTH=リソースのサイズ）
			HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, BufSizeText, &BufSizeTextSize, NULL); // BufSizeText => response.Headers.KnownHeaders [HttpHeaderContentLength].pRawValue

			DWORD BufSize = atol(BufSizeText) + 1;
			TCHAR* pBuf = new TCHAR[BufSize];
			memset(pBuf, 0x00, BufSize);

			DWORD ReadSize;
			/* コンテンツの内容を取得・表示 */
			InternetReadFile(hReq, pBuf, BufSize-1, &ReadSize);

			{	// レスポンスをログ出力する
				CString FileName(_T("ReceiveResponse_"));
				FileName.Append(CGetTime::GetStrTime() + _T(".xml"));

				CWriteDebugLog sResponseLog;
				sResponseLog.Initialize(FileName);
				sResponseLog.WriteDebuglog(pBuf, CWriteDebugLog::E_LOG_LEVEL_3, FALSE);
				// 画面出力
				CString *p = new CString();
				*p = pBuf;
				PostMessage(pParent->m_hWnd, CCommon::WM_USER_DSP_RECV, (WPARAM)p, 0);
			}

			/* 後処理 */
			delete [] pBuf;
		}else{
			// ログ
			CString strLog;
			strLog.Format(_T("Error : [CClient::HttpClientMain()] HttpSendRequest接続に失敗しました。GetLastError=%d"), GetLastError());
			pWriteLog->WriteDebuglog(strLog, CWriteDebugLog::E_LOG_LEVEL_1);
		
			// メッセージボックス
			CString strMessage;
			strMessage.Format(_T("接続に失敗しました。\nエラー番号：%d"), GetLastError());
			MessageBox(pParent->m_hWnd, strMessage, _T("Error"), MB_OK);
		}
	}

LABEL_ERROR:
	if (hReq)
	{
		::InternetCloseHandle(hReq);
		hReq = NULL;

		// ログ
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hReq)に成功しました。"), CWriteDebugLog::E_LOG_LEVEL_2);
	}

	if (hHttp){
		::InternetCloseHandle(hHttp);
		hHttp = NULL;

		// ログ
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hHttp)に成功しました。"), CWriteDebugLog::E_LOG_LEVEL_2);
	}
	
	if (hInet){
		::InternetCloseHandle(hInet);
		hInet = NULL;

		// ログ
		pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] InternetCloseHandle(hInet)に成功しました。"), CWriteDebugLog::E_LOG_LEVEL_2);
	}

	// ログ
	pWriteLog->WriteDebuglog(_T("[CClient::HttpClientMain()] -> OUT"), CWriteDebugLog::E_LOG_LEVEL_2);

	return TRUE;

}
	
/* ==================================================================================================== */
/**
	* @brief タグの値を変更
	* @param  nJmfType		: [i]送信JMF種別
	* @param  SourceJmf		: [i]変更前のJMFデータ
	* @retval タグを値に変更したJMFデータ
**/
/* ==================================================================================================== */
CString CClient::TagConversion( UINT nJmfType, const CString& SourceJmf )
{
	CString DstJmf(SourceJmf);

	// HeaderのDeviceID
	if ( DstJmf.Find( CONV_DEVICEID ) != -1 ) DstJmf.Replace(CONV_DEVICEID, CGetIniFileParam::GetDeviceID());

	// SubscriptionのRepeattime
	if ( DstJmf.Find( CONV_REPEATTIME ) != -1 ) DstJmf.Replace(CONV_REPEATTIME, CGetIniFileParam::GetSubscriptionRepeattime());
	// SubscriptionのURL
	if ( DstJmf.Find( CONV_URL ) != -1 ) DstJmf.Replace(CONV_URL, CGetIniFileParam::GetSubscriptionURL());
	// SubscriptionのChannelMode
	if ( DstJmf.Find( CONV_CHANNELMODE ) != -1 ) DstJmf.Replace(CONV_CHANNELMODE, CGetIniFileParam::GetSubscriptionChannelMode());

	// ResourceQuParamsのResourceDetails
	if ( DstJmf.Find( CONV_RESOURCEDETAILS ) != -1 ) DstJmf.Replace(CONV_RESOURCEDETAILS, CGetIniFileParam::GetResourceQuParamsResourceDetails());
	// ResourceQuParamsのResourceName
	if ( DstJmf.Find( CONV_RESOURCENAME ) != -1 ) DstJmf.Replace(CONV_RESOURCENAME, CGetIniFileParam::GetResourceQuParamsResourceName());
	// ResourceQuParamsのScope
	if ( DstJmf.Find( CONV_SCOPE ) != -1 ) DstJmf.Replace(CONV_SCOPE, CGetIniFileParam::GetResourceQuParamsScope());

	// StopPersChParamsのMessageType
	if ( DstJmf.Find( CONV_MESSAGETYPE ) != -1 ) DstJmf.Replace(CONV_MESSAGETYPE, CGetIniFileParam::GetStopPersChParamsMessageType());
	// StopPersChParamsのChannelID:Q1(デフォルトキー：仮)
	if ( DstJmf.Find( CONV_CHANNELID ) != -1 ) DstJmf.Replace(CONV_CHANNELID, CGetIniFileParam::GetStopPersChParamsChannelID());
	// StopPersChParamsのURL:http://192.168.0.40(デフォルトキー：仮)
	if ( DstJmf.Find( CONV_STOP_URL ) != -1 ) DstJmf.Replace(CONV_STOP_URL, CGetIniFileParam::GetStopPersChParamsURL());

	// ResourceCmdParamsのUpdateMethod
	if ( DstJmf.Find( CONV_UPDATEMETHOD ) != -1 ) DstJmf.Replace(CONV_UPDATEMETHOD, CGetIniFileParam::GetResourceCmdParamsUpdateMethod());

	// ResouceSetのName
	if ( DstJmf.Find( CONV_NAME ) != -1 ) DstJmf.Replace(CONV_NAME, CGetIniFileParam::GetResouceSetName());

	// ResourceのDescriptiveName
	if ( DstJmf.Find( CONV_DESCRIPTIVENAME ) != -1 ) DstJmf.Replace(CONV_DESCRIPTIVENAME, CGetIniFileParam::GetResourceDescriptiveName());
	// ResourceのExternalID
	if ( DstJmf.Find( CONV_EXTERNALID ) != -1 ) DstJmf.Replace(CONV_EXTERNALID, CGetIniFileParam::GetResourceExternalID());

	// MediaのDimension
	if ( DstJmf.Find( CONV_DIMENSION ) != -1 ) DstJmf.Replace(CONV_DIMENSION, CGetIniFileParam::GetMediaDimension());
	// MediaのMediaType
	if ( DstJmf.Find( CONV_MEDIATYPE ) != -1 ) DstJmf.Replace(CONV_MEDIATYPE, CGetIniFileParam::GetMediaMediaType());
	// MediaのThickness
	if ( DstJmf.Find( CONV_THICKNESS ) != -1 ) DstJmf.Replace(CONV_THICKNESS, CGetIniFileParam::GetMediaThickness());
	// Mediaのscr:UWTension
	if ( DstJmf.Find( CONV_UWTENSION ) != -1 ) DstJmf.Replace(CONV_UWTENSION, CGetIniFileParam::GetMediaUWTension());
	// Mediaのscr:MaxRunSpeed
	if ( DstJmf.Find( CONV_MAXRUNSPEED ) != -1 ) DstJmf.Replace(CONV_MAXRUNSPEED, CGetIniFileParam::GetMediaMaxRunSpeed());
	// MediaのRollDiameter
	if ( DstJmf.Find( CONV_ROLLDIAMETER ) != -1 ) DstJmf.Replace(CONV_ROLLDIAMETER, CGetIniFileParam::GetMediaRollDiameter());

	// HeaderのID設定
	if ( DstJmf.Find( CONV_SENDID ) != -1 ) DstJmf.Replace(CONV_SENDID, CGetIniFileParam::GetJmfID(nJmfType, CGetTime::GetStrDate()));

	// 日時設定
	if ( DstJmf.Find( CONV_TIME ) != -1 ) DstJmf.Replace(CONV_TIME, CCommon::CreateTimeStamp());


	if ( DstJmf.Find( CONV_STATUS ) != -1 ) DstJmf.Replace(CONV_STATUS, CGetIniFileParam::GetUwStatus());


	return DstJmf;
}