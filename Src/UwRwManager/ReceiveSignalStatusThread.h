/**
 * @file	ReceiveSignalStatusZThread.h
 * @brief	Signal Status process thread
**/

#pragma once
#include "ReceiveSignalStatus.h"

class CDataIF;

namespace uwrw_manager
{

class CRequestUwRwThread;

/**
 * @brief Signal Status process thread
**/
class CReceiveSignalStatusThread
{
public :
	/**
	 * @brief constructor
	**/
	CReceiveSignalStatusThread(const char* inDeviceName);

	/**
	 * @brief destructor
	**/
	virtual ~CReceiveSignalStatusThread();

	/**
	 * @brief start the thread
	**/
	void StartThread(CDataIF* dataIF);

	/**
	 * @brief stop the thread
	**/
	void EndThread();

	/**
	 * @brief check if thread is started
	 * @return true if started / false if not
	**/
	bool IsStarted();

private:

	/**
	 * @brief callback function to process thread, will call to non staitc ThreadFunction
	 * @param pData CReceiveSignalStatusThread object
	 * @return 0
	**/
	static UINT WINAPI ThreadFunction(LPVOID pData);

	/**
	 * @brief main thread processing function
	**/
	void ThreadFunction();

	ST_THREAD_INFO m_Thread;
	HANDLE m_ExitThreadEvent;
	CDataIF* m_dataIF;
	const char* m_DeviceName;
};
};	// namespace uwrw_manager