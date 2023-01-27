/**
	* @file		XmlLogData.h
	* @brief	UW��XML��ʒm���� �w�b�_�[�t�@�C��
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
class CXmlLogData
{
public:
	/**
		* @brief  XML���M���O�t�@�C���p�X�擾
		* @retval XML���M���O�t�@�C���p�X
	**/
	static std::string GetSendLogFilePath(const char* inDeviceName);

	/**
		* @brief  XML���X�|���X���O�t�@�C���p�X�擾
		* @retval XML���X�|���X���O�t�@�C���p�X
	**/
	static std::string GetResponseLogFilePath(const char* inDeviceName);

	/**
		* @brief  �w��t�@�C�����w�胁�����ɓǂݍ���
		* @param  inFilePath	: [i]�t�@�C���p�X
		* @param  outData		: [o]�t�@�C���̓��e
		* @retval TRUE=����AFALSE=�ُ�
	**/
	static BOOL ReadFile( const std::string& inFilePath, std::string& outData );

	/**
		* @brief  �w��t�@�C���Ɏw����e����������
		* @param  inFilePath	: [i]�t�@�C���p�X
		* @param  inData		: [i]�������݃f�[�^
		* @retval TRUE=����AFALSE=�ُ�
	**/
	static BOOL WriteFile( const std::string& inFilePath, std::string& inData );

	/**
		* @brief  ���ݓ��t���擾����
		* @retval ���ݓ��t(yyyyMMdd)
	**/
	static std::string CXmlLogData::GetStrDate();

protected:
	/**
		* @brief  ���W���[���t�H���_���擾����
		* @retval ���W���[���t�H���_
	**/
	static std::string GetModuleDirectory();

	/**
		* @brief  ���ݓ���XML���O�t�H���_���擾����
		* @retval ���ݓ���XML���O�t�H���_
	**/
	static std::string GetLogFolderPath(const char* inDeviceName);

	/**
		* @brief  �w���XML���O�t�@�C���p�X���擾����
		* @param  inName : [i]�t�@�C���̐ړ�����
		* @retval �w���XML���O�t�@�C���p�X
	**/
	static std::string GetLogFilePath(const std::string& inName, const char* inDeviceName);

	/**
		* @brief  ���ݓ���(�{�~���b)���擾����
		* @retval ���ݓ���(yyyyMMddhhmmss_999)
	**/
	static std::string CXmlLogData::GetStrDateTime();
};

};	// namespace uwrw_manager