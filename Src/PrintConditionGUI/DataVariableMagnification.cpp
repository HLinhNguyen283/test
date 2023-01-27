/**
 * @file	DataVariableMagnification.cpp
 * @brief	data class: 変倍
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataIF.h"
#include "DataVariableMagnification.h"
#include "Stub_JobManagerIF.h"
#include "Stub_PaperDBIF.h"
#include "Stub_PrinterStateIF.h"
#include "Ini_PrinterDescriptor.h"
#include "DlgVariableMagnification.h"
#include <iomanip>
#include "CommonCBRProc.h"
#include "PluginCBRProc.h"
#include "ResDef.h"

// ==========================================================================
// Local definitions
static const std::string Hyphen = "---";	//!< 変倍値のハイフン文字列
static const long InputMin = 10000;			//!< 変倍値の最小値(0.0001%)
static const long InputMax = 999999;		//!< 変倍値の最大値(0.0001%)

// ==========================================================================

// コンストラクタ
DataVariableMagnification::DataVariableMagnification() : 
	m_SystemSetting(NULL),
	m_DataIF(NULL),
	m_IsEditRipVariableMagnification(false),
	m_EditFrontX(0),
	m_EditFrontY(0),
	m_EditBackX(0),
	m_EditBackY(0),
	m_IsEditDatumPlaneFront(false)
{
}

// デストラクタ
DataVariableMagnification::~DataVariableMagnification()
{
}

// 初期化
bool DataVariableMagnification::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
{
	if((!inSystemSetting) || (!inDataIF))
	{
		return false;
	}

	m_SystemSetting = inSystemSetting;
	m_DataIF = inDataIF;

	return true;
}

// 印刷条件画面表示
bool DataVariableMagnification::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// 印刷条件名 or セクションIDの設定
	m_PrintConditionOrSectionID = inPrintConditionOrSectionID;

	// 変倍パラメータの初期化
	initializeVariableMagnificationValue();
	initializeDlgVariableMagnification();

	return true;
}

// 印刷条件変更後の更新
void DataVariableMagnification::UpdatePrintCondition(const char* inPrintConditionOrSection)
{
	// 保持している情報を全て初期化し直す
	m_PrintConditionOrSectionID = inPrintConditionOrSection;
	initializeVariableMagnificationValue();
	initializeDlgVariableMagnification();
}

// 変倍ボタンのコントロール表示状態更新イベント
void DataVariableMagnification::onView_VariableMagnification(bool& outIsShow, bool& outIsEnabled)
{
	// 表裏変倍機能がONかつ、EQモードの場合のみ表示
	outIsShow = 
		((m_SystemSetting->IsMagnificationAdjustment()) && (FALSE == m_SystemSetting->IsIpdsMode())) ? true : false;

	bool isTempEnable = true;
	if(true == m_DataIF->GetLockPrintCondtion() || true == m_DataIF->IsDisablePrintCondition())
	{
		// 印刷条件のロック設定がON、ユーザーが一般かつユーザー制限の「印刷条件の編集」または「ジョブの印刷条件の編集」がOFFの場合
		isTempEnable = false;
	}
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		bool reservePrintFlg = Stub_JobManagerIF::GetReservePrintMode(m_PrintConditionOrSectionID.c_str(), false);
		ENUM_PRE_RIP_PROGRESS preRIPStatus = DEF_PRE_RIP_PROGRESS_NONE;
		Stub_JobManagerIF::GetPreRIPStatus(m_PrintConditionOrSectionID.c_str(), preRIPStatus, false);
		// 印刷予約ジョブかつ事前RIPが完了していない場合
		if(!(reservePrintFlg && !(DEF_PRE_RIP_PROGRESS_COMP == preRIPStatus)) && isTempEnable)
		{
			isTempEnable = true;
		}
		else
		{
			isTempEnable = false;
		}

		outIsEnabled = isTempEnable;
	}
	// システムの印刷条件設定画面の場合
	else
	{
		outIsEnabled = isTempEnable;
	}
}

// 変倍値表示フィールドのコントロール表示状態更新イベント
void DataVariableMagnification::onView_VariableMagnificationField(bool& outIsShowFront, bool& outIsShowBack)
{
	// 表裏変倍機能がONかつ、EQモードの場合のみ表示(仮)
	bool isTmpShow = 
		((m_SystemSetting->IsMagnificationAdjustment()) && (FALSE == m_SystemSetting->IsIpdsMode())) ? true : false;

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		// 表面はisTmpShowの表示条件をそのまま設定
		outIsShowFront = isTmpShow;
		// 裏面は両面ジョブかどうかも判断する
		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
		if(!Stub_JobManagerIF::GetSidePrint(m_PrintConditionOrSectionID.c_str(), printSide, false))
		{
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::onView_VariableMagnificationField error JM_GetSidePrint");
			outIsShowBack = false;
		}
		outIsShowBack = ((DEF_PRINT_BOTH == printSide) &&  isTmpShow) ? true : false;
	}
	// システムの印刷条件設定画面の場合
	else
	{
		// 表面はisTmpShowの表示条件をそのまま設定
		outIsShowFront = isTmpShow;
		// 裏面はDED環境かどうかも判断する
		outIsShowBack = (m_SystemSetting->IsDED() &&  isTmpShow) ? true : false;
	}
}

// RIP変倍値設定のコントロール表示状態更新イベント
void DataVariableMagnification::onView_RipVariableMagnification(bool& outIsShow)
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		outIsShow = false;
	}
	// システムの印刷条件設定画面の場合
	else
	{
		outIsShow = true;
	}
}

// 変倍入力のコントロール表示状態更新イベント
void DataVariableMagnification::onView_VariableMagnificationInput(bool& outIsShowFront, bool& outIsShowBack, bool& outIsEnabled)
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		outIsShowFront = true;

		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
		if(!Stub_JobManagerIF::GetSidePrint(m_PrintConditionOrSectionID.c_str(), printSide, false))
		{
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::onView_VariableMagnificationInput error JM_GetSidePrint");
			outIsShowBack = false;
		}
		outIsShowBack = (DEF_PRINT_BOTH == printSide) ? true : false;

		outIsEnabled = true;
	}
	// システムの印刷条件設定画面の場合
	else
	{
		outIsShowFront = true;
		outIsShowBack = m_SystemSetting->IsDED() ? true : false;
		outIsEnabled = m_IsEditRipVariableMagnification ? true : false;
	}
}

// 基準面指定のコントロール表示状態更新イベント
void DataVariableMagnification::onView_VariableMagnificationReferencePlane(bool& outIsShow)
{
	// 自動変倍機能が有効の場合のみ表示する
	outIsShow = isAutoVariableMagnification();
}

// 登録ボタンのコントロール表示状態更新イベント
void DataVariableMagnification::onView_VariableMagnificationCommit(bool& outIsEnabled)
{
	outIsEnabled = false;

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		// 自動変倍の基準面が変更されたかどうか
		BOOL isEditDatumPlaneFront = (m_IsEditDatumPlaneFront) ? TRUE : FALSE;
		if(m_JobRipMagnificationInfo.IS_DATUM_PLANE_FRONT != isEditDatumPlaneFront)
		{
			outIsEnabled = true;
			return;
		}
		// 変倍値が変更されたかどうか
		if(m_JobRipMagnificationInfo.FRONT_X != m_EditFrontX ||
		   m_JobRipMagnificationInfo.FRONT_Y != m_EditFrontY ||
		   m_JobRipMagnificationInfo.BACK_X != m_EditBackX ||
		   m_JobRipMagnificationInfo.BACK_Y != m_EditBackY)
		{
			outIsEnabled = true;
		}		
	}
	// システムの印刷条件設定画面の場合
	else
	{
		// RIP変倍値指定が変更されたかどうか
		BOOL isEditRipVariableMagnification = (m_IsEditRipVariableMagnification) ? TRUE : FALSE;
		if(m_PDBRipMagnificationInfo.IS_ENABLE != isEditRipVariableMagnification)
		{
			outIsEnabled = true;
			return;
		}
		// 自動変倍の基準面が変更されたかどうか
		BOOL isEditDatumPlaneFront = (m_IsEditDatumPlaneFront) ? TRUE : FALSE;
		if(m_PDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT != isEditDatumPlaneFront)
		{
			outIsEnabled = true;
			return;
		}
		// 変倍値が変更されたかどうか
		if(m_PDBRipMagnificationInfo.FRONT_X != m_EditFrontX ||
		   m_PDBRipMagnificationInfo.FRONT_Y != m_EditFrontY ||
		   m_PDBRipMagnificationInfo.BACK_X != m_EditBackX ||
		   m_PDBRipMagnificationInfo.BACK_Y != m_EditBackY)
		{
			outIsEnabled = true;
		}
	}
}

// 変倍値表示フィールドのコントロール表示内容更新イベント
void DataVariableMagnification::onDraw_VariableMagnificationField(std::string& outFrontX, std::string& outFrontY, std::string& outBackX, std::string& outBackY)
{
	// 変倍値を文字列化する
	auto convertValue = [](double inValue, std::string& outValue)
	{
		std::stringstream strValue;
		strValue << std::setprecision(3) << std::setiosflags(std::ios::fixed) << inValue;
		outValue = strValue.str();
	};

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		convertValue(static_cast<double>(m_JobRipMagnificationInfo.FRONT_X) / 1000, outFrontX);
		convertValue(static_cast<double>(m_JobRipMagnificationInfo.FRONT_Y) / 1000, outFrontY);
		convertValue(static_cast<double>(m_JobRipMagnificationInfo.BACK_X) / 1000, outBackX);
		convertValue(static_cast<double>(m_JobRipMagnificationInfo.BACK_Y) / 1000, outBackY);	
	}
	// システムの印刷条件設定画面の場合
	else
	{
		if(TRUE == m_PDBRipMagnificationInfo.IS_ENABLE)
		{
			convertValue(static_cast<double>(m_PDBRipMagnificationInfo.FRONT_X) / 1000, outFrontX);
			convertValue(static_cast<double>(m_PDBRipMagnificationInfo.FRONT_Y) / 1000, outFrontY);
			convertValue(static_cast<double>(m_PDBRipMagnificationInfo.BACK_X) / 1000, outBackX);
			convertValue(static_cast<double>(m_PDBRipMagnificationInfo.BACK_Y) / 1000, outBackY);
		}
		else
		{
			outFrontX = Hyphen;
			outFrontY = Hyphen;
			outBackX = Hyphen;
			outBackY = Hyphen;
		}	
	}
}

// RIP変倍値設定のコントロール表示内容更新イベント
void DataVariableMagnification::onDraw_RipVariableMagnification(bool& outIsRipVariableMagnification)
{
	outIsRipVariableMagnification = m_IsEditRipVariableMagnification;
}

// 基準面指定のコントロール表示内容更新イベント
void DataVariableMagnification::onDraw_VariableMagnificationReferencePlane(bool& outIsEditDatumPlaneFront)
{
	outIsEditDatumPlaneFront = m_IsEditDatumPlaneFront;
}

// 変倍入力のコントロール表示内容更新イベント
void DataVariableMagnification::onDraw_VariableMagnificationInput(
	std::string& outFrontX, 
	std::string& outFrontY, 
	std::string& outBackX, 
	std::string& outBackY,
	bool& outIsEditRipVariableMagnification)
{
	// 変倍値を文字列化する
	auto convertValue = [](double inValue, std::string& outValue)
	{
		std::stringstream strValue;
		strValue << std::setprecision(3) << std::setiosflags(std::ios::fixed) << inValue;
		outValue = strValue.str();
	};

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		convertValue(static_cast<double>(m_EditFrontX) / 1000, outFrontX);
		convertValue(static_cast<double>(m_EditFrontY) / 1000, outFrontY);
		convertValue(static_cast<double>(m_EditBackX) / 1000, outBackX);
		convertValue(static_cast<double>(m_EditBackY) / 1000, outBackY);
		outIsEditRipVariableMagnification = true;
	}
	// システムの印刷条件設定画面の場合
	else
	{
		if(m_IsEditRipVariableMagnification)
		{
			convertValue(static_cast<double>(m_EditFrontX) / 1000, outFrontX);
			convertValue(static_cast<double>(m_EditFrontY) / 1000, outFrontY);
			convertValue(static_cast<double>(m_EditBackX) / 1000, outBackX);
			convertValue(static_cast<double>(m_EditBackY) / 1000, outBackY);
		}
		else
		{
			outFrontX = Hyphen;
			outFrontY = Hyphen;
			outBackX = Hyphen;
			outBackY = Hyphen;
		}
		outIsEditRipVariableMagnification = m_IsEditRipVariableMagnification;		
	}
}

// 変倍ボタンのコントロール操作イベント
void DataVariableMagnification::onPush_VariableMagnification(CBaseDataIF* inData, CBasePrinterIF* inPrinter)
{
	// 変倍ダイアログを表示
	initializeDlgVariableMagnification();
	DlgVariableMagnification dlgVariableMagnification;
	dlgVariableMagnification.Create(inData, inPrinter);
}

// RIP変倍値設定のコントロール操作イベント
void DataVariableMagnification::onPush_RipVariableMagnification(bool inIsRipVariableMagnification)
{
	m_IsEditRipVariableMagnification = inIsRipVariableMagnification;
}

// 基準面設定のコントロール操作イベント
void DataVariableMagnification::onPush_VariableMagnificationReferencePlane()
{
	// 基準面設定ボタン(トグル)の切り替え設定をする
	if(m_IsEditDatumPlaneFront)
	{
		m_IsEditDatumPlaneFront = false;
	}
	else
	{
		m_IsEditDatumPlaneFront = true;
	}
}

// 変倍入力のコントロール操作イベント
void DataVariableMagnification::onPush_VariableMagnificationInput(
	const long long& inFrontX, 
	const long long& inFrontY, 
	const long long& inBackX, 
	const long long& inBackY)
{
	// 編集値を更新する
	m_EditFrontX = inputCorrection(inFrontX);
	m_EditFrontY = inputCorrection(inFrontY);
	m_EditBackX = inputCorrection(inBackX);
	m_EditBackY = inputCorrection(inBackY);
}

// 変倍登録コントロール操作イベント
void DataVariableMagnification::onPush_VariableMagnificationCommit()
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		if(m_JobRipMagnificationInfo.IS_DATUM_PLANE_FRONT != (m_IsEditDatumPlaneFront ? TRUE : FALSE))
		{
			// 基準面の変更がある場合のみ、ジョブの印刷条件設定の編集済みフラグを設定する
			Stub_JobManagerIF::SetPrintConditionEditFlag(m_PrintConditionOrSectionID.c_str(), TRUE);
		}

		ST_JOB_RIP_MAGNIFICATION tmpJobRipMagnificationInfo;
		tmpJobRipMagnificationInfo.IS_DATUM_PLANE_FRONT = (m_IsEditDatumPlaneFront) ? TRUE : FALSE;
		tmpJobRipMagnificationInfo.FRONT_X = m_EditFrontX;
		tmpJobRipMagnificationInfo.FRONT_Y = m_EditFrontY;
		tmpJobRipMagnificationInfo.BACK_X = m_EditBackX;
		tmpJobRipMagnificationInfo.BACK_Y = m_EditBackY;

		// ジョブに変更内容を反映する
		if(!Stub_JobManagerIF::UpdateRipMagnification(m_PrintConditionOrSectionID, tmpJobRipMagnificationInfo))
		{
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::onPush_VariableMagnificationCommit error JM_UpdateRipMagnification");
			return;
		}

		// ジョブファイルを保存する
		Stub_JobManagerIF::SaveJobFile(m_PrintConditionOrSectionID.c_str());

		//「変更はこのジョブのみに反映されます」ダイアログ表示
		std::string dlgMsg = LoadResourceString(IDM_COMMIT_CLICK_JOB_LIST_SCREEN, RESOURCE_MSG_STR);
		ShowMessageBox(ID_MESSAGE_ERROR, IDM_COMMIT_CLICK_JOB_LIST_SCREEN, dlgMsg,  MBST_OK | MBST_ICONINFORMATION, NULL);
	}
	// システムの印刷条件設定画面の場合
	else
	{
		ST_PDB_RIP_MAGNIFICATION tmpPDBRipMagnificationInfo;
		tmpPDBRipMagnificationInfo.IS_ENABLE = (m_IsEditRipVariableMagnification) ? TRUE : FALSE;
		if(m_SystemSetting->IsDED())
		{
			tmpPDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT = (m_IsEditDatumPlaneFront) ? TRUE : FALSE;
		}
		else
		{
			// SES構成の場合は裏面が無いため、表面固定とする
			tmpPDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT = TRUE;
		}
		tmpPDBRipMagnificationInfo.FRONT_X = m_EditFrontX;
		tmpPDBRipMagnificationInfo.FRONT_Y = m_EditFrontY;
		tmpPDBRipMagnificationInfo.BACK_X = m_EditBackX;
		tmpPDBRipMagnificationInfo.BACK_Y = m_EditBackY;

		// システムの印刷条件に変更内容を反映する
		if(!Stub_PaperDBIF::SetRipMagnification(m_PrintConditionOrSectionID, tmpPDBRipMagnificationInfo))
		{
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::onPush_VariableMagnificationCommit error PDB_SetRipMagnification");
			return;
		}

		// 紙DBファイルをコミットする
		Stub_PaperDBIF::CommitPaperDB();

		//「変更はジョブテンプレート、新規に作成されるジョブに反映されますが...」ダイアログ表示
		std::string dlgMsg = LoadResourceString(IDM_COMMIT_CLICK_SYSTEM_SETTING_SCREEN, RESOURCE_MSG_STR);
		ShowMessageBox(ID_MESSAGE_ERROR, IDM_COMMIT_CLICK_SYSTEM_SETTING_SCREEN, dlgMsg,  MBST_OK | MBST_ICONINFORMATION, NULL);
	}

	initializeVariableMagnificationValue();
	initializeDlgVariableMagnification();
}

// 変倍終了ボタンのコントロール操作イベント
bool DataVariableMagnification::onPush_VariableMagnificationClose()
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		if((m_JobRipMagnificationInfo.IS_DATUM_PLANE_FRONT != (m_IsEditDatumPlaneFront ? TRUE : FALSE)) ||
			(m_JobRipMagnificationInfo.FRONT_X != m_EditFrontX) ||
			(m_JobRipMagnificationInfo.FRONT_Y != m_EditFrontY) ||
			(m_JobRipMagnificationInfo.BACK_X != m_EditBackX) ||
			(m_JobRipMagnificationInfo.BACK_Y != m_EditBackY))
		{
			return true;
		}			
	}
	// システムの印刷条件設定画面の場合
	else
	{
		if((m_PDBRipMagnificationInfo.IS_ENABLE != (m_IsEditRipVariableMagnification ? TRUE : FALSE)) ||
			(m_PDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT != (m_IsEditDatumPlaneFront ? TRUE : FALSE)) ||
			(m_PDBRipMagnificationInfo.FRONT_X != m_EditFrontX) ||
			(m_PDBRipMagnificationInfo.FRONT_Y != m_EditFrontY) ||
			(m_PDBRipMagnificationInfo.BACK_X != m_EditBackX) ||
			(m_PDBRipMagnificationInfo.BACK_Y != m_EditBackY))
		{
			return true;
		}		
	}

	return false;
}

// 変倍ダイアログを初期化する
void DataVariableMagnification::initializeDlgVariableMagnification()
{
	// 変倍ダイアログ表示時に関連するメンバを初期化する

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		m_EditFrontX = m_JobRipMagnificationInfo.FRONT_X;
		m_EditFrontY = m_JobRipMagnificationInfo.FRONT_Y;
		m_EditBackX = m_JobRipMagnificationInfo.BACK_X;
		m_EditBackY = m_JobRipMagnificationInfo.BACK_Y;
		m_IsEditDatumPlaneFront = (TRUE == m_JobRipMagnificationInfo.IS_DATUM_PLANE_FRONT) ? true : false;	
	}
	// システムの印刷条件設定画面の場合
	else
	{
		m_IsEditRipVariableMagnification = (TRUE == m_PDBRipMagnificationInfo.IS_ENABLE) ? true : false;
		m_EditFrontX = m_PDBRipMagnificationInfo.FRONT_X;
		m_EditFrontY = m_PDBRipMagnificationInfo.FRONT_Y;
		m_EditBackX = m_PDBRipMagnificationInfo.BACK_X;
		m_EditBackY = m_PDBRipMagnificationInfo.BACK_Y;
		m_IsEditDatumPlaneFront = (TRUE == m_PDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT) ? true : false;
	}
}

// 編集中の変倍値を取得する
void DataVariableMagnification::getVariableMagnificationInput(long long& outFrontX, long long& outFrontY, long long& outBackX, long long& outBackY)
{
	outFrontX = static_cast<long long>(m_EditFrontX);
	outFrontY = static_cast<long long>(m_EditFrontY);
	outBackX = static_cast<long long>(m_EditBackX);
	outBackY = static_cast<long long>(m_EditBackY);
}

// 自動変倍機能が有効か取得する
bool DataVariableMagnification::isAutoVariableMagnification()
{
	bool isAutoVariableMagnification = false;
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);

	// JIを搭載しているかどうか
	long jiOption = iniPrinterDescriptor.GetJetInspectionOption();
	// JI連携が有効かどうか
	bool isJI = TRUE == iniPrinterDescriptor.IsEnableJetInspection() ? true : false;
	// NozzleCheckerオプション が有効かどうか
	bool isNozzleChecker = false;
	if((2 == jiOption) || (4 == jiOption))	// JI Full or Middle
	{
		isNozzleChecker = true;
	}
	else if(3 == jiOption)	// JI Lite
	{
		isNozzleChecker = Stub_PrinterStateIF::IsRTMandNCCEnable();
	}
	else
	{
		isNozzleChecker = false;
	}
	// DED構成かどうか
	bool isDed = m_SystemSetting->IsDED();

	if((0 != jiOption) && (true == isJI) && (true == isNozzleChecker) && (true == isDed))
	{
		// ジョブの印刷条件設定画面の場合
		if(m_SystemSetting->IsStartedFromJobListScreen())
		{
			DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
			if(!Stub_JobManagerIF::GetSidePrint(m_PrintConditionOrSectionID.c_str(), printSide, false))
			{
				WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::isAutoVariableMagnification error JM_GetSidePrint");
			}
			// 両面ジョブの場合のみ自動変倍機能と判断する
			isAutoVariableMagnification = (DEF_PRINT_BOTH == printSide) ? true : false;

		}
		// システムの印刷条件設定画面の場合
		else
		{
			// 追加条件無しで自動変倍機能と判断する
			isAutoVariableMagnification = true;		
		}
	}

	return isAutoVariableMagnification;
}

// 変倍パラメータを初期化する
void DataVariableMagnification::initializeVariableMagnificationValue()
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		if(!Stub_JobManagerIF::GetRipMagnification(m_PrintConditionOrSectionID, m_JobRipMagnificationInfo))
		{
			m_JobRipMagnificationInfo.IS_DATUM_PLANE_FRONT = FALSE;
			m_JobRipMagnificationInfo.FRONT_X = 0;
			m_JobRipMagnificationInfo.FRONT_Y = 0;
			m_JobRipMagnificationInfo.BACK_X = 0;
			m_JobRipMagnificationInfo.BACK_Y = 0;
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::initializeVariableMagnificationValue error JM_GetRipMagnification");
		}
		else
		{
			// 変倍入力値を範囲内(10.000%～999.999%)に補正する
			m_JobRipMagnificationInfo.FRONT_X = inputCorrection(static_cast<long long>(m_JobRipMagnificationInfo.FRONT_X));
			m_JobRipMagnificationInfo.FRONT_Y = inputCorrection(static_cast<long long>(m_JobRipMagnificationInfo.FRONT_Y));
			m_JobRipMagnificationInfo.BACK_X = inputCorrection(static_cast<long long>(m_JobRipMagnificationInfo.BACK_X));
			m_JobRipMagnificationInfo.BACK_Y = inputCorrection(static_cast<long long>(m_JobRipMagnificationInfo.BACK_Y));
		}
	}
	// システムの印刷条件設定画面の場合
	else
	{
		if(!Stub_PaperDBIF::GetRipMagnification(m_PrintConditionOrSectionID, m_PDBRipMagnificationInfo))
		{
			m_PDBRipMagnificationInfo.IS_ENABLE = FALSE;
			m_PDBRipMagnificationInfo.IS_DATUM_PLANE_FRONT = m_SystemSetting->IsDED() ? FALSE : TRUE;
			m_PDBRipMagnificationInfo.FRONT_X = 100000;
			m_PDBRipMagnificationInfo.FRONT_Y = 100000;
			m_PDBRipMagnificationInfo.BACK_X = 100000;
			m_PDBRipMagnificationInfo.BACK_Y = 100000;
			WriteToLogBuf(LOG_DEBUG, "DataVariableMagnification::initializeVariableMagnificationValue error PDB_GetRipMagnification");
		}
		else
		{
			// 変倍入力値を範囲内(10.000%～999.999%)に補正する
			m_PDBRipMagnificationInfo.FRONT_X = inputCorrection(static_cast<long long>(m_PDBRipMagnificationInfo.FRONT_X));
			m_PDBRipMagnificationInfo.FRONT_Y = inputCorrection(static_cast<long long>(m_PDBRipMagnificationInfo.FRONT_Y));
			m_PDBRipMagnificationInfo.BACK_X = inputCorrection(static_cast<long long>(m_PDBRipMagnificationInfo.BACK_X));
			m_PDBRipMagnificationInfo.BACK_Y = inputCorrection(static_cast<long long>(m_PDBRipMagnificationInfo.BACK_Y));
		}
	}
}

// 変倍入力値を範囲内(10.000%～999.999%)に補正する
long DataVariableMagnification::inputCorrection(long long inInputValue)
{
	long tmpInputValue = 0;

	if(InputMin > inInputValue)			// 10%未満
	{
		tmpInputValue = InputMin;
	}
	else if(InputMax < inInputValue)	// 1000%以上
	{
		tmpInputValue = InputMax;
	}
	else
	{
		tmpInputValue = static_cast<long>(inInputValue);
	}

	return tmpInputValue;
}