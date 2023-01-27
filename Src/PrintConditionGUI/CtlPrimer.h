/**
 * @file	CtlPrimer.h
 * @brief	�v���C�}�[�̃R���g���[��
**/

#pragma once

// ==========================================================================
// Include files
#include "BaseCtl.h"

// ==========================================================================

/**
 * @brief	����Ŏg�p����`�F�b�N�{�b�N�X�\��
**/
class CCtlPrimer : public CBaseCtl
{
public:
	//�萔

	//�֐�

	/**
		@brief	�R���X�g���N�^
	**/
	CCtlPrimer();

	/**
		@brief	�f�X�g���N�^
	**/
	virtual ~CCtlPrimer();

	//�C�x���g
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();

	/**
		@brief �R���g���[������C�x���g
		@param[in] hWnd : �E�B���h�E�n���h��
		@param[in] essage : ���b�Z�[�W�ԍ�
		@param[in] wParam : �p�����[�^
		@param[in] lParam : �p�����[�^
		@retval	DEF_NONE:�ʒm�Ȃ�, DEF_EXIT_PAGE:��ʂ����
	**/
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	/**
		@brief	�R���g���[���\���l�X�V�C�x���g
	**/
	virtual void OnUpdateState();

	/**
		@brief	�R���g���[���X�e�[�^�X�C�x���g
	**/
	virtual void OnUpdateValue();

protected:
	//�֐�
	//�C�x���g

	/**
		@brief	�R���g���[�������ݒ�C�x���g
	**/
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//���i

	//�ϐ�

private:
	//�ϐ�
	
};
