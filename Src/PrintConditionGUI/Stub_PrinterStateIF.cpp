/*! \file Stub_PrinterStateIF.cpp
 *  \brief Stub_PrinterStateIF.cpp�̏���
 *  \author omori
 *  \date 2019/03/05 omori �V�K�쐬
*/

// ==========================================================================
// Include files
// ==========================================================================
#include "stdafx.h"
#include "Stub_PrinterStateIF.h"
#include "PrinterState_Callbacks.h"
#include "CommonCBRProc_OP.h"


// ==========================================================================
// Local definitions
// ==========================================================================
//! I/F�\���̖�
#define CBR SPrinterState_Callbacks

//! I/F�Q�b�^�[��
#define GetCallbacks PrinterState_GetCallbacks


//�S�摜�ۊǁiIMS�j���L�����ǂ���
bool Stub_PrinterStateIF::IsIMSEnable()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.isIMSEnable();
}

//OCR�f�R�[�h�iOCRandDEC�j���L�����ǂ���
bool Stub_PrinterStateIF::IsOCREnable()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.isOCRandDECEnable();
}

// Ranger���ڋ@���ǂ���
bool Stub_PrinterStateIF::IsUnitOfRanger()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.PrinterState_IsUnitOfRanger();

}

// ����@�̓�������擾����
ST_PRINTER_ACTION Stub_PrinterStateIF::GetPrinterAction()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		ST_PRINTER_ACTION errPrinterAction;
		// �ُ�P�[�X�ɂ�����@���쒆�ł͖����ݒ�Ƃ���
		errPrinterAction.adjustPrint.working = false;
		return errPrinterAction;
	}
	return cb.PrinterState_GetPrinterAction();
}

// ������j�^�[���m�Y�������`�F�b�N�iRTM&NCC�j���L�����ǂ���
bool Stub_PrinterStateIF::IsRTMandNCCEnable()
{
	CBR cb;
	if(!PrinterState_GetCallbacks(&cb)) {
		return false;
	}

	return cb.isRTMandNCCEnable();
}

// ���m�N���v�����^�[���ǂ���
bool Stub_PrinterStateIF::IsMonochromePrinter()
{
	CBR cb;
	if(!PrinterState_GetCallbacks(&cb)){
		return false;
	}

	return cb.IsMonochromePrinter();
}

// �v���C�}�[�ڑ��̗L�����擾����
bool Stub_PrinterStateIF::IsConnectedPrimer()
{
	CBR cb;
	if (!PrinterState_GetCallbacks(&cb)){
		return false;
	}
	return cb.IsConnectedPrimer();
}