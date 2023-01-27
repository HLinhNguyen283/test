/*! \file CtlDuplicateJIAreaSetting.cpp
 *  \brief duplicate JI area setting control for adding print settings
 *  \author SCREEN mizuta
 *  \date 2014/12/01 SCREEN mizuta new
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDuplicateJIAreaSetting.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
//! the control id list
enum{
	CTRLID_CK_DUPLICATE = 0,					//!< Duplicate check box

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlDuplicateJIAreaSetting::CCtlDuplicateJIAreaSetting()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlDuplicateJIAreaSetting::~CCtlDuplicateJIAreaSetting()
{
}


/*! \brief [event]set the control's property
*/
void CCtlDuplicateJIAreaSetting::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//check-box: Duplicate Jet Inspection area setting.
	{
		long ctlId = CTRLID_CK_DUPLICATE;
		m_ctlAttribute[ctlId].type = CT_CHECKBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_JI_DUPLICATING, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 120,  175, 120 + 670, 175 + 40);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlDuplicateJIAreaSetting::OnCreateItem()
{
}

/*! \brief コントロールのアイテムを破棄するイベント
*/
void CCtlDuplicateJIAreaSetting::OnDeleteItem()
{
	// チェックボックスの初期値をOFFにする
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	pData->SetDuplicateJIAreaSettings(false);
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlDuplicateJIAreaSetting::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			// duplicate print setting name menu pull-down
			if(ctlWnd == m_ctl[CTRLID_CK_DUPLICATE]){
				// change the selection print setting name
				bool jiValid = GetControlData(m_ctl[CTRLID_CK_DUPLICATE]) ? true : false;
				pData->SetDuplicateJIAreaSettings(jiValid);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDuplicateJIAreaSetting::OnUpdateState()
{
	if(m_ctl){
		// always show all controls
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		if(pData->IsJIDuplicatingEnable()){
			SetControlState(m_ctl[CTRLID_CK_DUPLICATE], CST_SHOW);
		}else{
			SetControlState(m_ctl[CTRLID_CK_DUPLICATE], CST_DISABLED);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlDuplicateJIAreaSetting::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	bool jiDuplivate = pData->GetDuplicateJIAreaSettings();
	SetControlData(m_ctl[CTRLID_CK_DUPLICATE], jiDuplivate ? 1 : 0);
}

