/**
 *	@file	UnwinderManager_OP.cpp
 *	@brief	CUwRwManager_OP
**/

#include "stdafx.h"
#include "UwRwManager_OP.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "CommonDef_OP.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

/**
 *	Constructor
**/
CUwRwManager_OP::CUwRwManager_OP()
{
	m_dataIF = NULL;
}

/**
 *	Destructor
**/
CUwRwManager_OP::~CUwRwManager_OP()
{
	m_dataIF = NULL;
}

/**
 * callback when a print condition is set as current
 */
void CUwRwManager_OP::OnSetCurrentPrintCondition()
{
	if (m_dataIF == nullptr) {
		return;
	}
	m_dataIF->OnSetCurrentPrintCondition();
}

/**
 * call back when the current print condition is updated
 */
void CUwRwManager_OP::OnUpdateCurrentPrintCondition()
{
	if (m_dataIF == nullptr) {
		return;
	}
	m_dataIF->OnUpdateCurrentPrintCondition();
}

/**
 * callback when job run (first job of sequence)
 *
 * @param inSectionId job section ID
 */
void CUwRwManager_OP::OnFirstJobRun(const std::string& inSectionId)
{
	if (m_data == nullptr)
	{
		return;
	}

	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
	dataIF->OnFirstJobRun(inSectionId);
}

/**
 *	call back when job running status is released
 */
void CUwRwManager_OP::OnEndJobRun()
{
	if (m_dataIF) {
		m_dataIF->OnEndJobRun();
	}
}

void CUwRwManager_OP::GetUWPaperThickness(long &outThickness)
{
	if (m_dataIF) {
		m_dataIF->GetUWPaperThickness(outThickness);
	}
}

void CUwRwManager_OP::GetUWPaperRemainingAmount(long &outUWPaperRemainingAmount)
{
	if (m_dataIF) {
		m_dataIF->GetUWPaperRemainingAmount(outUWPaperRemainingAmount);
	}
}

void CUwRwManager_OP::HasPaperInfoReceived(bool &outHasPaperInfoReceived)
{
	if (m_dataIF) {
		m_dataIF->HasPaperInfoReceived(outHasPaperInfoReceived);
	}
}

void CUwRwManager_OP::UpdateUWPaperThicknessForJob(const std::string &inSectionID, bool &outIsUpdateRequested)
{
	if (m_dataIF) {
		m_dataIF->UpdateUWPaperThicknessForJob(inSectionID, outIsUpdateRequested);
	}
}

void CUwRwManager_OP::UpdateUWPaperThicknessForConsecutiveJobs(const std::vector<std::string> &inConsecutiveJobSectionIDs, bool &outIsUpdateRequested)
{
	if (m_dataIF) {
		m_dataIF->UpdateUWPaperThicknessForConsecutiveJobs(inConsecutiveJobSectionIDs, outIsUpdateRequested);
	}
}

void CUwRwManager_OP::CheckUpdateUWPaperThicknessForCurrentPrintCondition(long inCurrentPaperThickness, bool &outIsUpdateRequested)
{
	if (m_dataIF) {
		m_dataIF->CheckUpdateUWPaperThicknessForCurrentPrintCondition(inCurrentPaperThickness, outIsUpdateRequested);
	}
}

/**
 *	Set plug-in module configuration information
 *	@param[out]	pModData : Pointer to module configuration information
 *	@retval : PLUGERR_NONE
**/
long CUwRwManager_OP::BuildUiModuleInfo(MODULE_INFO *pModData)
{
	pModData->MODULE_ID = MOD_UNWINDER_MANAGER;
	pModData->MODULE_VERSION = PLUGIN_VERSION;
	pModData->MODULE_TYPE = PIM_PROCESS;
	pModData->REQUIRE_CRITSEC = FALSE;
	pModData->INSTALL_SERVICE = FALSE;
	pModData->SERVICE_ADDRESS = NULL;
	pModData->UI_TYPE = UI_NONE;
	
	return PLUGERR_NONE;
}

/**
 *	Set the callback function for each system.
 *	@param[in]	inHostInfo : Host information
**/
void CUwRwManager_OP::SetSystemCallBackFunc(HOST inHostInfo)
{
	SetCommonCallBackFunc_OP(inHostInfo);
}

/**
 *	Create a data I / F class instance
 *	@retval true : Success
 *	@retval false : Failure
**/
BOOL CUwRwManager_OP::CreateDataIF()
{
	if(!m_data){
		CDataIF* data = new CDataIF();
		m_data = dynamic_cast<CBaseDataIF*>(data);
		m_dataIF = data;
	}
	return m_data != NULL;
}

/**
 *	Create a printer I / F class instance
 *	@retval true : Success
 *	@retval false : Failure
**/
BOOL CUwRwManager_OP::CreatePrinterIF()
{
	return FALSE;
}
