/*! \file PrintConditionGUI_OP.h
 *  \brief PrintConditionGUI
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/


#if !defined(__PRINT_CONDITION_GUI_OP__)
#define __PRINT_CONDITION_GUI_OP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiPlugin_DlgGenSetup.h"
#include "PrintConditionGUI.h"
#include "CommonDef_OP.h"
#include "PaperDBDef_OP.h"

#define PLUGIN_VERSION 0x0000

/*! \brief PrintConditionGUI
*/
class CPrintConditionGUI_OP : public CPrintConditionGUI
{
public:
	CPrintConditionGUI_OP();
	virtual ~CPrintConditionGUI_OP();

	PHOST GetHost();
	BOOL ShowDialogForMaintenance();
	virtual bool SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);
	virtual bool GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve);
protected:
	virtual long BuildUiModuleInfo(MODULE_INFO *pModData);
	virtual void SetSystemCallBackFunc(HOST inHostInfo);
	virtual BOOL CreateMainPage();
	virtual BOOL CreateDataIF();
	virtual BOOL CreatePrinterIF();

};

#endif // !defined(__PRINT_CONDITION_GUI_OP__)
