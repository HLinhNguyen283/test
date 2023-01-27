/**
 * @file	PrintConditionExporter_OP.cpp
 * @brief	PrintConditionExporter_OP
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintConditionExporter_OP.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "CommonDef_OP.h"
#include "ProcPluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PrinterIF.h"
// ==========================================================================

CPrintConditionExporter_OP::CPrintConditionExporter_OP()
{
	m_dataIF = NULL;
}

CPrintConditionExporter_OP::~CPrintConditionExporter_OP()
{
	m_dataIF = NULL;
}


long CPrintConditionExporter_OP::BuildUiModuleInfo(MODULE_INFO *pModData)
{
	pModData->MODULE_ID = MOD_PRINT_CONDITION_EXPORTER;		// 情報ブロックの識別子
	pModData->MODULE_VERSION = PLUGIN_VERSION;				// 同一MODULE_IDのバージョン番号
	pModData->MODULE_TYPE = PIM_PROCESS;					// モジュールの種類。PIM_INPUT,PIM_PROCESS,PIM_OUTPUT,PIM_UI
	pModData->REQUIRE_CRITSEC = FALSE;						// 管理モジュールによる資源管理が必要な場合TRUE
	pModData->INSTALL_SERVICE = FALSE;						// サービス機能要求用。TRUE:要求,FALSE:要求しない
	pModData->SERVICE_ADDRESS = NULL;						// サービス実行用関数のアドレス
	pModData->UI_TYPE = UI_NONE;							// UI_NONE:GUIなし,それ以外はUIあり.

	return PLUGERR_NONE;
}

BOOL CPrintConditionExporter_OP::CreateDataIF()
{
	if(!m_data){
		CDataIF* data = new CDataIF();
		m_data = dynamic_cast<CBaseDataIF*>(data);
		m_dataIF = data;
	}
	return m_data != NULL;
}


BOOL CPrintConditionExporter_OP::CreatePrinterIF()
{
	if(!m_printer){
		CPrinterIF* printer = new CPrinterIF();
		m_printer = dynamic_cast<CBasePrinterIF*>(printer);
	}

	return m_printer != NULL;
}

void CPrintConditionExporter_OP::SetSystemCallBackFunc(HOST inHostInfo)
{
	SetCommonCallBackFunc_OP(inHostInfo);
}

// システムの印刷条件更新を通知する
void CPrintConditionExporter_OP::onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	if(!m_data){
		return;
	}
	CDataIF* dataif = dynamic_cast<CDataIF*>(m_data);
	dataif->onUpdatePrintCondition(inPrintConditionName, inMigrationData);
}

// システムの印刷条件削除を通知する
void CPrintConditionExporter_OP::onDeletePrintCondition(const std::string& inPrintConditionName)
{
	if(!m_data){
		return;
	}
	CDataIF* dataif = dynamic_cast<CDataIF*>(m_data);
	dataif->onDeletePrintCondition(inPrintConditionName);
}

// JI、検査関連フォルダのエクスポート（ミラーリングコピー）
bool CPrintConditionExporter_OP::onExportInspectionData(const std::string& inJIDataFolder)
{
	if(!m_data){
		return false;
	}
	CDataIF* dataif = dynamic_cast<CDataIF*>(m_data);
	return dataif->onExportInspectionData(inJIDataFolder);
}
