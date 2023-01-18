/**
 * @file  TestCommon.cpp
 * @brief ‹¤’Ê’è‹` 
 * @author screen
 * @date  20211119 screen V‹Kì¬
 **/

#include "stdafx.h"
#include "OutputLog.h"


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

