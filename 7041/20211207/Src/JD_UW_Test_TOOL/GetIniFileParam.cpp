/**
 * @file  GetIniFileParam.cpp
 * @brief INI�t�@�C���l�擾 �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
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
// TP�̎�M�|�[�g(UW�̑��M�|�[�g):50102(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
int CGetIniFileParam::GetSendPortNumber()
{
	return GetPrivateProfileInt(D_SECTION_SETTING, _T("HTTP_SEND_PORT_NUMBER"), 50102, CCommon::GetToolIniPath());
}

//----------------------------------------------------------------------------
// TP�̑��M�|�[�g(UW�̎�M�|�[�g):50131(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
int CGetIniFileParam::GetRecvPortNumber()
{
	return GetPrivateProfileInt(D_SECTION_SETTING, _T("HTTP_RECV_PORT_NUMBER"), 50131, CCommon::GetToolIniPath());
}

//----------------------------------------------------------------------------
// TP��IP�A�h���X:192.168.0.30(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetRecvIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("HTTP_RECV_IP_ADDRESS"), _T("192.168.0.30"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// UW��IP�A�h���X:192.168.0.40(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSendIpAddress()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("HTTP_SEND_IP_ADDRESS"), _T("192.168.0.40"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// UW�̃t�H���_��:(�f�t�H���g�Ȃ�)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSendFolder()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_SETTING, _T("SEND_SEND_FOLDER"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// TP�̃t�H���_��:(�f�t�H���g�Ȃ�)
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
// Header��DeviceID:Truepress(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetDeviceID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Header, _T("DeviceID"), _T("Truepress"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscription��Repeattime:30(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionRepeattime()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("Repeattime"), _T("30"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscription��URL:xjmfurl(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionURL()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("URL"), _T("xjmfurl"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// subscription��ChannelMode:FireAndForget(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetSubscriptionChannelMode()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Subscription, _T("ChannelMode"), _T("FireAndForget"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParams��ResourceDetails:Full(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsResourceDetails()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("ResourceDetails"), _T("Full"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParams��ResourceName:Media(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsResourceName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("ResourceName"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceQuParams��Scope:Present(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceQuParamsScope()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceQuParams, _T("Scope"), _T("Present"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParams��MessageType:Media(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsMessageType()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("MessageType"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParams��ChannelID:Q1(�f�t�H���g�L�[�F��)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsChannelID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("ChannelID"), _T("Q1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// StopPersChParams��URL:http://192.168.0.40(�f�t�H���g�L�[�F��)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetStopPersChParamsURL()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_StopPersChParams, _T("URL"), _T("http://192.168.0.40"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResourceCmdParams��UpdateMethod:CompleteSet(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceCmdParamsUpdateMethod()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResourceCmdParams, _T("UpdateMethod"), _T("CompleteSet"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// ResouceSet��Name:Media(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResouceSetName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_ResouceSet, _T("Name"), _T("Media"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Resource��DescriptiveName:NextIJ#1(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceDescriptiveName()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Resource, _T("DescriptiveName"), _T("NextIJ#1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Resource��ExternalID:ID_1(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetResourceExternalID()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Resource, _T("ExternalID"), _T("ID_1"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Media��Dimension:520.00 305.00(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaDimension()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("Dimension"), _T("520.00 305.00"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Media��MediaType:NextIJ(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaMediaType()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("MediaType"), _T("NextIJ"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Media��Thickness:130(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaThickness()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("Thickness"), _T("130"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Media��scr:UWTension:5(�f�t�H���g�L�[)
//----------------------------------------------------------------------------
CString CGetIniFileParam::GetMediaUWTension()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_Media, _T("UWTension"), _T("5"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

//----------------------------------------------------------------------------
// Media��scr:MaxRunSpeed:120(�f�t�H���g�L�[)
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
// ���O�o�̓t�H���_
// --------------------------------------------------------------------------
CString CGetIniFileParam::GetOutputLogPath()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_LOG, _T("OUTPUT_LOG_PATH"), _T(""), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

// --------------------------------------------------------------------------
// �f�o�b�O�p�̃��O�o�̓��x�����w��
// --------------------------------------------------------------------------
int CGetIniFileParam::GetDebugLogLevel()
{
	return GetPrivateProfileInt(D_SECTION_LOG, _T("OUTPUT_LOG_LEVEL"), 0, CCommon::GetToolIniPath());
}

// --------------------------------------------------------------------------
// �����p�Ό����W���[��
// --------------------------------------------------------------------------
BOOL CGetIniFileParam::IsOpposite()
{
	return GetPrivateProfileInt(D_SECTION_DBG, _T("Opposite"), 0, CCommon::GetToolIniPath());
}

// --------------------------------------------------------------------------
// �����p�g��
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
	* @brief  Jmf��ʖ���ID���擾����
	* @param  nJmfType		: [i]���MJMF���
	* @param  NowDate		: [i]���ݓ�
	* @retval �h�c
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetJmfID(int nJmfType, const CString& NowDate)
{
	int nSendID = 1;

	if ( GetStartupDate() == NowDate )
	{	// ���t�̕ω��Ȃ�
		nSendID = GetSendID(nJmfType);	// ���MID�ԍ��擾
		nSendID++;						// ID���C���N�������g
		SetSendID(nJmfType, nSendID);	// ���MID�ԍ��X�V
	}
	else
	{	// ���t�̕ω�����
		SetStartupDate(NowDate);		// �{���̓��t��ݒ�
		ResetSendID();					// �S�Ă�SendID�����Z�b�g
		nSendID = 1;					// ���MID��1����
		SetSendID(nJmfType, nSendID);	// ���MID�ԍ��X�V
	}

	// �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
	CString JmfID;
	JmfID.Format(_T("%s_%s_%05d"), GetPrefix(nJmfType), NowDate, nSendID);
	
	return JmfID;
}

/* ==================================================================================================== */
/**
	* @brief  ���O��JMF�𑗐M�������t���擾����
	* @retval ���O��JMF�𑗐M�������t
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
	* @brief  JMF�𑗐M������t���X�V����
	* @param  Date		: [i]JMF�𑗐M������t
**/
/* ==================================================================================================== */
void CGetIniFileParam::SetStartupDate(const CString& Date)
{
	WritePrivateProfileString(D_SECTION_SETTING, _T("Date"), Date, CCommon::GetWorkIniPath());
}

/* ==================================================================================================== */
/**
	* @brief  JMF��ʖ��̒ʔԂ��擾����
	* @param  nJmfType	: [i]JMF���
	* @retval JMF��ʖ��̒ʔ�
**/
/* ==================================================================================================== */
int CGetIniFileParam::GetSendID(int nJmfType)
{
	return GetPrivateProfileInt(GetJmfSection(nJmfType), _T("ID"), 0, CCommon::GetWorkIniPath());
}

/* ==================================================================================================== */
/**
	* @brief  JMF��ʖ��̒ʔԂ��X�V����
	* @param  nJmfType	: [i]JMF���
	* @param  nSendID	: [i]�ʔ�
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
	* @brief  JMF��ʖ��̒ʔԂ̃Z�N�V���������擾����
	* @param  nJmfType	: [i]JMF���
	* @retval JMF��ʖ��̒ʔԂ̃Z�N�V������
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
	* @brief  JMF��ʖ��̒ʔԂ����Z�b�g����
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
	* @brief  JMF��ʖ��̂h�c�̐ړ������擾����
	* @param  nJmfType	: [i]JMF���
	* @retval  JMF��ʖ��̂h�c�̐ړ���
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
	* @brief  �u��ԊĎ��v�̂h�c�̐ړ������擾����
	* @retval �u��ԊĎ��v�̂h�c�̐ړ���
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
	* @brief  �u�p�����o�^�v�̂h�c�̐ړ������擾����
	* @retval �u�p�����o�^�v�̂h�c�̐ړ���
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
	* @brief  �u�ʒm�`���l���폜�v�̂h�c�̐ړ������擾����
	* @retval �u�ʒm�`���l���폜�v�̂h�c�̐ړ���
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
	* @brief  �u��������ݒ�v�̂h�c�̐ړ������擾����
	* @retval �u��������ݒ�v�̂h�c�̐ړ���
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
	* @brief  �uREADY��Ԃւ̑J�ڗv���v�̂h�c�̐ړ������擾����
	* @retval �uREADY��Ԃւ̑J�ڗv���v�̂h�c�̐ړ���
**/
/* ==================================================================================================== */
CString CGetIniFileParam::GetPrefixCommandWakeup()
{
	TCHAR	tcWork[256];
	GetPrivateProfileString(D_SECTION_PREFIX, D_JMF_CommandWakeup, _T("CW"), tcWork, sizeof(tcWork), CCommon::GetToolIniPath());
	return CString(tcWork);
}

