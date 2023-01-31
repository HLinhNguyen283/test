/**
	* @file		XmlSender.cpp
	* @brief	UWへXMLを通知する
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <http.h>
#include <wininet.h>
#include "Ini_UnwinderManager.h"
#include "XmlLogData.h"
#include "XmlSender.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  CXmlSenderコンストラクタ
	* @param  inXmlData : [i]通知するXMLデータ
**/
CXmlSender::CXmlSender(const std::string& inXmlData)
	: m_SendXmlData(inXmlData)
{
}

/**
	* @brief  CXmlSenderデストラクタ
**/
CXmlSender::~CXmlSender()
{
}

/**
	* @brief  XMLデータ通知
	* @retval 処理結果データ
	*		　正常の場合：レスポンスXMLデータ
	*		  異常の場合：[ERROR] エラー内容
**/
std::string CXmlSender::Doit()
{
	// リトライ値を取得する
	UINT RetryCount=0;
	UINT RetryInterval=0;
	GetRetryVal(RetryCount, RetryInterval);

	for( UINT i=0; i<=RetryCount; i++ )
	{
		// XMLデータを通知する
		BOOL nRet = Send();
		if ( !nRet )
		{	// 異常の場合は異常内容をログに出力する
			WriteToLogBuf(LOG_DEBUG, (char*)m_ResultData.c_str() );
		}
		else
		{	// 正常の場合
			break;
		}
	}

	return m_ResultData;
}

/**
	* @brief  XMLデータ通知
**/
BOOL CXmlSender::Send()
{
	USES_CONVERSION;				// A2W使用の場合の宣言

	BOOL nRet = FALSE;

	HINTERNET hInet = NULL;			// インターネットハンドル
	HINTERNET hHttp = NULL;			// 接続識別ハンドル
	HINTERNET hReq	= NULL;			// HTTP要求識別ハンドル

	// 送信情報取得
	std::string hostName = GetUwIpAddress();
	std::string ObjName = GetObjName();
	INTERNET_PORT nServerPort = GetSendPort();

	// インターネットのハンドルの作成
	hInet = InternetOpen("HttpClient", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hInet)
	{	// エラー
		SetErrorResultData("[CXmlSender::Doit] InternetOpen ERROR");
		goto LABEL_ERROR;
	}

	// インターネット上のサーバへの接続
	hHttp = InternetConnect(hInet, hostName.c_str(), nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(!hHttp)
	{	// エラー
		SetErrorResultData("[CXmlSender::Doit] InternetConnect ERROR");
		goto LABEL_ERROR;
	}

	// HTTPリクエストの作成
	hReq = HttpOpenRequest(hHttp, "POST", ObjName.c_str(), NULL, "", NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if(!hReq)
	{
		SetErrorResultData("[CXmlSender::Doit] HttpOpenRequest ERROR");
		goto LABEL_ERROR;
	}

	// HTTPリクエストを送信
	BOOL bRet = HttpSendRequest(hReq, NULL, 0, (LPVOID)m_SendXmlData.c_str(), m_SendXmlData.size()); 
	if (bRet)
	{
		// ログ
		WriteToLogBuf(LOG_DEBUG, "[CXmlSender::Doit] HttpSendRequest OK" );
	
		TCHAR chReadBuf[1024] = {0};
		DWORD dwSize = 1024;
		// HTTPリクエストに関連する情報を取得（HTTP_QUERY_STATUS_CODE=サーバから返された状態コード）
		HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE, chReadBuf, &dwSize, 0);
		// HTTPリクエストに関連する情報を取得（HTTP_QUERY_STATUS_TEXT=サーバから返された補足のテキスト）
		HttpQueryInfo(hReq, HTTP_QUERY_STATUS_TEXT, chReadBuf, &dwSize, 0);

		TCHAR BufSizeText[1000];
		DWORD BufSizeTextSize = 1000;
		// HTTPリクエストに関連する情報を取得（HTTP_QUERY_CONTENT_LENGTH=リソースのサイズ）
		HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, BufSizeText, &BufSizeTextSize, NULL);

		// HTTPレスポンスバッファの確保
		DWORD BufSize = atol(BufSizeText) + 1;
		TCHAR* pBuf = new TCHAR[BufSize];
		memset(pBuf, 0x00, BufSize);

		// HTTPレスポンス読み込み
		DWORD ReadSize = BufSize;
		nRet = InternetReadFile(hReq, pBuf, BufSize-1, &ReadSize);
		if ( nRet )
		{
			m_ResultData = pBuf;
			WriteToLogBuf(LOG_DEBUG, "[CXmlSender::Doit] InternetReadFile OK" );

			// レスポンスをログ出力する
			std::string strSavePath = CXmlLogData::GetResponseLogFilePath();
			CXmlLogData::WriteFile(strSavePath, m_ResultData);

			// UTF8 → Shift-JIS
			std::string ShiftJIS = ConvUTF8toShiftJIS(m_ResultData);
			m_ResultData = ShiftJIS;
		}
		else
		{
			SetErrorResultData("[CXmlSender::Doit] InternetReadFile ERROR");
		}

		// HTTPレスポンスバッファの開放
		delete [] pBuf;
	}
	else
	{	// 通知エラー
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
	* @brief  コード変換(UTF8 → Shift-JIS)
	* @param  InUTF8 : [i]UTF8の文字列
	* @retval Shift-JISの文字列
**/
std::string CXmlSender::ConvUTF8toShiftJIS( const std::string& InUTF8)
{
	//UTF-8からUTF-16へ変換
	const int nSize = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.c_str(), -1, NULL, 0 );
	BYTE* pUtf16 = new BYTE[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.c_str(), -1, (LPWSTR)pUtf16, nSize );

	//UTF-16からShift-JISへ変換
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
	* @brief  結果データにエラー情報を設定する
	* @param  inErrorMessage : [i]エラー情報
**/
void CXmlSender::SetErrorResultData(const std::string& inErrorMessage)
{
	m_ResultData = "[ERROR] " + inErrorMessage;
}

/**
	* @brief  UWのIPアドレスを取得する
	* @retval UWのIPアドレス
**/
std::string CXmlSender::GetUwIpAddress()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getUW_IpAddress();
}

/**
	* @brief  UWの受信フォルダ名を取得する
	* @retval UWの受信フォルダ名
**/
std::string CXmlSender::GetObjName()
{
	return "";
}

/**
	* @brief  UWの受信ポート番号を取得する
	* @retval UWの受信ポート番号
**/
UINT CXmlSender::GetSendPort()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return atoi(iniUnwinderManager.getSender_Port().c_str());
}

/**
	* @brief  通知時のリトライ値を取得する
	* @param  outRetryCount    : [o]リトライ回数
	* @param  outRetryInterval : [o]リトライのインターバル
**/
void CXmlSender::GetRetryVal(UINT& outRetryCount, UINT& outRetryInterval)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	outRetryCount = iniUnwinderManager.getSend_Retry_Count();
	outRetryInterval = iniUnwinderManager.getSend_Retry_Interval();
}

};	// namespace unwinder_manager