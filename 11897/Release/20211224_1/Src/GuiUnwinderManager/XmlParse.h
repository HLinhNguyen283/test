/**
	* @file		XmlParse.h
	* @brief	XML�̉�� �w�b�_�[�t�@�C��
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
* @brief XML�̉�� Base�N���X
*/
class CXmlParse
{
	enum JmfType
	{	// ���X�|���XJMF���
		E_JMF_Unknown,						// �s��
		E_JMF_ResponseStatus,				// �`���l���o�^�̃��X�|���X
		E_JMF_Response,						// �`���l���폜�A�p�����o�^�Ȃǂ̃��X�|���X
		E_JMF_SignalStatus,					// UW�����SignalStatus
	};

public:
	/**
		* @brief  CXmlParse�R���X�g���N�^
		* @param  outUwXjmfDataMap : [o]XML�f�[�^������o�������
	**/
	CXmlParse(UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParse�f�X�g���N�^
	**/
	virtual ~CXmlParse();

protected:
	/**
		* @brief  XML�f�[�^�̉��
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL Analys();

	/**
		* @brief  XML�̎�ʎ擾
		* @param  outNode : [o]Xml�f�[�^�̃m�[�h
		* @retval XML�̎��
	**/
	JmfType GetXmlType(MSXML2::IXMLDOMNode** outNode);

	/**
		* @brief  XML�f�[�^(ResponseStatus)�̉��
		*		  ��������(ReturnCode)�ƃ`���l��ID(refID)�����o��
		* @param  inNode : [i]Xml�f�[�^�̃m�[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL ParseResponseStatus(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  XML�f�[�^(Response)�̉��
		*		  ��������(ReturnCode)�����o��
		* @param  inNode : [i]Xml�f�[�^�̃m�[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL ParseResponse(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  XML�f�[�^(SignalStatus)�̉��
		*		  Ststus�ʒm�̏ꍇ�FUW�̏��(Status)�����o��
		*		  �p�����ʒm�̏ꍇ�FDescriptiveName�ADimension�AMediaType�ARollDiameter�AThickness�����o��
		* @param  inNode : [i]Xml�f�[�^�̃m�[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL ParseSignalStatus(MSXML2::IXMLDOMNodePtr inNode);

	/**
		* @brief  ���ʃf�[�^�ɃG���[����ݒ肷��
		* @param  inErrorMessage : [i]�G���[���
	**/
	void SetErrorResultData(const std::string& inErrorMessage);

	/**
		* @brief  ���ʃf�[�^�ɐ���I������ݒ肷��
	**/
	void SetSuccessResultData();

protected:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument;			// XML�h�L�������g
	MSXML2::IXMLDOMElementPtr	m_pRootElement;			// XML�̃��[�g��Element
	UwXjmfDataMap&				m_UwXjmfDataMap;		// XML�̃f�[�^MAP
	std::string					m_ResultData;			// ��������
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief XML�t�@�C���̉�� �N���X
*/
class CXmlParseFile : public CXmlParse
{
public:
	/**
		* @brief  CXmlParseFile�R���X�g���N�^(CXmlParse�̔h���N���X)
		* @param  inXmlFilePath    : [i]XML�t�@�C���̃p�X
		* @param  outUwXjmfDataMap : [o]XML�t�@�C��������o�������
	**/
	CXmlParseFile(const std::string& inXmlFilePath, UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParseFile�f�X�g���N�^
	**/
	virtual ~CXmlParseFile();

	/**
		* @brief  XML�t�@�C���̃p�[�X
		* @retval �p�[�X���ʃf�[�^
	**/
	std::string Doit();

protected:
	/**
		* @brief XML�t�@�C���̃��[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL LoadXml();

private:
	std::string					m_XmlFilePath;			// XML�t�@�C���̃p�X
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief XML�f�[�^�̉�� �N���X
*/
class CXmlParseData : public CXmlParse
{
public:
	/**
		* @brief  CXmlParseData�R���X�g���N�^(CXmlParse�̔h���N���X)
		* @param  inXmlData        : [i]XML�f�[�^
		* @param  outUwXjmfDataMap : [o]XML�f�[�^������o�������
	**/
	CXmlParseData(const std::string& inXmlData, UwXjmfDataMap& inUwXjmfDataMap);

	/**
		* @brief  CXmlParseData�f�X�g���N�^
	**/
	virtual ~CXmlParseData();

	/**
		* @brief  XML�f�[�^�̃p�[�X
		* @retval �p�[�X���ʃf�[�^
	**/
	std::string Doit();

protected:
	/**
		* @brief XML�f�[�^�̃��[�h
		* @retval TRUE:����AFALSE:�ُ�
	**/
	BOOL LoadXml();

private:
	std::string					m_XmlData;				// XML�f�[�^
};

};	// namespace unwinder_manager