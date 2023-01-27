/**
 *	@file	Plugin_IF.cpp
 *	@brief	HeadCleaningSetting I/F
**/

//////////////////////
//Include file
#include "stdafx.h"
#include "Common.h"
#include "PluginCommon.h"
#include "Plugin_IF.h"
#include "PluginCommon.h"
#include "UwRWManager_OP.h"
#include "Ini_PrinterDescriptor.h"
#include "UwRwManager_Callbacks.h"
#include <assert.h>

//////////////////////
//Contant definition

// ==========================================================================
// Local variables
static CUwRwManager_OP* st_unwinderManager_OP = NULL;			//!< Operating environment setting plug OP class.


// ==========================================================================
// Local definitions

/**
 *	Module class object creation
 *	@param[in]	pHost : Pointer to host information
 *	@param[out]	moduleObject : Module class pointer
 *	@retval true : Success
 *	@retval false : Failure
**/
BOOL CreateModuleObject(PHOST pHost, void **moduleObject)
{	
	if(!pHost || !moduleObject){
		return FALSE;
	}

	st_unwinderManager_OP = new CUwRwManager_OP();
	*moduleObject = dynamic_cast<CPlugin_Common*>(st_unwinderManager_OP);

	return (*moduleObject != NULL);
}

/**
 * callback when job run (first job of sequence)
 *
 * @param inSectionId job section ID
 */
void OnSetCurrentPrintCondition()
{
	if (st_unwinderManager_OP == nullptr)
	{
		return;
	}
	return st_unwinderManager_OP->OnSetCurrentPrintCondition();
}

/**
 * callback when a print condition is set as current
 */
void OnUpdateCurrentPrintCondition()
{
	if (st_unwinderManager_OP == nullptr)
	{
		return;
	}
	return st_unwinderManager_OP->OnUpdateCurrentPrintCondition();
}

/**
 * call back when the current print condition is updated
 */
void OnFirstJobRun(const std::string& inSectionId)
{
	if (st_unwinderManager_OP == nullptr)
	{
		return;
	}
	st_unwinderManager_OP->OnFirstJobRun(inSectionId);
}

/**
 * callback when job running status is released
 */
void OnEndJobRun()
{
	if (st_unwinderManager_OP)
	{
		st_unwinderManager_OP->OnEndJobRun();
	}
}

void GetUWPaperThickness(long &outThickness)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->GetUWPaperThickness(outThickness);
	}
}

void GetUWPaperRemaingAmount(long &outPaperRemainingAmount)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->GetUWPaperRemainingAmount(outPaperRemainingAmount);
	}
}

void HasPaperInfoReceived(bool &outHasPaperInfoReceived)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->HasPaperInfoReceived(outHasPaperInfoReceived);
	}
}

void CheckUpdateUWPaperThicknessForCurrentPrintCondition(long inCurrentPaperThickness, bool &outIsUpdateRequested)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->CheckUpdateUWPaperThicknessForCurrentPrintCondition(inCurrentPaperThickness, outIsUpdateRequested);
	}
}

void UpdateUWPaperThicknessForJob(const std::string &inSectionID, bool &outIsUpdateRequested)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->UpdateUWPaperThicknessForJob(inSectionID, outIsUpdateRequested);
	}
}

void UpdateUWPaperThicknessForConsecutiveJobs(const std::vector<std::string> &inConsecutiveJobSectionIDs, bool &outIsUpdateRequested)
{
	if (st_unwinderManager_OP) {
		st_unwinderManager_OP->UpdateUWPaperThicknessForConsecutiveJobs(inConsecutiveJobSectionIDs, outIsUpdateRequested);
	}
}

/**
 *  @brief	Get the UnwinderManager callback
 *  @param[out]	outCallbacks : callback struct
 *  @retval true : Success
 *  @retval false : Failure
**/
PLUGIN_MODULE_API bool _UnwinderManager_GetCallbacks(struct SUwRwManager_Callbacks* outCallbacks)
{
	if (!outCallbacks) {
		return false;
	}
	if (outCallbacks->StructVersion < DEF_UWRWMANAGER_CALLBACKS_VERSION_1) {
		assert(outCallbacks->StructVersion >= DEF_UWRWMANAGER_CALLBACKS_VERSION_1);
		return false;
	}
	//Version 1
	outCallbacks->OnSetCurrentPrintCondition	= OnSetCurrentPrintCondition;
	outCallbacks->OnUpdateCurrentPrintCondition = OnUpdateCurrentPrintCondition;
	outCallbacks->OnFirstJobRun					= OnFirstJobRun;
	outCallbacks->GetUWPaperThickness			= GetUWPaperThickness;
	outCallbacks->GetUWPaperRemaingAmount		= GetUWPaperRemaingAmount;
	outCallbacks->HasPaperInfoReceived			= HasPaperInfoReceived;

	//Version 2
	outCallbacks->OnEndJobRun = OnEndJobRun;
	outCallbacks->CheckUpdateUWPaperThicknessForCurrentPrintCondition = CheckUpdateUWPaperThicknessForCurrentPrintCondition;
	outCallbacks->UpdateUWPaperThicknessForJob = UpdateUWPaperThicknessForJob;
	outCallbacks->UpdateUWPaperThicknessForConsecutiveJobs = UpdateUWPaperThicknessForConsecutiveJobs;

#pragma message(__MSG__ "SUwRwManager_Callbacksのバージョンに合わせて、必要な関数のみセットしてください")
	return true;
}
