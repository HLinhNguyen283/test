/*! \file CtlJobSetting.cpp
 *  \brief ジョブ設定ボタンコントロール
 *  \author okada
 *  \date 2014/11/21 okada 新規作成
 */

#include "StdAfx.h"
#include "CtlJobSetting.h"
#include "DataIF.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "ResDef.h"
#include "PluginCBRProc.h"
#include "ProcPluginCBRProc.h"

const static char* gst_logmsg_btn_jobsetting = "CTRLID_BN_JOB_SETTING";	//!< ボタン押下時のログ文字列：ジョブ設定ボタン

//! the control id list
enum{
	CTRLID_BN_JOB_SETTING = 0,				//!< basic setup button

	CTLID_COUNT
};

 /*!
  * \brief コンストラクタ
  */
 CCtlJobSetting::CCtlJobSetting()
 {
 	m_ctlCount = CTLID_COUNT;
}

 /*!
  * \brief デストラクタ
  */
 CCtlJobSetting::~CCtlJobSetting()
 {
 }

/*! \brief コントロールステータス更新イベント
*/
void CCtlJobSetting::OnUpdateState()
{
	if (!m_ctl) {
		WriteToLogBuf(LOG_DEBUG, "CCtlJobSetting::OnUpdateState failed [control HWND NULL].");
		return;
	}

	CDataIF* dataIF  = dynamic_cast<CDataIF*>(m_data);
	if (!dataIF) {
		WriteToLogBuf(LOG_DEBUG, "CCtlJobSetting::OnUpdateState failed [DataIF NULL].");
		return;
	}

	DWORD dwState = CST_ENABLED;

	if (dataIF->IsIpdsMode()){
		// IPDSモードのときはJI有効のときのみ表示
		if(dataIF->IsSystemSupportJetInspection()) {
			dwState |= CST_SHOW;
		}else{
			dwState |= CST_HIDE;
		}
	}else {
		// EQモードのときは、システムの印刷条件表示時のみ表示
		if (dataIF->IsStartFromSystemSetting()) {
			dwState |= CST_SHOW;
		} else {
			dwState |= CST_HIDE;
		}
	}

	// Disable control when one of the next conditions met
	// - There is an item which is set (Do not allow to edit Job setting screen without saving.)
	if(true == dataIF->IsAbleSavePrintSetting())
	{
		dwState |= CST_DISABLED;
	} else {
		dwState |= CST_ENABLED;
	}

	SetControlState(m_ctl[CTRLID_BN_JOB_SETTING], dwState);

}

/*! \brief [event]the control operated
 *	\param hWnd [IN]window handle
 *  \param message [IN]messag id
 *  \param wParam [IN]parameter 1
 *  \param lParam [IN]parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlJobSetting::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = (HWND)lParam;

	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);

	if (m_ctl && dataIF) {
		switch (wParam) {
			case BN_CLICKED: {
				if (ctlWnd == m_ctl[CTRLID_BN_JOB_SETTING]){
					// ジョブ設定開始を通知.
					if (!dataIF->StartJobSetting()) {
						WriteToLogBuf(LOG_DEBUG, "StartJobSetting failed. [CTRLID_BN_JOB_SETTING]");
					}
				}
	
				break;
			}
		}
	}
	return DEF_NONE;
}

/*! \brief [event]set the control's property
*/
void CCtlJobSetting::OnSetAttribute()
{
	// button： Basic setup
	{
		long ctlId = CTRLID_BN_JOB_SETTING;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_IMAGE_BLEND | BNST_BITMAP | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_BASIC_SETUP, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 780, 610, 780 + 160, 610 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_BASIC_SETUP, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = gst_logmsg_btn_jobsetting;
	}
}