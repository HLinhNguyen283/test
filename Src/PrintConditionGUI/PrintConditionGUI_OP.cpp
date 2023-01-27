/*! \file PrintConditionGUI_OP.cpp
 *  \brief PrintConditionGUI
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintConditionGUI_OP.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "CommonDef_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "PageMain.h"
#include "DataIF.h"
#include "PrinterIF.h"
#include "DlgPageMain.h"

// ==========================================================================

/*! \brief constructor.
*/
CPrintConditionGUI_OP::CPrintConditionGUI_OP()
{
	m_size.cx = 1245;
	m_size.cy = 875;

}

/*! \brief destructor.
*/
CPrintConditionGUI_OP::~CPrintConditionGUI_OP()
{

}


/*! \brief UI_MODULE_INFOの構築
 *  
 *  プラグインモジュール構成情報をセットする。
 *  \param[out] pModData モジュール構成情報へのポインタ
 *  \return PLUGERR_NONE
*/
long CPrintConditionGUI_OP::BuildUiModuleInfo(MODULE_INFO *pModData)
{	
	pModData->MODULE_ID = MOD_PRINT_CONDITION;				// 情報ブロックの識別子
	pModData->MODULE_VERSION = PLUGIN_VERSION;				// 同一MODULE_IDのバージョン番号
	pModData->MODULE_TYPE = PIM_UI;							// モジュールの種類。PIM_INPUT,PIM_PROCESS,PIM_OUTPUT,PIM_UI
	pModData->REQUIRE_CRITSEC = FALSE;						// 管理モジュールによる資源管理が必要な場合TRUE
	pModData->INSTALL_SERVICE = FALSE;						// サービス機能要求用。TRUE:要求,FALSE:要求しない
	pModData->SERVICE_ADDRESS = NULL;						// サービス実行用関数のアドレス
	pModData->UI_TYPE = UI_JOB_SELECT;						// UI_NONE:GUIなし,それ以外はUIあり.

	_snprintf(pModData->MODULE_NAME, sizeof(pModData->MODULE_NAME), "%s", LoadResourceString(IDS_PAGE_NAME, RESOURCE_STR));//基本GUIの一般設定の一覧に表示される文字列


	return PLUGERR_NONE;
}

/*! \brief メインページクラスインスタンスを作成する
 *  \retval TRUE 成功
 *  \retval FALSE 失敗
 */
BOOL CPrintConditionGUI_OP::CreateMainPage()
{
	if(!m_page){
		CPageMain* page = new CPageMain();
		m_page = dynamic_cast<CBasePage*>(page);
	}

	return m_page != NULL;
}

/*! \brief データI/Fクラスインスタンスを作成する
 *  \retval TRUE 成功
 *  \retval FALSE 失敗
 */
BOOL CPrintConditionGUI_OP::CreateDataIF()
{
	if(!m_data){
		CDataIF* data = new CDataIF(this);
		m_data = dynamic_cast<CBaseDataIF*>(data);
	}

	return m_data != NULL;
}

/*! \brief プリンタI/Fクラスインスタンスを作成する
 *  \retval TRUE 成功
 *  \retval FALSE 失敗
 */
BOOL CPrintConditionGUI_OP::CreatePrinterIF()
{
	if(!m_printer){
		CPrinterIF* printer = new CPrinterIF();
		m_printer = dynamic_cast<CBasePrinterIF*>(printer);
	}

	return m_printer != NULL;
}

/*! \brief システム毎のコールバック関数を設定する.
 *  \param[in] inHostInfo ホスト情報
 */
void CPrintConditionGUI_OP::SetSystemCallBackFunc(HOST inHostInfo)
{
	SetCommonCallBackFunc_OP(inHostInfo);
}

/*! \brief Get Host info
 *  \retval the host info
 */
PHOST CPrintConditionGUI_OP::GetHost()
{
	return &m_inHostInfo;
}

/*! \brief Show dialog of Print Condition GUI
* \retval TRUE/FALSE: success/fail
*/
BOOL CPrintConditionGUI_OP::ShowDialogForMaintenance()
{
	CDataIF pData(this, true);
	CPrinterIF pPrinter;

	CBaseDataIF* baseData = dynamic_cast<CBaseDataIF*>(&pData);
	CBasePrinterIF* basePrinter = dynamic_cast<CBasePrinterIF*>(&pPrinter);

	//initialize data
	if (FALSE == pData.Initialize()
		|| FALSE == pPrinter.Initialize()) {
			return FALSE;
	}

	//set interface
	pPrinter.SetInterface(baseData);
	pData.SetInterface(basePrinter);

	//initialize UI
	pData.UI_Initialize();
	pPrinter.UI_Initialize();

	CDlgPageMain page;
	page.Create(baseData, basePrinter);

	//finalize
	pPrinter.UI_Finalize();
	pData.UI_Finalize();

	pPrinter.Finalize();
	pData.Finalize();

	return TRUE;
}

/**
 *	Set current dotgain curve value to PrintConditionGUI
 *	@param[in]	inSystem : input eComSystemType
 *	@param[in]	inDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CPrintConditionGUI_OP::SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	if (m_data) {
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		pData->SetCurrentDotGainCurve(inSystem, inDotgainCurve);
		return true;
	}
	return false;
}

/**
 *	Get current dotGain curve value from PrintconditionGUI
 *	@param[in]	inSystem : eComSystemType value
 *	@param[out]	outDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CPrintConditionGUI_OP::GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	if (m_data) {
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		return pData->GetCurrentDotGainCurve(inSystem, outDotgainCurve);
	}
	return false;
}
