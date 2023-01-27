/*! \file CtlVariableMagnificationCommitClose.cpp
 *  \brief 登録/終了ボタンコントロール
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlVariableMagnificationCommitClose.h"
#include "ResDef.h"
#include "DataIF.h"
#include "PluginCBRProc.h"

// ==========================================================================
// Local definitions
static const long ButtonWidth = 130;		//!< ボタンの幅
static const long HeightWidth = 60;			//!< ボタンの高さ

//! ウィンドウID一覧
enum
{
	CTRLID_BN_COMMIT = 0,		//!< Button: 登録ボタン
	CTRLID_BN_CLOSE,			//!< Button: 終了ボタン

	CTLID_COUNT
};

// ==========================================================================

// コンストラクタ
CCtlVariableMagnificationCommitClose::CCtlVariableMagnificationCommitClose()
{
	m_ctlCount = CTLID_COUNT;
}

// デストラクタ
CCtlVariableMagnificationCommitClose::~CCtlVariableMagnificationCommitClose()
{
}

// コントロール属性設定イベント
void CCtlVariableMagnificationCommitClose::OnSetAttribute()
{
	// button: 登録ボタン
	{
		long ctlId = CTRLID_BN_COMMIT;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_SHOW | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SAVE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 190, 245, 190 + ButtonWidth, 245 + HeightWidth);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_SAVE, RESOURCE_BMP));
	}

	// button: 終了ボタン
	{
		long ctlId = CTRLID_BN_CLOSE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_SHOW | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 340, 245, 340 + ButtonWidth, 245 + HeightWidth);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_EXIT, RESOURCE_BMP));
	}
}

// コントロール操作イベント
long CCtlVariableMagnificationCommitClose::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);
	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam)
	{
	case BN_CLICKED:
		{
			// 登録ボタン
			if(ctlWnd == m_ctl[CTRLID_BN_COMMIT])
			{
				dataIF->onPush_VariableMagnificationCommit();
				return DEF_EXIT_PAGE;
			}
			// 終了ボタン
			else if(ctlWnd == m_ctl[CTRLID_BN_CLOSE])
			{
				if(dataIF->onPush_VariableMagnificationClose())
				{
					char msg[256];
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_DISCARD_CHANGES_CONFIRMATION, LoadResourceString(IDM_DISCARD_CHANGES_CONFIRMATION, RESOURCE_MSG_STR));

					if(IDNO == ShowMessageBox(msg, MBST_YESNO | MBST_ICONINFORMATION, NULL))
					{
						return DEF_NONE;
					}
					else
					{
						return DEF_EXIT_PAGE;
					}
				}
				else
				{
					return DEF_EXIT_PAGE;
				}
			}
			else
			{
				// 何もする必要無し
			}
		}
		break;
	}

	return DEF_NONE;
}

// コントロールステータスイベント
void CCtlVariableMagnificationCommitClose::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		DWORD dwState = 0;
		bool isEnabled = false;
		dataIF->onView_VariableMagnificationCommit(isEnabled);

		//入力値に変更があった場合、「登録ボタン」を有効にする
		if(isEnabled)
		{
			dwState = CST_ENABLED;
		}
		else
		{
			dwState = CST_DISABLED;
		}
		SetControlState(m_ctl[CTRLID_BN_COMMIT], dwState);
	}
}
