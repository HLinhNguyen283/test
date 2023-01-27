/*! \file PrintConditionExportXML.cpp
 *  \brief PrintConditionExportXML.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
 *  \date 2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "PrintConditionExportXML.h"
#include "CommonCBRProc.h"
#include "DataSystemSetting.h"
#include "CommonCBRProc_OP.h"

#include <sstream>
// ==========================================================================

namespace export_xml
{
 //コンストラクタ
PrintCondition::PrintCondition(const SPrintConditionData& inPrintConditionData)
	:m_PrintConditionData(inPrintConditionData)
{
}

// デストラクタ
PrintCondition::~PrintCondition()
{
}

// 印刷条件名の取得
std::string PrintCondition::GetPrintConditionName() const
{
	return m_PrintConditionData.printConditionName;
}

// 用紙種類の取得
std::string PrintCondition::GetPaperType() const
{
	return m_PrintConditionData.migrationData.PaperType;
}

// 段差補正名の取得
std::string PrintCondition::GetLevelTuneName() const
{
	return m_PrintConditionData.migrationData.LevelTuneName;
}

// 用紙設定情報の取得
bool PrintCondition::GetPaperSetting(SPaperSetting& outPaperSetting) const
{
	auto getUnitModeData = [](const eUNIT_MODE inUnitMode, std::string& outPageFeedSideUnit)
	{
		std::map<eUNIT_MODE, std::string> unitModeList;
		unitModeList.insert(std::make_pair(UNIT_MODE_MM, "mm"));
		unitModeList.insert(std::make_pair(UNIT_MODE_INCH, "inch"));
		unitModeList.insert(std::make_pair(UNIT_1PER2_INCH, "1/2inch"));
		unitModeList.insert(std::make_pair(UNIT_1PER6_INCH, "1/6inch"));

		auto target = unitModeList.find(inUnitMode);
		if(target == unitModeList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible PageFeedSize (Input : " << inUnitMode << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outPageFeedSideUnit = target->second;
	};

	auto getMarkSide = [](const DEF_PRINT_SIDE inMarkSide, std::string& outMarkSide)
	{
		std::map<DEF_PRINT_SIDE, std::string> markSideList;
		markSideList.insert(std::make_pair(DEF_PRINT_FRONT, "Front"));
		markSideList.insert(std::make_pair(DEF_PRINT_BACK, "Back"));
		auto target = markSideList.find(inMarkSide);

		if(target == markSideList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible PageFeedSize (Input : " << inMarkSide << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outMarkSide = target->second;
	};

	try
	{
		SPaperSetting paperSetting;

		// 用紙幅
		paperSetting.m_width = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperSizeW), DEF_LENGTH_MM, DEF_LENGTH_POINT) / 1000.0;

		// 用紙厚み
		paperSetting.m_thickness = m_PrintConditionData.migrationData.Thickness;

		// DNS設定
		paperSetting.m_DNSStatus = m_PrintConditionData.migrationData.DNSStatus;

		// テンション
		paperSetting.m_tension = m_PrintConditionData.migrationData.Tension;

		// パンチ穴用紙の時
		if(TRUE == m_PrintConditionData.migrationData.PaperOption.IS_HOLES)
		{
			paperSetting.m_punchHole = true;
		}
		// パンチ穴 +ミシン目有り用紙の時
		else if(TRUE == m_PrintConditionData.migrationData.PaperOption.IS_HOLES_PERFORATIONS)
		{
			paperSetting.m_punchHole = true;
			paperSetting.m_sewing = true;
		}

		// 事前マーク
		paperSetting.m_marker = (m_PrintConditionData.migrationData.PaperOption.IS_PRE_MARK == TRUE? true : false);

		// 事前印刷
		paperSetting.m_prePrinted = (m_PrintConditionData.migrationData.PaperOption.IS_PRE_MARK_PAPER == TRUE? true : false);

		// マークオフセット(主走査)
		paperSetting.m_markerOffset = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperOption.PRE_MARK_OFFSET), DEF_LENGTH_MM, DEF_LENGTH_POINT) / 1000.0;

		// ページ送りサイズの単位の取得
		std::string pageFeedSideUnit;
		getUnitModeData(m_PrintConditionData.migrationData.PaperSizeH.UNIT, pageFeedSideUnit);

		// ページ送りサイズ
		if (TRUE == m_PrintConditionData.migrationData.PaperSizeH.IS_SIZE_H)
		{
			paperSetting.m_pageFeedSize = ConvertUnit(static_cast<double>(m_PrintConditionData.migrationData.PaperSizeH.SIZE_H), m_PrintConditionData.migrationData.PaperSizeH.UNIT, UNIT_MODE_POINT) / 1000.0;
		}
		else
		{
			paperSetting.m_pageFeedSize = 0.0;
		}

		// ページ送りサイズの単位
		paperSetting.m_pageFeedSizeUnit = pageFeedSideUnit;

		// マークの印刷面
		std::string markSide;
		getMarkSide(m_PrintConditionData.migrationData.PaperOption.PRE_MARK_SIDE, markSide);
		paperSetting.m_markSide = markSide;
		
		//ミシン目パターンの繰り返し数
		paperSetting.m_repeatSheets = m_PrintConditionData.migrationData.PaperOption.REP_PATER_CNT;

		outPaperSetting = paperSetting;
		return true;
	}
	catch(std::exception& e)
	{
		writeErrorMessage(e.what());
		return false;
	}
}

//印刷モードの取得
bool PrintCondition::GetPrintMode(std::string& outPrintMode) const
{
	try
	{
		auto target = m_PrintConditionData.printModeDataList.find(m_PrintConditionData.migrationData.PrintMode.RESOURCE_ID);
		if(target == m_PrintConditionData.printModeDataList.end())
		{
			throw std::runtime_error("Not compatible printMode");
		}
		outPrintMode = target->second;
		return true;
	}
	catch(std::exception& e)
	{
		// 対応していないパターンの場合
		writeErrorMessage(e.what());
		return false;
	}
}

//印刷速度の取得
double PrintCondition::GetPrintSpeed() const
{
	return m_PrintConditionData.migrationData.PrintSpeed;
}

// 乾燥温度情報
void PrintCondition::GetDryerTemperature(SDryerTemperture& outDryerTemperture) const
{
	SDryerTemperture dryerTemperture;

	// ヒートローラー乾燥温度（自機）
	dryerTemperture.m_heatRollerSelf = m_PrintConditionData.migrationData.DryTempPrint.HEAT_TEMP_SELF;

	// ヒートローラー乾燥温度（他機）	
	dryerTemperture.m_heatRollerPart = m_PrintConditionData.migrationData.DryTempPrint.HEAT_TEMP_PART;

	// 温風アシスト乾燥温度（自機）
	dryerTemperture.m_heatFanSelf = m_PrintConditionData.migrationData.DryTempPrint.FAN_TEMP_SELF;

	// 温風アシスト乾燥温度（他機）
	dryerTemperture.m_heatFanPart = m_PrintConditionData.migrationData.DryTempPrint.FAN_TEMP_PART;

	// 先塗り乾燥温度1
	dryerTemperture.m_precoater1 =  m_PrintConditionData.migrationData.DryTempPrint.PRE_TEMP_SELF;

	// 先塗り乾燥温度2
	dryerTemperture.m_precoater2 =  m_PrintConditionData.migrationData.DryTempPrint.PRE_TEMP_PART;

	// NIR設定（自機）
	dryerTemperture.m_NirPowerSelf = m_PrintConditionData.migrationData.DryTempPrint.NIR_POWER_SELF;

	// NIR設定（他機）
	dryerTemperture.m_NirPowerPart = m_PrintConditionData.migrationData.DryTempPrint.NIR_POWER_PART;

	// サブヒートローラー乾燥温度（自機）
	dryerTemperture.m_SubHeatRollerSelf = m_PrintConditionData.migrationData.DryTempPrint.SUB_HEAT_TEMP_SELF;

	// サブヒートローラー乾燥温度（他機）	
	dryerTemperture.m_SubHeatRollerPart = m_PrintConditionData.migrationData.DryTempPrint.SUB_HEAT_TEMP_PART;

	outDryerTemperture = dryerTemperture;

}

// ICCプリセット情報の取得
void PrintCondition::GetICCPreset(SICCPreset& outICCPreset) const
{
	SICCPreset iccPreset;
	
	// ICCプリセット名
	iccPreset.m_descriptiveName = m_PrintConditionData.migrationData.ICCPreset;

	// ICCプリセットID
	iccPreset.m_presetID = m_PrintConditionData.migrationData.ICCPresetID;

	outICCPreset = iccPreset;
}

// ジョブ基本設定情報の取得
void PrintCondition::GetJobBase(SJobBase& outJobBase) const
{

	SJobBase jobBase;

	// インフォメーションシートを出力するかどうか
	jobBase.m_informationSheet = m_PrintConditionData.migrationData.JobBaseFlushingPage.informationSheet;

	// フラッシングページを挿入するかどうか
	jobBase.m_flushingPage = (m_PrintConditionData.migrationData.JobBaseFlushingPage.flushingPageType == DEF_FLUSHING_PAGE_USER? true : false);

	// フラッシングページのパターン名
	jobBase.m_patternName = m_PrintConditionData.migrationData.JobBaseFlushingPage.flushingPage;

	// フラッシングページの挿入間隔
	jobBase.m_interval = m_PrintConditionData.migrationData.JobBaseFlushingPage.interval;

	// ジョブ前に挿入するかどうか
	jobBase.m_preJob = m_PrintConditionData.migrationData.JobBaseFlushingPage.beforeJob;

	// ジョブ前に挿入する回数
	jobBase.m_preJobCount = m_PrintConditionData.migrationData.JobBaseFlushingPage.printCountOfBeforeJob;

	// ジョブ後に挿入するかどうか
	jobBase.m_postJob = m_PrintConditionData.migrationData.JobBaseFlushingPage.afterJob;

	// フラッシングページのオフセット（表面・副走査）
	jobBase.m_offsetFront = static_cast<long>(m_PrintConditionData.migrationData.JobBaseFlushingPage.offset[0] * 1000);

	// フラッシングページのオフセット（裏面・副走査）
	jobBase.m_offsetBack = static_cast<long>(m_PrintConditionData.migrationData.JobBaseFlushingPage.offset[1] * 1000);

	// ジョブ前のクリーニング判定レベル
	jobBase.m_cleaningJudgmentLevelBeforeJob = static_cast<long>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.CleaningJudgmentLevelBeforeJob);
	
	// ジョブ後のクリーニング判定レベル
	jobBase.m_cleaningJudgmentLevelAfterJob = static_cast<long>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.CleaningJudgmentLevelAfterJob);
	
	// 印刷停止設定
	jobBase.m_printStopSetting = static_cast<EPrintStopSetting>(m_PrintConditionData.migrationData.JobBaseCleaningSetting.PrintStopSetting);

	outJobBase = jobBase;

}

// レイアウト情報の取得
bool PrintCondition::GetLayout(SLayout& outLayout) const
{
	auto getCutterMarkAlign = [](const DEF_POSTPROCESS_ALIGNMENT inAliment, long& outCutterMarkAlign)
	{
		switch(inAliment)
		{
		case DEF_POSTPROCESS_LEFT:
			 outCutterMarkAlign = -1;
			 break;

		case DEF_POSTPROCESS_NONE:
			 outCutterMarkAlign = 0;
			 break;

		case DEF_POSTPROCESS_RIGHT:
			 outCutterMarkAlign = 1;
			 break;

		// 上記以外の場合
		default:
			throw std::runtime_error("Not compatible CutterMarkerOffset");
		}
	};

	auto getPatternType = [](const DEF_FLUSHING_PATTERN inPattern, std::string& outStr)
	{
		//有効なフラッシングパターン種類の一覧を作成する
		std::map<DEF_FLUSHING_PATTERN, std::string> patternTypeList;
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_NONE, "None"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_1, "Line1"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_2, "Line2"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_4, "Line4"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_8, "Line8"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_16, "Line16"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_1, "Star1"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_2, "Star2"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_4, "Star4"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_STAR_8, "Star8"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_1_STAR, "Line1+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_2_STAR, "Line2+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_4_STAR, "Line4+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_8_STAR, "Line8+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_16_STAR, "Line16+Star"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_32, "Line32"));
		patternTypeList.insert(std::make_pair(DEF_FLUSHING_PATTERN_LINE_32_STAR, "Line32+Star"));

		auto target = patternTypeList.find(inPattern);
		if(target == patternTypeList.end())
		{
			//存在しないフラッシングパターンの場合はエクスポートできない
			std::stringstream errorMessage;
			errorMessage << "Not compatible FlusingPage (Input : " << inPattern << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outStr = target->second;
	};

	auto getLineOptions = [](const ST_JOB_FLUSHING_PATTERN_ADD& inLayoutFlushingPatternAdd, std::string& outStr)
	{
		// オプション
		std::string optionStr;
		switch(inLayoutFlushingPatternAdd.lineOption)
		{
		case DEF_LINE_OPTION_C0:
			optionStr = "C0-";
			break;
		case DEF_LINE_OPTION_NONE:
			optionStr = "NONE-";
			break;
		case DEF_LINE_OPTION_S0:
			optionStr = "S0-";
			break;
		case DEF_LINE_OPTION_S2:
			optionStr = "S2-";
			break;
		default:
			//存在しないオプションの場合はエクスポートできない
			std::stringstream errorMessage;
			errorMessage << "Not compatible LineOptions (Option : " << inLayoutFlushingPatternAdd.lineOption << ")";
			throw std::runtime_error(errorMessage.str());
		}

		// オプション2
		std::string optionStr2;
		switch(inLayoutFlushingPatternAdd.lineOption2)
		{
		case DEF_LINE_OPTION2_STANDARD:
			optionStr2 = "STANDARD";
			break;
		case DEF_LINE_OPTION2_CROSS_STITCH:
			optionStr2 = "CROSS_STITCH";
			break;
		default:
			//存在しないオプション2の場合はエクスポートできない
			std::stringstream errorMessage;
			errorMessage << "Not compatible LineOptions (Option2 : " << inLayoutFlushingPatternAdd.lineOption2 << ")";
			throw std::runtime_error(errorMessage.str());
		}

		outStr = optionStr + optionStr2;
	};

	try
	{
		SLayout layout;

		//画像のオフセット（表面・主副）
		layout.m_offsetFront.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[0].x * 1000);
		layout.m_offsetFront.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[0].y * 1000);

		//画像のオフセット（裏面・主副）
		layout.m_offsetBack.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[1].x * 1000);
		layout.m_offsetBack.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutImagePosition.offset[1].y * 1000);

		//サイド検査をするかどうか
		layout.m_verification = (m_PrintConditionData.migrationData.LayoutVerifyMark.enableVerify == TRUE? true : false);

		//エラーにする連続シート数
		layout.m_continuousPageCount = m_PrintConditionData.migrationData.LayoutVerifyMark.errorSheets;

		//印刷開始マークを出力するかどうか
		layout.m_cueMark = (m_PrintConditionData.migrationData.LayoutVerifyMark.printStartMark == TRUE? true : false);

		//バーコードを出力するかどうか
		layout.m_barcode = (m_PrintConditionData.migrationData.LayoutVerifyMark.verifyMark == TRUE? true : false);

		//バーコードのオフセット（表面・主走査）
		layout.m_barcodeOffsetFront.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetX[0] * 1000);
		layout.m_barcodeOffsetFront.second = 0; //副走査は0固定

		//バーコードのオフセット（裏面・主副）
		layout.m_barcodeOffsetBack.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetX[1] * 1000);
		layout.m_barcodeOffsetBack.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutVerifyMark.offsetY * 1000);

		//フラッシングパターン
		std::string patternType;
		getPatternType(m_PrintConditionData.migrationData.LayoutFlushingPattern.flushingPattern, patternType);
		layout.m_flushingPattern = patternType;
		layout.m_flushingPatternDef = m_PrintConditionData.migrationData.LayoutFlushingPattern.flushingPattern;

		// ラインフラッシングのずらし量
		layout.m_gapBetweenColors = m_PrintConditionData.migrationData.LayoutFlushingPattern.gap;

		// ラインフラッシングのオフセット（表面・副走査）
		layout.m_lineOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPattern.offsetY[0] * 1000);

		// ラインフラッシングのオフセット（裏面・副走査）
		layout.m_lineOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPattern.offsetY[1] * 1000);

		// ページ内、ラインフラッシング
		layout.m_LineGap = (TRUE == m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.enableMultiLine) ? true : false;

		// ページ内、ラインフラッシング間隔（表面・副走査）
		layout.m_LineGapFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.position2ndLine[0] * 1000);

		// ページ内、ラインフラッシング間隔（裏面・副走査）
		layout.m_LineGapBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.position2ndLine[1] * 1000);

		// ページ内、ラインフラッシングの補正オフセット（表面・副走査）
		layout.m_LineGapOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0] * 1000);

		// ページ内、ラインフラッシングの補正オフセット（裏面・副走査）
		layout.m_LineGapOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[1] * 1000);

		// ページ内、ラインフラッシングの表示位置("Top" or "Bottom")
		if(TRUE == m_PrintConditionData.migrationData.LayoutVertivalAlignmentPPFLSPTN)
		{
			layout.m_LinePosition  = "Bottom";
		}
		else
		{
			layout.m_LinePosition  = "Top";
		}		

		// ページ内、ラインフラッシングのオプション情報
		getLineOptions(m_PrintConditionData.migrationData.LayoutFlushingPatternAdd, layout.m_LineOptions);

		// カッターマークの印刷面
		layout.m_cutterMark = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.side);

		// カッターマークの準備シート数
		layout.m_cutterMarkPageCount = m_PrintConditionData.migrationData.LayoutPostProcess.sheets;

		// カッターマークのサイズ（主副）
		layout.m_cutterMarkSize.first = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.size.cx * 1000);
		layout.m_cutterMarkSize.second = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.size.cy * 1000);

		// カッターマークのオフセット（表面・主走査）
		layout.m_cutterMarkOffsetFront = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.offsetX * 1000);

		//カッターマークのオフセット（裏面・主走査）
		layout.m_cutterMarkOffsetBack = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcess.offsetX_back * 1000);

		// カッターマークオフセットの基準位置（表面）
		long cutterMarkAlign_Flont = 0;
		getCutterMarkAlign(m_PrintConditionData.migrationData.LayoutPostProcess.alignment, cutterMarkAlign_Flont);
		layout.m_cutterMarkAlignFront = cutterMarkAlign_Flont;

		// カッターマークオフセットの基準位置（裏面）
		long cutterMarkAlign_Back = 0;
		getCutterMarkAlign(m_PrintConditionData.migrationData.LayoutPostProcess.alignment_back, cutterMarkAlign_Back);
		layout.m_cutterMarkAlignBack = cutterMarkAlign_Back;

		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		layout.m_adjustmentSheetLength = static_cast<long>(m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetLength * 1000);
		layout.m_afterJobSheets = m_PrintConditionData.migrationData.LayoutPostProcessAdd.afterJobSheets;
		layout.m_adjustmentSheetAfterJob = m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob;
		layout.m_adjustmentSheetSizeEnabled = static_cast<bool>(m_PrintConditionData.migrationData.LayoutPostProcessAdd.adjustmentSheetSizeEnabled);
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

		outLayout = layout;
		return true;
	}
	catch(std::exception& e)
	{
		// 対応していないパターンの場合
		writeErrorMessage(e.what());
		return false;
	}
}

// トーンカーブ情報取得
bool PrintCondition::GetToneCurve(SToneCurve& outToneCurve, bool inIsFront) const
{
	try
	{
		AnchorPoint anchorPoint;
		OneColorPoint oneColorPoint;
		SToneCurve toneCurve;

		// 表面・裏面の区別
		std::string side;

		 // 相手機かどうか
		long isPart = 0;

		getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isPart);

		toneCurve.m_side = side;

		// トーンカーブ名
		toneCurve.m_descriptiveName = m_PrintConditionData.migrationData.DotgainCurve[isPart].DOTGAIN_NAME;

		// アンカーポイント情報4色×5点分
		for(int color = ANCHOR_COL_BLACK; color <= ANCHOR_COL_CYAN; ++color)
		{
			std::string colorName;
			switch(color)
			{
			case ANCHOR_COL_BLACK:
				colorName = "K";
				break;
			case ANCHOR_COL_YELLOW:
				colorName = "Y";
				break;
			case ANCHOR_COL_MAGENTA:
				colorName = "M";
				break;
			case ANCHOR_COL_CYAN:
				colorName = "C";
				break;
			}

			// アンカーポイント情報
			for(int pointID = 0; pointID < AnchorPointNum; ++pointID)
			{
				// 基準濃度値
				anchorPoint.m_beforeConPoint = static_cast<long>(m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].POSITION.x);

				// カーブデータの数値
				anchorPoint.m_afterConPoint = static_cast<long>(m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].POSITION.y);

				// 編集タイプ
				anchorPoint.m_anchorPointEditType = (FALSE == m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].EDIT_TYPE) ? 0 : 1;

				// アンカーポイント種別(0:表示のみ, 1:カーブ描画)
				if(FALSE == m_PrintConditionData.migrationData.DotgainCurve[isPart].ANCHOR_POINT[color][pointID].KIND)
				{
					anchorPoint.m_anchorPointType = 0;
				}
				else
				{
					anchorPoint.m_anchorPointType = 1;
				}

				// 1色分のデータ
				oneColorPoint.point[pointID] = anchorPoint;
			}
			toneCurve.anchorPoint.insert(std::make_pair(colorName, oneColorPoint));
		}
		outToneCurve = toneCurve;
		return true;
	}
	catch(std::exception& e)
	{
		// 対応していないパターンの場合
		writeErrorMessage(e.what());
		return false;
	}
}

// JetInspection情報の取得
void PrintCondition::GetJetInspection(SJetInspection& outJetInspection) const
{
	// 検査条件名
	outJetInspection.m_ParamaterName = m_PrintConditionData.migrationData.JICondition;
	// 一時マスクファイルパス
	//V3.40JD014T5 2019/09/19 DB記載ファイルが実在しない場合は空文字列に変換
	std::string TempMaskFile = m_PrintConditionData.migrationData.TemporalMaskFile;
	if( !TempMaskFile.empty() )
	{
		std::string TempMaskFullPath = GetModuleDirectory() + TempMaskFile;
		if( !PathFileExists(TempMaskFullPath.c_str()) )	TempMaskFile.clear();
	}
	outJetInspection.m_TemporalMaskFilePath = TempMaskFile;
}

// JetInspectionエリア情報の取得
bool PrintCondition::GetJetInspectionArea(SJetInspectionArea& outJetInspectionArea, bool inIsFront) const
{
	try
	{
		std::string side;	// 表面・裏面の区別
		long isPart = 0;	// 相手機かどうか
		// 印刷面を取得する
		getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isPart);

		// 検査設定、対象面
		outJetInspectionArea.m_side = side;
		// 非検査エリア左端
		outJetInspectionArea.m_NonInspectionAreaLeft = m_PrintConditionData.migrationData.NonInspectionLeft[isPart];
		// 非検査エリア右端
		outJetInspectionArea.m_NonInspectionAreaRight = m_PrintConditionData.migrationData.NonInspectionRight[isPart];

		long index = 1;
		for(auto itID = m_PrintConditionData.migrationData.NonInspectionArea[isPart].begin(); itID != m_PrintConditionData.migrationData.NonInspectionArea[isPart].end(); ++itID)
		{
			SJetInspectionNonInspectionRect NonInspectionRect;

			// 非検査矩形エリアID
			NonInspectionRect.m_ID = index;
			// 非検査矩形エリア開始位置
			NonInspectionRect.m_StartPos.first = itID->START_POS_X;
			NonInspectionRect.m_StartPos.second = itID->START_POS_Y;
			// 非検査矩形エリア終了位置
			NonInspectionRect.m_EndPos.first = itID->END_POS_X;
			NonInspectionRect.m_EndPos.second = itID->END_POS_Y;

			outJetInspectionArea.m_NonInspectionRect.push_back(NonInspectionRect);
			++index;
		}

		return true;
	}
	catch(std::exception& e)
	{
		// 対応していないパターンの場合
		writeErrorMessage(e.what());
		return false;
	}
}

// JetInspectionデコード情報の取得
bool PrintCondition::GetJetInspectionDecode(SJetInspectionDecode& outJetInspectionDecode, bool inIsFront) const
{
	// 数値単位系の文字列を取得する
	auto getUnitStr = [](const DEF_LENGTH_UNIT inUnit, std::string& outUnitStr)
	{
		std::map<DEF_LENGTH_UNIT, std::string> unitStrList;
		unitStrList.insert(std::make_pair(DEF_LENGTH_MM, "mm"));
		unitStrList.insert(std::make_pair(DEF_LENGTH_INCH, "inch"));
		unitStrList.insert(std::make_pair(DEF_LENGTH_POINT, " point"));

		auto target = unitStrList.find(inUnit);
		if(target == unitStrList.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Not compatible JetInspection Decode Unit (Input : " << inUnit << ")";
			throw std::runtime_error(errorMessage.str());
		}
		outUnitStr = target->second;
	};

	// ラベル種類の文字列を取得する
	auto getLabelStr = [](DEF_DECODE_LABEL_TYPE inLabelType, UNI_DECODE_PATTERN inDecodePattern, std::string& outLabelName)
	{
		switch(inLabelType)
		{
		case DEF_DECODE_OCR:
			switch(inDecodePattern.pat_OCR)
			{
			case DEF_OCR_NUMBER:
				outLabelName = "OCR_Number";
				break;
			case DEF_OCR_TEXT:
				outLabelName = "OCR_Text";
				break;
			case DEF_OCR_PRIORITY_NUMBER:
				outLabelName = "OCR_PriorityNumber";
				break;
			case DEF_OCR_PRIORITY_TEXT:
				outLabelName = "OCR_PriorityText";
				break;
			}
			break;
		case DEF_DECODE_1D_BARCODE:
			switch(inDecodePattern.pat_1D)
			{
			case DEF_1D_CODE39:
				outLabelName = "1D_CODE39";
				break;
			case DEF_1D_CODE128:
				outLabelName = "1D_CODE128";
				break;
			case DEF_1D_NW7:
				outLabelName = "1D_NW7";
				break;
			case DEF_1D_CODE39_CD:
				outLabelName = "1D_CODE39CD";
				break;
			}
			break;
		case DEF_DECODE_2D_BARCODE:
			switch(inDecodePattern.pat_2D)
			{
			case DEF_2D_QR_CODE:
				outLabelName = "2D_QrCode";
				break;
			case DEF_2D_DATA_MATRIX:
				outLabelName = "2D_DataMatrix";
				break;
			case DEF_2D_PDF417:
				outLabelName = "2D_Pdf417";
				break;
			}
			break;
		}

		if(outLabelName.empty())
		{
			throw std::runtime_error("Not compatible JetInspectionDecode LabelName");
		}
	};

	// 角度情報の文字列を取得する
	auto getAngleStr = [](DEF_OCR_ANGLE inAngle, std::string& outAngbleName)
	{
		switch(inAngle)
		{
		case DEF_OCR_ANGLE_0:
			outAngbleName = "0";
			break;
		case DEF_OCR_ANGLE_90:
			outAngbleName = "90";
			break;
		case DEF_OCR_ANGLE_180:
			outAngbleName = "180";
			break;
		case DEF_OCR_ANGLE_270:
			outAngbleName = "270";
			break;
		}

		if(outAngbleName.empty())
		{
			throw std::runtime_error("Not compatible JetInspectionDecode AngleName");
		}
	};

	DEF_LENGTH_UNIT unit = UserModeGUI_GetLengthUnit();
	std::string side;	// 表面・裏面の区別
	long isDummy = 0;
	// 印刷面を取得する
	getSide(CDataSystemSetting::GetInstance()->IsDED(), inIsFront, side, isDummy);


	// 「デコード」タブ、エラー停止連続シート数
	outJetInspectionDecode.m_ErrorSheets = m_PrintConditionData.migrationData.Decode.errorSheets;

	// 数値単位系
	getUnitStr(unit, outJetInspectionDecode.m_Unit);

	// デコード処理の有効／無効
	outJetInspectionDecode.m_IsEnabled = (TRUE == m_PrintConditionData.migrationData.Decode.enableDecode) ? true : false;

	try
	{
		if(inIsFront)
		{
			for(auto itFront = m_PrintConditionData.migrationData.Decode.decodeDetailF.begin(); itFront != m_PrintConditionData.migrationData.Decode.decodeDetailF.end(); ++itFront)
			{
				SJetInspectionDecodeDetails tmpFrontDecodeDetails;

				// 対象面（表or裏）
				tmpFrontDecodeDetails.m_Side = side;

				// サイズ設定
				tmpFrontDecodeDetails.m_Size.first = itFront->size.cx;
				tmpFrontDecodeDetails.m_Size.second = itFront->size.cy;

				// 位置設定
				tmpFrontDecodeDetails.m_Offset.first = itFront->offset.x;
				tmpFrontDecodeDetails.m_Offset.second = itFront->offset.y;

				// ラベル名称
				tmpFrontDecodeDetails.m_Name = itFront->name;

				// ラベル種類
				getLabelStr(itFront->labelType, itFront->pattern, tmpFrontDecodeDetails.m_Type);

				// （OCR限定）角度情報
				getAngleStr(itFront->ocrAngle, tmpFrontDecodeDetails.m_Angle);

				// デコードエラー発生時、エラー停止判定カウントの対象にするか
				tmpFrontDecodeDetails.m_ErrorStop = (TRUE == itFront->checkError) ? true : false;

				outJetInspectionDecode.m_DetailsList.push_back(tmpFrontDecodeDetails);
			}
		}
		else
		{
			for(auto itBack = m_PrintConditionData.migrationData.Decode.decodeDetailB.begin(); itBack != m_PrintConditionData.migrationData.Decode.decodeDetailB.end(); ++itBack)
			{
				SJetInspectionDecodeDetails tmpFrontDecodeDetails;

				// 対象面（表or裏）
				tmpFrontDecodeDetails.m_Side = side;

				// サイズ設定
				tmpFrontDecodeDetails.m_Size.first = itBack->size.cx;
				tmpFrontDecodeDetails.m_Size.second = itBack->size.cy;

				// 位置設定
				tmpFrontDecodeDetails.m_Offset.first = itBack->offset.x;
				tmpFrontDecodeDetails.m_Offset.second = itBack->offset.y;

				// ラベル名称
				tmpFrontDecodeDetails.m_Name = itBack->name;

				// ラベル種類
				getLabelStr(itBack->labelType, itBack->pattern, tmpFrontDecodeDetails.m_Type);

				// （OCR限定）角度情報
				getAngleStr(itBack->ocrAngle, tmpFrontDecodeDetails.m_Angle);

				// デコードエラー発生時、エラー停止判定カウントの対象にするか
				tmpFrontDecodeDetails.m_ErrorStop = (TRUE == itBack->checkError) ? true : false;

				outJetInspectionDecode.m_DetailsList.push_back(tmpFrontDecodeDetails);
			}
		}
	}
	catch(std::exception& e)
	{
		// 対応していないパターンの場合
		writeErrorMessage(e.what());
		return false;
	}

	return true;
}

// 変倍情報の取得
bool PrintCondition::GetVariableMagnification(ST_PDB_RIP_MAGNIFICATION& outVariableMagnification) const
{
	// 変倍値の有効範囲をチェックする
	auto checkRange = [](long inValue)->bool
	{
		if((10000 > inValue) || (1000000 <= inValue))
		{
			return false;
		}
		return true;
	};

	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.FRONT_X)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.FRONT_Y)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.BACK_X)) return false;
	if(!checkRange(m_PrintConditionData.migrationData.RipMagnification.BACK_Y)) return false;

	outVariableMagnification = m_PrintConditionData.migrationData.RipMagnification;
	return true;
}

// エラー内容の書き込み
void PrintCondition::writeErrorMessage(const std::string& inError) const
{
	// 対応していないパターンの場合
	std::stringstream log;
	log << "ERROR:" << inError << "  [DescriptiveName:" << m_PrintConditionData.printConditionName << "]";
	WriteToLogBuf(LOG_DEBUG, log.str());
}

// 印刷面の取得
void PrintCondition::getSide(const bool inIsDED, const bool inIsFront, std::string& outSide, long& outIsPart) const
{
	// SES構成の時
	if(false == inIsDED)
	{
		if(true == inIsFront)
		{
			outSide = "Front";
			outIsPart = 0;
		}else
		{
			// SESに裏面は存在しない
			std::stringstream errorMessage;
			errorMessage << "Not compatible Side (Input : Back)";
			throw std::runtime_error(errorMessage.str());
		}
	}
	// DED構成の時
	else
	{
		if(true == inIsFront)
		{
			outSide = "Front";
			outIsPart = 1;

		}else
		{
			outSide = "Back";
			outIsPart = 0;
		}
	}
}

}//namespace export_xml