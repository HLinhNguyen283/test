/**
 * @file  JmfData.cpp
 * @brief JMFの雛形データ
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
#include "JmfData.h"
#include "GetIniFileParam.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ==================================================================================================== */
/**
	* @brief  送信データの雛形を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval  送信データの雛形
**/
/* ==================================================================================================== */
CString GetJmfData(UINT nJmfType)
{
	CString JmfData;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		// 状態監視
		JmfData = Jmf_QueryStatus;
		break;
	case CCommon::E_JMF_QueryResource:
		// 用紙情報登録
		JmfData = Jmf_QueryResource;
		break;
	case CCommon::E_JMF_StopPersChParams:
		// チャネル削除
		JmfData = Jmf_StopPersChParams;
		break;
	case CCommon::E_JMF_CommandResource:
		// 印刷条件設定
		JmfData = Jmf_CommandResource;
		break;
	case CCommon::E_JMF_CommandWakeup:
		// READY状態への遷移要請
		JmfData = Jmf_CommandWakeup;
		break;
	case CCommon::E_JMF_SignalStatusPaper:
		// UWからのSignalStatus(Paper)
		JmfData = Jmf_SignalStatusPaper;
		break;
	case CCommon::E_JMF_SignalStatus:
		// UWからのSignalStatus
		JmfData = Jmf_SignalStatus;
		break;
	}

	return JmfData;
}

/* ==================================================================================================== */
/**
	* @brief  送信レスポンスデータの雛形を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval  送信レスポンスデータの雛形
**/
/* ==================================================================================================== */
CString GetJmfResponseData(UINT nJmfType)
{
	CString JmfData;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		// 状態監視
		JmfData = Jmf_QueryStatusRes;
		break;
	case CCommon::E_JMF_QueryResource:
		// 用紙情報登録
		JmfData = Jmf_QueryResourceRes;
		break;
	case CCommon::E_JMF_StopPersChParams:
		// 通知チャネル削除
		JmfData = Jmf_StopPersChParamsRes;
		break;
	case CCommon::E_JMF_CommandResource:
		// 印刷条件設定
		JmfData = Jmf_CommandResourceRes;
		break;
	case CCommon::E_JMF_CommandWakeup:
		// READY状態への遷移要請
		JmfData = Jmf_CommandWakeupRes;
		break;
	}

	CString s;
	s.Format( JmfData, CGetIniFileParam::GetReturnCode() );
	JmfData = s;

		// 日時設定
	if ( JmfData.Find( CONV_TIME ) != -1 ) JmfData.Replace(CONV_TIME, CCommon::CreateTimeStamp());

	return JmfData;
}

//----------------------------------------------------------------------------
// 状態監視
//----------------------------------------------------------------------------
TCHAR Jmf_QueryStatus[] = 
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<QueryStatus>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
		<Subscription RepeatTime=\"\\REPEATTIME\\\" URL=\"\\URL\\\" />\r\n\
		<StatusQuParams />\r\n\
	</QueryStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// 用紙情報登録
//----------------------------------------------------------------------------
TCHAR Jmf_QueryResource [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<QueryResource>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
		<Subscription ChannelMode=\"\\CHANNELMODE\\\" URL=\"\\URL\\\" />\r\n\
		<ResourceQuParams ResourceDetails=\"\\RESOURCEDETAILS\\\" ResourceName=\"\\RESOURCENAME\\\" Scope=\"\\SCOPE\\\" />\r\n\
	</QueryResource>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// チャネル削除
//----------------------------------------------------------------------------
TCHAR Jmf_StopPersChParams [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<StopPersChParams ChannelID=\"\\CHANNELID\\\" MessageType=\"\\MESSAGETYPE\\\" URL=\"\\STOP_URL\\\" />\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// 印刷条件設定
//----------------------------------------------------------------------------
TCHAR Jmf_CommandResource [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\" xmlns:scr=\"http://www.screen.co.jp/JDF/JDFSchema_3_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<CommandResource>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
		<ResourceCmdParams UpdateMethod=\"\\UPDATEMETHOD\\\">\r\n\
			<ResourceSet Name=\"\\NAME\\\">\r\n\
				<Resource DescriptiveName=\"\\DESCRIPTIVENAME\\\" ExternalID=\"\\EXTERNALID\\\">\r\n\
					<Media Dimension=\"\\DIMENSION\\\" MediaType=\"\\MEDIATYPE\\\" Thickness=\"\\THICKNESS\\\" scr:UWTension=\"\\UWTENSION\\\" scr:MaxRunSpeed=\"\\MAXRUNSPEED\\\" />\r\n\
				</Resource>\r\n\
			</ResourceSet>\r\n\
		</ResourceCmdParams>\r\n\
	</CommandResource>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// READY状態への遷移要請
//----------------------------------------------------------------------------
TCHAR Jmf_CommandWakeup [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<CommandWakeup>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	</CommandWakeup>\r\n\
</XJMF>\r\n");


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 試験対向用
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// UWからのJmf_SignalStatusPaper(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_SignalStatusPaper [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<SignalStatus>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" refID=\"Q1\" />\r\n\
		<ResourceInfo Scope=\"Present\">\r\n\
			<ResourceSet Name=\"Media\">\r\n\
				<Resource DescriptiveName=\"\\DESCRIPTIVENAME\\\" ExternalID=\"\\EXTERNALID\\\">\r\n\
					<Media Dimension=\"\\DIMENSION\\\" MediaType=\"\\MEDIATYPE\\\" RollDiameter=\"\\ROLLDIAMETER\\\" Thickness=\"\\THICKNESS\\\" />\r\n\
				</Resource>\r\n\
			</ResourceSet>\r\n\
		</ResourceInfo>\r\n\
	</SignalStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// UWからのSignalStatus(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_SignalStatus [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<SignalStatus>\r\n\
		<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" refID=\"Q1\" />\r\n\
		<DeviceInfo Status=\"\\STATUS\\\" />\r\n\
	</SignalStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// 状態監視のレスポンス(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_QueryStatusRes[] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"Truepress\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<ResponseStatus ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"20\" refID=\"Status1\" />\r\n\
	</ResponseStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// 用紙情報登録のレスポンス(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_QueryResourceRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<ResponseStatus ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" refID=\"Q1\" />\r\n\
	</ResponseStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// チャネル削除のレスポンス(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_StopPersChParamsRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<Response ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" refID=\"Q1\" />\r\n\
		<SubscriptionInfo DeviceID=\"UW0001\" MessageType=\"Media\" ChannelID=\"Q1\">\r\n\
		<Subscription ChannelMode=\"FireAndForget\" URL=\"http://192.168.0.30:50102\" />\r\n\
		</SubscriptionInfo>\r\n\
	</Response>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// 印刷条件設定のレスポンス(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_CommandResourceRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\" xmlns:scr=\"http://www.screen.co.jp/JDF/JDFSchema_3_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<Response ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
		<ResourceInfo CommandResult=\"Replaced\">\r\n\
			<ResourceSet Name=\"Media\">\r\n\
				<Resource DescriptiveName=\"NextIJ #1\" ExternalID=\"ID_1\">\r\n\
					<Media Dimension=\"D\" MediaType=\"M\" Thickness=\"81\" scr:UWTension=\"5\" />\r\n\
				</Resource>\r\n\
			</ResourceSet>\r\n\
		</ResourceInfo>\r\n\
	</Response>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// READY状態への遷移要請のレスポンス(データ固定)
//----------------------------------------------------------------------------
TCHAR Jmf_CommandWakeupRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<Response ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	</Response>\r\n\
</XJMF>\r\n");
