/*! \file DlgAddDryerSettingsWithPrecoat.cpp
 *  \brief Add dryer settings dialog with pre-coater
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddDryerSettingsWithPrecoat.h"
#include "DataIF.h"

#include "CtlCommitAddDryerSettings.h"
#include "CtlPrecoatDryerSettings.h"
#include "CtlFanDryerSettings.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		600//!< ダイアログの幅
#define DEF_DLG_H		410//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_FAN_DRYER_SETTINGS = 0,					//!< Fan dryer setting control
	CTLGP_PRECOAT_DRYER_SETTINGS,					//!< Precoat dryer setting control
	CTLGP_COMMIT_ADD_DRYER_SETTING,					//!< Commit to add dryer setting control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddDryerSettingsWithPrecoat::DlgAddDryerSettingsWithPrecoat()
 *  \brief コンストラクタ
 */
DlgAddDryerSettingsWithPrecoat::DlgAddDryerSettingsWithPrecoat()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddDryerSettingsWithPrecoat::~DlgAddDryerSettingsWithPrecoat()
 *  \brief デストラクタ
 */
DlgAddDryerSettingsWithPrecoat::~DlgAddDryerSettingsWithPrecoat()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgAddDryerSettingsWithPrecoat::OnSetAttribute()
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

	// Precoat dryer setting control
	CCtlPrecoatDryerSettings* ctlPrecoatDryerSetting = new CCtlPrecoatDryerSettings();
	m_controls[CTLGP_PRECOAT_DRYER_SETTINGS] = (CBaseCtl*)ctlPrecoatDryerSetting;
	
	// Commit to add dryer setting control
	POINT pos = {300, 330};
	CCtlCommitAddDryerSettings* ctlCommitAddDryerSetting = new CCtlCommitAddDryerSettings();
	ctlCommitAddDryerSetting->SetPersonalInfo(pos);
	m_controls[CTLGP_COMMIT_ADD_DRYER_SETTING] = (CBaseCtl*)ctlCommitAddDryerSetting;
}
