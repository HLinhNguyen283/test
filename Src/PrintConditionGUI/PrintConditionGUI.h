/*! \file PrintConditionGUI.h
 *  \brief PrintConditionGUI
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/

#if !defined(__PRINT_CONDITION_GUI__)
#define __PRINT_CONDITION_GUI__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiPlugin_DlgGenSetup.h"

#define PLUGIN_VERSION 0x0000

/*! \brief PrintConditionGUI
*/
class CPrintConditionGUI : public CGuiPlugin_DlgGenSetup
{
public:
	CPrintConditionGUI();
	virtual ~CPrintConditionGUI();

protected:

};

#endif // !defined(__PRINT_CONDITION_GUI__)
