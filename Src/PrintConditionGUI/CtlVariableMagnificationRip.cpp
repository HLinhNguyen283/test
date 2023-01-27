/*! \file CtlVariableMagnificationRip.cpp
 *  \brief RIP変倍値設定コントロール
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlVariableMagnificationRip.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PluginCBRProc.h"

// ==========================================================================
// Local definitions
static const long CheckBoxWidth = 262;		//!< チェックボックスの幅
static const long CheckBoxHeigt = 50;		//!< チェックボックスの高さ

//! ウィンドウID一覧
enum
{
	CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET = 0,		//!< check-box: Rip変倍指定チェックボックス

	CTLID_COUNT
};

// ==========================================================================

// コンストラクタ
CCtlVariableMagnificationRip::CCtlVariableMagnificationRip()
{
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlVariableMagnificationRip::~CCtlVariableMagnificationRip()
{
}

// コントロール属性設定イベント
void CCtlVariableMagnificationRip::OnSetAttribute()
{
	// check-box: Rip変倍指定チェックボックス
	{
		long ctlId = CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET;
		m_ctlAttribute[ctlId].type = CT_CHECKBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_RIP_VARIABLE_MAGNIFICATION_SET, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect,  30,  20, 30 + CheckBoxWidth,  20 + CheckBoxHeigt);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

// コントロール操作イベント
long CCtlVariableMagnificationRip::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);
	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			if(m_ctl[CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET] == ctlWnd)
			{
				bool isRipVariableMagnification = GetControlData(m_ctl[CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET]) != 0;
				dataIF->onPush_RipVariableMagnification(isRipVariableMagnification);
				break;
			}
		}
	}

	return DEF_NONE;
}

// コントロール表示状態更新イベント
void CCtlVariableMagnificationRip::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isShow = false;

		dataIF->onView_RipVariableMagnification(isShow);
		DWORD show = isShow ? CST_SHOW : CST_HIDE;
		SetControlState(m_ctl[CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET], show);
	}
}

// コントロール表示内容更新イベント
void CCtlVariableMagnificationRip::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isRipVariableMagnification = false;

		dataIF->onDraw_RipVariableMagnification(isRipVariableMagnification);
		SetControlData(m_ctl[CTRLID_CB_RIP_VARIABLE_MAGNIFICATION_SET], isRipVariableMagnification ? 1 : 0);
	}
}
