/**
 * @file	Plugin_IF.cpp
 * @brief	機種ごとの I/F
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "Plugin_IF.h"
#include "PluginCommon.h"
#include "PrintConditionImporter_OP.h"
#include "PrintConditionImporter_Callbacks.h"
#include <assert.h>

// ==========================================================================
// Local values
static CPrintConditionImporter_OP* st_PrintConditionImporterObj = NULL;

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

	st_PrintConditionImporterObj = new CPrintConditionImporter_OP();
	*moduleObject = dynamic_cast<CPlugin_Common*>(st_PrintConditionImporterObj);

	return (*moduleObject != NULL);
}


/**
 * @brief	指定された印刷条件名がインポート時のベースとなるものか判定する
 * @param[in]	inPrintConditionName : 印刷条件名
 * @retval	true : ベースである
 * @retval	false : ベースではない
**/
static bool IsBasePrintCondition(const std::string& inPrintConditionName)
{
	if(st_PrintConditionImporterObj == NULL){
		return false;
	}
	return st_PrintConditionImporterObj->IsBasePrintCondition(inPrintConditionName);
}

PLUGIN_MODULE_API bool _PrintConditionImporter_GetCallbacks(struct SPrintConditionImporter_Callbacks* outCallbacks)
{
	if(!outCallbacks){
		return false;
	}
	DWORD callBackVersion = DEF_PRINT_CONDITION_IMPORTER_CALLBACKS_VERSION_1;
	if(outCallbacks->StructVersion < callBackVersion){
		assert(outCallbacks->StructVersion >= callBackVersion);
		return false;
	}
	//Version 1
	outCallbacks->PrintConditionImporter_IsBasePrintCondition = IsBasePrintCondition;

/*
	if(outCallbacks->StructVersion >= ++callBackVersion){
		//Version 2
	}
*/

#pragma message(__MSG__ "SPrintConditionImporter_Callbacksのバージョンに合わせて、必要な関数のみセットしてください")
	return true;
}

