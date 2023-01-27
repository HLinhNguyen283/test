/*! \file CtlPrintingSpeedSelection.cpp
 *  \brief Printing Speed Selection control
 *  \author GCS Tan Ho
 *  \date 2013/05/13 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintingSpeedSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "CommonUiMsg_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "Ini_PrintConditionGUI.h"

#include <sstream>

// ==========================================================================
// Local definitions

// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,				//!< Static-box: Print speed icon
	CTRLID_PD_PRINT_SPEED,			//!< Print speed selection pull-down
	CTRLID_SC_UNIT,					//!< Print speed unit

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlPrintingSpeedSelection::CCtlPrintingSpeedSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPrintingSpeedSelection::~CCtlPrintingSpeedSelection()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPrintingSpeedSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: print speed icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 690, 10, 690 + 60, 10 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINT_SPEED_ICON, RESOURCE_BMP);
	}

	// pull-down: print speed selection
	{
		long ctlId = CTRLID_PD_PRINT_SPEED;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 760, 10, 760 + 135, 10 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}

	// static-box: print speed unix
	{
		long ctlId = CTRLID_SC_UNIT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_PRINT_SPEED_UNIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 900, 20, 900 + 80, 20 + 45);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrintingSpeedSelection::OnCreateItem()
{
	// Registers items for print speed menu pull-down
	RegisterPrintSpeedItems();
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlPrintingSpeedSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_PRINT_SPEED_LIST:
		{
			if(wParam == CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM){
				// Delete all item
				while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_SPEED], 0)){
					SetControlItem(m_ctl[CTRLID_PD_PRINT_SPEED], 0, NULL);
				}

				// Reregister the all items of printing speed
				RegisterPrintSpeedItems();
			}
		}

		break;
	}

	return DEF_NONE;
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPrintingSpeedSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	////////////////
	// サブ関数
	////////////////

	auto checkPrintSpeed = [&](long inPrintSpeed)->void
	{
		////////////////
		// サブ関数
		////////////////

		// inMsg文字列中のinTypeFieldで指定した文字列をinValueの値に置き換える
		auto changeMessage = [](const std::string& inMsg, const std::string& inTypeField, long inValue)->const std::string
		{
			std::string msg = inMsg;
			size_t pos = msg.find(inTypeField);
			if (pos != std::string::npos) {
				msg.replace(pos, 2, std::to_string(static_cast<long long>(inValue)));
			}
			return msg;
		};

		/////////////////
		// 処理
		/////////////////

		if (inPrintSpeed == 15000) {
			// 警告メッセージ（OKボタン）を表示する
			std::stringstream msgBase;
			msgBase << (ID_MESSAGE_ERROR + IDM_MSG_PRINTSPEED_BASE) << "\n" << LoadResourceString(IDM_MSG_PRINTSPEED_BASE, RESOURCE_MSG_STR);
			std::string msg = msgBase.str();

			CIni_PrintConditionGUI ini_PrintConditionGUI;
			ini_PrintConditionGUI.Initialize(TRUE);
			{
				// 坪量の上限適正値
				msg = changeMessage(msg, "%d", ini_PrintConditionGUI.GetHighSpeedProperValueWeightMin()) ;
				// 坪量の下限適正値
				msg = changeMessage(msg, "%d", ini_PrintConditionGUI.GetHighSpeedProperValueWeightMax()) ;
				// テンションの上限適正値
				msg = changeMessage(msg, "%d", ini_PrintConditionGUI.GetHighSpeedProperValueTensionMax()) ;

				CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

				// アンダーコートの適正値
				if (pData->IsExistPrecoater() == true) {
					msg += changeMessage(LoadResourceString(IDM_MSG_PRINTSPEED_UNDERCOAT, RESOURCE_MSG_STR), "%d", ini_PrintConditionGUI.GetHighSpeedProperValueUnderCoat()) ;
				}

				// プロテクターコートの適正値
				if (pData->IsExistOvercoater(1) == true || pData->IsExistOvercoater(2) == true) {
					msg += changeMessage(LoadResourceString(IDM_MSG_PRINTSPEED_PROTECTORCOAT, RESOURCE_MSG_STR), "%d", ini_PrintConditionGUI.GetHighSpeedProperValueProtectorCoat()) ;
				}
			}
			ini_PrintConditionGUI.Finalize();

			ShowMessageBox(const_cast<char*>(msg.c_str()), MBST_ICONWARNING | MBST_OK, NULL);
		}
	};

	/////////////////
	// 処理
	/////////////////

	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case CBN_SELCHANGE:
		{
			// Print speed menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_PRINT_SPEED]){
				long nSelection = GetControlData(m_ctl[CTRLID_PD_PRINT_SPEED]);

				// change the selection print speed
				pData->SetSelectPrintSpeed(nSelection);

				// 印刷速度をチェックして必要であれば警告メッセージを表示する
				checkPrintSpeed(pData->GetSelectPrintSpeedValue());
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPrintingSpeedSelection::OnUpdateState()
{
	if(m_ctl){
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		// Always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}

		// the effective for print mode selection pull-down menu
		DWORD dwState = CST_ENABLED;


		// Disable control when the selected print settings is locked.
		if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
			dwState = CST_DISABLED;
		}

		SetControlState(m_ctl[CTRLID_PD_PRINT_SPEED], dwState);
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrintingSpeedSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print speed pull-down menu
	long nSelection = 0;
	
	if(true == pData->GetSelectPrintSpeed(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_PRINT_SPEED], nSelection);
	}
}

/*! \brief Register all items for Print speed pull-down
*/
void CCtlPrintingSpeedSelection::RegisterPrintSpeedItems()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Get print mode items
	PDITEMINFO pd_item = {0};
	long nItemID = 0;
	const char* strSpeed = NULL;
	while(true == pData->GetPrintSpeedList(nItemID, &strSpeed)){
		pd_item.text = (char*)strSpeed;
		SetControlItem(m_ctl[CTRLID_PD_PRINT_SPEED], nItemID++, &pd_item);
		strSpeed = NULL;
	}
	OnUpdateValue();
}
