/**
 * @file	PrintConditionImporter_OP.cpp
 * @brief	PrintConditionImporter_OP
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintConditionImporter_OP.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "CommonDef_OP.h"
#include "ProcPluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PrinterIF.h"
// ==========================================================================

CPrintConditionImporter_OP::CPrintConditionImporter_OP()
{
	m_dataIF = NULL;
}

CPrintConditionImporter_OP::~CPrintConditionImporter_OP()
{
	m_dataIF = NULL;
}


long CPrintConditionImporter_OP::BuildUiModuleInfo(MODULE_INFO *pModData)
{
	pModData->MODULE_ID = MOD_PRINT_CONDITION_IMPORTER;		// 情報ブロックの識別子
	pModData->MODULE_VERSION = PLUGIN_VERSION;				// 同一MODULE_IDのバージョン番号
	pModData->MODULE_TYPE = PIM_PROCESS;					// モジュールの種類。PIM_PROCESS,PIM_UI
	pModData->REQUIRE_CRITSEC = FALSE;						// 管理モジュールによる資源管理が必要な場合TRUE
	pModData->INSTALL_SERVICE = FALSE;						// サービス機能要求用。TRUE:要求,FALSE:要求しない
	pModData->SERVICE_ADDRESS = NULL;						// サービス実行用関数のアドレス
	pModData->UI_TYPE = UI_NONE;							// UI_NONE:GUIなし,それ以外はUIあり.
	return PLUGERR_NONE;
}

BOOL CPrintConditionImporter_OP::CreateDataIF()
{
	if(!m_data){
		CDataIF* data = new CDataIF();
		m_data = dynamic_cast<CBaseDataIF*>(data);
		m_dataIF = data;
	}
	return m_data != NULL;
}


BOOL CPrintConditionImporter_OP::CreatePrinterIF()
{
	if(!m_printer){
		CPrinterIF* printer = new CPrinterIF();
		m_printer = dynamic_cast<CBasePrinterIF*>(printer);
	}

	return m_printer != NULL;
}

void CPrintConditionImporter_OP::SetSystemCallBackFunc(HOST inHostInfo)
{
	SetCommonCallBackFunc_OP(inHostInfo);
}

bool CPrintConditionImporter_OP::IsBasePrintCondition(const std::string& inPrintConditionName)
{
	if(m_dataIF == NULL){
		return false;
	}

	return m_dataIF->IsBasePrintCondition(inPrintConditionName);
}
