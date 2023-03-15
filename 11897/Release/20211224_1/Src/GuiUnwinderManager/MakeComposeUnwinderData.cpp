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
#include "Ini_UnwinderManager.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_UnwinderManager_work.h"
#include "XmlLogData.h"
#include "MakeComposeUnwinderData.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  CMakeComposeUnwinderData�R���X�g���N�^
**/
CMakeComposeUnwinderData::CMakeComposeUnwinderData()
{
}

/**
	* @brief  CMakeComposeUnwinderData�f�X�g���N�^
**/
CMakeComposeUnwinderData::~CMakeComposeUnwinderData()
{
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�iQueryStatus�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^MAP
**/
UwXjmfDataMap CMakeComposeUnwinderData::ParameterQueryStatus()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] Start");
	UwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryStatus);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryStatus);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryStatus);
		NameValueTable["URL"]				= GetURL(E_Compose_QueryStatus);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryStatus] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�iQueryResource�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^MAP
**/
UwXjmfDataMap CMakeComposeUnwinderData::ParameterQueryResource()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] Start");
	UwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_QueryResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryResource);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryResource);
		NameValueTable["ChannelMode"]		= GetChannelMode();
		NameValueTable["URL"]				= GetURL(E_Compose_QueryResource);
		NameValueTable["ResourceDetails"]	= GetResourceDetails();
		NameValueTable["ResourceName"]		= GetResourceName();
		NameValueTable["Scope"]				= GetScope();

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterQueryResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  �`���l���폜�iStopPersChParams�j�̑��M�f�[�^���쐬����
	* @param  inType : [i]�폜�`���l�����(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval XML���M�f�[�^MAP
**/
UwXjmfDataMap CMakeComposeUnwinderData::ParameterStopPersChParams(int inType)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] Start");
	UwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_StopPersChParams);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_StopPersChParams);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["ChannelID"]			= GetChannelID(inType);
		NameValueTable["MessageType"]		= GetMessageType();
		NameValueTable["URL"]				= GetURL(inType);

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterStopPersChParams] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  ��������ݒ�iCommandResource�j�̑��M�f�[�^���쐬����
	* @param  inUnwinderPaper : [i]����������
	* @retval XML���M�f�[�^MAP
**/
UwXjmfDataMap CMakeComposeUnwinderData::ParameterCommandResource(const CUnwinderPaper& inUnwinderPaper)
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] Start");
	UwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_CommandResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandResource);
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
	
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandResource] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  READY��Ԃւ̑J�ځiCommandWakeup�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^MAP
**/
UwXjmfDataMap CMakeComposeUnwinderData::ParameterCommandWakeup()
{
	WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] Start");
	UwXjmfDataMap NameValueTable;
	NameValueTable.clear();
	try
	{
		NameValueTable["Type"]				= GetType(E_Compose_CommandWakeup);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandWakeup);
		NameValueTable["Time"]				= GetTimeStamp();

		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] End");
		return NameValueTable;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CMakeComposeUnwinderData::ParameterCommandWakeup] Exception");
		return NameValueTable;
	}
}

/**
	* @brief  ���ݎ������]�[���`���Ŏ擾����
	* @retval �]�[���`���̌��ݎ���
**/
std::string CMakeComposeUnwinderData::GetTimeStamp()
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
std::string CMakeComposeUnwinderData::GetDeviceID()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	return iniPrinterDescriptor.getDeviceID();
};

/**
	* @brief  XJMF�̎�ʕ�������擾����
	* @param  inType : [i]XJMF�̎��
	* @retval XJMF�̎�ʕ�����
**/
std::string CMakeComposeUnwinderData::GetType(int inType)
{
	switch(inType) {
	case E_Compose_QueryStatus: 	return "QueryStatus";
	case E_Compose_QueryResource:	return "QueryResource";
	case E_Compose_StopPersChParams:return "StopPersChParams";
	case E_Compose_CommandResource:	return "CommandResource";
	case E_Compose_CommandWakeup:	return "CommandWakeup";
	}
	return "";		// ���蓾�Ȃ�
}

/**
	* @brief  XJMF�ɐݒ肷�郊�s�[�g�^�C�����擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval ���s�[�g�^�C��
**/
std::string CMakeComposeUnwinderData::GetRepeatTime(int inType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�`���l���o�^
		return iniUnwinderManager.getQueryStatus_RepeatTime();
	}
	else
	{	// �p�����ʒm�p�`���l���o�^
		return iniUnwinderManager.getQueryResource_RepeatTime();
	}
}

/**
	* @brief  XJMF�ɐݒ肷��URL���擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval URL
**/
std::string CMakeComposeUnwinderData::GetURL(int inType)
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	std::string IpAddress  = iniPrinterDescriptor.getMyIpAddress();

	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string ReceiverPort = iniUnwinderManager.getReceiver_Port();
	std::string FolderName;
	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�`���l���o�^
		FolderName = iniUnwinderManager.getStatus_Folder();
	}
	else
	{	// �p�����ʒm�p�`���l���o�^
		FolderName = iniUnwinderManager.getResource_Folder();
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
std::string CMakeComposeUnwinderData::GetChannelMode()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ChannelMode();
}

/**
	* @brief  XJMF�ɐݒ肷��ResourceDetails���擾����
	* @retval ResourceDetails
**/
std::string CMakeComposeUnwinderData::GetResourceDetails()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ResourceDetails();
}

/**
	* @brief  XJMF�ɐݒ肷��ResourceName���擾����
	* @retval ResourceName
**/
std::string CMakeComposeUnwinderData::GetResourceName()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_ResourceName();
}

/**
	* @brief  XJMF�ɐݒ肷��Scope���擾����
	* @retval Scope
**/
std::string CMakeComposeUnwinderData::GetScope()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getQueryResource_Scope();
}

/**
	* @brief  XJMF�ɐݒ肷��`���l���h�c���擾����
	* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval �`���l���h�c
**/
std::string CMakeComposeUnwinderData::GetChannelID(int inType)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);

	std::string CannelID;
	if ( inType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�̃`���l���h�c
		CannelID = iniUnwinderManager_work.getQueryStatus_CannelID();
	}
	else
	{	// �p�����ʒm�p�̃`���l���h�c
		CannelID = iniUnwinderManager_work.getQueryResource_CannelID();
	}
	return CannelID;
}

/**
	* @brief  XJMF�ɐݒ肷��MessageType���擾����
	* @retval MessageType
**/
std::string CMakeComposeUnwinderData::GetMessageType()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getStopPersChParams_MessageType();
}

/**
	* @brief  XJMF�ɐݒ肷��UpdateMethod���擾����
	* @retval UpdateMethod
**/
std::string CMakeComposeUnwinderData::GetUpdateMethod()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getCommandResource_UpdateMethod();
}

/**
	* @brief  XJMF�ɐݒ肷��Name���擾����
	* @retval Name
**/
std::string CMakeComposeUnwinderData::GetName()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	return iniUnwinderManager.getCommandResource_Name();
}

/**
	* @brief  XJMF�ɐݒ肷��ID���擾����
	*         �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
	* @param  inType : [i]JMF���
	* @retval ID
**/
std::string CMakeComposeUnwinderData::GetID(int inType)
{
	std::string NowDate = CXmlLogData::GetStrDate();		// ���ݓ����擾
	UINT nSendID = 1;

	if ( GetLastSendData() == NowDate )
	{	// ���t�̕ω��Ȃ�
		nSendID = GetSendID(inType);	// ���MID�ԍ��擾
		nSendID++;						// ID���C���N�������g
		SetSendID(inType, nSendID);		// ���MID�ԍ��X�V
	}
	else
	{	// ���t�̕ω�����
		PutLastSendData(NowDate);		// �{���̓��t��ݒ�
		ResetSendID();					// �S�Ă�SendID�����Z�b�g
		nSendID = 1;					// ���MID��1����
		SetSendID(inType, nSendID);		// ���MID�ԍ��X�V
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
UINT CMakeComposeUnwinderData::GetSendID(int inType)
{
	UINT nID;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nID = GetQueryStatusID();
		break;
	case E_Compose_QueryResource:
		nID = GetQueryResourceID();
		break;
	case E_Compose_StopPersChParams:
		nID = GetStopPersChParamsID();
		break;
	case E_Compose_CommandResource:
		nID = GetCommandResourceID();
		break;
	case E_Compose_CommandWakeup:
		nID = GetCommandWakeupID();
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
BOOL CMakeComposeUnwinderData::SetSendID(int inType, UINT inSendID)
{
	char buf[64];
	_snprintf_s(buf, sizeof(buf) - 1, "%d", inSendID);
	const std::string strSendID(buf);

	BOOL nRet = FALSE;
	switch(inType) {
	case E_Compose_QueryStatus: 
		nRet = PutQueryStatusID(strSendID);
		break;
	case E_Compose_QueryResource:
		nRet = PutQueryResourceID(strSendID);
		break;
	case E_Compose_StopPersChParams:
		nRet = PutStopPersChParamsID(strSendID);
		break;
	case E_Compose_CommandResource:
		nRet = PutCommandResourceID(strSendID);
		break;
	case E_Compose_CommandWakeup:
		nRet = PutCommandWakeupID(strSendID);
		break;
	}
	return nRet;
}

/**
	* @brief  JMF��ʖ��̑��MID�����Z�b�g����
**/
void CMakeComposeUnwinderData::ResetSendID()
{
	PutQueryStatusID("0");
	PutQueryResourceID("0");
	PutStopPersChParamsID("0");
	PutCommandResourceID("0");
	PutCommandWakeupID("0");
}

/**
	* @brief  JMF��ʖ���ID�̐ړ��������擾����
	* @param  inType : [i]JMF���
	* @retval �ړ�����
**/
std::string CMakeComposeUnwinderData::GetPrefix(int inType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string Prefix;
	switch(inType) {
	case E_Compose_QueryStatus: 
		Prefix = iniUnwinderManager.getPrefix_QueryStatus();
		break;
	case E_Compose_QueryResource:
		Prefix = iniUnwinderManager.getPrefix_QueryResource();
		break;
	case E_Compose_StopPersChParams:
		Prefix = iniUnwinderManager.getPrefix_StopPersChParams();
		break;
	case E_Compose_CommandResource:
		Prefix = iniUnwinderManager.getPrefix_CommandResource();
		break;
	case E_Compose_CommandWakeup:
		Prefix = iniUnwinderManager.getPrefix_CommandWakeup();
		break;
	}
	return Prefix;
}

/**
	* @brief  �O��JMF�v���������t���擾����
	* @retval ���t
**/
std::string CMakeComposeUnwinderData::GetLastSendData()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getLastSendDate();
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���擾����
	* @retval ��ԊĎ��p�`���l���o�^�̑��MID
**/
UINT CMakeComposeUnwinderData::GetQueryStatusID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getQueryStatusID();
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�̑��MID���擾����
	* @retval �p�����ʒm�p�`���l���o�^�̑��MID
**/
UINT CMakeComposeUnwinderData::GetQueryResourceID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getQueryResourceID();
}

/**
	* @brief  �`���l���폜�̑��MID���擾����
	* @retval �`���l���폜�̑��MID
**/
UINT CMakeComposeUnwinderData::GetStopPersChParamsID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getStopPersChParamsID();
}

/**
	* @brief  ��������ݒ�̑��MID���擾����
	* @retval ��������ݒ�̑��MID
**/
UINT CMakeComposeUnwinderData::GetCommandResourceID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getCommandResourceID();
}

/**
	* @brief  READY��Ԃւ̑J�ڂ̑��MID���擾����
	* @retval READY��Ԃւ̑J�ڂ̑��MID
**/
UINT CMakeComposeUnwinderData::GetCommandWakeupID()
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.getCommandWakeupID();
}

/**
	* @brief  JMF�v���������t���X�V����
	* @param  inDate : [i]���t(YYYYMMDD)
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutLastSendData(const std::string& inDate)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putLastSendDate(inDate);
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���X�V����
	* @param  inID : ��ԊĎ��p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutQueryStatusID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryStatusID(inID);
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^���X�V����
	* @param  inID : �p�����ʒm�p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutQueryResourceID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryResourceID(inID);
}

/**
	* @brief  �`���l���폜�̑��MID���X�V����
	* @param  inID : �`���l���폜�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutStopPersChParamsID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putStopPersChParamsID(inID);
}

/**
	* @brief  ��������ݒ�̑��MID���X�V����
	* @param  inID : ��������ݒ�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutCommandResourceID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandResourceID(inID);
}

/**
	* @brief  READY��Ԃւ̑J�ڂ̑��MID���X�V����
	* @param  inID : READY��Ԃւ̑J�ڂ̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutCommandWakeupID(const std::string& inID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandWakeupID(inID);
}

};	// namespace unwinder_manager