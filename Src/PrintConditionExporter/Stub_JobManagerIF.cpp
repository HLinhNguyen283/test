/**
 * @file	Stub_JobManagerIF.cpp
 * @brief	stub of JobManager plugin
**/

#include "stdafx.h"
#include "JobManager_Callbacks.h"
#include "Stub_JobManagerIF.h"
#include "CommonCBRProc_OP.h"

//! I/F構造体名
#define CBR SJobManager_Callbacks

//! I/Fゲッター名
#define GetCallbacks JM_GetCallbacks

// フラッシングページの詳細情報一覧を取得する
bool Stub_JobManagerIF::GetDetailedFlushingPageList(std::vector<flushing_page_list_ini::SItem>& outItems)
{
	CBR callbacks;
	if (!GetCallbacks(&callbacks))
	{
		return false;
	}

	return callbacks.JM_GetDetailedFlushingPageList(outItems);
}
