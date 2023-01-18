/**
	* @file		ReceiveSignalStatus.cpp
	* @brief	SignalStatus受信
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include "Utility.h"
#include "ExecCommand.h"
#include "Ini_UnwinderManager.h"
#include "Ini_UnwinderManager_work.h"
#include "ReceiveSignalStatus.h"
// ==========================================================================

namespace unwinder_manager
{

CReceiveSignalStatus::CReceiveSignalStatus(CWnd* pWnd)
{
	m_pWnd = pWnd;	// ★★★テスト用★★★
}
CReceiveSignalStatus::~CReceiveSignalStatus()
{
}

/**
	* @brief  UWandRW_Receiverから通知されるSignalStatusを受信する
**/
BOOL CReceiveSignalStatus::MainLoop()
{
 	WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::MainLoop] Start");

	HANDLE hPipe = INVALID_HANDLE_VALUE;
    hPipe = CreateNamedPipe("\\\\.\\pipe\\Unwinder",		//lpName
                           PIPE_ACCESS_INBOUND,            // dwOpenMode
                           PIPE_TYPE_BYTE | PIPE_WAIT,     // dwPipeMode
                           1,                              // nMaxInstances
                           0,                              // nOutBufferSize
                           0,                              // nInBufferSize
                           100,                            // nDefaultTimeOut
                           NULL);                          // lpSecurityAttributes

    if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::stringstream ss;
		ss << "[CReceiveSignalStatus::MainLoop] CreateNamedPipe(\\\\.\\pipe\\Unwinder) Error GetLastError=" << GetLastError();
		WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
        return FALSE;
    }

	if (!ConnectNamedPipe(hPipe, NULL))
	{
		std::stringstream ss;
		ss << "[CReceiveSignalStatus::MainLoop] ConnectNamedPipe Error GetLastError=" << GetLastError();
		WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
        return FALSE;
	}

	BOOL nRet = TRUE; 
    while (nRet)
	{

        char szBuff[10];
		ZeroMemory(szBuff,sizeof(szBuff));

        if ( ReadData(hPipe, szBuff, 8) )
		{
			long DataSize = atol(szBuff);
			char *pXmlData = new char[DataSize+1];
			ZeroMemory(pXmlData,DataSize+1);

			if ( ReadData(hPipe, pXmlData, DataSize) )
			{
				std::string XmlData;
				XmlData.append(pXmlData);
				delete [] pXmlData;

				AnalyzeData(XmlData);
			}
			else
			{
				delete [] pXmlData;
				std::stringstream ss;
				ss << "[CReceiveSignalStatus::MainLoop] ReadFile Error GetLastError=" << GetLastError();
				WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
				nRet = FALSE;
			}
        }
		else
		{
			std::stringstream ss;
			ss << "[CReceiveSignalStatus::MainLoop] ReadFile Error GetLastError=" << GetLastError();
			WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
			nRet = FALSE;
		}
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

 	WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::MainLoop] End");
	return nRet;
}

/**
	* @brief  Pipe読み込み
	* @param  inPipe  : [i]PIPEハンドル
	* @param  outData : [i]読み込みデータのポインタ
	* @param  inSize  : [i]読み込みデータの長さ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CReceiveSignalStatus::ReadData( HANDLE inPipe, char* OutData, DWORD inSize )
{
	char* p = OutData;

	while( inSize )
	{
		DWORD ReadSize;
		BOOL nRet = ReadFile(inPipe, p, inSize, &ReadSize, NULL);
		if ( nRet )
		{
			inSize -= ReadSize;
			p += ReadSize;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**
	* @brief  UWandRW_Receiverから通知されるSignalStatusを受信する
	* @param  inSize : [i]読み込みデータの長さ
	* @retval TRUE(固定)
**/
BOOL CReceiveSignalStatus::AnalyzeData( std::string& inXmldata )
{
	// UWからのレスポンスデータを解析する
	std::string PickupData = ExecuteParseXml(inXmldata);
	if ( CheckPickupData( PickupData ) )
	{
		std::string Type	= SelectPickupData(PickupData, "Type");			// 受信コマンド種別
		std::string SubType	= SelectPickupData(PickupData, "SubType");		// 受信コマンドサブ種別

		if ( Type == "SignalStatus" )
		{
			if ( SubType == "Status" )
			{
				std::string Status = SelectPickupData(PickupData, "Status");
				ReceiveStatusInfo(Status);
			}
			else if ( SubType == "Paper" )
			{
				std::string DescriptiveName = SelectPickupData(PickupData, "DescriptiveName");
				std::string Dimension		= SelectPickupData(PickupData, "Dimension");
				std::string MediaType		= SelectPickupData(PickupData, "MediaType");
				std::string RollDiameter	= SelectPickupData(PickupData, "RollDiameter");
				std::string Thickness		= SelectPickupData(PickupData, "Thickness");
				ReceivePaperInfo(DescriptiveName,Dimension,MediaType,RollDiameter,Thickness);
			}

			DataDisp(PickupData);		// ★★★テスト用★★★ 
		}
	}
	return TRUE;
}

/**
	* @brief  inSignalDataを解析するUWandRW_Parse_Xml.exeを呼び出し終了を待つ
	* @param  inSignalData : [i]UWからのSignalStatusデータ
	* @retval 異常の場合 [ERROR]エラー内容
	* @retval 正常の場合 ブランク区切りの抽出データ(項目名１=値１△項目名２=値２..)
**/
std::string CReceiveSignalStatus::ExecuteParseXml( std::string& inSignalData )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Start");

		CUtility::replaceString(inSignalData,"\"", "\\\"");		// XML内のダブルコートはエスケープする必要がある
		CUtility::replaceString(inSignalData,"\"/>", "\" />");	// 何故かダブルコートと１行でノード終了の/>の間にブランクがないとXMLロードで例外が発生する

		std::string applicationName = "UWandRW_Parse_Xml.exe";
		std::string command = "\"" + inSignalData + "\"";

		// [★★PENDING★★]スレッド管理のスレッド開始処理をする(すでにスレッド生成済みなら例外スロー)
		//ThreadManager& threadManager = ThreadManager::getInstance_onCreateThread();
		//threadManager.onCreateThread_InkCodeManageThread();

		// UWandRW_Parse_Xml.exeを起動し実行結果ファイルを文字列に読み込む
		unwinder_manager::ExecCommand& execCommand = unwinder_manager::ExecCommand::getInstance_onRunning();
		std::string PickupData = execCommand.exec(applicationName, command);

		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] End");

		return PickupData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Exception Startup UWandRW_Parse_Xml.exe");
		return "[ERROR] Startup UWandRW_Parse_Xml.exe";
	}
}

/**
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  inData        : [i]UWandRW_Parse_Xml.exeの結果データ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
BOOL CReceiveSignalStatus::CheckPickupData( const std::string& inData )
{
	if ( inData.find("[ERROR]") == std::string::npos )
	{
		return TRUE;	// 正常
	}
	else
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::CheckPickupData] Error");
		return FALSE;	// 異常
	}
}

/**
	* @brief  UWandRW_Parse_Xml.exeが正常にデータを出力したかチェックする
	* @param  inScrData    : [i]UWandRW_Parse_Xml.exeの結果データ
	* @param  inSelectName : [i]抽出データ名称
	* @retval 抽出データ
**/
std::string CReceiveSignalStatus::SelectPickupData( const std::string& inScrData, const std::string& inSelectName )
{
	std::vector<std::string> strList = CUtility::splitString(inScrData, ' ');
	for( auto ite = strList.begin(); ite != strList.end(); ite++ )
	{
		if ( ite->compare(0,inSelectName.size(),inSelectName) == 0 )
		{
			size_t pos = ite->find("=");
			if ( std::string::npos != pos )
			{
				return ite->substr(pos+1);
			}
		}
	}
	return "";
}

/**
	* @brief  ステータス情報受信時の処理
	* @param  inStatus : [i]UWから通知されたステータス
	* @retval TRUE(固定)
**/
BOOL CReceiveSignalStatus::ReceiveStatusInfo( const std::string& inStatus )
{
	// タイマーの停止
	KillTimerStatusReceive();
	// タイマーの開始
	SetTimerStatusReceive();

	Ini_UnwinderManager_work iniUnwinderManager_work;
	iniUnwinderManager_work.Initialize(TRUE);
	iniUnwinderManager_work.putUnwinderStatus(inStatus);
	return TRUE;
}

/**
	* @brief  用紙情報受信時の処理
	* @param  inDescriptiveName	 : [i]UWから通知された印刷条件名
	* @param  inDimension		 : [i]UWから通知されたDimension
	* @param  inMediaType		 : [i]UWから通知されたMediaType
	* @param  inRollDiameter	 : [i]UWから通知されたRollDiameter
	* @param  inThickness		 : [i]UWから通知されたThickness
	* @retval TRUE(固定)
**/
BOOL CReceiveSignalStatus::ReceivePaperInfo( const std::string& inDescriptiveName,
		const std::string& inDimension,
		const std::string& inMediaType,
		const std::string& inRollDiameter,
		const std::string& inThickness )
{
	// タイマーの停止
	SetTimerPaperReceive();
	// タイマーの開始
	SetTimerPaperReceive();

	//
	// ★★★必要な処理を実施する★★★
	//

	return TRUE;
}

/**
	* @brief  SignalStatus(STATUS)タイムアウトタイマー設定
**/
void CReceiveSignalStatus::SetTimerStatusReceive()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryStatus_RepeatTime().c_str()) + 10) * 1000;
	// タイマー設定
	m_pWnd->SetTimer(TIMER_STATUS_RECEIVE, nTimeout, NULL);
}

/**
	* @brief  SignalStatus(PAPER)タイムアウトタイマー設定
**/
void CReceiveSignalStatus::SetTimerPaperReceive()
{
	Ini_UnwinderManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryResource_RepeatTime().c_str()) + 10) * 1000;
	// タイマー設定
	m_pWnd->SetTimer(TIMER_PAPER_RECEIVE, nTimeout, NULL);
}

/**
	* @brief  SignalStatus(STATUS)タイムアウトタイマー停止
**/
void CReceiveSignalStatus::KillTimerStatusReceive()
{
	m_pWnd->KillTimer(TIMER_STATUS_RECEIVE);
}

/**
	* @brief  SignalStatus(PAPER)タイムアウトタイマー停止
**/
void CReceiveSignalStatus::KillTimerPaperReceive()
{
	m_pWnd->KillTimer(TIMER_PAPER_RECEIVE);
}

// ★★★テスト用★★★
void CReceiveSignalStatus::DataDisp( const std::string& s )
{
	CString *p = new CString();
	*p = s.c_str();
	PostMessage(m_pWnd->m_hWnd, WM_USER_DSP_RECV, (WPARAM)p, 0);
}


};	// namespace ink_code_manager