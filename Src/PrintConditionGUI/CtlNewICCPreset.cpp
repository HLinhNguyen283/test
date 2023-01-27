/*! \file CtlNewICCPreset.cpp
 *  \brief new ICC preset name
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlNewICCPreset.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_MAX_LENGTH				64

//! the control id list
enum{
	CTRLID_ST_ICC_PRESET_ICON = 0,					//!< ICC Preset icon
	CTRLID_EB_ICC_PRESET_NAME,						//!< ICC Preset name edit-box

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlNewICCPreset::CCtlNewICCPreset()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlNewICCPreset::~CCtlNewICCPreset()
{
}


/*! \brief [event]set the control's property
*/
void CCtlNewICCPreset::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//icon: ICC Preset
	{
		long ctlId = CTRLID_ST_ICC_PRESET_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 25, 50, 25 + 50, 50 + 50);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_TYPE_ICON, RESOURCE_BMP);
	}

	//edit-box: ICC Preset name
	{
		long ctlId = CTRLID_EB_ICC_PRESET_NAME;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL | EBST_IME;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 110, 45, 110 + 450, 45 + 60);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlNewICCPreset::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	switch(wParam)
	{
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// ICC Preset name edit-box
			if(ctlWnd == m_ctl[CTRLID_EB_ICC_PRESET_NAME]){
				// change the ICC Preset name
				char* strICCPresetName = (char*)GetControlData(m_ctl[CTRLID_EB_ICC_PRESET_NAME]);
				pData->SetAddICCPresetNameDlg(strICCPresetName);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief Control value update event
*/
void CCtlNewICCPreset::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// display the ICC Preset name
	const char* strICCPresetName = pData->GetAddICCPresetNameDlg();
	SetControlData(m_ctl[CTRLID_EB_ICC_PRESET_NAME], (DWORD)strICCPresetName);
}