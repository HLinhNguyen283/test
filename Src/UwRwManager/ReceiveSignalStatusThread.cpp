/**
 * @file	ReceiveSignalStatusThread.cpp
 * @brief	Signal Status process thread
**/

#include "stdafx.h"
#include "CommonCBRProc.h"
#include "ReceiveSignalStatus.h"
#include "ReceiveSignalStatusThread.h"
#include "DataIF.h"

namespace uwrw_manager
{

/**
 * @brief constructor
**/
CReceiveSignalStatusThread::CReceiveSignalStatusThread(const char* inDeviceName)
{
	memset(&m_Thread, 0x00, sizeof(ST_THREAD_INFO));
	m_ExitThreadEvent = CreateEvent(NULL, TRUE, FALSE, "End thread event");
	m_dataIF = NULL; 
	m_DeviceName = inDeviceName;
}

/**
 * @brief destructor
**/
CReceiveSignalStatusThread::~CReceiveSignalStatusThread()
{

}

/**
 * @brief start the thread
**/
void CReceiveSignalStatusThread::StartThread(CDataIF* dataIF)
{
	m_dataIF = dataIF;
	UINT threadID = 0;
	CReceiveSignalStatus &receiveSignalStatusInstance = uwrw_manager::CReceiveSignalStatus::GetInstance(m_DeviceName);
	receiveSignalStatusInstance.SetExit(false);
	m_Thread = TM_CreateThread(nullptr, 0, ThreadFunction, this, 0, &threadID, _T("UwRwManager.dll"), _T("SignalStatusThread"), nullptr);
}

/**
 * @brief stop the thread
**/
void CReceiveSignalStatusThread::EndThread()
{
	SetEvent(m_ExitThreadEvent);
	CReceiveSignalStatus &receiveSignalStatusInstance = uwrw_manager::CReceiveSignalStatus::GetInstance(m_DeviceName);
	receiveSignalStatusInstance.SetExit(true);
	CancelSynchronousIo(m_Thread.thread_handle);
	TM_DeleteThread(&m_Thread);
	m_Thread.thread_handle = NULL;
}

/**
 * @brief check if thread is started
 * @return true if started / false if not
**/
bool CReceiveSignalStatusThread::IsStarted()
{
	return m_Thread.thread_handle != NULL;
}

/**
 * @brief callback function to process thread, will call to non staitc ThreadFunction
 * @param pData CReceiveSignalStatusThread object
 * @return 0
**/
UINT WINAPI CReceiveSignalStatusThread::ThreadFunction(LPVOID pData)
{
	CReceiveSignalStatusThread* receiveThread = reinterpret_cast<CReceiveSignalStatusThread*>(pData);
	receiveThread->ThreadFunction();
	return 0;
}

/**
 * @brief main thread processing function
**/
void CReceiveSignalStatusThread::ThreadFunction()
{
	uwrw_manager::CReceiveSignalStatus &receiveSignalStatusInstance = uwrw_manager::CReceiveSignalStatus::GetInstance(m_DeviceName);
	receiveSignalStatusInstance.ReceiveSignalStatusInfo(m_dataIF);
}

};	// namespace uwrw_manager