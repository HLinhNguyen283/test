/**
 * @file	DataPrimer.cpp
 * @brief	data class: プライマー
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataPrimer.h"
#include "Stub_JobManagerIF.h"
#include "Stub_PaperDBIF.h"

// ==========================================================================
// Local definitions

// ==========================================================================

// コンストラクタ
DataPrimer::DataPrimer() : 
	m_SystemSetting(NULL),
	m_DataIF(NULL),
	m_IsPrimer(false),
	m_IsPrePrimer(false)
{
}

// デストラクタ
DataPrimer::~DataPrimer()
{
}

// 初期化
bool DataPrimer::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
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
bool DataPrimer::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// プライマーのON/OFFボタンの初期化
	initializePrimer(inPrintConditionOrSectionID);

	return true;
}

// ジョブ印刷条件設定コピーイベント
void DataPrimer::onCopyJobPrintCondition(bool& outIsPrimer)
{
	outIsPrimer = m_IsPrimer;
}

// プライマーのON/OFFボタンか変更されたかどうか
bool DataPrimer::IsChanged()
{
	if(m_IsPrePrimer != m_IsPrimer)
	{
		return true;
	}
	
	return false;
}

// 印刷条件の保存
bool DataPrimer::Save()
{
	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		Stub_JobManagerIF::SetPrimerStatus(m_PrintConditionOrSectionID, m_IsPrimer);
	}
	// システムの印刷条件設定画面の場合
	else
	{
		Stub_PaperDBIF::SetPrimerStatus(m_PrintConditionOrSectionID, m_IsPrimer);
	}

	// プライマーのON/OFFボタンの初期化
	initializePrimer(m_PrintConditionOrSectionID.c_str());

	return true;
}

// プライマーのON/OFFボタンのコントロール表示状態更新イベント
void DataPrimer::onView_Primer(bool& outIsShow, bool& outIsEnabled)
{
	m_SystemSetting->IsConnectedPrimer() ? outIsShow = true : outIsShow = false;

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

// 印刷条件変更後の更新
void DataPrimer::UpdatePrintCondition(const char* inPrintConditionOrSection)
{
	// プライマーのON/OFFボタンの初期化
	initializePrimer(inPrintConditionOrSection);
}

// プライマーのON/OFFボタンのボタン押下イベント
void DataPrimer::onPush_Primer(bool inState)
{
	m_IsPrimer = inState;
}

// プライマーのON/OFFボタンのコントロール表示内容更新イベント
void DataPrimer::onDraw_Primer(bool& outState)
{
	outState = m_IsPrimer;
}

// プライマーのON/OFFボタンの初期化
void DataPrimer::initializePrimer(const char* inPrintConditionOrSectionID)
{
	m_PrintConditionOrSectionID = inPrintConditionOrSectionID;
	bool isTempPrimer = false;

	// ジョブの印刷条件設定画面の場合
	if(m_SystemSetting->IsStartedFromJobListScreen())
	{
		if(!Stub_JobManagerIF::GetPrimerStatus(m_PrintConditionOrSectionID, isTempPrimer))
		{
			isTempPrimer = false;
		}
	}
	// システムの印刷条件設定画面の場合
	else
	{
		if(!Stub_PaperDBIF::GetPrimerStatus(m_PrintConditionOrSectionID, isTempPrimer))
		{
			isTempPrimer = false;
		}
	}

	m_IsPrimer = isTempPrimer;
	m_IsPrePrimer = m_IsPrimer;
}