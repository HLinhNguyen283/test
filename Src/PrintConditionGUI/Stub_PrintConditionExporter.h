/*! \file Stub_PrintConditionExporter.h
 *  \brief Stub_PrintConditionExporter.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/02/15 pt.ju-tanaka �V�K�쐬
*/

#pragma once

// ==========================================================================
// Include files
#include <string>

// ==========================================================================

class Stub_PrintConditionExporter
{
public:
	/**
	 * @brief	�R���X�g���N�^
	**/
	Stub_PrintConditionExporter();

	/**
	 * @brief	�f�X�g���N�^
	**/
	~Stub_PrintConditionExporter();

	/**
	 * @brief	JI�A�����֘A�t�H���_�̃G�N�X�|�[�g�i�~���[�����O�R�s�[�j
	 * @param[in]	���������t�H���_�iClient�t�H���_����̑��΃p�X�j
	**/
	static bool onExportInspectionData(const std::string& inJIDataFolder);
};