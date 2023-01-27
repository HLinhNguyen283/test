/*! \file Stub_PaperDBIF.cpp
 *  \brief Stub_PaperDBIF.cpp�̏���
 *  \author omori
 *  \date 2019/03/04 omori �V�K�쐬
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "Stub_PaperDBIF.h"
#include "PaperDB_Callbacks.h"

// ==========================================================================
// Local definitions
//! I/F�\���̖�
#define CBR SPaperDB_Callbacks

//! I/F�Q�b�^�[��
#define GetCallbacks PaperDB_GetCallbacks

//! I/F���s
#define MACRO_EXEC(func){	\
	CBR cbr;	\
	if(GetCallbacks(&cbr)){	\
		func;	\
	}	\
}

// ==========================================================================


// JI�����������擾����
bool Stub_PaperDBIF::GetJICondition(const char* inPrintConditionName, std::string& outJICondition)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetJICondition(inPrintConditionName, outJICondition));
	return res;
}

// �摜�ۊǂ�ݒ肷��
bool Stub_PaperDBIF::SetImageStorage(const std::string& inPrintConditionName, const bool inIsImageStorage)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetImageStorage(inPrintConditionName, inIsImageStorage));
	return res;
}

// �摜�ۊǂ��擾����
bool Stub_PaperDBIF::IsImageStorage(const std::string& inPrintConditionName)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_IsImageStorage(inPrintConditionName));
	return res;
}

// �V�X�e���̈����������ϔ{�p�����[�^���擾����
bool Stub_PaperDBIF::GetRipMagnification(const std::string& inPrintConditionName, ST_PDB_RIP_MAGNIFICATION& outRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetRipMagnification(inPrintConditionName, outRipMagnificationData));
	return res;
}

// �V�X�e���̈�������ɕϔ{�p�����[�^��ݒ肷��
bool Stub_PaperDBIF::SetRipMagnification(const std::string& inPrintConditionName, const ST_PDB_RIP_MAGNIFICATION& inRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetRipMagnification(inPrintConditionName, inRipMagnificationData));
	return res;
}

// ��DB���R�~�b�g����
bool Stub_PaperDBIF::CommitPaperDB()
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_CommitPaperDB());
	return res;
}

// �V�X�e���̈�������Ƀv���C�}�[��ON/OFF��ݒ肷��
bool Stub_PaperDBIF::SetPrimerStatus(const std::string& inPrintConditionName, const bool& inIsPrimerSetting)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetPrimerStatus(inPrintConditionName, inIsPrimerSetting));
	return res;
}

// �V�X�e���̈����������v���C�}�[��ON/OFF���擾����
bool Stub_PaperDBIF::GetPrimerStatus(const std::string& inPrintConditionName, bool& outIsPrimerSetting)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetPrimerStatus(inPrintConditionName, outIsPrimerSetting));
	return res;
}