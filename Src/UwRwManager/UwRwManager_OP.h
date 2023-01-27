/**
 *	@file	UnwinderManager_OP.h
 *	@brief	CUwRwManager_OP
**/

#if !defined(__UWRW_MANAGER_OP__)
#define __UWRW_MANAGER_OP__

#include "UwRwManager.h"
#include "DataIF.h"

#define PLUGIN_VERSION 0x0000

/**
 * 
 */
class CUwRwManager_OP : public CUwRwManager
{
public:
	CUwRwManager_OP();
	virtual ~CUwRwManager_OP();

	/**
	 * callback when a print condition is set as current
	 */
	void OnSetCurrentPrintCondition();

	/**
	 * call back when the current print condition is updated
	 */
	void OnUpdateCurrentPrintCondition();

	/**
	 * callback when job run (first job of sequence)
	 *
	 * @param inSectionId job section ID
	 */
	void OnFirstJobRun(const std::string& inSectionId);

	/**
	 * call back when job running status is released
	 */
	void OnEndJobRun();

	void GetUWPaperThickness(long &outThickness);
	void GetUWPaperRemainingAmount(long &outUWPaperRemainingAmount);
	void HasPaperInfoReceived(bool &outHasPaperInfoReceived);

	void UpdateUWPaperThicknessForJob(const std::string &inSectionID, bool &outIsUpdateRequested);
	void UpdateUWPaperThicknessForConsecutiveJobs(const std::vector<std::string> &inConsecutiveJobSectionIDs, bool &outIsUpdateRequested);
	void CheckUpdateUWPaperThicknessForCurrentPrintCondition(long inCurrentPaperThickness, bool &outIsUpdateRequested);

protected:

	virtual long BuildUiModuleInfo(MODULE_INFO* pModData);
	virtual void SetSystemCallBackFunc(HOST inHostInfo);
	virtual BOOL CreateDataIF();
	virtual BOOL CreatePrinterIF();

	CDataIF* m_dataIF;
};

#endif // !defined(__UWRW_MANAGER_OP__)
