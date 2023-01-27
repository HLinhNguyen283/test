/*! \file DlgSelectPreprintedMarkSide.cpp
 *  \brief The control to contains controls to select preprinted mark side
 *  \author GCS Tan Ho
 *  \date 2013/05/20 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgSelectPreprintedMarkSide.h"
#include "DataIF.h"

#include "CtlSideSelection.h"
#include "CtlCloseSelectPreprintedMarkSideDlg.h"

// ==========================================================================
// Local definitions

//! コントロールグループ一覧
enum{
	CTLGP_SIDE_SELECTION,						//!< Side selection control
	CTLGP_CLOSE_SCREEN,							//!< Close screen control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgSelectPreprintedMarkSide::DlgSelectPreprintedMarkSide()
 *  \brief コンストラクタ
 */
DlgSelectPreprintedMarkSide::DlgSelectPreprintedMarkSide()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgSelectPreprintedMarkSide::~DlgSelectPreprintedMarkSide()
 *  \brief デストラクタ
 */
DlgSelectPreprintedMarkSide::~DlgSelectPreprintedMarkSide()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgSelectPreprintedMarkSide::OnSetAttribute()
{
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	RECT DlgRect =
	{
		670,
		530,
		670 + 71,
		530 + 142
	};
	BYTE filterAlpha = 1;

	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__), filterAlpha);

	//コントロールクラスをnewしてm_controls[0]～[m_controlCount-1]に登録する
	// Side selection control
	CCtlSideSelection* ctlSideSelection = new CCtlSideSelection();
	m_controls[CTLGP_SIDE_SELECTION] = (CBaseCtl*)ctlSideSelection;

	// Close screen control
	CCtlCloseSelectPreprintedMarkSideDlg* ctlCloseScreen = new CCtlCloseSelectPreprintedMarkSideDlg();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;
}
