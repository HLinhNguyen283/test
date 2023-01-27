/*! \file DataIF.h
 *  \brief data interface class
 *  \author GCS MinhVu
 *  \date 2013/12/11 GCS MinhVu created
*/
#if !defined(_DATA_IF_H_)
#define _DATA_IF_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "BaseDataIF.h"
#include "CommonDef_OP.h"
#include "CommonCBRProc_OP.h"
#include "PaperDBDef_OP.h"
#include "DataSystemSetting.h"
#include "DataPrintMode.h"
#include "DataVariableMagnification.h"
#include "DataPrimer.h"
#include <vector>
// ==========================================================================

class CDataPrintSettings;
class CDataPrintMode;
class CDataPaperType;
class CDataSystemSetting;
class CDataDotgainCurve;
class CDataICCPresetSetting;
class CDataPrintHeadAlignment;
class CDataPreOverCoat;
class CDataPaperSettings;
class CDataDryerSetting;
class CDataJetInspection;
class CDataPrintConditionModeJobSetting;
class CDataOutputColorSetting;
class DataVariableMagnification;
class DataPrimer;

class CPrintConditionGUI_OP;

/*! \brief data interface class
*/
class CDataIF : public CBaseDataIF
{
public:
	/**
		Enum for listbox event
	*/
	typedef enum{
		DEF_UPDATE_ADD_ITEM,			//!< event: add item
		DEF_UPDATE_EDIT_ITEM,			//!< event: edit item
		DEF_UPDATE_DEL_ITEM,			//!< event: delete item
		DEF_UPDATE_REREGISTER_ALL_ITEM,	//!< event: reregister all items
	}DEF_PRINT_CONDITION_UPDATE;

	//method
	CDataIF(CPrintConditionGUI_OP* inParent, bool inIsStartForMaintenance = false);
	virtual ~CDataIF();
	//
	virtual BOOL Initialize();
	virtual void Finalize();
	virtual BOOL UI_Initialize();
	virtual void UI_Finalize();

	virtual long UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Print Settings
	bool GetPrintConditionList(long inItemID, const char** outPrintCondition);
	bool GetPrintConditionListPaperOption(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide);
	bool GetCurrentPrintCondition(bool* outSelecting, const char** outPrintCondition);
	bool IsPermissionAddPrintCondition();
	bool IsPermissionDeletePrintCondition();
	bool IsDisablePrintCondition();
	bool IsPermissionEditJobPrintCondition();
	void SetSelectPrintCondition(long inItemID);
	bool InitAddPrintConditionDlg();
	bool InitDeletePrintConditionDlg();
	bool GetSelectPrintCondition(long* outItemID);
	void SetCurrentPrintCondition();
	void SetCurrentPrintCondition(bool inSetChangeParam);
	bool GetLockPrintCondtion();
	void SetLockPrintCondtion(bool inLock);
	bool CmpSelectingAndSystemCurrentPrintCondition(bool& outSelecting);

	bool GetPrintConditionList_AddDlg(long inItemID, const char** outName);
	bool GetPrintConditionListPaperOption_AddDlg(long inItemID, bool* outHole, bool* outHoleAndPerforat, bool* outPreprintedMark, bool* outPreprintedPaper, DEF_PRINT_SIDE* outMarkSide);
	const char* GetAddPrintConditionName_AddDlg();
	void SetAddPrintConditionName_AddDlg(char* inName);
	bool GetCurrentPrintCondition_AddDlg(bool* outSelecting, const char** outPrintCondition);
	bool GetSelectPrintCondition_AddDlg(long* outItemID);
	void SetSelectPrintCondition_AddDlg(long inItemID);
	bool SaveAddPrintSettingDlg();
	bool CancelAddPrintSettingDlg();
	
	void SetDeletePrintConditionDlg(HWND inDialog);
	bool GetDeletePrintConditionList_DelDlg(long inItemID, const char** outName);
	void SetDeletePrintCondition_DelDlg(long inItemID);
	bool DeletePrintConditions_DelDlg();
	bool ExitDeletePrintConditionDlg();

	bool IsUpdatedCurrentPrintSetting();
	// ~Print Settings

	// Print Mode
	bool GetPrintModeList(long inItemID, const char** outMode);
	void SetSelectPrintMode(long inItemID);
	bool GetSelectPrintMode(long* outItemID);
	bool GetPrintSpeedList(long inItemID, const char** outSpeed);
	void SetSelectPrintSpeed(long inItemID);
	bool GetSelectPrintSpeed(long* outItemID);
	long GetSelectPrintSpeedValue();
	void GetPaperTensionRange(long* outMin, long* outMax);
	void SetSelectPaperTension(long inItemID);
	bool GetSelectPaperTension(long* outItemID);
	long GetSelectPaperTensionValue();
	// ~Print Mode

	// Paper Settings
	long GetPaperWidth();
	void SetPaperWidth(long inWidth);

	bool GetUsePaperFeedLengthSetting();
	void SetUsePaperFeedLengthSetting(bool inUse);
	long GetPaperFeedLength();
	void SetPaperFeedLength(long inLength);

	long GetPaperThickness();
	void SetPaperThickness(long inThickness);

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

	bool GetDuplicateJIAreaSettings();
	void SetDuplicateJIAreaSettings(bool inState);
	double GetDuplicateInitNonInspectionAreaDefault();

	// ~Paper Settings

	DEF_PRINT_SIDE GetPaperPreviewSide();
	void SetPaperPreviewSide(DEF_PRINT_SIDE inSide);
	bool GetPaperPreview(DEF_PRINT_SIDE inSide, HBITMAP* outImage);

	bool IsJobRunning();
	bool GetPrintHeadAlignmentList(long inItemID, const char** outAlignment);
	void SetSelectPrintHeadAlignment(long inItemID);
	bool InitAddPrintHeadAlignmentDlg();
	bool InitDeletePrintHeadAlignmentDlg();
	bool GetSelectPrintHeadAlignment(long* outItemID);
	bool GetPaperTypeList(long inItemID, const char** outType);
	void SetSelectPaperType(long inItemID);
	bool InitAddPaperTypeDlg();
	bool InitDeletePaperTypeDlg();
	bool GetSelectPaperType(long *outItemID);
	void GetPrecoatRange(long* outMin, long* outMax);
	void SetSelectPrecoat(long inPrinterID, long inItemID);
	bool IsExistPrecoater();
	bool IsDED();
	bool GetSelectPrecoat(long inPrinterID, long* outItemID);
	void GetOvercoatRange(long* outMin, long* outMax);
	void SetSelectOvercoat(long inPrinterID, long inItemID);
	bool IsExistOvercoater(long inPrinterID);
	bool GetSelectOvercoat(long inPrinterID, long* outItemID);
	bool GetPrintDryerSettingList(long inItemID, const char** outDryer);
	bool InitAddDryerSettingDlg();
	bool InitDeleteDryerSettingDlg();
	void SetSelectPrintDryerSetting(long inItemID);
	bool GetSelectPrintDryerSetting(long* outItemID);
	const char* GetSystemUnitLength(DEF_LENGTH_UNIT* outUnit);
	bool InitSelectPreprintedMarkSideDlg();
	bool InitToneSettingDlg();
	void OnCopyJobPrintCondition(bool& outIsShowCopyMessage);
	bool SavePrintCondition();
	bool IsAbleSavePrintSetting();
	DEF_USER_MODE_ID GetSystemUserMode();
	bool IsSavedPrintSetting();
	bool IsPaperSizeChanged();
	bool IsJIOptionEnable();
	bool EnterScreen();
	bool ExitScreen();

	void SetAddPrintHeadAlignmentName_AddDlg(char* inAlignment);
	const char* GetAddPrintHeadAlignmentName_AddDlg();
	bool GetPrintHeadAlignmentList_AddDlg(long inItemID, const char** outAlignment);
	void SetSelectPrintHeadAlignment_AddDlg(long inItemID);
	bool GetSelectPrintHeadAlignment_AddDlg(long* outItemID);
	bool SaveAddPrintHeadAlignmentDlg();
	bool CancelAddPrintHeadAlignmentDlg();
	void SetDeletePrintHeadAlignmentDlg(HWND inDialog);
	bool GetDeletePrintHeadAlignmentList_DelDlg(long inItemID, const char** outAlignment);
	bool SetDeletePrintHeadAlignment_DelDlg(long inItemID);
	bool DeletePrintHeadAlignments_DelDlg();
	bool ExitDeletePrintHeadAlignmentDlg();
	void SetAddPaperTypeName_AddDlg(char* inType);
	const char* GetAddPaperTypeName_AddDlg();
	bool GetPaperTypeList_AddDlg(long inItemID, const char** outType);
	void SetSelectPaperType_AddDlg(long inItemID);
	bool GetSelectPaperType_AddDlg(long* outItemID);
	bool SaveAddPaperTypeDlg();
	bool CancelAddPaperTypeDlg();
	void SetDeletePaperTypeDlg(HWND inDialog);
	bool GetDeletePaperTypeList_DelDlg(long inItemID, const char** outType);
	bool SetDeletePaperType_DelDlg(long inItemID);
	bool DeletePaperTypes_DelDlg();
	bool ExitDeletePaperTypeDlg();
	void SetHeatRollerTemperature_AddDlg(long inPrinterID, char* inTemperature);
	void SetDryerFunTemperature_AddDlg(long inPrinterID, char* inTemperature);
	bool IsExistDryerUnit(long inPrinterID);	//YTRAN: CDataIF_SystemSetting
	bool IsExistHeatRoller(long inPrinterID);	//YTRAN: CDataIF_SystemSetting
	const char* GetHeatRollerTemperature_AddDlg(long inPrinterID);
	const char* GetDryerFunTemperature_AddDlg(long inPrinterID);
	void SetPrecoatTemperature_AddDlg(long inPrinterID, char* inTemperature);
	const char* GetPrecoatTemperature_AddDlg(long inPrinterID);
	bool IsSavedDryerSetting_AddDlg();
	const char* GetNirPower_AddDlg(long inPrinterID);
	void SetNirPower_AddDlg(long inPrinterID, char* inNirPower);

	bool SaveAddDryerSettingDlg();
	bool CancelAddDryerSettingDlg();
	void SetDeleteDryerSettingDlg(HWND inDialog);
	bool GetDeleteDryerSettingList_DelDlg(long inItemID, const char** outDryer);
	void SetDeleteDryerSetting_DelDlg(long inItemID);
	bool DeleteDryerSettings_DelDlg();
	bool ExitDeleteDryerSettingDlg();
	void SetSelectPreprintedMarkSide_SelectDlg(DEF_PRINT_SIDE inSide);
	bool ExitSelectPreprintedMarkSideDlg();
	bool IsMaster();

	// new method for princondition
	bool IsStartFromSystemSetting();
	bool GetJobName(char ** outJobName);
	void GetCurrentPrintModeName(char ** outModeName);

	//Dotgain Curve
	bool GetDotgainCurve_ForDrawing(ST_PDB_DOTGAIN_CURVE& outDotgainCurve);
	void GetLinearDotGainData_ForDrawing(ST_PDB_ANCHOR_POINT* outAnchor);

	bool IsSystemSupportJetInspection();
	bool IsHaveJetInspection();
	bool GetJetInspectionList(long inItemID, const char** outJI);
	bool IsJIExistInJIList();
	void SetSelectJetInspection(long inItemID);
	bool GetSelectJetInspection(long* outItemID);
	void SetImageStorage(const bool inIsEnable);
	bool IsImageStorage();

	bool IsDotgainCurveDefault_ForDrawing();
	long GetToneFrameWidth_ForDrawing();
	COLORREF GetToneFrameColor_ForDrawing();
	UINT GetDisplayColorSize(DEF_PRINT_SIDE inSide) const;
	bool GetDisplayColor(DEF_PRINT_SIDE inSide, const char* inColorName, COLORREF& outColor);
	//ICC Preset
	bool GetICCPresetList(long inItemId, const char** outICCPreset);
	void SetSelectICCPreset(long inItemId);
	bool GetSelectICCPreset(long* outItemId);
	bool AddSystemICCPreset();
	void SetAddICCPresetNameDlg(char* inICCPreset);
	const char* GetAddICCPresetNameDlg();
	void CancelAddICCPresetNameDlg();
	bool IsAddICCPresetNameExist();
	bool IsExistName();
	void RemoveICCPresetNameNotExist();
	bool IsICCProfileNotAssignedToICCPreset();
	void CheckICCPresetUse();

	PHOST GetHost();

	bool IsPrintConditionEdited();

	bool IsBlankNotAllowed();
	bool IsSimplexJob();

	//Non inspection area
	BOOL GetNonInspectionRect(long inItemID, DEF_PRINT_SIDE inSide, RECT& outRect);
	void GetEliminatedInfo(DEF_PRINT_SIDE inSide, bool inIsLeft, double& outLeft, double& outRight);
	BOOL GetMaxMinPaperSize(long& outMaxWidth, long& outMinWidth, long& outMaxHeight, long& outMinHeight);
	BOOL GetVerifyMarkOffset(DEF_PRINT_SIDE inSide, double& outOffsetX, double& outOffsetY);
	BOOL GetEnablePrintStartMark();
	BOOL GetVerifyMarkTarget();
	eUNIT_MODE GetPaperLengthUnit();
	BOOL GetPaperReso(DEF_PRINT_SIDE inSide, double& outResoX, double& outResoY);
	void GetRipSizeH(double& outRipHeight);

	bool StartJobSetting();
	bool GetPrintConditionNamesOfRunningJob(std::vector<std::string>& outPrintConditions);

	void SetPostMessageWindow(HWND inWnd);
	HWND GetPostMessageWindow();

	bool IsSupport150mpm();
	bool IsSupport120DegC(bool inIsSelf);
	bool IsSupport120DegC_V3(bool inIsSelf);
	bool IsSupport140DegC(bool inIsSelf);
	bool IsSupport100mpm();
	bool IsSupportPrintModeForCoatedOffset();

	BOOL IsIpdsMode();
		
	void SetCurrentConditionBtnOnOffOpen(bool inOnOff);
	bool GetCurrentConditionBtnOnOffOpen();
	void SetCurrentConditionBtnOnOff(bool inOnOff);
	bool GetCurrentConditionBtnOnOff();

	void SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);
	bool GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve);

	bool IsExistNirDryerUnit(long inPrinterID);
	bool IsExistMultiColor();
	void SetSelectOutputColor(long inItemId);
	bool GetSelectOutputColor(long* outItemId);

	bool IsImageStorageEnable();

	bool IsJIDuplicatingEnable();

	/**
		@brief	表面の事前印刷マークの副走査オフセットを取得する
		@retval	表面の事前印刷マークの副走査オフセット(mm)
	*/
	long GetPreMarkOffsetFrontY();

	/**
		@brief	ジョブ管理にQueueStatusの発行を依頼する
	*/
	void SendQueueStatusCommand();

	/**
		@brief	削除する印刷条件名を使用しているジョブがいないかのチェックする
		@param[in]	inPrintConditionName : 印刷条件名
		@param[out]	outUsingJob : 使っているジョブ名(1件分)
		@retval	true : 使用中
		@retval	false : 未使用
	*/
	bool IsPrintConditionUsedForJobs(const std::string& inPrintConditionName, std::string& outUsingJob);

	/*!
	 *  \brief レイアウトイメージ情報を取得する./ Get the layout image information from the print condition
	 *  \param inPrintConditionName [IN]印刷条件名 / Print condition
	 *  \param inUnit [IN]単位 / Unit （UNIT_MODE_MM or UNIT_MODE_INCH or UNIT_MODE_POINT）
	 *  \param outLayoutImagePosition		[OUT] 画像データ情報 / Image position information
	 *  \param outLayoutFlushingPattern		[OUT] フラッシングパターン情報 / Flushing pattern information
	 *  \param outLayoutFlushingPatternAdd	[OUT] フラッシングパターンLineオプション情報 / Flushing pattern line option information
	 *  \param outLayoutFlushingPatternAdd2	[OUT] Flushing pattern 2nd line information
	 *  \param outLayoutVerifyMark			[OUT] 検査マーク情報 / Verify mark information
	 *  \param outLayoutPostProcess			[OUT] 後処理マーク情報 / Post process information
	 *  \param outEnableAlignBottomPPFLSPTN	[OUT] The status to enable bottom alignment of PostProcess and Flushing Pattern
	 *  \return true：成功 / Success、false：失敗 / Failure.
	 */
	bool GetFlushingPattern(const std::string& inPrintConditionName, ST_JOB_FLUSHING_PATTERN& outFlushingPattern, ST_JOB_FLUSHING_PATTERN_ADD& outFlushingPatternAdd, BOOL& outVerticalAlignment_PP_FLSPTN);

	/**
	 *	Getting the select print mode
	 *	@retval	the select print mode
	**/
	void GetCurrentPrintMode(PrintMode& outPrintMode);

	/**
		モノクロ印刷設定を取得する(表)
		@retval	true : モノクロ印刷
		@retval	false : 多色印刷
	*/
	bool IsMonochromePrint_F();

	/**
	 *	Getting the select nir power
	 *	@retval	the select nir power
	**/
	long GetSelectNirPower(bool isSelf);

	/**
	* @brief インポート時ベースの印刷条件名を削除確認後、キャンセルしたかのチェック。
	* @param[in]	inPrintConditionName : 印刷条件名
	* @retval	true : キャンセルしたので処理中断。
	* @retval	false : 処理続行。
	**/
	bool IsCancelDeleteBasePrintCondition(const std::string& inPrintConditionName);

	/**
		@brief	Renger搭載有無を取得
		@retval	true : 搭載
		@retval	false : 非搭載
	*/
	bool IsUnitOfRanger();

	void SetDryerTemp_SubHeatRoller(long inPrinterID, char* inTemperature);

	const char*  GetDryerTemp_SubHeatRoller(long inPrinterID);

	bool IsExistDNS();

	/**
		@brief	変倍ボタンのコントロール表示状態更新イベント
		@param[out]	outIsShow : 変倍ボタンの表示状態
		@param[out]	outIsEnabled : 変倍ボタンの有効状態
	*/
	void onView_VariableMagnification(bool& outIsShow, bool& outIsEnabled);

	/**
		@brief	変倍値表示フィールドのコントロール表示状態更新イベント
		@param[out]	outIsShowFront : 表面の変倍値フィールドの表示状態
		@param[out]	outIsShowBack :  裏面の変倍値フィールドの表示状態
	*/
	void onView_VariableMagnificationField(bool& outIsShowFront, bool& outIsShowBack);

	/**
		@brief	RIP変倍値設定のコントロール表示状態更新イベント
		@param[out]	outIsEnabled : 変倍入力の有効状態
	*/
	void onView_RipVariableMagnification(bool& outIsShow);

	/**
		@brief	変倍入力のコントロール表示状態更新イベント
		@param[out]	outIsShowFront : 表面の変倍入力の表示状態
		@param[out]	outIsShowBack  : 裏面の変倍入力の表示状態
		@param[out]	outIsEnabled   : 変倍入力の有効状態
	*/
	void onView_VariableMagnificationInput(bool& outIsShowFront, bool& outIsShowBack, bool& outIsEnabled);

	/**
		@brief	基準面指定のコントロール表示状態更新イベント
		@param[out]	outIsEnabled : 基準面指定の表示状態
	*/
	void onView_VariableMagnificationReferencePlane(bool& outIsShow);

	/**
		@brief	登録ボタンのコントロール表示状態更新イベント
		@param[out]	outIsEnabled : 変倍入力の有効状態
	*/
	void onView_VariableMagnificationCommit(bool& outIsEnabled);

	/**
		@brief	変倍値表示フィールドのコントロール表示内容更新イベント
	*/
	void onDraw_VariableMagnificationField(std::string& outFrontX, std::string& outFrontY, std::string& outBackX, std::string& outBackY);

	/**
		@brief	RIP変倍値設定のコントロール表示内容更新イベント
		@param[out]	outIsRipVariableMagnification : RIP変倍値設定状態(ture:ON、false:OFF)
	*/
	void onDraw_RipVariableMagnification(bool& outIsRipVariableMagnification);

	/**
		@brief	基準面指定のコントロール表示内容更新イベント
		@param[out]	outIsEditDatumPlaneFront : 編集中の基準面が表面かどうか(true:表面、false:裏面)
	*/
	void onDraw_VariableMagnificationReferencePlane(bool& outIsEditDatumPlaneFront);

	/**
		@brief	変倍入力のコントロール表示内容更新イベント
		@param[out]	outFrontX : 編集中の横の変倍値(表面)
		@param[out]	outFrontY : 編集中の縦の変倍値(表面)
		@param[out]	outBackX : 編集中の横の変倍値(裏面)
		@param[out]	outBackY : 編集中の縦の変倍値(裏面)
		@param[out]	outIsEditRipVariableMagnification : 編集中のRIP変倍値指定(true:ON、false:OFF)
	*/
	void onDraw_VariableMagnificationInput(
		std::string& outFrontX, 
		std::string& outFrontY, 
		std::string& outBackX, 
		std::string& outBackY, 
		bool& outIsEditRipVariableMagnification);

	/**
		@brief	変倍ボタンのコントロール操作イベント
		@param[in]	inData : CBaseDataIFクラスインスタンス
		@param[in]	inPrinter : CBasePrinterIFクラスインスタンス
	*/
	void onPush_VariableMagnification(CBaseDataIF* inData, CBasePrinterIF* inPrinter);

	/**
		@brief	RIP変倍値設定のコントロール操作イベント
	*/
	void onPush_RipVariableMagnification(bool inIsRipVariableMagnification);

	/**
		@brief	基準面設定のコントロール操作イベント
	*/
	void onPush_VariableMagnificationReferencePlane();

	/**
		@brief	変倍入力のコントロール操作イベント
		@param[out]	inFrontX : 編集中の横の変倍値(表面)
		@param[out]	inFrontY : 編集中の縦の変倍値(表面)
		@param[out]	inBackX : 編集中の横の変倍値(裏面)
		@param[out]	inBackY : 編集中の縦の変倍値(裏面)
	*/
	void onPush_VariableMagnificationInput(
		const long long & inFrontX, 
		const long long& inFrontY, 
		const long long& inBackX, 
		const long long& inBackY);

	/**
		@brief	変倍登録ボタンのコントロール操作イベント
	*/
	void onPush_VariableMagnificationCommit();

	/**
		@brief	変倍終了ボタンのコントロール操作イベント
		@retval	true : 未保存の設定有り
		@retval	false : 未保存の設定無し
	*/
	bool onPush_VariableMagnificationClose();

	/**
		@brief	変倍ダイアログを初期化する
	*/
	void initializeDlgVariableMagnification();

	/**
		@brief	編集中の変倍値を取得する
		@param[out]	outFrontX : 編集中の横の変倍値(表面)
		@param[out]	outFrontY : 編集中の縦の変倍値(表面)
		@param[out]	outBackX : 編集中の横の変倍値(裏面)
		@param[out]	outBackY : 編集中の縦の変倍値(裏面)
	*/
	void getVariableMagnificationInput(long long& outFrontX, long long& outFrontY, long long& outBackX, long long& outBackY);

	/**
		@brief	プライマーのON/OFFボタンのコントロール表示状態更新イベント
		@param[out]	outIsShow : プライマーのON/OFFボタンの表示状態
		@param[out]	outIsEnabled : プライマーのON/OFFボタンの有効状態
	*/
	void onView_Primer(bool& outIsShow, bool& outIsEnabled);

	/**
		@brief	プライマーのON/OFFボタンのボタン押下イベント
		@param[in]	inState : プライマーのON/OFFボタンの状態
	*/
	void onPush_Primer(bool inState);

	/**
		@brief	プライマーのON/OFFボタンのコントロール表示内容更新イベント
		@param[in]	outState : プライマーのON/OFFボタンの状態
	*/
	void onDraw_Primer(bool& outState);

	/**
		@brief	全画像保管ボタンのコントロール表示状態更新イベント
		@param[out]	outIsShow : 全画像保管ボタンの表示状態
		@param[out]	outIsEnabled : 全画像保管ボタンの有効状態
	*/
	void onView_ImageStorage(bool& outIsShow, bool& outIsEnabled);

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

	/**
	 *	Get CDataPaperSettings's object
	 *	retval: CDataPaperSettings's object
	**/
	CDataPaperSettings * GetDataPaperSettings(void);

protected:
	//member
	CDataSystemSetting* m_systemSetting;
	CDataPrintSettings* m_PrintSettings;
	CDataPrintMode* m_PrintMode;
	CDataPaperType* m_paperType;
	CDataDotgainCurve* m_dotgainCurve;
	CDataICCPresetSetting* m_iccPresetSetting;
	CDataPrintHeadAlignment* m_printHeadAlignment;
	CDataPreOverCoat* m_preOverCoat;
	CDataPaperSettings* m_PaperSettings;
	CDataDryerSetting* m_dryerSetting;
	CDataJetInspection* m_jetInspection;
	CDataPrintConditionModeJobSetting* m_PrintConditionModeJobSetting;
	CDataOutputColorSetting* m_OutputColorSetting;
	std::unique_ptr<DataVariableMagnification> m_DataVariableMagnification;		//!< 変倍データクラス
	std::unique_ptr<DataPrimer> m_DataPrimer;		//!< プライマーデータクラス

	bool m_IsUpdated;					//!< Flag for checking the setting of current job is updated or not
	bool m_IsSimplexJob;				//!< Check the current job is simplex job or not 
	char m_heatRollerTemp[2][32];		//!< Temperature of hear roller (Add dryer setting dialog)
	char m_dryerFanTemp[2][32];			//!< Temperature of dryer fan (Add dryer setting dialog)
	char m_precoaterTemp[2][32];		//!< Temperature of precoater (Add dryer setting dialog)
	char m_nirPower[2][32];				//!< Value of nir power (Add dryer setting dialog)
	char m_subHeatRollerTemp[2][32];	//!< Temperature of SubHeatRoller
	CPrintConditionGUI_OP*	m_Parent;	//!< The parent object

	bool m_IsPushCurrentConditionBtnOpen;	//!< 画面Open時のCurrentConditionボタンのOn/Off状態
	bool m_IsPushCurrentConditionBtn;		//!< 現在のCurrentConditionボタンのOn/Off状態

	//method
	void UpdateCurrent();

private:

};

#endif // !defined(_DATA_IF_H_)
