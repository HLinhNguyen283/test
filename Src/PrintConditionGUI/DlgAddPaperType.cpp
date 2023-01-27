﻿/*! \file DlgAddPaperType.cpp
 *  \brief Add Paper Type Dialog
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo Created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddPaperType.h"
//
#include "CtlNewPaperType.h"
#include "CtlDuplicatePaperType.h"
#include "CtlSaveAndExitPaperType.h"

// ==========================================================================
// Local definitions
#define W_DIALOG			815		//!< Width of Dlg
#define H_DIALOG			250		//!< Height of Dlg

//! コントロールグループ一覧
enum{
	CTLGP_PRINT_SETTING_NAME = 0,		//!< new print setting name control
	CTLGP_PRINT_SETTING_DUPLICATE,	//!< print setting for duplicating control
	CTLGP_COMMIT,										//!< save and exit control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddPaperType::DlgAddPaperType()
 *  \brief コンストラクタ
 */
DlgAddPaperType::DlgAddPaperType()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddPaperType::~DlgAddPaperType()
 *  \brief デストラクタ
 */
DlgAddPaperType::~DlgAddPaperType()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgAddPaperType::OnSetAttribute()
{
	//ダイアログ作成
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	int Width = jobSelect_ClientRect.right - jobSelect_ClientRect.left + 1;
	int Height = jobSelect_ClientRect.bottom - jobSelect_ClientRect.top + 1;

	RECT DlgRect =
	{
		(Width - W_DIALOG) / 2,
		(Height - H_DIALOG) / 2,
		((Width - W_DIALOG) / 2) + W_DIALOG,
		((Height - H_DIALOG) / 2) + H_DIALOG
	};

	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__));

	//コントロールクラスをnewしてm_controls[0]～[m_controlCount-1]に登録する

	// new paper type name control
	CCtlNewPaperType* newPaperType = new CCtlNewPaperType();
	m_controls[CTLGP_PRINT_SETTING_NAME] = (CBaseCtl*)newPaperType;

	// duplicate paper type control
	CCtlDuplicatePaperType* duplicatePaperType = new CCtlDuplicatePaperType();
	m_controls[CTLGP_PRINT_SETTING_DUPLICATE] = (CBaseCtl*)duplicatePaperType;

	// save and exit control
	CCtlSaveAndExitPaperType* commitPaperType = new CCtlSaveAndExitPaperType();
	m_controls[CTLGP_COMMIT] = (CBaseCtl*)commitPaperType;
}

