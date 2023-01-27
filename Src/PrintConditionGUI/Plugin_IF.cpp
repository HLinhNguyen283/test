/*! \file Plugin_IF.cpp
 *  \brief 機種ごとのI/F
 *  \author Sugita
 *  \date 2013/04/25 Sugita 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "Common.h"
#include "Plugin_IF.h"
#include "PluginCommon.h"
#include "PrintConditionGUI_OP.h"
#include "PrintConditionGUI_OP_IPDS.h"
#include "Ini_PrinterDescriptor.h"
#include "PaperDBDef_OP.h"

// ==========================================================================
// Local values
static CPrintConditionGUI_OP* st_PrintConditionGUIObj = NULL;

// ==========================================================================
// Local definitions


// ==========================================================================
// Local functions

// ==========================================================================


/*! \brief モジュールクラスオブジェクト作成
 *  
 *  \param[in] pHost ホスト情報へのポインタ
 *  \param[out] moduleObject モジュールクラスポインタ
 *  \retval TRUE 成功
 *  \retval FALSE 失敗
*/
BOOL CreateModuleObject(PHOST pHost, void **moduleObject)
{	
	if(!pHost || !moduleObject){
		return FALSE;
	}

	// 設定取得
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	bool isIpdsMode = (TRUE == iniPrinterDescriptor.IsIpdsMode());
	iniPrinterDescriptor.Finalize();

	if (isIpdsMode) {
		st_PrintConditionGUIObj = new CPrintConditionGUI_OP_IPDS();
	}
	else {
		st_PrintConditionGUIObj = new CPrintConditionGUI_OP();
	}

	*moduleObject = dynamic_cast<CPlugin_Common*>(st_PrintConditionGUIObj);

	return (*moduleObject != NULL);
}

/*! \brief Show dialog of Print Condition GUI
 * \retval TRUE/FALSE: success/fail
 */
PLUGIN_MODULE_API BOOL _ShowDialogForMaintenance()
{
	if (NULL == st_PrintConditionGUIObj) {
		return FALSE;
	}
	return st_PrintConditionGUIObj->ShowDialogForMaintenance();
}

/**
 *	Set current dotgain curve value to PrintConditionGUI
 *	@param[in]	inSystem : input eComSystemType
 *	@param[in]	inDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
PLUGIN_MODULE_API bool _SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	if (NULL == st_PrintConditionGUIObj) {
		return false;
	}
	return st_PrintConditionGUIObj->SetCurrentDotGainCurve(inSystem, inDotgainCurve);
}

/**
 *	Get current dotGain curve value from PrintconditionGUI
 *	@param[in]	inSystem : eComSystemType value
 *	@param[out]	outDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
PLUGIN_MODULE_API bool _GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	if (NULL == st_PrintConditionGUIObj) {
		return false;
	}
	return st_PrintConditionGUIObj->GetCurrentDotGainCurve(inSystem, outDotgainCurve);
}

//Create I/F for outer plugins if necessary.