/**
 * @file	Stub_PaperDBIF.cpp
 * @brief	stub of paper DB plugin
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <vector>
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "PaperDB_Callbacks.h"
#include "Stub_PaperDBIF.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SPaperDB_Callbacks* outCbr);

// ==========================================================================

/**
 * @brief	I/F取得
 * @param[out]	outCbr : I/F構造体
 * @retval	true : 成功
 * @retval	false : 失敗
**/
static bool GetCallbacks(SPaperDB_Callbacks* outCbr)
{
	if(!PaperDB_GetCallbacks(outCbr)){
		return false;
	}
	return true;	
}

//業務移行用データ取得
bool CStub_PaperDBIF::PDB_GetPrintConditionMigration
	(const std::string& inPrintConditionName, 
	 DEF_LENGTH_UNIT inLengthUnit, 
	 eMODE_RESO_SPEED_UNIT inSpeedUnit, 
	 ST_PDB_MIGRATION_DATA& outMigrationData)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetPrintConditionMigration(inPrintConditionName, inLengthUnit, inSpeedUnit, outMigrationData);
	return res;
}

/*!
 *  \brief 用紙種類を取得する。
 *  \param inPrintConditionName [IN]印刷条件名
 *  \param outPaperType	[OUT]用紙種類
 *  \return true：成功、false：失敗.
 */
bool CStub_PaperDBIF::PDB_GetPaperType(const char* inPrintConditionName, std::string& outPaperType)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetPaperType(inPrintConditionName, outPaperType);

	return res;
}


//インポート・エクスポート用の排他処理（読み書き禁止）を開始する
bool CStub_PaperDBIF::PDB_UniqueLock_forImportExport(NS_SRWLock::CUniqueTransaction<bool>& outLockObj)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_UniqueLock_forImportExport(outLockObj);
	return res;
}

//全レコードを呼び出し順に取得.
bool CStub_PaperDBIF::PDB_GetPrintConditionName(std::string& outPrintConditionName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetPrintConditionName(outPrintConditionName);
	return res;
}


//紙DBアクセスCS取得。
bool CStub_PaperDBIF::PDB_EnterCommonPaperDBCS()
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_EnterCommonPaperDBCS();
	return res;
}

//紙DBアクセスCS開放。
bool CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS()
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_LeaveCommonPaperDBCS();
	return res;
}
