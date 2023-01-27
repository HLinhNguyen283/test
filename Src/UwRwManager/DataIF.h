/**
 *	@file	DataIF.h
 *	@brief	data interface class
**/

#if !defined(_DATA_IF_H_)
#define _DATA_IF_H_




// ==========================================================================
// Include files

#include "BaseDataIF.h"
#include "MakeComposeUwRwData.h"
#include "RequestUwRwThread.h"
#include <vector>
// ==========================================================================

/**
 *	data interface class
**/
class CDataIF : public CBaseDataIF,
				public uwrw_manager::CMakeComposeUwRwData
{
public:
	//method
	CDataIF();
	virtual ~CDataIF();

	virtual BOOL Initialize();
	virtual void Finalize();

	virtual BOOL PIM_InitSystem();
	virtual BOOL PIM_ExitSystem();

	void OnSetCurrentPrintCondition(const char* inDeviceName = DEF_UW);

	void OnUpdateCurrentPrintCondition(const char* inDeviceName = DEF_UW);

	void OnFirstJobRun(const std::string& inSectionId, const char* inDeviceName = DEF_UW);

	void OnEndJobRun(const char* inDeviceName = DEF_UW);

	void GetUWPaperThickness(long &outThickness, const char* inDeviceName = DEF_UW);
	void GetUWPaperRemainingAmount(long &outPaperRemainingAmount, const char* inDeviceName = DEF_UW);
	void HasPaperInfoReceived(bool &outHasPaperInfoReceived, const char* inDeviceName = DEF_UW);

	void UpdateDisplayUWStatus(bool inUWStatus, const char* inDeviceName);

	void UpdateUWPaperThicknessForJob(const std::string &inSectionID, bool &outIsUpdateRequested);
	void UpdateUWPaperThicknessForConsecutiveJobs(const std::vector<std::string> &inConsecutiveJobSectionIDs, bool &outIsUpdateRequested);
	void CheckUpdateUWPaperThicknessForCurrentPrintCondition(long inCurrentPaperThickness, bool &outIsUpdateRequested);

	void getDisplayErrorDialogEnableRef(bool& outDisplayErrEnable);
	void getCriticalSectionRef(ST_CRITICAL_SECTION_INFO& outCs);
protected:
	//method

	//event

	//parts

	//member

private:
	uwrw_manager::CRequestUwRwThread m_RequestUnwinderThread;
	uwrw_manager::CRequestUwRwThread m_RequestRewinderThread;
	uwrw_manager::CReceiveSignalStatus &m_receiveUWSignalStatus;
	uwrw_manager::CReceiveSignalStatus &m_receiveRWSignalStatus;

	bool m_displayWarningDialogEnable[2];
	volatile bool m_displayErrorDialogEnable;
	ST_CRITICAL_SECTION_INFO m_cs;
};




#endif // !defined(_DATA_IF_H_)
