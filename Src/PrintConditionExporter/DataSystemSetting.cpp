/**
 * @file	DataSystemSetting.cpp
 * @brief	システムのデータ
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataSystemSetting.h"
#include "Ini_PrinterDescriptor.h"
#include <memory>
#include <algorithm>
#include "Ini_PrintConditionExporter.h"
#include "Ini_PrinterParameter.h"
#include "Ini_FlushingPageMagnification.h"
#include "Stub_PrinterStateIF.h"
#include "Stub_JobManagerIF.h"
#include <vector>


// ==========================================================================

std::shared_ptr<CDataSystemSetting> st_this;


//シングルトンオブジェクト作成
bool CDataSystemSetting::CreateInstance()
{
	st_this = std::shared_ptr<CDataSystemSetting>(new CDataSystemSetting());
	st_this->Initialize();

	return true;
}

//シングルトンオブジェクト破棄
void CDataSystemSetting::DeleteInstance()
{
	st_this.reset();
}

//シングルトンオブジェクト取得
std::shared_ptr<CDataSystemSetting> CDataSystemSetting::GetInstance()
{
	return st_this;
}

//コンストラクタ
CDataSystemSetting::CDataSystemSetting()
{
	m_IsDED = false;
	m_IsDebugLog = false;
	m_IsDebugXmlExportLog = false;
	m_AutoUpdatePrintConditionRetryInterval = 0;
	m_AutoUpdatePrintConditionRetryIntervalXML = 0;
	m_EnableAutoUpdatePrintConditionText = false;
	m_EnableAutoUpdatePrintConditionXML = false;
	m_JIOption = 0;
	m_IsDualSimplex = false;
	m_isMagnifigationAdustment = false;
}

//デストラクタ
CDataSystemSetting::~CDataSystemSetting()
{
}

//初期化イベント
void CDataSystemSetting::Initialize()
{
	CIni_PrinterDescriptor ini_PrinterDescriptor;
	ini_PrinterDescriptor.Initialize(TRUE);
	CIni_PrintConditionExporter iniPrintConditionExporter;
	iniPrintConditionExporter.Initialize(TRUE);
	CIni_PrinterParameter iniPrinterParameter;
	iniPrinterParameter.Initialize(TRUE);

	m_IsDED = (ini_PrinterDescriptor.IsDedSystem() == TRUE) ? true : false;
	m_IsDebugLog = iniPrintConditionExporter.IsDebugLog();
	iniPrintConditionExporter.GetOutputRootFolder(m_OutputRootFolder);
	iniPrintConditionExporter.GetOutputRootFolderXML(m_OutputRootFolderXML);
	m_AutoUpdatePrintConditionRetryInterval = iniPrintConditionExporter.GetAutoUpdatePrintConditionRetryInterval();
	m_AutoUpdatePrintConditionRetryIntervalXML = iniPrintConditionExporter.GetAutoUpdatePrintConditionRetryIntervalXML();
	m_EnableAutoUpdatePrintConditionText = iniPrintConditionExporter.IsEnableAutoUpdatePrintConditionText();
	m_EnableAutoUpdatePrintConditionXML = iniPrintConditionExporter.IsEnableAutoUpdatePrintConditionXML();
	m_IsDebugXmlExportLog = iniPrintConditionExporter.IsDebugXmlExportLog();

	m_EnableAutoUpdateFixIP = iniPrintConditionExporter.IsEnableAutoUpdateFixIP();
	iniPrintConditionExporter.GetOutputRootFolderInspectionData(m_OutputRootFolderJIData);

	for(long printModeID = 0; ; ++printModeID)
	{
		std::string printModeName;
		bool result = iniPrinterParameter.getPrintModeName(printModeID, printModeName);
		if(false == result)
		{
			break;
		}
		std::string printMode = iniPrinterParameter.getEQName(printModeID);
		m_PrintModeDataList.insert(std::make_pair(printModeName, printMode));

		char printModeMemuName[64];
		if( iniPrinterParameter.GetPrintModeMenuName(printModeID, printModeMemuName, 64) ) {
			m_PrintModeMenuList[printModeName] = printModeMemuName;
		}
	}

	m_PCNameEQ = ini_PrinterDescriptor.GetEqCenterPcName();
	m_IpAddressSelf = ini_PrinterDescriptor.GetClientPcIPAddr(true);
	m_IpAddressPart = ini_PrinterDescriptor.GetClientPcIPAddr(false);
	m_JIOption = ini_PrinterDescriptor.GetJetInspectionOption();
	m_IsDualSimplex = ini_PrinterDescriptor.IsEnableExchangePrinterCompositionOption();

	Ini_FlushingPageMagnification iniFlushingPageMagnification;
	iniFlushingPageMagnification.Initialize(TRUE);
	m_isMagnifigationAdustment = iniFlushingPageMagnification.IsMagnificationAdjustment();

	std::vector<flushing_page_list_ini::SItem> flushingPageList;
	bool isExist = Stub_JobManagerIF::GetDetailedFlushingPageList(flushingPageList);
	auto itrEnd = flushingPageList.end();

	m_NozzleCheckEnabledFlushingPage.clear();
	for (auto flushingPageListItr = flushingPageList.begin(); flushingPageListItr != itrEnd; ++flushingPageListItr)
	{
		if (flushingPageListItr->m_IsNozzleCheck)
		{
			m_NozzleCheckEnabledFlushingPage.push_back(flushingPageListItr->m_PageName);
		}
	}
}

//DED構成判定
bool CDataSystemSetting::IsDED() const
{
	return m_IsDED;
}

//Log.htmlに出力するかどうか
bool CDataSystemSetting::IsDebugLog() const
{
	return m_IsDebugLog;
}

// Log.htmlにXMLエクスポート処理のログを出力するかどうか
bool CDataSystemSetting::IsDebugXmlExportLog() const
{
	return m_IsDebugXmlExportLog;
}

// EQ制御サーバー上にあるテキスト出力先のルートフォルダ
void CDataSystemSetting::GetOutputRootFolder(std::string& outPath) const
{
	outPath = m_OutputRootFolder;
}

// EQ制御サーバー上にあるXML出力先のルートフォルダ
void CDataSystemSetting::GetOutputRootFolderXML(std::string& outPath) const
{
	outPath = m_OutputRootFolderXML;
}

// EQ制御サーバー上にあるXML出力先のルートフォルダ
void CDataSystemSetting::GetOutputRootFolderInspectionData(std::string& outPath) const
{
	outPath = m_OutputRootFolderJIData;
}

// EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔
long CDataSystemSetting::GetAutoUpdatePrintConditionRetryInterval() const
{
	return m_AutoUpdatePrintConditionRetryInterval;
}

// EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔
long CDataSystemSetting::GetAutoUpdatePrintConditionRetryIntervalXML() const
{
	return m_AutoUpdatePrintConditionRetryIntervalXML;
}

// EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能の有効状態
bool CDataSystemSetting::IsEnableAutoUpdatePrintConditionText() const
{
	return m_EnableAutoUpdatePrintConditionText;
}

// EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能の有効状態
bool CDataSystemSetting::IsEnableAutoUpdatePrintConditionXML() const
{
	return m_EnableAutoUpdatePrintConditionXML;
}

// （デバッグ用）XML/TEXT出力先の<IPAddress>-<構成名>フォルダを192.168.0.30固定で処理するか
bool CDataSystemSetting::IsEnableAutoUpdateFixIP() const
{
	return m_EnableAutoUpdateFixIP;
}

// プリンターパラメータの情報
void CDataSystemSetting::onExportXML(std::map<std::string, std::string>& outPrintModeDataList) const
{ 
	outPrintModeDataList = m_PrintModeDataList;
}

// EQUIOS Center PCのPC名
void CDataSystemSetting::GetEqCenterPcName(std::string& outPCNameEQ) const
{
	outPCNameEQ = m_PCNameEQ;
}

// 自機のIPアドレス
void CDataSystemSetting::GetClientPcIPAddrSelf(std::string& outIpAddressSelf) const
{
	outIpAddressSelf = m_IpAddressSelf;
}

// 相手機のIPアドレス
void CDataSystemSetting::GetClientPcIPAddrPart(std::string& outIpAddressPart) const
{
	outIpAddressPart = m_IpAddressPart;
}

// JetInspectorとの接続設定
long CDataSystemSetting::GetJetInspectionOption() const
{
	return m_JIOption;
}

// DualSimplex環境設定の有効状態
bool CDataSystemSetting::IsDualSimplex() const
{
	return m_IsDualSimplex;
}

// 印刷モードメニュー名の取得
bool CDataSystemSetting::GetPrintModeMenuName(const char* inModeName, std::string& outModeMenuName)const
{
	auto target = m_PrintModeMenuList.find(inModeName);
	if( target == m_PrintModeMenuList.end() )
	{
		return false;
	}
	outModeMenuName = target->second;
	return true;
}

// 表裏倍率調整用のフラッシングページを出力するかどうか
bool CDataSystemSetting::IsMagnificationAdjustment() const
{
	return m_isMagnifigationAdustment;
}

// ノズルチェック機能が可能かどうか
bool CDataSystemSetting::isNozzleCheckEnabled(const std::string& inFlushingPageName) const
{
	if(inFlushingPageName.empty())
	{
		return false;
	}
	auto it = std::find(
		m_NozzleCheckEnabledFlushingPage.begin(),
		m_NozzleCheckEnabledFlushingPage.end(),
		inFlushingPageName);

	if((it != m_NozzleCheckEnabledFlushingPage.end()) && Stub_PrinterStateIF::IsRTMandNCCEnable())
	{
		// ノズルチェック機能が可能なフラッシングページ 且つ 印刷モニター＆ノズル抜けチェック（RTM&NCC）が有効な場合
		return true;
	}
	return false;
}
