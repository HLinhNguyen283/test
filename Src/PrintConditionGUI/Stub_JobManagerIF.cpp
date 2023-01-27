/**
	@file	Stub_JobManagerIF.cpp
	@brief	stub of jobmanager plugin
	@author	to-hamaguchi
	@date	2017/09/13 to-hamaguchi 新規作成
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "Stub_JobManagerIF.h"
#include "JobManager_Callbacks.h"

// ==========================================================================
// Local definitions
//! I/F構造体名
#define CBR SJobManager_Callbacks

//! I/Fゲッター名
#define GetCallbacks JM_GetCallbacks

//! I/F実行
#define MACRO_EXEC(func){	\
	CBR cbr;	\
	if(GetCallbacks(&cbr)){	\
		func;	\
	}	\
}

// ==========================================================================

// constructor
Stub_JobManagerIF::Stub_JobManagerIF()
{
}

// destructor
Stub_JobManagerIF::~Stub_JobManagerIF()
{
}

// ジョブ印刷条件設定コピー依頼
void Stub_JobManagerIF::OnBeforeSaveJobPrintCondition(PrintConditionGUI::ST_COPY_TARGET_JOB_PRINT_CONDITION inCopyTargetJobPrintCondition, bool &outIsShowCopyMessage)
{
	MACRO_EXEC(cbr.JM_OnBeforeSaveJobPrintCondition(inCopyTargetJobPrintCondition, outIsShowCopyMessage));
}

// JI検査条件を設定する
bool Stub_JobManagerIF::SetJICondition(const char* inSectionID, std::string inJICondition, const bool inIsImageStorage)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_SetJICondition(inSectionID, inJICondition, inIsImageStorage));
	return res;
}

// JI検査条件を取得する
bool Stub_JobManagerIF::GetJICondition(const char* inSectionID, std::string& outJICondition, bool& outIsImageStorage)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetJICondition(inSectionID, outJICondition, outIsImageStorage));
	return res;
}

// 印刷予約ジョブかどうか取得する
bool Stub_JobManagerIF::GetReservePrintMode(const char* inSectionID, bool inIsSetupEditingData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetReservePrintMode(inSectionID, inIsSetupEditingData));
	return res;
}

// 事前RIPステータスを取得する
bool Stub_JobManagerIF::GetPreRIPStatus(const char* inSectionID, ENUM_PRE_RIP_PROGRESS& outPreRIPStatus, bool inIsSetupEditingData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetPreRIPStatus(inSectionID, outPreRIPStatus, inIsSetupEditingData));
	return res;
}

// ジョブから変倍パラメータを取得する
bool Stub_JobManagerIF::GetRipMagnification(const std::string& inSectionID, ST_JOB_RIP_MAGNIFICATION& outRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetRipMagnification(inSectionID, outRipMagnificationData));
	return res;
}
 
// ジョブの変倍パラメータを更新する
bool Stub_JobManagerIF::UpdateRipMagnification(const std::string& inSectionID, const ST_JOB_RIP_MAGNIFICATION& inRipMagnificationData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_UpdateRipMagnification(inSectionID, inRipMagnificationData));
	return res;
}

// 印刷面を取得する
bool Stub_JobManagerIF::GetSidePrint(const char* inSectionID, DEF_PRINT_SIDE& outSidePrint, bool inIsSetupEditingData)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetSidePrint(inSectionID, outSidePrint, inIsSetupEditingData));
	return res;
}

// ジョブの印刷条件設定の編集済みフラグ設定。
bool Stub_JobManagerIF::SetPrintConditionEditFlag(const char* inSectionID, BOOL inEditFlag)
{
	long res = 0;
	MACRO_EXEC(res = cbr.JM_SetPrintConditionEditFlag(inSectionID, inEditFlag));
	return res;
}

// ジョブファイル更新
long Stub_JobManagerIF::SaveJobFile(const char* inSectionID)
{
	long res = 0;
	MACRO_EXEC(res = cbr.JM_SaveJobFile(inSectionID));
	return res;
}
// プライマーの接続状態を設定する
void Stub_JobManagerIF::SetPrimerStatus(const std::string& inSectionID, bool inIsPrimer)
{
	MACRO_EXEC(cbr.JM_SetPrimerStatus(inSectionID, inIsPrimer));
}

// プライマーの接続状態を取得する
bool Stub_JobManagerIF::GetPrimerStatus(const std::string& inSectionID, bool& outIsPrimer)
{
	bool res = false;
	MACRO_EXEC(res = cbr.JM_GetPrimerStatus(inSectionID, outIsPrimer));
	return res;
}
