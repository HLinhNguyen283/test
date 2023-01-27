/*! \file CtlVariableMagnificationReferencePlane.cpp
 *  \brief 基準面指定コントロール
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlVariableMagnificationReferencePlane.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PluginCBRProc.h"

// ==========================================================================
// Local definitions
static const long ButtonWidth = 60;			//!< ボタンの幅
static const long HeightWidth = 60;			//!< ボタンの高さ

//! ウィンドウID一覧
enum
{
	CTRLID_BN_REFERENCE_PLANE_FRONT = 0,	//!< : Button: 基準面指定(表)ボタン
	CTRLID_BN_REFERENCE_PLANE_BACK,			//!< : Button: 基準面指定(裏)ボタン

	CTLID_COUNT
};

// ==========================================================================

// コンストラクタ
CCtlVariableMagnificationReferencePlane::CCtlVariableMagnificationReferencePlane()
{
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlVariableMagnificationReferencePlane::~CCtlVariableMagnificationReferencePlane()
{
}

// コントロール属性設定イベント
void CCtlVariableMagnificationReferencePlane::OnSetAttribute()
{
	// button: 基準面指定(表)ボタン
	{
		long ctlId = CTRLID_BN_REFERENCE_PLANE_FRONT;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_FRONT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 245, 30 + ButtonWidth, 245 + HeightWidth);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_FRONT, RESOURCE_BMP));
	}

	// button: 基準面指定(裏)ボタン
	{
		long ctlId = CTRLID_BN_REFERENCE_PLANE_BACK;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BACK, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 90, 245, 90 + ButtonWidth, 245 + HeightWidth);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_BACK, RESOURCE_BMP));
	}
}

// コントロール操作イベント
long CCtlVariableMagnificationReferencePlane::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);
	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			if((m_ctl[CTRLID_BN_REFERENCE_PLANE_FRONT] == ctlWnd) || (m_ctl[CTRLID_BN_REFERENCE_PLANE_BACK] == ctlWnd))
			{
				dataIF->onPush_VariableMagnificationReferencePlane();
				break;
			}
		}
	}

	return DEF_NONE;
}

// コントロールステータスイベント
void CCtlVariableMagnificationReferencePlane::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isShow = false;

		dataIF->onView_VariableMagnificationReferencePlane(isShow);
		DWORD show = isShow ? CST_SHOW : CST_HIDE;
		SetControlState(m_ctl[CTRLID_BN_REFERENCE_PLANE_FRONT], show);
		SetControlState(m_ctl[CTRLID_BN_REFERENCE_PLANE_BACK], show);
	}
}

// コントロール表示内容更新イベント
void CCtlVariableMagnificationReferencePlane::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isEditDatumPlaneFront = false;
		BNITEMINFO bn_FrontItem = {0};
		BNITEMINFO bn_BackItem = {0};
		
		dataIF->onDraw_VariableMagnificationReferencePlane(isEditDatumPlaneFront);
		bn_FrontItem.bLED_On = isEditDatumPlaneFront;
		bn_BackItem.bLED_On = isEditDatumPlaneFront ? false : true;

		SetControlItem(m_ctl[CTRLID_BN_REFERENCE_PLANE_FRONT], 0, &bn_FrontItem);
		SetControlData(m_ctl[CTRLID_BN_REFERENCE_PLANE_FRONT], isEditDatumPlaneFront ? 1 : 0);
		SetControlItem(m_ctl[CTRLID_BN_REFERENCE_PLANE_BACK], 0, &bn_BackItem);
		SetControlData(m_ctl[CTRLID_BN_REFERENCE_PLANE_BACK], isEditDatumPlaneFront ? 0 : 1);
	}	
}
