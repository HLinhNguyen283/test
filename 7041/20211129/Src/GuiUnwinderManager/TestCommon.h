/**
 * @file  TestCommon.h
 * @brief ���ʒ�` �w�b�_�[�t�@�C��
 * @author screen
 * @date  20211119 screen �V�K�쐬
 **/

#pragma once


enum JmfType
{	// JMF���
	WM_USER_JMF_QueryStatus = (WM_USER+100),	// ��ԊĎ�
	WM_USER_JMF_QueryResource,					// �p�����o�^
	WM_USER_JMF_StopPersChParams,				// �`���l���폜
	WM_USER_JMF_CommandResource,				// ��������ݒ�
	WM_USER_JMF_CommandWakeup,					// READY��Ԃւ̑J�ڗv��

	WM_USER_JMF_QueryStatus_END,				// ��ԊĎ������ʒm
	WM_USER_JMF_QueryResource_END,				// �p�����o�^�����ʒm
	WM_USER_JMF_StopPersChParams_END,			// �`���l���폜�����ʒm
	WM_USER_JMF_CommandResource_END,			// ��������ݒ芮���ʒm
	WM_USER_JMF_CommandWakeup_END,				// READY��Ԃւ̑J�ڗv�������ʒm

	WM_USER_DSP_SEND,							// ���M�f�[�^�\���ʒm
	WM_USER_DSP_RECV,							// ��M�f�[�^�\���ʒm
};

//! ���O�o�͌`��
enum {
	LOG_STATUS = 0,		//!< �ʏ�̋L�^
	LOG_ERROR,			//!< �G���[���
	LOG_WARNING,		//!< �x�����
	LOG_DEBUG,			//!< �f�o�b�O���
	LOG_HTML,			//!< HTML ���ڏo��
	LOG_OPERATION		//!< �I�y���[�V�������
};

void WriteToLogBuf(long type, char* pString);
