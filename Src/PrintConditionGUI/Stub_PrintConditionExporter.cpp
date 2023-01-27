/*! \file Stub_PrintConditionExporter.cpp
 *  \brief Stub_PrintConditionExporter.cpp�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/15 pt.ju-tanaka �V�K�쐬
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Stub_PrintConditionExporter.h"
#include "PrintConditionExporter_Callbacks.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"

// ==========================================================================
// Local definitions
// ==========================================================================
//! I/F�\���̖�
#define CBR SPrintConditionExporter_Callbacks

//! I/F�Q�b�^�[��
#define GetCallbacks PrintConditionExporter_GetCallbacks

// JI�A�����֘A�t�H���_�̃G�N�X�|�[�g�i�~���[�����O�R�s�[�j
bool Stub_PrintConditionExporter::onExportInspectionData(const std::string& inJIDataFolder)
{
	CBR cbr;
	if(!GetCallbacks(&cbr)) {
		//�v���O�C�������[�h����Ă��Ȃ��󋵂ł͐���I�������Ƃ���
		return true;
	}
	std::string pathToJIDataFolder = GetModuleDirectory() + inJIDataFolder;
	if( !PathFileExists(pathToJIDataFolder.c_str()) )	return	true;
	return	cbr.onExportInspectionData(inJIDataFolder);
}