/**
 * @file	PrinterIF.cpp
 * @brief	printer interface class
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "PrinterIF.h"
#include "BasePageDef.h"
#include "ProcPluginCBRProc.h"
#include "ResDef.h"
#include "CommonUIMsg.h"
#include "OutputCBR_OP.h"

// ==========================================================================
// Local definitions

// ==========================================================================

CPrinterIF::CPrinterIF()
{
	m_data = NULL;
}

CPrinterIF::~CPrinterIF()
{
}

void CPrinterIF::SetInterface(void* inData)
{
	CBaseDataIF* data = (CBaseDataIF*)inData;
	m_data = dynamic_cast<CDataIF*>(data);
}

BOOL CPrinterIF::UI_Initialize()
{
	return TRUE;
}

void CPrinterIF::UI_Finalize()
{
}

long CPrinterIF::UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DEF_NONE;
}

