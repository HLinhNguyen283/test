/*! \file	PrintConditionGUI_OP_IPDS.h
 *  \brief	PrintConditionGUI_OP
 *  \author	omoto
 *  \date	2017/02/22 omoto new
*/

#if !defined(__PRINT_CONDITION_GUI_OP_IPDS__)
#define __PRINT_CONDITION_GUI_OP_IPDS__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrintConditionGUI_OP.h"

#define PLUGIN_VERSION 0x0000

/*! \brief PrintConditionGUI
*/
class CPrintConditionGUI_OP_IPDS : public CPrintConditionGUI_OP
{
public:
	CPrintConditionGUI_OP_IPDS();
	virtual ~CPrintConditionGUI_OP_IPDS();

protected:
	virtual void SetSystemCallBackFunc(HOST inHostInfo);

};

#endif // !defined(__PRINT_CONDITION_GUI_OP_IPDS__)
