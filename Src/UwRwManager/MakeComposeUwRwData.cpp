/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml�N���p�����[�^�쐬
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <map>
#include "XmlLogData.h"
#include "MakeComposeUwRwData.h"
#include "CommonCBRProc.h"
// ===========================================================================

namespace uwrw_manager
{

/**
	* @brief  CMakeComposeUwRwData�R���X�g���N�^
**/
CMakeComposeUwRwData::CMakeComposeUwRwData()
{
}

/**
	* @brief  CMakeComposeUwRwData�f�X�g���N�^
**/
CMakeComposeUwRwData::~CMakeComposeUwRwData()
{
}

/**
 * @brief Init data
 */
void CMakeComposeUwRwData::initialize()
{
	m_IniPrinterDescriptor.Initialize(TRUE);
	m_IniUwRwManager.Initialize(TRUE);
	m_IniUwRwManager_work.Initialize(TRUE);
}

/**
 * @brief clean up data
 */
void CMakeComposeUwRwData::finalize()
{
	m_IniPrinterDescriptor.Finalize();
	m_IniUwRwManager.Finalize();
	m_IniUwRwManager_work.Finalize();
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�iQueryStatus�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^MAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterQueryStatus(const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryStatus);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryStatus, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryStatus);
		NameValueTable["URL"]				= GetURL(E_Compose_QueryStatus, inDeviceName);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryStatus] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�iQueryResource�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^MAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterQueryResource(const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryResource, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryResource);
		NameValueTable["ChannelMode"]		= GetChannelMode();
		NameValueTable["URL"]				= GetURL(E_Compose_QueryResource, inDeviceName);
		NameValueTable["ResourceDetails"]	= GetResourceDetails();
		NameValueTable["ResourceName"]		= GetResourceName();
		NameValueTable["Scope"]				= GetScope();

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterQueryResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  �`���l���폜�iStopPersChParams�j�̑��M�f�[�^���쐬����
	* @param  inType : [i]�폜�`���l�����(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval XML���M�f�[�^MAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterStopPersChParams(int inType, const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_StopPersChParams);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_StopPersChParams, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["ChannelID"]			= GetChannelID(inType, inDeviceName);
		NameValueTable["MessageType"]		= GetMessageType();
		NameValueTable["URL"]				= GetURL(inType, inDeviceName);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterStopPersChParams] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  ��������ݒ�iCommandResource�j�̑��M�f�[�^���쐬����
	* @param  inUnwinderPaper : [i]����������
	* @retval XML���M�f�[�^MAP
**/
UwRwXjmfDataMap CMakeComposeUwRwData::ParameterCommandResource(const CUwRwPaper& inUnwinderPaper, const char* inDeviceName)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] Start");
	UwRwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_CommandResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandResource, inDeviceName);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["UpdateMethod"]		= GetUpdateMethod();
		NameValueTable["Name"]				= GetName();
		NameValueTable["DescriptiveName"]	= inUnwinderPaper.DescriptiveName;
		NameValueTable["ExternalID"]		= inUnwinderPaper.ExternalID;
		NameValueTable["Dimension"]			= inUnwinderPaper.Dimension;
		NameValueTable["MediaType"]			= inUnwinderPaper.MediaType;
		NameValueTable["Thickness"]			= inUnwinderPaper.Thickness;
		NameValueTable["UWTension"]			= inUnwinderPaper.UWTension;
		NameValueTable["MaxRunSpeed"]		= inUnwinderPaper.MaxRunSpeed;
	
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUwRwData::ParameterCommandResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  ���ݎ������]�[���`���Ŏ擾����
	* @retval �]�[���`���̌��ݎ���
**/
std::string CMakeComposeUwRwData::GetTimeStamp()
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);

	std::string Bias;
	if (tzInfo.Bias > 0 ) Bias = "-";
	else Bias = _T("+");

	char cTemp[512];
	_snprintf_s( cTemp, sizeof(cTemp)-1,"%04d-%02d-%02dT%02d:%02d:%02d%s%02d:%02d",
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond,
			Bias.c_str(), abs(tzInfo.Bias) / 60, abs(tzInfo.Bias) % 60);

	return std::string(cTemp);
}

/**
	* @brief  XJMF�ɐݒ肷��DeviceID���擾����
	* @retval DeviceID
**/
std::string CMakeComposeUwRwData::GetDeviceID()
{
	std::string deviceID = m_IniPrinterDescriptor.GetPrinterModelType(true, NULL);
	deviceID.append("_");
	char cTemp[10];
	_snprintf_s(cTemp, _TRUNCATE, "%0.4d", m_IniPrinterDescriptor.GetSerialNum(true));
	deviceID.append(cTemp);
	return deviceID;
};

/**
	* @brief  XJMF�̎�ʕ�������擾����
	* @param  inType : [i]XJMF�̎��
	* @retval XJMF�̎�ʕ�����
**/
std::string CMakeComposeUwRwData::GetType(int inType)
{
	switch(inType) {
	case E_Compose_QueryStatus: 	return "QueryStatus";
	case E_Compose_QueryResource:	return "QueryResource";
	case E_Compose_StopPersChParams:return "StopPersChParams";
	case E_Compose_CommandResource:	return "CommandResource";
	}
	return "";		// ���蓾�Ȃ�
}

/**
	* @brief  XJMF�ɐݒ肷�郊�s�[�g�^�C�����擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval ���s�[�g�^�C��
**/
std::string CMakeComposeUwRwData::GetRepeatTime(int inType)
{

	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�`���l���o�^
		return m_IniUwRwManager.getQueryStatus_RepeatTime();
	}
	else
	{	// �p�����ʒm�p�`���l���o�^
		return m_IniUwRwManager.getQueryResource_RepeatTime();
	}
}

/**
	* @brief  XJMF�ɐݒ肷��URL���擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval URL
**/
std::string CMakeComposeUwRwData::GetURL(int inType, const char* inDeviceName)
{
	std::string IpAddress  = m_IniPrinterDescriptor.GetClientPcIPAddr(true);

	std::string ReceiverPort = m_IniUwRwManager.getReceiver_Port(inDeviceName);
	std::string FolderName;
	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�`���l���o�^
		FolderName = m_IniUwRwManager.getStatus_Folder(inDeviceName);
	}
	else
	{	// �p�����ʒm�p�`���l���o�^
		FolderName = m_IniUwRwManager.getResource_Folder(inDeviceName);
	}

	std::string URL = "http://" + IpAddress + ":" + ReceiverPort;
	if ( !FolderName.empty() )
	{
		URL.append("/");
		URL.append(FolderName);
	}
	return URL;
}

/**
	* @brief  XJMF�ɐݒ肷��ChannelMode���擾����
	* @retval ChannelMode
**/
std::string CMakeComposeUwRwData::GetChannelMode()
{
	return m_IniUwRwManager.getQueryResource_ChannelMode();
}

/**
	* @brief  XJMF�ɐݒ肷��ResourceDetails���擾����
	* @retval ResourceDetails
**/
std::string CMakeComposeUwRwData::GetResourceDetails()
{
	return m_IniUwRwManager.getQueryResource_ResourceDetails();
}

/**
	* @brief  XJMF�ɐݒ肷��ResourceName���擾����
	* @retval ResourceName
**/
std::string CMakeComposeUwRwData::GetResourceName()
{
	return m_IniUwRwManager.getQueryResource_ResourceName();
}

/**
	* @brief  XJMF�ɐݒ肷��Scope���擾����
	* @retval Scope
**/
std::string CMakeComposeUwRwData::GetScope()
{
	return m_IniUwRwManager.getQueryResource_Scope();
}

/**
	* @brief  XJMF�ɐݒ肷��`���l���h�c���擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval �`���l���h�c
**/
std::string CMakeComposeUwRwData::GetChannelID(int inType, const char* inDeviceName)
{

	std::string CannelID;
	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�̃`���l���h�c
		CannelID = m_IniUwRwManager_work.getQueryStatus_CannelID(inDeviceName);
	}
	else
	{	// �p�����ʒm�p�̃`���l���h�c
		CannelID = m_IniUwRwManager_work.getQueryResource_CannelID(inDeviceName);
	}
	return CannelID;
}

/**
	* @brief  XJMF�ɐݒ肷��MessageType���擾����
	* @retval MessageType
**/
std::string CMakeComposeUwRwData::GetMessageType()
{
	return m_IniUwRwManager.getStopPersChParams_MessageType();
}

/**
	* @brief  XJMF�ɐݒ肷��UpdateMethod���擾����
	* @retval UpdateMethod
**/
std::string CMakeComposeUwRwData::GetUpdateMethod()
{
	return m_IniUwRwManager.getCommandResource_UpdateMethod();
}

/**
	* @brief  XJMF�ɐݒ肷��Name���擾����
	* @retval Name
**/
std::string CMakeComposeUwRwData::GetName()
{
	return m_IniUwRwManager.getCommandResource_Name();
}

/**
	* @brief  XJMF�ɐݒ肷��ID���擾����
	*         �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
	* @param  inType : [i]JMF���
	* @retval ID
**/
std::string CMakeComposeUwRwData::GetID(int inType, const char* inDeviceName)
{
	std::string NowDate = CXmlLogData::GetStrDate();		// ���ݓ����擾
	UINT nSendID = 1;

	if ( GetLastSendData(inDeviceName) == NowDate )
	{	// ���t�̕ω��Ȃ�
		nSendID = GetSendID(inType, inDeviceName);	// ���MID�ԍ��擾
		nSendID++;						// ID���C���N�������g
		SetSendID(inType, nSendID, inDeviceName);		// ���MID�ԍ��X�V
	}
	else
	{	// ���t�̕ω�����
		PutLastSendData(NowDate, inDeviceName);		// �{���̓��t��ݒ�
		ResetSendID(inDeviceName);					// �S�Ă�SendID�����Z�b�g
		nSendID = 1;					// ���MID��1����
		SetSendID(inType, nSendID, inDeviceName);		// ���MID�ԍ��X�V
	}

	// �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
	char buf[64];
	_snprintf_s( buf, sizeof(buf) - 1, "%s_%s_%05d", GetPrefix(inType).c_str(), NowDate.c_str(), nSendID);

	return std::string(buf);
}

/**
	* @brief  JMF��ʖ��̑O��v���������MID���擾����
	* @param  inType : [i]JMF���
	* @retval ���MID
**/
UINT CMakeComposeUwRwData::GetSendID(int inType, const char* inDeviceName)
{
	UINT nID;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nID = GetQueryStatusID(inDeviceName);
		break;
	case E_Compose_QueryResource:
		nID = GetQueryResourceID(inDeviceName);
		break;
	case E_Compose_StopPersChParams:
		nID = GetStopPersChParamsID(inDeviceName);
		break;
	case E_Compose_CommandResource:
		nID = GetCommandResourceID(inDeviceName);
		break;
	}
	return nID;
}

/**
	* @brief  JMF��ʖ��̑��MID��ۑ�����
	* @param  inType   : [i]JMF���
	* @param  inSendID : [i]���M�h�c
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::SetSendID(int inType, UINT inSendID, const char* inDeviceName)
{
	char buf[64];
	_snprintf_s(buf, sizeof(buf) - 1, "%d", inSendID);
	const std::string strSendID(buf);

	BOOL nRet = FALSE;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nRet = PutQueryStatusID(strSendID, inDeviceName);
		break;
	case E_Compose_QueryResource:
		nRet = PutQueryResourceID(strSendID, inDeviceName);
		break;
	case E_Compose_StopPersChParams:
		nRet = PutStopPersChParamsID(strSendID, inDeviceName);
		break;
	case E_Compose_CommandResource:
		nRet = PutCommandResourceID(strSendID, inDeviceName);
		break;
	}
	return nRet;
}

/**
	* @brief  JMF��ʖ��̑��MID�����Z�b�g����
**/
void CMakeComposeUwRwData::ResetSendID(const char* inDeviceName)
{
	PutQueryStatusID("0", inDeviceName);
	PutQueryResourceID("0", inDeviceName);
	PutStopPersChParamsID("0", inDeviceName);
	PutCommandResourceID("0", inDeviceName);
}

/**
	* @brief  JMF��ʖ���ID�̐ړ��������擾����
	* @param  inType : [i]JMF���
	* @retval �ړ�����
**/
std::string CMakeComposeUwRwData::GetPrefix(int inType)
{

	std::string Prefix;
	switch(inType) {
	case E_Compose_QueryStatus: 
		Prefix = m_IniUwRwManager.getPrefix_QueryStatus();
		break;
	case E_Compose_QueryResource:
		Prefix = m_IniUwRwManager.getPrefix_QueryResource();
		break;
	case E_Compose_StopPersChParams:
		Prefix = m_IniUwRwManager.getPrefix_StopPersChParams();
		break;
	case E_Compose_CommandResource:
		Prefix = m_IniUwRwManager.getPrefix_CommandResource();
		break;
	}
	return Prefix;
}

/**
	* @brief  �O��JMF�v���������t���擾����
	* @retval ���t
**/
std::string CMakeComposeUwRwData::GetLastSendData(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getLastSendDate(inDeviceName);
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���擾����
	* @retval ��ԊĎ��p�`���l���o�^�̑��MID
**/
UINT CMakeComposeUwRwData::GetQueryStatusID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getQueryStatusID(inDeviceName);
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�̑��MID���擾����
	* @retval �p�����ʒm�p�`���l���o�^�̑��MID
**/
UINT CMakeComposeUwRwData::GetQueryResourceID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getQueryResourceID(inDeviceName);
}

/**
	* @brief  �`���l���폜�̑��MID���擾����
	* @retval �`���l���폜�̑��MID
**/
UINT CMakeComposeUwRwData::GetStopPersChParamsID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getStopPersChParamsID(inDeviceName);
}

/**
	* @brief  ��������ݒ�̑��MID���擾����
	* @retval ��������ݒ�̑��MID
**/
UINT CMakeComposeUwRwData::GetCommandResourceID(const char* inDeviceName)
{
	return m_IniUwRwManager_work.getCommandResourceID(inDeviceName);
}

/**
	* @brief  JMF�v���������t���X�V����
	* @param  inDate : [i]���t(YYYYMMDD)
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutLastSendData(const std::string& inDate, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putLastSendDate(inDate, inDeviceName);
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���X�V����
	* @param  inID : ��ԊĎ��p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutQueryStatusID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putQueryStatusID(inID, inDeviceName);
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^���X�V����
	* @param  inID : �p�����ʒm�p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutQueryResourceID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putQueryResourceID(inID, inDeviceName);
}

/**
	* @brief  �`���l���폜�̑��MID���X�V����
	* @param  inID : �`���l���폜�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutStopPersChParamsID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putStopPersChParamsID(inID, inDeviceName);
}

/**
	* @brief  ��������ݒ�̑��MID���X�V����
	* @param  inID : ��������ݒ�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutCommandResourceID(const std::string& inID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putCommandResourceID(inID, inDeviceName);
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̃`���l��ID��INI�t�@�C���ɕۑ�����
	* @param  inCannelID : [i]�`���l���h�c
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutStatusCannelID(const std::string& inCannelID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putStatusCannelID(inCannelID, inDeviceName);
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�̃`���l��ID��INI�t�@�C���ɕۑ�����
	* @param  inCannelID : [i]�`���l���h�c
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUwRwData::PutResourceCannelID(const std::string& inCannelID, const char* inDeviceName)
{
	return m_IniUwRwManager_work.putResourceCannelID(inCannelID, inDeviceName);
}

/**
 * @brief Get ExternalID from ini
 * @return value as string
**/
std::string CMakeComposeUwRwData::GetCommandResourceExternalID()
{
	return m_IniUwRwManager.getCommandResource_ExternalID();
}

/**
 * @brief Get SEND_RETRY_COUNT value from ini
 * @return value as uint
**/
UINT CMakeComposeUwRwData::GetSendRetryCount(const char* inDeviceName)
{
	return m_IniUwRwManager.getSend_Retry_Count(inDeviceName);
}

/**
 * @brief Get SEND_RETRY_INTERVAL value from ini
 * @return value as uint
**/
UINT CMakeComposeUwRwData::GetSendRetryInterval(const char* inDeviceName)
{
	return m_IniUwRwManager.getSend_Retry_Interval(inDeviceName);
}

/**
 * Get timeout Timer Status
 * @return timeout in milliseconds
**/
UINT CMakeComposeUwRwData::GetTimeoutTimerStatus(const char* inDeviceName)
{
	return (atoi(m_IniUwRwManager.getQueryStatus_RepeatTime().c_str()) + m_IniUwRwManager.getSignalstatus_Timeout_Judegment_Waittime(inDeviceName)) * 1000;
}

/**
 * Get timeout Timer Paper
 * @return timeout in milliseconds
**/
UINT CMakeComposeUwRwData::GetTimeoutTimerPaper(const char* inDeviceName)
{
	return (atoi(m_IniUwRwManager.getQueryResource_RepeatTime().c_str()) + m_IniUwRwManager.getSignalstatus_Timeout_Judegment_Waittime(inDeviceName)) * 1000;
}

};	// namespace uwrw_manager