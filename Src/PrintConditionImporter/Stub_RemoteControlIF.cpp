/**
 * @file	Stub_RemoteControlIF.cpp
 * @brief	stub of RemoteControl plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CommonCBRProc_OP.h"
#include "Stub_RemoteControlIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================


CStub_RemoteControlIF::CStub_RemoteControlIF()
{
}

CStub_RemoteControlIF::~CStub_RemoteControlIF()
{
}

void CStub_RemoteControlIF::Remote_JI_DeleteAdjustPrint(char* pPaperName, ENUM_SYSTEM_CONFIG enSystem)
{
	Remote_JetInspector_DeleteAdjustPrint(pPaperName, enSystem);
}