/**
	* @file	GuiUnwinderManager.h
	* @brief	TP-UW�C���^�[�t�F�[�X�m�F�p�A�v�����C�� �w�b�_�[�t�@�C��
	* @details (TP��UW�̃C���^�[�t�F�[�X�m�F�p�A�v��)
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
 **/

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

// ===========================================================================
// Include files
#include "resource.h"		// ���C�� �V���{��
// ===========================================================================

/**
 * @brief TP-UW�C���^�[�t�F�[�X�m�F�p�A�v�����C��
*/
class CJD_UW_Test_TOOLApp : public CWinApp
{
public:
	CJD_UW_Test_TOOLApp();
	virtual BOOL InitInstance();

// ����
public:
	DECLARE_MESSAGE_MAP()
};

extern CJD_UW_Test_TOOLApp theApp;