/*! \file PrinterIF.cpp
 *  \brief printer interface class
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "PrinterIF.h"
#include "BasePageDef.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "CommonUIMsg.h"
#include "OutputCBR_OP.h"
#include "Stub_PrinterStateIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

/*! \brief constructor
*/
CPrinterIF::CPrinterIF()
{
	m_data = NULL;
}

/*! \brief destructor
*/
CPrinterIF::~CPrinterIF()
{
}

/*! \brief set interface
 *  \param[in] inData : class of data interface
*/
void CPrinterIF::SetInterface(void* inData)
{
	CBaseDataIF* data = (CBaseDataIF*)inData;
	m_data = dynamic_cast<CDataIF*>(data);
}

/*! \brief UI_Initialize.
 *  \retval TRUE : success
 *  \retval FALSE : fail
*/
BOOL CPrinterIF::UI_Initialize()
{
	return TRUE;
}

/*! \brief UI_Finalize.
*/
void CPrinterIF::UI_Finalize()
{
}

/*! \brief window message procedure
 *  \param[in] hWnd : window handle
 *  \param[in] Message : message id
 *  \param[in] wParam : parameter 1
 *  \param[in] lParam : parameter 2
 *  \retval DEF_NONE : continue showing the screen
 *  \retval DEF_EXIT_PAGE : close the screen
 *  \retval DEF_UPDATE_STATE : update status of all controls
 *  \retval DEF_UPDATE_VALUE : update data of all controls
*/
long CPrinterIF::UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DEF_NONE;
}

/*! \brief Check if the printer stoped or not.
 *  \retval true : the printer stoped
 *  \retval false : the printer does not stop
*/
bool CPrinterIF::IsPrinterStop()
{
	return true;
}

// 調整印刷中かどうか取得する
bool CPrinterIF::IsDuringAdjustPrinting()
{
	// プリンターの動作情報を取得する
	ST_PRINTER_ACTION printerAction = Stub_PrinterStateIF::GetPrinterAction();

	if(printerAction.adjustPrint.working)
	{
		if(kActionInfoModule_Other != printerAction.adjustPrint.modlue)
		{
			// 印刷濃度均一化画面、段差調整画面、ヘッドクリーニング画面から調整印刷実行中の場合、調整印刷中を返す
			return true;
		}
	}

	// それ以外からの画面の場合、調整印刷中ではないを返す
	return false;
}