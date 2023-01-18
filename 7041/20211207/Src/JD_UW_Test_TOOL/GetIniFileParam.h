/**
 * @file  GetIniFileParam.h
 * @brief INIファイル値取得 ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/
#pragma once

#define D_SECTION_SETTING			_T("SETTING")
#define D_SECTION_LOG				_T("LOG")
#define D_SECTION_DBG				_T("DBG")
#define D_SECTION_PREFIX			_T("PREFIX")

#define D_SECTION_Header			_T("Header")
#define D_SECTION_Subscription		_T("Subscription")
#define D_SECTION_ResourceQuParams	_T("ResourceQuParams")
#define D_SECTION_StopPersChParams	_T("StopPersChParams")
#define D_SECTION_ResourceCmdParams	_T("ResourceCmdParams")
#define D_SECTION_ResouceSet		_T("ResouceSet")
#define D_SECTION_Resource			_T("Resource")
#define D_SECTION_Media				_T("Media")

#define D_JMF_QueryStatus			_T("QueryStatus")
#define D_JMF_QueryResource			_T("QueryResource")
#define D_JMF_StopPersChParams		_T("StopPersChParams")
#define D_JMF_CommandResource		_T("CommandResource")
#define D_JMF_CommandWakeup			_T("CommandWakeup")

#define D_SECTION_Responce			_T("Responce")

/**
* @brief INI情報取得/更新クラス
*/
class CGetIniFileParam
{
public:
	//----------------------------------------------------------------------------
	// SETTING
	//----------------------------------------------------------------------------
	// TPの受信ポート(UWの送信ポート):50102(デフォルトキー)
	static int GetSendPortNumber();
	// TPの送信ポート(UWの受信ポート):50131(デフォルトキー)
	static int GetRecvPortNumber();
	// TPのIPアドレス:192.168.0.30(デフォルトキー)
	static CString GetRecvIpAddress();
	// UWのIPアドレス:192.168.0.40(デフォルトキー)
	static CString GetSendIpAddress();
	// UWのフォルダ名:(デフォルトなし)
	static CString GetSendFolder();

	//----------------------------------------------------------------------------
	// Header
	//----------------------------------------------------------------------------
	// HeaderのDeviceID:Truepress(デフォルトキー)
	static CString GetDeviceID();

	//----------------------------------------------------------------------------
	// Subscription
	//----------------------------------------------------------------------------
	// subscriptionのRepeattime:30(デフォルトキー)
	static CString GetSubscriptionRepeattime();
	// subscriptionのURL:xjmfurl(デフォルトキー)
	static CString GetSubscriptionURL();
	// subscriptionのChannelMode:FireAndForget(デフォルトキー)
	static CString GetSubscriptionChannelMode();

	//----------------------------------------------------------------------------
	// ResourceQuParams
	//----------------------------------------------------------------------------
	// ResourceQuParamsのResourceDetails:Full(デフォルトキー)
	static CString GetResourceQuParamsResourceDetails();
	// ResourceQuParamsのResourceName:Media(デフォルトキー)
	static CString GetResourceQuParamsResourceName();
	// ResourceQuParamsのScope:Present(デフォルトキー)
	static CString GetResourceQuParamsScope();

	//----------------------------------------------------------------------------
	// StopPersChParams
	//----------------------------------------------------------------------------
	// StopPersChParamsのMessageType:Media(デフォルトキー)
	static CString GetStopPersChParamsMessageType();
	// StopPersChParamsのChannelID:Q1(デフォルトキー：仮)
	static CString GetStopPersChParamsChannelID();
	// StopPersChParamsのURL:http://192.168.0.40(デフォルトキー：仮)
	static CString GetStopPersChParamsURL();

	//----------------------------------------------------------------------------
	// ResourceCmdParams
	//----------------------------------------------------------------------------
	// ResourceCmdParamsのUpdateMethod:CompleteSet(デフォルトキー)
	static CString GetResourceCmdParamsUpdateMethod();

	//----------------------------------------------------------------------------
	// ResouceSet
	//----------------------------------------------------------------------------
	// ResouceSetのName:Media(デフォルトキー)
	static CString GetResouceSetName();

	//----------------------------------------------------------------------------
	// Resource
	//----------------------------------------------------------------------------
	// ResourceのDescriptiveName:NextIJ#1(デフォルトキー)
	static CString GetResourceDescriptiveName();
	// ResourceのExternalID:ID_1(デフォルトキー)
	static CString GetResourceExternalID();

	//----------------------------------------------------------------------------
	// Media
	//----------------------------------------------------------------------------
	// MediaのDimension:520.00 305.00(デフォルトキー)
	static CString GetMediaDimension();
	// MediaのMediaType:NextIJ(デフォルトキー)
	static CString GetMediaMediaType();
	// MediaのThickness:130(デフォルトキー)
	static CString GetMediaThickness();
	// Mediaのscr:UWTension:5(デフォルトキー)
	static CString GetMediaUWTension();
	// Mediaのscr:MaxRunSpeed:120(デフォルトキー)
	static CString GetMediaMaxRunSpeed();

	static CString GetMediaRollDiameter();
	static CString GetUwStatus();
	static UINT GetStatusRepeattime();
	static UINT GetPaperRepeattime();

	//----------------------------------------------------------------------------
	// LOG
	//----------------------------------------------------------------------------
	static CString GetOutputLogPath();
	static int GetDebugLogLevel();

	//----------------------------------------------------------------------------
	// 試験モジュール関連
	//----------------------------------------------------------------------------
	static BOOL IsOpposite();

	//----------------------------------------------------------------------------
	// Work.ini
	//----------------------------------------------------------------------------
	static CString GetJmfID(int nJmfType, const CString& NowDate);
	static CString GetStartupDate();
	static int GetSendID(int nJmfType);
	static void SetStartupDate(const CString& Date);
	static void SetSendID(int nJmfType, int nSendID);
	static CString GetJmfSection(int nJmfType);
	static void ResetSendID();

	//----------------------------------------------------------------------------
	// PREFIX
	//----------------------------------------------------------------------------
	static CString GetPrefix(int nJmfType);
	static CString GetPrefixQueryStatus();
	static CString GetPrefixQueryResource();
	static CString GetPrefixStopPersChParams();
	static CString GetPrefixCommandResource();
	static CString GetPrefixCommandWakeup();

	// --------------------------------------------------------------------------
	// 試験用拡張（2021-11-25 Add）
	// --------------------------------------------------------------------------
	static UINT GetReturnCode();

	static CString GetSendStatusFolder();
	static CString GetSendPaperFolder();
};