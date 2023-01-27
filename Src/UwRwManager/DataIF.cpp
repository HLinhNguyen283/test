/**
 *	@file	DataIF.cpp
 *	@brief	data interface class
**/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataIF.h"
#include "PluginCBRProc.h"
#include "PluginCBRProc_OP.h"
#include "CommonUiMsg_OP.h"
#include "CommonUiMsg.h"
#include "ReceiveSignalStatus.h"
#include "PaperDB_Callbacks.h"
#include "ResDef.h"
#include "JobManager_Callbacks.h"

// ==========================================================================

using namespace uwrw_manager;

/**
 *	constructor
**/
CDataIF::CDataIF() : m_receiveUWSignalStatus(CReceiveSignalStatus::GetInstance("UW")), 
					 m_receiveRWSignalStatus(CReceiveSignalStatus::GetInstance("RW")), 
					 m_RequestRewinderThread("RW"), 
					 m_RequestUnwinderThread("UW")
{
	m_displayWarningDialogEnable[0] = false;
	m_displayWarningDialogEnable[1] = false;
	m_displayErrorDialogEnable = true;
	memset(&m_cs, 0, sizeof(ST_CRITICAL_SECTION_INFO));
}

/**
 *	destructor
**/
CDataIF::~CDataIF()
{

}

/**
 *	initialize
 *	@retval TRUE : success
 *	@retval FALSE : fail
**/
BOOL CDataIF::Initialize()
{
	CMakeComposeUwRwData::initialize();
	m_RequestUnwinderThread.Initialize(this);
	m_RequestRewinderThread.Initialize(this);
	CSM_InitializeCriticalSection(&m_cs, MOD_UNWINDER_MANAGER, "UWRWManager", 5000);
	return TRUE;
}

/**
 *	finalize
**/
void CDataIF::Finalize()
{
	m_RequestUnwinderThread.Finalize();
	m_RequestRewinderThread.Finalize();
	CMakeComposeUwRwData::finalize();
	CSM_DeleteCriticalSection(&m_cs);
}

/**
 * system init
 *	@retval TRUE : success
 *	@retval FALSE : fail
 */
BOOL CDataIF::PIM_InitSystem()
{
	m_displayWarningDialogEnable[0] = false;
	m_displayWarningDialogEnable[1] = false;
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1 
	if (m_IniPrinterDescriptor.getUnwinderOption() == 1)
	{
		CReceiveSignalStatus &receiveSignalStatusInstance = CReceiveSignalStatus::GetInstance(DEF_UW);
		receiveSignalStatusInstance.SetRecvPaperInfoStartingStatus(true);
		long nPaperAmount = atoi(m_IniUwRwManager_work.getPaperRemainingAmount(DEF_UW).c_str());
		receiveSignalStatusInstance.SetUWPaperRemainingAmount(nPaperAmount);
		m_RequestUnwinderThread.StartThread();
	}
	if (m_IniPrinterDescriptor.getRewinderOption() == 1) 
	{
		CReceiveSignalStatus &receiveSignalStatusInstance = CReceiveSignalStatus::GetInstance(DEF_RW);
		receiveSignalStatusInstance.SetRecvPaperInfoStartingStatus(true);
		long nPaperAmount = atoi(m_IniUwRwManager_work.getPaperRemainingAmount(DEF_RW).c_str());
		receiveSignalStatusInstance.SetUWPaperRemainingAmount(nPaperAmount);
		m_RequestRewinderThread.StartThread();
	}
	return TRUE;
}

/**
 * system exits
 *	@retval TRUE : success
 *	@retval FALSE : fail
 */
BOOL CDataIF::PIM_ExitSystem()
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1))
	{
		m_RequestUnwinderThread.EndThread();
	}
	if (m_IniPrinterDescriptor.getRewinderOption() == 1)
	{
		m_RequestRewinderThread.EndThread();
	}
	return TRUE;
}

/**
 * callback when job run (first job of sequence)
 * @param inSectionId job section ID
 */
void CDataIF::OnFirstJobRun(const std::string& inSectionId, const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_RequestUnwinderThread.MsgNotifyAndQueryResource(inSectionId);
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_RequestRewinderThread.MsgNotifyAndQueryResource(inSectionId);
	}
}

/**
 * callback when a print condition is set as current
 */
void CDataIF::OnSetCurrentPrintCondition(const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_RequestUnwinderThread.MsgNotifyAndQueryResource();
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_RequestRewinderThread.MsgNotifyAndQueryResource();
	}
}

/**
 * call back when the current print condition is updated
 */
void CDataIF::OnUpdateCurrentPrintCondition(const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_RequestUnwinderThread.MsgNotifyAndQueryResource();
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_RequestRewinderThread.MsgNotifyAndQueryResource();
	}
}

void CDataIF::OnEndJobRun(const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_RequestUnwinderThread.MsgNotifyAndQueryResource();
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_RequestRewinderThread.MsgNotifyAndQueryResource();
	}
}

void CDataIF::GetUWPaperThickness(long &outThickness, const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_receiveUWSignalStatus.GetUWPaperThickness(outThickness);
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_receiveRWSignalStatus.GetUWPaperThickness(outThickness);
	}
}

void CDataIF::GetUWPaperRemainingAmount(long &outPaperRemainingAmount, const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_receiveUWSignalStatus.GetUWPaperRemainingAmount(outPaperRemainingAmount);
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_receiveRWSignalStatus.GetUWPaperRemainingAmount(outPaperRemainingAmount);
	}
}

void CDataIF::HasPaperInfoReceived(bool &outHasPaperInfoReceived, const char* inDeviceName)
{
	// Only do if UW_CONNECT_FUNCTION in PrinterDescriptor is 1
	if ((m_IniPrinterDescriptor.getUnwinderOption() == 1) && !strcmp(inDeviceName, DEF_UW))
	{
		m_receiveUWSignalStatus.HasPaperInfoReceived(outHasPaperInfoReceived);
	}
	if ((m_IniPrinterDescriptor.getRewinderOption() == 1) && !strcmp(inDeviceName, DEF_RW))
	{
		m_receiveRWSignalStatus.HasPaperInfoReceived(outHasPaperInfoReceived);
	}
}

/**
 *	Update display UW status icon and display warning dialog
 *	if UW is offline when control is starting
 *	@param[in] inUWStatus: UW status
**/
void CDataIF::UpdateDisplayUWStatus(bool inUWStatus, const char* inDeviceName)
{
	bool isUW = (strcmp(inDeviceName, DEF_UW) == 0);
	if (inUWStatus)
	{
		if (isUW) {
			PostMessage(GetStatusBarWindow(), UWM_STATUSBAR_UW_STATUS_ON, NULL, NULL);
		}
		m_displayWarningDialogEnable[isUW? 0: 1] = true;
	}
	else
	{
		if (isUW) {
			PostMessage(GetStatusBarWindow(), UWM_STATUSBAR_UW_STATUS_OFF, NULL, NULL);
		}
		if (m_displayWarningDialogEnable[isUW ? 0 : 1])
		{
			char erroMsg[512] = { 0 };
			_snprintf(erroMsg, sizeof(erroMsg) - 1, "%d\n%s", (ID_MESSAGE_UNWINDERMANAGER + IDM_NOTIFY_UW_STATUS), (char*)LoadResourceString(IDM_NOTIFY_UW_STATUS, RESOURCE_MSG_STR), inDeviceName);
			ShowMessageBox(erroMsg, MBST_ICONERROR | MBST_OK | MBST_MODELESS, NULL);
			m_displayWarningDialogEnable[isUW ? 0 : 1] = false;
		}
	}
}

/**
 *	Check whether or not update of paper thickness value notified from UW to the current print condition is requested.
 *	@param[in] inCurrentPaperThickness: paper thickness value of the current print condition
 *	@param[out] outIsUpdateRequested: whether or not the update is requested (true: requested, false: not requested)
**/
void CDataIF::CheckUpdateUWPaperThicknessForCurrentPrintCondition(long inCurrentPaperThickness, bool &outIsUpdateRequested)
{
	bool isUpdateRequested = false;
	bool hasReceivedPaperInfo = false;
	HasPaperInfoReceived(hasReceivedPaperInfo);
	// Only update the paper thickness when the paper thickness has been received from UW
	if (hasReceivedPaperInfo) {
		long UWThickness = THICKNESS_INVALID_VALUE;
		GetUWPaperThickness(UWThickness);
		if (UWThickness != THICKNESS_INVALID_VALUE) {
			if (inCurrentPaperThickness != UWThickness) {
				std::stringstream msg;
				msg << ID_MESSAGE_UNWINDERMANAGER + IDM_UPDATE_PAPER_THICKNESS_TO_CURRENT_PRINT_CONDITION << "\n" << LoadResourceString(IDM_UPDATE_PAPER_THICKNESS_TO_CURRENT_PRINT_CONDITION, RESOURCE_MSG_STR);
				int result = ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_YESNO | MBST_ICONWARNING, NULL);
				if (result == IDYES) {
					isUpdateRequested = true;
				}
			}
		}
		else {
			WriteToLogBuf(LOG_DEBUG, "CDataIF::CheckUpdateUWPaperThicknessForCurrentPrintCondition() error - Invalid UW paper thickness value.");
		}
	}
	outIsUpdateRequested = isUpdateRequested;
}

void CDataIF::getDisplayErrorDialogEnableRef(bool& outDisplayErrEnable)
{
	outDisplayErrEnable = m_displayErrorDialogEnable;
}

void CDataIF::getCriticalSectionRef(ST_CRITICAL_SECTION_INFO & outCs)
{
	outCs = m_cs;
}

/**
 *	Update paper thickness value notified from UW to the top job
 *	@param[in] inSectionID: section ID of job
 *	@param[out] outIsUpdateRequested: Update of the job paper thickness is requested or not (true: requested, false: not requested)
**/
void CDataIF::UpdateUWPaperThicknessForJob(const std::string &inSectionID, bool &outIsUpdateRequested)
{
	// Reflect paper thickness info which is notified from UW to the print condition of the top job when press run it.
	bool isReflectPaperThickness = false;
	SJobManager_Callbacks jobManagerCallbacks;
	if (JM_GetCallbacks(&jobManagerCallbacks)) {
		bool hasReceivedPaperInfo = false;
		HasPaperInfoReceived(hasReceivedPaperInfo);
		bool isReflectUWPaperThickness = false;
		// Only update the job print condition when there has been the paper info received from UW.
		if (hasReceivedPaperInfo) {
			long jobThickness;
			const char* jobSectionID = inSectionID.c_str();
			if (jobManagerCallbacks.JM_GetPaperThickness(jobSectionID, jobThickness)) {
				long UWThickness = THICKNESS_INVALID_VALUE;
				GetUWPaperThickness(UWThickness);
				if (UWThickness != THICKNESS_INVALID_VALUE) {
					if (UWThickness != jobThickness) {
						std::stringstream msg;
						msg << ID_MESSAGE_UNWINDERMANAGER + IDM_UPDATE_PAPER_THICKNESS_TO_JOB << "\n" << LoadResourceString(IDM_UPDATE_PAPER_THICKNESS_TO_JOB, RESOURCE_MSG_STR);
						int result = ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_YESNO | MBST_ICONWARNING, NULL);
						if (result == IDYES) {
							if (jobManagerCallbacks.JM_SetPaperThickness(jobSectionID, UWThickness)) {
								jobManagerCallbacks.JM_SetPrintConditionEditFlag(jobSectionID, TRUE);
								if (!jobManagerCallbacks.JM_SaveJobFile(jobSectionID)) {
									WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForJob() error - Cannot save to the job file");
								}
							}
							else {
								WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForJob() error - Cannot save paper thickness to job");
							}
							isReflectPaperThickness = true;
						}
					}
				}
				else {
					WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForJob() error - Invalid UW paper thickness value.");
				}
			}
			else {
				WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForJob() error - Cannot get paper thickness of job.");
			}
		}
	}
	else {
		WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForJob() error - Cannot get job manager callback.");
	}
	outIsUpdateRequested = isReflectPaperThickness;
}

/**
 *	Update paper thickness value notified from UW to consecutive jobs
 *	@param[in] inConsecutiveJobSectionIDs: vector of section IDs of consecutive jobs
 *	@param[out] outIsUpdateRequested: Update of the job paper thickness is requested or not (true: requested, false: not requested)
**/
void CDataIF::UpdateUWPaperThicknessForConsecutiveJobs(const std::vector<std::string> &inConsecutiveJobSectionIDs, bool &outIsUpdateRequested)
{
	// Reflect paper thickness info which is notified from UW to the print condition of jobs.
	bool isReflectPaperThickness = false;
	SJobManager_Callbacks jobManagerCallbacks;
	if (JM_GetCallbacks(&jobManagerCallbacks)) {
		bool hasReceivedPaperInfo = false;
		HasPaperInfoReceived(hasReceivedPaperInfo);
		// Only update the job print condition when there has been the paper info received from UW.
		if (hasReceivedPaperInfo) {
			long UWThickness = THICKNESS_INVALID_VALUE;
			GetUWPaperThickness(UWThickness);
			if (UWThickness != THICKNESS_INVALID_VALUE) {
				long jobThickness;
				for (UINT idx = 0; idx < inConsecutiveJobSectionIDs.size(); idx++) {
					if (jobManagerCallbacks.JM_GetPaperThickness(inConsecutiveJobSectionIDs.at(idx).c_str(), jobThickness)) {
						if (UWThickness != jobThickness) {
							std::stringstream msg;
							msg << ID_MESSAGE_UNWINDERMANAGER + IDM_UPDATE_PAPER_THICKNESS_TO_JOB << "\n" << LoadResourceString(IDM_UPDATE_PAPER_THICKNESS_TO_JOB, RESOURCE_MSG_STR);
							int result = ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_YESNO | MBST_ICONWARNING, NULL);
							if (result == IDYES) {
								isReflectPaperThickness = true;
							}
							break;
						}
					}
					else {
						WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Cannot get paper thickness of job.");
					}
				}

				if (isReflectPaperThickness) {
					for (UINT idx = 0; idx < inConsecutiveJobSectionIDs.size(); idx++) {
						const char* sectionID = inConsecutiveJobSectionIDs.at(idx).c_str();
						if (jobManagerCallbacks.JM_GetPaperThickness(sectionID, jobThickness)) {
							if (UWThickness != jobThickness) {
								if (jobManagerCallbacks.JM_SetPaperThickness(sectionID, UWThickness)) {
									jobManagerCallbacks.JM_SetPrintConditionEditFlag(sectionID, TRUE);
									if (!jobManagerCallbacks.JM_SaveJobFile(sectionID)) {
										WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Cannot save to the job file");
									}
								}
								else {
									WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Cannot save paper thickness to job");
								}
							}
						}
						else {
							WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Cannot get paper thickness of job.");
						}
					}
				}
			}
			else {
				WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Invalid UW paper thickness value.");
			}
		}
	}
	else {
		WriteToLogBuf(LOG_DEBUG, "CDataIF::UpdateUWPaperThicknessForConsecutiveJobs() error - Cannot get Job manager callback.");
	}
	outIsUpdateRequested = isReflectPaperThickness;
}
