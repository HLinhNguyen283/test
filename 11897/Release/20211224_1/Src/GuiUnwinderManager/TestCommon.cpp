/**
	* @file		TestCommon.cpp
	* @brief	�����p���ʒ�` 
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "OutputLog.h"
// ===========================================================================

void WriteToLogBuf(long type, char* pString)
{
	static COutputLog sOutputLog;
	static bool b1st = true;

	if ( b1st )
	{
		b1st = false;
		sOutputLog.Start("LOG\\GuiUnwinderManager");
	}
	sOutputLog.Output(pString);
}

