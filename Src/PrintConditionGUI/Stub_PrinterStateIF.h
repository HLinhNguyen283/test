/*! \file Stub_PrinterStateIF.h
 *  \brief Stub_PrinterStateIF.h�̏���
 *  \author omori
 *  \date 2019/03/05 omori �V�K�쐬
*/

#pragma once
#include "CommonCBRProc.h"
#include "PrinterStateDef_OP.h"

class Stub_PrinterStateIF
{

public:

	/**
	 * @brief	�R���X�g���N�^
	**/
	Stub_PrinterStateIF();

	/**
	 * @brief	�f�X�g���N�^
	**/
	~Stub_PrinterStateIF();


	/**
	@brief	�S�摜�ۊǁiIMS�j���L�����ǂ���
	@retval	true : �L��
	@retval	false : ����
	*/
	static bool IsIMSEnable();

	/**
	@brief	OCR�f�R�[�h�iOCRandDEC�j���L�����ǂ���
	@retval	true : �L��
	@retval	false : ����
	*/
	static bool IsOCREnable();

	/*!
	*  \brief Ranger���ڗL�����擾�ifalse:�����ځAtrue:���ځj
	*  \return false:�����ځAtrue:����
	*/
	static bool IsUnitOfRanger();

	/**
	 *	@brief ����@�̓�������擾����
	 *	@retval	����@�̓�����
	**/
	static ST_PRINTER_ACTION GetPrinterAction();

	/**
		@brief	������j�^�[���m�Y�������`�F�b�N�iRTM&NCC�j���L�����ǂ���
		@retval	true : �L��
		@retval	false : ����
	*/
	static bool IsRTMandNCCEnable();

	/*!
		@brief ���m�N���v�����^�[���ǂ���
		@retval	true : ���m�N���@
		@retval	false : �J���[�@
	*/
	static bool IsMonochromePrinter();

	/**
		@brief	�v���C�}�[�ڑ��̗L�����擾����
		@retval	true : �v���C�}�[�ڑ��L��
		@retval	false : �v���C�}�[�ڑ�����
	*/
	static bool IsConnectedPrimer();
};