/**
 * @file	DataSystemSetting.cpp
 * @brief	システムのデータ
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataSystemSetting.h"
#include "PaperDBDef_OP.h"
#include "Ini_PrintConditionImporter.h"
#include "Ini_PrinterParameter.h"
#include "Ini_PrinterDescriptor.h"
#include "Ini_MainteJetInspection.h"
#include "Ini_PrintConditionGUI.h"
#include "Ini_ToneSettingGUI.h"
#include "CriticalSectionLocker.h"
#include "Stub_PrinterStateIF.h"
#include "Stub_JobManagerIF.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include "Ini_StringsPrinterParameter.h"
#include "Ini_FlushingPageMagnification.h"
// ==========================================================================

//　内部クラス
/**
 * @brief	印刷モードに紐づく情報
**/
class CDataSystemSetting::CPrintModeData {
public:
	/**
	 * @brief	コンストラクタ
	 * @param[in]	inPrintMode : 紙DB用の印刷モード
	 * @param[in]	inSpeedList : 印刷モードに紐づく速度一覧
	**/
	CPrintModeData(
		const ST_PDB_PRINT_MODE&	inPrintMode,
		const std::vector<long>&	inSpeedList) : m_PrintMode(inPrintMode), m_SpeedList(inSpeedList)
	{
	}

	CPrintModeData(
		const CPrintModeData&	inData) : m_PrintMode(inData.m_PrintMode), m_SpeedList(inData.m_SpeedList)
	{
	}

	/**
	 * @brief	デストラクタ
	**/
	virtual ~CPrintModeData() {}

	/**
	 * @brief	紙DB用の印刷モード取得
	 * @retval	紙DB用の印刷モード
	**/
	const ST_PDB_PRINT_MODE& GetPrintMode() const { return m_PrintMode; }

	/**
	 * @brief	指定の印刷速度が有効値であるかチェックする
	 * @param[in]	inSpeedValue : 印刷速度　[単位:0.01 m/min]
	 * @retval	true : 有効値である
	 * @retval	false : 有効値でない
	**/
	bool CheckEffectiveSpeed(
		long inSpeedValue) const
	{
		return std::find(m_SpeedList.begin(), m_SpeedList.end(), inSpeedValue) != m_SpeedList.end();
	}

	/**
	 * @brief	印刷モード差異の最高速度設定を取得
	 * @retval	最高印刷速度　[単位:0.01 m/min]
	**/
	long GetMaxSpeed() const
	{
		return *std::max_element(m_SpeedList.begin(), m_SpeedList.end());
	}

private:
	ST_PDB_PRINT_MODE m_PrintMode;	//!< 紙DB用の印刷モード.
	std::vector<long> m_SpeedList;	//!< 印刷モードに紐づく速度一覧[単位:0.01 m/min].
};


//--------//
CDataSystemSetting::CDataSystemSetting()
{
	m_IsDED = false;
	m_IsDebugLog = false;
	m_IsRegisterJI = false;
	m_BasePrintConditionName = "";
	m_NonInspectionAreaDefault = 0.0;
	m_NonInspectionAreaDefault_Punch = 0.0;
	m_FileNameCounter = 0;
	m_dotgainRatio = 0.0;
	m_IsHaveJetInspection = false;
	m_importTimeout = 0;
	memset(&m_CsFileNameCounter, 0x00, sizeof(m_CsFileNameCounter));
	CSM_InitializeCriticalSection(&m_CsFileNameCounter, MOD_PRINT_CONDITION_IMPORTER, "FileNameCounter", 5000);
	m_JIOption = 0;
	m_MagnificationAdjustment = false;
}

CDataSystemSetting::~CDataSystemSetting()
{
	CSM_DeleteCriticalSection(&m_CsFileNameCounter);
}

void CDataSystemSetting::Initialize()
{
	auto isSupport150mpm = [&]()->bool
	{
		bool ret_val = Stub_PrinterStateIF::isSupportOf150mpm(true);
		if( m_IsDED )
		{
			if( ret_val )
			{
				ret_val = Stub_PrinterStateIF::isSupportOf150mpm(false);
			}
		}
		char buf[256];
		sprintf(buf,"isSupport150mpm[%s]",(ret_val) ? "true" : "false");
		std::string msg;
		msg = buf;
		WriteToLogBuf(LOG_DEBUG, msg);
		return	ret_val;
	};

	auto isSupport100mpm = [&]()->bool
	{
		bool ret_val = Stub_PrinterStateIF::isSupportOf100mpm(true);
		if( m_IsDED )
		{
			if( ret_val )
			{
				ret_val = Stub_PrinterStateIF::isSupportOf100mpm(false);
			}
		}
		char buf[256];
		sprintf(buf,"isSupport100mpm[%s]",(ret_val) ? "true" : "false");
		std::string msg;
		msg = buf;
		WriteToLogBuf(LOG_DEBUG, msg);
		return	ret_val;
	};

	auto isPrintModeCoatedOffset = [&]()->bool
	{
		bool ret_val = Stub_PrinterStateIF::isUnitOfPrintModeForCoatedOffset(true);
		if( m_IsDED )
		{
			if( ret_val )
			{
				ret_val = Stub_PrinterStateIF::isUnitOfPrintModeForCoatedOffset(false);
			}
		}
		char buf[256];
		sprintf(buf,"isPrintModeCoatedOffset[%s]",(ret_val) ? "true" : "false");
		std::string msg;
		msg = buf;
		WriteToLogBuf(LOG_DEBUG, msg);
		return	ret_val;
	};

	auto isSixColors = [&]()->bool
	{
		bool ret_val = Stub_PrinterStateIF::isUnitOfSixColors(true);
		if( m_IsDED )
		{
			if( ret_val )
			{
				ret_val = Stub_PrinterStateIF::isUnitOfSixColors(false);
			}
		}
		char buf[256];
		sprintf(buf,"isSixColors[%s]",(ret_val) ? "true" : "false");
		std::string msg;
		msg = buf;
		WriteToLogBuf(LOG_DEBUG, msg);
		return	ret_val;
	};

	auto makePrintModeParam = [&]() {
		CIni_PrinterParameter ini;
		ini.Initialize(TRUE);
		long index = -1;
		
		bool support100mpm = isSupport100mpm();
		bool support150mpm = isSupport150mpm();
		bool PrintModeCoatedOffset = isPrintModeCoatedOffset();
		bool SixColors = isSixColors();
		
		while (true) {
			std::string eqname = ini.getEQName(++index);
			if (eqname.empty()) {
				break;
			}

			ST_PDB_PRINT_MODE mode = {0};
			{
				double resox = 0.0, resoy = 0.0;
				ini.GetPrintModePrintReso(index, &resox, &resoy);
				mode.CMYK_PRN_RESO_X = (long)resox;
				mode.CMYK_PRN_RESO_Y = (long)resoy;
			}
			{
				double resox = 0.0, resoy = 0.0;
				ini.GetPrintModeRIPReso(index, &resox, &resoy);
				mode.CMYK_RIP_RESO_X = (long)resox;
				mode.CMYK_RIP_RESO_Y = (long)resoy;
			}
			{
				double resox = 0.0, resoy = 0.0;
				ini.GetPrintModePrintResoOP(index, &resox, &resoy);
				mode.ELS_PRN_RESO_X = (long)resox;
				mode.ELS_PRN_RESO_Y = (long)resoy;
			}
			{
				double resox = 0.0, resoy = 0.0;
				ini.GetPrintModeRIPResoOP(index, &resox, &resoy);
				mode.ELS_RIP_RESO_X = (long)resox;
				mode.ELS_RIP_RESO_Y = (long)resoy;
			}

			std::string modename;
			ini.getPrintModeName(index,modename);
			_snprintf(mode.RESOURCE_ID, sizeof (mode.RESOURCE_ID) - 1, "%s", modename.c_str());
			long speedID = 0;
			ini.GetPrintSpeedID(modename,speedID);
			long max_speed = ini.getMaxPrintSpeed(mode.CMYK_PRN_RESO_X,mode.CMYK_PRN_RESO_Y,speedID);

			// 100mpmオプション無効時は印刷モード「標準2」（最高速100mpmの印刷モード）を無効とする
			if( !support100mpm )
			{
				if( max_speed == 10000 )	continue;
			}
			
			// 150mpmオプション無効時は印刷モード「高速2」（最高速150mpmの印刷モード）を無効とする
			if( !support150mpm )
			{
				if( max_speed == 15000 )	continue;
			}
			
			// コート紙用印刷モード搭載無効の時は「標準 （オフセットコート紙）」（印刷階調値:36）を無効とする
			if( !PrintModeCoatedOffset )
			{
				long print_step;
				ini.GetPrintStep(modename,print_step);
				if( print_step == 36 )	continue;
			}
			
			// 6色非搭載の時は追加色ありの印刷モードを無効とする
			if( !SixColors )
			{
				if( ini.IsSupportAdditionalColor(index) )	continue;
			}
			
			{
				/**
					網名は複数個存在するが、ここでは先頭のものを設定しておく
				*/
				std::string screening;
				ini.GetSPMName(index + 1, 1, screening);
				_snprintf(mode.SCREENING_FAMILY, sizeof (mode.SCREENING_FAMILY) - 1, "%s", screening.c_str());
			}
			std::vector<long> speeds;
			{
				/**
					@see speedID については、#696 T3 対応の詳細設計書を参照。
				*/
				long keyID = 0;
				long speed = 0;
				while (ini.getPrintSpeed(keyID++, mode.CMYK_PRN_RESO_X, mode.CMYK_PRN_RESO_Y, speedID, speed)) {
					speeds.push_back(speed);
				}
			}
			CPrintModeData data(mode, speeds);
			m_PrintModeList.insert(std::make_pair(eqname, data));
		}
	};


	CIni_PrintConditionImporter ini_PrintConditionImporter;
	ini_PrintConditionImporter.Initialize(TRUE);
	// この処理を抜けるときにini_PrintConditionImporterを閉じる処理を登録する
		std::unique_ptr<CIni_PrintConditionImporter, std::function<void(CIni_PrintConditionImporter*)>> ini_PrintConditionImporterReleaser =
		std::unique_ptr<CIni_PrintConditionImporter, std::function<void(CIni_PrintConditionImporter*)>>(&ini_PrintConditionImporter, [&](CIni_PrintConditionImporter* inIni_PrintConditionImporter)->void{inIni_PrintConditionImporter->Finalize();});

	// DEDかどうかを取得する
	auto getDED = []()->bool
	{
		CIni_PrinterDescriptor ini_PrinterDescriptor;
		ini_PrinterDescriptor.Initialize(TRUE);
		// この処理を抜けるときにini_PrinterDescriptorを閉じる処理を登録する
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>> ini_PrinterDescriptorReleaser =
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>>(&ini_PrinterDescriptor, [&](CIni_PrinterDescriptor* inIni_PrinterDescriptor)->void{inIni_PrinterDescriptor->Finalize();});
		return ((ini_PrinterDescriptor.IsDedSystem() == TRUE)? true : false);
	};

	// インポート時のベースとなる印刷条件名を取得する
	auto getBasePrintConditionName = [&ini_PrintConditionImporter]()->std::string
	{
		std::string name = "";
		ini_PrintConditionImporter.GetBasePrintConditionName(name);
		return name;
	};

	// 非検査エリア(紙端)のデフォルト値（パンチなし用紙用）を取得する
	auto getNonInspectionAreaDefault = []()->double
	{
		// note: JetInspection画面設定のデフォルト値と共用しているため、JetInspection画面設定のデフォルト値を変更するとここにも影響する

		CIni_MainteJetInspection ini_MainteJetInspection;
		ini_MainteJetInspection.Initialize(TRUE);
		// この処理を抜けるときにini_MainteJetInspectionを閉じる処理を登録する
		std::unique_ptr<CIni_MainteJetInspection, std::function<void(CIni_MainteJetInspection*)>> ini_MainteJetInspectionReleaser =
		std::unique_ptr<CIni_MainteJetInspection, std::function<void(CIni_MainteJetInspection*)>>(&ini_MainteJetInspection, [&](CIni_MainteJetInspection* inIni_MainteJetInspection)->void{inIni_MainteJetInspection->Finalize();});
		return ini_MainteJetInspection.GetDefNonInspectionArea_NoPunch() / 100.0;
	};

	// 非検査エリア(紙端)のデフォルト値（パンチあり用紙用）を取得する
	auto getNonInspectionAreaDefault_Punch = []()->double
	{
		// note: JetInspection画面設定のデフォルト値と共用しているため、JetInspection画面設定のデフォルト値を変更するとここにも影響する

		CIni_MainteJetInspection ini_MainteJetInspection;
		ini_MainteJetInspection.Initialize(TRUE);
		// この処理を抜けるときにini_MainteJetInspectionを閉じる処理を登録する
		std::unique_ptr<CIni_MainteJetInspection, std::function<void(CIni_MainteJetInspection*)>> ini_MainteJetInspectionReleaser =
		std::unique_ptr<CIni_MainteJetInspection, std::function<void(CIni_MainteJetInspection*)>>(&ini_MainteJetInspection, [&](CIni_MainteJetInspection* inIni_MainteJetInspection)->void{inIni_MainteJetInspection->Finalize();});
		return ini_MainteJetInspection.GetDefNonInspectionArea_Punch() / 100.0;
	};
	auto getDotgainRatio = []()-> double
	{
		CIni_ToneSettingGUI iniToneSettingGUI;
		iniToneSettingGUI.Initialize(TRUE);
		// この処理を抜けるときにini_ToneSettingGUIを閉じる処理を登録する
		std::unique_ptr<CIni_ToneSettingGUI, std::function<void(CIni_ToneSettingGUI*)>> iniToneSettingGUIReleaser =
		std::unique_ptr<CIni_ToneSettingGUI, std::function<void(CIni_ToneSettingGUI*)>>(&iniToneSettingGUI, [&](CIni_ToneSettingGUI* iniToneSettingGUI)->void{iniToneSettingGUI->Finalize();});
		return iniToneSettingGUI.GetDotgainRatio();
	};
	//JIがあるかどうか
	auto getIsHaveJetInspection = []()->bool
	{
		CIni_PrinterDescriptor ini_PrinterDescriptor;
		ini_PrinterDescriptor.Initialize(TRUE);
		// この処理を抜けるときにini_PrinterDescriptorを閉じる処理を登録する
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>> ini_PrinterDescriptorReleaser =
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>>(&ini_PrinterDescriptor, [&](CIni_PrinterDescriptor* inIni_PrinterDescriptor)->void{inIni_PrinterDescriptor->Finalize();});
		return ((ini_PrinterDescriptor.GetJetInspectionOption() == 0) ? false : true);
	};
	// JetInspectorとの接続設定を取得する
	auto getJetInspectionOption = []()->long
	{
		CIni_PrinterDescriptor ini_PrinterDescriptor;
		ini_PrinterDescriptor.Initialize(TRUE);
		// この処理を抜けるときにini_PrinterDescriptorを閉じる処理を登録する
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>> ini_PrinterDescriptorReleaser =
		std::unique_ptr<CIni_PrinterDescriptor, std::function<void(CIni_PrinterDescriptor*)>>(&ini_PrinterDescriptor, [&](CIni_PrinterDescriptor* inIni_PrinterDescriptor)->void{inIni_PrinterDescriptor->Finalize();});
		auto option = ini_PrinterDescriptor.GetJetInspectionOption();
		return option;
	};

	m_IsDED = getDED();
	m_IsRegisterJI = ini_PrintConditionImporter.IsRegisterJetInspection();
	m_IsDebugLog = ini_PrintConditionImporter.IsDebugLog();
	m_BasePrintConditionName = getBasePrintConditionName();
	m_NonInspectionAreaDefault = getNonInspectionAreaDefault();
	m_NonInspectionAreaDefault_Punch = getNonInspectionAreaDefault_Punch();
	m_dotgainRatio = getDotgainRatio();
	m_IsHaveJetInspection = getIsHaveJetInspection();
	m_importTimeout = ini_PrintConditionImporter.GetTimeoutImport() * 1000;
	m_JIOption = getJetInspectionOption();

	Ini_FlushingPageMagnification	iniFlushingPageMagnification;
	iniFlushingPageMagnification.Initialize(TRUE);
	m_MagnificationAdjustment = iniFlushingPageMagnification.IsMagnificationAdjustment();

	makePrintModeParam();

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


bool CDataSystemSetting::GetPrintMode(
	const std::string&	inPrintModeName,
	ST_PDB_PRINT_MODE&	outPrintMode) const
{
	auto target = m_PrintModeList.find(inPrintModeName);
	if (target == m_PrintModeList.end()) {
		return false;
	}
	outPrintMode = target->second.GetPrintMode();
	return true;
}

bool CDataSystemSetting::CheckEffectiveSpeed(
	const std::string&	inPrintModeName,
	long				inSpeedValue) const
{
	auto target = m_PrintModeList.find(inPrintModeName);
	if (target == m_PrintModeList.end()) {
		return false;
	}
	return target->second.CheckEffectiveSpeed(inSpeedValue);
}

long CDataSystemSetting::GetMaxSpeed(
	const std::string&	inPrintModeName) const
{
	auto target = m_PrintModeList.find(inPrintModeName);
	if (target == m_PrintModeList.end()) {
		return 0;
	}
	return target->second.GetMaxSpeed();
}

bool CDataSystemSetting::IsDED() const
{
	return m_IsDED;
}

bool CDataSystemSetting::IsRegisterJetInspection() const
{
	return m_IsRegisterJI;
}

const std::string& CDataSystemSetting::GetBasePrintConditionName() const
{
	return m_BasePrintConditionName;
}

double CDataSystemSetting::GetNonInspectionAreaDefault(bool isPunched) const
{
	return (isPunched) ? m_NonInspectionAreaDefault_Punch : m_NonInspectionAreaDefault;
}

bool CDataSystemSetting::IsDebugLog() const
{
	return m_IsDebugLog;
}

unsigned long CDataSystemSetting::GetFileCounter()
{
	StCriticalSectionLocker cs(&m_CsFileNameCounter);
	unsigned long ret = m_FileNameCounter++;
	//ファイル名が長くなりすぎないように１秒間に作らなさそうな数にとどめておく
	if(m_FileNameCounter > 100000){
		m_FileNameCounter = 0;
	}
	return ret;
}

double CDataSystemSetting::GetDotgainRatio()
{
	return m_dotgainRatio;
}

DWORD CDataSystemSetting::GetTimeoutImport()
{
	return m_importTimeout;
}

/**
	* @brief	フラッシングページが登録済みか（解像度、JIチェックあり）
	* @param[in] inFlushingPageName	フラッシングページ名
	* @param[in] inResoX			主走査解像度
	* @param[in] inResoY			副走査解像度
	* @retval	true : 登録済み
	* @retval	false : 未登録
**/
bool CDataSystemSetting::isSystemFlushingPage(
	const std::string& inFlushingPageName, 
	const long inResoX, 
	const long inResoY) const
{
	std::vector<flushing_page_list_ini::SItem> flushingPageList;
	bool isExist = Stub_JobManagerIF::GetDetailedFlushingPageList(flushingPageList);
	auto itrEnd = flushingPageList.end();
	for (auto flushingPageListItr = flushingPageList.begin(); flushingPageListItr != itrEnd; ++flushingPageListItr)
	{	
		// 印刷モニター＆ノズル抜けチェック（RTM&NCC）が無効な環境では、ノズル欠けフラッシングページを未登録扱いする
		if (flushingPageListItr->m_IsNozzleCheck && (Stub_PrinterStateIF::IsRTMandNCCEnable() == false))
		{
			continue;
		}

		// 出力解像度にあったデータが用意されているかを確認
		if ((inResoX > 0) && (inResoY > 0))
		{
			int i = flushingPageListItr->m_CreatedCMYKResos.size() - 1;
			while (i >= 0) 
			{
				if ((static_cast<long>(flushingPageListItr->m_CreatedCMYKResos[i].m_ResoX) == inResoX) &&
					(static_cast<long>(flushingPageListItr->m_CreatedCMYKResos[i].m_ResoY) == inResoY)) 
				{
					break;
				}
				i--;
			}

			if (i < 0)
			{
				continue;
			}
		}

		if (inFlushingPageName == flushingPageListItr->m_PageName)
		{
			return true;
		}
	}

	return false;
}

// JetInspectorとの接続設定
long CDataSystemSetting::GetJetInspectionOption() const
{
	return m_JIOption;
}

// プリンターの印刷モード取得
std::string CDataSystemSetting::getPrintModeName(const std::string&	inPrintModeName)
{
	ST_PDB_PRINT_MODE printMode = {0};
	if(GetPrintMode(inPrintModeName, printMode) == false)
	{
		return std::string("");
	}

	CIni_StringsPrinterParameter iniData;
	iniData.Initialize(TRUE);
	char tmpData[256] = {0};
	iniData.GetPrintModeMenName(printMode.RESOURCE_ID, tmpData, sizeof(tmpData));
	iniData.Finalize();

	return std::string(tmpData);
}

/**
 * @brief	自動変倍機能が使用可能か否かを返す
 * @retval	true : 使用可能
 * @retval	false : 使用不可
**/
bool CDataSystemSetting::isAvailableAutoMagnification()
{
	return m_MagnificationAdjustment;
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
