/*! \file CtlGpMain.cpp
 *  \brief Main group-box
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlGpMain.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "PluginCBRProc.h"
#include "ResDef.h"

// ==========================================================================
// Local definitions

// ==========================================================================

/*! \brief コンストラクタ
*/
CCtlGpMain::CCtlGpMain()
{
	//このクラスで作成するコントロール数をセット
	m_ctlCount = CTRLID_GB_MAIN_COUNT;
}

/*! \brief デストラクタ
*/
CCtlGpMain::~CCtlGpMain()
{
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlGpMain::OnSetAttribute()
{
	//作成するコントロールの属性をm_ctlAttribute[0]～[m_ctlCount-1]に登録する

	// GroupBox: Main group
	{
		long ctlId = CTRLID_GB_MAIN;
		m_ctlAttribute[ctlId].type = CT_GROUPBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | GBST_SUB;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 90, 30 + 1185, 90 + 695);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlGpMain::OnCreateItem()
{
	if(m_ctl){
		// set a dark gray color to main group-box control
		SetControlColor(m_ctl[CTRLID_GB_MAIN], CC_FACE, RGB(169, 169, 169));
	}
}