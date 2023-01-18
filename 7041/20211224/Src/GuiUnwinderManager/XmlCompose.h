/**
	* @file		XmlCompose.h
	* @brief	XML�̍쐬 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;

#include <map>

namespace unwinder_manager
{

typedef std::map<std::string, std::string> UwXjmfDataMap;

/**
* @brief XML�̍쐬�N���X
*/
class CXmlCompose
{
public:
	/**
		* @brief  CXmlCompose�R���X�g���N�^
		* @param  inUwXjmfDataMap : [i]XML�f�[�^�ɐݒ肷����MAP
	**/
	CXmlCompose(UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlCompose�f�X�g���N�^
	**/
	~CXmlCompose();

	/**
		* @brief  XML�f�[�^�쐬
		* @retval �������ʃf�[�^
		*		�@����̏ꍇ�F�쐬����XML�f�[�^
		*		  �ُ�̏ꍇ�F[ERROR] �G���[���e
	**/
	std::string Doit();

protected:
	/**
		* @brief ���`XML�f�[�^�̃��[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL LoadXml();

	/**
		* @brief XML�h�L�������g�쐬
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL CreateXML();

	/**
		* @brief  XML����(QueryStatus)�쐬
		* @retval TRUE�F����AFALSE�F�ُ�
	**/
	BOOL CreateQueryStatus();

	/**
		* @brief  XML����(QueryResource)�쐬
		* @retval TRUE�F����AFALSE�F�ُ�
	**/
	BOOL CreateQueryResource();

	/**
		* @brief  XML����(StopPersChParams)�쐬
		* @retval TRUE�F����AFALSE�F�ُ�
	**/
	BOOL CreateStopPersChParams();

	/**
		* @brief  XML����(CommandResource)�쐬
		* @retval TRUE�F����AFALSE�F�ُ�
	**/
	BOOL CreateCommandResource();

	/**
		* @brief  XML����(CommandWakeup)�쐬
		* @retval TRUE�F����AFALSE�F�ُ�
	**/
	BOOL CreateCommandWakeup();

	/**
		* @brief XML�f�[�^�̐ݒ�
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL SaveXML();

	/**
		* @brief  ���ʃf�[�^�ɃG���[����ݒ肷��
		* @param  inErrorMessage : [i]�G���[���
	**/
	void SetErrorResultData(const std::string& inErrorMessage);

protected:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument;			// XML�h�L�������g
	MSXML2::IXMLDOMElementPtr	m_pRootElement;			// XML�̃��[�g��Element
	UwXjmfDataMap&				m_UwXjmfDataMap;		// XML�̃f�[�^MAP
	std::string					m_ResultData;			// ��������
};

};	// namespace unwinder_manager