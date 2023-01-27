/**
 * @file	CtlPrimer.cpp
 * @brief	プライマーのコントロール
**/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "CtlPrimer.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! ウィンドウID一覧
enum
{
	CTRLID_BN_PRIMER = 0,	//!< プライマーボタン
	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_primer = "CTRLID_BN_PRIMER";
// ==========================================================================

// コンストラクタ
CCtlPrimer::CCtlPrimer()
{
	//このクラスで作成するコントロール数をセット
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlPrimer::~CCtlPrimer()
{
}

// コントロール属性設定イベント
void CCtlPrimer::OnSetAttribute()
{
	//作成するコントロールの属性をm_ctlAttribute[0]～[m_ctlCount-1]に登録する

	// Button: プライマーボタン
	{
		long ctlId = CTRLID_BN_PRIMER;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_PRIMER, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 1045, 150, 1045 + 70, 150 + 70);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_PRIMER, RESOURCE_BMP));
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_primer;
	}
}

// コントロール操作イベント
long CCtlPrimer::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);
	CDataIF* data = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			if (ctlWnd == m_ctl[CTRLID_BN_PRIMER])
			{
				long isON = GetControlData(m_ctl[CTRLID_BN_PRIMER]);
				// データ部にボタン押下状態を通知する
				data->onPush_Primer(isON == 0 ? false : true);
			}
		}
		break;
	}
	return DEF_NONE;
}

// コントロール表示値更新イベント
void CCtlPrimer::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* data = dynamic_cast<CDataIF*>(m_data);
		bool isShow = false;
		bool isEnabled = false;

		// データ部から表示状態と有効状態を取得する
		data->onView_Primer(isShow, isEnabled);

		// 表示状態の設定
		{
			if(isShow)
			{
				SetControlState(m_ctl[CTRLID_BN_PRIMER], CST_SHOW);
			}
		}

		// 有効状態の設定
		{
			DWORD dwState = isEnabled ? CST_ENABLED : CST_DISABLED;
			SetControlState(m_ctl[CTRLID_BN_PRIMER], dwState);
		}
	}
}

// コントロールステータスイベント
void CCtlPrimer::OnUpdateState()
{
	CDataIF* data = dynamic_cast<CDataIF*>(m_data);

	bool isOn = false;
	// データ部からボタン状態を取得する
	data->onDraw_Primer(isOn);

	BNITEMINFO bn_item = {0};
	bn_item.bLED_On = isOn;
	SetControlItem(m_ctl[CTRLID_BN_PRIMER], 0, &bn_item);
	SetControlData(m_ctl[CTRLID_BN_PRIMER], static_cast<DWORD>(isOn));
}
