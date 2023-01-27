/*! \file DlgDeleteDryerSettings.cpp
 *  \brief The control to contains controls to delete dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
 */
// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgDeleteDryerSettings.h"
#include "DataIF.h"

#include "CtlDeleteDryerSettingsSelection_DlgDeleteDryer.h"
#include "CtlClose_DlgDeleteDryer.h"

// ==========================================================================
// Local definitions
#define DEF_DLG_W		800//!< ダイアログの幅
#define DEF_DLG_H		680//!< ダイアログの高さ

//! コントロールグループ一覧
enum{
	CTLGP_DELETE_DRYER_SETTINGS = 0,					//!< Delete dryer settings control
	CTLGP_CLOSE_SCREEN,									//!< Close screen control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgDeleteDryerSettings::DlgDeleteDryerSettings()
 *  \brief コンストラクタ
 */
DlgDeleteDryerSettings::DlgDeleteDryerSettings()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgDeleteDryerSettings::~DlgDeleteDryerSettings()
 *  \brief デストラクタ
 */
DlgDeleteDryerSettings::~DlgDeleteDryerSettings()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgDeleteDryerSettings::OnSetAttribute()
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
	// Delete dryer settings control
	CCtlDeleteDryerSettingsSelection_DlgDeleteDryer* ctlDeleteDryerSettings = new CCtlDeleteDryerSettingsSelection_DlgDeleteDryer();
	m_controls[CTLGP_DELETE_DRYER_SETTINGS] = (CBaseCtl*)ctlDeleteDryerSettings;

	// Close screen control
	CCtlClose_DlgDeleteDryer* ctlCloseScreen = new CCtlClose_DlgDeleteDryer();
	m_controls[CTLGP_CLOSE_SCREEN] = (CBaseCtl*)ctlCloseScreen;
}

/*! \brief event to set the properties of the dialog
*/
void DlgDeleteDryerSettings::OnCreateItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Notify the dialog handle to DataIF
	pData->SetDeleteDryerSettingDlg(this->m_hWnd);
}