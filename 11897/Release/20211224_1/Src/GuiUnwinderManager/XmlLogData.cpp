/**
	* @file		XmlLogData.cpp
	* @brief	XML���O�֘A���W���[��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "XmlLogData.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  XML���M���O�t�@�C���p�X�擾
	* @retval XML���M���O�t�@�C���p�X
**/
std::string CXmlLogData::GetSendLogFilePath()
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	std::string LogFolderPath = GetLogFolderPath();
	if ( !::PathFileExists( LogFolderPath.c_str() ) )
	{	// ���݂��Ȃ��ꍇ�̓t�H���_���쐬����
		::SHCreateDirectory( NULL, A2W(LogFolderPath.c_str()) );
	}
	std::string LogFilePath = GetLogFilePath("Send");
	if ( ::PathFileExists( LogFilePath.c_str() ) )
	{	// ���݂���ꍇ�͂P�~���҂��čēx�t�@�C���p�X���쐬����
		Sleep(1);
		LogFilePath = GetLogFilePath("Send");
	}
	return LogFilePath;
}

/**
	* @brief  XML���X�|���X���O�t�@�C���p�X�擾
	* @retval XML���X�|���X���O�t�@�C���p�X
**/
std::string CXmlLogData::GetResponseLogFilePath()
{
	USES_CONVERSION;				// A2W�g�p�̏ꍇ�̐錾

	std::string LogFolderPath = GetLogFolderPath();
	if ( !::PathFileExists( LogFolderPath.c_str() ) )
	{	// ���݂��Ȃ��ꍇ�̓t�H���_���쐬����
		::SHCreateDirectory( NULL, A2W(LogFolderPath.c_str()) );
	}
	std::string LogFilePath = GetLogFilePath("Response");
	if ( ::PathFileExists( LogFilePath.c_str() ) )
	{	// ���݂���ꍇ�͂P�~���҂��čēx�t�@�C���p�X���쐬����
		Sleep(1);
		LogFilePath = GetLogFilePath("Send");
	}
	return LogFilePath;
}

/**
	* @brief  �w��t�@�C�����w�胁�����ɓǂݍ���
	* @param  inFilePath	: [i]�t�@�C���p�X
	* @param  outData		: [o]�t�@�C���̓��e
	* @retval TRUE=����AFALSE=�ُ�
**/
BOOL CXmlLogData::ReadFile( const std::string& inFilePath, std::string& outData )
{
	BOOL nRet = FALSE;
	char *pcBuffer = NULL;

	HANDLE hFile = CreateFile( inFilePath.c_str(), FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwSize = GetFileSize( hFile, NULL );	//���DWORD�͍l�����Ȃ�
		if ( dwSize != -1 )
		{
			//�t�@�C����ǂݍ��ރo�b�t�@���m��
			pcBuffer = new TCHAR[dwSize + 1];
			ZeroMemory( pcBuffer, dwSize + 1 );
			//�t�@�C���ǂݍ���
			DWORD dwRead, dwTotal = 0;
			while ( dwTotal != dwSize )
			{
				BOOL bRet = ::ReadFile( hFile, &pcBuffer[dwTotal], dwSize, &dwRead, NULL );
				if ( bRet == 0 )
				{	// �ǂݍ��ݏI��
					break;
				}
				dwTotal += dwRead;	// �ǂݎ��o�C�g���v�����Z
			}
			if ( dwTotal == dwSize )
			{
				// �ǂݍ��ݐ���I��
				nRet = TRUE;
			}
		}
	}
	if ( pcBuffer )
	{
		outData.append(pcBuffer);
		// �������J��
		delete [] pcBuffer;
	}
	CloseHandle( hFile );

	return nRet;
}

/**
	* @brief  �w��t�@�C���Ɏw����e����������
	* @param  inFilePath	: [i]�t�@�C���p�X
	* @param  inData		: [i]�������݃f�[�^
	* @retval TRUE=����AFALSE=�ُ�
**/
BOOL CXmlLogData::WriteFile( const std::string& inFilePath, std::string& inData )
{
	BOOL nRet = FALSE;

	HANDLE hFile = CreateFile( inFilePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwWrittenSize;
		if ( ::WriteFile( hFile, inData.c_str() , (DWORD)inData.length(), &dwWrittenSize, NULL ) )
		{	// �������ݐ���
			nRet = TRUE;
		}
		CloseHandle( hFile );
	}

	return nRet;
}

/**
	* @brief  ���W���[���t�H���_���擾����
	* @retval ���W���[���t�H���_
**/
std::string CXmlLogData::GetModuleDirectory()
{
	TCHAR szFullPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	std::string s(szFullPath);
	size_t pos = s.rfind("\\");

	return s.substr(0, pos);
}

/**
	* @brief  ���ݓ���XML���O�t�H���_���擾����
	* @retval ���ݓ���XML���O�t�H���_
**/
std::string CXmlLogData::GetLogFolderPath()
{
	std::string LogFolderPath = GetModuleDirectory();
	LogFolderPath.append("\\LOG\\UW_CONNECT\\");
	LogFolderPath.append(GetStrDate());
	LogFolderPath.append(".xml");
	return LogFolderPath;
}

/**
	* @brief  �w���XML���O�t�@�C���p�X���擾����
	* @param  inName : [i]�t�@�C���̐ړ�����
	* @retval �w���XML���O�t�@�C���p�X
**/
std::string CXmlLogData::GetLogFilePath(const std::string& inName)
{
	std::string LogFilePath = GetLogFolderPath();
	LogFilePath.append("\\");
	LogFilePath.append(inName);
	LogFilePath.append("_");
	LogFilePath.append(GetStrDateTime());
	LogFilePath.append(".xml");
	return LogFilePath;
}

/**
	* @brief  ���ݓ��t���擾����
	* @retval ���ݓ��t(yyyyMMdd)
**/
std::string CXmlLogData::GetStrDate()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// ���t���𕶎���ɕϊ�
	char buf[64];
	_snprintf_s( buf, sizeof(buf) -1 , "%04d%02d%02d", st.wYear, st.wMonth, st.wDay );

	return std::string(buf);
}

/**
	* @brief  ���ݓ���(�{�~���b)���擾����
	* @retval ���ݓ���(yyyyMMddhhmmss_999)
**/
std::string CXmlLogData::GetStrDateTime()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// ���t�Ǝ������𕶎���ɕϊ�
	char buf[64];
	_snprintf_s( buf, sizeof(buf) -1 , "%04d%02d%02d%02d%02d%02d_%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

	return std::string(buf);
}

};	// namespace unwinder_manager