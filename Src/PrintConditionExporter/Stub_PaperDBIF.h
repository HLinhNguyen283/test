/**
 * @file	Stub_PaperDBIF.h
 * @brief	stub of paper DB plugin
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "PaperDBDef_OP.h"
#include "PaperDB_MigrationDef.h"
#include "ThreadSafeShareLock.h"
#include <string>

// ==========================================================================

/**
 * @brief	stub of paper DB plugin
**/
class CStub_PaperDBIF
{
public:

	/**
	 * @brief	印刷条件の業務移行情報を取得する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inLengthUnit : 長さ単位（DEF_LENGTH_MM or DEF_LENGTH_INCH or DEF_LENGTH_POINT）
	 * @param[in]	inSpeedUnit : 速度の単位（DEF_MODE_RESO_SPEED_M or DEF_MODE_RESO_SPEED_100）
	 * @param[out]	outMigrationData : 業務移行情報
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetPrintConditionMigration(const std::string& inPrintConditionName, DEF_LENGTH_UNIT inLengthUnit, 
		eMODE_RESO_SPEED_UNIT inSpeedUnit, ST_PDB_MIGRATION_DATA& outMigrationData);

	/*!
	 *  \brief 用紙種類を取得する。
	 *  \param inPrintConditionName [IN]印刷条件名
	 *  \param outPaperType	[OUT]用紙種類
	 *  \return true：成功、false：失敗.
	 */
	static bool PDB_GetPaperType(const char* inPrintConditionName, std::string& outPaperType);

	/**
	 * @brief	インポート・エクスポート用の排他処理（読み書き禁止）を開始する
	 * @param[out]	outLockObj : 排他制御のオブジェクト
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	 * @note	上位（取得先）でこの排他制御のオブジェクトが破棄されたら自動開放
	**/
	static bool PDB_UniqueLock_forImportExport(NS_SRWLock::CUniqueTransaction<bool>& outLockObj);


	/*!
	 *  \brief 全レコードを呼び出し順に取得.
	 *         前後でCS取得、解放を行う事.
	 *  \param outPrintConditionName [OUT]取得出来た印刷条件名
	 *  \return true：成功、false：失敗
	 */
	static bool PDB_GetPrintConditionName(std::string& outPrintConditionName);

	/*!
	 *  \brief 紙DBアクセスCS取得。
	 *  \return true:成功、false:失敗
	 */
	static bool PDB_EnterCommonPaperDBCS();

	/*!
	 *  \brief 紙DBアクセスCS開放。
	 *  \return true:成功、false:失敗
	 */
	static bool PDB_LeaveCommonPaperDBCS();

};