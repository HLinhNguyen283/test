/*! \file DlgAddICCPreset.cpp
 *  \brief Add ICC Preset Dialog
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh Created
 */

// ==========================================================================
// Include files
#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonCBRProc.h"
#include "DlgAddICCPreset.h"
#include "CtlNewICCPreset.h"
#include "CtlSaveAndExitICCPresetDlg.h"

// ==========================================================================
// Local definitions
#define W_DIALOG			585		//!< Width of Dlg
#define H_DIALOG			270		//!< Height of Dlg

//! Control list
enum{
	CTLGP_ICC_PRESET_NAME = 0,		//!< new ICC preset name control
	CTLGP_COMMIT,					//!< save and exit control

	CTLGP_COUNT
};
// ==========================================================================

/*! \fn void DlgAddICCPreset::DlgAddICCPreset()
 *  \brief contructor
 */
DlgAddICCPreset::DlgAddICCPreset()
{
	m_controlCount = CTLGP_COUNT;
}

/*! \fn void DlgAddICCPreset::~DlgAddICCPreset()
 *  \brief destructor
 */
DlgAddICCPreset::~DlgAddICCPreset()
{
}

/*! \brief [event]set the control's property
*/
void DlgAddICCPreset::OnSetAttribute()
{
	RECT clientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &clientRect);

	int Width = clientRect.right - clientRect.left + 1;
	int Height = clientRect.bottom - clientRect.top + 1;

	RECT DlgRect =
	{
		(Width - W_DIALOG) / 2,
		(Height - H_DIALOG) / 2,
		((Width - W_DIALOG) / 2) + W_DIALOG,
		((Height - H_DIALOG) / 2) + H_DIALOG
	};

	// オフセットの加算
	OffsetRect(&DlgRect, clientRect.left, clientRect.top);

	SetAttribute(&DlgRect, FALSE, PathFindFileName(__FILE__));

	// new ICC preset name control
	CCtlNewICCPreset* newICCPreset = new CCtlNewICCPreset();
	m_controls[CTLGP_ICC_PRESET_NAME] = (CBaseCtl*)newICCPreset;

	// save and exit control
	CCtlSaveAndExitICCPresetDlg* commitICCPreset = new CCtlSaveAndExitICCPresetDlg();
	m_controls[CTLGP_COMMIT] = (CBaseCtl*)commitICCPreset;
}

