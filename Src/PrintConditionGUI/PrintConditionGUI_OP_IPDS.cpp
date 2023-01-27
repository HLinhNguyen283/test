/*! \file	PrintConditionGUI_OP_IPDS.cpp
 *  \brief	PrintConditionGUI_OP
 *  \author	omoto
 *  \date	2017/02/22 omoto new
*/

// ==========================================================================
// Include files
// ==========================================================================

#include "stdafx.h"
#include "PrintConditionGUI_OP_IPDS.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "CommonCBRProc_OP_IPDS.h"
#include "CommonDef_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "PageMain.h"
#include "DataIF.h"
#include "PrinterIF.h"
#include "DlgPageMain.h"

// ==========================================================================

/*! \brief	constructor
*/
CPrintConditionGUI_OP_IPDS::CPrintConditionGUI_OP_IPDS()
{
}

/*! \brief	destructor
*/
CPrintConditionGUI_OP_IPDS::~CPrintConditionGUI_OP_IPDS()
{
}

/*! \brief		システム毎のコールバック関数を設定する
 *  \param[in]	inHostInfo ホスト情報
 */
void CPrintConditionGUI_OP_IPDS::SetSystemCallBackFunc(HOST inHostInfo)
{
	CPrintConditionGUI_OP::SetSystemCallBackFunc(inHostInfo);
	SetCommonCallBackFunc_OP_IPDS(inHostInfo);
}
