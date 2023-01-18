/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml�N���p�����[�^�쐬 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/

#pragma once

#include <map>

namespace unwinder_manager
{

//
// ����������
//
class CUnwinderPaper
{
public:
	std::string DescriptiveName;		// ���������
	std::string Dimension;				// �p�����Ǝc��
	std::string MediaType;				// ���f�B�A��
	std::string Thickness;				// ����
	std::string UWTension;				// �e���V����
	std::string MaxRunSpeed;			// ����ō����x
	std::string ExternalID;				// �O��ID
};

/**
* @brief UWandRW_Compose_Xml�N���p�����[�^�쐬�N���X
*/
class CMakeComposeUnwinderData
{
public:
	CMakeComposeUnwinderData();
	~CMakeComposeUnwinderData();
	//
	// UW�ւ�XJMF�v��
	//
	enum TestNo
	{ 
		E_Compose_QueryStatus,				// ��ԊĎ��p�`���l���o�^
		E_Compose_QueryResource,			// �p�����ʒm�p�`���l���o�^
		E_Compose_StopPersChParams,			// �`���l���폜
		E_Compose_CommandResource,			// ��������ݒ�
		E_Compose_CommandWakeup,			// READY��Ԃւ̑J��
	};

	//
	// UWandRW_Compose_Xml�N���p�����[�^�쐬
	//

	/**
		* @brief  ��ԊĎ��p�`���l���o�^�iQueryStatus�j�̑��M�f�[�^���쐬����
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string ParameterQueryStatus();

	/**
		* @brief  �p�����ʒm�p�`���l���o�^�iQueryResource�j�̑��M�f�[�^���쐬����
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string ParameterQueryResource();

	/**
		* @brief  �`���l���폜�iStopPersChParams�j�̑��M�f�[�^���쐬����
		* @param  inType : [i]�폜�`���l�����(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string ParameterStopPersChParams(int inType);

	/**
		* @brief  ��������ݒ�iCommandResource�j�̑��M�f�[�^���쐬����
		* @param  inUnwinderPaper : [i]����������
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string ParameterCommandResource(const CUnwinderPaper& inUnwinderPaper);

	/**
		* @brief  READY��Ԃւ̑J�ځiCommandWakeup�j�̑��M�f�[�^���쐬����
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string ParameterCommandWakeup();

private:
	/**
		* @brief  ����MAP���XML���M�f�[�^���쐬����
		* @param  inNameValueTable : [i]���̂ƒl���y�A�Ƃ����e�[�u��
		* @retval XML���M�f�[�^�i���ږ��P=�l�P�����ږ��Q=�l�Q..�j
	**/
	std::string MakeString(const std::map<std::string, std::string>& inNameValueTable);

	/**
		* @brief  ���ݎ������]�[���`���Ŏ擾����
		* @retval �]�[���`���̌��ݎ���
	**/
	std::string GetTimeStamp();

	/**
		* @brief  XJMF�ɐݒ肷��DeviceID���擾����
		* @retval DeviceID
	**/
	std::string GetDeviceID();

	/**
		* @brief  XJMF�̎�ʕ�������擾����
		* @param  inType : [i]XJMF�̎��
		* @retval XJMF�̎�ʕ�����
	**/
	std::string GetType(int inType);

	/**
		* @brief  XJMF�ɐݒ肷�郊�s�[�g�^�C�����擾����
		* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval ���s�[�g�^�C��
	**/
	std::string GetRepeatTime(int inType);

	/**
		* @brief  XJMF�ɐݒ肷��URL���擾����
		* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval URL
	**/
	std::string GetURL(int inType);

	/**
		* @brief  XJMF�ɐݒ肷��ChannelMode���擾����
		* @retval ChannelMode
	**/
	std::string GetChannelMode();

	/**
		* @brief  XJMF�ɐݒ肷��ResourceDetails���擾����
		* @retval ResourceDetails
	**/
	std::string GetResourceDetails();

	/**
		* @brief  XJMF�ɐݒ肷��ResourceName���擾����
		* @retval ResourceName
	**/
	std::string GetResourceName();

	/**
		* @brief  XJMF�ɐݒ肷��Scope���擾����
		* @retval Scope
	**/
	std::string GetScope();

	/**
		* @brief  XJMF�ɐݒ肷��`���l���h�c���擾����
		* @param  inType : [i]JMF���(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval �`���l���h�c
	**/
	std::string GetChannelID(int inType);

	/**
		* @brief  XJMF�ɐݒ肷��MessageType���擾����
		* @retval MessageType
	**/
	std::string GetMessageType();

	/**
		* @brief  XJMF�ɐݒ肷��UpdateMethod���擾����
		* @retval UpdateMethod
	**/
	std::string GetUpdateMethod();

	/**
		* @brief  XJMF�ɐݒ肷��Name���擾����
		* @retval Name
	**/
	std::string GetName();

	/**
		* @brief  XJMF�ɐݒ肷��ID���擾����
		*         �h�c�́A�ړ�����_���t(YYYYMMDD)_���P�ʁEJMF�P�ʂ̘A��(1�`n)
		* @param  inType : [i]JMF���
		* @retval ID
	**/
	std::string GetID(int inType);

	/**
		* @brief  JMF��ʖ��̑O��v���������MID���擾����
		* @param  inType : [i]JMF���
		* @retval ���MID
	**/
	UINT GetSendID(int inType);

	/**
		* @brief  JMF��ʖ��̑��MID��ۑ�����
		* @param  inType   : [i]JMF���
		* @param  inSendID : [i]���M�h�c

		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL SetSendID(int inType, UINT inSendID);

	/**
		* @brief  JMF��ʖ��̑��MID�����Z�b�g����
	**/
	void ResetSendID();

	/**
		* @brief  JMF��ʖ���ID�̐ړ��������擾����
		* @param  inType : [i]JMF���
		* @retval �ړ�����
	**/
	std::string GetPrefix(int inType);

	/**
		* @brief  �O��JMF�v���������t���擾����
		* @retval ���t
	**/
	std::string GetLastSendData();

	/**
		* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���擾����
		* @retval ��ԊĎ��p�`���l���o�^�̑��MID
	**/
	UINT GetQueryStatusID();

	/**
		* @brief  �p�����ʒm�p�`���l���o�^�̑��MID���擾����
		* @retval �p�����ʒm�p�`���l���o�^�̑��MID
	**/
	UINT GetQueryResourceID();

	/**
		* @brief  �`���l���폜�̑��MID���擾����
		* @retval �`���l���폜�̑��MID
	**/
	UINT GetStopPersChParamsID();

	/**
		* @brief  ��������ݒ�̑��MID���擾����
		* @retval ��������ݒ�̑��MID
	**/
	UINT GetCommandResourceID();

	/**
		* @brief  READY��Ԃւ̑J�ڂ̑��MID���擾����
		* @retval READY��Ԃւ̑J�ڂ̑��MID
	**/
	UINT GetCommandWakeupID();

	/**
		* @brief  JMF�v���������t���X�V����
		* @param  inDate : [i]���t(YYYYMMDD)
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutLastSendData(const std::string& inDate);

	/**
		* @brief  ��ԊĎ��p�`���l���o�^�̑��MID���X�V����
		* @param  inID : ��ԊĎ��p�`���l���o�^�̑��MID
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutQueryStatusID(const std::string& inID);

	/**
		* @brief  �p�����ʒm�p�`���l���o�^���X�V����
		* @param  inID : �p�����ʒm�p�`���l���o�^�̑��MID
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutQueryResourceID(const std::string& inID);

	/**
		* @brief  �`���l���폜�̑��MID���X�V����
		* @param  inID : �`���l���폜�̑��MID
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutStopPersChParamsID(const std::string& inID);

	/**
		* @brief  ��������ݒ�̑��MID���X�V����
		* @param  inID : ��������ݒ�̑��MID
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutCommandResourceID(const std::string& inID);

	/**
		* @brief  READY��Ԃւ̑J�ڂ̑��MID���X�V����
		* @param  inID : READY��Ԃւ̑J�ڂ̑��MID
		* @retval TRUE�F����
		* @retval FALSE�F�ُ�
	**/
	BOOL PutCommandWakeupID(const std::string& inID);
};

};	// namespace ink_code_manager