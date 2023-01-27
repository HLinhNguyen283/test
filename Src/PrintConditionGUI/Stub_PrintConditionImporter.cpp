/*! \file Stub_PrintConditionImporter.cpp
 *  \brief 印刷条件インポーター用のスタブ.
 *  \author pt.ju-tanaka
 *  \date 2019/2/5 pt.ju-tanaka
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Stub_PrintConditionImporter.h"
#include "PrintConditionImporter_Callbacks.h"
#include "CommonCBRProc_OP.h"

// ==========================================================================
// Local definitions

// ==========================================================================
bool GetCallbacks(struct SPaperDB_Callbacks* outCallbacks);

// ==========================================================================
/**
 * @brief	Get the callback methods of the module.
 * @param[out]	outCallbacks : the callback methods of the module
 *  \return true：成功 / Success、false：失敗 / Failure.
**/
bool GetCallbacks(struct SPrintConditionImporter_Callbacks* outCallbacks)
{
	if (!PrintConditionImporter_GetCallbacks(outCallbacks)) {
		return false;
	}
	return true;
}

//指定された印刷条件名がインポート時のベースとなるものか判定する
bool IsBasePrintCondition(const std::string& inPrintConditionName)
{
	struct SPrintConditionImporter_Callbacks callbacks;
	if(!GetCallbacks(&callbacks)) {
		return false;
	}
	return callbacks.PrintConditionImporter_IsBasePrintCondition(inPrintConditionName);
}
