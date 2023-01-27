/*! \file Stub_PaperDBIF.cpp
 *  \brief Stub_PaperDBIF.cppの処理
 *  \author omori
 *  \date 2019/03/04 omori 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "Stub_PaperDBIF.h"
#include "PaperDB_Callbacks.h"

// ==========================================================================
// Local definitions
//! I/F構造体名
#define CBR SPaperDB_Callbacks

//! I/Fゲッター名
#define GetCallbacks PaperDB_GetCallbacks

//! I/F実行
#define MACRO_EXEC(func){	\
	CBR cbr;	\
	if(GetCallbacks(&cbr)){	\
		func;	\
	}	\
}

// ==========================================================================


// JI検査条件を取得する
bool Stub_PaperDBIF::GetJICondition(const char* inPrintConditionName, std::string& outJICondition)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetJICondition(inPrintConditionName, outJICondition));
	return res;
}

// 画像保管を設定する
bool Stub_PaperDBIF::SetImageStorage(const std::string& inPrintConditionName, const bool inIsImageStorage)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetImageStorage(inPrintConditionName, inIsImageStorage));
	return res;
}

// 画像保管を取得する
bool Stub_PaperDBIF::IsImageStorage(const std::string& inPrintConditionName)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_IsImageStorage(inPrintConditionName));
	return res;
}

// システムの印刷条件から変倍パラメータを取得する
bool Stub_PaperDBIF::GetRipMagnification(const std::string& inPrintConditionName, ST_PDB_RIP_MAGNIFICATION& outRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetRipMagnification(inPrintConditionName, outRipMagnificationData));
	return res;
}

// システムの印刷条件に変倍パラメータを設定する
bool Stub_PaperDBIF::SetRipMagnification(const std::string& inPrintConditionName, const ST_PDB_RIP_MAGNIFICATION& inRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetRipMagnification(inPrintConditionName, inRipMagnificationData));
	return res;
}

// 紙DBをコミットする
bool Stub_PaperDBIF::CommitPaperDB()
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_CommitPaperDB());
	return res;
}

// システムの印刷条件にプライマーのON/OFFを設定する
bool Stub_PaperDBIF::SetPrimerStatus(const std::string& inPrintConditionName, const bool& inIsPrimerSetting)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_SetPrimerStatus(inPrintConditionName, inIsPrimerSetting));
	return res;
}

// システムの印刷条件からプライマーのON/OFFを取得する
bool Stub_PaperDBIF::GetPrimerStatus(const std::string& inPrintConditionName, bool& outIsPrimerSetting)
{
	bool res = false;
	MACRO_EXEC(res = cbr.PDB_GetPrimerStatus(inPrintConditionName, outIsPrimerSetting));
	return res;
}