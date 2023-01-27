/**
 * @file	Plugin_IF.cpp
 * @brief	機種ごとの I/F
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "Plugin_IF.h"
#include "PluginCommon.h"
#include "PrintConditionExporter_OP.h"
#include "PaperDB_MigrationDef.h"
#include "PrintConditionExporter_Callbacks.h"
#include <assert.h>

// ==========================================================================
// Local values
static CPrintConditionExporter_OP* st_PrintConditionExporterObj = NULL;

// ==========================================================================
// Local definitions

// ==========================================================================
// Local functions

// ==========================================================================

BOOL CreateModuleObject(PHOST pHost, void **moduleObject)
{	
	if(!pHost || !moduleObject){
		return FALSE;
	}

	st_PrintConditionExporterObj = new CPrintConditionExporter_OP();
	*moduleObject = dynamic_cast<CPlugin_Common*>(st_PrintConditionExporterObj);

	return (*moduleObject != NULL);
}

/**
 * @brief	システムの印刷条件更新を通知する
 * @param[in]	inPrintConditionName : 印刷条件名
 * @param[in]	inMigrationData : 業務移行用構造体
**/
static void onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	if(!st_PrintConditionExporterObj){
		return;
	}
	return st_PrintConditionExporterObj->onUpdatePrintCondition(inPrintConditionName, inMigrationData);
}

/**
 * @brief	システムの印刷条件削除を通知する
 * @param[in]	inPrintConditionName : 印刷条件名
**/
static void onDeletePrintCondition(const std::string& inPrintConditionName)
{
	if(!st_PrintConditionExporterObj){
		return;
	}
	return st_PrintConditionExporterObj->onDeletePrintCondition(inPrintConditionName);
}

/**
 * @brief	JI、検査関連フォルダのエクスポート（ミラーリングコピー）
 * @param[in]	検査条件フォルダ（Clientフォルダからの相対パス）
**/
static bool onExportInspectionData(const std::string& inJIDataFolder)
{
	if(!st_PrintConditionExporterObj){
		return false;
	}
	return st_PrintConditionExporterObj->onExportInspectionData(inJIDataFolder);
}

// PrintConditionExporter 公開 callbackを取得する
PLUGIN_MODULE_API bool _PrintConditionExporter_GetCallbacks(struct SPrintConditionExporter_Callbacks*	outCallbacks)
{
	if (!outCallbacks) {
		return false;
	}
	if(outCallbacks->StructVersion < DEF_PRINT_CONDITION_EXPORTER_CALLBACKS_VERSION_1){
		assert(outCallbacks->StructVersion >= DEF_PRINT_CONDITION_EXPORTER_CALLBACKS_VERSION_1);
		return false;
	}
	//Version 1
	outCallbacks->onUpdatePrintCondition = onUpdatePrintCondition;
	outCallbacks->onDeletePrintCondition = onDeletePrintCondition;

	if(outCallbacks->StructVersion >= DEF_PRINT_CONDITION_EXPORTER_CALLBACKS_VERSION_2){
		//Version 2
		outCallbacks->onExportInspectionData = onExportInspectionData;
	}

#pragma message(__MSG__ "SPrintConditionExporter_Callbacksのバージョンに合わせて、必要な関数のみセットしてください")
	return true;
}