/**
 * @file	Stub_JetInspectorIF.cpp
 * @brief	stub of JetInspector plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CommonCBRProc_OP.h"
#include "Stub_JetInspectorIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

CStub_JetInspectorIF::CStub_JetInspectorIF()
{
}

CStub_JetInspectorIF::~CStub_JetInspectorIF()
{
}

BOOL CStub_JetInspectorIF::JI_IsJIOnline()
{
	return JetInspector_IsJIOnline();
}

void CStub_JetInspectorIF::JI_DeleteAdjustPrint(char* pPaperName, ENUM_SYSTEM_CONFIG enSystem)
{
	return JetInspector_DeleteAdjustPrint(pPaperName, enSystem);
}