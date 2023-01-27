/**
	@file	Stub_Mainte_JetInspectionIF.cpp
	@brief	stub of Mainte_JetInspection plugin
	@author	K.Matsuoka
	@date	2019/07/17 K.Matsuoka 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <fstream>
#include <map>

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "Stub_Mainte_JetInspectionIF.h"
#include "MainteJetInspection_Callbacks.h"
#include "Ini_PrintConditionImporter.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SMainteJetInspection_Callbacks* outCbr);

// ==========================================================================

static bool isLogEnabled()
{
	static bool inited = false;			//!< 初期化済みかどうか
	static bool enabled = false;		//!< ログ有効

	if(inited == false)
	{	// 未初期化ならINIファイルを参照してログ設定を読み込む
		CIni_PrintConditionImporter ini_PrintConditionImporter;
		ini_PrintConditionImporter.Initialize(TRUE);

		inited = true;		// 初期化済み
		enabled = ini_PrintConditionImporter.IsDebugLog();		// デバッグログ設定

		ini_PrintConditionImporter.Finalize();
	}
	return enabled;
}


//デバッグ出力(Debug構成時のみ)
static void debugout(std::string inLine)
{	
	if(isLogEnabled() == false)
	{	//デバッグログ無効
		return;
	}
	std::string fileName = std::string(::GetModuleDirectory()) + std::string("log\\PrintConditionImporter\\stub.txt");
	std::ofstream outputFile(fileName, std::ios_base::out|std::ios_base::app);
    outputFile << inLine << std::endl;
    outputFile.close();
}

//double用to_string()
static std::string to_string(double inVal)
{
	return std::to_string(static_cast<long double>(inVal));
}

//STRUCT_JI_FLAW_INFOのダンプ
static void debugoutJIFlawInfo(std::vector<STRUCT_JI_FLAW_INFO>& inJIFlawInfo)
{
	debugout("---- debugoutJIFlawInfo");
	for(auto& it = inJIFlawInfo.begin(); it != inJIFlawInfo.end(); it++)
	{
		std::string kind = "";
		switch(it->EN_FLAW_KIND)
		{
		case EN_DIRT:			kind = "Blot";			break;
		case EN_LOST:			kind = "Lack";			break;
		case EN_WHITE_LINE:		kind = "White Streak";	break;
		case EN_STREAK_DEFECT:	kind = "Streak Defect"; break;
		default:				kind = "unknown:" + std::to_string(static_cast<long long>(it->EN_FLAW_KIND)); break;
		}

		debugout("インデクス:" + to_string(std::distance(inJIFlawInfo.begin(), it)));
		debugout("検出種別:" + kind);
		debugout("中心座標(X)(mm):" + to_string(it->COORDINATE_X));
		debugout("中心座標(Y)(mm):" + to_string(it->COORDINATE_Y));
		debugout("幅(mm):" + to_string(it->WIDTH));
		debugout("高さ(mm):" + to_string(it->HEIGHT));
		debugout("一時マスクの幅(mm):" + to_string(it->TEMPORAY_MASK_WIDTH));
		debugout("一時マスクの高さ(mm):" + to_string(it->TEMPORAY_MASK_HEIGHT));
		debugout("--------------------------------------");
	}//for
	debugout("");
}


//SMaintJI_VerificationParamsのダンプ
static void debugoutSMaintJI_VerificationParams(STRUCT_VERIFICATION_PARAMS& inVerificationParams)
{
	debugout("---- debugoutSMaintJI_VerificationParams");

	debugout("印刷自動停止(0:OFF 1:赤 2:赤橙)" + inVerificationParams.AutoPrintStop);
	debugout("印刷停止する累積回数(橙)(回数) " + inVerificationParams.AutoStopDefectCountOrange);
	debugout("印刷停止する累積回数(赤)(回数) " + inVerificationParams.AutoStopDefectCountRed);
	debugout("欠点出現率しきい値(橙)(%)" + inVerificationParams.DefectAppearanceRatioOrange);
	debugout("欠点出現率しきい値(赤)(%)" + inVerificationParams.DefectAppearanceRatioRed);
	debugout("欠落検出(段階)" + inVerificationParams.DefectDetectionSensitivity);
	debugout("欠陥高さ(mm)" + inVerificationParams.DetectionLevelHeight);
	debugout("欠陥幅(mm)" + inVerificationParams.DetectionLevelWidth);
	debugout("汚れ・欠落検査マスク幅(mm)" + inVerificationParams.DetectionMaskWidth);
	debugout("スジ汚れ写り除去(段階)" + inVerificationParams.DirtStreakBackImageRemoval);
	debugout("スジ汚れフィルタサイズ(mm)" + inVerificationParams.DirtStreakFilter);
	debugout("スジ汚れ欠陥最大幅(mm)" + inVerificationParams.DirtStreakMaximumWidth);
	debugout("スジ汚れ欠陥最小幅(mm)" + inVerificationParams.DirtStreakMinimumWidth);
	debugout("スジ汚れ積算高さ(mm)" + inVerificationParams.DirtStreakSigma);
	debugout("スジ汚れ値(段階)" + inVerificationParams.DirtStreakValue);
	debugout("スジ汚れ(0:OFF 1:ON)" + inVerificationParams.IsDirtStreakDetection);
	debugout("事前印刷マスク幅(mm)" + inVerificationParams.PrePrintMaskWidth);
	debugout("汚れ検出(段階)" + inVerificationParams.StainDetectionSensitivity);
	debugout("白スジ積算飽和値(段階)" + inVerificationParams.StreakDetectionLevelStep);
	debugout("白スジマスク幅(mm)" + inVerificationParams.StreakDetectionLevelWidth);
	debugout("白スジ検出(段階)" + inVerificationParams.StreakDetectionSensitivity);
	debugout("--------------------------------------");
	debugout("");
}

//SMaintJI_AIAssistParamsのダンプ
static void debugoutSMaintJI_AIAssistParams(STRUCT_AIASSIST_PARAMS* inAIAssistParams)
{
	if( inAIAssistParams ) {
		debugout("---- debugoutSMaintJI_AIAssistParams");

		debugout("AIアシストレベル" + inAIAssistParams->AssistRange);
		debugout("欠落検出" + inAIAssistParams->DefectDetectionSensitivity);
		debugout("欠陥高さ(mm)" + inAIAssistParams->DetectionLevelHeight);
		debugout("欠陥幅(mm)" + inAIAssistParams->DetectionLevelWidth);
		debugout("スジ汚れ判定" + inAIAssistParams->DirtStreakValue);
		debugout("AIアシスト機能の有効／無効" + inAIAssistParams->EnableAIAssist);
		debugout("AIフィルタリングの有効／無効" + inAIAssistParams->EnableAIFiltering);
		debugout("AI自動設定の有効／無効" + inAIAssistParams->EnableAutoSetting);
		debugout("警告レベル" + inAIAssistParams->NotificationLevel);
		debugout("汚れ判定レベル" + inAIAssistParams->StainDetectionSensitivity);
		debugout("白スジ判定レベル" + inAIAssistParams->StreakDetectionSensitivity);
		debugout("--------------------------------------");
		debugout("");
	}
}

/**
	I/F取得
	@param[out]	outCbr : I/F構造体
	@retval	true : 成功
	@retval	false : 失敗
*/
static bool GetCallbacks(SMainteJetInspection_Callbacks* outCbr)
{
	if(!MainteJetInspection_GetCallbacks(outCbr)){
		return false;
	}
	return true;	
}

/**
	constructor
*/
CStub_Mainte_JetInspectionIF::CStub_Mainte_JetInspectionIF()
{
}

/**
	destructor
*/
CStub_Mainte_JetInspectionIF::~CStub_Mainte_JetInspectionIF()
{
}

BOOL CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveJIFlawInfoListToTempMaskPath(
	std::string& ioFilePath,
	std::vector<STRUCT_JI_FLAW_INFO> inJIFlawInfoListFront,
	std::vector<STRUCT_JI_FLAW_INFO> inJIFlawInfoListBack)
{
	debugout("SaveJIFlawInfoListToTempMaskPath ioFilePath:" + ioFilePath);
	debugout("対象面（表)");
	debugoutJIFlawInfo(inJIFlawInfoListFront);
	debugout("対象面（裏)");
	debugoutJIFlawInfo(inJIFlawInfoListBack);

	SMainteJetInspection_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	if(cbr.MainteJetInspection_SaveJIFlawInfoListToTempMaskPath == nullptr)
	{
		return false;
	}

	BOOL ret = cbr.MainteJetInspection_SaveJIFlawInfoListToTempMaskPath(ioFilePath, inJIFlawInfoListFront, inJIFlawInfoListBack);
	debugout("ioFilePath(returned value): " + ioFilePath);
	debugout("ret: " + std::string((ret == TRUE ? "TRUE" : "FALSE")));

	return ret;
}

long CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveInspectionParameter(
	std::string& inInspectionName,
	STRUCT_VERIFICATION_PARAMS& inVerifyParamsFront,
	STRUCT_VERIFICATION_PARAMS& inVerifyParamsBack)
{
	debugout("SaveInspectionParameter" + inInspectionName);
	debugout("対象面（表)");
	debugoutSMaintJI_VerificationParams(inVerifyParamsFront);
	debugout("対象面（裏)");
	debugoutSMaintJI_VerificationParams(inVerifyParamsBack);

	SMainteJetInspection_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return -1;
	}
	if(cbr.MainteJetInspection_SaveInspectionParameter == nullptr)
	{
		return -1;
	}
	long ret = cbr.MainteJetInspection_SaveInspectionParameter(inInspectionName, inVerifyParamsFront, inVerifyParamsBack);
	debugout("ret: " + std::to_string(static_cast<long long>(ret)));

	return ret;
}

long CStub_Mainte_JetInspectionIF::MainteJetInspection_SaveInspectionAIAssistParameter(
	std::string& inInspectionName,
	STRUCT_VERIFICATION_PARAMS& inVerifyParamsFront,
	STRUCT_VERIFICATION_PARAMS& inVerifyParamsBack,
	STRUCT_AIASSIST_PARAMS* inAIAssistParmsFront,
	STRUCT_AIASSIST_PARAMS* inAIAssistParmsBack)
{
	debugout("SaveInspectionAIAssistParameter" + inInspectionName);
	debugout("対象面（表)");
	debugoutSMaintJI_VerificationParams(inVerifyParamsFront);
	debugoutSMaintJI_AIAssistParams(inAIAssistParmsFront);
	debugout("対象面（裏)");
	debugoutSMaintJI_VerificationParams(inVerifyParamsBack);
	debugoutSMaintJI_AIAssistParams(inAIAssistParmsBack);

	SMainteJetInspection_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return -1;
	}
	if(cbr.MainteJetInspection_SaveInspectionAIAssistParameter == nullptr)
	{
		return -1;
	}
	long ret = cbr.MainteJetInspection_SaveInspectionAIAssistParameter(inInspectionName, inVerifyParamsFront, inVerifyParamsBack, inAIAssistParmsFront, inAIAssistParmsBack);
	debugout("ret: " + std::to_string(static_cast<long long>(ret)));

	return ret;
}
