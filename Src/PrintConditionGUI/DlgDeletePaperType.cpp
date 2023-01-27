/*! \file DlgDeletePaperType.cpp
 *  \brief The control to contains controls to delete Paper type.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgDeletePaperType.h"
#include "DataIF.h"

#include "CtlDeletePaperTypeSelection.h"
#include "CtlClose_DlgDeletePaperType.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		800//!< ダイアログの幅
#define DEF_DLG_H		680//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_DELETE_PAPER_TYPE = 0,					//!< Delete paper type control
	CTLGP_CLOSE_SCREEN,								//!< Close screen control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgDeletePaperType::DlgDeletePaperType()
 *  \brief コンストラクタ
 */
DlgDeletePaperType::DlgDeletePaperType()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgDeletePaperType::~DlgDeletePaperType()
 *  \brief デストラクタ
 */
DlgDeletePaperType::~DlgDeletePaperType()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgDeletePaperType::OnSetAttribute()
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
	// Delete paper type control
	CCtlDeletePaperTypeSelection* ctlDeletePaperType = new CCtlDeletePaperTypeSelection();
	m_controls[CTLGP_DELETE_PAPER_TYPE] = (CBaseCtl*)ctlDeletePaperType;

	// Close screen control
	CCtlClose_DlgDeletePaperType* ctlCloseScreen = new CCtlClose_DlgDeletePaperType();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;
}

/*! \brief event to set the properties of the dialog
*/
void DlgDeletePaperType::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Notify the dialog handle to DataIF
	pData->SetDeletePaperTypeDlg(this->m_hWnd);
}