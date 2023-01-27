/*! \file Stub_PrintConditionImporter.h
 *  \brief 印刷条件インポーター用のスタブ.
 *  \author pt.ju-tanaka
 *  \date 2019/2/5 pt.ju-tanaka
*/

#pragma once

// ==========================================================================
// Include files

// ==========================================================================

/**
 * @brief	指定された印刷条件名がインポート時のベースとなるものか判定する
 * @param[in]	inPrintConditionName : 印刷条件名
 * @retval	true : ベースである
 * @retval	false : ベースではない
**/
bool IsBasePrintCondition(const std::string& inPrintConditionName);
