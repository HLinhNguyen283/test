/*! \file DataPaperSettings.h
 *  \brief data paper settings class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
#if !defined(_DATA_PAPER_SETTINGS_H_)
#define _DATA_PAPER_SETTINGS_H_

// ==========================================================================
// Include files

#include <Windows.h>
#include <string>
#include "DataSystemSetting.h"
#include "JobDef_PrintCondition_OP.h"
using namespace std;

// ==========================================================================


/*! \brief data interface class
*/
class CDataPaperSettings
{
public:
	//method
	CDataPaperSettings();
	virtual ~CDataPaperSettings();
	//
	virtual BOOL Initialize(CDataSystemSetting* inSystemSetting);
	virtual void Finalize();

	bool IsChanged();
	bool IsPaperSizeChanged();
	bool Save();
	void OnCopyJobPrintCondition(PrintConditionGUI::ST_COPY_TARGET_PAPER_SETTINGS &outPaperSettings);

	long GetPaperWidth();
	void SetPaperWidth(long inWidth);

	bool GetUsePaperFeedLengthSetting();
	void SetUsePaperFeedLengthSetting(bool inUse);
	long GetPaperFeedLength();
	void SetPaperFeedLength(long inLength);

	long GetPaperThickness();
	void SetPaperThickness(long inThickness);
	void SetCurrentPaperThickness(long inThickness);

	bool GetPaperFeedUnitList(long inItemID, const char** outUnit);
	bool GetSelectPaperFeedUnit(long* outItemID);
	void SetSelectPaperFeedUnit(long inItemID);

	bool GetOptionTractorHole();
	void SetOptionTractorHole(bool inUse);
	bool GetOptionTractorHolesAndPerforations();
	void SetOptionTractorholesAndPerforations(bool inUse);
	long GetOptionPerforationInterval();
	void SetOptionPerforationInterval(long inInterval);

	bool GetOptionPreprintedMark(DEF_PRINT_SIDE* outMarkSide);
	void SetOptionPreprintedMark(bool inUse);
	long GetOptionPreprintedMarkOffset();
	void SetOptionPreprintedMarkOffset(long inOffset);

	bool GetOptionPreprintedPaper();
	void SetOptionPreprintedPaper(bool inUse);

	bool GetDynamicNozzleShift();
	void SetDynamicNozzleShift(bool inUse);

	bool InitSelectPreprintedMarkSideDlg();
	void SetSelectPreprintedMarkSide_SelectDlg(DEF_PRINT_SIDE inSide);
	bool ExitSelectPreprintedMarkSideDlg();

	void UpdatePrintCondition(const char* inPrintConditionOrSection);

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();

	DEF_PRINT_SIDE GetPaperPreviewSide();
	void SetPaperPreviewSide(DEF_PRINT_SIDE inSide);
	bool GetPaperPreview(DEF_PRINT_SIDE inSide, HBITMAP* outImage);
	HBITMAP LoadJpegImage(DEF_PRINT_SIDE inSide);

	BOOL GetNonInspectionRect(long inItemID, DEF_PRINT_SIDE inSide, RECT& outRect);
	void GetEliminatedInfo(DEF_PRINT_SIDE inSide, bool inIsLeft, double& outLeft, double& outRight);
	BOOL GetMaxMinPaperSize(long& outMaxWidth, long& outMinWidth, long& outMaxHeight, long& outMinHeight);
	BOOL GetVerifyMarkOffset(DEF_PRINT_SIDE inSide, double& outOffsetX, double& outOffsetY);
	BOOL GetEnablePrintStartMark();
	BOOL GetVerifyMarkTarget();
	eUNIT_MODE GetPaperLengthUnit();
	BOOL GetPaperReso(DEF_PRINT_SIDE inSide, double& outResoX, double& outResoY);
	void GetRipSizeH(double& outRipHeight);

	bool GetDuplicateJIAreaSettings();
	void SetDuplicateJIAreaSettings(bool inState);
	double GetDuplicateInitNonInspectionAreaDefault();

	/**
		@brief	坪量の取得
		@retval	坪量
	*/
	double getPaperWeight();

	/**
		@brief	坪量の設定
		@param[in]	inPaperWeight : 坪量
	*/
	void setPaperWeight(const double inPaperWeight);

protected:
	//member
	CDataSystemSetting* m_SystemSetting;
	SPaperDB_Callbacks* m_PaperDB_Callbacks;				//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;					//!< Callbacks for Job Manager

	char m_PrintConditionOrSection[256];					//!< In case the GUI is started from Job List Screen : SectionID
															//!< In case the GUI is started from System Setting Screen : Print Condition Name

	vector<eUNIT_MODE> m_PaperLengthUnits;					//!< List of unit mode

	long m_CurrentPaperWidth;								//!< The current paper width
	long m_PaperWidth;										//!< The select paper width

	ST_PDB_PAPER_SIZE_H m_CurrentPaperLength;				//!< The current paper length
	ST_PDB_PAPER_SIZE_H m_PaperLength;						//!< The select paper length

	long m_CurrentPaperThickness;							//!< The current paper thickness
	long m_PaperThickness;									//!< The select paper thickness

	ST_PDB_PAPER_OPTION m_CurrentPaperOption;				//!< The current paper option
	ST_PDB_PAPER_OPTION m_PaperOption;						//!< The paper option

	long m_PaperWidthMin;									//!< The min paper width
	long m_PaperWidthMax;									//!< The max paper width
	long m_PaperWidthDef;									//!< The default paper width

	long m_PaperLengthMin;									//!< The min paper length
	long m_PaperLengthMax;									//!< The max paper length
	long m_PaperLengthDef;									//!< The default paper length

	long m_PaperThicknessMin;								//!< The min paper thickness
	long m_PaperThicknessMax;								//!< The max paper thickness
	long m_PaperThicknessDef;								//!< The default paper thickness

	long m_PatternIntervalMin;								//!< The min pattern interval
	long m_PatternIntervalMax;								//!< The max pattern interval
	long m_PatternIntervalDef;								//!< The default pattern interval

	long m_PrePrintMarkOffset;								//!< The pre-print mark offset

	bool m_CurrentDynamicNozzleShift;						//!< Current dynamic nozzle shift
	bool m_DynamicNozzleShift;								//!< Dynamic nozzle shift

	DEF_PRINT_SIDE m_PrintSideSelection;					//!< The paper preview side
	HBITMAP m_BitmapPaperReview[DEF_PRINT_BOTH];			//!< The paper preview

	static const char* st_unit_mm;							//!< unit of length(mm)
	static const char* st_unit_inch;						//!< unit of length(inch)
	static const char* st_unit_1_2_inch;					//!< unit of length(1/2 inch)
	static const char* st_unit_1_6_inch;					//!< unit of length(1/6 inch)

	vector<STRUCT_NON_INSPECTION_AREA> m_NonInspectionList[DEF_PRINT_BOTH];	//!< A list non-inspection area: 0 : Front, 1 : Back
	ST_JOB_VERIFY_MARK m_jobVerifyMark;						//!< Job verify mark information

	bool m_DuplicateJIAreaSettings;							//!< Duplicate JetInspection area settings status
	double m_NonInspectionAreaDef;							//!< The default non-inspection area

	double m_nonInspectionFrontLeft;						//!< emilinated front left
	double m_nonInspectionFrontRight;						//!< emilinated front right
	double m_nonInspectionBackLeft;							//!< emilinated back left
	double m_nonInspectionBackRight;						//!< emilinated back right

	double m_PaperResoX[DEF_PRINT_BOTH];					//!< Paper resolution follow X coordinate
	double m_PaperResoY[DEF_PRINT_BOTH];					//!< Paper resolution follow Y coordinate
	long m_RipHeight;										//!< The RIP height of specified job

	double m_currentPaperWeight;							//!< The current paper weight
	double m_paperWeight;									//!< The select paper weight

	//method
	void InitPaperWidth();
	void InitPaperLength();
	void InitPaperThickness();
	void InitPaperOption();
	void InitPaperPreview();
	void ReleasePaperPreview();
	void InitDynamicNozzleShift();
	void InitNonInspectionAreaList();
	void InitNonInspectionAreaInfo();
	void InitVerifyMark();
	void InitPaperReso();
	void InitRIPHeight();
	/**
		@brief	坪量の初期化
	*/
	void InitPaperWeight();

private:

};


#endif // !defined(_DATA_PAPER_SETTINGS_H_)
