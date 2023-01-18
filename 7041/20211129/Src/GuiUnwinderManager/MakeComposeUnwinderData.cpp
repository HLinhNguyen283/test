/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml�N���p�����[�^�쐬
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <map>
#include "Utility.h"
#include "Ini_UnwinderManager.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_UnwinderManager_work.h"
#include "MakeComposeUnwinderData.h"
// ===========================================================================

namespace unwinder_manager
{

CMakeComposeUnwinderData::CMakeComposeUnwinderData()
{
}
CMakeComposeUnwinderData::~CMakeComposeUnwinderData()
{
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�iQueryStatus�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::ParameterQueryStatus()
{
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable["Type"]				= GetType(E_Compose_QueryStatus);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_QueryStatus);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["RepeatTime"]		= GetRepeatTime(E_Compose_QueryStatus);
		NameValueTable["URL"]				= GetURL(E_Compose_QueryStatus);

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ParameterQueryStatus");
		return "";
	}
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^�iQueryResource�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::ParameterQueryResource()
{
	try
	{
		std::map<std::string, std::string> NameValueTable;
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

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ParameterQueryStatus");
		return "";
	}
}

/**
	* @brief  �`���l���폜�iStopPersChParams�j�̑��M�f�[�^���쐬����
	* @param  InType : [i]�폜�`���l�����(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::ParameterStopPersChParams(int InType)
{
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable["Type"]				= GetType(E_Compose_StopPersChParams);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_StopPersChParams);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["ChannelID"]			= GetChannelID(InType);
		NameValueTable["MessageType"]		= GetMessageType();
		NameValueTable["URL"]				= GetURL(InType);

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ParameterStopPersChParams");
		return "";
	}
}

/**
	* @brief  ��������ݒ�iCommandResource�j�̑��M�f�[�^���쐬����
	* @param  InUnwinderPaper : [i]����������
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::ParameterCommandResource(const CUnwinderPaper& InUnwinderPaper)
{
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable["Type"]				= GetType(E_Compose_CommandResource);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandResource);
		NameValueTable["Time"]				= GetTimeStamp();
		NameValueTable["UpdateMethod"]		= GetUpdateMethod();
		NameValueTable["Name"]				= GetName();
		NameValueTable["DescriptiveName"]	= InUnwinderPaper.DescriptiveName;
		NameValueTable["ExternalID"]		= InUnwinderPaper.ExternalID;
		NameValueTable["Dimension"]			= InUnwinderPaper.Dimension;
		NameValueTable["MediaType"]			= InUnwinderPaper.MediaType;
		NameValueTable["Thickness"]			= InUnwinderPaper.Thickness;
		NameValueTable["UWTension"]			= InUnwinderPaper.UWTension;
		NameValueTable["MaxRunSpeed"]		= InUnwinderPaper.MaxRunSpeed;
	
		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ParameterCommandResource");
		return "";
	}
}

/**
	* @brief  READY��Ԃւ̑J�ځiCommandWakeup�j�̑��M�f�[�^���쐬����
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::ParameterCommandWakeup()
{
	try
	{
		std::map<std::string, std::string> NameValueTable;
		NameValueTable["Type"]				= GetType(E_Compose_CommandWakeup);
		NameValueTable["DeviceID"]			= GetDeviceID();
		NameValueTable["ID"]				= GetID(E_Compose_CommandWakeup);
		NameValueTable["Time"]				= GetTimeStamp();

		return MakeString(NameValueTable);
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[Exception] ParameterCommandWakeup");
		return "";
	}
}

/**
	* @brief  ����MAP���XML���M�f�[�^���쐬����
	* @param  InNameValueTable : [i]���̂ƒl���y�A�Ƃ����e�[�u��
	* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
**/
std::string CMakeComposeUnwinderData::MakeString(const std::map<std::string,std::string>& InNameValueTable)
{
	std::string strData;
	bool bFst = true;
	for( auto ite = InNameValueTable.begin(); ite != InNameValueTable.end(); ite++ )
	{
		if ( !bFst ) strData.append( " " );
		strData.append( ite->first );
		strData.append( "=" );

		std::string s = ite->second;
		CUtility::replaceString(s," ", "@@@");		// �u�����N��@@@�ɕϊ�����
		strData.append( s );
		bFst = false;
	}
	return strData;
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
	* @brief  XJMF�̎�ʂ��擾����
	* @retval XJMF�̎��
**/
std::string CMakeComposeUnwinderData::GetType(int nType)
{
	switch(nType) {
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
	* @param  InType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval ���s�[�g�^�C��
**/
std::string CMakeComposeUnwinderData::GetRepeatTime(int nType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	if ( nType == E_Compose_QueryStatus )
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
	* @param  InType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval URL
**/
std::string CMakeComposeUnwinderData::GetURL(int nType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string IpAddress  = iniUnwinderManager.getUW_IpAddress();
	std::string SenderPort = iniUnwinderManager.getSender_Port();
	std::string FolderName;
	if ( nType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�`���l���o�^
		FolderName = iniUnwinderManager.getStatus_Folder();
	}
	else
	{	// �p�����ʒm�p�`���l���o�^
		FolderName = iniUnwinderManager.getResource_Folder();
	}

	std::string URL = "http://" + IpAddress + ":" + SenderPort;
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
	* @param  InType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
	* @retval �`���l���h�c
**/
std::string CMakeComposeUnwinderData::GetChannelID(int nType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string CannelID;
	if ( nType == E_Compose_QueryStatus )
	{	// ��ԊĎ��p�̃`���l���h�c
		CannelID = iniUnwinderManager.getQueryStatus_CannelID();
	}
	else
	{	// �p�����ʒm�p�̃`���l���h�c
		CannelID = iniUnwinderManager.getQueryResource_CannelID();
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
	* @param  InType : [i]JMF���
	* @retval ID
**/
std::string CMakeComposeUnwinderData::GetID(int nType)
{
	std::string NowDate = CUtility::GetStrDate();		// ���ݓ����擾
	UINT nSendID = 1;

	if ( GetLastSendData() == NowDate )
	{	// ���t�̕ω��Ȃ�
		nSendID = GetSendID(nType);		// ���MID�ԍ��擾
		nSendID++;						// ID���C���N�������g
		SetSendID(nType, nSendID);		// ���MID�ԍ��X�V
	}
	else
	{	// ���t�̕ω�����
		PutLastSendData(NowDate);		// �{���̓��t��ݒ�
		ResetSendID();					// �S�Ă�SendID�����Z�b�g
		nSendID = 1;					// ���MID��1����
		SetSendID(nType, nSendID);	// ���MID�ԍ��X�V
	}

	// �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
	char buf[64];
	_snprintf_s( buf, sizeof(buf) - 1, "%s_%s_%05d", GetPrefix(nType).c_str(), NowDate.c_str(), nSendID);

	return std::string(buf);
}

/**
	* @brief  JMF��ʖ��̑O��v���������MID���擾����
	* @param  InType : [i]JMF���
	* @retval ���MID
**/
UINT CMakeComposeUnwinderData::GetSendID(int nType)
{
	UINT nID;
	switch(nType) {
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
	* @param  InType : [i]JMF���
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::SetSendID(int nType, UINT nSendID)
{
	char buf[64];
	_snprintf_s(buf, sizeof(buf) - 1, "%d", nSendID);
	const std::string strSendID(buf);

	BOOL nRet = FALSE;
	switch(nType) {
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
	PutQueryStatusID(0);
	PutQueryResourceID(0);
	PutStopPersChParamsID(0);
	PutCommandResourceID(0);
	PutCommandWakeupID(0);
}

/**
	* @brief  JMF��ʖ���ID�̐ړ��������擾����
	* @param  InType : [i]JMF���
	* @retval �ړ�����
**/
std::string CMakeComposeUnwinderData::GetPrefix(int nType)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	std::string Prefix;
	switch(nType) {
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
	* @param  InDate : [i]���t(YYYYMMDD)
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutLastSendData(const std::string& InDate)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putLastSendDate(InDate);
}

/**
	* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���X�V����
	* @param  InID : ��ԊĎ��p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutQueryStatusID(const std::string& InID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryStatusID(InID);
}

/**
	* @brief  �p�����ʒm�p�`���l���o�^���X�V����
	* @param  InID : �p�����ʒm�p�`���l���o�^�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutQueryResourceID(const std::string& InID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putQueryResourceID(InID);
}

/**
	* @brief  �`���l���폜�̑��MID���X�V����
	* @param  InID : �`���l���폜�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutStopPersChParamsID(const std::string& InID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putStopPersChParamsID(InID);
}

/**
	* @brief  ��������ݒ�̑��MID���X�V����
	* @param  InID : ��������ݒ�̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutCommandResourceID(const std::string& InID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandResourceID(InID);
}

/**
	* @brief  READY��Ԃւ̑J�ڂ̑��MID���X�V����
	* @param  InID : READY��Ԃւ̑J�ڂ̑��MID
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CMakeComposeUnwinderData::PutCommandWakeupID(const std::string& InID)
{
	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	return iniUnwinderManager_work.putCommandWakeupID(InID);
}

};	// namespace ink_code_manager