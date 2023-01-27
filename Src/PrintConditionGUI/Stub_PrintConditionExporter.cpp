/*! \file Stub_PrintConditionExporter.cpp
 *  \brief Stub_PrintConditionExporter.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/15 pt.ju-tanaka 新規作成
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
//! I/F構造体名
#define CBR SPrintConditionExporter_Callbacks

//! I/Fゲッター名
#define GetCallbacks PrintConditionExporter_GetCallbacks

// JI、検査関連フォルダのエクスポート（ミラーリングコピー）
bool Stub_PrintConditionExporter::onExportInspectionData(const std::string& inJIDataFolder)
{
	CBR cbr;
	if(!GetCallbacks(&cbr)) {
		//プラグインがロードされていない状況では正常終了扱いとする
		return true;
	}
	std::string pathToJIDataFolder = GetModuleDirectory() + inJIDataFolder;
	if( !PathFileExists(pathToJIDataFolder.c_str()) )	return	true;
	return	cbr.onExportInspectionData(inJIDataFolder);
}