/**
	* @file  Utility.cpp
	* @brief �ėp���W���[��
	* @author cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include "Utility.h"
// ===========================================================================

namespace unwinder_manager
{

/**
	* @brief  ������u��
	* @param  IoStrData : [io]�Ώە�����
	* @param  InSrc     : [i]�ϊ��O
	* @param  InDst     : [i]�ϊ��O
	* @retval �����񒆂̕�����S�Ēu������
**/
void CUtility::replaceString( std::string& IoStrData, const std::string InSrc, const std::string InDst )
{

	bool Loop = true;
	std::string::size_type n = 0;
	while( Loop )
	{
		n = IoStrData.find(InSrc, n);
		if (n != IoStrData.npos)
		{
			//�u�������񌟏o
			IoStrData.replace( n, InSrc.size(), InDst) ;
			n+=2;
		} else {
			Loop = false;
		}
	}
}

/**
	* @brief  ���������蕶���ŋ�؂�
	* @param  InStrData : [i]�Ώە�����
	* @param  InDelim   : [i]��؂蕶��
	* @retval ������̃��X�g
**/
std::vector<std::string> CUtility::splitString( const std::string& InStrData, char InDelim )
{
    std::istringstream iss(InStrData);
    std::string StrTemp;
    std::vector<std::string> strList;
    while (std::getline(iss, StrTemp, InDelim))
	{
		strList.push_back(StrTemp);
	}
    return strList;
}

/**
	* @brief  YYYYMMDD�̌`���Ō��ݓ����擾����
	* @retval ���ݓ�
**/
std::string CUtility::GetStrDate()
{
	// ���݂̎������擾
	SYSTEMTIME st;
	GetLocalTime(&st);

	// �������𕶎���ɕϊ�=>20090727
	char buf[64];
	_snprintf_s( buf, sizeof(buf) - 1, "%04d%02d%02d", (int)st.wYear, (int)st.wMonth, (int)st.wDay );

	return std::string(buf);
}


};	// namespace ink_code_manager