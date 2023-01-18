/**
	* @file		Common.cpp
	* @brief	���ʏ���
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
 **/

// ===========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
// ===========================================================================

/**
	* @brief  ���W���[���t�H���_���擾����
	* @retval ���W���[���t�H���_
**/
CString CCommon::GetModuleDirectory()
{
	TCHAR szFullPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	std::string s(szFullPath);
	size_t pos = s.rfind("\\");

	return s.substr(0, pos).c_str();
}

/**
	* @brief  �w��t�@�C�����������ɓǂݍ���
	* @param  inFilePath	: [i]�t�@�C���p�X
	* @param  outData		: [o]�t�@�C���̓��e
	* @retval TRUE=����AFALSE=�ُ�
**/
BOOL CCommon::ReadFile( const CString& inFilePath, CString& outData )
{
	BOOL bRet = FALSE;
	TCHAR *pcBuffer = NULL;

	HANDLE hFile = CreateFile( inFilePath, FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
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
				outData.Append( pcBuffer );
				bRet = TRUE;
			}
		}
	}
	if ( pcBuffer )
	{
		// �������J��
		delete [] pcBuffer;
	}
	CloseHandle( hFile );

	return bRet;
}
