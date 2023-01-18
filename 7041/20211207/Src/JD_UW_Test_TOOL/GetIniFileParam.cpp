/**
 * @file  GetIniFileParam.cpp
 * @brief INIファイル値取得 ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "GetIniFileParam.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------
// TPの受信ポート(UWの送信ポート):50102(デフォルトキー)
//----------------------------------------------------------------------------
int CGetIniFileParam::GetSendPortNumber()
{
	return GetPrivateProfileInt(D_SECTION_SETTING, _T("HTTP_SEND_PORT_NUMBER"), 50102, CCommon::GetToolIniPath());
}

//----------------------------------------------------------------------------
// TPの送信ポート(UWの受信ポート):50131(デフォルトキー)
//----------------------------------------------------------------------------
int CGetIniFileParam::GetRecvPortNumber()
{
	return GetPrivateProfileInt(D_SECTION_SETTING, _T("HTTP_RECV_PORT_NUMBER"), 50131, CCommon::GetToolIniPath());
}

//----------------------------------------------------------------------------
// TPのIPアドレス:192.168.0.30(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetRecvIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("HTTP_RECV_IP_ADDRESS"), _T("192.168.0.30"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// UWのIPアドレス:192.168.0.40(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSendIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("HTTP_SEND_IP_ADDRESS"), _T("192.168.0.40"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// UWのフォルダ名:(デフォルトなし)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSendFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("SEND_SEND_FOLDER"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// TPのフォルダ名:(デフォルトなし)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSendStatusFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("SEND_STATUS_FOLDER"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}
CString CGetIniFileParam::GetSendPaperFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("SEND_PAPER_FOLDER"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// HeaderのDeviceID:Truepress(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetDeviceID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Header, _T("DeviceID"), _T("Truepress"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscriptionのRepeattime:30(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionRepeattime()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("Repeattime"), _T("30"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscriptionのURL:xjmfurl(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionURL()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("URL"), _T("xjmfurl"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscriptionのChannelMode:FireAndForget(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionChannelMode()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("ChannelMode"), _T("FireAndForget"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParamsのResourceDetails:Full(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsResourceDetails()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("ResourceDetails"), _T("Full"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParamsのResourceName:Media(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsResourceName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("ResourceName"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParamsのScope:Present(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsScope()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("Scope"), _T("Present"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParamsのMessageType:Media(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsMessageType()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("MessageType"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParamsのChannelID:Q1(デフォルトキー：仮)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsChannelID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("ChannelID"), _T("Q1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParamsのURL:http://192.168.0.40(デフォルトキー：仮)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsURL()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("URL"), _T("http://192.168.0.40"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceCmdParamsのUpdateMethod:CompleteSet(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceCmdParamsUpdateMethod()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceCmdParams, _T("UpdateMethod"), _T("CompleteSet"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResouceSetのName:Media(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResouceSetName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResouceSet, _T("Name"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceのDescriptiveName:NextIJ#1(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceDescriptiveName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Resource, _T("DescriptiveName"), _T("NextIJ#1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceのExternalID:ID_1(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceExternalID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Resource, _T("ExternalID"), _T("ID_1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// MediaのDimension:520.00 305.00(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaDimension()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("Dimension"), _T("520.00 305.00"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// MediaのMediaType:NextIJ(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaMediaType()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("MediaType"), _T("NextIJ"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// MediaのThickness:130(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaThickness()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("Thickness"), _T("130"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Mediaのscr:UWTension:5(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaUWTension()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("UWTension"), _T("5"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Mediaのscr:MaxRunSpeed:120(デフォルトキー)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaMaxRunSpeed()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("scr:MaxRunSpeed"), _T("120"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

CString CGetIniFileParam::GetMediaRollDiameter()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("RollDiameter"), _T("0"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

CString CGetIniFileParam::GetUwStatus()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(_T("Status"), _T("Status"), _T("wait"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

UINT CGetIniFileParam::GetStatusRepeattime()
{
	return GetPrivateProfileInt(_T("Status"), _T("Repeattime"), 30, CCommon::GetToolIniPath());
}

UINT CGetIniFileParam::GetPaperRepeattime()
{
	return GetPrivateProfileInt(D_SECTION_Media, _T("Repeattime"), 30, CCommon::GetToolIniPath());
}


// --------------------------------------------------------------------------
// ログ出力フォルダ
// --------------------------------------------------------------------------
CString CGetIniFileParam::GetOutputLogPath()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_LOG, _T("OUTPUT_LOG_PATH"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

// --------------------------------------------------------------------------
// デバッグ用のログ出力レベルを指定
// --------------------------------------------------------------------------
int CGetIniFileParam::GetDebugLogLevel()
{
	return GetPrivateProfileInt(D_SECTION_LOG, _T("OUTPUT_LOG_LEVEL"), 0, CCommon::GetToolIniPath());
}

// --------------------------------------------------------------------------
// 試験用対向モジュール
// --------------------------------------------------------------------------
BOOL CGetIniFileParam::IsOpposite()
{
	return GetPrivateProfileInt(D_SECTION_DBG, _T("Opposite"), 0, CCommon::GetToolIniPath());
}

// --------------------------------------------------------------------------
// 試験用拡張
// --------------------------------------------------------------------------
UINT CGetIniFileParam::GetReturnCode()
{
	return GetPrivateProfileInt(D_SECTION_Responce, _T("ReturnCode"), 0, CCommon::GetToolIniPath());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Work.ini
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ==================================================================================================== */
/**
	* @brief  Jmf種別毎のIDを取得する
	* @param  nJmfType		: [i]送信JMF種別
	* @param  NowDate		: [i]現在日
	* @retval ＩＤ
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetJmfID(int nJmfType, const CString& NowDate)
{
	int nSendID = 1;

	if ( GetStartupDate() == NowDate )
	{	// 日付の変化なし
		nSendID = GetSendID(nJmfType);	// 送信ID番号取得
		nSendID++;						// IDをインクリメント
		SetSendID(nJmfType, nSendID);	// 送信ID番号更新
	}
	else
	{	// 日付の変化あり
		SetStartupDate(NowDate);		// 本日の日付を設定
		ResetSendID();					// 全てのSendIDをリセット
		nSendID = 1;					// 送信IDは1から
		SetSendID(nJmfType, nSendID);	// 送信ID番号更新
	}

	// ＩＤは、接頭文字_日付(YYYYMMDD)_日単位・JMF単位の連番(1～n)
	CString JmfID;
	JmfID.Format(_T("%s_%s_%05d"), GetPrefix(nJmfType), NowDate, nSendID);
	
	return JmfID;
}

/* ==================================================================================================== */
/**
	* @brief  直前にJMFを送信した日付を取得する
	* @retval 直前にJMFを送信した日付
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetStartupDate()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("Date"), _T(""), tcWork, sizeof(tcWork), CCommon::GetWorkIniPath());
	return CString(tcWork);
}

/* ==================================================================================================== */
/**
	* @brief  JMFを送信する日付を更新する
	* @param  Date		: [i]JMFを送信する日付
**/
/* ==================================================================================================== */
void CGetIniFileParam::SetStartupDate(const CString& Date)
{
	WritePrivateProfileString(D_SECTION_SETTING, _T("Date"), Date, CCommon::GetWorkIniPath());
}

/* ==================================================================================================== */
/**
	* @brief  JMF種別毎の通番を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval JMF種別毎の通番
**/
/* ==================================================================================================== */
int CGetIniFileParam::GetSendID(int nJmfType)
{
	return GetPrivateProfileInt(GetJmfSection(nJmfType), _T("ID"), 0, CCommon::GetWorkIniPath());
}

/* ==================================================================================================== */
/**
	* @brief  JMF種別毎の通番を更新する
	* @param  nJmfType	: [i]JMF種別
	* @param  nSendID	: [i]通番
**/
/* ==================================================================================================== */
void CGetIniFileParam::SetSendID(int nJmfType, int nSendID)
{
	CString ID;
	ID.Format(_T("%d"), nSendID);
	WritePrivateProfileString(GetJmfSection(nJmfType), _T("ID"), ID, CCommon::GetWorkIniPath());
}

/* ==================================================================================================== */
/**
	* @brief  JMF種別毎の通番のセクション名を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval JMF種別毎の通番のセクション名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetJmfSection(int nJmfType)
{
	CString JmfSection;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		JmfSection = D_JMF_QueryStatus;
		break;
	case CCommon::E_JMF_QueryResource:
		JmfSection = D_JMF_QueryResource;
		break;
	case CCommon::E_JMF_StopPersChParams:
		JmfSection = D_JMF_StopPersChParams;
		break;
	case CCommon::E_JMF_CommandResource:
		JmfSection = D_JMF_CommandResource;
		break;
	case CCommon::E_JMF_CommandWakeup:
		JmfSection = D_JMF_CommandWakeup;
		break;
	}

	return JmfSection;
}

/* ==================================================================================================== */
/**
	* @brief  JMF種別毎の通番をリセットする
**/
/* ==================================================================================================== */
void CGetIniFileParam::ResetSendID()
{
	SetSendID(CCommon::E_JMF_QueryStatus,		0);
	SetSendID(CCommon::E_JMF_QueryResource,		0);
	SetSendID(CCommon::E_JMF_StopPersChParams,	0);
	SetSendID(CCommon::E_JMF_CommandResource,	0);
	SetSendID(CCommon::E_JMF_CommandWakeup,		0);
}

/* ==================================================================================================== */
/**
	* @brief  JMF種別毎のＩＤの接頭名を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval  JMF種別毎のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefix(int nJmfType)
{
	CString JmfPrefix;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		JmfPrefix = GetPrefixQueryStatus();
		break;
	case CCommon::E_JMF_QueryResource:
		JmfPrefix = GetPrefixQueryResource();
		break;
	case CCommon::E_JMF_StopPersChParams:
		JmfPrefix = GetPrefixStopPersChParams();
		break;
	case CCommon::E_JMF_CommandResource:
		JmfPrefix = GetPrefixCommandResource();
		break;
	case CCommon::E_JMF_CommandWakeup:
		JmfPrefix = GetPrefixCommandWakeup();
		break;
	}

	return JmfPrefix;
}

/* ==================================================================================================== */
/**
	* @brief  「状態監視」のＩＤの接頭名を取得する
	* @retval 「状態監視」のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixQueryStatus()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_QueryStatus, _T("Status"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

/* ==================================================================================================== */
/**
	* @brief  「用紙情報登録」のＩＤの接頭名を取得する
	* @retval 「用紙情報登録」のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixQueryResource()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_QueryResource, _T("Q"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

/* ==================================================================================================== */
/**
	* @brief  「通知チャネル削除」のＩＤの接頭名を取得する
	* @retval 「通知チャネル削除」のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixStopPersChParams()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_StopPersChParams, _T("ST"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

/* ==================================================================================================== */
/**
	* @brief  「印刷条件設定」のＩＤの接頭名を取得する
	* @retval 「印刷条件設定」のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixCommandResource()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_CommandResource, _T("CR"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

/* ==================================================================================================== */
/**
	* @brief  「READY状態への遷移要請」のＩＤの接頭名を取得する
	* @retval 「READY状態への遷移要請」のＩＤの接頭名
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixCommandWakeup()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_CommandWakeup, _T("CW"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

