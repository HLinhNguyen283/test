/*! \file DlgAddDryerSettings.cpp
 *  \brief The control edits a heat roller setting/dryer fun setting for printer 1 and 2.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddDryerSettings.h"
#include "DataIF.h"

#include "CtlCommitAddDryerSettings.h"
#include "CtlFanDryerSettings.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		600//!< ダイアログの幅
#define DEF_DLG_H		310//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_FAN_DRYER_SETTINGS = 0,					//!< Fan dryer setting control
	CTLGP_COMMIT_ADD_DRYER_SETTING,					//!< Commit to add dryer setting control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddDryerSettings::DlgAddDryerSettings()
 *  \brief コンストラクタ
 */
DlgAddDryerSettings::DlgAddDryerSettings()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddDryerSettings::~DlgAddDryerSettings()
 *  \brief デストラクタ
 */
DlgAddDryerSettings::~DlgAddDryerSettings()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgAddDryerSettings::OnSetAttribute()
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
	// Fan dryer setting control
	CCtlFanDryerSettings* ctlFanDryerSetting = new CCtlFanDryerSettings();
	m_controls[CTLGP_FAN_DRYER_SETTINGS] = (CBaseCtl*)ctlFanDryerSetting;
	
	// Commit to add dryer setting control
	POINT pos = {300, 230};
	CCtlCommitAddDryerSettings* ctlCommitAddDryerSetting = new CCtlCommitAddDryerSettings();
	ctlCommitAddDryerSetting->SetPersonalInfo(pos);
	m_controls[CTLGP_COMMIT_ADD_DRYER_SETTING] = (CBaseCtl*)ctlCommitAddDryerSetting;
}
