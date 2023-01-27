/*! \file Stub_PrintConditionExporter.h
 *  \brief Stub_PrintConditionExporter.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/15 pt.ju-tanaka 新規作成
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
	 * @brief	コンストラクタ
	**/
	Stub_PrintConditionExporter();

	/**
	 * @brief	デストラクタ
	**/
	~Stub_PrintConditionExporter();

	/**
	 * @brief	JI、検査関連フォルダのエクスポート（ミラーリングコピー）
	 * @param[in]	検査条件フォルダ（Clientフォルダからの相対パス）
	**/
	static bool onExportInspectionData(const std::string& inJIDataFolder);
};