/**
	* @file		CallUnwinderRequest.cpp
	* @brief	Unwinder要求窓口(★★★サンプル★★★）
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <psapi.h>
#include <string>
#include "Ini_PrinterDescriptor.h"
#include "Ini_UnwinderManager.h"
#include "RequestUnwinder.h"
#include "MakeComposeUnwinderData.h"
#include "ReceiveSignalStatus.h"
#include "CallUnwinderRequest.h"
// ==========================================================================

namespace unwinder_manager
{

/**
	* @brief  Unwionder処理の有効/無効を取得する
	* @retval TRUE：Unwionder処理有効
	* @retval FALSE：Unwionder処理無効
**/
BOOL CCallUnwinderRequest::IsUnwinderFunction()
{
	Ini_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	return iniPrinterDescriptor.getUnwinderOption();
}

/**
	* @brief  Unwionder処理無効の場合の処理
	* @retval 無効の文字列(仮)
**/
std::string CCallUnwinderRequest::NotSupportedUnwinderFunction()
{
	// 仮作成(未サポートの場合にログを出力する等の場合はこの部分で行う)
	return "Not supported unwinder function.";
}

/**
	* @brief  UWに状態監視用チャネル登録を要求するサンプル
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallQueryStatus(CWnd* pWnd)
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryStatus] Start");
		// UWに状態監視用チャネル登録を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestQueryStatus();
		if ( Result.find(SuccessLabel) != std::string::npos )
		{	// 正常
			SetTimerStatusReceive(pWnd);
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryStatus] End SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryStatus] End ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryStatus] End Exception");
		return "Exception CallQueryStatus";
	}
}

/**
	* @brief  UWに用紙情報通知用チャネル登録を要求するサンプル
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallQueryResource(CWnd* pWnd)
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryResource] Start");
		// UWに用紙情報通知用チャネル登録を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestQueryResource();
		if ( Result.find(SuccessLabel) != std::string::npos )
		{	// 正常
			SetTimerPaperReceive(pWnd);
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryResource] End SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryResource] End ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallQueryResource] End Exception");
		return "Exception CallQueryResource";
	}
}

/**
	* @brief  UWにチャネル削除を要求するサンプル
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	* @param  inStopType : [i]チャネル種別
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallStopPersChParams(CWnd* pWnd,int InType)
{
	const int nQueryResource = 1;
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallStopPersChParams] Start");
		int StopType = CMakeComposeUnwinderData::E_Compose_QueryStatus;
		if ( InType == nQueryResource )
		{
			StopType = CMakeComposeUnwinderData::E_Compose_QueryResource;
		}

		// 削除するチャネル種別を引数にしてUWにチャネル削除を要求する
		// ・状態監視用チャネルを削除する場合は、CMakeComposeUnwinderData::E_Compose_QueryStatus
		// ・用紙情報通知用チャネルを削除する場合は、CMakeComposeUnwinderData::E_Compose_QueryResource
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestStopPersChParams(StopType);
		if ( Result.find(SuccessLabel) != std::string::npos )
		{	// 正常
			if ( StopType == CMakeComposeUnwinderData::E_Compose_QueryStatus )
			{
				KillTimerStatusReceive(pWnd);
			} else {
				KillTimerPaperReceive(pWnd);
			}

			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallStopPersChParams] End SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallStopPersChParams] End ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallStopPersChParams] End Exception");
		return "Exception CallStopPersChParams";
	}
}

/**
	* @brief  UWに印刷条件設定を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallCommandResource()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandResource] Start");
		// 何らかの方法で印刷条件を取得する
		CUnwinderPaper sUnwinderPaper;
		sUnwinderPaper.DescriptiveName = "Next IJ #1";
		sUnwinderPaper.MediaType = "NextIJ";
		sUnwinderPaper.Dimension = "520.00 305.00";
		sUnwinderPaper.MaxRunSpeed = "120";
		sUnwinderPaper.Thickness = "130";
		sUnwinderPaper.UWTension = "5";
		sUnwinderPaper.ExternalID = "Ex001";

		// 印刷条件を引数にしてUWに印刷条件設定を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestCommandResource(&sUnwinderPaper);
		if ( Result.find(SuccessLabel) != std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandResource] End SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandResource] End ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandResource] End Exception");
		return "Exception CallCommandResource";
	}
}

/**
	* @brief  UWにREADY状態への遷移を要求するサンプル
	* @retval 便宜上結果を文字列で戻している(仮)
**/
std::string CCallUnwinderRequest::CallCommandWakeup()
{
	try
	{
		if ( !IsUnwinderFunction() )
		{	// Unwinder 未サポート
			return NotSupportedUnwinderFunction();
		}

		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandWakeup] Start");
		// UWにREADY状態への遷移を要求する
		CRequestUnwinder sRequestUnwinder;
		std::string Result = sRequestUnwinder.RequestCommandWakeup();
		if ( Result.find(SuccessLabel) != std::string::npos )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandWakeup] End SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandWakeup] End ERROR");
		}

		return Result;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallCommandWakeup] End Exception");
		return "Exception CallCommandWakeup";
	}
}

/**
	* @brief  UWからSignalStatusを受信するサンプル
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
**/
void CCallUnwinderRequest::CallSignalStatus(CWnd* pWnd)
{
	CReceiveSignalStatus sReceiveSignalStatus(pWnd);
	while(1)
	{
		sReceiveSignalStatus.MainLoop();
		// UWandRW_Receiverが終了した場合Pipe読み出しでエラーが発生して終了するので、一定時間経過後に再呼び出しする
		Sleep(3000);
	}
}

/**
	* @brief  プロセスの状態をチェックする
	* @param  InProcessName  : [i]プロセス名称
**/
void CCallUnwinderRequest::CallProcessCheck( const std::string& InProcessName )
{
	if ( ProcessRunningCheck(InProcessName) == FALSE )
	{	// プロセスが存在しない
		WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::CallProcessCheck] Process not running ERROR");
		if ( ExecuteProcess(InProcessName) )
		{	// 正常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::ExecuteProcess] SUCCESS");
		}
		else
		{	// 異常
			WriteToLogBuf(LOG_DEBUG,"[CCallUnwinderRequest::ExecuteProcess] ERROR");
		}
	}
}

/**
	* @brief  プロセスが存在するかチェックする
	* @param  InProcessName  : [i]プロセス名称
**/
BOOL CCallUnwinderRequest::ProcessRunningCheck( const std::string& InProcessName )
{
	BOOL nRet = FALSE;

	DWORD ProcessID[ 1024 ];
	DWORD dwSize;
    // プロセス識別子の取得
    EnumProcesses( ProcessID, sizeof(ProcessID), &dwSize );
    DWORD dwMax = (dwSize / sizeof(DWORD));
    // プロセス識別子からプロセス名を列挙
	for ( DWORD dwNow = 0 ; dwNow < dwMax && !nRet; dwNow++ )
	{
		TCHAR   szFile[ 1024 ] = { 0 };
		HMODULE Module[ 1024 ] = { 0 };
        
		// プロセスのフルパス名を取得
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcessID[dwNow]);
		if ( hProcess != NULL )
		{
			if ( EnumProcessModules(hProcess,Module,sizeof(Module),&dwSize) )
			{
				GetModuleFileNameEx( hProcess, Module[0], szFile, sizeof(szFile) );
				std::string s(szFile);
				if ( s.find(InProcessName) != std::string::npos )
				{
					nRet = TRUE;
				}
            }
            CloseHandle( hProcess );
        }
    }
	return nRet;
}

/**
	* @brief  プロセスを起動する
	* @param  InProcessName  : [i]プロセス名称
**/
BOOL CCallUnwinderRequest::ExecuteProcess( const std::string& InProcessName )
{
	STARTUPINFO startupInfo = {0};						// 表示に関する属性
	startupInfo.cb = sizeof(startupInfo);				// 構造体サイズ
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;			// 構造体の有効なメンバを指定
	startupInfo.wShowWindow = SW_HIDE;					// ウィンドウ表示形式(HIDE:表示しない)

	PROCESS_INFORMATION processInformation = {0};		// プロセスとそのプライマリスレッドの情報

	// プロセスの起動
	BOOL result = CreateProcess(
					nullptr,
					const_cast<char*>(InProcessName.c_str()),	// 実行するコマンド
					nullptr,
					nullptr,
					TRUE,
					0,
					nullptr,
					nullptr,
					&startupInfo,
					&processInformation);

	if (result == 0)
	{
		throw std::runtime_error("Cannot create process.");
	}

	// スレッドのハンドルはいらないのですぐ閉じる
	CloseHandle(processInformation.hThread);

	return TRUE;
}

/**
	* @brief  SignalStatus(STATUS)タイムアウトタイマー設定
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
**/
void CCallUnwinderRequest::SetTimerStatusReceive(CWnd* pWnd)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryStatus_RepeatTime().c_str()) + iniUnwinderManager.getSignalstatus_Timeout_Judegment_Waittime()) * 1000;
	// タイマー設定
	pWnd->SetTimer(TIMER_STATUS_RECEIVE, nTimeout, NULL);
}

/**
	* @brief  SignalStatus(PAPER)タイムアウトタイマー設定
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
**/
void CCallUnwinderRequest::SetTimerPaperReceive(CWnd* pWnd)
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryResource_RepeatTime().c_str()) + iniUnwinderManager.getSignalstatus_Timeout_Judegment_Waittime()) * 1000;
	// タイマー設定
	pWnd->SetTimer(TIMER_PAPER_RECEIVE, nTimeout, NULL);
}

/**
	* @brief  SignalStatus(STATUS)タイムアウトタイマー停止
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
**/
void CCallUnwinderRequest::KillTimerStatusReceive(CWnd* pWnd)
{
	pWnd->KillTimer(TIMER_STATUS_RECEIVE);
}

/**
	* @brief  SignalStatus(PAPER)タイムアウトタイマー停止
	* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
**/
void CCallUnwinderRequest::KillTimerPaperReceive(CWnd* pWnd)
{
	pWnd->KillTimer(TIMER_PAPER_RECEIVE);
}

};	// namespace unwinder_manager