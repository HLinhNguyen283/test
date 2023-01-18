/**
 * @file  JD_UW_Test_TOOL.cpp
 * @brief TP-UWインターフェース確認用アプリメイン 実装ファイル
 * @details (TPとUWのインターフェース確認用アプリ)
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
#include "GetTime.h"
#include "WriteDebugLog.h"
#include "JD_UW_Test_TOOL.h"
#include "JD_UW_Test_TOOLDlg.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CJD_UW_Test_TOOLApp

BEGIN_MESSAGE_MAP(CJD_UW_Test_TOOLApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLApp コンストラクション
	* @retval FALSE(固定)
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLApp::CJD_UW_Test_TOOLApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CJD_UW_Test_TOOLApp オブジェクトです。
CJD_UW_Test_TOOLApp theApp;

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLApp 初期化処理
	* @retval FALSE(固定)
**/
/* ==================================================================================================== */
BOOL CJD_UW_Test_TOOLApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	CString FileName(_T("JD_UW_Test_TOOL_"));
	FileName.Append(CGetTime::GetStrDate() + _T(".txt"));

	CWriteDebugLog sWriteDebugLog;
	sWriteDebugLog.Initialize(FileName);
	sWriteDebugLog.WriteDebuglog(_T("TP-UW インターフェース確認 開始"), CWriteDebugLog::E_LOG_LEVEL_1);

	CJD_UW_Test_TOOLDlg dlg(&sWriteDebugLog);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}

	sWriteDebugLog.WriteDebuglog(_T("TP-UW インターフェース確認 終了"), CWriteDebugLog::E_LOG_LEVEL_1);

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

