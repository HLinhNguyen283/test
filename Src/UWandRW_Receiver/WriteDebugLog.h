/**
	* @file		WriteDebugLog.h
	* @brief	�f�o�b�O�p���O�̏o�� �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

#define	D_DEFAULT_FOLDER_NAME	_T("LOG")

/**
 * @brief �f�o�b�O�p���O�̏o�̓N���X
*/
class CWriteDebugLog
{
public:
	enum E_LOG_LEVEL
	{ 
		E_LOG_LEVEL_1 = 1,			// ��v���O
		E_LOG_LEVEL_2,				// �֐��g���[�X���O
		E_LOG_LEVEL_3				// ����M�f�[�^���O
	};
public:
	/**
		* @brief CWriteDebugLog �R���X�g���N�V����
	**/
	CWriteDebugLog();

	/**
		* @brief CWriteDebugLog �f�X�g���N�V����
	**/
	virtual ~CWriteDebugLog();
		
	/**
		* @brief ��������
		* @param InFileName �t�@�C����
	**/
	void Initialize( const CString& inFileName );

	/**
		* @brief ���������iXML��M�p�j
		* @param InFileName �t�@�C���p�X��
	**/
	void InitializeXml( const CString& inFileName, const char* inDeviceName);

	/**
		* @brief �f�o�b�O�p���O���o��
		* @param InStrLogInfo ���O�ɏ����o��������
		* @param InDebugLevel �f�o�b�O����Level
		* @param InTime       TRUE=�����o�͂���AFALSE=�����o�͂Ȃ�
	*/ 
	void WriteDebuglog( const CString& InStrLogInfo, E_LOG_LEVEL inDebugLevel, BOOL inTime = TRUE );

private:
	CRITICAL_SECTION	m_cs;					//!< �A�v���P�[�P�[�V�������O�p�̃N���e�B�J���Z�N�V����
	int					m_Level;				//!< ���O���x��
	CString				m_strFileName;			//!< ���O�o�̓t�H���_�̓��t
	CString				m_strDate;				//!< ���O�o�̓t�H���_�̓��t
	CString				m_strOutputPath;		//!< ���O�o�̓t�@�C���p�X
};