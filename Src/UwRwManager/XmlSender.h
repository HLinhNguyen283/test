/**
	* @file		XmlSender.h
	* @brief	XML���O�֘A���W���[�� �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

#include <map>

namespace uwrw_manager
{

/**
* @brief UW��XML��ʒm����N���X
*/
class CXmlSender
{
public:
	/**
		* @brief  CXmlSender�R���X�g���N�^
		* @param  inXmlData : [i]�ʒm����XML�f�[�^
	**/
	CXmlSender(const std::string& inXmlData, const char* inDeviceName);

	/**
		* @brief  CXmlSender�f�X�g���N�^
	**/
	~CXmlSender();

	/**
		* @brief  XML�f�[�^�ʒm
		* @retval �������ʃf�[�^
		*		�@����̏ꍇ�F���X�|���XXML�f�[�^
		*		  �ُ�̏ꍇ�F[ERROR] �G���[���e
	**/
	BOOL Doit(std::string& result);

protected:
	/**
		* @brief  XML�f�[�^�ʒm
	**/
	BOOL Send();

	/**
		* @brief  �R�[�h�ϊ�(UTF8 �� Shift-JIS)
		* @param  InUTF8 : [i]UTF8�̕�����
		* @retval Shift-JIS�̕�����
	**/
	std::string ConvUTF8toShiftJIS(const std::string& InUTF8);

	/**
		* @brief  ���ʃf�[�^�ɃG���[����ݒ肷��
		* @param  inErrorMessage : [i]�G���[���
	**/
	void SetErrorResultData(const std::string& inErrorMessage);
	
	/**
		* @brief  UW��IP�A�h���X���擾����
		* @retval UW��IP�A�h���X
	**/
	std::string GetUwIpAddress();
	
	/**
		* @brief  UW�̎�M�t�H���_�����擾����
		* @retval UW�̎�M�t�H���_��
	**/
	std::string GetObjName();
	
	/**
		* @brief  UW�̎�M�|�[�g�ԍ����擾����
		* @retval UW�̎�M�|�[�g�ԍ�
	**/
	UINT GetSendPort();

	std::string					m_SendXmlData;		// �ʒm����XML�f�[�^
	std::string					m_ResultData;		// ��������
	const char*					m_DeviceName;		// deveice name to notify
};

};	// namespace uwrw_manager