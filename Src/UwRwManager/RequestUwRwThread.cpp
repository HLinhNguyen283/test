/**
 * @file	RequestUnwinderThread.cpp
 * @brief	Main process thread
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <string>
#include <sstream>
#include <psapi.h>
#include <Shlwapi.h>
#include "RequestUwRwThread.h"
#include "MakeComposeUwRwData.h"
#include "CommonCBRProc.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "CommonCBRProc_OP.h"
#include "PaperDB_Callbacks.h"
#include "JobManager_Callbacks.h"
#include "CommonConvertUnit.h"
#include "DataIF.h"
#include "ReceiveSignalStatus.h"
// ===========================================================================

#define WM_USER_SET_STATUS_TIMER		WM_USER+100
#define WM_USER_SET_PAPERINFO_TIMER		WM_USER+101
#define WM_USER_NOTIFY_QUERY_RESOURCE	WM_USER+102

#define RECEIVER_EXE_NAME _T("UWandRW_Receiver.exe")

namespace uwrw_manager
{

/**
 * @brief CRequestUwRwThread constructor
**/
CRequestUwRwThread::CRequestUwRwThread(const char* inDevicename):
	m_ExitThread(false),
	m_ReceiverRunning(false),
	m_StatusRequested(false),
	m_ResourceRequested(false),
	m_ExitThreadEvent(NULL),
	m_ReceiverProcess(NULL),
	m_dataIF(NULL),
	m_StatusReceivingTimeId(0),
	m_PaperReceivingTimeId(0),
	m_DeviceName(inDevicename), 
	m_ReceiveThread(inDevicename)
{
	memset(&m_thread, 0, sizeof(ST_THREAD_INFO)) ;
	CReceiveSignalStatus &receiveSignalStatus = CReceiveSignalStatus::GetInstance(m_DeviceName);
	receiveSignalStatus.SetRequestThread(this);
}

/**
 * @brief CRequestUwRwThread destructor
**/
CRequestUwRwThread::~CRequestUwRwThread()
{
}

/**
 * @brief initialize data
 * @param[in] inDataIF plugin DataIF object
**/
void CRequestUwRwThread::Initialize(CDataIF* inDataIF)
{
	m_dataIF = inDataIF;
	m_RequestUwRw.Initialize(inDataIF, this);
	m_ExitThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

/**
 * @brief finalize data
**/
void CRequestUwRwThread::Finalize()
{
	m_RequestUwRw.Finalize();
	CloseHandle(m_ExitThreadEvent);
}

/**
 * @brief start the thread
**/
void CRequestUwRwThread::StartThread()
{
	UINT threadID = 0;
	m_thread = TM_CreateThread(
		nullptr,
		0,
		threadEventProc,
		(void*)this,
		0,
		&threadID,
		_T("UwRwManager.dll"),
		_T("RequestUwRwThread"),
		nullptr);
}

/**
 * @brief stop the thread
**/
void CRequestUwRwThread::EndThread()
{
	SetEvent(m_ExitThreadEvent);
	m_ExitThread = true;
	TM_DeleteThread(&m_thread);
	m_thread.thread_handle = NULL;
}

/**
 * @brief put thread to sleep also check for thread exit or Receiver end
 * Call from RequestUnwinder function
 * @param[in] inMillisec time to sleep in milliseconds
 * @return true if thread exit or Receiver end / false if not
**/
bool CRequestUwRwThread::WaitCheckExit(int inMillisec)
{
	HANDLE handles[2];
	handles[0] = m_ExitThreadEvent;
	int nCount = m_ReceiverProcess == NULL ? 1 : 2;
	handles[1] = m_ReceiverProcess;
	DWORD ret = WaitForMultipleObjects(nCount, handles, FALSE, inMillisec);
	switch (ret)
	{
	case WAIT_OBJECT_0:
		m_ExitThread = true;
		return true;
	case WAIT_OBJECT_0 + 1:
		m_ReceiverRunning = false;
		return true;
	}
	return false;
}

/**
 * @brief call from callback function to notify and then query UW paper info
 * @param inSectionId empty if call from set current condition
 *                    section ID if called from job running process
**/
void CRequestUwRwThread::NotifyAndQueryResource(const std::string& inSectionId)
{
	// #11897: temporarily comment out
	//CUwRwPaper unwinderPaper;
	//if (inSectionId.empty())
	//{
	//	if (!GetPrintConditionResourceInfo(unwinderPaper))
	//	{
	//		WriteToLogBuf(LOG_DEBUG, "NotifyAndQueryResource: GetPrintConditionResourceInfo fails");
	//		return;
	//	}
	//}
	//else
	//{
	//	if (!GetJobResourceInfo(unwinderPaper, inSectionId))
	//	{
	//		WriteToLogBuf(LOG_DEBUG, "NotifyAndQueryResource: GetJobResourceInfo fails");
	//		return;
	//	}
	//}

	CReceiveSignalStatus &receiveSignalStatus = CReceiveSignalStatus::GetInstance(m_DeviceName);
	//std::string result = m_RequestUwRw.RequestCommandResource(&unwinderPaper);
	std::string result = SuccessLabel;
	if (result.find(SuccessLabel) != std::string::npos)
	{
		result = m_RequestUwRw.RequestQueryResource();
		if (result.find(SuccessLabel) != std::string::npos)
		{
			SetTimerPaperReceive();
			m_ResourceRequested = true;
		}
		else {
			receiveSignalStatus.SetStatusOfPaperInfoReceiving(false);
		}
	}
	else {
		receiveSignalStatus.SetStatusOfPaperInfoReceiving(false);
	}
}

/**
 * @brief sent msg from ReceiveSignalStatus thread (to call SetTimerStatusReceive in this thread)
**/
void CRequestUwRwThread::MsgSetTimerStatusReceive()
{
	PostThreadMessage(m_thread.thread_id, WM_USER_SET_STATUS_TIMER, 0, 0);
}

/**
 * @brief sent msg from ReceiveSignalStatus thread (to call SetTimerPaperReceive in this thread)
**/
void CRequestUwRwThread::MsgSetTimerPaperReceive()
{
	PostThreadMessage(m_thread.thread_id, WM_USER_SET_PAPERINFO_TIMER, 0, 0);
}

/**
 * @brief  setting for timeout timer of SignalStatus(STATUS)
**/
void CRequestUwRwThread::SetTimerStatusReceive()
{
	UINT nTimeout = m_dataIF->GetTimeoutTimerStatus(m_DeviceName);
	// replace exist timer
	m_StatusReceivingTimeId = SetTimer(NULL, m_StatusReceivingTimeId, nTimeout, NULL);
}

/**
 * @brief  setting for timeout timer of SignalStatus(PAPER)
**/
void CRequestUwRwThread::SetTimerPaperReceive()
{
	UINT nTimeout = m_dataIF->GetTimeoutTimerPaper(m_DeviceName);
	// replace exist timer
	m_PaperReceivingTimeId = SetTimer(NULL, m_PaperReceivingTimeId, nTimeout, NULL);
}

/**
 * @brief  stop timeout timer of SignalStatus(STATUS)
**/
void CRequestUwRwThread::KillTimerStatusReceive()
{
	KillTimer(NULL, m_StatusReceivingTimeId);
	m_StatusReceivingTimeId = 0;
}

/**
 * @brief  stop timeout timer of SignalStatus(PAPER)
**/
void CRequestUwRwThread::KillTimerPaperReceive()
{
	KillTimer(NULL, m_PaperReceivingTimeId);
	m_PaperReceivingTimeId = 0;
}

#define CheckReturn(Func) \
	if (!Func)            \
	{                     \
		return false;     \
	}

/**
 * @brief Get necessary paper info from current print condition
 * @param[out] outUnwinderPaper paper info
 * @return true if get success/ false if get fail
**/
bool CRequestUwRwThread::GetPrintConditionResourceInfo(CUwRwPaper& outUnwinderPaper)
{
	SPaperDB_Callbacks cb;

	CheckReturn(PaperDB_GetCallbacks(&cb));
	std::string name, mediaType, externalID;
	long nPaperAmount, nSizeW, nThickness, nTension, nSpeed;
	CheckReturn(cb.PDB_GetCurrentPrintCondition(name));
	CheckReturn(cb.PDB_GetPaperSizeW(name.c_str(), DEF_LENGTH_POINT, nSizeW));
	CheckReturn(cb.PDB_GetPaperType(name.c_str(), mediaType));
	CheckReturn(cb.PDB_GetPaperThickness(name.c_str(), nThickness));
	CheckReturn(cb.PDB_GetPaperTension(name.c_str(), nTension));

	ST_PDB_PRINT_MODE sPrintModeParam;
	CheckReturn(cb.PDB_GetPrintMode(name.c_str(), sPrintModeParam));
	CheckReturn(cb.PDB_GetModeResoSpeed(name.c_str(), sPrintModeParam.CMYK_PRN_RESO_X, sPrintModeParam.CMYK_PRN_RESO_Y, DEF_MODE_RESO_SPEED_M, nSpeed));

	externalID = m_dataIF->GetCommandResourceExternalID();
	m_dataIF->GetUWPaperRemainingAmount(nPaperAmount);
	char szTmp[256];
	outUnwinderPaper.DescriptiveName = name;
	sprintf_s(szTmp, "%.2f %.2f", (double)nSizeW / 1000.0, (double)nPaperAmount);
	outUnwinderPaper.Dimension = szTmp;
	outUnwinderPaper.MediaType = mediaType;
	sprintf_s(szTmp, "%d", nThickness);
	outUnwinderPaper.Thickness = szTmp;
	sprintf_s(szTmp, "%d", nTension);
	outUnwinderPaper.UWTension = szTmp;
	sprintf_s(szTmp, "%d", nSpeed);
	outUnwinderPaper.MaxRunSpeed = szTmp;
	outUnwinderPaper.ExternalID = externalID;
	return true;
}

/**
 * @brief Get necessary paper info from job print condition
 * @param[out] outUnwinderPaper paper info
 * @param[in] inSectionId job section ID
 * @return true if get success/ false if get fail
**/
bool CRequestUwRwThread::GetJobResourceInfo(CUwRwPaper& outUnwinderPaper, const std::string& inSectionId)
{
	SJobManager_Callbacks cb;
	CheckReturn(JM_GetCallbacks(&cb));
	std::string name, mediaType, externalID;
	long nPaperAmount, nSizeW, nThickness, nTension, nSpeed;
	CheckReturn(cb.JM_GetPrintCondition(inSectionId.c_str(), name));
	CheckReturn(cb.JM_GetPaperSizeW(inSectionId.c_str(), DEF_LENGTH_POINT, nSizeW));
	CheckReturn(cb.JM_GetPaperType(inSectionId.c_str(), mediaType));
	CheckReturn(cb.JM_GetPaperThickness(inSectionId.c_str(), nThickness));
	CheckReturn(cb.JM_GetPaperTension(inSectionId.c_str(), nTension));
	CheckReturn(cb.JM_GetModeResoSpeed(inSectionId.c_str(), DEF_MODE_RESO_SPEED_M, nSpeed));

	externalID = m_dataIF->GetCommandResourceExternalID();
	m_dataIF->GetUWPaperRemainingAmount(nPaperAmount);
	char szTmp[256];
	outUnwinderPaper.DescriptiveName = name;
	sprintf_s(szTmp, "%.2f %.2f", (double)nSizeW / 1000.0, (double)nPaperAmount);
	outUnwinderPaper.Dimension = szTmp;
	outUnwinderPaper.MediaType = mediaType;
	sprintf_s(szTmp, "%d", nThickness);
	outUnwinderPaper.Thickness = szTmp;
	sprintf_s(szTmp, "%d", nTension);
	outUnwinderPaper.UWTension = szTmp;
	sprintf_s(szTmp, "%d", nSpeed);
	outUnwinderPaper.MaxRunSpeed = szTmp;
	outUnwinderPaper.ExternalID = externalID;
	return true;
}

/**
 * @brief call from callback function to notify and then query UW paper info
 * @param inSectionId empty if call from set current condition
 *                    section ID if called from job running process
**/
void CRequestUwRwThread::MsgNotifyAndQueryResource(const std::string & inSectionId)
{
	if (m_StatusRequested)
	{
		char* pSectionId = new char[256];
		strcpy_s(pSectionId, 256, inSectionId.c_str());
		PostThreadMessage(m_thread.thread_id, WM_USER_NOTIFY_QUERY_RESOURCE, (WPARAM)pSectionId, 0);
	}
}

/**
 * @brief callback function to process thread, will call to ThreadProc
 * @param [in] inParam CRequestUwRwThread object
 * @return 0
**/
UINT WINAPI CRequestUwRwThread::threadEventProc(LPVOID inParam)
{
	CRequestUwRwThread* RequestUwRwThread = reinterpret_cast<CRequestUwRwThread*>(inParam);
	RequestUwRwThread->ThreadProc();
	return 0;
}

/**
 * @brief main thread processing function
**/
void CRequestUwRwThread::ThreadProc()
{
	m_dataIF->PutStatusCannelID("", m_DeviceName);
	m_dataIF->PutResourceCannelID("", m_DeviceName);
	while (true)
	{
		CheckReceiverRunning();
		if (m_ExitThread)
		{
			return;
		}

		while (true)
		{
			// query and update UW status
			CheckUWRWStatus();
			if (m_ExitThread || !m_ReceiverRunning)
			{
				break;
			}

			// command & query paper resource when controller started
			if (m_StatusRequested)
			{
				NotifyAndQueryResource();
			}

			if (m_ExitThread || !m_ReceiverRunning)
			{
				break;
			}

			if (m_StatusRequested && !m_ReceiveThread.IsStarted())
			{
				m_ReceiveThread.StartThread(m_dataIF);
				
			}

			CheckEvents();

			if (m_ExitThread || !m_ReceiverRunning)
			{
				break;
			}
		}

		// delete channel when Receiver terminated or system exit
		Cleanup();

		if (m_ExitThread)
		{
			return;
		}
	}
}

/**
 * @brief Check if a process is running
 * @param[in] szExeName executable file name
 * @param[out] hProcess process handle if running
 * @return true if processing is running / false if not
*/
static bool CheckProcessRunning(LPCTSTR szExeName, HANDLE& hProcess)
{

	DWORD ProcessID[1024];
	DWORD dwSize;
	EnumProcesses(ProcessID, sizeof(ProcessID), &dwSize);
	DWORD dwMax = (dwSize / sizeof(DWORD));
	for (DWORD dwNow = 0; dwNow < dwMax; dwNow++)
	{
		TCHAR   szFile[1024] = { 0 };
		HMODULE Module[1024] = { 0 };

		HANDLE hCheckProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID[dwNow]);
		if (hCheckProcess != NULL)
		{
			if (EnumProcessModules(hCheckProcess, Module, sizeof(Module), &dwSize))
			{
				GetModuleFileNameEx(hCheckProcess, Module[0], szFile, sizeof(szFile));
				LPCTSTR szModuleName = PathFindFileName(szFile);
				if (_tcscmp(szModuleName, szExeName) == 0)
				{
					hProcess = hCheckProcess;
					return true;
				}
			}
			CloseHandle(hCheckProcess);
		}
	}
	return false;
}

/**
 * @brief check if UWandRW_Receiver.exe running
 *        if not display warning dialog and wait until it running
**/
void CRequestUwRwThread::CheckReceiverRunning()
{
	ST_CRITICAL_SECTION_INFO cs;
	m_dataIF->getCriticalSectionRef(cs);
	m_ReceiverRunning = CheckProcessRunning(RECEIVER_EXE_NAME, m_ReceiverProcess);
	CSM_EnterCriticalSection(&cs);
	bool displayErrorDialog;
	m_dataIF->getDisplayErrorDialogEnableRef(displayErrorDialog);
	if (!m_ReceiverRunning)
	{
		if (displayErrorDialog) 
		{
			displayErrorDialog = false;
			// warning dialog when Receiver is not running
			char errorMsg[512] = { 0 };
			_snprintf(errorMsg, sizeof(errorMsg) - 1, "%d\n%s", (ID_MESSAGE_UNWINDERMANAGER + IDM_NOTIFY_RECEIVER_STATUS), (char*)LoadResourceString(IDM_NOTIFY_RECEIVER_STATUS, RESOURCE_MSG_STR));
			ShowMessageBox(errorMsg, MBST_ICONERROR | MBST_OK | MBST_MODELESS, NULL);
			CReceiveSignalStatus &receiveSignalStatus = CReceiveSignalStatus::GetInstance(m_DeviceName);
			receiveSignalStatus.SetStatusOfPaperInfoReceiving(false);
			// change icon translucent
			m_dataIF->UpdateDisplayUWStatus(false, m_DeviceName);
		}
	}
	CSM_LeaveCriticalSection(&cs);
	while (!m_ReceiverRunning)
	{
		DWORD ret = WaitForSingleObject(m_ExitThreadEvent, 1000);
		if (ret == WAIT_OBJECT_0)
		{
			m_ExitThread = true;
			return;
		}
		m_ReceiverRunning = CheckProcessRunning(RECEIVER_EXE_NAME, m_ReceiverProcess);
		m_dataIF->UpdateDisplayUWStatus(false, m_DeviceName);
	}
	displayErrorDialog = true;
}

/**
 * @brief register UW status channel
**/
void CRequestUwRwThread::CheckUWRWStatus()
{
	// query status
	std::string result = m_RequestUwRw.RequestQueryStatus();
	if (result.find(SuccessLabel) == std::string::npos)
	{
		CReceiveSignalStatus &receiveSignalStatus = CReceiveSignalStatus::GetInstance(m_DeviceName);
		receiveSignalStatus.SetStatusOfPaperInfoReceiving(false);
		// change icon translucent
		m_dataIF->UpdateDisplayUWStatus(false, m_DeviceName);
	}
	else
	{
		SetTimerStatusReceive();
		m_StatusRequested = true;
		// change icon normal
		//m_dataIF->UpdateDisplayUWStatus(true);
	}
}

/**
 * @brief process messages
**/
void CRequestUwRwThread::CheckEvents()
{
	HANDLE handles[2];
	handles[0] = m_ExitThreadEvent;
	handles[1] = m_ReceiverProcess;
	while (true)
	{
		DWORD ret = MsgWaitForMultipleObjects(2, handles, FALSE, INFINITE, QS_POSTMESSAGE | QS_TIMER);
		switch (ret)
		{
		case WAIT_OBJECT_0:
			m_ExitThread = true;
			return;
		case WAIT_OBJECT_0 + 1:
			m_ReceiverRunning = false;
			return;
		case WAIT_OBJECT_0 + 2:
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
				case WM_TIMER:
					if (msg.wParam == m_StatusReceivingTimeId || msg.wParam == m_PaperReceivingTimeId)
					{
						// timer event received, so no SignalStatus received from UW
						// UW may already stopped
						if (msg.wParam == m_PaperReceivingTimeId)
						{
							CReceiveSignalStatus &receiveSignalStatus = CReceiveSignalStatus::GetInstance(m_DeviceName);
							receiveSignalStatus.SetStatusOfPaperInfoReceiving(false);
						}

						if (m_ReceiveThread.IsStarted())
						{
							m_ReceiveThread.EndThread();
						}
						// delete channel ID
						m_dataIF->PutStatusCannelID("", m_DeviceName);
						m_dataIF->PutResourceCannelID("", m_DeviceName);
						KillTimerStatusReceive();
						KillTimerPaperReceive();
						m_StatusRequested = false;
						m_ResourceRequested = false;
						m_dataIF->UpdateDisplayUWStatus(false, m_DeviceName);
						
						return;
					}
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					break;
				case WM_USER_SET_STATUS_TIMER:
					SetTimerStatusReceive();
					break;
				case WM_USER_SET_PAPERINFO_TIMER:
					SetTimerPaperReceive();
					break;
				case WM_USER_NOTIFY_QUERY_RESOURCE:
				{
					// notify & query resource when change print condition or job running
					char* pSectionId = (char*)msg.wParam;
					NotifyAndQueryResource(pSectionId);
					delete pSectionId;
					break;
				}
				default:
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					break;
				}
			}
			break;
		}
	}
}

/**
 * @brief cleanup when exit
 *        also call RequestStopPersChParams
**/
void CRequestUwRwThread::Cleanup()
{
	KillTimerStatusReceive();
	KillTimerPaperReceive();
	if (m_StatusRequested)
	{
		m_RequestUwRw.RequestStopPersChParams(CMakeComposeUwRwData::E_Compose_QueryStatus);
		m_StatusRequested = false;
	}
	if (m_ResourceRequested)
	{
		m_RequestUwRw.RequestStopPersChParams(CMakeComposeUwRwData::E_Compose_QueryResource);
		m_ResourceRequested = false;
	}
	if (m_ReceiveThread.IsStarted())
	{
		m_ReceiveThread.EndThread();
	}
}

const char* CRequestUwRwThread::getDevicename()
{
	return m_DeviceName;
}

};	// namespace uwrw_manager