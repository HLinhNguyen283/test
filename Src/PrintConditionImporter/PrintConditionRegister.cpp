/**
 * @file	PrintConditionRegister.cpp
 * @brief	１件分の印刷条件を登録する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintConditionRegister.h"
#include "CommonCBRProc_OP.h"
#include "PrintConditionForXML.h"
#include "Stub_PaperDBIF.h"
#include "Stub_PrinterStateIF.h"
#include "Stub_PrintProfileMgrIF.h"
#include "DotgainFileCreator.h"
#include "UpdatePrintConditionName.h"
#include "DataSystemSetting.h"
#include "CommonConvertUnit.h"
#include "Stub_Mainte_JetInspectionIF.h"
#include <iomanip>
#include "ResDef.h"
#include "ProcPluginCBRProc.h"
// ==========================================================================

// Local variables
static const long NumberOfColors = 5;		//!< 色数定義
static const bool s_front = true;
static const bool s_back = false;


/**
 *	@brief	リソース文字列に文字列引数を合成してログ文字列を合成する
 *	@param inResouceMsg リソースID
 *	@param inArguments 引数文字列の配列(出現順)
 *	@return 合成されたログ文字列
**/
static std::string composeLogMsg(const long inResouceMsg, const std::vector<std::string>& inArguments)
{
	std::string msg = (char*)LoadResourceString(inResouceMsg, RESOURCE_STR);
	for(auto it = inArguments.begin(); it != inArguments.end(); ++it)
	{
		size_t posIdx = msg.find("%s");
		if(posIdx != std::string::npos)
		{
			msg.replace(posIdx, 2, *it);
		}
	}
	return msg;
}

/**
 *	@brief	リソース文字列に文字列引数を合成してログ文字列を合成する(引数1個)
 *	@param inResouceMsg リソースID
 *	@param inArgument 引数文字列
 *	@return 合成されたログ文字列
**/
static std::string composeLogMsg(const long inResouceMsg, const std::string inArgument)
{
	std::vector<std::string> arguments;
	arguments.push_back(inArgument);
	return composeLogMsg(inResouceMsg, arguments);
}

/**
 *	@brief	リソース文字列に文字列引数を合成してログ文字列を合成する(引数2個)
 *	@param inResouceMsg リソースID
 *	@param inArgument1 引数文字列1
 *	@param inArgument2 引数文字列2
 *	@return 合成されたログ文字列
**/
static std::string composeLogMsg(const long inResouceMsg, const std::string inArgument1, const std::string inArgument2)
{
	std::vector<std::string> arguments;
	arguments.push_back(inArgument1);
	arguments.push_back(inArgument2);
	return composeLogMsg(inResouceMsg, arguments);
}

/**
 *	@brief	リソース文字列に文字列引数を合成してログ文字列を合成する(引数3個)
 *	@param inResouceMsg リソースID
 *	@param inArgument1 引数文字列1
 *	@param inArgument2 引数文字列2
 *	@param inArgument3 引数文字列3
 *	@return 合成されたログ文字列
**/
static std::string composeLogMsg(const long inResouceMsg, const std::string inArgument1, const std::string inArgument2, const std::string inArgument3)
{
	std::vector<std::string> arguments;
	arguments.push_back(inArgument1);
	arguments.push_back(inArgument2);
	arguments.push_back(inArgument3);
	return composeLogMsg(inResouceMsg, arguments);
}


/**
 * @brief	CPrintConditionRegisterが使うAPI
**/
class CPrintConditionRegister::API
{
private:
	/**
	 * @brief constructor
	**/
	API();
public:

	/**
	 * @brief destructor
	**/
	virtual ~API();

	/**
	 * @brief	紙DBのシステム設定関連の"存在チェック+追加"の一連処理をしてる間に必要なCSを取得する
	**/
	static void EnterSystemSettingPaperDBCS()
	{
		CStub_PaperDBIF::PDB_EnterCommonPaperDBCS();
	}


	/**
	 * @brief	紙DBのシステム設定関連の"存在チェック+追加"の一連処理をしてる間に必要なCSを解放する
	**/
	static void LeaveSystemSettingPaperDBCS()
	{
		CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS();
	}

	/**
	 * @brief	指定の乾燥温度の存在チェック（システム）./ Check the existence of the dryer temperature information
	 * @param[in]	inDryTempPrint : 乾燥温度 / Dryer temperature information to be checked
	 * @retval	true : 存在する / Found 
	 * @retval	false : 存在しない / Not found
	**/
	static bool IsSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint)
	{
		return CStub_PaperDBIF::PDB_IsSystemDryTempPrint(inDryTempPrint);
	}

	/**
	 * @brief	乾燥温度追加（システム）. / Add the dryer temperature information (to the list in the system)
	 * @param[in]	inDryTempPrint : 新規乾燥温度 / Dryer temperature information to be added
	**/
	static void AddSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint)
	{
		//事前に有無チェックした上で追加するので失敗しないはずだから、戻り値をチェックしない
		CStub_PaperDBIF::PDB_AddSystemDryTempPrint(inDryTempPrint);
	}

	/**
	 * @brief	指定のトーンカーブ情報の存在チェック（システム）./ Check the existence of the tone curve information
	 * @param[in]	inDotgainCurve : トーンカーブ情報 / Tone curve information to be checked
	 * @retval	true : 存在する / Found 
	 * @retval	false : 存在しない / Not found
	**/
	static bool IsSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
	{
		return CStub_PaperDBIF::PDB_IsSystemDotgainCurve(inDotgainCurve);
	}

	/**
	 * @brief	トーンカーブ情報追加（システム）./ Add the tone curve (to the list in the system)
	 * @param[in]	inDotgainCurve : 新規トーンカーブ情報 / Tone curve to be added
	**/
	static void AddSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
	{
		//事前に有無チェックした上で追加するので失敗しないはずだから、戻り値をチェックしない
		CStub_PaperDBIF::PDB_AddSystemDotgainCurve(inDotgainCurve);
	}

	/**
	 * @brief	トーンカーブ情報を全レコードを呼び出し順に取得（システム）./ Inquire tone curve information one by one
	 *           前後でCS取得、解放を行う事. / Acquire the Critical Section beforehand and release it afterward
	 *           必ず失敗するまで実行する必要がある / Repeat calling the function until false is returned.
	 * @param[in]	index : 0からのインデックス値 / Index (starting from 0)
	 * @param[out]	outDotgainCurve : 取得出来たトーンカーブ情報 / Tone curve information
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool GetSystemDotgainCurve(int index, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
	{
		return CStub_PaperDBIF::PDB_GetSystemDotgainCurve(index, outDotgainCurve);
	}

	/**
	 * @brief	全データ取得関数
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inLengthUnit : 長さ単位（DEF_LENGTH_MM or DEF_LENGTH_INCH or DEF_LENGTH_POINT）
	 * @param[in]	inSpeedUnit : 速度の単位（DEF_MODE_RESO_SPEED_M or DEF_MODE_RESO_SPEED_100）
	 * @param[out]	outData : 業務移行用構造体
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool GetAllParameter(const std::string& inPrintConditionName, DEF_LENGTH_UNIT inLengthUnit,
		eMODE_RESO_SPEED_UNIT inSpeedUnit, ST_PDB_MIGRATION_DATA& outData)
	{
		return CStub_PaperDBIF::PDB_GetPrintConditionMigration(inPrintConditionName, inLengthUnit, inSpeedUnit, outData);
	}

	/**
	 * @brief	全データ設定関数
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inData : 業務移行用構造体
	**/
	static void SetAllParameter(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inData)
	{
		//失敗しないはずだから、戻り値をチェックしない
		CStub_PaperDBIF::PDB_SetPrintConditionMigration(inPrintConditionName, inData);
	}

	/**
	 * @brief	指定の印刷条件の存在チェック. / Check the existence of the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition to be checked
	 * @retval	true : 存在する / Found
	 * @retval	false : 存在しない / Not found
	**/
	static bool IsPrintConditionName(const std::string& inPrintConditionName)
	{
		return CStub_PaperDBIF::PDB_IsPrintConditionName(inPrintConditionName.c_str());
	}

	/**
	 * @brief	紙DBをコミットする。Commit the paper DB
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool CommitPaperDB()
	{
		return CStub_PaperDBIF::PDB_CommitPaperDB();
	}
};


// local defines
#define ERRROR_IF_NULL(inValue, inErrStr) { if(inValue == nullptr){ std::stringstream err; err << "Error. Cannot get " << inErrStr; throw std::runtime_error(err.str());} }


CPrintConditionRegister::CPrintConditionRegister(const std::string& inPrintConditionName, CDataSystemSetting& inDataSystemSetting) 
	: m_PrintConditionName(inPrintConditionName), m_DataSystemSetting(inDataSystemSetting), m_PrintModeName(""), m_isAppend(false), m_DebugoutFile("")
{
}

CPrintConditionRegister::~CPrintConditionRegister()
{
	//印刷条件の登録に失敗した際に、作成済みのファイルを全削除する(m_TempFileList)
	for(auto file = m_TempFileList.begin(); file != m_TempFileList.end(); ++file){
		if(DeleteFile((*file).c_str()) == FALSE){
			std::stringstream log;
			log << "CPrintConditionRegister::~CPrintConditionRegister() Error. Cannot delete file. " << *file;
			WriteToLogBuf(LOG_DEBUG, log.str());
		}
	}
}

void CPrintConditionRegister::LoadInitValue(bool inIsAppend)
{
	std::string basePrintConditionName("");
	if(inIsAppend)
	{
		//ベースとなる印刷条件名
		basePrintConditionName = m_DataSystemSetting.GetBasePrintConditionName();
	}
	else
	{
		//上書き対象となる印刷条件名
		basePrintConditionName = m_PrintConditionName;
	}
	// 全レコードを取得
	if(API::IsPrintConditionName(basePrintConditionName) == false){
		std::stringstream err;
		err << "Error. Is not exist. " << basePrintConditionName;
		throw std::runtime_error(err.str());
	}
	if(API::GetAllParameter(basePrintConditionName, UserModeGUI_GetLengthUnit(), DEF_MODE_RESO_SPEED_M, m_DataForPDB) == false){
		std::stringstream err;
		err << "Error. Cannot get all parameters. " << basePrintConditionName;
		throw std::runtime_error(err.str());
	}
}

bool CPrintConditionRegister::IsRegisteredPrintConditionName()
{
	//指定の印刷条件が紙DBに登録済みかチェック
	return API::IsPrintConditionName(m_PrintConditionName);
}

void CPrintConditionRegister::Import(bool inIsAppend, CPrintConditionForXML& inXML)
{
	m_isAppend = inIsAppend;
//	DWORD a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0, a8 = 0, a9 = 0, a10 = 0;
	//ベースの値をデバッグ出力
	debugout(E_DEBUGOUT_DB_BASE, inIsAppend);

	std::pair<bool,bool> hasElements;
	//ベース条件に対して引継ぎの前処理をおこなう
	try
	{
		// フォーマット変換前に必要な情報をXMLから取得
		hasElements = hasJiElements(inXML);
		CUpdatePrintConditionName updatePrintConditionName(m_DataSystemSetting, &m_DataForPDB, hasElements);
		// XMLでのパンチ有無情報を設定
		bool punchPaperFlag = getPunchPaperFlag(inXML);
		updatePrintConditionName.SetPunchedPaperFlag(punchPaperFlag);

		// 前処理本体
		updatePrintConditionName.preProc();

//	a1 = GetTickCount();
		//各要素を紙DBに登録するフォーマットに変換する
		setDataForPDB(inXML);

//	a2 = GetTickCount();
		//別途設定が必要な項目に対して、複製処理を行う		
//	a3 = GetTickCount();

//	a4 = GetTickCount();
		if(inIsAppend == true)
		{
			updatePrintConditionName.CopyNecessaryFiles(m_TempFileList);
		}
//	a5 = GetTickCount();
		//システムに未登録の項目があれば設定する
		addSystemItem();

		API::EnterSystemSettingPaperDBCS();

//	a6 = GetTickCount();
		if(inIsAppend == true){
			//印刷条件名の新規追加を依頼する
			updatePrintConditionName.Add(m_PrintConditionName);
		}
		
//	a7 = GetTickCount();
		//全データ設定
		API::SetAllParameter(m_PrintConditionName, m_DataForPDB);

		API::LeaveSystemSettingPaperDBCS();

//	a8 = GetTickCount();
		//紙DBへ反映する
		CommitToPaperDB();
//	a9 = GetTickCount();
		m_TempFileList.clear();
//	a10 = GetTickCount();
		//更新後の値をデバッグ出力
		debugout(E_DEBUGOUT_DB_UPDATE);

/*		std::stringstream log;
		log << "Time:[" <<a10 << "," <<a1 << "] Total:" << a10-a1 << "," << a2-a1 << "," << a3-a2 << ","
			<< a4-a3 << "," << a5-a4 << "," << a6-a5 << "," << a7-a6 << "," << a8-a7
			<< "," << a9-a8 << "," << a10-a9;
		WriteToLogBuf(LOG_DEBUG, log.str());*/
	}
	catch(int){
		throw std::runtime_error("Error. CPrintConditionRegister::Import");
	}
}

void CPrintConditionRegister::setDataForPDB(const CPrintConditionForXML& inXML)
{
	//PDB_AddPrintConditionName 該当項目
	ImportPrintConditionName(
		inXML.GetString(CPrintConditionForXML::kPrintConditionName)
	);

	//PDB_SetPaperType 該当項目
	ImportPaperType(
		inXML.GetString(CPrintConditionForXML::kPaperType)
	);
	//PDB_SetLevelTuneName 該当項目
	ImportLevelTuneName(
		inXML.GetString(CPrintConditionForXML::kLevelTuneName)
	);

	//PDB_SetPaperSizeW 該当項目
	ImportPaperSizeW(
		inXML.GetDouble(CPrintConditionForXML::kPaperWidth)
	);

	//PDB_SetPaperThickness 該当項目
	ImportPaperThickness(
		inXML.GetLong(CPrintConditionForXML::kPaperThickness)
	);

	//PDB_SetPaperTension 該当項目
	ImportPaperTension(
		inXML.GetLong(CPrintConditionForXML::kPaperTension)
	);

	//PDB_SetDNSStatus 該当項目
	ImportDNS(
		inXML.GetBool(CPrintConditionForXML::kDNS)
	);

	//PDB_SetPaperOption 該当項目
	ImportPaperOption(
		inXML.GetBool(CPrintConditionForXML::kPaperPunchHole),
		inXML.GetBool(CPrintConditionForXML::kPaperSewing),
		inXML.GetBool(CPrintConditionForXML::kPaperMarker),
		inXML.GetBool(CPrintConditionForXML::kPaperPrePrinted),
		inXML.GetDouble(CPrintConditionForXML::kPaperMarkerOffset),
		inXML.GetMarkSide(CPrintConditionForXML::kPaperMarkSide)
	);

	//PDB_SetPaperSizeH 該当項目
	ImportPaperSizeH(
		inXML.GetDouble(CPrintConditionForXML::kPaperPageFeedSize),
		inXML.GetPageFeedUnit(CPrintConditionForXML::kPaperPageFeedSizeUnit)
	);

	//PDB_SetPrintMode 該当項目
	ImportPrintMode(
		inXML.GetString(CPrintConditionForXML::kPrintMode)
	);

	//PDB_SetModeResoSpeed 該当項目
	ImportModeResoSpeed(
		inXML.GetDouble(CPrintConditionForXML::kPrintSpeed)
	);

	//PDB_SetDryTempPrint 該当項目
	ImportDryTempPrint(
		inXML.GetLong(CPrintConditionForXML::kHeatRollerSelf),
		inXML.GetLong(CPrintConditionForXML::kHeatRollerPart),
		inXML.GetLong(CPrintConditionForXML::kHeatFanSelf),
		inXML.GetLong(CPrintConditionForXML::kHeatFanPart),
		inXML.GetLong(CPrintConditionForXML::kNIRPowerSelf),
		inXML.GetLong(CPrintConditionForXML::kNIRPowerPart),
		inXML.GetLong(CPrintConditionForXML::kPrecoater1),
		inXML.GetLong(CPrintConditionForXML::kPrecoater2),
		inXML.GetLong(CPrintConditionForXML::kSubHeatRollerSelf),
		inXML.GetLong(CPrintConditionForXML::kSubHeatRollerPart)
	);

	//PDB_SetICCPreset 該当項目
	ImportICCPreset(
		inXML.GetString(CPrintConditionForXML::kICCPresetName),
		inXML.GetString(CPrintConditionForXML::kICCPresetID)
	);

	//PDB_SetDotgainCurve 該当項目
	ImportDotgainCurve(
		inXML.GetTone(CPrintConditionForXML::kToneCurve)
	);

	//PDB_SetJobBaseParameterWithPrintConditionName 該当項目
	ImportJobBaseParameterWithPrintConditionName(
		inXML.GetBool(CPrintConditionForXML::kJobBaseInformationSheet),
		inXML.GetBool(CPrintConditionForXML::kJobBaseFlushingPage),
		inXML.GetString(CPrintConditionForXML::kJobBasePatternName),
		inXML.GetLong(CPrintConditionForXML::kJobBaseInterval),
		inXML.GetBool(CPrintConditionForXML::kJobBasePreJob),
		inXML.GetLong(CPrintConditionForXML::kJobBasePreJobCount),
		inXML.GetBool(CPrintConditionForXML::kJobBasePostJob),
		inXML.GetLong(CPrintConditionForXML::kJobBaseOffsetFront),
		inXML.GetLong(CPrintConditionForXML::kJobBaseOffsetBack),
		inXML.GetLong(CPrintConditionForXML::kJobBaseCleaningJudgmentLevelBeforeJob),
		inXML.GetLong(CPrintConditionForXML::kJobBaseCleaningJudgmentLevelAfterJob),
		inXML.GetString(CPrintConditionForXML::kJobBasePrintStopSetting)
	);

	//PDB_SetLayoutImageParameterWithPrintConditionName 該当項目
	ImportLayoutImageParameterWithPrintConditionName(
		inXML.GetXYPair(CPrintConditionForXML::kLayoutOffsetFront),
		inXML.GetXYPair(CPrintConditionForXML::kLayoutOffsetBack),
		inXML.GetBool(CPrintConditionForXML::kLayoutVerification),
		inXML.GetLong(CPrintConditionForXML::kLayoutContinuousPageCount),
		inXML.GetBool(CPrintConditionForXML::kLayoutCueMark),
		inXML.GetBool(CPrintConditionForXML::kLayoutBarcode),
		inXML.GetXYPair(CPrintConditionForXML::kLayoutBarcodeOffsetFront),
		inXML.GetXYPair(CPrintConditionForXML::kLayoutBarcodeOffsetBack),
		inXML.GetString(CPrintConditionForXML::kLayoutFlushingPattern),
		inXML.GetLong(CPrintConditionForXML::kLayoutGapBetweenColors),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineOffsetFront),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineOffsetBack),
		inXML.GetBool(CPrintConditionForXML::kLayoutLineGap),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineGapFront),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineGapBack),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineGapOffsetFront),
		inXML.GetLong(CPrintConditionForXML::kLayoutLineGapOffsetBack),
		inXML.GetString(CPrintConditionForXML::kLayoutLinePosotion),
		inXML.GetString(CPrintConditionForXML::kLayoutLineOptions),
		inXML.GetCutterMarkSide(CPrintConditionForXML::kLayoutCutterMark),
		inXML.GetLong(CPrintConditionForXML::kLayoutCutterMarkPageCount),
		inXML.GetXYPair(CPrintConditionForXML::kLayoutCutterMarkSize),
		inXML.GetLong(CPrintConditionForXML::kLayoutCutterMarkOffsetFront),
		inXML.GetLong(CPrintConditionForXML::kLayoutCutterMarkOffsetBack),
		inXML.GetCutterMarkAlign(CPrintConditionForXML::kLayoutCutterMarkAlignFront),
		inXML.GetCutterMarkAlign(CPrintConditionForXML::kLayoutCutterMarkAlignBack),
		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		inXML.GetBool(CPrintConditionForXML::kLayoutAdjustmentSheetSizeEnabled),
		inXML.GetLong(CPrintConditionForXML::kLayoutAdjustmentSheetLength),
		inXML.GetLong(CPrintConditionForXML::kLayoutAdjustmentSheetAfterJob),
		inXML.GetLong(CPrintConditionForXML::kLayoutAfterJobSheets)
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	);

	// HDでは「フルスペック検査装置搭載環境」のみInspectionタグをインポート
	if(m_DataSystemSetting.GetJetInspectionOption() == 2)
	{
		ImportInspection(
			inXML.GetInspection(CPrintConditionForXML::kInspection),
			inXML.GetString(CPrintConditionForXML::kInspectionJICondition),
			inXML.GetString(CPrintConditionForXML::kInspectionTemporalMaskFile)
		);
	}

	// HDでは「フルスペック検査装置搭載環境 or OCRデコード有効環境」のみDecodeタグをインポート
	if(m_DataSystemSetting.GetJetInspectionOption() == 2 || (Stub_PrinterStateIF::isOCRandDECEnable() == true) )
	{

		//PDB_SetLayoutImageParameterWithPrintConditionNameEx 該当項目
		ImportDecode(
			inXML.GetDecode(CPrintConditionForXML::kDecode),
			inXML.GetLong(CPrintConditionForXML::kDecodeErrorSheets),
			inXML.GetString(CPrintConditionForXML::kDecodeUnit),
			inXML.GetBool(CPrintConditionForXML::kDecodeIsEnabled)
		);
	}

	// 自動変倍機能が使用可能な場合Scaling情報をインポート
	if(m_DataSystemSetting.isAvailableAutoMagnification())
	{
		ImportScaling(
			inXML.GetBool(CPrintConditionForXML::kScalingValue),		
			inXML.GetString(CPrintConditionForXML::kScalingBaseSide),	
			inXML.GetDouble(CPrintConditionForXML::kScalingValueFrontW),
			inXML.GetDouble(CPrintConditionForXML::kScalingValueFrontH),
			inXML.GetDouble(CPrintConditionForXML::kScalingValueBackW),
			inXML.GetDouble(CPrintConditionForXML::kScalingValueBackH),
			m_DataSystemSetting.IsDED(),
			m_DataForPDB.RipMagnification
		);
	}
}

void CPrintConditionRegister::ImportPrintConditionName(
	const CValue<std::string>* inPrintConditionName
)
{
	ERRROR_IF_NULL(inPrintConditionName, "kPrintConditionName");

	/// @remark CDataRequestImport::OneImport()で取得しており、業務移行用構造体にも該当フィールドがないので何もしていない
}

//用紙種類パラメータを決定する
void CPrintConditionRegister::ImportPaperType(
	const CValue<std::string>* inPaperType
)
{
	ERRROR_IF_NULL(inPaperType, "kPaperType");

	//既存用紙種類名が設定されている場合のみ取り込み可能
	//（用紙種類が存在しない場合は取り込みエラー）

	m_isAddPaperType = false;

	//用紙種類
	std::string paperType = "";
	if(inPaperType->Get(paperType) == true)
	{
		bool isSystemPaperType = CStub_PaperDBIF::PDB_IsSystemPaperType(paperType.c_str());
		if(isSystemPaperType)
		{	// 既存用紙種類名が設定されている場合のみ取り込み可能
			m_DataForPDB.PaperType = paperType;
		}
		else
		{
#ifdef	NOT_APPEND_PAPER_TYPE
			// （用紙種類が存在しない場合は取り込みエラー）
			//　用紙種類名[%s]が未定義のため、印刷条件[%s]を取り込むことが出来ません。 
			//用紙種類を追加してください。
			std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_DEFINED_PAPERTYPE, paperType, m_PrintConditionName);
			throw LogSupressException(msg);
#else
			// 用紙種類名未定義の場合、追加対応を実施する
			bool isAddShading = Stub_PrintProfileMgrIF::AddPrintMode(kPrintProfile_Kind_Shading,paperType.c_str());
			bool isAddLinearize = Stub_PrintProfileMgrIF::AddPrintMode(kPrintProfile_Kind_Linearize,paperType.c_str());
			if( isAddShading && isAddLinearize )
			{
				// シェーディングおよびリニアライズのデフォルト設定プロファイル情報作成に成功
				if( CStub_PaperDBIF::PDB_AddSystemPaperType(paperType.c_str()) )
				{
					// 印刷条件DB、システム情報上への用紙種類追加に成功
					m_DataForPDB.PaperType = paperType;
					m_isAddPaperType = true;
				}
				else
				{
					// 印刷条件DB、システム情報上への用紙種類追加に失敗
					std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_APPEND_PAPERTYPE, paperType, m_PrintConditionName);
					throw LogSupressException(msg);
				}
			}
			else
			{
				// シェーディングまたはリニアライズのデフォルト設定プロファイル情報作成に失敗
				long res_id;
				if( !(isAddShading || isAddLinearize) )		res_id = IDS_PROFILE_SHADING_AND_LINEARIZE;
				else if( !isAddShading )					res_id = IDS_PROFILE_SHADING;
				else										res_id = IDS_PROFILE_LINEARIZE;
				std::string add_msg = (char*)LoadResourceString(res_id, RESOURCE_STR);
				std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_APPEND_PAPERTYPE_PROFILE, paperType, add_msg, m_PrintConditionName);
				throw LogSupressException(msg);
			}
#endif
		}
	}	
}

//段差補正名パラメータを決定する
void CPrintConditionRegister::ImportLevelTuneName(
	const CValue<std::string>* inLevelTuneName
)
{
	ERRROR_IF_NULL(inLevelTuneName, "kLevelTuneName");

	//既存段差補正名が設定されている場合のみ取り込み可能
	//存在しない場合はベース印刷条件の値を採用

	m_isAddLevelTuneName = false;

	//段差補正名
	std::string levelTuneName = "";
	if(inLevelTuneName->Get(levelTuneName) == true)
	{
		bool isSystemLevelTuneName = CStub_PaperDBIF::PDB_IsSystemLevelTuneName(levelTuneName.c_str());
		if(isSystemLevelTuneName)
		{
			m_DataForPDB.LevelTuneName = levelTuneName;
		}
#ifdef	NOT_APPEND_ALLIGNMENT
		else if(m_isAppend)
		{	// 新規のときのみログ出力
			// （存在しない場合はBasePrintConditon設定を採用）
			//　段差補正名[%s]が未定義のため、印刷条件[%s]には印刷条件
			//[BasePrintCondition]の段差補正設定を適用しました。
			std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_DEFINED_ALLIGNMENT, levelTuneName, m_PrintConditionName);
			WriteToLogBuf(LOG_STATUS, msg);
		}
#else
		else
		{
			// 段差補正名未定義の場合、追加対応を実施する
			bool isAddAllignment = Stub_PrintProfileMgrIF::AddPrintMode(kPrintProfile_Kind_HeadAlignment,levelTuneName.c_str());
			bool isAddRegister = Stub_PrintProfileMgrIF::AddPrintMode(kPrintProfile_Kind_RegisterSetting,levelTuneName.c_str());
			if( isAddAllignment && isAddRegister )
			{
				// 段差補正および見当設定のデフォルト設定プロファイル情報作成に成功
				if( CStub_PaperDBIF::PDB_AddSystemLevelTuneName(levelTuneName.c_str()) )
				{
					// 印刷条件DB、システム情報上への段差補正追加に成功
					m_DataForPDB.LevelTuneName = levelTuneName;
					m_isAddLevelTuneName = true;
				}
				else
				{
					// 印刷条件DB、システム情報上への段差補正追加に失敗
					long res_id = ( m_isAppend ) ? IDS_STAT_IMPORT_NOT_APPEND_ALLIGNMENT : IDS_STAT_IMPORT_NOT_MODIFIED_ALLIGNMENT;
					std::string msg = composeLogMsg(res_id, levelTuneName, m_PrintConditionName);
					WriteToLogBuf(LOG_STATUS, msg);
				}
			}
			else
			{
				// 段差補正および見当設定のデフォルト設定プロファイル情報作成に失敗
				if( !isAddAllignment )
				{
					std::stringstream msg;
					msg << "CPrintConditionRegister::ImportLevelTuneName Not Create HeadAllignment profile[Name:" << levelTuneName << "]";
					WriteToLogBuf(LOG_DEBUG, msg.str());
				}
				if( !isAddRegister )
				{
					std::stringstream msg;
					msg << "CPrintConditionRegister::ImportLevelTuneName Not Create RegisterSetting profile[Name:" << levelTuneName << "]";
					WriteToLogBuf(LOG_DEBUG, msg.str());
				}
				long res_id = ( m_isAppend ) ? IDS_STAT_IMPORT_NOT_APPEND_ALLIGNMENT : IDS_STAT_IMPORT_NOT_MODIFIED_ALLIGNMENT;
				std::string msg = composeLogMsg(res_id, levelTuneName, m_PrintConditionName);
				WriteToLogBuf(LOG_STATUS, msg);
			}
		}


#endif
	}	
}

//用紙幅設定用パラメータを決定する
void CPrintConditionRegister::ImportPaperSizeW(
	const CValue<double>* inPaperWidth
)
{
	ERRROR_IF_NULL(inPaperWidth, "kPaperWidth");

	//用紙幅(point→0.001mm単位)
	m_DataForPDB.PaperSizeW = static_cast<long>((ConvertUnit(inPaperWidth->Get(), DEF_LENGTH_POINT, DEF_LENGTH_MM) * 1000.0) + 0.5);
	m_DataForPDB.LengthUnit_PaperW = DEF_LENGTH_MM;
}

void CPrintConditionRegister::ImportPaperThickness(
	const CValue<long>* inPaperThickness
)
{
	ERRROR_IF_NULL(inPaperThickness, "kPaperThickness");

	//用紙厚み
	m_DataForPDB.Thickness = inPaperThickness->Get();
}

void CPrintConditionRegister::ImportPaperTension(
	const CValue<long>* inPaperTension
)
{
	ERRROR_IF_NULL(inPaperTension, "kPaperTension");

	//テンション
	m_DataForPDB.Tension = inPaperTension->Get();
}

void CPrintConditionRegister::ImportDNS(
	const CValue<bool>* inDns
)
{
	ERRROR_IF_NULL(inDns, "kDNS");

	bool dns= false;
	if( inDns->Get(dns) == true )
	{
		//DNS
		m_DataForPDB.DNSStatus = dns;
	}
}

//紙オプション設定用パラメータを決定する
void CPrintConditionRegister::ImportPaperOption(
	const CValue<bool>* inPaperPunchHole,
	const CValue<bool>* inPaperSewing,
	const CValue<bool>* inPaperMarker,
	const CValue<bool>* inPaperPrePrinted,
	const CValue<double>* inPaperMarkerOffset,
	const CValue<CPrintConditionForXML::EMarkSide>* inPaperMarkSide//,
)
{
	ERRROR_IF_NULL(inPaperPunchHole, "kPaperPunchHole");
	ERRROR_IF_NULL(inPaperSewing, "kPaperSewing");
	ERRROR_IF_NULL(inPaperMarker, "kPaperMarker");
	ERRROR_IF_NULL(inPaperPrePrinted, "kPaperPrePrinted");
	ERRROR_IF_NULL(inPaperMarkerOffset, "kPaperMarkerOffset");
	ERRROR_IF_NULL(inPaperMarkSide, "kPaperMarkSide");

	//パンチ穴用紙
	m_DataForPDB.PaperOption.IS_HOLES = ((inPaperPunchHole->Get() && !(inPaperPunchHole->Get() && inPaperSewing->Get())) == true? TRUE : FALSE);

	//ミシン目用紙
	m_DataForPDB.PaperOption.IS_HOLES_PERFORATIONS = ((inPaperPunchHole->Get() && inPaperSewing->Get()) == true? TRUE : FALSE);

	//事前マーク
	m_DataForPDB.PaperOption.IS_PRE_MARK = (inPaperMarker->Get() == true? TRUE : FALSE);

	//事前印刷
	m_DataForPDB.PaperOption.IS_PRE_MARK_PAPER = (inPaperPrePrinted->Get() == true? TRUE : FALSE);

	//マークオフセット（主走査）
	if(inPaperMarker->Get() == true){
		double paperMarkerOffset = 0.0;
		if( inPaperMarkerOffset->Get(paperMarkerOffset) == true)
		{	// Markerが trueの場合のみ取り込み実施。
			m_DataForPDB.LengthUnit_Option = DEF_LENGTH_MM;
			m_DataForPDB.PaperOption.PRE_MARK_OFFSET = static_cast<long>((ConvertUnit(paperMarkerOffset, DEF_LENGTH_POINT, m_DataForPDB.LengthUnit_Option) * 1000.0) + 0.5);
		}
		else
		{	// (Markerが trueで記述省略時はエラー)
			throw std::runtime_error("Error. kPaperMarker is true, but kPaperMarkerOffset is not defined");
		}
	}

	//マークの印刷面
	CPrintConditionForXML::EMarkSide paperMarkSide = CPrintConditionForXML::kMarkSide_Empty;
	if(inPaperMarker->Get() == true){
		if(inPaperMarkSide->Get(paperMarkSide) == true)
		{	// Markerが trueの場合のみ取り込み実施。
			switch(paperMarkSide){
			case CPrintConditionForXML::kMarkSide_Front:
				m_DataForPDB.PaperOption.PRE_MARK_SIDE = DEF_PRINT_FRONT;
				break;
			case CPrintConditionForXML::kMarkSide_Back:
				m_DataForPDB.PaperOption.PRE_MARK_SIDE = DEF_PRINT_BACK;
				break;
			default:
				throw std::runtime_error("Error. Undefined kPaperMarkSide");
			}
		}
		else
		{	// (Markerが trueで記述省略時はエラー)
			throw std::runtime_error("Error. kPaperMarker is true, but kPaperMarkSide is not defined");
		}
	}
}

void CPrintConditionRegister::ImportPaperSizeH(
	const CValue<double>* inPaperPageFeedSize,
	const CValue<CPrintConditionForXML::EPageFeedUnit>* inPaperPageFeedSizeUnit
)
{
	ERRROR_IF_NULL(inPaperPageFeedSize, "kPaperPageFeedSize");
	ERRROR_IF_NULL(inPaperPageFeedSizeUnit, "kPaperPageFeedSizeUnit");

	double sizeH = inPaperPageFeedSize->Get();
	CPrintConditionForXML::EPageFeedUnit unit = inPaperPageFeedSizeUnit->Get();

	//値毎に変換する
	eUNIT_MODE unitMode = UNIT_MODE_DEFAULT;
	switch(unit){
	case CPrintConditionForXML::kPageFeedUnit_mm:
		unitMode = UNIT_MODE_MM;
		break;
	case CPrintConditionForXML::kPageFeedUnit_inch:
		unitMode = UNIT_MODE_INCH;
		break;
	case CPrintConditionForXML::kPageFeedUnit_inch2:
		unitMode = UNIT_1PER2_INCH;
		break;
	case CPrintConditionForXML::kPageFeedUnit_inch6:
		unitMode = UNIT_1PER6_INCH;
		break;
	default:
		std::stringstream err;
		err << "Error. No PaperPageFeedSizeUnit. " << unit;
		throw std::runtime_error(err.str());
	}

	//ページ送りサイズ
	m_DataForPDB.PaperSizeH.SIZE_H = static_cast<long>(ConvertUnit(sizeH, UNIT_MODE_POINT, unitMode) * 1000.0 + 0.5);
	if(m_DataForPDB.PaperSizeH.SIZE_H == 0){
		m_DataForPDB.PaperSizeH.IS_SIZE_H = FALSE;
	}else{
		m_DataForPDB.PaperSizeH.IS_SIZE_H = TRUE;
	}

	//ページ送りサイズの単位
	m_DataForPDB.PaperSizeH.UNIT = unitMode;
}

void CPrintConditionRegister::ImportPrintMode(
	const CValue<std::string>* inPrintMode
)
{
	ERRROR_IF_NULL(inPrintMode, "kPrintMode");

	//印刷モード
	m_PrintModeName = inPrintMode->Get();

	//印刷モード一覧から該当の印刷モード名のものを探して、なければ例外を投げる
	if(m_DataSystemSetting.GetPrintMode(m_PrintModeName, m_DataForPDB.PrintMode) == false){
		//　このプリンターで未サポートの印刷モード[%s]を設定しているため、印刷条件
		//[%s]を取り込むことが出来ません。
		std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_DEFINED_PRINTMODE, m_PrintModeName, m_PrintConditionName);
		throw LogSupressException(msg);
	}
}

//印刷速度設定用データを決定する
void CPrintConditionRegister::ImportModeResoSpeed(
	const CValue<double>* inPrintSpeed
)
{
	ERRROR_IF_NULL(inPrintSpeed, "kPrintSpeed");

	//印刷速度
	double printSpeed = 0.0;
	if(inPrintSpeed->Get(printSpeed) == true)
	{
		m_DataForPDB.PrntResoX = m_DataForPDB.PrintMode.CMYK_PRN_RESO_X;
		m_DataForPDB.PrntResoY = m_DataForPDB.PrintMode.CMYK_PRN_RESO_Y;
		m_DataForPDB.PrintSpeed = static_cast<long>(printSpeed * 100);
		m_DataForPDB.SpeedUnit = DEF_MODE_RESO_SPEED_100;

		//PrinterParameter.iniに未定義の印刷速度が指定された場合は取り込みエラーになる
		if(m_DataSystemSetting.CheckEffectiveSpeed(m_PrintModeName, m_DataForPDB.PrintSpeed) == false){
			std::string mode = m_DataSystemSetting.getPrintModeName(m_PrintModeName);
			std::stringstream speed;
			speed << m_DataForPDB.PrintSpeed;
			//　このプリンターの印刷モード[%s]で未サポートの印刷速度[%s]を設定している
			//ため、印刷条件[%s]を取り込むことが出来ません。
			std::string msg = composeLogMsg(
				IDS_STAT_IMPORT_NOT_DEFINED_PRINTSPEED, 
				mode, 
				speed.str(), 
				m_PrintConditionName);
			throw LogSupressException(msg);
		}
	}
	else
	{
//		WriteToLogBuf(LOG_DEBUG, "CPrintConditionRegister::ImportModeResoSpeed print speed is null");

		// XML上で速度指定が無い場合は、印刷モードの最高速を採用する
		m_DataForPDB.PrntResoX = m_DataForPDB.PrintMode.CMYK_PRN_RESO_X;
		m_DataForPDB.PrntResoY = m_DataForPDB.PrintMode.CMYK_PRN_RESO_Y;
		m_DataForPDB.PrintSpeed = static_cast<long>(printSpeed * 100);
		m_DataForPDB.SpeedUnit = DEF_MODE_RESO_SPEED_100;
		m_DataForPDB.PrintSpeed = m_DataSystemSetting.GetMaxSpeed(m_PrintModeName);
		
		char buf[256];
		sprintf(buf,"CPrintConditionRegister::ImportModeResoSpeed set speed[%d]",m_DataForPDB.PrintSpeed);
		std::string msg;
		msg = buf;
		WriteToLogBuf(LOG_DEBUG,msg);
	}
}

//乾燥温度設定用データを決定する
void CPrintConditionRegister::ImportDryTempPrint(
	const CValue<long>* inHeatRollerSelf,
	const CValue<long>* inHeatRollerPart,
	const CValue<long>* inHeatFanSelf,
	const CValue<long>* inHeatFanPart,
	const CValue<long>* inNIRPowerSelf,
	const CValue<long>* inNIRPowerPart,
	const CValue<long>* inPrecoater1,
	const CValue<long>* inPrecoater2,
	const CValue<long>* inSubHeatRollerSelf,
	const CValue<long>* inSubHeatRollerPart
)
{
	ERRROR_IF_NULL(inHeatRollerSelf, "kHeatRollerSelf");
	ERRROR_IF_NULL(inHeatRollerPart, "kHeatRollerPart");
	ERRROR_IF_NULL(inHeatFanSelf, "kHeatFanSelf");
	ERRROR_IF_NULL(inHeatFanPart, "kHeatFanPart");
	ERRROR_IF_NULL(inNIRPowerSelf, "kNIRPowerSelf");
	ERRROR_IF_NULL(inNIRPowerPart, "kNIRPowerPart");
	ERRROR_IF_NULL(inPrecoater1, "kPrecoater1");
	ERRROR_IF_NULL(inPrecoater2, "kPrecoater2");
	ERRROR_IF_NULL(inSubHeatRollerSelf, "kSubHeatRollerSelf");
	ERRROR_IF_NULL(inSubHeatRollerPart, "kSubHeatRollerPart");

	//ヒートローラー乾燥温度（自機）
	long heatRollerSelf = 0;
	if(inHeatRollerSelf->Get(heatRollerSelf) == true){
		m_DataForPDB.DryTempPrint.HEAT_TEMP_SELF = heatRollerSelf;
	}

	if(m_DataSystemSetting.IsDED() == true)
	{
		//ヒートローラー乾燥温度（他機）
		long heatRollerPart = 0;
		if(inHeatRollerPart->Get(heatRollerPart) == true){
			m_DataForPDB.DryTempPrint.HEAT_TEMP_PART = heatRollerPart;
		}
	}

	//温風ファン搭載（自機）
	if(Stub_PrinterStateIF::isLoadingHeaterFan(true))
	{
		long heatFanSelf = 0;
		if(inHeatFanSelf->Get(heatFanSelf) == true)
		{
			//温風アシスト乾燥温度（自機）
			m_DataForPDB.DryTempPrint.FAN_TEMP_SELF = heatFanSelf;
		}
	}

	//温風ファン搭載（他機）
	if(m_DataSystemSetting.IsDED() == true)
	{
		if(Stub_PrinterStateIF::isLoadingHeaterFan(false))
		{
			long heatFanPart = 0;
			if(inHeatFanPart->Get(heatFanPart) == true)
			{
				//温風アシスト乾燥温度（他機）
				m_DataForPDB.DryTempPrint.FAN_TEMP_PART = heatFanPart;
			}
		}
	}

	//NIR搭載（自機）
	if(Stub_PrinterStateIF::isUnitOfNir(true))
	{
		long nIRPowerSelf = 0;
		if(inNIRPowerSelf->Get(nIRPowerSelf) == true)
		{
			//IR設定（自機）
			m_DataForPDB.DryTempPrint.NIR_POWER_SELF = nIRPowerSelf;
		}
	}

	//NIR搭載（他機）
	if(m_DataSystemSetting.IsDED() == true)
	{
		if(Stub_PrinterStateIF::isUnitOfNir(false))
		{
			long nIRPowerPart = 0;
			if(inNIRPowerPart->Get(nIRPowerPart) == true)
			{
				//IR設定（他機）
				m_DataForPDB.DryTempPrint.NIR_POWER_PART = nIRPowerPart;
			}
		}
	}

	//プレコート搭載
	if(Stub_PrinterStateIF::isLoadingPreCoat())
	{
		long precoater1 = 0;
		if(inPrecoater1->Get(precoater1) == true)
		{
			//先塗り乾燥温度１
			m_DataForPDB.DryTempPrint.PRE_TEMP_SELF = precoater1;
		}
		if(m_DataSystemSetting.IsDED() == true)
		{
			long precoater2 = 0;
			if(inPrecoater2->Get(precoater2) == true)
			{
				//先塗り乾燥温度２
				m_DataForPDB.DryTempPrint.PRE_TEMP_PART = precoater2;
			}
		}
	}

	//Ranger搭載
	if(Stub_PrinterStateIF::isUnitOfRanger())
	{
		//サブヒートローラー乾燥温度（自機）
		long subHeatRollerSelf = 0;
		if(inSubHeatRollerSelf->Get(subHeatRollerSelf) == true){
			m_DataForPDB.DryTempPrint.SUB_HEAT_TEMP_SELF = subHeatRollerSelf;
		}

		if(m_DataSystemSetting.IsDED() == true)
		{
			//サブヒートローラー乾燥温度（他機）
			long SubheatRollerPart = 0;
			if(inSubHeatRollerPart->Get(SubheatRollerPart) == true){
				m_DataForPDB.DryTempPrint.SUB_HEAT_TEMP_PART = SubheatRollerPart;
			}
		}
	}
}

void CPrintConditionRegister::ImportICCPreset(
	const CValue<std::string>* inICCPresetName,
	const CValue<std::string>* inICCPresetID
)
{
	ERRROR_IF_NULL(inICCPresetName, "kICCPresetName");
	ERRROR_IF_NULL(inICCPresetID, "kICCPresetID");

	//ICCプリセット名
	std::string						ICCPreset = "";
	if(inICCPresetName->Get(ICCPreset) == true)
	{
		m_DataForPDB.ICCPreset = ICCPreset;
	}

	//ICCプリセット ID
	std::string						ICCPresetID = "";
	if(inICCPresetID->Get(ICCPresetID) == true)
	{
		m_DataForPDB.ICCPresetID = ICCPresetID;
	}
}

//トーンカーブ情報設定用データを決定する
void CPrintConditionRegister::ImportDotgainCurve(
	const CValue<CPrintConditionForXML::ToneCurve>* inToneCurve
)
{
	ERRROR_IF_NULL(inToneCurve, "kToneCurve");

	// 記述省略時は「デフォルト設定」:名前が空文字列
	m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_SELF].DOTGAIN_NAME[0] = '\0';
	m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_PART].DOTGAIN_NAME[0] = '\0';
	m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_SELF].DOTGAIN_SHAR_NAME[0] = '\0';
	m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_PART].DOTGAIN_SHAR_NAME[0] = '\0';
	CPrintConditionForXML::ToneCurve toneCurve;
	if(inToneCurve->Get(toneCurve) == true){
		if(m_DataSystemSetting.IsDED() == true){
			//DED(自機)
			importDotgainCurve(toneCurve, COM_SYSTEM_TYPE_SELF, false);

			//DED(相手機)
			importDotgainCurve(toneCurve, COM_SYSTEM_TYPE_PART, true);
		}
		else{
			//SES(自機)
			importDotgainCurve(toneCurve, COM_SYSTEM_TYPE_SELF, true);
		}
	}
}

void CPrintConditionRegister::importDotgainCurve(const CPrintConditionForXML::ToneCurve& inToneCurve, eComSystemType inSystemType, bool inIsFront)
{
	if(inToneCurve.isExist(inIsFront) == false){
		//存在しない面については編集しない
		return;
	}
	//トーンカーブ名（GUI 表示）
	std::string dotgainName = inToneCurve.getDescriptiveName(inIsFront);
	_snprintf(m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_NAME, DEF_PDB_DOTGAIN_NAME_SIZE -1, "%s", dotgainName.c_str());

	//m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_SHAR_NAME は後で決める

	//アンカーポイント情報 4 色 x 5点分
	for(int color = ANCHOR_COL_BLACK; color < NumberOfColors; ++color){
		CPrintConditionForXML::ToneCurve::EColor toneCurveColor = CPrintConditionForXML::ToneCurve::kToneCurve_Color_C;
		switch(color){
		case ANCHOR_COL_BLACK:
			toneCurveColor = CPrintConditionForXML::ToneCurve::kToneCurve_Color_K;
			break;
		case ANCHOR_COL_YELLOW:
			toneCurveColor = CPrintConditionForXML::ToneCurve::kToneCurve_Color_Y;
			break;
		case ANCHOR_COL_MAGENTA:
			toneCurveColor = CPrintConditionForXML::ToneCurve::kToneCurve_Color_M;
			break;
		case ANCHOR_COL_CYAN:
			toneCurveColor = CPrintConditionForXML::ToneCurve::kToneCurve_Color_C;
			break;
		default:
			throw std::runtime_error("Error. Undefined kToneCurve_Color.");
		}
	
		for(int pointID = 0; pointID < ANCHOR_PT_CNT; ++pointID){
			long	beforeConPoint = 0;
			long	afterConPoint = 0;
			long	anchorPointEditType = 0;
			long	anchorPointType = 0;
			inToneCurve.getAnchorPoint(inIsFront, toneCurveColor, pointID, beforeConPoint, afterConPoint, anchorPointEditType, anchorPointType);

			//BeforeConPoint (変換なし)
			m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[color][pointID].POSITION.x = beforeConPoint;

			//AfterConPoint (変換なし)
			m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[color][pointID].POSITION.y = afterConPoint;

			//UIで編集したポイントか？
			m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[color][pointID].EDIT_TYPE = (anchorPointEditType == 0) ? FALSE : TRUE;

			//トーンカーブ作成に使用するかどうかはアンカーポイント種別で決まる
			m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[color][pointID].KIND = (anchorPointType == 0) ? FALSE : TRUE;
		}
	}
	//Allの値をC/M/Y/Kの値を決定する(本ファイルはトーンカーブファイル作成には使用しない)
	setToneColorAll(inSystemType);

	//トーンファイルが必要であれば作成、不要であれば既存のデータに上書きを行う
	createDotgainFile(inSystemType);

}

void CPrintConditionRegister::createDotgainFile(eComSystemType inSystemType)
{
	auto getColor = [](int inColorID)->CDotgainFileCreator::E_COLORTYPE
	{
		switch(inColorID){
		case ANCHOR_COL_BLACK:
			return CDotgainFileCreator::COLOER_K;
		case ANCHOR_COL_YELLOW:
			return CDotgainFileCreator::COLOER_Y;
		case ANCHOR_COL_MAGENTA:
			return CDotgainFileCreator::COLOER_M;
		case ANCHOR_COL_CYAN:
			return CDotgainFileCreator::COLOER_C;
		default:
			throw std::runtime_error("Undefined Dotgain Color.");
		}
	};
	auto getSameNameDotgainCurve = [&](ST_PDB_DOTGAIN_CURVE& ioDotgainCurve) ->bool
	{
		bool isExist = CPrintConditionRegister::API::IsSystemDotgainCurve(ioDotgainCurve);
		if(!isExist){
			return false;
		}
		int index = 0;
		ST_PDB_DOTGAIN_CURVE tmpDotgainCurve = {0};
		while(CPrintConditionRegister::API::GetSystemDotgainCurve(index++, tmpDotgainCurve)){
			if(strncmp(ioDotgainCurve.DOTGAIN_NAME, tmpDotgainCurve.DOTGAIN_NAME, DEF_PDB_DOTGAIN_NAME_SIZE) == 0){
				memcpy(&ioDotgainCurve, &tmpDotgainCurve, sizeof(ST_PDB_DOTGAIN_CURVE));
				return true;
			}
		}

		if(inSystemType == COM_SYSTEM_TYPE_PART){
			//相手機の場合は、自機(紙DBには未登録)と被る可能性があるので自機とも比較する
			if(strncmp(ioDotgainCurve.DOTGAIN_NAME, m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_SELF].DOTGAIN_NAME, DEF_PDB_DOTGAIN_NAME_SIZE) == 0){
				//一致している場合は、上書きする
				memcpy(&ioDotgainCurve, &m_DataForPDB.DotgainCurve[COM_SYSTEM_TYPE_SELF], sizeof(ST_PDB_DOTGAIN_CURVE));
				return true;
			}
		}
		return false;
	};

	API::EnterSystemSettingPaperDBCS();
	bool isGetSame = getSameNameDotgainCurve(m_DataForPDB.DotgainCurve[inSystemType]);
	API::LeaveSystemSettingPaperDBCS();
	if(isGetSame){
		//同一のトーンファイル名がある場合は、同一のトーンの値に上書きしてファイルを作らない
		//m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_SHAR_NAMEもはまる
		return;
	}

	CDotgainFileCreator dotgainFileCreator(m_DataSystemSetting);
	for(int colorID = ANCHOR_COL_BLACK; colorID < NumberOfColors; ++colorID){
		CDotgainFileCreator::S_DOTGAIN_INFO dotgainInfo;
		dotgainInfo.ColorType = getColor(colorID);
		memcpy(dotgainInfo.AnChorPoint, m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[colorID], sizeof(ST_PDB_ANCHOR_POINT) * ANCHOR_PT_CNT);
		std::string filePath;
		bool ret = dotgainFileCreator.Write(dotgainInfo, filePath);
		if(!ret){
			throw std::runtime_error("Error. Create Dotgain File");
		}
		//後でゴミを削除できるように作成したファイルはリストに登録しておく
		m_TempFileList.push_back(filePath);
	}
	//m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_SHAR_NAMEに作成したファイルの値を設定する
	std::string fileID = dotgainFileCreator.GetCommonName();
	_snprintf(m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_SHAR_NAME, DEF_PDB_DOTGAIN_SHAR_NAME_SIZE -1, "%s", fileID.c_str());

	std::stringstream msg;
	msg << "Create Dotgain File[Name:" << m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_NAME << ", FileID: " <<  fileID << "]";
	WriteToLogBuf(LOG_DEBUG, msg.str());
}

void CPrintConditionRegister::setToneColorAll(eComSystemType inSystemType)
{
	auto isDotgainCurveEachColorEqual = [&](ST_PDB_DOTGAIN_CURVE& inDotgainCurve) -> bool
	{
		bool isEqual = true;
		for(int colorIndex = ANCHOR_COL_BLACK; colorIndex < NumberOfColors -1; ++colorIndex) {
			int nAnchorPoint = colorIndex + 1;
			long res = memcmp(inDotgainCurve.ANCHOR_POINT[colorIndex], inDotgainCurve.ANCHOR_POINT[nAnchorPoint], sizeof(inDotgainCurve.ANCHOR_POINT[nAnchorPoint]));
			if(0 != res) {
				isEqual = false;
				break;
			}
		}
		return isEqual;
	};

	auto getLinearDotGainData = [&](ST_PDB_ANCHOR_POINT* outAnchor, size_t inSize)
	{
		#define DEF_ANCHOR_PT_UNIT			1000	//!< Unit of the anchor point
		#define MAX_DOTGAIN_CURVE_PT_X		255		//!< Maximum value of dot gain curve (X coordinate)
		#define MAX_DOTGAIN_CURVE_PT_Y		255		//!< Maximum value of dot gain curve (Y coordinate)

		if(!outAnchor || inSize < 5){
			return;
		}

		outAnchor[0].EDIT_TYPE = FALSE;
		outAnchor[0].KIND = TRUE;
		outAnchor[0].POSITION.x = 0;
		outAnchor[0].POSITION.y = 0;

		outAnchor[1].EDIT_TYPE = FALSE;
		outAnchor[1].KIND = FALSE;
		outAnchor[1].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 25) / 100;
		outAnchor[1].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 25) / 100;

		outAnchor[2].EDIT_TYPE = FALSE;
		outAnchor[2].KIND = FALSE;
		outAnchor[2].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 50) / 100;
		outAnchor[2].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 50) / 100;

		outAnchor[3].EDIT_TYPE = FALSE;
		outAnchor[3].KIND = FALSE;
		outAnchor[3].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 75) / 100;
		outAnchor[3].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 75) / 100;

		outAnchor[4].EDIT_TYPE = FALSE;
		outAnchor[4].KIND = TRUE;
		outAnchor[4].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 100) / 100;
		outAnchor[4].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 100) / 100;
	};

	if(isDotgainCurveEachColorEqual(m_DataForPDB.DotgainCurve[inSystemType])){
		//CMYKのアンカーポイントが一致していれば、Kの値をコピーする
		memcpy(m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[0], m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[1], sizeof(m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[1]));
	}else{
		//一致していないときはリニアを設定する
		getLinearDotGainData(m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[0], sizeof(m_DataForPDB.DotgainCurve[inSystemType].ANCHOR_POINT[0]));
	}
}

void CPrintConditionRegister::ImportJobBaseParameterWithPrintConditionName(
	const CValue<bool>* inJobBaseInformationSheet,
	const CValue<bool>* inJobBaseFlushingPage,
	const CValue<std::string>* inJobBasePatternName,
	const CValue<long>* inJobBaseInterval,
	const CValue<bool>* inJobBasePreJob,
	const CValue<long>* inJobBasePreJobCount,
	const CValue<bool>* inJobBasePostJob,
	const CValue<long>* inJobBaseOffsetFront,
	const CValue<long>* inJobBaseOffsetBack,
	const CValue<long>* inJobBaseCleaningJudgmentLevelBeforeJob,
	const CValue<long>* inJobBaseCleaningJudgmentLevelAfterJob,
	const CValue<std::string>* inJobBasePrintStopSetting
)
{
	auto isChangedLengthUnit = [&]()->bool
	{
		long tempLong = 0;
		if(inJobBaseOffsetFront->Get(tempLong) == true){
			return true;
		}
		if(inJobBaseOffsetBack->Get(tempLong) == true){
			return true;
		}
		return false;
	};


	ERRROR_IF_NULL(inJobBaseInformationSheet, "kJobBaseInformationSheet");
	ERRROR_IF_NULL(inJobBaseFlushingPage, "kJobBaseFlushingPage");
	ERRROR_IF_NULL(inJobBasePatternName, "kJobBasePatternName");
	ERRROR_IF_NULL(inJobBaseInterval, "kJobBaseInterval");
	ERRROR_IF_NULL(inJobBasePreJob, "kJobBasePreJob");
	ERRROR_IF_NULL(inJobBasePreJobCount, "kJobBasePreJobCount");
	ERRROR_IF_NULL(inJobBasePostJob, "kJobBasePostJob");
	ERRROR_IF_NULL(inJobBaseOffsetFront, "kJobBaseOffsetFront");
	ERRROR_IF_NULL(inJobBaseOffsetBack, "kJobBaseOffsetBack");
	ERRROR_IF_NULL(inJobBaseCleaningJudgmentLevelBeforeJob, "kJobBaseCleaningJudgmentLevelBeforeJob");
	ERRROR_IF_NULL(inJobBaseCleaningJudgmentLevelAfterJob, "kJobBaseCleaningJudgmentLevelAfterJob");
	ERRROR_IF_NULL(inJobBasePrintStopSetting, "kJobBasePrintStopSetting");

	DEF_LENGTH_UNIT srcLengthUnit = m_DataForPDB.LengthUnit_JobBase;
	if(isChangedLengthUnit() == true){
		m_DataForPDB.LengthUnit_JobBase = DEF_LENGTH_MM;
	}

	//インフォメーションシートを出力するかどうか
	bool informationSheet = false;
	if(inJobBaseInformationSheet->Get(informationSheet) == true)
	{
		m_DataForPDB.JobBaseFlushingPage.informationSheet = informationSheet;
	}

	//フラッシングページを挿入するかどうか
	bool flushingPageType = false;
	if(inJobBaseFlushingPage->Get(flushingPageType) == true)
	{
		m_DataForPDB.JobBaseFlushingPage.flushingPageType = ( flushingPageType ? DEF_FLUSHING_PAGE_USER : DEF_FLUSHING_PAGE_NONE);
	}

	//FlushingPage が true
	if(flushingPageType == true)
	{
		//フラッシングページのパターン名
		std::string flushingPageName = "";
		if(inJobBasePatternName->Get(flushingPageName) == true)
		{
			if(m_DataSystemSetting.isSystemFlushingPage(flushingPageName, m_DataForPDB.PrntResoX, m_DataForPDB.PrntResoY) == true)
			{
				_snprintf(m_DataForPDB.JobBaseFlushingPage.flushingPage, DEF_JOB_NAME_MAX, "%s", flushingPageName.c_str());
			}
			else if(m_isAppend)
			{	// 新規のときのみログ出力
				//　フラッシングページ名[%s]が未定義のため、印刷条件[%s]には印刷条件
				//[BasePrintCondition]のフラッシングページ設定を適用しました。
				std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_DEFINED_FLUSHPAGE, flushingPageName, m_PrintConditionName);
				WriteToLogBuf(LOG_STATUS, msg);
			}
		}

		//フラッシングページの挿入間隔
		long interval = 0;
		if(inJobBaseInterval->Get(interval) == true){
			m_DataForPDB.JobBaseFlushingPage.interval = interval;
		}
	}

	//ジョブ前に挿入するかどうか
	bool beforeJob = false;
	if(inJobBasePreJob->Get(beforeJob) == true)
	{
		m_DataForPDB.JobBaseFlushingPage.beforeJob = beforeJob;
	}

	// PreJob が true
	if(beforeJob == true)
	{
		//ジョブ前に挿入する回数
		long printCountOfBeforeJob = 0;
		if(inJobBasePreJobCount->Get(printCountOfBeforeJob) == true){
			m_DataForPDB.JobBaseFlushingPage.printCountOfBeforeJob = printCountOfBeforeJob;
		}
	}

	//ジョブ後に挿入するかどうか
	bool afterJob = false;
	if(inJobBasePostJob->Get(afterJob) == true)
	{
		m_DataForPDB.JobBaseFlushingPage.afterJob = afterJob;
	}

	// FlushingPage が true
	if(flushingPageType == true)
	{
		//フラッシングページのオフセット（表面・副走査）
		long jobBaseOffsetFront = 0;
		if(inJobBaseOffsetFront->Get(jobBaseOffsetFront) == true){
			m_DataForPDB.JobBaseFlushingPage.offset[0] = static_cast<double>(jobBaseOffsetFront) / 1000.0;
		}
		else{
			m_DataForPDB.JobBaseFlushingPage.offset[0] = ConvertUnit(m_DataForPDB.JobBaseFlushingPage.offset[0], srcLengthUnit, m_DataForPDB.LengthUnit_JobBase);
		}

		// かつ DED構成
		if(m_DataSystemSetting.IsDED() == true)
		{
			//フラッシングページのオフセット（裏面・副走査）
			long jobBaseOffsetBack = 0;
			if(inJobBaseOffsetBack->Get(jobBaseOffsetBack) == true){
				m_DataForPDB.JobBaseFlushingPage.offset[1] = static_cast<double>(jobBaseOffsetBack) / 1000.0;
			}
			else{
				m_DataForPDB.JobBaseFlushingPage.offset[1] = ConvertUnit(m_DataForPDB.JobBaseFlushingPage.offset[1], srcLengthUnit, m_DataForPDB.LengthUnit_JobBase);
			}
		}
	}

	// ジョブ前のクリーニング判定レベル
	long cleaningJudgmentLevelBeforeJob = 0;
	if(inJobBaseCleaningJudgmentLevelBeforeJob->Get(cleaningJudgmentLevelBeforeJob) == true)
	{
		m_DataForPDB.JobBaseCleaningSetting.CleaningJudgmentLevelBeforeJob = cleaningJudgmentLevelBeforeJob;
	}

	// ジョブ後のクリーニング判定レベル
	long cleaningJudgmentLevelAfterJob = 0;
	if(inJobBaseCleaningJudgmentLevelAfterJob->Get(cleaningJudgmentLevelAfterJob) == true)
	{
		m_DataForPDB.JobBaseCleaningSetting.CleaningJudgmentLevelAfterJob = cleaningJudgmentLevelAfterJob;
	}

	// 印刷停止設定
	std::string printStopSetting;
	if(inJobBasePrintStopSetting->Get(printStopSetting) == true)
	{
		if(printStopSetting == "Stop")
		{
			m_DataForPDB.JobBaseCleaningSetting.PrintStopSetting = kStopIfMissingNozzle;
		}
		else if(printStopSetting == "NotStop")
		{
			m_DataForPDB.JobBaseCleaningSetting.PrintStopSetting = kNotStopIfMissingNozzle;
		}
		else
		{
			std::stringstream err;
			err << "Error. No PrintStopSetting. " << printStopSetting;
			throw std::runtime_error(err.str());
		}		
	}
}

void CPrintConditionRegister::ImportLayoutImageParameterWithPrintConditionName(
	const CValue<CPrintConditionForXML::xypair_t>* inLayoutOffsetFront,
	const CValue<CPrintConditionForXML::xypair_t>* inLayoutOffsetBack,
	const CValue<bool>* inLayoutVerification,
	const CValue<long>* inLayoutContinuousPageCount,
	const CValue<bool>* inLayoutCueMark,
	const CValue<bool>* inLayoutBarcode,
	const CValue<CPrintConditionForXML::xypair_t>* inLayoutBarcodeOffsetFront,
	const CValue<CPrintConditionForXML::xypair_t>* inLayoutBarcodeOffsetBack,
	const CValue<std::string>* inLayoutFlushingPattern,
	const CValue<long>* inLayoutGapBetweenColors,
	const CValue<long>* inLayoutLineOffsetFront,
	const CValue<long>* inLayoutLineOffsetBack,
	const CValue<bool>* inLayoutLineGap,
	const CValue<long>* inLayoutLineGapFront,
	const CValue<long>* inLayoutLineGapBack,
	const CValue<long>* inLayoutLineGapOffsetFront,
	const CValue<long>* inLayoutLineGapOffsetBack,
	const CValue<std::string>* inLayoutLinePosotion,
	const CValue<std::string>* inLayoutLineOptions,
	const CValue<CPrintConditionForXML::ECutterMarkSide>* inLayoutCutterMark,
	const CValue<long>* inLayoutCutterMarkPageCount,
	const CValue<CPrintConditionForXML::xypair_t>* inLayoutCutterMarkSize,
	const CValue<long>* inLayoutCutterMarkOffsetFront,
	const CValue<long>* inLayoutCutterMarkOffsetBack,
	const CValue<CPrintConditionForXML::ECutterMarkAlign>* inLayoutCutterMarkAlignFront,
	const CValue<CPrintConditionForXML::ECutterMarkAlign>* inLayoutCutterMarkAlignBack,
	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	const CValue<bool>* inLayoutAdjustmentSheetSizeEnabled,
	const CValue<long>* inLayoutAdjustmentSheetLength,
	const CValue<long>* inLayoutAdjustmentSheetAfterJob,
	const CValue<long>* inLayoutAfterJobSheets
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
)
{
	auto getPatternProp = [&](const std::string& inStr, std::pair<DEF_FLUSHING_PATTERN, bool>& outProp) ->bool
	{
		typedef std::pair<DEF_FLUSHING_PATTERN, bool> PropPair;		// enumとラインフラッシングかどうか
		static std::map<std::string, PropPair> s_Map;
		if(s_Map.empty()){
			// HD設定可能な値の一覧
			s_Map["None"   ]		= std::make_pair(DEF_FLUSHING_PATTERN_NONE, false);
			s_Map["Line1"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_1, true);
			s_Map["Line2"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_2, true);
			s_Map["Line4"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_4, true);
			s_Map["Line8"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_8, true);
			s_Map["Line16" ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_16, true);
			s_Map["Line32" ]		= std::make_pair(DEF_FLUSHING_PATTERN_LINE_32, true);
			s_Map["Star1"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_STAR_1, false);
			s_Map["Star2"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_STAR_2, false);
			s_Map["Star4"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_STAR_4, false);
			s_Map["Star8"  ]		= std::make_pair(DEF_FLUSHING_PATTERN_STAR_8, false);
			s_Map["Line1+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_1_STAR, true);
			s_Map["Line2+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_2_STAR, true);
			s_Map["Line4+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_4_STAR, true);
			s_Map["Line8+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_8_STAR, true);
			s_Map["Line16+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_16_STAR, true);
			s_Map["Line32+Star"  ]	= std::make_pair(DEF_FLUSHING_PATTERN_LINE_32_STAR, true);
		}
		auto pattern = s_Map.find(inStr);
		if(pattern == s_Map.end()) {
			return false;
		}
		outProp = pattern->second;
		return true;
	};

	// フラッシングパターンのenum値
	auto getPatternType = [&](const std::string& inStr, DEF_FLUSHING_PATTERN& outPattern) ->bool
	{
		typedef std::pair<DEF_FLUSHING_PATTERN, bool> PropPair;		// enumとラインフラッシングかどうか
		PropPair prop;
		if(getPatternProp(inStr, prop) == true)
		{
			outPattern = prop.first;	// enumを返す
			return true;
		}
		else
		{
			return false;
		}
	};

	//ラインフラッシングかどうか
	auto isLinePattern = [&](const std::string& inStr) ->bool
	{
		typedef std::pair<DEF_FLUSHING_PATTERN, bool> PropPair;		// enumとラインフラッシングかどうか
		PropPair prop;
		if(getPatternProp(inStr, prop) == true)
		{
			return prop.second;		// ラインフラッシングかどうかを返す
		}
		else
		{
			return false;
		}
	};

	auto getLineOptions = [](const std::string& inStr, DEF_LINE_TYPE_OPTION& outOption, DEF_LINE_TYPE_OPTION2& outOption2) ->bool
	{
		typedef std::pair<DEF_LINE_TYPE_OPTION, DEF_LINE_TYPE_OPTION2> OptionsPair;
		static std::map<std::string, OptionsPair> s_Map;
		if(s_Map.empty()){
			// HD設定可能な値の一覧
			s_Map["C0-STANDARD"]				= std::make_pair(DEF_LINE_OPTION_C0, DEF_LINE_OPTION2_STANDARD);
			s_Map["C0-CROSS_STITCH"]			= std::make_pair(DEF_LINE_OPTION_C0, DEF_LINE_OPTION2_CROSS_STITCH);
			s_Map["NONE-STANDARD"]				= std::make_pair(DEF_LINE_OPTION_NONE, DEF_LINE_OPTION2_STANDARD);
			s_Map["NONE-CROSS_STITCH"]			= std::make_pair(DEF_LINE_OPTION_NONE, DEF_LINE_OPTION2_CROSS_STITCH);
			s_Map["S0-STANDARD"]				= std::make_pair(DEF_LINE_OPTION_S0, DEF_LINE_OPTION2_STANDARD);
			s_Map["S0-CROSS_STITCH"]			= std::make_pair(DEF_LINE_OPTION_S0, DEF_LINE_OPTION2_CROSS_STITCH);
			s_Map["S2-STANDARD"]				= std::make_pair(DEF_LINE_OPTION_S2, DEF_LINE_OPTION2_STANDARD);
			s_Map["S2-CROSS_STITCH"]			= std::make_pair(DEF_LINE_OPTION_S2, DEF_LINE_OPTION2_CROSS_STITCH);
		}
		auto typePattern = s_Map.find(inStr);
		if(typePattern == s_Map.end()) {
			return false;
		}
		outOption = typePattern->second.first;
		outOption2 = typePattern->second.second;
		return true;
	};

	ERRROR_IF_NULL(inLayoutOffsetFront, "kLayoutOffsetFront");
	ERRROR_IF_NULL(inLayoutOffsetBack, "kLayoutOffsetBack");
	ERRROR_IF_NULL(inLayoutVerification, "kLayoutVerification");
	ERRROR_IF_NULL(inLayoutContinuousPageCount, "kLayoutContinuousPageCount");
	ERRROR_IF_NULL(inLayoutCueMark, "kLayoutCueMark");
	ERRROR_IF_NULL(inLayoutBarcode, "kLayoutBarcode");
	ERRROR_IF_NULL(inLayoutBarcodeOffsetFront, "kLayoutBarcodeOffsetFront");
	ERRROR_IF_NULL(inLayoutBarcodeOffsetBack, "kLayoutBarcodeOffsetBack");
	ERRROR_IF_NULL(inLayoutFlushingPattern, "kLayoutFlushingPattern");
	ERRROR_IF_NULL(inLayoutGapBetweenColors, "kLayoutGapBetweenColors");
	ERRROR_IF_NULL(inLayoutLineOffsetFront, "kLayoutLineOffsetFront");
	ERRROR_IF_NULL(inLayoutLineOffsetBack, "kLayoutLineOffsetBack");
	ERRROR_IF_NULL(inLayoutLineGap,	"kLayoutLineGap");
	ERRROR_IF_NULL(inLayoutLineGapFront, "kLayoutLineGapFront");
	ERRROR_IF_NULL(inLayoutLineGapBack, "kLayoutLineGapBack");
	ERRROR_IF_NULL(inLayoutLineGapOffsetFront, "kLayoutLineGapOffsetFront");
	ERRROR_IF_NULL(inLayoutLineGapOffsetBack, "kLayoutLineGapOffsetBack");
	ERRROR_IF_NULL(inLayoutLinePosotion, "kLayoutLinePosotion");
	ERRROR_IF_NULL(inLayoutLineOptions, "kLayoutLineOptions");
	ERRROR_IF_NULL(inLayoutLineGapOffsetFront, "kLayoutLineGapOffsetFront");
	ERRROR_IF_NULL(inLayoutLineGapOffsetBack, "kLayoutLineGapOffsetBack");
	ERRROR_IF_NULL(inLayoutCutterMark, "kLayoutCutterMark");
	ERRROR_IF_NULL(inLayoutCutterMarkPageCount, "kLayoutCutterMarkPageCount");
	ERRROR_IF_NULL(inLayoutCutterMarkSize, "kLayoutCutterMarkSize");
	ERRROR_IF_NULL(inLayoutCutterMarkOffsetFront, "kLayoutCutterMarkOffsetFront");
	ERRROR_IF_NULL(inLayoutCutterMarkOffsetBack, "kLayoutCutterMarkOffsetBack");
	ERRROR_IF_NULL(inLayoutCutterMarkAlignFront, "kLayoutCutterMarkAlignFront");
	ERRROR_IF_NULL(inLayoutCutterMarkAlignBack, "kLayoutCutterMarkAlignBack");
	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	ERRROR_IF_NULL(inLayoutAdjustmentSheetSizeEnabled,"kLayoutAdjustmentSheetSizeEnabled");
	ERRROR_IF_NULL(inLayoutAdjustmentSheetAfterJob,"kLayoutAdjustmentSheetAfterJob");
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

	DEF_LENGTH_UNIT srcLengthUnit = m_DataForPDB.LengthUnit_Layout;
	m_DataForPDB.LengthUnit_Layout = DEF_LENGTH_MM;

	//画像のオフセット（表面・主副）
	CPrintConditionForXML::xypair_t layoutOffsetFront = std::make_pair(0, 0);
	if(inLayoutOffsetFront->Get(layoutOffsetFront) == true)
	{
		m_DataForPDB.LayoutImagePosition.offset[0].x = static_cast<double>(layoutOffsetFront.first) / 1000.0;
		m_DataForPDB.LayoutImagePosition.offset[0].y = static_cast<double>(layoutOffsetFront.second) / 1000.0;
	}

	//画像のオフセット（裏面・主副）
	if(m_DataSystemSetting.IsDED() == true)	// SES構成は取り込み対象外
	{
		CPrintConditionForXML::xypair_t layoutOffsetBack = std::make_pair(0, 0);
		if( inLayoutOffsetBack->Get(layoutOffsetBack) == true)
		{
			m_DataForPDB.LayoutImagePosition.offset[1].x = static_cast<double>(layoutOffsetBack.first) / 1000.0;
			m_DataForPDB.LayoutImagePosition.offset[1].y = static_cast<double>(layoutOffsetBack.second) / 1000.0;
		}
	}

	//サイド検査をするかどうか
	bool enableVerify = false;
	if(inLayoutVerification->Get(enableVerify) == true)
	{
		m_DataForPDB.LayoutVerifyMark.enableVerify = ( enableVerify ? TRUE : FALSE);
	}

	// Verification　が true
	if(enableVerify == true)
	{
		//エラーにする連続シート数
		long layoutContinuousPageCount = 0;
		if(inLayoutContinuousPageCount->Get(layoutContinuousPageCount) == true){
			m_DataForPDB.LayoutVerifyMark.errorSheets = layoutContinuousPageCount;
		}
	}

	//印刷開始マークを出力するかどうか
	bool printStartMark = false;
	if(inLayoutCueMark->Get(printStartMark) == true)
	{
		m_DataForPDB.LayoutVerifyMark.printStartMark = (printStartMark ? TRUE : FALSE);
	}

	//バーコードを出力するかどうか
	bool verifyMark = false;
	if(inLayoutBarcode->Get(verifyMark) == true)
	{
		m_DataForPDB.LayoutVerifyMark.verifyMark = ( verifyMark ? TRUE : FALSE);
	}

	// Barcode が true
	if(verifyMark == true)
	{
		//バーコードのオフセット（表面・主走査。副走査は0 固定）
		CPrintConditionForXML::xypair_t layoutBarcodeOffsetFront;
		if(inLayoutBarcodeOffsetFront->Get(layoutBarcodeOffsetFront) == true){
			m_DataForPDB.LayoutVerifyMark.offsetX[0] = static_cast<double>(layoutBarcodeOffsetFront.first) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutVerifyMark.offsetX[0] = ConvertUnit(m_DataForPDB.LayoutVerifyMark.offsetX[0], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		// かつ　DED構成
		if(m_DataSystemSetting.IsDED() == true)
		{
			//バーコードのオフセット（裏面・主副）
			CPrintConditionForXML::xypair_t layoutBarcodeOffsetBack;
			if(inLayoutBarcodeOffsetBack->Get(layoutBarcodeOffsetBack) == true){
				m_DataForPDB.LayoutVerifyMark.offsetX[1] = static_cast<double>(layoutBarcodeOffsetBack.first) / 1000.0;
				m_DataForPDB.LayoutVerifyMark.offsetY = static_cast<double>(layoutBarcodeOffsetBack.second) / 1000.0;
			}
			else{
				m_DataForPDB.LayoutVerifyMark.offsetX[1] = ConvertUnit(m_DataForPDB.LayoutVerifyMark.offsetX[1], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
				m_DataForPDB.LayoutVerifyMark.offsetY = ConvertUnit(m_DataForPDB.LayoutVerifyMark.offsetY, srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			}
		}
	}

	//フラッシングパターン
	std::string pattern = "";
	DEF_FLUSHING_PATTERN patternDef = DEF_FLUSHING_PATTERN_NONE;
	if(inLayoutFlushingPattern->Get(pattern) == true)
	{
		if(getPatternType(pattern, patternDef) == true)
		{
			m_DataForPDB.LayoutFlushingPattern.flushingPattern = patternDef;
		}
		else
		{
			//　このプリンターで未サポートのフラッシングパターン[%s]を設定しているた
			//め、印刷条件[%s]を取り込むことが出来ません。
			std::string msg = composeLogMsg(IDS_STAT_IMPORT_NOT_SUPPORTED_FLUSHPATTERN, pattern, m_PrintConditionName);
			throw LogSupressException(msg);
		}
	}

	// FlushingPattern が None 以外 
	if(patternDef != DEF_FLUSHING_PATTERN_NONE)
	{
		//ラインフラッシングのずらし量
		long layoutGapBetweenColors = 0;
		if(inLayoutGapBetweenColors->Get(layoutGapBetweenColors) == true){
			m_DataForPDB.LayoutFlushingPattern.gap = layoutGapBetweenColors;
		}

		//ラインフラッシングのオフセット（表面・副走査）
		long layoutLineOffsetFront = 0;
		if(inLayoutLineOffsetFront->Get(layoutLineOffsetFront) == true){
			m_DataForPDB.LayoutFlushingPattern.offsetY[0] = static_cast<double>(layoutLineOffsetFront) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutFlushingPattern.offsetY[0] = ConvertUnit(m_DataForPDB.LayoutFlushingPattern.offsetY[0], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		// SES構成は取り込み対象外
		if(m_DataSystemSetting.IsDED() == true)
		{
			//ラインフラッシングのオフセット（裏面・副走査）
			long layoutLineOffsetBack = 0;
			if(inLayoutLineOffsetBack->Get(layoutLineOffsetBack) == true){
				m_DataForPDB.LayoutFlushingPattern.offsetY[1] = static_cast<double>(layoutLineOffsetBack) / 1000.0;
			}
			else{
				m_DataForPDB.LayoutFlushingPattern.offsetY[1] = ConvertUnit(m_DataForPDB.LayoutFlushingPattern.offsetY[1], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			}
		}
	}
	
	// ラインフラッシング設定時のみ、設定
	if(isLinePattern(pattern))
	{
		//ページ内、ラインフラッシング
		bool layoutLineGap = false;
		if(inLayoutLineGap->Get(layoutLineGap) == true){
			m_DataForPDB.LayoutFlushingPatternAdd2.enableMultiLine = layoutLineGap;
		}

		//ページ内、ラインフラッシング間隔（表面・副走査）
		long layoutLineGapFront = 0;
		if(inLayoutLineGapFront->Get(layoutLineGapFront) == true){
			m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[0] = static_cast<double>(layoutLineGapFront) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[0] = ConvertUnit(m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[0], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		// SES構成は取り込み対象外
		if(m_DataSystemSetting.IsDED() == true)
		{
			//ページ内、ラインフラッシング間隔（裏面・副走査）
			long layoutLineGapBack = 0;
			if(inLayoutLineGapBack->Get(layoutLineGapBack) == true){
				m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[1] = static_cast<double>(layoutLineGapBack) / 1000.0;
			}
			else{
				m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[1] = ConvertUnit(m_DataForPDB.LayoutFlushingPatternAdd2.position2ndLine[1], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			}
		}

		//ページ内、ラインフラッシングの補正オフセット（表面・副走査）
		long layoutLineGapOffsetFront = 0;
		if(inLayoutLineGapOffsetFront->Get(layoutLineGapOffsetFront) == true){
			m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[0] = static_cast<double>(layoutLineGapOffsetFront) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[0] = ConvertUnit(m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[0], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		// SES構成は取り込み対象外
		if(m_DataSystemSetting.IsDED() == true)
		{
			//ページ内、ラインフラッシングの補正オフセット（裏面・副走査）
			long layoutLineGapOffsetBack = 0;
			if(inLayoutLineGapOffsetBack->Get(layoutLineGapOffsetBack) == true){
				m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[1] = static_cast<double>(layoutLineGapOffsetBack) / 1000.0;
			}
			else{
				m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[1] = ConvertUnit(m_DataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[1], srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			}
		}
	}

	//Bottom指定時、ラインフラッシングの下端配置および後処理マークの下端配置がON
	std::string linePosotion = "";
	if(inLayoutLinePosotion->Get(linePosotion) == true)
	{
		if(linePosotion == "Top")
		{
			m_DataForPDB.LayoutVertivalAlignmentPPFLSPTN = FALSE;	//Top:false
		}
		else if(linePosotion == "Bottom")
		{
			m_DataForPDB.LayoutVertivalAlignmentPPFLSPTN = TRUE;	//Bottom:true
		}
		else
		{
			throw std::runtime_error("Error. Unknown LinePosotion:" + linePosotion);
		}
	}

	//ラインフラッシングのオプション情報
	std::string lineOption = "";
	if(inLayoutLineOptions->Get(lineOption) == true)
	{
		DEF_LINE_TYPE_OPTION option = DEF_LINE_OPTION_C0;
		DEF_LINE_TYPE_OPTION2 option2 = DEF_LINE_OPTION2_STANDARD;
		if(getLineOptions(lineOption, option, option2) == true)
		{
			m_DataForPDB.LayoutFlushingPatternAdd.lineOption = option;
			m_DataForPDB.LayoutFlushingPatternAdd.lineOption2 = option2;
		}
	}

	//カッターマークの印刷面
	CPrintConditionForXML::ECutterMarkSide LayoutCutterMark = CPrintConditionForXML::kCutterMarkSide_Off;
	if(inLayoutCutterMark->Get(LayoutCutterMark) == true)
	{
		switch(LayoutCutterMark){
		case CPrintConditionForXML::kCutterMarkSide_Off:
			m_DataForPDB.LayoutPostProcess.side = DEF_SELECT_SIDE_NONE;
			break;
		case CPrintConditionForXML::kCutterMarkSide_Front:
			m_DataForPDB.LayoutPostProcess.side = DEF_SELECT_SIDE_FRONT;
			break;
		case CPrintConditionForXML::kCutterMarkSide_Back:
			m_DataForPDB.LayoutPostProcess.side = DEF_SELECT_SIDE_BACK;
			break;
		case CPrintConditionForXML::kCutterMarkSide_Both:
			m_DataForPDB.LayoutPostProcess.side = DEF_SELECT_SIDE_BOTH;
			break;
		default:
			std::stringstream err;
			err << "Error. No LayoutCutterMark. " << LayoutCutterMark;
			throw std::runtime_error(err.str());
		}
	}
	
	// CutterMark　が off 以外
	if(LayoutCutterMark != CPrintConditionForXML::kCutterMarkSide_Off)
	{
		//カッターマークの準備シート数
		long layoutCutterMarkPageCount = 0;
		if(inLayoutCutterMarkPageCount->Get(layoutCutterMarkPageCount) == true){
			m_DataForPDB.LayoutPostProcess.sheets = layoutCutterMarkPageCount;
		}

		//カッターマークのサイズ（主副）
		CPrintConditionForXML::xypair_t  layoutCutterMarkSize;
		if(inLayoutCutterMarkSize->Get(layoutCutterMarkSize) == true){
			m_DataForPDB.LayoutPostProcess.size.cx = static_cast<double>(layoutCutterMarkSize.first) / 1000.0;
			m_DataForPDB.LayoutPostProcess.size.cy = static_cast<double>(layoutCutterMarkSize.second) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutPostProcess.size.cx = ConvertUnit(m_DataForPDB.LayoutPostProcess.size.cx, srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			m_DataForPDB.LayoutPostProcess.size.cy = ConvertUnit(m_DataForPDB.LayoutPostProcess.size.cy, srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		//カッターマークのオフセット（表面・主走査）
		long layoutCutterMarkOffsetFront = 0;
		if(inLayoutCutterMarkOffsetFront->Get(layoutCutterMarkOffsetFront) == true){
			m_DataForPDB.LayoutPostProcess.offsetX = static_cast<double>(layoutCutterMarkOffsetFront) / 1000.0;
		}
		else{
			m_DataForPDB.LayoutPostProcess.offsetX = ConvertUnit(m_DataForPDB.LayoutPostProcess.offsetX, srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
		}

		// かつ DED構成
		if(m_DataSystemSetting.IsDED() == true)
		{
			//カッターマークのオフセット（裏面・主走査）
			long layoutCutterMarkOffsetBack = 0;
			if(inLayoutCutterMarkOffsetBack->Get(layoutCutterMarkOffsetBack) == true){
				m_DataForPDB.LayoutPostProcess.offsetX_back = static_cast<double>(layoutCutterMarkOffsetBack) / 1000.0;
			}
			else{
				m_DataForPDB.LayoutPostProcess.offsetX_back = ConvertUnit(m_DataForPDB.LayoutPostProcess.offsetX_back, srcLengthUnit, m_DataForPDB.LengthUnit_Layout);
			}
		}

		//カッターマークオフセットの基準位置（表面）
		CPrintConditionForXML::ECutterMarkAlign layoutCutterMarkAlignFront = CPrintConditionForXML::kCutterMarkAlign_Center;
		if(inLayoutCutterMarkAlignFront->Get(layoutCutterMarkAlignFront) == true){
			switch(layoutCutterMarkAlignFront){
			case CPrintConditionForXML::kCutterMarkAlign_Left:
				m_DataForPDB.LayoutPostProcess.alignment = DEF_POSTPROCESS_LEFT;
				break;
			case CPrintConditionForXML::kCutterMarkAlign_Center:
				m_DataForPDB.LayoutPostProcess.alignment = DEF_POSTPROCESS_NONE;
				break;
			case CPrintConditionForXML::kCutterMarkAlign_Right:
				m_DataForPDB.LayoutPostProcess.alignment = DEF_POSTPROCESS_RIGHT;
				break;
			default:
				std::stringstream err;
				err << "Error. No LayoutCutterMarkAlignFront. " << layoutCutterMarkAlignFront;
				throw std::runtime_error(err.str());
			}
		}

		// かつ DED構成
		if(m_DataSystemSetting.IsDED() == true)
		{
			//カッターマークオフセットの基準位置（裏面）
			CPrintConditionForXML::ECutterMarkAlign layoutCutterMarkAlignBack = CPrintConditionForXML::kCutterMarkAlign_Center;
			if(inLayoutCutterMarkAlignBack->Get(layoutCutterMarkAlignBack) == true){
				switch(layoutCutterMarkAlignBack){
				case CPrintConditionForXML::kCutterMarkAlign_Left:
					m_DataForPDB.LayoutPostProcess.alignment_back = DEF_POSTPROCESS_LEFT;
					break;
				case CPrintConditionForXML::kCutterMarkAlign_Center:
					m_DataForPDB.LayoutPostProcess.alignment_back = DEF_POSTPROCESS_NONE;
					break;
				case CPrintConditionForXML::kCutterMarkAlign_Right:
					m_DataForPDB.LayoutPostProcess.alignment_back = DEF_POSTPROCESS_RIGHT;
					break;
				default:
					std::stringstream err;
					err << "Error. No LayoutCutterMarkAlignBack. " << layoutCutterMarkAlignBack;
					throw std::runtime_error(err.str());
				}
			}
		}

		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		bool layoutAdjustmentSheetSizeEnabled = false;
		if(inLayoutAdjustmentSheetSizeEnabled->Get(layoutAdjustmentSheetSizeEnabled) == true){
			m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetSizeEnabled = layoutAdjustmentSheetSizeEnabled;
		}

		if ( m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetSizeEnabled ) {
			ERRROR_IF_NULL(inLayoutAdjustmentSheetLength,"kLayoutAdjustmentSheetLength");
			long layoutAdjustmentSheetLength = 0;
			if (inLayoutAdjustmentSheetLength->Get(layoutAdjustmentSheetLength) == true )
			{
				m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetLength = static_cast<double>(layoutAdjustmentSheetLength) / 1000.0;
			}
		}
		else {
			m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetLength = 0.0;
		}

		long layoutAdjustmentSheetAfterJob = 0;
		if(inLayoutAdjustmentSheetAfterJob->Get(layoutAdjustmentSheetAfterJob) == true) {
			m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetAfterJob = static_cast<ADJUSTMENT_SHEET_AFTER_JOB_OPTION>(layoutAdjustmentSheetAfterJob);
		}

		if ( m_DataForPDB.LayoutPostProcessAdd.adjustmentSheetAfterJob == SET_SHEET_COUNT ) {
			ERRROR_IF_NULL(inLayoutAfterJobSheets,"kLayoutAfterJobSheets");
			long layoutAfterJobSheets = 0;
			if(inLayoutAfterJobSheets->Get(layoutAfterJobSheets) == true) {
				m_DataForPDB.LayoutPostProcessAdd.afterJobSheets = layoutAfterJobSheets;
			}
		}
		else {
			m_DataForPDB.LayoutPostProcessAdd.afterJobSheets = 0;
		}
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	}
}

//検査パラメータを決定する
void CPrintConditionRegister::ImportInspection(
	const CValue<CPrintConditionForXML::Inspection>* inInspection,
	const CValue<std::string>* inJICondition,
	const CValue<std::string>* inTemporalMaskFile
)
{
	ERRROR_IF_NULL(inInspection, "kInspection");
	ERRROR_IF_NULL(inJICondition, "kInspectionJICondition");
	ERRROR_IF_NULL(inTemporalMaskFile, "kTemporalMaskFile");

	CPrintConditionForXML::Inspection inspection;
	if(inInspection->Get(inspection) != true)
	{
		return;
	}

	//PDB_SetJICondition 該当項目
	//検査条件名
	std::string jiCondition = "";
	if(inJICondition->Get(jiCondition) == true)
	{
		m_DataForPDB.JICondition = jiCondition;
	}	

	//PDB_SetTemporalMaskFile 該当項目
	//一時マスクファイルパス
	std::string temporalMaskFile = "";
	if(inTemporalMaskFile->Get(temporalMaskFile) == true)
	{
		m_DataForPDB.TemporalMaskFile = temporalMaskFile;
	}

	//JI Mainteを呼び出してXMLを出力する
	ImportTemporalMask(
		inspection,
		m_DataForPDB.TemporalMaskFile //パスが格納されて返る
	);

	//PDB_SetNonInspectionAreaTable 該当項目
	//PDB_SetNonInspectionAreaList 該当項目
	auto getNonInspectionArea = [&](bool inSide) ->void
	{
		// 自機/相手機
		int sideIndex = 
			m_DataSystemSetting.IsDED() ?
			(inSide ? 1 : 0) :		// DEDのとき相手機：自機
			0;						// SESのとき自機
		inspection.getNonInspectionAreaLeft(inSide, m_DataForPDB.NonInspectionLeft[sideIndex]);
		inspection.getNonInspectionAreaRight(inSide, m_DataForPDB.NonInspectionRight[sideIndex]);

		size_t index = 0;
		std::string iD = "";
		auto startPos = std::make_pair<long, long>(0,0);
		auto endPos = std::make_pair<long, long>(0,0);

		m_DataForPDB.NonInspectionArea[sideIndex].clear();
		while(inspection.getRectInfo(
			inSide,
			index++,
			iD,
			startPos,
			endPos))
		{
			STRUCT_NON_INSPECTION_AREA nonInspectionArea;
			
			nonInspectionArea.STR_AREA_NAME = iD.c_str()[0];
			nonInspectionArea.START_POS_X = startPos.first;
			nonInspectionArea.START_POS_Y = startPos.second;
			nonInspectionArea.END_POS_X = endPos.first;
			nonInspectionArea.END_POS_Y = endPos.second;
			m_DataForPDB.NonInspectionArea[sideIndex].push_back(nonInspectionArea);
		}
	};

	m_DataForPDB.NonInspectionArea.clear();
	m_DataForPDB.NonInspectionArea.resize(2);
	if(m_DataSystemSetting.IsDED() == true)
	{	//DED構成の場合、Element "InspectionArea"は表裏2個の情報を持つ
		getNonInspectionArea(s_front);
		getNonInspectionArea(s_back);
	}
	else
	{	//SES構成時は1個のみ
		getNonInspectionArea(s_front);
	}

	// JI Mainteを呼び出してXMLを出力する
	ImportVerificationParams(
		inspection,
		m_DataForPDB.JICondition		//条件名が変わることがある
	);

}

//デコードパラメータを決定する
void CPrintConditionRegister::ImportDecode(
	const CValue<CPrintConditionForXML::Decode>* inDecode,
	const CValue<long>* inErrorSheets,
	const CValue<std::string>* inUnit,
	const CValue<bool>* inIsEnabled)
{
	ERRROR_IF_NULL(inDecode, "kDecode");
	ERRROR_IF_NULL(inErrorSheets, "kErrorSheets");
	ERRROR_IF_NULL(inUnit, "kUnit");
	ERRROR_IF_NULL(inIsEnabled, "kDecodeIsEnabled");

	auto getUnitType = [&](const std::string& inStr, eUNIT_MODE& outUnitType) ->bool
	{
		static std::map<std::string, eUNIT_MODE> s_Map;
		if(s_Map.empty()){
			// NX設定可能な値の一覧
			s_Map["mm"   ] = UNIT_MODE_MM;
			s_Map["inch" ] = UNIT_MODE_INCH;
			s_Map["point"] = UNIT_MODE_POINT;
		}
		auto unitType = s_Map.find(inStr);
		if(unitType == s_Map.end()) {
			return false;
		}
		outUnitType = unitType->second;
		return true;
	};

	auto getLabelTypePattern = [](const std::string& inStr, DEF_DECODE_LABEL_TYPE& outType, UNI_DECODE_PATTERN& outPattern) ->bool
	{
		typedef std::pair<DEF_DECODE_LABEL_TYPE, long> TypePatternPair;
		static std::map<std::string, TypePatternPair> s_Map;
		if(s_Map.empty()){
			// NX設定可能な値の一覧
			s_Map["OCR_Number"]				= std::make_pair(DEF_DECODE_OCR, DEF_OCR_NUMBER);
			s_Map["OCR_Text"]				= std::make_pair(DEF_DECODE_OCR, DEF_OCR_TEXT);
			s_Map["OCR_PriorityNumber"]		= std::make_pair(DEF_DECODE_OCR, DEF_OCR_PRIORITY_NUMBER);
			s_Map["OCR_PriorityText"  ]		= std::make_pair(DEF_DECODE_OCR, DEF_OCR_PRIORITY_TEXT);
			s_Map["1D_CODE39"  ]			= std::make_pair(DEF_DECODE_1D_BARCODE, DEF_1D_CODE39 );
			s_Map["1D_CODE128"  ]			= std::make_pair(DEF_DECODE_1D_BARCODE, DEF_1D_CODE128);
			s_Map["1D_NW7"  ]				= std::make_pair(DEF_DECODE_1D_BARCODE, DEF_1D_NW7);
			s_Map["1D_CODE39CD"  ]			= std::make_pair(DEF_DECODE_1D_BARCODE, DEF_1D_CODE39);
			s_Map["2D_QrCode"  ]			= std::make_pair(DEF_DECODE_2D_BARCODE, DEF_2D_QR_CODE);
			s_Map["2D_DataMatrix"  ]		= std::make_pair(DEF_DECODE_2D_BARCODE, DEF_2D_DATA_MATRIX);
			s_Map["2D_Pdf417"  ]			= std::make_pair(DEF_DECODE_2D_BARCODE, DEF_2D_PDF417);
		}
		auto typePattern = s_Map.find(inStr);
		if(typePattern == s_Map.end()) {
			return false;
		}
		outType = typePattern->second.first;
		switch(outType)
		{
		case DEF_DECODE_OCR:
			outPattern.pat_OCR = static_cast<DEF_OCR_PATTERN>(typePattern->second.second);
			break;
		case DEF_DECODE_1D_BARCODE:
			outPattern.pat_1D = static_cast<DEF_1D_BARCODE_PATTERN>(typePattern->second.second);
			break;
		case DEF_DECODE_2D_BARCODE:
			outPattern.pat_2D = static_cast<DEF_2D_BARCODE_PATTERN>(typePattern->second.second);
			break;
		default:
			return false;		// ここはMapの定義ミス
		}
		return true;
	};
	auto getAngle = [&](const std::string& inStr, DEF_OCR_ANGLE& outAngle) ->bool
	{
		static std::map<std::string, DEF_OCR_ANGLE> s_Map;
		if(s_Map.empty()){
			// NX設定可能な値の一覧
			s_Map["0"  ]	= DEF_OCR_ANGLE_0;
			s_Map["90"  ]	= DEF_OCR_ANGLE_90;
			s_Map["180"  ]	= DEF_OCR_ANGLE_180;
			s_Map["270"  ]	= DEF_OCR_ANGLE_270;
		}
		auto typePattern = s_Map.find(inStr);
		if(typePattern == s_Map.end()) {
			return false;
		}
		outAngle = typePattern->second;
		return true;
	};

	//エラー停止連続シート数
	long errorSheets = 0;
	if(inErrorSheets->Get(errorSheets) == true)
	{
		m_DataForPDB.Decode.errorSheets = errorSheets;
	}	

	//数値単位系
	std::string unit = "";
	eUNIT_MODE unitMode = UNIT_MODE_DEFAULT;
	if(inUnit->Get(unit) == true)
	{
		if(getUnitType(unit, unitMode) != true)
		{
			throw std::runtime_error("Error. unknown Decode::Unit:" + unit);
		}
	}
	
	//デコード処理の有効／無効
	bool isEnabled = false;
	if(inIsEnabled->Get(isEnabled) == true)
	{
		m_DataForPDB.Decode.enableDecode = isEnabled;
	}	

	//Decode詳細
	CPrintConditionForXML::Decode decode;
	if(inDecode->Get(decode) == true){
		std::string					side;
		std::pair<double, double>	size;
		std::pair<double, double>	offset;
		std::string					name;
		std::string					type;
		std::string					angle;
		bool						errorStop;

		m_DataForPDB.Decode.decodeDetailF.clear();
		m_DataForPDB.Decode.decodeDetailB.clear();

		size_t num = decode.getDetailNum();
		for(size_t i = 0; i < num; i++)
		{
			if(decode.getDetail(
				i,
				side,
				size,
				offset,
				name,
				type,
				angle,
				errorStop
				) == false)
			{
				continue;
			}
			ST_JOB_DECODE_DETAIL detailST = {0};
			//SizeとOffsetは「デコード情報の単位系」->mmに変換
			detailST.size.cx = ConvertUnit(size.first, unitMode, UNIT_MODE_MM);
			detailST.size.cy = ConvertUnit(size.second, unitMode, UNIT_MODE_MM);
			detailST.offset.x = ConvertUnit(offset.first, unitMode, UNIT_MODE_MM);
			detailST.offset.y = ConvertUnit(offset.second, unitMode, UNIT_MODE_MM);
			_snprintf(detailST.name, sizeof(detailST.name), "%s", name.c_str());
			if(!type.empty())
			{
				DEF_DECODE_LABEL_TYPE labelType;
				UNI_DECODE_PATTERN labelPattern;
				if(getLabelTypePattern(type, labelType, labelPattern) == true)
				{
					detailST.labelType = labelType;
					detailST.pattern = labelPattern;
				}
				else
				{
					throw std::runtime_error("Error. unknown DecodeDetails::Type:" + type);
				}
			}
			if(!angle.empty())
			{
				DEF_OCR_ANGLE angleDef = DEF_OCR_ANGLE_0;
				if(getAngle(angle, angleDef) == true)
				{
					detailST.ocrAngle = angleDef;
				}
				else
				{
					throw std::runtime_error("Error. unknown DecodeDetails::Angle:" + angle);
				}
			}
			detailST.checkError = errorStop ? TRUE : FALSE;


			if(!side.empty())
			{
				if(side == "Front")
				{
					m_DataForPDB.Decode.decodeDetailF.push_back(detailST);
				}
				else if(side == "Back")
				{	
					if(m_DataSystemSetting.IsDED() == true)
					{	//SES構成時、Side="Back"のElement"DecodeDetails"は無視する
						m_DataForPDB.Decode.decodeDetailB.push_back(detailST);
					}
				}
				else
				{
					throw std::runtime_error("Error. unknown DecodeDetails::Side:" + side);
				}
			}
		}// for

		//・ST_JOB_DECODEの下記のパラメータも詳細に合わせて更新
		//デコード領域数(表/裏)
		//デコード領域の総数
		//デコード領域の最大数
		//総Decodeエリア数(削除済み含む)
		m_DataForPDB.Decode.totalNumberOfDecode = m_DataForPDB.Decode.decodeDetailF.size() + m_DataForPDB.Decode.decodeDetailB.size();
		//総Decodeエリア数
		m_DataForPDB.Decode.numberOfDecodeMax   = m_DataForPDB.Decode.decodeDetailF.size() + m_DataForPDB.Decode.decodeDetailB.size();
		//Decodeエリア数(Front)
		m_DataForPDB.Decode.numberOfDecode[0]   = m_DataForPDB.Decode.decodeDetailF.size();
		//Decodeエリア数(Back)
		m_DataForPDB.Decode.numberOfDecode[1]   = m_DataForPDB.Decode.decodeDetailB.size();

		//以下のメンバは設定していない
		//m_DataForPDB.Decode.settingName[DEF_DECODE_SETTING_NAME_MAX + 1]
	}

}

//検査条件詳細情報をインポートする
void CPrintConditionRegister::ImportVerificationParams(
	const CPrintConditionForXML::Inspection& inInspection,
	std::string& ioJiCondition
)
{
	auto convBool2Bit = [](std::string inStr) ->std::string
	{
		static std::map<std::string, std::string> s_Map;
		if (s_Map.empty()) {
			s_Map.insert(std::pair<std::string, std::string>("true", "1"));
			s_Map.insert(std::pair<std::string, std::string>("false", "0"));
		}
		auto it = s_Map.find(inStr);
		if(it == s_Map.end())
		{
			return "";
		}
		return it->second;
	};

	auto getVerificationParams = [&](bool inSide, STRUCT_VERIFICATION_PARAMS& outVerificationParams) ->void
	{
		std::map<std::string, std::pair<bool, std::string> > verificationParams;
		inInspection.getVerificationParams(inSide, verificationParams);

		outVerificationParams.AutoPrintStop				  = verificationParams["scr:AutoPrintStop"].second;
		outVerificationParams.AutoStopDefectCountOrange	  = verificationParams["scr:AutoStopDefectCountOrange"].second;
		outVerificationParams.AutoStopDefectCountRed	  = verificationParams["scr:AutoStopDefectCountRed"].second;
		outVerificationParams.DefectAppearanceRatioOrange = verificationParams["scr:DefectAppearanceRatioOrange"].second;
		outVerificationParams.DefectAppearanceRatioRed	  = verificationParams["scr:DefectAppearanceRatioRed"].second;
		outVerificationParams.DefectDetectionSensitivity  = verificationParams["scr:DefectDetectionSensitivity"].second;
		outVerificationParams.DetectionLevelHeight		  = verificationParams["scr:DetectionLevelHeight"].second;
		outVerificationParams.DetectionLevelWidth		  = verificationParams["scr:DetectionLevelWidth"].second;
		outVerificationParams.DetectionMaskWidth		  = verificationParams["scr:DetectionMaskWidth"].second;
		outVerificationParams.DirtStreakBackImageRemoval  = verificationParams["scr:DirtStreakBackImageRemoval"].second;
		outVerificationParams.DirtStreakFilter			  = verificationParams["scr:DirtStreakFilter"].second;
		outVerificationParams.DirtStreakMaximumWidth	  = verificationParams["scr:DirtStreakMaximumWidth"].second;
		outVerificationParams.DirtStreakMinimumWidth	  = verificationParams["scr:DirtStreakMinimumWidth"].second;
		outVerificationParams.DirtStreakSigma			  = verificationParams["scr:DirtStreakSigma"].second;
		outVerificationParams.DirtStreakValue			  = verificationParams["scr:DirtStreakValue"].second;
		outVerificationParams.IsDirtStreakDetection		  = convBool2Bit(verificationParams["scr:IsDirtStreakDetection"].second);	// "true"/"false"→"1"/"0"変換
		outVerificationParams.PrePrintMaskWidth			  = verificationParams["scr:PrePrintMaskWidth"].second;
		outVerificationParams.StainDetectionSensitivity	  = verificationParams["scr:StainDetectionSensitivity"].second;
		outVerificationParams.StreakDetectionLevelStep	  = verificationParams["scr:StreakDetectionLevelStep"].second;
		outVerificationParams.StreakDetectionLevelWidth	  = verificationParams["scr:StreakDetectionLevelWidth"].second;
		outVerificationParams.StreakDetectionSensitivity  = verificationParams["scr:StreakDetectionSensitivity"].second;
	};

	auto getAIAssitParams = [&](bool inSide, STRUCT_AIASSIST_PARAMS& outAIAssistParams) ->bool
	{
		std::map<std::string, std::pair<bool, std::string> > aiassistParams;
		inInspection.getAIAssistParams(inSide, aiassistParams);

		if( aiassistParams.empty() )	return	false;

		outAIAssistParams.AssistRange					 = aiassistParams["scr:AssistRange"].second;
		outAIAssistParams.DefectDetectionSensitivity	 = aiassistParams["scr:DefectDetectionSensitivity"].second;
		outAIAssistParams.DetectionLevelHeight			 = aiassistParams["scr:DetectionLevelHeight"].second;
		outAIAssistParams.DetectionLevelWidth			 = aiassistParams["scr:DetectionLevelWidth"].second;
		outAIAssistParams.DirtStreakValue				 = aiassistParams["scr:DirtStreakValue"].second;
		outAIAssistParams.EnableAIAssist				 = convBool2Bit(aiassistParams["scr:EnableAIAssist"].second);
		outAIAssistParams.EnableAIFiltering				 = convBool2Bit(aiassistParams["scr:EnableAIFiltering"].second);
		outAIAssistParams.EnableAutoSetting				 = convBool2Bit(aiassistParams["scr:EnableAutoSetting"].second);
		outAIAssistParams.NotificationLevel				 = aiassistParams["scr:NotificationLevel"].second;
		outAIAssistParams.StainDetectionSensitivity		 = aiassistParams["scr:StainDetectionSensitivity"].second;
		outAIAssistParams.StreakDetectionSensitivity	 = aiassistParams["scr:StreakDetectionSensitivity"].second;
		
		return	true;
	};

	auto checkApiResult = [&](const long inRet, const std::string& jiConditionOld, const std::string& inJiCondition) ->void
	{
		std::string msg;
		switch(inRet)
		{
		case  0:	// 0:既存検査条件と設定内容同一のため既存ファイルを流用しました。
		case  1:	// 1:検査条件の追加に成功しました。
			break;
		case  2:	// 2:指定名で内容の異なる検査条件が存在するため､条件名を変更して保存しました。
			//　検査条件名[%s]が既に存在するため、[%s]に名前を変更して印刷条件[%s]に適
			//用しました。
			msg = composeLogMsg(IDS_STAT_IMPORT_EXIST_INSPECTSETTING, jiConditionOld, inJiCondition, m_PrintConditionName);
			WriteToLogBuf(LOG_STATUS, msg);
			break;
		case -1:	//-1:検査条件の追加に失敗しました。
		case -2:	//-2:指定名で内容の異なる検査条件が保存できませんでした。
		// 保存失敗はインポート1件失敗とする
			throw std::runtime_error("Error. MainteJetInspection_SaveInspectionParameter failed: " 
				+ inJiCondition 
				+ " " 
				+ std::to_string(static_cast<long long>(inRet)));
		}
	};
	
	std::string jiConditionOld = ioJiCondition;
	long ret = 0;
	if(m_DataSystemSetting.IsDED() == true)
	{	//DED構成の場合、Element "scr:VerificationParams"は表裏2個の情報を持つ
		STRUCT_VERIFICATION_PARAMS verificationParamsFront;
		STRUCT_VERIFICATION_PARAMS verificationParamsBack;
		getVerificationParams(s_front, verificationParamsFront);
		getVerificationParams(s_back, verificationParamsBack);
		
		STRUCT_AIASSIST_PARAMS aiassistParamsFront;
		STRUCT_AIASSIST_PARAMS aiassistParamsBack;
		STRUCT_AIASSIST_PARAMS* p_aiassistParamsFront;
		STRUCT_AIASSIST_PARAMS* p_aiassistParamsBack;
		bool chk_aiassistParamsFront;
		bool chk_aiassistParamsBack;

		chk_aiassistParamsFront = getAIAssitParams(s_front, aiassistParamsFront);
		chk_aiassistParamsBack  = getAIAssitParams(s_back, aiassistParamsBack);
		p_aiassistParamsFront = chk_aiassistParamsFront ? &aiassistParamsFront : NULL;
		p_aiassistParamsBack = chk_aiassistParamsBack ? &aiassistParamsBack : NULL;

		ret = CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveInspectionAIAssistParameter(ioJiCondition, verificationParamsFront, verificationParamsBack, p_aiassistParamsFront, p_aiassistParamsBack);
		checkApiResult(ret, jiConditionOld, ioJiCondition);
	}
	else
	{	//SES構成時は1個のみ
		STRUCT_VERIFICATION_PARAMS verificationParamsFront;
		STRUCT_VERIFICATION_PARAMS verificationParamsDummy;		// SESの場合はbackはないため、ダミーとしている
		getVerificationParams(s_front, verificationParamsFront);

		STRUCT_AIASSIST_PARAMS aiassistParamsFront;
		STRUCT_AIASSIST_PARAMS* p_aiassistParamsFront;
		bool chk_aiassistParamsFront;

		chk_aiassistParamsFront = getAIAssitParams(s_front, aiassistParamsFront);
		p_aiassistParamsFront = chk_aiassistParamsFront ? &aiassistParamsFront : NULL;

		ret = CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveInspectionAIAssistParameter(ioJiCondition, verificationParamsFront, verificationParamsDummy, p_aiassistParamsFront, NULL);
		checkApiResult(ret, jiConditionOld, ioJiCondition);
	}
}

void CPrintConditionRegister::ImportTemporalMask(
	const CPrintConditionForXML::Inspection& inInspection,
	std::string& ioTemporalMaskRelPath
)
{
	auto getFlawKind = [&](const std::string& inStr, ENUM_FLAW_KIND& outFlawKind) ->bool
	{
		static std::map<std::string, ENUM_FLAW_KIND> s_Map;
		if(s_Map.empty()){
			// NX設定可能な値の一覧
			s_Map["Blot"]				= EN_DIRT;
			s_Map["Lack"]				= EN_LOST;
			s_Map["White Streak"]		= EN_WHITE_LINE;
			s_Map["Streak Defect"  ]	= EN_STREAK_DEFECT;
		}
		auto flawKind = s_Map.find(inStr);
		if(flawKind == s_Map.end()) {
			return false;
		}
		outFlawKind = flawKind->second;
		return true;
	};

	auto getTemporalMask = [&](bool inSide, std::vector<STRUCT_JI_FLAW_INFO>& outJIFlawInfoList) ->void
	{
		int sideIndex = inSide ? 0 : 1;

		size_t index = 0;
		std::string type;
		std::pair<double, double> pos;
		std::pair<double, double> size;
		std::pair<double, double> temporalMaskSize;

		outJIFlawInfoList.clear();
		while(inInspection.getTemporalMaskInfo(
			inSide,
			index++,
			type,
			pos,
			size,
			temporalMaskSize))
		{
			STRUCT_JI_FLAW_INFO jIFlawInfo;

			ENUM_FLAW_KIND flawKind = EN_MAX_SIZE;
			if(getFlawKind(type, flawKind) != true)
			{
				throw std::runtime_error("Error. unknown scr:TemporalMask::Type:" + type);
			}
			jIFlawInfo.EN_FLAW_KIND			= flawKind;		
			jIFlawInfo.COORDINATE_X			= pos.first;
			jIFlawInfo.COORDINATE_Y			= pos.second;		
			jIFlawInfo.WIDTH				= size.first;		
			jIFlawInfo.HEIGHT				= size.second;	
			jIFlawInfo.TEMPORAY_MASK_WIDTH	= temporalMaskSize.first;
			jIFlawInfo.TEMPORAY_MASK_HEIGHT	= temporalMaskSize.second;
			outJIFlawInfoList.push_back(jIFlawInfo);			
		}
	};

	std::vector<STRUCT_JI_FLAW_INFO> jIFlawInfoFront;
	std::vector<STRUCT_JI_FLAW_INFO> jIFlawInfoBack;
	BOOL ret = FALSE;
	if(m_DataSystemSetting.IsDED() == true)
	{	//DED構成の場合、Element "scr:TemporalMask"は表裏2個の情報を持つ
		getTemporalMask(s_front, jIFlawInfoFront);
		getTemporalMask(s_back, jIFlawInfoBack);
		ret = CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveJIFlawInfoListToTempMaskPath(ioTemporalMaskRelPath, jIFlawInfoFront, jIFlawInfoBack);
		if(ret == FALSE)
		{	// API呼び出し失敗はインポート1件失敗とする
			throw std::runtime_error("Error. SaveJIFlawInfoListToTempMaskPath failed. temporalMaskRelPath:" + ioTemporalMaskRelPath);
		}
	}
	else
	{	//SES構成時は1個のみ
		getTemporalMask(s_front, jIFlawInfoFront);
		std::vector<STRUCT_JI_FLAW_INFO> jIFlawInfoDummy;		// SESの場合はbackはないため、ダミーとしている

		ret = CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveJIFlawInfoListToTempMaskPath(ioTemporalMaskRelPath, jIFlawInfoFront, jIFlawInfoDummy);
		if(ret == FALSE)
		{	// API呼び出し失敗はインポート1件失敗とする
			throw std::runtime_error("Error. SaveJIFlawInfoListToTempMaskPath failed.(SES) temporalMaskRelPath:" + ioTemporalMaskRelPath);
		}
	}
}

//検査パラメータエレメントが存在するかどうか
std::pair<bool,bool> CPrintConditionRegister::hasJiElements(
			const CPrintConditionForXML& inXML
)
{
	CPrintConditionForXML::Inspection inspection;
	CPrintConditionForXML::Decode decode;

	std::pair<bool,bool> ret =
		std::make_pair<bool,bool>(
			inXML.GetInspection(CPrintConditionForXML::kInspection)->Get(inspection),
			inXML.GetDecode(CPrintConditionForXML::kDecode)->Get(decode));

	return ret;
}

//パンチフラグが有効かどうか
bool CPrintConditionRegister::getPunchPaperFlag(
			const CPrintConditionForXML& inXML
)
{
	const CValue<bool>* inPaperPunchHole = inXML.GetBool(CPrintConditionForXML::kPaperPunchHole);
	const CValue<bool>* inPaperSewing = inXML.GetBool(CPrintConditionForXML::kPaperSewing);
		
	//パンチ穴用紙
	bool isHoles = 
		inPaperPunchHole->Get() && 
		!(inPaperPunchHole->Get() && inPaperSewing->Get());

	//ミシン目用紙
	bool isHolesPerforations = inPaperPunchHole->Get() && inPaperSewing->Get();

	return isHoles ||isHolesPerforations;

}

void CPrintConditionRegister::addSystemItem()
{
	//システムのトーンカーブ名一覧に未登録の場合はそちらにも登録する
	auto addSystemDotgainCurve = [&](eComSystemType inSystemType)->void
	{
		std::string dotgainName = m_DataForPDB.DotgainCurve[inSystemType].DOTGAIN_NAME;
		if(dotgainName.empty() == true){
			return;
		}

		if(CPrintConditionRegister::API::IsSystemDotgainCurve(m_DataForPDB.DotgainCurve[inSystemType]) == false){
			CPrintConditionRegister::API::AddSystemDotgainCurve(m_DataForPDB.DotgainCurve[inSystemType]);
		}
	};

	//システムのヒートローラー乾燥温度一覧に未登録の場合はそちらにも登録する
	{
		if(API::IsSystemDryTempPrint(m_DataForPDB.DryTempPrint) == false){
			API::AddSystemDryTempPrint(m_DataForPDB.DryTempPrint);
		}
	}

	//システムのトーンカーブ名一覧に未登録の場合はそちらにも登録する
	{
		if(m_DataSystemSetting.IsDED() == true){
			//DED(自機)
			addSystemDotgainCurve(COM_SYSTEM_TYPE_SELF);
			//DED(相手機)
			addSystemDotgainCurve(COM_SYSTEM_TYPE_PART);
		}
		else{
			//SES(自機)
			addSystemDotgainCurve(COM_SYSTEM_TYPE_SELF);
		}
	}
}

bool CPrintConditionRegister::CommitToPaperDB()
{
	//紙DB I/F 関数コール
	if(API::CommitPaperDB() == false){
		WriteToLogBuf(LOG_DEBUG, "CPrintConditionRegister::CommitToPaperDB() Error. Cannot commit.");
		throw 1;
	}

	return true;
}

void CPrintConditionRegister::debugout(E_DEBUGOUT_DB inDebugoutDB, bool inIsAppend)
{
	if(m_DataSystemSetting.IsDebugLog() == false){
		//ログ出力しない場合、出さない
		return;
	}

	auto getPrintConditionName = [&](E_DEBUGOUT_DB inDebugoutDB)->std::string
	{
		if((inDebugoutDB == E_DEBUGOUT_DB_BASE) && (inIsAppend == true)){
			return m_DataSystemSetting.GetBasePrintConditionName();
		}
		return m_PrintConditionName;
	};

	//印刷条件名取得
	const std::string printConditionName = getPrintConditionName(inDebugoutDB);


	auto getDebugoutFile = [&](E_DEBUGOUT_DB inDebugoutDB, const std::string& inPrintConditionName)->std::string
	{
		std::stringstream debugoutFile;

		SYSTEMTIME systemTime = {0};
		GetLocalTime(&systemTime);

		if(m_DebugoutFile.str().empty() == true){
			m_DebugoutFile << GetModuleDirectory() << "log\\PrintConditionImporter\\";
			m_DebugoutFile << std::setfill('0') << std::setw(4) << systemTime.wYear;
			m_DebugoutFile << std::setfill('0') << std::setw(2) << systemTime.wMonth;
			m_DebugoutFile << std::setfill('0') << std::setw(2) << systemTime.wDay;
			m_DebugoutFile << std::setfill('0') << std::setw(2) << systemTime.wHour;
			m_DebugoutFile << std::setfill('0') << std::setw(2) << systemTime.wMinute;
			m_DebugoutFile << std::setfill('0') << std::setw(2) << systemTime.wSecond;
		}

		debugoutFile << m_DebugoutFile.str();
		debugoutFile << (inDebugoutDB == CPrintConditionRegister::E_DEBUGOUT_DB_BASE? "_ImportBaseDB_" : "_ImportUpdateDB_");
		debugoutFile << inPrintConditionName;
		debugoutFile << ".txt";

		return debugoutFile.str();
	};

	//出力先ファイル名を生成
	const std::string debugoutFile = getDebugoutFile(inDebugoutDB, printConditionName);


	auto getLastErrorString = [](DWORD inErrID)->const std::string
	{
		LPVOID errBuf = GetLastErrorString(inErrID);
		if(errBuf == nullptr){
			return "";
		}

		const std::string errStr = static_cast<LPTSTR>(errBuf);
		FreeLastErrorString(errBuf);
		return errStr;
	};

	auto createFile = [&debugoutFile, &getLastErrorString]()->HANDLE
	{
		size_t fileStartPos = debugoutFile.find_last_of("\\");
		if (fileStartPos == std::string::npos) {
			return INVALID_HANDLE_VALUE;
		}
		CreateDirectory(debugoutFile.substr(0, fileStartPos + 1).c_str(), NULL);

		HANDLE fileHandle = CreateFile(debugoutFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if(fileHandle == INVALID_HANDLE_VALUE){
			const std::string lastErr = getLastErrorString(GetLastError());
			std::stringstream log;
			log << " : Cannot create file\npath:" << debugoutFile << "\n" << lastErr;
			throw std::runtime_error(log.str());
		}

		return fileHandle;
	};

	auto closeFile = [&debugoutFile, &getLastErrorString](HANDLE inFileHandle)->void
	{
		if(inFileHandle == INVALID_HANDLE_VALUE){
			return;
		}

		if(CloseHandle(inFileHandle) == 0){
			const std::string lastErr = getLastErrorString(GetLastError());
			std::stringstream log;
			log << "CPrintConditionRegister::debugout() : Cannot close file.\npath:" << debugoutFile << "\n" << lastErr;
			WriteToLogBuf(LOG_DEBUG, log.str());
		}
	};

	auto writeFile = [&debugoutFile, &getLastErrorString](HANDLE inFileHandle, const std::string& inString)->void
	{
		if(inFileHandle == INVALID_HANDLE_VALUE){
			return;
		}

		DWORD writeSize = 0;
		if(WriteFile(inFileHandle, inString.c_str(), inString.size(), &writeSize, NULL) == 0){
			const std::string lastErr = getLastErrorString(GetLastError());
			std::stringstream log;
			log << " : Cannot write file.\npath:" << debugoutFile << "\n" << lastErr;
			throw std::runtime_error(log.str());
		}

		if(inString.size() > writeSize){
			std::stringstream log;
			log << " : Cannot write file,\npath:" << debugoutFile << "\nwrite size = " << writeSize;
			throw std::runtime_error(log.str());
		}
	};

	//stringstream使いたいので定義
	#define createFile(outFileHandle) { outFileHandle = createFile(); }
	#define writeFile(inFileHandle, inString) { std::stringstream str; str << std::fixed << std::setprecision( std::numeric_limits<double>::max_digits10) << inString; writeFile(inFileHandle, str.str()); }

	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	try {
		//ファイル出力処理
		createFile(fileHandle);
	
		ST_PDB_MIGRATION_DATA dataForPDB;			//!< 紙DBとのやり取り用のデータ(初期値取得/印刷条件設定)
		if(API::GetAllParameter(printConditionName, UserModeGUI_GetLengthUnit(), DEF_MODE_RESO_SPEED_M, dataForPDB) == false){
			writeFile(fileHandle, "Cannot get " << printConditionName);
		}
		else{
			writeFile(fileHandle, "印刷条件名(◎)\t" << printConditionName << std::endl);
			writeFile(fileHandle, "用紙種類(◎)\t" << dataForPDB.PaperType << std::endl);
			writeFile(fileHandle, "段差補正名(○※)\t" << dataForPDB.LevelTuneName  << std::endl);
			writeFile(fileHandle, "用紙幅(◎)\t" << dataForPDB.PaperSizeW << std::endl);
			writeFile(fileHandle, "用紙厚み(◎)\t" << dataForPDB.Thickness << std::endl);
			writeFile(fileHandle, "テンション(◎)\t" << dataForPDB.Tension << std::endl);
			writeFile(fileHandle, "DNS(◎)\t" << dataForPDB.DNSStatus << std::endl);
	
			writeFile(fileHandle, "パンチ穴用紙(◎)\t" << (dataForPDB.PaperOption.IS_HOLES==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "ミシン目用紙(◎)\t" << (dataForPDB.PaperOption.IS_HOLES_PERFORATIONS==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "事前マーク(◎)\t" << (dataForPDB.PaperOption.IS_PRE_MARK==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "事前印刷(◎)\t" << (dataForPDB.PaperOption.IS_PRE_MARK_PAPER==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "マークオフセット（主走査）(△※)\t" << dataForPDB.PaperOption.PRE_MARK_OFFSET << std::endl);
			writeFile(fileHandle, "ページ送りサイズ(◎)\t" << dataForPDB.PaperSizeH.SIZE_H << std::endl);
			{
				std::string UNIT = "";
				switch(dataForPDB.PaperSizeH.UNIT){
				case UNIT_MODE_MM:		UNIT = "mm"; 	break;
				case UNIT_MODE_INCH:	UNIT = "inch";		break;
				case UNIT_1PER2_INCH:	UNIT = "1/2inch";	break;
				case UNIT_1PER6_INCH:	UNIT = "1/6inch";	break;
				default:				UNIT = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.PaperSizeH.UNIT));	break;
				}
				writeFile(fileHandle, "ページ送りサイズの単位(◎)\t" << UNIT << std::endl);
			}
			{
				std::string PRE_MARK_SIDE = "";
				switch(dataForPDB.PaperOption.PRE_MARK_SIDE){
				case DEF_PRINT_FRONT:	PRE_MARK_SIDE = "Front";	break;
				case DEF_PRINT_BACK:	PRE_MARK_SIDE = "Back";		break;
				default:				PRE_MARK_SIDE = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.PaperOption.PRE_MARK_SIDE));	break;
				}
				writeFile(fileHandle, "マークの印刷面(△※)\t" << PRE_MARK_SIDE << std::endl);
			}
			writeFile(fileHandle, "ミシン目パターンの繰り返し数(×)\t" << dataForPDB.PaperOption.REP_PATER_CNT << std::endl);
	
			writeFile(fileHandle, "印刷モード(◎※)CMYK_PRN_RESO_X\t" << dataForPDB.PrintMode.CMYK_PRN_RESO_X << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)CMYK_PRN_RESO_Y\t" << dataForPDB.PrintMode.CMYK_PRN_RESO_Y << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)CMYK_RIP_RESO_X\t" << dataForPDB.PrintMode.CMYK_RIP_RESO_X << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)CMYK_RIP_RESO_Y\t" << dataForPDB.PrintMode.CMYK_RIP_RESO_Y << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)ELS_PRN_RESO_X\t" << dataForPDB.PrintMode.ELS_PRN_RESO_X << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)ELS_PRN_RESO_Y\t" << dataForPDB.PrintMode.ELS_PRN_RESO_Y << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)ELS_RIP_RESO_X\t" << dataForPDB.PrintMode.ELS_RIP_RESO_X << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)ELS_RIP_RESO_Y\t" << dataForPDB.PrintMode.ELS_RIP_RESO_Y << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)SCREENING_FAMILY\t" << dataForPDB.PrintMode.SCREENING_FAMILY << std::endl);
			writeFile(fileHandle, "印刷モード(◎※)RESOURCE_ID\t" << dataForPDB.PrintMode.RESOURCE_ID << std::endl);
	
			writeFile(fileHandle, "印刷速度(○)\t" << dataForPDB.PrintSpeed << std::endl);
	
			writeFile(fileHandle, "ヒートローラー乾燥温度（自機）(○)\t" << dataForPDB.DryTempPrint.HEAT_TEMP_SELF << std::endl);
			writeFile(fileHandle, "ヒートローラー乾燥温度（他機）(△)\t" << dataForPDB.DryTempPrint.HEAT_TEMP_PART << std::endl);
			writeFile(fileHandle, "温風アシスト乾燥温度（自機）(△)\t" << dataForPDB.DryTempPrint.FAN_TEMP_SELF << std::endl);
			writeFile(fileHandle, "温風アシスト乾燥温度（他機）(△)\t" << dataForPDB.DryTempPrint.FAN_TEMP_PART << std::endl);
			writeFile(fileHandle, "NIR乾燥温度（自機）(△)\t" << dataForPDB.DryTempPrint.NIR_POWER_SELF << std::endl);
			writeFile(fileHandle, "NIR乾燥温度（他機）(△)\t" << dataForPDB.DryTempPrint.NIR_POWER_PART << std::endl);
			writeFile(fileHandle, "先塗り乾燥温度１(△)\t" << dataForPDB.DryTempPrint.PRE_TEMP_SELF << std::endl);
			writeFile(fileHandle, "先塗り乾燥温度２(△)\t" << dataForPDB.DryTempPrint.PRE_TEMP_PART << std::endl);
			writeFile(fileHandle, "ICCプリセット名(○)\t" << dataForPDB.ICCPreset << std::endl);
			writeFile(fileHandle, "ICCプリセットID(○)\t" << dataForPDB.ICCPresetID << std::endl);
	
			for(int systemType = 0; systemType < 2; ++systemType ) {
				writeFile(fileHandle, "トーンカーブ名（GUI 表示）(△)\t" << dataForPDB.DotgainCurve[systemType].DOTGAIN_NAME << std::endl);
				writeFile(fileHandle, "共通ファイル名\t" << dataForPDB.DotgainCurve[systemType].DOTGAIN_SHAR_NAME << std::endl);
				writeFile(fileHandle, "アンカーポイント情報 4 色 x 5点分(△) BeforeConPoint, AfterConPoint, AnchorPointEditType, AnchorPointType" << std::endl);
				for(int color = ANCHOR_COL_ALL; color < NumberOfColors; ++color){
					switch(color){
					case ANCHOR_COL_ALL:		writeFile(fileHandle, "ALL");	break;
					case ANCHOR_COL_BLACK:		writeFile(fileHandle, "K");		break;
					case ANCHOR_COL_YELLOW:		writeFile(fileHandle, "Y");		break;
					case ANCHOR_COL_MAGENTA:	writeFile(fileHandle, "M");		break;
					case ANCHOR_COL_CYAN:		writeFile(fileHandle, "C");		break;
					default:					writeFile(fileHandle, "unknown:" + std::to_string(static_cast<long long>(color)));		break;
					}
					for(int pointID = 0; pointID < ANCHOR_PT_CNT; ++pointID){
						writeFile(fileHandle, "\t" << dataForPDB.DotgainCurve[systemType].ANCHOR_POINT[color][pointID].POSITION.x << "\t" << dataForPDB.DotgainCurve[systemType].ANCHOR_POINT[color][pointID].POSITION.y << "\t" << dataForPDB.DotgainCurve[systemType].ANCHOR_POINT[color][pointID].EDIT_TYPE << "\t" << dataForPDB.DotgainCurve[systemType].ANCHOR_POINT[color][pointID].KIND << std::endl);
					}
				}
			}
	
			writeFile(fileHandle, "インフォメーションシートを出力するかどうか(○)\t" << (dataForPDB.JobBaseFlushingPage.informationSheet==true?"true":"false") << std::endl);
			{
				std::string flushingPageType = "";
				switch(dataForPDB.JobBaseFlushingPage.flushingPageType){
				case DEF_FLUSHING_PAGE_NONE:		flushingPageType = "NONE"; 	break;
				case DEF_FLUSHING_PATTERN_LINE_1:	flushingPageType = "USER"; 	break;
				default:							flushingPageType = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.JobBaseFlushingPage.flushingPageType)); 	break;
				}
				writeFile(fileHandle, "フラッシングページを挿入するかどうか(○)\t" << flushingPageType << std::endl);
			}
			writeFile(fileHandle, "フラッシングページのパターン名(△)\t" << dataForPDB.JobBaseFlushingPage.flushingPage << std::endl);
			writeFile(fileHandle, "フラッシングページの挿入間隔(△)\t" << dataForPDB.JobBaseFlushingPage.interval << std::endl);
			writeFile(fileHandle, "ジョブ前に挿入するかどうか(○)\t" << (dataForPDB.JobBaseFlushingPage.beforeJob==true?"true":"false") << std::endl);
			writeFile(fileHandle, "ジョブ前に挿入する回数(△)\t" << dataForPDB.JobBaseFlushingPage.printCountOfBeforeJob << std::endl);
			writeFile(fileHandle, "ジョブ後に挿入するかどうか(○)\t" << (dataForPDB.JobBaseFlushingPage.afterJob==true?"true":"false") << std::endl);
			writeFile(fileHandle, "フラッシングページのオフセット（表面・副走査）(△)\t" << dataForPDB.JobBaseFlushingPage.offset[0] << std::endl);
			writeFile(fileHandle, "フラッシングページのオフセット（裏面・副走査）(△)\t" << dataForPDB.JobBaseFlushingPage.offset[1] << std::endl);
	
			writeFile(fileHandle, "画像のオフセット（表面・主副）(○)\t" << dataForPDB.LayoutImagePosition.offset[0].x << "\t" << dataForPDB.LayoutImagePosition.offset[0].y << std::endl);
			writeFile(fileHandle, "画像のオフセット（裏面・主副）(△)\t" << dataForPDB.LayoutImagePosition.offset[1].x << "\t" << dataForPDB.LayoutImagePosition.offset[1].y << std::endl);
			writeFile(fileHandle, "サイド検査をするかどうか(○)\t" << (dataForPDB.LayoutVerifyMark.enableVerify==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "エラーにする連続シート数(△)\t" << dataForPDB.LayoutVerifyMark.errorSheets << std::endl);
			writeFile(fileHandle, "印刷開始マークを出力するかどうか(○)\t" << (dataForPDB.LayoutVerifyMark.printStartMark==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "バーコードを出力するかどうか(○)\t" << (dataForPDB.LayoutVerifyMark.verifyMark==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "バーコードのオフセット（表面・主走査。副走査は0 固定）(△)\t" << dataForPDB.LayoutVerifyMark.offsetX[0] << std::endl);
			writeFile(fileHandle, "バーコードのオフセット（裏面・主副）(△)\t" << dataForPDB.LayoutVerifyMark.offsetX[1] << "\t" << dataForPDB.LayoutVerifyMark.offsetY << std::endl);
			{
				std::string flushingPattern = "";
				switch(dataForPDB.LayoutFlushingPattern.flushingPattern){
				case DEF_FLUSHING_PATTERN_NONE:			flushingPattern = "None";	break;
				case DEF_FLUSHING_PATTERN_LINE_1:		flushingPattern = "Line1";	break;
				case DEF_FLUSHING_PATTERN_LINE_2:		flushingPattern = "Line2";	break;
				case DEF_FLUSHING_PATTERN_LINE_4:		flushingPattern = "Line4";	break;
				case DEF_FLUSHING_PATTERN_LINE_8:		flushingPattern = "Line8";	break;
				case DEF_FLUSHING_PATTERN_LINE_16:		flushingPattern = "Line16";	break;
				case DEF_FLUSHING_PATTERN_LINE_32:		flushingPattern = "Line32";	break;
				case DEF_FLUSHING_PATTERN_STAR_1:		flushingPattern = "Star1";	break;
				case DEF_FLUSHING_PATTERN_STAR_2:		flushingPattern = "Star2";	break;
				case DEF_FLUSHING_PATTERN_STAR_4:		flushingPattern = "Star4";	break;
				case DEF_FLUSHING_PATTERN_STAR_8:		flushingPattern = "Star8";	break;
				case DEF_FLUSHING_PATTERN_LINE_1_STAR:	flushingPattern = "Line1+Star";	break;
				case DEF_FLUSHING_PATTERN_LINE_2_STAR:	flushingPattern = "Line2+Star";	break;
				case DEF_FLUSHING_PATTERN_LINE_4_STAR:	flushingPattern = "Line4+Star";	break;
				case DEF_FLUSHING_PATTERN_LINE_8_STAR:	flushingPattern = "Line8+Star";	break;
				case DEF_FLUSHING_PATTERN_LINE_16_STAR:	flushingPattern = "Line16+Star";	break;
				case DEF_FLUSHING_PATTERN_LINE_32_STAR:	flushingPattern = "Line32+Star";	break;
				default:								flushingPattern = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.LayoutFlushingPattern.flushingPattern));	break;
				}
				writeFile(fileHandle, "フラッシングパターン(○※)\t" << flushingPattern << std::endl);
			}
			writeFile(fileHandle, "ラインフラッシングのずらし量(△)\t" << dataForPDB.LayoutFlushingPattern.gap << std::endl);
			writeFile(fileHandle, "ラインフラッシングのオフセット（表面・副走査）(△)\t" << dataForPDB.LayoutFlushingPattern.offsetY[0] << std::endl);
			writeFile(fileHandle, "ラインフラッシングのオフセット（裏面・副走査）(△)\t" << dataForPDB.LayoutFlushingPattern.offsetY[1] << std::endl);

			writeFile(fileHandle, "ページ内、ラインフラッシング(△)\t" << (dataForPDB.LayoutFlushingPatternAdd2.enableMultiLine==TRUE?"TRUE":"FALSE") << std::endl);
			writeFile(fileHandle, "ページ内、ラインフラッシング間隔（表面・副走査）(△)\t" << dataForPDB.LayoutFlushingPatternAdd2.position2ndLine[0] << std::endl);
			writeFile(fileHandle, "ページ内、ラインフラッシング間隔（裏面・副走査）(△)\t" << dataForPDB.LayoutFlushingPatternAdd2.position2ndLine[1] << std::endl);
			writeFile(fileHandle, "ページ内、ラインフラッシングの補正オフセット（表面・副走査）(△)\t" << dataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[0] << std::endl);
			writeFile(fileHandle, "ページ内、ラインフラッシングの補正オフセット（裏面・副走査）(△)\t" << dataForPDB.LayoutFlushingPatternAdd2.offsetY2ndLine[1] << std::endl);
			writeFile(fileHandle, "Bottom指定時、ラインフラッシングの下端配置および後処理マークの下端配置がON(△)\t" << (dataForPDB.LayoutVertivalAlignmentPPFLSPTN==TRUE?"TRUE":"FALSE") << std::endl);

			{
				std::string option = "";
				switch(m_DataForPDB.LayoutFlushingPatternAdd.lineOption){
				case DEF_LINE_OPTION_C0:	option = "C0";	break;
				case DEF_LINE_OPTION_NONE:	option = "NONE";	break;
				case DEF_LINE_OPTION_S0:	option = "S0";	break;
				case DEF_LINE_OPTION_S2:	option = "S2";	break;
				default:					option = "unknown:" + std::to_string(static_cast<long long>(m_DataForPDB.LayoutFlushingPatternAdd.lineOption));	break;
				}
				writeFile(fileHandle, "ラインフラッシングのオプション情報(△)\t" << option << std::endl);
			}
			{
				std::string option2 = "";
				switch(m_DataForPDB.LayoutFlushingPatternAdd.lineOption2){
				case DEF_LINE_OPTION2_STANDARD:		option2 = "STANDARD";		break;
				case DEF_LINE_OPTION2_CROSS_STITCH:	option2 = "CROSS_STITCH";	break;
				default:							option2 = "unknown:" + std::to_string(static_cast<long long>(m_DataForPDB.LayoutFlushingPatternAdd.lineOption2));	break;
				}
				writeFile(fileHandle, "ラインフラッシングのオプション情報2(△)\t" << option2 << std::endl);
			}

			{
				std::string side = "";
				switch(dataForPDB.LayoutPostProcess.side){
				case DEF_SELECT_SIDE_NONE:	side = "Off";	break;
				case DEF_SELECT_SIDE_FRONT:	side = "Front";	break;
				case DEF_SELECT_SIDE_BACK:	side = "Back";	break;
				case DEF_SELECT_SIDE_BOTH:	side = "Both";	break;
				default:					side = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.LayoutPostProcess.side));	break;
				}
				writeFile(fileHandle, "カッターマークの印刷面(○)\t" << side << std::endl);
			}
			writeFile(fileHandle, "カッターマークの準備シート数(△)\t" << dataForPDB.LayoutPostProcess.sheets << std::endl);
			writeFile(fileHandle, "カッターマークのサイズ（主副）(△)\t" << dataForPDB.LayoutPostProcess.size.cx << "\t" << dataForPDB.LayoutPostProcess.size.cy << std::endl);
			writeFile(fileHandle, "カッターマークのオフセット（表面・主走査）(△)\t" << dataForPDB.LayoutPostProcess.offsetX << std::endl);
			writeFile(fileHandle, "カッターマークのオフセット（裏面・主走査）(△)\t" << dataForPDB.LayoutPostProcess.offsetX_back << std::endl);
			{
				std::string alignment = "";
				switch(dataForPDB.LayoutPostProcess.alignment){
				case DEF_POSTPROCESS_NONE:	alignment = "NONE";		break;
				case DEF_POSTPROCESS_LEFT:	alignment = "LEFT";		break;
				case DEF_POSTPROCESS_RIGHT:	alignment = "RIGHT";	break;
				default:					alignment = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.LayoutPostProcess.alignment));	break;
				}
				writeFile(fileHandle, "カッターマークオフセットの基準位置（表面）(△)\t" << alignment << std::endl);
			}
			{
				std::string alignment_back = "";
				switch(dataForPDB.LayoutPostProcess.alignment_back){
				case DEF_POSTPROCESS_NONE:	alignment_back = "NONE";	break;
				case DEF_POSTPROCESS_LEFT:	alignment_back = "LEFT";	break;
				case DEF_POSTPROCESS_RIGHT:	alignment_back = "RIGHT";	break;
				default:					alignment_back = "unknown:" + std::to_string(static_cast<long long>(dataForPDB.LayoutPostProcess.alignment_back));	break;
				}
				writeFile(fileHandle, "カッターマークオフセットの基準位置（裏面）(△)\t" << alignment_back << std::endl);
			}

			writeFile(fileHandle, "JI系の情報の登録が必要かどうか\t" << m_DataSystemSetting.IsRegisterJetInspection() << std::endl);

			{
				std::string file = "";
				double resoX;
				double resoY;
				std::string jobName;
				SYSTEMTIME date;
				if (CStub_PaperDBIF::PDB_GetJIPhotoOnJobPrint(printConditionName.c_str(), COM_SYSTEM_TYPE_SELF, file, resoX, resoY, jobName, date) == true) {
					writeFile(fileHandle, "ジョブ印刷時のJI撮影画像ファイル\t" << file << std::endl);
				}
				else{
					writeFile(fileHandle, "ジョブ印刷時のJI撮影画像ファイル\t<<なし>>" << std::endl);
				}	
			}

			{
				std::string file = "";
				if (CStub_PaperDBIF::PDB_GetScanFileName(printConditionName.c_str(), COM_SYSTEM_TYPE_SELF, file) == true) {
					writeFile(fileHandle, "スキャン画像ファイル\t" << file << std::endl);
				}
				else{
					writeFile(fileHandle, "スキャン画像ファイル\t<<なし>>" << std::endl);
				}	
			}

			{
				DEF_LENGTH_UNIT systemUnit = UserModeGUI_GetLengthUnit();
				ST_JOB_IMAGE_POSITION imagePosition = {};
				ST_JOB_FLUSHING_PATTERN flushingPattern = {};
				ST_JOB_FLUSHING_PATTERN_ADD flushingPatternAdd = {};
				ST_JOB_FLUSHING_PATTERN_ADD2 flushingPatternAdd2 = {};
				ST_JOB_VERIFY_MARK verifyMark = {};
				ST_JOB_POSTPROCESS postProcess = {};
				ST_JOB_POSTPROCESS_ADD postProcessAdd = {};
				ST_JOB_DECODE decode;
				BOOL layoutVertivalAlignmentPPFLSPTN = FALSE;

				if (CStub_PaperDBIF::PDB_GetLayoutImageParameterWithPrintConditionNameEx(
					printConditionName.c_str(),
					systemUnit,
					imagePosition,
					flushingPattern,
					flushingPatternAdd,
					flushingPatternAdd2,
					verifyMark,
					postProcess,
					postProcessAdd,
					decode,
					layoutVertivalAlignmentPPFLSPTN) == true)
				{
					writeFile(fileHandle, "レイアウト画面設定のデコード情報\t" << decode.enableDecode << "\t"  << decode.settingName << std::endl);
				}
				else{
					writeFile(fileHandle, "レイアウト画面設定のデコード情報\t<<なし>>" << std::endl);
				}
			}

			writeFile(fileHandle, "検査条件名(△)\t" << dataForPDB.JICondition << std::endl);
			//検査条件詳細情報(△)のデバッグログはスタブにて
			if(dataForPDB.TemporalMaskFile.empty())
			{
				writeFile(fileHandle, "一時マスクファイルパス(△)\t<<なし>>" << std::endl);
			}
			else
			{
				writeFile(fileHandle, "一時マスクファイルパス(△)\t" << dataForPDB.TemporalMaskFile << std::endl);
			}
			//一時マスク情報(△)のデバッグログはスタブにて
			writeFile(fileHandle, "非検査エリア(紙端)(△)(紙端:左)(0:自機)\t" << dataForPDB.NonInspectionLeft[0] << std::endl);
			writeFile(fileHandle, "非検査エリア(紙端)(△)(紙端:左)(1:相手機)\t" << dataForPDB.NonInspectionLeft[1] << std::endl);
			writeFile(fileHandle, "非検査エリア(紙端)(△)(紙端:右)(0:自機)\t" << dataForPDB.NonInspectionRight[0] << std::endl);
			writeFile(fileHandle, "非検査エリア(紙端)(△)(紙端:右)(1:相手機)\t" << dataForPDB.NonInspectionRight[1] << std::endl);

			{
				if(dataForPDB.NonInspectionArea.size() == 0)
				{
					writeFile(fileHandle, "非検査エリア(矩形)(△)\t<<なし>>" << std::endl);		
				}
				else
				{
					for(auto& jt = dataForPDB.NonInspectionArea.begin(); jt != dataForPDB.NonInspectionArea.end(); jt++)
					{
						for(auto& it = jt->begin(); it != jt->end(); it++)
						{
							writeFile(fileHandle, "非検査エリア(矩形)(△)" 
								<< (std::distance(dataForPDB.NonInspectionArea.begin(), jt) == 0 ? "自機" : "相手機") << ":" 
								<< std::distance(jt->begin(), it) << "\t"
								<< ((it-> STR_AREA_NAME == '\0') ? "<空>" : std::string(1, it-> STR_AREA_NAME)) << "\t"
								<< it-> START_POS_X << "\t"
								<< it-> START_POS_Y << "\t"
								<< it-> END_POS_X << "\t"
								<< it-> END_POS_Y
								<< std::endl);
						}
					}
				}
			}
			writeFile(fileHandle, "「デコード」タブ、エラー停止連続シート数(△)\t" << dataForPDB.Decode.errorSheets << std::endl);
			writeFile(fileHandle, "デコード情報(△)\t"
				<< (dataForPDB.Decode.enableDecode ? "TRUE" : "FALSE") << "\t"
				<< dataForPDB.Decode.errorSheets << "\t"
				<< dataForPDB.Decode.totalNumberOfDecode << "\t"
				<< dataForPDB.Decode.numberOfDecodeMax << "\t"
				<< dataForPDB.Decode.numberOfDecode[0] << "\t"
				<< dataForPDB.Decode.numberOfDecode[1] << "\t"
				<< std::string(dataForPDB.Decode.settingName) << "\t"
				<< std::endl);

			auto writeDecodeDetail = [&writeFile](HANDLE inFileHandle, const std::string& inTitle, const std::vector<ST_JOB_DECODE_DETAIL>& inDetailList)->void
			{
				if(inDetailList.size() == 0)
				{
					writeFile(inFileHandle, inTitle + "\t<<なし>>" << std::endl);				
				}
				else
				{
					writeFile(inFileHandle, "タイトル" << "\t"
						<< "インデクス" << "\t" 
						<< "size x" << "\t" 
						<< "size y" << "\t" 
						<< "offset x" << "\t" 
						<< "offset y" << "\t" 
						<< "area name" << "\t" 
						<< "label type" << "\t" 
						<< "pattern" << "\t" 
						<< "angle for OCR pattern" << "\t" 
						<< "check a decoding error(TRUE: ON, FALSE: OFF)"
						<< std::endl);
				}
				for(auto& it = inDetailList.begin(); it != inDetailList.end(); it++)
				{
					std::string labelType = "";
					std::string pattern = "";
					switch(it->labelType)
					{
					case DEF_DECODE_OCR:
						{
							labelType = "OCR";
							switch(it->pattern.pat_OCR)
							{
							case DEF_OCR_NUMBER:			pattern = "OCR_NUMBER";				break;
							case DEF_OCR_TEXT:				pattern = "OCR_TEXT";				break;
							case DEF_OCR_PRIORITY_NUMBER:	pattern = "OCR_PRIORITY_NUMBER";	break;
							case DEF_OCR_PRIORITY_TEXT:		pattern = "OCR_PRIORITY_TEXT";		break;
							default:						pattern = "unknown:" + std::to_string(static_cast<long long>(it->pattern.pat_OCR));				break;
							}
						}
						break;
					case DEF_DECODE_1D_BARCODE:
						{
							labelType = "1D_BARCODE";
							switch(it->pattern.pat_1D)
							{
							case DEF_1D_CODE39:		pattern = "1D_CODE39";		break;
							case DEF_1D_CODE128:	pattern = "1D_CODE128:";	break;
							case DEF_1D_NW7:		pattern = "1D_NW7";			break;
							case DEF_1D_CODE39_CD:	pattern = "1D_CODE39_CD";	break;
							default:				pattern = "unknown:" + std::to_string(static_cast<long long>(it->pattern.pat_1D));		break;
							}
						}
						break;
					case DEF_DECODE_2D_BARCODE:
						{
							labelType = "2D_BARCODE";
							switch(it->pattern.pat_2D)
							{
							case DEF_2D_QR_CODE:		pattern = "2D_QR_CODE";		break;
							case DEF_2D_DATA_MATRIX:	pattern = "2D_DATA_MATRIX";	break;
							case DEF_2D_PDF417:			pattern = "2D_PDF417";		break;
							default:					pattern = "unknown:" + std::to_string(static_cast<long long>(it->pattern.pat_2D));		break;
							}
						}
						break;
					default:
						labelType = "unknown";
						pattern = "unknown";
						break;
					}

					std::string ocrAngle ="";
					switch(it->ocrAngle)
					{
					case DEF_OCR_ANGLE_0:		ocrAngle = "OCR_ANGLE_0";	break;
					case DEF_OCR_ANGLE_90:		ocrAngle = "OCR_ANGLE_90";	break;
					case DEF_OCR_ANGLE_180:		ocrAngle = "OCR_ANGLE_180";	break;
					case DEF_OCR_ANGLE_270:		ocrAngle = "OCR_ANGLE_270";	break;
					default:					ocrAngle = "unknown:" + std::to_string(static_cast<long long>(it->ocrAngle));		break;
					}

					writeFile(inFileHandle, inTitle 
						<< std::distance(inDetailList.begin(), it) << "\t" 
						<< it->size.cx << "\t" 
						<< it->size.cy << "\t" 
						<< it->offset.x << "\t" 
						<< it->offset.y << "\t" 
						<< std::string(it->name) << "\t" 
						<< labelType << "\t" 
						<< pattern << "\t" 
						<< ocrAngle << "\t" 
						<< (it->checkError == TRUE ? "TRUE" : "FALSE")
						<< std::endl);
				}
			};
			writeDecodeDetail(fileHandle, "デコード詳細情報(△)decoding area(front)", dataForPDB.Decode.decodeDetailF);
			writeDecodeDetail(fileHandle, "デコード詳細情報(△)decoding area(back)", dataForPDB.Decode.decodeDetailB);

			writeFile(fileHandle, "変倍機能のON/OFF(△)\t" << dataForPDB.RipMagnification.IS_ENABLE << std::endl);
			writeFile(fileHandle, "基準面(△)\t"<< dataForPDB.RipMagnification.IS_DATUM_PLANE_FRONT << std::endl);
			writeFile(fileHandle, "変倍値(表:横)(△)\t" << dataForPDB.RipMagnification.FRONT_X << std::endl);
			writeFile(fileHandle, "変倍値(表:縦)(△)\t" << dataForPDB.RipMagnification.FRONT_Y << std::endl);
			writeFile(fileHandle, "変倍値(裏:横)(△)\t" << dataForPDB.RipMagnification.BACK_X << std::endl);
			writeFile(fileHandle, "変倍値(裏:縦)(△)\t" << dataForPDB.RipMagnification.BACK_Y << std::endl);
		}
	
		closeFile(fileHandle);
	}
	catch(std::runtime_error& error){
		std::stringstream log;
		log << "CPrintConditionRegister::debugout " << error.what();
		WriteToLogBuf(LOG_DEBUG, log.str());
		closeFile(fileHandle);
	}
}

/**
 * @brief	変倍パラメータを決定する
 * @param[in]	inScalingValue : 変倍機能のON/OFF
 * @param[in]	inScalingBaseSide : 基準面
 * @param[in]	inScalingValueFrontW : 変倍値(表:横)
 * @param[in]	inScalingValueFrontH : 変倍値(表:縦)
 * @param[in]	inScalingValueBackW : 変倍値(裏:横)
 * @param[in]	inScalingValueBackH : 変倍値(裏:縦)
 * @param[in]	inScalingValueBackH : 変倍値(裏:縦)
 * @param[in]	inIsDED : DEDかSESか(true:DED/false:SES)
 * @param[out]	outRipMagnification : 値をセットする変倍パラメータ構造体
 * @note	エラー時は例外を投げる
*/
void CPrintConditionRegister::ImportScaling(
	const CValue<bool>* inScalingValue,
	const CValue<std::string>* inScalingBaseSide,
	const CValue<double>* inScalingValueFrontW,
	const CValue<double>* inScalingValueFrontH,
	const CValue<double>* inScalingValueBackW,
	const CValue<double>* inScalingValueBackH,
	const bool& inIsDED,
	ST_PDB_RIP_MAGNIFICATION& outRipMagnification)
{
	/**
	 * @brief	変倍値の範囲チェック
	 * @param[in]	inValue : 変倍値
	 * @return 範囲内かどうか(true:範囲内/false:範囲外)
	*/
	auto isInRange = [](long inValue)->bool
	{
		if( (10000 > inValue) || (1000000 <= inValue) )
		{
			return false;
		}
		return true;
	};

	ERRROR_IF_NULL(inScalingValue, "kScalingValue");
	ERRROR_IF_NULL(inScalingBaseSide, "kScalingBaseSide");
	ERRROR_IF_NULL(inScalingValueFrontW, "kScalingValueFrontW");
	ERRROR_IF_NULL(inScalingValueFrontH, "kScalingValueFrontH");
	ERRROR_IF_NULL(inScalingValueBackW, "kScalingValueBackW");
	ERRROR_IF_NULL(inScalingValueBackH, "kScalingValueBackH");

	// xmlファイルから取得していればxmlファイルの値、取得できなかった場合は初期値を設定
	BOOL isEnable = FALSE;
	BOOL isDatumPlaneFront = ( inIsDED ? FALSE : TRUE );
	long frontX = 100000;
	long frontY = 100000;
	long backX = 100000;
	long backY = 100000;

	if( inScalingValue->IsSet() )
	{
		isEnable = ( inScalingValue->Get() ? TRUE : FALSE );
	}

	if( inIsDED )
	{
		if( inScalingBaseSide->IsSet() )
		{
			std::string baseSide = inScalingBaseSide->Get();
			isDatumPlaneFront = ( (baseSide == "Front") ? TRUE : FALSE );
		}
	}

	if( inScalingValueFrontW->IsSet() )
	{
		frontX = static_cast<long>(inScalingValueFrontW->Get() * 1000);
	}

	if( inScalingValueFrontH->IsSet() )
	{
		frontY = static_cast<long>(inScalingValueFrontH->Get() * 1000);
	}

	if( inIsDED )
	{
		if( inScalingValueBackW->IsSet() )
		{
			backX = static_cast<long>(inScalingValueBackW->Get() * 1000);
		}

		if( inScalingValueBackH->IsSet() )
		{
			backY = static_cast<long>(inScalingValueBackH->Get() * 1000);
		}
	}

	if(( !isInRange(frontX) ) || ( !isInRange(frontY) ) || ( !isInRange(backX) ) || ( !isInRange(backY) ))
	{
		std::stringstream err;
		err << " ImportScaling Error. ScalingValue is Out of range." 
			<< " frontX:" << frontX << " frontY:" << frontY
			<< " backX:" << backX << " backY:" << backY;
		throw std::runtime_error(err.str());
	}

	outRipMagnification.IS_ENABLE = isEnable;
	outRipMagnification.IS_DATUM_PLANE_FRONT = isDatumPlaneFront;
	outRipMagnification.FRONT_X = frontX;
	outRipMagnification.FRONT_Y = frontY;
	outRipMagnification.BACK_X = backX;
	outRipMagnification.BACK_Y = backY;
}

/**
 * @brief	用紙種類の新規追加確認
 * @param[out]	PaperTypeName : 新規追加した用紙種類名
 * @retval	true : 新規追加実施
 * @retval	false : 追加なし
**/
bool CPrintConditionRegister::IsAppendPaperType(
	std::string& PaperTypeName
)
{
	PaperTypeName = ( m_isAddPaperType ) ? m_DataForPDB.PaperType : "";
	return	m_isAddPaperType;
}

/**
 * @brief	段差補正の新規追加確認
 * @param[out]	Name : 新規追加した段差補正名
 * @retval	true : 新規追加実施
 * @retval	false : 追加なし
**/
bool CPrintConditionRegister::IsAppendAllignment(
	std::string& AllignmentName
)
{
	AllignmentName = ( m_isAddLevelTuneName ) ? m_DataForPDB.LevelTuneName : "";
	return	m_isAddLevelTuneName;

}

