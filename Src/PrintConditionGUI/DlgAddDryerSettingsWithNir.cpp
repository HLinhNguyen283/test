/*! \file DlgAddDryerSettingsWithNir.cpp
 *  \brief Add dryer settings dialog with nir
 *  \author SCREEN
 *  \date 2018/01/25 SCREEN created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddDryerSettingsWithNir.h"
#include "DataIF.h"

#include "CtlCommitAddDryerSettings.h"
#include "CtlNirDryerSettings.h"
#include "CtlFanDryerSettings.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		600//!< ダイアログの幅
#define DEF_DLG_H		410//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_FAN_DRYER_SETTINGS = 0,					//!< Fan dryer setting control
	CTLGP_NIR_DRYER_SETTINGS,						//!< Nir dryer setting control
	CTLGP_COMMIT_ADD_DRYER_SETTING,					//!< Commit to add dryer setting control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddDryerSettingsWithNir::DlgAddDryerSettingsWithNir()
 *  \brief コンストラクタ
 */
DlgAddDryerSettingsWithNir::DlgAddDryerSettingsWithNir()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddDryerSettingsWithNir::~DlgAddDryerSettingsWithNir()
 *  \brief デストラクタ
 */
DlgAddDryerSettingsWithNir::~DlgAddDryerSettingsWithNir()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgAddDryerSettingsWithNir::OnSetAttribute()
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

	// Nir power setting control
	CCtlNirDryerSettings* ctlNirDryerSetting = new CCtlNirDryerSettings();
	m_controls[CTLGP_NIR_DRYER_SETTINGS] = (CBaseCtl*)ctlNirDryerSetting;
	
	// Commit to add dryer setting control
	POINT pos = {300, 330};
	CCtlCommitAddDryerSettings* ctlCommitAddDryerSetting = new CCtlCommitAddDryerSettings();
	ctlCommitAddDryerSetting->SetPersonalInfo(pos);
	m_controls[CTLGP_COMMIT_ADD_DRYER_SETTING] = (CBaseCtl*)ctlCommitAddDryerSetting;
}
