/**
 * @file  Common.cpp
 * @brief ���ʏ���
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
// ===========================================================================

/* ==================================================================================================== */
/**
	* @brief  ���W���[���̃J�����g�t�H���_���擾����
	* @retval ���W���[���̃J�����g�t�H���_
**/
/* ==================================================================================================== */
CString CCommon::GetCurrentDirectory()
{
	TCHAR szDirectoryName[MAX_PATH];
	::GetCurrentDirectory( sizeof(szDirectoryName)/sizeof(szDirectoryName[0]), szDirectoryName);

	return CString( szDirectoryName );
}

/* ==================================================================================================== */
/**
	* @brief  �c�[����ini�t�@�C��(JD_UW_Test_TOOL.ini)�p�X���擾
	* @retval �c�[����ini�t�@�C���p�X
**/
/* ==================================================================================================== */
CString CCommon::GetToolIniPath()
{
	CString s =  GetCurrentDirectory() + _T("\\") + TEST_TOOL_INI;
	return s;
}

/* ==================================================================================================== */
/**
	* @brief  �c�[���̃��[�Nini�t�@�C��(JD_UW_Test_WORK.ini)�p�X���擾
	*�@�@�@�@ JD_UW_Test_WORK.ini�͖{�c�[�����쐬����INI�t�@�C��
	* @retval �c�[���̃��[�Nini�t�@�C���p�X
**/
/* ==================================================================================================== */
CString CCommon::GetWorkIniPath()
{
	return GetCurrentDirectory() + _T("\\") + TEST_WORK_INI;
}

/* ==================================================================================================== */
/**
	* @brief  �w��t�@�C�����������ɓǂݍ���
	* @param  inFilePath	: [i]�t�@�C���p�X
	* @param  outData		: [o]�t�@�C���̓��e
	* @retval TRUE=����AFALSE=�ُ�
**/
/* ==================================================================================================== */
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

/* ==================================================================================================== */
/**
	* @brief  ���ݎ������]�[���`���Ŗ߂�
	* @retval �]�[���`���̌��ݎ���
**/
/* ==================================================================================================== */
CString CCommon::CreateTimeStamp()
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);

	CString Bias, TimeStamp;
	if (tzInfo.Bias > 0 ) Bias = _T("-");
	else Bias = _T("+");
	TimeStamp.Format( _T("%04d-%02d-%02dT%02d:%02d:%02d%s%02d:%02d"),
			localTime.wYear, localTime.wMonth, localTime.wDay,
			localTime.wHour, localTime.wMinute, localTime.wSecond,
			Bias.GetString(), abs(tzInfo.Bias) / 60, abs(tzInfo.Bias) % 60);

	return TimeStamp;
}

/**
	* @brief  �R�[�h�ϊ�(UTF8 �� Shift-JIS)
	* @param  InUTF8 : [i]UTF8�̕�����
	* @retval Shift-JIS�̕�����
**/
CString CCommon::ConvUTF8toShiftJIS( const CString& InUTF8)
{
	//UTF-8����UTF-16�֕ϊ�
	const int nSize = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.GetString(), -1, NULL, 0 );
	BYTE* pUtf16 = new BYTE[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)InUTF8.GetString(), -1, (LPWSTR)pUtf16, nSize );

	//UTF-16����Shift-JIS�֕ϊ�
	const int nSizeSJis = ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pUtf16, -1, NULL, 0, NULL, NULL );
	BYTE* pSJis = new BYTE[ nSizeSJis * 2 ];
	ZeroMemory( pSJis, nSizeSJis * 2 );
	::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pUtf16, -1, (LPSTR)pSJis, nSizeSJis, NULL, NULL );

	CString ShiftJIS((char*)pSJis);
	delete [] pUtf16;
	delete [] pSJis;

	return ShiftJIS;
}

/**
	* @brief  �R�[�h�ϊ�(Shift-JIS �� UTF8)
	* @param  InShiftJIS : [i]ShiftJIS�̕�����
	* @retval Shift-JIS�̕�����
**/
CString CCommon::ConvShiftJIStoUTF8( const CString& InShiftJIS)
{
	//ShiftJIS����UTF-16�֕ϊ�
	const int nSize = ::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)InShiftJIS.GetString(), -1, NULL, 0 );
 	BYTE* pUtf16 = new BYTE[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)InShiftJIS.GetString(), -1, (LPWSTR)pUtf16, nSize );
 
	//UTF-16����Shift-JIS�֕ϊ�
	const int nSizeUtf8 = ::WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)pUtf16, -1, NULL, 0, NULL, NULL );
	BYTE* pUtf8 = new BYTE[ nSizeUtf8 * 2 ];
	ZeroMemory( pUtf8, nSizeUtf8 * 2 );
	::WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)pUtf16, -1, (LPSTR)pUtf8, nSizeUtf8, NULL, NULL );


	CString Utf8((char*)pUtf8);
	delete [] pUtf16;
	delete [] pUtf8;

	return Utf8;
}

