/**
 * @file  JmfData.cpp
 * @brief JMF�̐��`�f�[�^
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
	* @brief  ���M�f�[�^�̐��`���擾����
	* @param  nJmfType	: [i]JMF���
	* @retval  ���M�f�[�^�̐��`
**/
/* ==================================================================================================== */
CString GetJmfData(UINT nJmfType)
{
	CString JmfData;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		// ��ԊĎ�
		JmfData = Jmf_QueryStatus;
		break;
	case CCommon::E_JMF_QueryResource:
		// �p�����o�^
		JmfData = Jmf_QueryResource;
		break;
	case CCommon::E_JMF_StopPersChParams:
		// �`���l���폜
		JmfData = Jmf_StopPersChParams;
		break;
	case CCommon::E_JMF_CommandResource:
		// ��������ݒ�
		JmfData = Jmf_CommandResource;
		break;
	case CCommon::E_JMF_CommandWakeup:
		// READY��Ԃւ̑J�ڗv��
		JmfData = Jmf_CommandWakeup;
		break;
	case CCommon::E_JMF_SignalStatusPaper:
		// UW�����SignalStatus(Paper)
		JmfData = Jmf_SignalStatusPaper;
		break;
	case CCommon::E_JMF_SignalStatus:
		// UW�����SignalStatus
		JmfData = Jmf_SignalStatus;
		break;
	}

	return JmfData;
}

/* ==================================================================================================== */
/**
	* @brief  ���M���X�|���X�f�[�^�̐��`���擾����
	* @param  nJmfType	: [i]JMF���
	* @retval  ���M���X�|���X�f�[�^�̐��`
**/
/* ==================================================================================================== */
CString GetJmfResponseData(UINT nJmfType)
{
	CString JmfData;

	switch(nJmfType) {
	case CCommon::E_JMF_QueryStatus:
		// ��ԊĎ�
		JmfData = Jmf_QueryStatusRes;
		break;
	case CCommon::E_JMF_QueryResource:
		// �p�����o�^
		JmfData = Jmf_QueryResourceRes;
		break;
	case CCommon::E_JMF_StopPersChParams:
		// �ʒm�`���l���폜
		JmfData = Jmf_StopPersChParamsRes;
		break;
	case CCommon::E_JMF_CommandResource:
		// ��������ݒ�
		JmfData = Jmf_CommandResourceRes;
		break;
	case CCommon::E_JMF_CommandWakeup:
		// READY��Ԃւ̑J�ڗv��
		JmfData = Jmf_CommandWakeupRes;
		break;
	}

	CString s;
	s.Format( JmfData, CGetIniFileParam::GetReturnCode() );
	JmfData = s;

		// �����ݒ�
	if ( JmfData.Find( CONV_TIME ) != -1 ) JmfData.Replace(CONV_TIME, CCommon::CreateTimeStamp());

	return JmfData;
}

//----------------------------------------------------------------------------
// ��ԊĎ�
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
// �p�����o�^
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
// �`���l���폜
//----------------------------------------------------------------------------
TCHAR Jmf_StopPersChParams [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"\\DEVICEID\\\" ID=\"\\SendID\\\" Time=\"\\TIME\\\" />\r\n\
	<StopPersChParams ChannelID=\"\\CHANNELID\\\" MessageType=\"\\MESSAGETYPE\\\" URL=\"\\STOP_URL\\\" />\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// ��������ݒ�
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
// READY��Ԃւ̑J�ڗv��
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
// �����Ό��p
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// UW�����Jmf_SignalStatusPaper(�f�[�^�Œ�)
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
// UW�����SignalStatus(�f�[�^�Œ�)
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
// ��ԊĎ��̃��X�|���X(�f�[�^�Œ�)
//----------------------------------------------------------------------------
TCHAR Jmf_QueryStatusRes[] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"Truepress\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<ResponseStatus ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"20\" refID=\"Status1\" />\r\n\
	</ResponseStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// �p�����o�^�̃��X�|���X(�f�[�^�Œ�)
//----------------------------------------------------------------------------
TCHAR Jmf_QueryResourceRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<ResponseStatus ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" refID=\"Q1\" />\r\n\
	</ResponseStatus>\r\n\
</XJMF>\r\n");

//----------------------------------------------------------------------------
// �`���l���폜�̃��X�|���X(�f�[�^�Œ�)
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
// ��������ݒ�̃��X�|���X(�f�[�^�Œ�)
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
// READY��Ԃւ̑J�ڗv���̃��X�|���X(�f�[�^�Œ�)
//----------------------------------------------------------------------------
TCHAR Jmf_CommandWakeupRes [] =
_T("<XJMF xmlns=\"http://www.CIP4.org/JDFSchema_2_0\">\r\n\
	<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	<Response ReturnCode=\"%d\">\r\n\
		<Header DeviceID=\"UW0001\" ID=\"l_000002\" Time=\"\\TIME\\\" />\r\n\
	</Response>\r\n\
</XJMF>\r\n");
