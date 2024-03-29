/**
 * @file  TestCommon.cpp
 * @brief 共通定義 
 * @author screen
 * @date  20211119 screen 新規作成
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

