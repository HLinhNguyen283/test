/**
	* @file  Utility.h
	* @brief �ėp���W���[�� �w�b�_�t�@�C��
	* @author cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/

#pragma once

// ===========================================================================
// Include files
#include <string>
#include <vector>
// ===========================================================================

namespace uwrw_manager
{

/**
* @brief �ėp���W���[���N���X
*/
class CUtility
{
public:
	/* ==================================================================================================== */
	/**
		* @brief  ������u��
		* @param  IoStrData : [io]�Ώە�����
		* @param  InSrc     : [i]�ϊ��O
		* @param  InDst     : [i]�ϊ��O
		* @retval �����񒆂̕�����S�Ēu������
	**/
	/* ==================================================================================================== */
	static void replaceString( std::string& IoStrData, const std::string InSrc, const std::string InDst );

	/* ==================================================================================================== */
	/**
		* @brief  ���������蕶���ŋ�؂�
		* @param  InStrData : [i]�Ώە�����
		* @param  InDelim   : [i]��؂蕶��
		* @retval ������̃��X�g
	**/
	/* ==================================================================================================== */
	static std::vector<std::string> splitString( const std::string& InStrData, char InDelim );

	/* ==================================================================================================== */
	/**
		* @brief  YYYYMMDD�̌`���Ō��ݓ����擾����
		* @retval ���ݓ�
	**/
	/* ==================================================================================================== */
	static std::string GetStrDate();

};

};	// namespace ink_code_manager