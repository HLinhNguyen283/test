/*! \file DlgPrintConditionDeleting.cpp
 *  \brief The control to contains controls to delete Print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgPrintConditionDeleting.h"
#include "DataIF.h"

#include "CtlPrintConditionList.h"
#include "CtlClosePrintConditionDeleteDlg.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		800//!< ダイアログの幅
#define DEF_DLG_H		680//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_DELETE_PRINT_SETTINGS = 0,				//!< Delete Print settings control
	CTLGP_CLOSE_SCREEN,								//!< Close screen control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgPrintConditionDeleting::DlgPrintConditionDeleting()
 *  \brief コンストラクタ
 */
DlgPrintConditionDeleting::DlgPrintConditionDeleting()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgPrintConditionDeleting::~DlgPrintConditionDeleting()
 *  \brief デストラクタ
 */
DlgPrintConditionDeleting::~DlgPrintConditionDeleting()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgPrintConditionDeleting::OnSetAttribute()
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
	// Delete Print settings control
	CCtlPrintConditionList* ctlPrintConditionList = new CCtlPrintConditionList();
	m_controls[CTLGP_DELETE_PRINT_SETTINGS] = (CBaseCtl*)ctlPrintConditionList;

	// Close screen control
	CCtlClosePrintConditionDeleteDlg* ctlCloseScreen = new CCtlClosePrintConditionDeleteDlg();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;
}

/*! \brief event to set the properties of the dialog
*/
void DlgPrintConditionDeleting::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Notify the dialog handle to DataIF
	pData->SetDeletePrintConditionDlg(this->m_hWnd);
}