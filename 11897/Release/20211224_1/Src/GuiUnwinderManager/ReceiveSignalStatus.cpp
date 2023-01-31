/**
	* @file		ReceiveSignalStatus.cpp
	* @brief	SignalStatus受信
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include "Ini_UnwinderManager.h"
#include "Ini_UnwinderManager_work.h"
#include "XmlParse.h"
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
BOOL CReceiveSignalStatus::AnalyzeData( const std::string& inXmldata )
{
	// UWからのレスポンスデータを解析する
	UwXjmfDataMap sUwXjmfDataMap;
	std::string ResultData = ExecuteParseXml(inXmldata, sUwXjmfDataMap);
	if ( ResultData.find(ErrorLabel) == std::string::npos )
	{
		WriteToLogBuf(LOG_DEBUG, (char*)ResultData.c_str());

		std::string Type	= sUwXjmfDataMap["Type"];			// 受信コマンド種別
		std::string SubType	= sUwXjmfDataMap["SubType"];		// 受信コマンドサブ種別

		if ( Type == "SignalStatus" )
		{
			if ( SubType == "Status" )
			{
				WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Receive SignalStatus(Status)");
				std::string Status = sUwXjmfDataMap["Status"];
				ReceiveStatusInfo(Status);
			}
			else if ( SubType == "Paper" )
			{
				WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Receive SignalStatus(Paper)");
				std::string DescriptiveName = sUwXjmfDataMap["DescriptiveName"];
				std::string Dimension		= sUwXjmfDataMap["Dimension"];
				std::string MediaType		= sUwXjmfDataMap["MediaType"];
				std::string RollDiameter	= sUwXjmfDataMap["RollDiameter"];
				std::string Thickness		= sUwXjmfDataMap["Thickness"];
				ReceivePaperInfo(DescriptiveName,Dimension,MediaType,RollDiameter,Thickness);
			}
			else
			{
				WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Warning SubType unkown");
			}

			DataDisp(ResultData);		// ★★★テスト用★★★ 
		}
		else
		{
			WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Warning Type unkown");
		}
	}
	return TRUE;
}

/**
	* @brief  inSignalDataを解析する
	* @param  inSignalData     : [i]UWからのSignalStatusデータ
	* @param  outUwXjmfDataMap : [o]SignalStatusから取り出したデータ
	* @retval TRUE：正常
	* @retval FALSE：異常
**/
std::string CReceiveSignalStatus::ExecuteParseXml( const std::string& inSignalData, UwXjmfDataMap& outUwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Start");

		CXmlParseData sParseXmlData(inSignalData,outUwXjmfDataMap);
		std::string ResultData = sParseXmlData.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] End");

		return ResultData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Exception");
		return FALSE;
	}
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
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryStatus_RepeatTime().c_str()) + iniUnwinderManager.getSignalstatus_Timeout_Judegment_Waittime()) * 1000;
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
	UINT nTimeout = (atoi(iniUnwinderManager.getQueryResource_RepeatTime().c_str()) + iniUnwinderManager.getSignalstatus_Timeout_Judegment_Waittime()) * 1000;
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
void CReceiveSignalStatus::DataDisp( const std::string& inResultData )
{
	CString *p = new CString();
	p->Append(inResultData.c_str());
	PostMessage(m_pWnd->m_hWnd, WM_USER_DSP_RECV, (WPARAM)p, 0);
}

};	// namespace unwinder_manager