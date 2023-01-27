/**
 *	@file	ReceiveSignalStatus.cpp
 *	@brief	SignalStatus受信
 *	@author	cec K.Miyachi
 *	@date		2021/12/24 cec K.Miyachi 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include "Utility.h"
#include "Ini_UwRwManager.h"
#include "Ini_UwRwManager_work.h"
#include "XmlParse.h"
#include "ReceiveSignalStatus.h"
#include "RequestUwRwThread.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "PluginCBRProc.h"
#include "PaperDB_Callbacks.h"
#include "JobManager_Callbacks.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================

namespace uwrw_manager
{

/**
 *	CReceiveSignalStatus constructor
**/
CReceiveSignalStatus::CReceiveSignalStatus()
{
	m_dataIF = NULL;
	m_paperInfoReceivingStatus = true;
	m_isRecvPaperInfoStarting = false;
	m_receivedPaperInfo = false;
	m_UWThickness = THICKNESS_INVALID_VALUE;
	m_paperRemainingAmount = 0;
	m_RequestThread = NULL;
	m_IsExit = false;
}

/**
 *	CReceiveSignalStatus destructor
**/
CReceiveSignalStatus::~CReceiveSignalStatus()
{
}

/**
 *	Receive signal status info from UW
 *	@param[in] inDataIF: data IF
**/
void CReceiveSignalStatus::ReceiveSignalStatusInfo(CDataIF* inDataIF)
{
	m_dataIF = inDataIF;
	ReceiveInfo();
}

/**
 *	Receive signal status info notified from UWandRW_Receiver
 *	@retval TRUE: succeed
 *	@retval FALSE: failed
**/
BOOL CReceiveSignalStatus::ReceiveInfo()
{
	WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::MainLoop] Start");

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	hPipe = CreateNamedPipe(string("\\\\.\\pipe\\").append(strcmp(m_RequestThread->getDevicename(), "RW")? "Unwinder" : "Rewinder").c_str(),		//lpName
						   //Temporary comment PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,            // dwOpenMode
						   PIPE_ACCESS_INBOUND,
						   PIPE_TYPE_BYTE | PIPE_WAIT,     // dwPipeMode
						   1,                              // nMaxInstances
						   0,                              // nOutBufferSize
						   0,                              // nInBufferSize
						   100,                            // nDefaultTimeOut
						   NULL);                          // lpSecurityAttributes

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::stringstream ss;
		ss << "[CReceiveSignalStatus::ReceiveInfo] CreateNamedPipe(\\\\.\\pipe\\" << (strcmp(m_RequestThread->getDevicename(), "RW") ? "Unwinder" : "Rewinder") <<") Error GetLastError=" << GetLastError();
		WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
		return FALSE;
	}

	if (!ConnectNamedPipe(hPipe, NULL))
	{
		std::stringstream ss;
		ss << "[CReceiveSignalStatus::ReceiveInfo] Pipe Error GetLastError=" << GetLastError();
		WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
		CloseHandle(hPipe);
		return FALSE;
	}

	BOOL nRet = TRUE; 
	while (nRet)
	{
		if (m_IsExit)
		{
			break;
		}

		char szBuff[10];
		ZeroMemory(szBuff,sizeof(szBuff));

		if ( ReadData(hPipe, szBuff, 8) )
		{
			if (m_IsExit)
			{
				break;
			}
			long DataSize = atol(szBuff);
			char *pXmlData = new char[DataSize+1];
			ZeroMemory(pXmlData,DataSize+1);

			if ( ReadData(hPipe, pXmlData, DataSize) )
			{
				if (m_IsExit)
				{
					break;
				}
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
 *	Pipe reading
 *	@param[in] inPipe: PIPE handle
 *	@param[out] outData: pointer for read data
 *	@param[in] inSize: read data length
 *	@retval TRUE：succeed
 *	@retval FALSE：failed
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
			std::stringstream ss;
			ss << "[CReceiveSignalStatus::ReceiveInfo] Pipe Error GetLastError=" << GetLastError();
			WriteToLogBuf(LOG_DEBUG, (char*)ss.str().c_str());
			return FALSE;
		}
	}
	return TRUE;
}

/**
 *	Analyze signal status info notified from UWandRW_Receiver
 *	@param[in] inXmldata: data which is read from pipe
 *	@retval TRUE(fixed)
**/
BOOL CReceiveSignalStatus::AnalyzeData( const std::string& inXmldata )
{
	// Parse response data from UW
	//CUtility::replaceString(inXmldata, " ", "@@@");
	UwRwXjmfDataMap sUwXjmfDataMap;
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
				std::string DescriptiveName		= sUwXjmfDataMap["DescriptiveName"];
				std::string Dimension			= sUwXjmfDataMap["Dimension"];
				std::string MediaType			= sUwXjmfDataMap["MediaType"];
				std::string RollDiameter		= sUwXjmfDataMap["RollDiameter"];
				std::string Thickness			= sUwXjmfDataMap["Thickness"];
				std::string Weight				= sUwXjmfDataMap["Weight"];
				std::string StopRollDiameter	= sUwXjmfDataMap["StopRollDiameter"];
				std::string UWWebTension		= sUwXjmfDataMap["UWWebTension"];
				std::string RWWebTension		= sUwXjmfDataMap["RWWebTension"];
				std::string RWRollTension		= sUwXjmfDataMap["RWRollTension"];
				ReceivePaperInfo(DescriptiveName,Dimension,MediaType,RollDiameter,Thickness, Weight,
					StopRollDiameter, UWWebTension, RWWebTension, RWRollTension);
			}
			else
			{
				WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Warning SubType unkown");
			}

		}
		else
		{
			WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::AnalyzeData] Warning Type unkown");
		}
	}
	return TRUE;
}

/**
 *	@brief  parse xml data
 *	@param[in] inSignalData     : SignalStatus info from UW
 *	@param[in] outUwXjmfDataMap : data parsed from SignalStatus
 *	@retval : processing result
	**/
std::string CReceiveSignalStatus::ExecuteParseXml( const std::string& inSignalData, UwRwXjmfDataMap& outUwXjmfDataMap )
{
	try
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Start");

		CXmlParseData sParseXmlData(inSignalData, outUwXjmfDataMap);
		std::string ResultData = sParseXmlData.Doit();

		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] End");

		return ResultData;
	}
	catch(...)
	{
		WriteToLogBuf(LOG_DEBUG,"[CReceiveSignalStatus::ExecuteParseXml] Exception");
		return "";
	}
}

/**
 *	Processing when the status info is received
 *	@param[in] inStatus: status notified by UW
 *	@retval: TRUE(fixed)
**/
BOOL CReceiveSignalStatus::ReceiveStatusInfo( const std::string& inStatus )
{
	m_dataIF->UpdateDisplayUWStatus(true, m_RequestThread->getDevicename());
	// send message to request thread to set status timer
	m_RequestThread->MsgSetTimerStatusReceive();

	CIni_UwRwManager_work iniUwRwManager_work;
	iniUwRwManager_work.Initialize(TRUE);
	iniUwRwManager_work.putUwRwStatus(inStatus, m_RequestThread->getDevicename());
	return TRUE;
}

/**
 *	Processing when the paper info is received
 *	@param[in] inDescriptiveName: print condition name notified by UW
 *	@param[in] inDimension		: Dimension notified by UW
 *	@param[in] inMediaType		: MediaType notified by UW
 *	@param[in] inRollDiameter	: RollDiameter notified by UW
 *	@param[in] inThickness		: Thickness notified by UW
 *	@param[in] inWeight			: Weight notified by UW
 *	@param[in] inStopRollDiameter	: Setting value of stop roll diameter of UW/RW
 *	@param[in] inUWWebTension		: UW dancer web tension notified by UW
 *	@param[in] inRWWebTension		: WW dancer web tension notified by UW
 *	@param[in] inRWRollTension		: RW winding tension notified by UW
 *	@retval : TRUE(fixed)
**/
BOOL CReceiveSignalStatus::ReceivePaperInfo( const std::string& inDescriptiveName,
		const std::string& inDimension,
		const std::string& inMediaType,
		const std::string& inRollDiameter,
		const std::string& inThickness,
		const std::string& inWeight,
		const std::string& inStopRollDiameter,
		const std::string& inUWWebTension,
		const std::string& inRWWebTension,
		const std::string& inRWRollTension)
{
	// send message to request thread to set paper info timer
	m_RequestThread->MsgSetTimerPaperReceive();

	long thickness = atol(inThickness.c_str());
	m_UWThickness = thickness;
	m_receivedPaperInfo = true;

	// Update the current print condition when the controller is started
	// Update is only happened when this is the first time paper info received from UW and there is no error in the receiving at this time.
	if (m_isRecvPaperInfoStarting && m_paperInfoReceivingStatus) {
		SPaperDB_Callbacks paperDBCallbacks;
		if (PaperDB_GetCallbacks(&paperDBCallbacks)) {
			string printConditionName = "";
			if (paperDBCallbacks.PDB_GetCurrentPrintCondition(printConditionName)) {
				long pdbThickness = 0;
				if (paperDBCallbacks.PDB_GetPaperThickness(printConditionName.c_str(), pdbThickness)) {
					if (pdbThickness != thickness) {
						std::stringstream msg;
						msg << ID_MESSAGE_UNWINDERMANAGER + IDM_UPDATE_PAPER_THICKNESS_TO_CURRENT_PRINT_CONDITION << "\n" << LoadResourceString(IDM_UPDATE_PAPER_THICKNESS_TO_CURRENT_PRINT_CONDITION, RESOURCE_MSG_STR);
						int result = ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_YESNO | MBST_ICONWARNING, NULL);
						if (result == IDYES) {
							if (paperDBCallbacks.PDB_SetPaperThickness(printConditionName.c_str(), thickness)){
								if (!paperDBCallbacks.PDB_CommitPaperDB()) {
									WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Cannot commit to PDB");
								}
							}
							else {
								WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Cannot set paper thickness to PDB");
							}
						}
					}
				}
				else {
					WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Cannot get paper thickness from PDB");
				}
			}
			else {
				WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Cannot get current print condition.");
			}
		}
		else {
			WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Cannot get PaperDB callback.");
		}
		SetRecvPaperInfoStartingStatus(false);
	}

	// Update value of paper roll diameter and paper remaining amount to ini file.
	CIni_UwRwManager_work ini_UWMamagerWork;
	if (!ini_UWMamagerWork.Initialize(TRUE))
	{
		WriteToLogBuf(LOG_DEBUG, "CReceivesignalStatus::ReceivePaperInfo() error - CIni_UwRwManager_work is failed to initialization.");
	}
	if (inRollDiameter != "-1")
	{
		ini_UWMamagerWork.putPaperRollDiameter(inRollDiameter, m_RequestThread->getDevicename());
	}

	std::vector<std::string> vDimension = CUtility::splitString(inDimension, ' ');
	if (vDimension.size() != 2) {
		WriteToLogBuf(LOG_DEBUG, "CReceiveSignalStatus::ReceivePaperInfo() error - Invalid dimension.");
	}
	else {
		const std::string& paperRemainingAmount = vDimension.at(1);
		if (paperRemainingAmount != "-1")
		{
			m_paperRemainingAmount = atoi(paperRemainingAmount.c_str());
			ini_UWMamagerWork.putPaperRemainingAmount(paperRemainingAmount, m_RequestThread->getDevicename());
		}
	}

	if (inStopRollDiameter != "-1")
	{
		ini_UWMamagerWork.putPaperStopRollDiameter(inStopRollDiameter, m_RequestThread->getDevicename());
	}

	ini_UWMamagerWork.Finalize();

	return TRUE;
}

/**
 * @brief Set a CRequestUwRwThread object
 * @param[in] inRequestThread CRequestUwRwThread object
**/
void CReceiveSignalStatus::SetRequestThread(CRequestUwRwThread* inRequestThread)
{
	m_RequestThread = inRequestThread;
}

/**
 *	Get the paper info receiving status (whether or not the info has been received)
 *	@param[out] outHasPaperInfoReceived: whether or not paper info has been received (true: has been received, false: has not been received)
**/
void CReceiveSignalStatus::HasPaperInfoReceived(bool &outHasPaperInfoReceived)
{
	outHasPaperInfoReceived = m_receivedPaperInfo;
}

/**
 *	Get value of paper thickness which is notified from UW
 *	@param[out] outThickness: paper thickness value notified from UW
**/
void CReceiveSignalStatus::GetUWPaperThickness(long &outThickness)
{
	outThickness = m_UWThickness;
}

/**
 * Get value of paper remaining amount which is notified from UW
 *	@param[out] outPaperRemainingAmount: paper remaining amount notified from UW
**/
void CReceiveSignalStatus::GetUWPaperRemainingAmount(long &outPaperRemainingAmount)
{
	outPaperRemainingAmount = m_paperRemainingAmount;
}

/**
 * Set value of paper remaining amount
 *	@param[in] inPaperRemainingAmount: paper remaining amount
**/
void CReceiveSignalStatus::SetUWPaperRemainingAmount(long inPaperRemainingAmount)
{
	m_paperRemainingAmount = inPaperRemainingAmount;
}

/**
 *	Set status of paper info receiving
 *	@param[in] status: paper info receiving status (is false if there is any error in receiving of paper info from UW, otherwise is true)
**/
void CReceiveSignalStatus::SetStatusOfPaperInfoReceiving(bool status)
{
	m_paperInfoReceivingStatus = status;
}

/**
 *	Set the status for starting of paper info receiving 
 *	@param[in] status: status for starting of paper info receiving (status is true if it is the first time paper info received from UW from when the controller is started, else it is false)
**/
void CReceiveSignalStatus::SetRecvPaperInfoStartingStatus(bool status)
{
	m_isRecvPaperInfoStarting = status;
}

/**
 * @brief Call from CReceiveSignalStatusThread to set/reset break loop condition
 * @param[in] inVal Value to set
**/
void CReceiveSignalStatus::SetExit(bool inVal)
{
	m_IsExit = inVal;
}

};	// namespace uwrw_manager