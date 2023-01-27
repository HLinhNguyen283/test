/*! \file CtlVariableMagnificationInput.cpp
 *  \brief 変倍入力コントロール
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlVariableMagnificationInput.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PluginCBRProc.h"
#include "CommonUiMsg.h"

// ==========================================================================
// Local definitions
static const long EditBoxMaxLength = 7;		//!< エディットボックスの最大桁数
static const long EditBoxHeight = 62;		//!< エディットボックスの高さ
static const long EditBoxWidth = 133;		//!< エディットボックスの幅
static const std::string Percent = "%";		//!< "%"文字
static const long PercentSize = 50;			//!< "%"文字サイズ

//! ウィンドウID一覧
enum
{
	CTRLID_SC_ICON_FRONT,										//!< static-box: プリンタアイコン(表面)
	CTRLID_SC_ICON_LEFT_RIGH_FRONT,								//!< static-box: 左右アイコン(表面)
	CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT,			//!< edit-box: 左右変倍値(表面/数値)
	CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_FRONT,	//!< edit-box: 左右変倍値(表面/"---")
	CTRLID_SC_ICON_UP_DOWN_FRONT,								//!< static-box: 上下アイコン(表面)
	CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT,				//!< Edit-box: 上下変倍値(表面/数値)
	CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_FRONT,		//!< Edit-box: 上下変倍値(表面/"---")
	CTRLID_SC_PERCENT_FRONT,									//!< static-box: "%"文字(表面)

	CTRLID_SC_ICON_BACK,										//!< static-box: プリンタアイコン(裏面)
	CTRLID_SC_ICON_LEFT_RIGH_BACK,								//!< static-box: 左右アイコン(裏面)
	CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK,			//!< edit-box: 左右変倍値(裏面/数値)
	CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_BACK,	//!< edit-box: 左右変倍値(裏面/"---")
	CTRLID_SC_ICON_UP_DOWN_BACK,								//!< static-box: 上下アイコン(裏面)
	CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK,				//!< Edit-box: 上下変倍値(裏面/数値)
	CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_BACK,		//!< Edit-box: 上下変倍値(裏面/"---")
	CTRLID_SC_PERCENT_BACK,										//!< static-box: "%"文字(裏面)

	CTLID_COUNT
};

// ==========================================================================

// コンストラクタ
CCtlVariableMagnificationInput::CCtlVariableMagnificationInput()
{
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlVariableMagnificationInput::~CCtlVariableMagnificationInput()
{
}

// コントロール属性設定イベント
void CCtlVariableMagnificationInput::OnSetAttribute()
{
	//!< static-box: プリンタアイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 100, 30 + 40, 100 + 40);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_FRONT, RESOURCE_BMP));
	}

	//!< static-box: 左右アイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_LEFT_RIGH_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 70, 105, 70 + 30, 105 + 30);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_LEFT_RIGHT, RESOURCE_BMP));
	}

	//!< edit-box: 左右変倍値(表面/数値)
	{
		long ctlId = CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 105, 90, 105 + EditBoxWidth, 90 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< edit-box: 左右変倍値(表面//"---")
	{
		long ctlId = CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_FRONT;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 105, 90, 105 + EditBoxWidth, 90 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< static-box: 左右アイコン(表面)
	{
		long ctlId = CTRLID_SC_ICON_UP_DOWN_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 105, 250 + 30, 105 + 30);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_UP_DOWN, RESOURCE_BMP));
	}

	//!< edit-box: 上下変倍値(表面/数値)
	{
		long ctlId = CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 290, 90, 290 + EditBoxWidth, 90 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< edit-box: 上下変倍値(表面/"---")
	{
		long ctlId = CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_FRONT;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 290, 90, 290 + EditBoxWidth, 90 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< static-box: "%"文字(表面)
	{
		long ctlId = CTRLID_SC_PERCENT_FRONT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = Percent.c_str();
		SetRect(&m_ctlAttribute[ctlId].rect, 440, 100, 440 + PercentSize, 100 + PercentSize);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//!< static-box: プリンタアイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 175, 30 + 40, 175 + 40);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_ICON_SIDE_BACK, RESOURCE_BMP));
	}

	//!< static-box: 左右アイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_LEFT_RIGH_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 70, 180, 70 + 30, 180 + 30);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_LEFT_RIGHT, RESOURCE_BMP));
	}

	//!< edit-box: 左右変倍値(裏面/数値)
	{
		long ctlId = CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 105, 165, 105 + EditBoxWidth, 165 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< edit-box: 左右変倍値(裏面/"---")
	{
		long ctlId = CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_BACK;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 105, 165, 105 + EditBoxWidth, 165 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< static-box: 左右アイコン(裏面)
	{
		long ctlId = CTRLID_SC_ICON_UP_DOWN_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 180, 250 + 30, 180 + 30);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_UP_DOWN, RESOURCE_BMP));
	}

	//!< edit-box: 上下変倍値(裏面/数値)
	{
		long ctlId = CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 290, 165, 290 + EditBoxWidth, 165 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< edit-box: 上下変倍値(裏面/"---")
	{
		long ctlId = CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_BACK;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = nullptr;
		SetRect(&m_ctlAttribute[ctlId].rect, 290, 165, 290 + EditBoxWidth, 165 + EditBoxHeight);
		m_ctlAttribute[ctlId].param = EditBoxMaxLength;
	}

	//!< static-box: "%"文字(裏面)
	{
		long ctlId = CTRLID_SC_PERCENT_BACK;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_TEXT;
		m_ctlAttribute[ctlId].text = Percent.c_str();
		SetRect(&m_ctlAttribute[ctlId].rect, 440, 175, 440 + PercentSize, 175 + PercentSize);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

// コントロール操作イベント
long CCtlVariableMagnificationInput::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);

	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			long long frontX;
			long long frontY;
			long long backX;
			long long backY;
			dataIF->getVariableMagnificationInput(frontX, frontY, backX, backY);

			if(ctlWnd == m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT])
			{
				// 左右変倍値(表面)
				char* frontXStr = reinterpret_cast<char*>(GetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT]));
				frontX = static_cast<long long>(atof(frontXStr) * 1000);
			}
			else if(ctlWnd == m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT])
			{
				// 上下変倍値(表面)
				char* frontYStr = reinterpret_cast<char*>(GetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT]));
				frontY = static_cast<long long>(atof(frontYStr) * 1000);
			}
			else if(ctlWnd == m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK])
			{
				// 左右変倍値(裏面)
				char* backXStr = reinterpret_cast<char*>(GetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK]));
				backX = static_cast<long long>(atof(backXStr) * 1000);
			}
			else if(ctlWnd == m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK])
			{
				// 上下変倍値(裏面)
				char* backYStr = reinterpret_cast<char*>(GetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK]));
				backY = static_cast<long long>(atof(backYStr) * 1000);
			}
			else
			{
				// 何もする必要無し
			}

			dataIF->onPush_VariableMagnificationInput(frontX, frontY, backX, backY);

			break;
		}
	}

	return DEF_NONE;
}

// コントロールステータスイベント
void CCtlVariableMagnificationInput::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isFrontShow = false;
		bool isBackShow = false;
		bool isEnabled = false;

		dataIF->onView_VariableMagnificationInput(isFrontShow, isBackShow, isEnabled);
		DWORD frontShow = isFrontShow ? CST_SHOW : CST_HIDE;
		DWORD backShow = isBackShow ? CST_SHOW : CST_HIDE;
		if(isEnabled)
		{
			// 数値用設定
			SetControlState(m_ctl[CTRLID_SC_ICON_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_FRONT], frontShow | CST_ENABLED);	
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_PERCENT_FRONT], frontShow | CST_ENABLED);

			SetControlState(m_ctl[CTRLID_SC_ICON_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_BACK], backShow | CST_ENABLED);	
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_PERCENT_BACK], backShow | CST_ENABLED);

			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], CST_HIDE);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], CST_HIDE);
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_BACK], CST_HIDE);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_BACK], CST_HIDE);
		}
		else
		{
			// "---"用設定
			SetControlState(m_ctl[CTRLID_SC_ICON_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_FRONT], frontShow | CST_ENABLED);	
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT], CST_HIDE);
			SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_FRONT], frontShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT], CST_HIDE);
			SetControlState(m_ctl[CTRLID_SC_PERCENT_FRONT], frontShow | CST_ENABLED);

			SetControlState(m_ctl[CTRLID_SC_ICON_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_SC_ICON_LEFT_RIGH_BACK], backShow | CST_ENABLED);	
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK], CST_HIDE);
			SetControlState(m_ctl[CTRLID_SC_ICON_UP_DOWN_BACK], backShow | CST_ENABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK], CST_HIDE);
			SetControlState(m_ctl[CTRLID_SC_PERCENT_BACK], backShow | CST_ENABLED);

			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], frontShow | CST_DISABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], frontShow | CST_DISABLED);
			SetControlState(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_BACK], backShow | CST_DISABLED);
			SetControlState(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_BACK], backShow | CST_DISABLED);
		}
	}
}

// コントロール表示値更新イベント
void CCtlVariableMagnificationInput::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		std::string frontX;
		std::string frontY;
		std::string backX;
		std::string backY;
		bool isEditRipVariableMagnification = false;

		dataIF->onDraw_VariableMagnificationInput(frontX, frontY, backX, backY, isEditRipVariableMagnification);
		// 変倍値の更新
		if(isEditRipVariableMagnification)
		{
			SetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_FRONT], reinterpret_cast<DWORD>(frontX.c_str()));
			SetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_FRONT], reinterpret_cast<DWORD>(frontY.c_str()));
			SetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_BACK], reinterpret_cast<DWORD>(backX.c_str()));
			SetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_BACK], reinterpret_cast<DWORD>(backY.c_str()));
		}
		else
		{
			SetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], reinterpret_cast<DWORD>(frontX.c_str()));
			SetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_FRONT], reinterpret_cast<DWORD>(frontY.c_str()));
			SetControlData(m_ctl[CTRLID_EB_LEFT_RIGHT_VARIABLE_MAGNIFICATION_HYPHEN_BACK], reinterpret_cast<DWORD>(backX.c_str()));
			SetControlData(m_ctl[CTRLID_EB_UP_DOWN_VARIABLE_MAGNIFICATION_HYPHEN_BACK], reinterpret_cast<DWORD>(backY.c_str()));
		}
	}
}
