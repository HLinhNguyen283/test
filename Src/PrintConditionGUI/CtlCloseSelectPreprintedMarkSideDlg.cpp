/*! \file CtlCloseSelectPreprintedMarkSideDlg.cpp
 *  \brief Close screen control of Select preprinted mark side dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/20 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlCloseSelectPreprintedMarkSideDlg.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

/*! \brief constructor
*/
CCtlCloseSelectPreprintedMarkSideDlg::CCtlCloseSelectPreprintedMarkSideDlg()
{
	//set to create the number of control.
	m_ctlCount = 0;

}

/*! \brief destructor
*/
CCtlCloseSelectPreprintedMarkSideDlg::~CCtlCloseSelectPreprintedMarkSideDlg()
{
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlCloseSelectPreprintedMarkSideDlg::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_FILTER_CLICK_DLG_CLOSE:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
			// Close the dialog
			pData->ExitSelectPreprintedMarkSideDlg();
			return DEF_EXIT_PAGE;
		}
		break;
	}

	return DEF_NONE;
}
