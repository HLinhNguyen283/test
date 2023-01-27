/*! \file DlgVariableMagnification.cpp
 *  \brief 変倍ダイアログ
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DlgVariableMagnification.h"
#include "DataIF.h"
#include "CtlVariableMagnificationInput.h"
#include "CtlVariableMagnificationCommitClose.h"
#include "CtlVariableMagnificationRip.h"
#include "CtlVariableMagnificationReferencePlane.h"

// ==========================================================================
// Local definitions
static const long DlgWidth = 500;		//!< ダイアログの幅
static const long DlgHeight = 330;		//!< ダイアログの高さ

//! コントロールグループ一覧
enum
{
	CTLGP_VARIABLE_MAGNIFICATION_INPUT = 0,		//!< 変倍入力コントロール
	CTLGP_COMMIT_CLOSE,							//!< 登録/終了ボタンコントロール
	CTLGP_RIP_VARIABLE_MAGNIFICATION_SET,		//!< RIP変倍値設定コントロール
	CTLGP_REFERENCE_PLANE,						//!< 基準面指定コントロール

	CTLGP_COUNT
};

// ==========================================================================

// コンストラクタ
DlgVariableMagnification::DlgVariableMagnification()
{
	m_controlCount = CTLGP_COUNT;
}

// デストラクタ
DlgVariableMagnification::~DlgVariableMagnification()
{
}

// コントロール属性設定イベント
void DlgVariableMagnification::OnSetAttribute()
{
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	int Width = jobSelect_ClientRect.right - jobSelect_ClientRect.left + 1;
	int Height = jobSelect_ClientRect.bottom - jobSelect_ClientRect.top + 1;

	RECT DlgRect =
	{
		(Width -  DlgWidth) / 2,
		(Height - DlgHeight) / 2,
		((Width - DlgWidth) / 2) + DlgWidth,
		((Height - DlgHeight) / 2) + DlgHeight
	};
	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__));

	//コントロールクラスをnewしてm_controls[0]～[m_controlCount-1]に登録する

	// 変倍の入力
	{
		CCtlVariableMagnificationInput* ctlVariableMagnificationInput = new CCtlVariableMagnificationInput();
		m_controls[CTLGP_VARIABLE_MAGNIFICATION_INPUT] = reinterpret_cast<CBaseCtl*>(ctlVariableMagnificationInput);
	}

	// 登録/終了ボタン
	{
		CCtlVariableMagnificationCommitClose* ctlVariableMagnificationCommitClose = new CCtlVariableMagnificationCommitClose();
		m_controls[CTLGP_COMMIT_CLOSE] = reinterpret_cast<CBaseCtl*>(ctlVariableMagnificationCommitClose);
	}

	// RIP変倍値設定
	{
		CCtlVariableMagnificationRip* ctlVariableMagnificationRip = new CCtlVariableMagnificationRip();
		m_controls[CTLGP_RIP_VARIABLE_MAGNIFICATION_SET] = reinterpret_cast<CBaseCtl*>(ctlVariableMagnificationRip);
	}

	// 基準面指定
	{
		CCtlVariableMagnificationReferencePlane* ctlVariableMagnificationReferencePlane = new CCtlVariableMagnificationReferencePlane();
		m_controls[CTLGP_REFERENCE_PLANE] = reinterpret_cast<CBaseCtl*>(ctlVariableMagnificationReferencePlane);
	}
}