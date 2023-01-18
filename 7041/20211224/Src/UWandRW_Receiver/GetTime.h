/**
	* @file		GetTime.h
	* @brief	���ݓ����擾 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/
#pragma once

/**
 * @brief ���ݓ����擾�N���X
*/
class CGetTime
{
public:
	/**
		* @brief  yyyyMMddhhmmss_999 �̌`���Ō��ݓ������擾����
		* @retval ���ݓ����iyyyyMMddhhmmss_999�j
	**/
	static CString GetStrTimeMM();

	/**
		* @brief  yyyy/MM/dd hh:mm:ss �̌`���Ō��ݓ������擾����
		* @retval ���ݓ����iyyyy/MM/dd hh:mm:ss�j
	**/
	static CString GetStrTimeSlCo();

	/**
		* @brief  yyyyMMdd �̌`���Ō��ݓ��t���擾����
		* @retval ���ݓ����iyyyyMMdd�j
	**/
	static CString GetStrDate();
};