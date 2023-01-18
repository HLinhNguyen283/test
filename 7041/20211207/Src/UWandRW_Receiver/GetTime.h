/**
	* @file		GetTime.h
	* @brief	���ݓ����擾 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/
#pragma once

/**
 * @brief ���ݓ����擾�N���X
*/
class CGetTime
{
	public:
		/**
		* @brief 2009:12:21:04:45:47 �̌`���Ō��݂̓������擾����
		* @return �u2009:12:21:04:45:47�v�`���̕�����
		*/
		static CString GetStrTimeCo();

		/**
		* @brief 20091221044547 �̌`���Ō��݂̓������擾����
		* @return �u20091221044547�v�`���̕�����
		*/
		static CString GetStrTime();

		/**
		* @brief 20091221044547_999 �̌`���Ō��݂̓������擾����
		* @return �u20091221044547_999�v�`���̕�����
		*/
		static CString GetStrTimeMM();

		/**
		* @brief 20091221044547 �̌`���Ō��݂̓������擾����
		* @return �u20091221044547�v�`����int�^�̐��l
		*/
		static int GetIntTime();
		
		/**
		* @brief 2009/12/21/04/45/47 �̌`���Ō��݂̓������擾����
		* @return �u2009/12/21/04/45/47�v�`���̕�����
		*/
		static CString GetStrTimeSla();

		/**
		* @brief 2009/12/21 04:45:47 �̌`���Ō��݂̓������擾����
		* @return �u2009/12/21 04:45:47�v�`���̕�����
		*/
		static CString GetStrTimeSlCo();

		/**
		* @brief 2009:12:21 �̌`���Ō��݂̓��t���擾����
		* @return �u2009:12:21�v�`���̕�����
		*/
		static CString GetStrDateCo();

		/**
		* @brief 20091221 �̌`���Ō��݂̓��t���擾����
		* @return �u20091221�v�`���̕�����
		*/
		static CString GetStrDate();

		/**
		* @brief 20091221 �̌`���Ō��݂̓��t���擾����
		* @return �u20091221�v�`����int�^�̐��l
		*/
		static int GetIntDate();
		
		/**
		* @brief 2009/12/21 �̌`���Ō��݂̓��t���擾����
		* @return �u2009/12/21�v�`���̕�����
		*/
		static CString GetStrDateSla();
};