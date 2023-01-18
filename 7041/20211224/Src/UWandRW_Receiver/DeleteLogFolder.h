/**
	* @file		DeleteLogFolder.h
	* @brief	�Â����O�t�H���_�폜 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

#pragma once

/**
* @brief �Â����O�t�H���_�폜�N���X
*/
class CWriteDebugLog;
class CDeleteLogFolder
{
	CWriteDebugLog* m_pWriteDebugLog;
public:
	/**
		* @brief  CDeleteLogFolder �R���X�g���N�V����
	**/
	CDeleteLogFolder(CWriteDebugLog* inWriteDebugLog);

	/**
		* @brief  CDeleteLogFolder �f�X�g���N�V����
	**/
	virtual ~CDeleteLogFolder();

	/**
		* @brief  �ۑ����Ԃ��o�߂������O�t�H���_���폜����
		* @param  inFolderPath	: [i]�g�b�v�t�H���_�p�X
		* @param  inPeriodDate	: [i]�ۑ�����
		* @retval TRUE=����AFALSE=�ُ�
	**/
	BOOL Doit( const CString& inFolderPath, UINT inPeriodDate );

	/**
		* @brief  �w��t�H���_���̃t�H���_�ꗗ���擾����
		* @param  inFolderPath   : [i]�w��t�H���_
		* @param  inSearchFolder : [i]�w��t�H���_�p�^�[���i��F"*"�j
		* @retval �t�H���_���̈ꗗ
	**/
	std::vector<CString> GetFolderList( const CString& inFolderPath, const CString& inSearchFolder );

	/**
		* @brief  �t�H���_���X�g�̃t�H���_���폜����
		* @param  inFolderPath		: [i]�g�b�v�t�H���_�p�X
		* @param  inDeleteFolderList: [i]�폜�t�H���_���̃��X�g
	**/
	void DeleteFoldertoList( const CString& inFolderPath, const std::vector<CString>& inDeleteFolderList );

	/**
		* @brief  �w��t�H���_���폜����
		* @param  inFolderPath : [i]�폜�t�H���_
		* @retval TRUE=����AFALSE=�ُ�
	**/
	BOOL DeleteFolderOnes( const std::string& inFolderPath );

};