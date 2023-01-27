/*! \file CtlVariableMagnification.cpp
 *  \brief 変倍コントロール
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlVariableMagnification.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PluginCBRProc.h"
#include "DlgVariableMagnification.h"

// ==========================================================================
// Local definitions

//! ウィンドウID一覧
enum
{
	CTRLID_BN_VARIABLE_MAGNIFICATION = 0,				//!< Button: 変倍ボタン

	CTRLID_SC_ICON_FRONT,								//!< static-box: プリンタアイコン(表面)
	CTRLID_SC_ICON_LEFT_RIGH_FRONT,						//!< static-box: 左右アイコン(表面)
	CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT,	//!< static-box: 左右変倍値(表面)
	CTRLID_SC_ICON_UP_DOWN_FRONT,						//!< static-box: 上下アイコン(表面)
	CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT,		//!< static-box: 上下変倍値(表面)

	CTRLID_SC_ICON_BACK,								//!< static-box: プリンタアイコン(裏面)
	CTRLID_SC_ICON_LEFT_RIGH_BACK,						//!< static-box: 左右アイコン(裏面)
	CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK,	//!< static-box: 左右変倍値(裏面)
	CTRLID_SC_ICON_UP_DOWN_BACK,						//!< static-box: 上下アイコン(裏面)
	CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_BACK,		//!< static-box: 上下変倍値(裏面)	

	CTLID_COUNT
};

// ==========================================================================

// コンストラクタ
CCtlVariableMagnification::CCtlVariableMagnification()
{
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlVariableMagnification::~CCtlVariableMagnification()
{
}

// コントロール属性設定イベント
void CCtlVariableMagnification::OnSetAttribute()
{
	//button： 変倍ボタン
	{
		long ctlId = CTRLID_BN_VARIABLE_MAGNIFICATION;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_VARIABLE_MAGNIFICATION, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 770, 150, 770 + 130, 150 + 70);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_CORRECT_PRINT_POS, RESOURCE_BMP));
	}

	//!< static-box: プリンタアイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 770, 99, 770 + 23, 99 + 23);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_FRONT_MINI, RESOURCE_BMP));
	}

	//!< static-box: 左右アイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_LEFT_RIGH_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 793, 100, 793 + 21, 100 + 21);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_LEFT_RIGHT_MINI, RESOURCE_BMP));
	}

	//!< static-box: 左右変倍値(表面)
	{
		long ctlId = CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 816, 97, 816 + 48, 97 + 28);
		m_ctlAttribute[ctlId].param = 0;
	}

	//!< static-box: 上下アイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_UP_DOWN_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 866, 101, 866 + 21, 101 + 21);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_UP_DOWN_MINI, RESOURCE_BMP));
	}

	//!< static-box: 上下変倍値(表面)
	{
		long ctlId = CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 889, 97, 889 + 48, 97 + 28);
		m_ctlAttribute[ctlId].param = 0;
	}


	//!< static-box: プリンタアイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 770, 125, 770 + 23, 125 + 23);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_BACK_MINI, RESOURCE_BMP));
	}

	//!< static-box: 左右アイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_LEFT_RIGH_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 793, 126, 793 + 21, 126 + 21);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_LEFT_RIGHT_MINI, RESOURCE_BMP));
	}

	//!< static-box: 左右変倍値(裏面)
	{
		long ctlId = CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 816, 123, 816 + 48, 123 + 28);
		m_ctlAttribute[ctlId].param = 0;
	}

	//!< static-box: 上下アイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_UP_DOWN_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 866, 126, 866 + 21, 126 + 21);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_UP_DOWN_MINI, RESOURCE_BMP));
	}

	//!< static-box: 上下変倍値(裏面)
	{
		long ctlId = CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 889, 123, 889 + 48, 123 + 28);
		m_ctlAttribute[ctlId].param = 0;
	}
}

// コントロール操作イベント
long CCtlVariableMagnification::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);

	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
		case BN_CLICKED:
			{
				// 変倍ボタン
				if(ctlWnd == m_ctl[CTRLID_BN_VARIABLE_MAGNIFICATION])
				{
					dataIF->onPush_VariableMagnification(m_data, m_printer);
				}
			}
			break;
	}
	
	return DEF_NONE;
}

// コントロール表示状態更新イベント
void CCtlVariableMagnification::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isButtonShow = false;
		bool isButtonEnabled = false;
		bool isFieldShowFront = false;
		bool isFieldShowBack = false;

		// データ部から表示状態や有効状態を取得する
		dataIF->onView_VariableMagnification(isButtonShow, isButtonEnabled);
		dataIF->onView_VariableMagnificationField(isFieldShowFront, isFieldShowBack);

		// 変倍ボタン表示状態の設定
		{
			if(isButtonShow)
			{
				SetControlState(m_ctl[CTRLID_BN_VARIABLE_MAGNIFICATION], CST_SHOW);
			}
		}

		// 変倍ボタン有効状態の設定
		{
			DWORD dwState = isButtonEnabled ? CST_ENABLED : CST_DISABLED;
			SetControlState(m_ctl[CTRLID_BN_VARIABLE_MAGNIFICATION], dwState);
		}

		// 変倍値表示フィールド表示状態の設定
		{
			if(isFieldShowFront)
			{
				SetControlState(m_ctl[CTRLID_SC_ICON_FRONT], CST_SHOW);
				SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_FRONT], CST_SHOW);
				SetControlState(m_ctl[CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT], CST_SHOW);
				SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_FRONT], CST_SHOW);
				SetControlState(m_ctl[CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT], CST_SHOW);

				if(isFieldShowBack)
				{
					SetControlState(m_ctl[CTRLID_SC_ICON_BACK], CST_SHOW);
					SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_BACK], CST_SHOW);
					SetControlState(m_ctl[CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK], CST_SHOW);
					SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_BACK], CST_SHOW);
					SetControlState(m_ctl[CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_BACK], CST_SHOW);
				}
			}
		}
	}
}

// コントロール表示値更新イベント
void CCtlVariableMagnification::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		std::string frontX;
		std::string frontY;
		std::string backX;
		std::string backY;

		// 変倍値表示フィールドの更新
		dataIF->onDraw_VariableMagnificationField(frontX, frontY, backX, backY);
		SetControlData(m_ctl[CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT], reinterpret_cast<DWORD>(frontX.c_str()));
		SetControlData(m_ctl[CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT], reinterpret_cast<DWORD>(frontY.c_str()));
		SetControlData(m_ctl[CTRLID_SC_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK], reinterpret_cast<DWORD>(backX.c_str()));
		SetControlData(m_ctl[CTRLID_SC_UP_DOWN_VARIABLE_MAGNIFICATION_BACK], reinterpret_cast<DWORD>(backY.c_str()));
	}
}
