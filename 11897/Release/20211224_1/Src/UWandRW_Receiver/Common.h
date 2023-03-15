/**
	* @file		Common.h
	* @brief	���ʒ�` �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
 **/

#pragma once

/**
* @brief ����API�N���X
*/
class CCommon
{
public:
	enum
	{	// �X���b�h�ԗv��
		WM_USER_SIGNAL_STATUS = (WM_USER+100),	// SignalStatus��Pipe�o�͗v��
	};

	/**
		* @brief  ���W���[���t�H���_���擾����
		* @retval ���W���[���t�H���_
	**/
	static CString GetModuleDirectory();

	/**
		* @brief  �w��t�@�C�����������ɓǂݍ���
		* @param  inFilePath	: [i]�t�@�C���p�X
		* @param  outData		: [o]�t�@�C���̓��e
		* @retval TRUE=����AFALSE=�ُ�
	**/
	static BOOL ReadFile( const CString& inFilePath, CString& outData );

};