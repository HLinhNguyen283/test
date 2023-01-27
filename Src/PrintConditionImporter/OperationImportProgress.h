/**
 * @file	OparationImportProgress.h
 * @brief	ImportProgress.txt�̑���(�쐬�E�폜��)�N���X
**/

#pragma once
//warning C4290: C++ �̗�O�̎w��͖�������܂��B�֐��� __declspec(nothrow) �łȂ����Ƃ̂ݕ\������܂��B ��}��
#pragma warning( disable : 4290 )

#include "Common.h"
#include <list>

/**
	@brief	ImportProgress.txt�̑���(�쐬�E�폜��)���s��
*/

class COperationImportProgress
{
public:
	//! ��荞�݌���
	enum E_END_RESULT
	{
		END_RESULT_SUCCESS,	//!< ����
		END_RESULT_SKIP,	//!< �X�L�b�v
		END_RESULT_ERROR	//!< ���s
	};

	/**
	 * @brief	constructor
	 * @param[in]	inOutputURL : �o�͐��URL�p�X(UNC)
	**/
	COperationImportProgress(const std::string& inOutputURL) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~COperationImportProgress() throw();

	/**
	 * @brief	�������̎�荞�݌��ʂ��o�͂���
	**/
	void WriteSuccess(const std::string& inPrintConditionName, E_END_RESULT inResult) throw();

	/**
	 * @brief	�t�@�C�����쐬����
	 * @note	�G���[�������͗�O�𓊂���
	 **/
	void createFile() throw(std::runtime_error);

	/**
		@brief	�t�@�C���폜
	*/
	bool deleteImportProggressFile();


private:

	/**
	 * @brief	�t�@�C�������
	 * @param[in]	inFileHandle : �t�@�C���n���h��
	**/
	void closeFile(HANDLE inFileHandle) throw();

	/**
	 * @brief	�t�@�C���ɏ���
	 * @param[in]	inString : ������
	 * @note	�G���[�������͗�O�𓊂���
	**/
	void writeFile(const std::string& inString) throw(std::runtime_error);

	/**
		@brief	�t�@�C���폜
	*/
	void deleteFile();

	/**
	 * @brief	�G���[�R�[�h�̕�������擾����
	 * @param[in]	inErrID : �G���[�R�[�h
	 * @retval	�G���[�R�[�h�̕�����
	**/
	const std::string getLastErrorString(DWORD inErrID) throw();


	std::string m_OutputFilePath;	//!< �o�̓t�@�C���̃t���p�X
	std::list<std::string> m_FaileList;	//!< ���ʈꗗ(���O+��荞�݌���)

};

