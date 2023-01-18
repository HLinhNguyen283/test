/**
 * @file  JmfData.h
 * @brief JMF�̐��`�f�[�^ �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/
#pragma once

/* ==================================================================================================== */
/**
	* @brief  ���M�f�[�^�̐��`���擾����
	* @param  nJmfType	: [i]JMF���
	* @retval  ���M�f�[�^�̐��`
**/
/* ==================================================================================================== */
CString GetJmfData(UINT nJmfType);

/* ==================================================================================================== */
/**
	* @brief  ���M���X�|���X�f�[�^�̐��`���擾����
	* @param  nJmfType	: [i]JMF���
	* @retval  ���M���X�|���X�f�[�^�̐��`
**/
/* ==================================================================================================== */
CString GetJmfResponseData(UINT nJmfType);

extern TCHAR Jmf_QueryStatus[];				// ��ԊĎ�
extern TCHAR Jmf_QueryResource[];			// �p�����o�^
extern TCHAR Jmf_StopPersChParams[];		// �`���l���폜
extern TCHAR Jmf_CommandResource[];			// ��������ݒ�
extern TCHAR Jmf_CommandWakeup[];			// READY��Ԃւ̑J�ڗv��

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����Ό��p
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern TCHAR Jmf_SignalStatus [];			// UW�����SignalStatus(�f�[�^�Œ�)
extern TCHAR Jmf_SignalStatusPaper[];
extern TCHAR Jmf_QueryStatusRes[];			// ��ԊĎ��̃��X�|���X(�f�[�^�Œ�)
extern TCHAR Jmf_QueryResourceRes[];		// �p�����o�^�̃��X�|���X(�f�[�^�Œ�)
extern TCHAR Jmf_StopPersChParamsRes[];		// �`���l���폜�̃��X�|���X(�f�[�^�Œ�)
extern TCHAR Jmf_CommandResourceRes[];		// ��������ݒ�̃��X�|���X(�f�[�^�Œ�)
extern TCHAR Jmf_CommandWakeupRes[];		// READY��Ԃւ̑J�ڗv���̃��X�|���X(�f�[�^�Œ�)

