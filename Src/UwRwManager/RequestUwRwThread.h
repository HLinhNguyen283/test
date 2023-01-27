/**
 * @file	RequestUnwinderThread.h
 * @brief	Main process thread
**/

#pragma once

#include "RequestUwRw.h"
#include "Common.h"
#include "ReceiveSignalStatusThread.h"

class CDataIF;

namespace uwrw_manager
{

class CUwRwPaper;

/**
 *	@brief Main process thread
**/
class CRequestUwRwThread
{
public:

	/**
	 * @brief CRequestUwRwThread constructor
	**/
	CRequestUwRwThread(const char* inDeviceName);

	/**
	 * @brief CRequestUwRwThread destructor
	**/
	virtual ~CRequestUwRwThread();

	/**
	 * @brief initialize data
	 * @param[in] inDataIF plugin DataIF object
	**/
	void Initialize(CDataIF* inDataIF);

	/**
	 * @brief finalize data
	**/
	void Finalize();

	/**
	 * @brief start the thread
	**/
	void StartThread();

	/**
	 * @brief stop the thread
	**/
	void EndThread();

	/**
	 * @brief put thread to sleep also check for thread exit or Receiver end
	 * Call from RequestUnwinder function
	 * @param[in] inMillisec time to sleep in milliseconds
	 * @return true if thread exit or Receiver end / false if not
	**/
	bool WaitCheckExit(int inMillisec);

	/**
	 * @brief call from callback function to notify and then query UW paper info
	 * @param inSectionId empty if call from set current condition 
	 *                    section ID if called from job running process
	**/
	void MsgNotifyAndQueryResource(const std::string& inSectionId = "");

	/**
	 * @brief sent msg from ReceiveSignalStatus thread (to call SetTimerStatusReceive in this thread)
	**/
	void MsgSetTimerStatusReceive();

	/**
	 * @brief sent msg from ReceiveSignalStatus thread (to call SetTimerPaperReceive in this thread)
	**/
	void MsgSetTimerPaperReceive();

	/**
	 * @brief  setting for timeout timer of SignalStatus(STATUS)
	**/
	void SetTimerStatusReceive();

	/**
	 * @brief  setting for timeout timer of SignalStatus(PAPER)
	**/
	void SetTimerPaperReceive();

	/**
	 * @brief  stop timeout timer of SignalStatus(STATUS)
	**/
	void KillTimerStatusReceive();

	/**
	 * @brief  stop timeout timer of SignalStatus(PAPER)
	**/
	void KillTimerPaperReceive();

	const char* getDevicename();

private:

	/**
	 * @brief callback function to process thread, will call to ThreadProc
	 * @param [in] inParam CRequestUwRwThread object
	 * @return 0
	**/
	static UINT WINAPI threadEventProc(LPVOID inParam);

	/**
	 * @brief main thread processing function
	**/
	void ThreadProc();

	/**
	 * @brief check if UWandRW_Receiver.exe running
	 *        if not display warning dialog and wait until it running
	**/
	void CheckReceiverRunning();

	/**
	 * @brief register UW status channel
	**/
	void CheckUWRWStatus();

	/**
	 * @brief 1. notify UW paper info
	 *        2. register UW paper info channel
	 * @param inSectionId empty if call from set current condition
	 *                    section ID if called from job running process
	**/
	void NotifyAndQueryResource(const std::string& inSectionId = "");

	/**
	 * @brief Get necessary paper info from current print condition
	 * @param[out] outUnwinderPaper paper info
	 * @return true if get success/ false if get fail
	**/
	bool GetPrintConditionResourceInfo(CUwRwPaper& outUnwinderPaper);

	/**
	 * @brief Get necessary paper info from job print condition
	 * @param[out] outUnwinderPaper paper info
	 * @param[in] inSectionId job section ID
	 * @return true if get success/ false if get fail
	**/
	bool GetJobResourceInfo(CUwRwPaper& outUnwinderPaper, const std::string& inSectionId);

	/**
	 * @brief process messages
	**/
	void CheckEvents();

	/**
	 * @brief cleanup when exit
	 *        also call RequestStopPersChParams
	**/
	void Cleanup();

	CRequestUwRw m_RequestUwRw;
	ST_THREAD_INFO m_thread;
	bool m_ExitThread;
	bool m_ReceiverRunning;
	bool m_StatusRequested;
	bool m_ResourceRequested;
	HANDLE m_ExitThreadEvent;
	HANDLE m_ReceiverProcess;
	CReceiveSignalStatusThread m_ReceiveThread;
	CDataIF* m_dataIF;
	UINT m_PaperReceivingTimeId;
	UINT m_StatusReceivingTimeId;
	const char* m_DeviceName;
};

};	// namespace uwrw_manager