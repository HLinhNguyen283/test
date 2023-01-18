/**
 * @file  GetIniFileParam.h
 * @brief INI�t�@�C���l�擾 �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
* @brief INI���擾/�X�V�N���X
*/
class CGetIniFileParam
{
public:
	//----------------------------------------------------------------------------
	// SETTING
	//----------------------------------------------------------------------------
	// TP�̎�M�|�[�g(UW�̑��M�|�[�g):50102(�f�t�H���g�L�[)
	static int GetSendPortNumber();
	// TP�̑��M�|�[�g(UW�̎�M�|�[�g):50131(�f�t�H���g�L�[)
	static int GetRecvPortNumber();
	// TP��IP�A�h���X:192.168.0.30(�f�t�H���g�L�[)
	static CString GetRecvIpAddress();
	// UW��IP�A�h���X:192.168.0.40(�f�t�H���g�L�[)
	static CString GetSendIpAddress();
	// UW�̃t�H���_��:(�f�t�H���g�Ȃ�)
	static CString GetSendFolder();

	//----------------------------------------------------------------------------
	// Header
	//----------------------------------------------------------------------------
	// Header��DeviceID:Truepress(�f�t�H���g�L�[)
	static CString GetDeviceID();

	//----------------------------------------------------------------------------
	// Subscription
	//----------------------------------------------------------------------------
	// subscription��Repeattime:30(�f�t�H���g�L�[)
	static CString GetSubscriptionRepeattime();
	// subscription��URL:xjmfurl(�f�t�H���g�L�[)
	static CString GetSubscriptionURL();
	// subscription��ChannelMode:FireAndForget(�f�t�H���g�L�[)
	static CString GetSubscriptionChannelMode();

	//----------------------------------------------------------------------------
	// ResourceQuParams
	//----------------------------------------------------------------------------
	// ResourceQuParams��ResourceDetails:Full(�f�t�H���g�L�[)
	static CString GetResourceQuParamsResourceDetails();
	// ResourceQuParams��ResourceName:Media(�f�t�H���g�L�[)
	static CString GetResourceQuParamsResourceName();
	// ResourceQuParams��Scope:Present(�f�t�H���g�L�[)
	static CString GetResourceQuParamsScope();

	//----------------------------------------------------------------------------
	// StopPersChParams
	//----------------------------------------------------------------------------
	// StopPersChParams��MessageType:Media(�f�t�H���g�L�[)
	static CString GetStopPersChParamsMessageType();
	// StopPersChParams��ChannelID:Q1(�f�t�H���g�L�[�F��)
	static CString GetStopPersChParamsChannelID();
	// StopPersChParams��URL:http://192.168.0.40(�f�t�H���g�L�[�F��)
	static CString GetStopPersChParamsURL();

	//----------------------------------------------------------------------------
	// ResourceCmdParams
	//----------------------------------------------------------------------------
	// ResourceCmdParams��UpdateMethod:CompleteSet(�f�t�H���g�L�[)
	static CString GetResourceCmdParamsUpdateMethod();

	//----------------------------------------------------------------------------
	// ResouceSet
	//----------------------------------------------------------------------------
	// ResouceSet��Name:Media(�f�t�H���g�L�[)
	static CString GetResouceSetName();

	//----------------------------------------------------------------------------
	// Resource
	//----------------------------------------------------------------------------
	// Resource��DescriptiveName:NextIJ#1(�f�t�H���g�L�[)
	static CString GetResourceDescriptiveName();
	// Resource��ExternalID:ID_1(�f�t�H���g�L�[)
	static CString GetResourceExternalID();

	//----------------------------------------------------------------------------
	// Media
	//----------------------------------------------------------------------------
	// Media��Dimension:520.00 305.00(�f�t�H���g�L�[)
	static CString GetMediaDimension();
	// Media��MediaType:NextIJ(�f�t�H���g�L�[)
	static CString GetMediaMediaType();
	// Media��Thickness:130(�f�t�H���g�L�[)
	static CString GetMediaThickness();
	// Media��scr:UWTension:5(�f�t�H���g�L�[)
	static CString GetMediaUWTension();
	// Media��scr:MaxRunSpeed:120(�f�t�H���g�L�[)
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
	// �������W���[���֘A
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
	// �����p�g���i2021-11-25 Add�j
	// --------------------------------------------------------------------------
	static UINT GetReturnCode();

	static CString GetSendStatusFolder();
	static CString GetSendPaperFolder();
};