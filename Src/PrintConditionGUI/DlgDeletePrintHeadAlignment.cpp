/*! \file DlgDeletePrintHeadAlignment.cpp
 *  \brief The control to contains controls to delete Print head alignment.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgDeletePrintHeadAlignment.h"
#include "DataIF.h"

#include "CtlDeletePrintHeadAlignmentSelection.h"
#include "CtlClose_DlgDeletePrintHead.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		800//!< ダイアログの幅
#define DEF_DLG_H		680//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_DELETE_PRINT_HEAD = 0,					//!< Delete Print head alignment control
	CTLGP_CLOSE_SCREEN,								//!< Close screen control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgDeletePrintHeadAlignment::DlgDeletePrintHeadAlignment()
 *  \brief コンストラクタ
 */
DlgDeletePrintHeadAlignment::DlgDeletePrintHeadAlignment()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgDeletePrintHeadAlignment::~DlgDeletePrintHeadAlignment()
 *  \brief デストラクタ
 */
DlgDeletePrintHeadAlignment::~DlgDeletePrintHeadAlignment()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgDeletePrintHeadAlignment::OnSetAttribute()
{
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	int Width = jobSelect_ClientRect.right - jobSelect_ClientRect.left + 1;
	int Height = jobSelect_ClientRect.bottom - jobSelect_ClientRect.top + 1;

	RECT DlgRect =
	{
		(Width -  DEF_DLG_W) / 2,
		(Height - DEF_DLG_H) / 2,
		((Width - DEF_DLG_W) / 2) + DEF_DLG_W,
		((Height - DEF_DLG_H) / 2) + DEF_DLG_H
	};

	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__));

	//コントロールクラスをnewしてm_controls[0]～[m_controlCount-1]に登録する
	// Delete Print head alignment selection control
	CCtlDeletePrintHeadAlignmentSelection* ctlPrintHeadSelection = new CCtlDeletePrintHeadAlignmentSelection();
	m_controls[CTLGP_DELETE_PRINT_HEAD] = (CBaseCtl*)ctlPrintHeadSelection;

	// Close screen control
	CCtlClose_DlgDeletePrintHead* ctlCloseScreen = new CCtlClose_DlgDeletePrintHead();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;
}

/*! \brief event to set the properties of the dialog
*/
void DlgDeletePrintHeadAlignment::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Notify the dialog handle to DataIF
	pData->SetDeletePrintHeadAlignmentDlg(this->m_hWnd);
}