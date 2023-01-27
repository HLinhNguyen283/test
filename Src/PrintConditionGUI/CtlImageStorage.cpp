/*! \file CtlImageStorage.cpp
 *  \brief CtlImageStorage.cppの処理
 *  \author omori
 *  \date 2019/02/28 omori 新規作成
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "CtlImageStorage.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

//! the control id list
enum{
	CTRLID_BN_IMAGE_STORAGE = 0, //!< ボタン：IMSの有効無効切替

	CTLID_COUNT
};


/**
	@brief	constructor
*/
CCtlImageStorage::CCtlImageStorage()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}


/**
	@brief	destructor
*/
CCtlImageStorage::~CCtlImageStorage()
{
}

void CCtlImageStorage::OnSetAttribute()
{

	// ボタン
	{
		long ctlId = CTRLID_BN_IMAGE_STORAGE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_IMAGE_STORAGE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 590, 605, 590 + 70, 605 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_IMAGE_STOAGE, RESOURCE_BMP);
	}
}


long CCtlImageStorage::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);

	if(BN_CLICKED == wParam)
	{
		if(ctlWnd == m_ctl[CTRLID_BN_IMAGE_STORAGE])
		{
			CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
			dataIF->SetImageStorage(GetControlData(m_ctl[CTRLID_BN_IMAGE_STORAGE]));
		}
	}
	return DEF_NONE;
}

void CCtlImageStorage::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isShow = false;
		bool isEnabled = false;

		// データ部から表示状態と有効状態を取得する
		dataIF->onView_ImageStorage(isShow, isEnabled);

		// 表示状態の設定
		{
			if(isShow)
			{
				SetControlState(m_ctl[CTRLID_BN_IMAGE_STORAGE], CST_SHOW);
			}
		}

		// 有効状態の設定
		{
			DWORD dwState = isEnabled ? CST_ENABLED : CST_DISABLED;
			SetControlState(m_ctl[CTRLID_BN_IMAGE_STORAGE], dwState);
		}
	}
}

void CCtlImageStorage::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
		bool isOn = dataIF->IsImageStorage();
		BNITEMINFO bn_item = {0};
		bn_item.bLED_On = isOn;
		SetControlData(m_ctl[CTRLID_BN_IMAGE_STORAGE], isOn);
		SetControlItem(m_ctl[CTRLID_BN_IMAGE_STORAGE], 0, &bn_item);
	}
}