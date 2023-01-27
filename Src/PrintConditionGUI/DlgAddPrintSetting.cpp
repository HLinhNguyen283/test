/*! \file DlgAddPrintSetting.cpp
 *  \brief Add Print Setting Dialog
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo Created
 */

// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddPrintSetting.h"
//
#include "CtlNewPrintSettingName.h"
#include "CtlDuplicatePrintSettingName.h"
#include "CtlSaveAndExitPrintSetting.h"
#include "CtlDuplicateJIAreaSetting.h"

// ==========================================================================
// Local definitions
#define W_DIALOG			815		//!< Width of Dlg
#define H_DIALOG			310		//!< Height of Dlg

//! コントロールグループ一覧
enum{
	CTLGP_PRINT_SETTING_NAME = 0,		//!< new print setting name control
	CTLGP_PRINT_SETTING_DUPLICATE,	//!< print setting for duplicating control
	CTLGP_COMMIT,										//!< save and exit control
	CTLGP_JI_AREAINFO_DUPLICATE,	//!< duplicate JI area setting control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddPrintSetting::DlgAddPrintSetting()
 *  \brief コンストラクタ
 */
DlgAddPrintSetting::DlgAddPrintSetting()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddPrintSetting::~DlgAddPrintSetting()
 *  \brief デストラクタ
 */
DlgAddPrintSetting::~DlgAddPrintSetting()
{
}

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void DlgAddPrintSetting::OnSetAttribute()
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

	// new print setting name control
	CCtlNewPrintSettingName* newPrintSetting = new CCtlNewPrintSettingName();
	m_controls[CTLGP_PRINT_SETTING_NAME] = (CBaseCtl*)newPrintSetting;

	// duplicate print setting control
	CCtlDuplicatePrintSettingName* duplicatePrintSetting = new CCtlDuplicatePrintSettingName();
	m_controls[CTLGP_PRINT_SETTING_DUPLICATE] = (CBaseCtl*)duplicatePrintSetting;

	// save and exit control
	CCtlSaveAndExitPrintSetting* commitPrintSetting = new CCtlSaveAndExitPrintSetting();
	m_controls[CTLGP_COMMIT] = (CBaseCtl*)commitPrintSetting;

	//  duplicate JI area setting control
	CCtlDuplicateJIAreaSetting* duplicateJI = new CCtlDuplicateJIAreaSetting();
	m_controls[CTLGP_JI_AREAINFO_DUPLICATE] = (CBaseCtl*)duplicateJI;

}

