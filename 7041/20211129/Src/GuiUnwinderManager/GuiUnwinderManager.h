/**
 * @file  GuiUnwinderManager.h
 * @brief TP-UWインターフェース確認用アプリメイン ヘッダーファイル
 * @details (TPとUWのインターフェース確認用アプリ)
 * @author screen
 * @date  20211119 screen 新規作成
 **/

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

// ===========================================================================
// Include files
#include "resource.h"		// メイン シンボル
// ===========================================================================

/**
 * @brief TP-UWインターフェース確認用アプリメイン
*/
class CJD_UW_Test_TOOLApp : public CWinApp
{
public:
	CJD_UW_Test_TOOLApp();
	virtual BOOL InitInstance();

// 実装
public:
	DECLARE_MESSAGE_MAP()
};

extern CJD_UW_Test_TOOLApp theApp;