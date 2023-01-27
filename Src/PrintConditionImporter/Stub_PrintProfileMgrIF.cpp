/**
 *	@file	Stub_PrintProfileMgrIF.cpp
 *	@brief	Stub of PrintProfileMgr_HD plugin
 *	@author	
 *	@date	2022/5/16 kmatsuoka 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Stub_PrintProfileMgrIF.h"

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SPrintProfile_Callbacks* outCbr);

// ==========================================================================

/**
 * @brief	I/F取得
 * @param[out]	outCbr : I/F構造体
 * @retval	true : 成功
 * @retval	false : 失敗
**/
static bool GetCallbacks(SPrintProfile_Callbacks* outCbr)
{
	if(!PrintProfile_GetCallbacks(outCbr)){
		return false;
	}
	return true;	
}

// ==========================================================================


// constructor
Stub_PrintProfileMgrIF::Stub_PrintProfileMgrIF()
{
}

// destructor
Stub_PrintProfileMgrIF::~Stub_PrintProfileMgrIF()
{
}

//指定印刷モードの管理を追加する
bool Stub_PrintProfileMgrIF::AddPrintMode(EPrintProfile_Kind inType, const char* inMedia)
{
	SPrintProfile_Callbacks PrintProfileCb;
	if(!GetCallbacks(&PrintProfileCb)){
		return false;
	}
	bool isAddPrintMode = PrintProfileCb.AddPrintMode(inType, inMedia);

	return isAddPrintMode;
}
