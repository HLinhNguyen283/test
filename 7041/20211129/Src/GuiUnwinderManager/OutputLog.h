/**
 * @file  OutputLog.h
 * @brief ���O�o�̓N���X �w�b�_�[�t�@�C��
 * @date  20190610 CEC Miyachi �V�K�쐬
 **/

#pragma once
#include <fstream>

// ===========================================================================
//
// COutputLog �N���X
//
// ===========================================================================
/**
 * @brief ���O�o�̓N���X
*/
class COutputLog
{
public:
	COutputLog();
	~COutputLog();

//--------------------------------------
// �����o�֐�
//--------------------------------------
public:
	void Start( std::string strFileName );
	void Output( const std::string& strLog, bool IsOutputTime = true );
	void Output( const std::wstring& wstrLog, bool IsOutputTime=true );
protected:
private:
	std::string GetTime();
	std::string GetThreadId();
	std::string GetModuleDirectory();

//--------------------------------------
// �����o�ϐ�
//--------------------------------------
public:
protected:
private:
	CRITICAL_SECTION	m_cs;					//!< �A�v���P�[�P�[�V�������O�p�̃N���e�B�J���Z�N�V����
	CRITICAL_SECTION	m_cs_http;				//!< http���O�p�̃N���e�B�J���Z�N�V����

	std::string			m_strPath;				//!< �A�v���P�[�P�[�V�������O�p�̃t�@�C���p�X
	std::string			m_strFileName;			//!< �A�v���P�[�P�[�V�������O�p�̃t�@�C������

	std::ofstream		m_ofs;					//!< �A�v���P�[�P�[�V�������O�p�̃t�@�C���X�g���[��
};
