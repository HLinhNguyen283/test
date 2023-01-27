/**
 * @file	Stub_CenterTransIF.cpp
 * @brief	stub of center trans plugin
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "CenterTrans_Callbacks.h"
#include "Stub_CenterTransIF.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SCenterTransCallbacks* outCbr);

// ==========================================================================

/**
 * @brief	I/F取得
 * @param[out]	outCbr : I/F構造体
 * @retval	true : 成功
 * @retval	false : 失敗
**/
static bool GetCallbacks(SCenterTransCallbacks* outCbr)
{
	if(!CenterTrans_GetCallback(outCbr)){
		return false;
	}
	return true;	
}

CStub_CenterTransIF::CStub_CenterTransIF()
{
}

CStub_CenterTransIF::~CStub_CenterTransIF()
{
}

size_t CStub_CenterTransIF::Register_ReceiveProc(receiveCallbackProcPtr inProc, void* inUserData)
{
	SCenterTransCallbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.m_Register_ReceiveProc(inProc, inUserData);
}

void CStub_CenterTransIF::Unregister_ReceiveProc(size_t inID)
{
	SCenterTransCallbacks cbr;
	if(!GetCallbacks(&cbr)){
		return;
	}
	return cbr.m_Unregister_ReceiveProc(inID);
}
