/*! \file WriteTextFile.cpp
 *  \brief WriteTextFile.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/03/11 pt.ju-tanaka 新規作成
 *  \date 2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "ExportWriteTextFile.h"
#include "ProcPluginCBRProc.h"
#include "ResDef.h"
#include "Utility.h"
#include "ConvertMD5Hash.h"
#include "DataSystemSetting.h"
#include "Stub_PrinterStateIF.h"
#include "DataSystemSetting.h"

#include <sstream>
#include <fstream>
#include <iomanip>

static char* getDecodeKind(DEF_DECODE_LABEL_TYPE type, UNI_DECODE_PATTERN pat)
{
	char* ret_val;

	ret_val = "";
	switch( type )
	{
	case DEF_DECODE_OCR:
		{
			switch( pat.pat_OCR )
			{
			case DEF_OCR_NUMBER:
				ret_val = "OCR_Number";
				break;
			case DEF_OCR_TEXT:
				ret_val = "OCR_Text";
				break;
			case DEF_OCR_PRIORITY_NUMBER:
				ret_val = "OCR_PrinorityNumber";
				break;
			case DEF_OCR_PRIORITY_TEXT:
				ret_val = "OCR_PrinorityText";
				break;
			}
		}
		break;
	case DEF_DECODE_1D_BARCODE:
		{
			switch( pat.pat_1D )
			{
			case DEF_1D_CODE39:
				ret_val = "1D_CODE39";
				break;
			case DEF_1D_CODE128:
				ret_val = "1D_CODE128";
				break;
			case DEF_1D_NW7:
				ret_val = "1D_NW7";
				break;
			case DEF_1D_CODE39_CD:
				ret_val = "1D_CODE39CD";
				break;
			}
		}
		break;
	case DEF_DECODE_2D_BARCODE:
		{
			switch( pat.pat_2D )
			{
			case DEF_2D_QR_CODE:
				ret_val = "2D_QrCode";
				break;
			case DEF_2D_DATA_MATRIX:
				ret_val = "2D_DataMatrix";
				break;
			case DEF_2D_PDF417:
				ret_val = "2D_Pdf417";
				break;
			}
		}
	}
	return ret_val;
}

// ==========================================================================

/**
 * @brief	ExportWriteTextFileが使うAPI
**/
class ExportWriteTextFile::API
{
public:
	/**
	 * @brief	DEDかどうか
	 * @retval	true : DEDである
	 * @retval	false : DEDではない
	**/
	static bool IsDED()
	{
		auto system = CDataSystemSetting::GetInstance();
		bool ded = system->IsDED();

		return ded;
	}

	/**
	 * @brief	JI接続設定
	 * @retval	0 : 未接続、1 : デコードのみ、2 : フルスペック
	**/
	static long getJIoption()
	{
		auto system = CDataSystemSetting::GetInstance();
		long retval = system->GetJetInspectionOption();

		return retval;
	}

	/**
	 * @brief	印刷モードメニュー名の取得
	 * @retval	true :  リソースID有効
	 * @retval	false : リソースID無効
	**/
	static bool getPrintModeMenuName(const char* inMode, std::string& outName)
	{
		auto system = CDataSystemSetting::GetInstance();
		bool retval = system->GetPrintModeMenuName(inMode, outName);

		return retval;
	}

};

//コンストラクタ
ExportWriteTextFile::ExportWriteTextFile()
{
}

//デストラクタ
ExportWriteTextFile::~ExportWriteTextFile()
{
}

//テキストファイル出力イベント
bool ExportWriteTextFile::onWriteTextFile(
	const std::string& inExportFilePath, 
	const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	std::string bufStr;
	//文字列追加。
	auto addStr = [&](long inResID, const std::string& inValue)->void
	{
		std::stringstream outStr;
		std::string label;
		label = LoadResourceString(inResID, RESOURCE_STR);
		outStr << label << ":" << inValue << "\n";
		bufStr += outStr.str();
	};

	//文字列追加（書式指定付きラベル対応）。
	auto addStr2 = [&](long inResID, long cntr, const char* inValue)->void
	{
		std::stringstream outStr;
		std::string label, str;
		char format[256] = {0};
		char format2[256] = {0};
		label = LoadResourceString(inResID, RESOURCE_STR);
		_snprintf(format, sizeof(format), label.c_str(), cntr);
		label = format;
		_snprintf(format2, sizeof(format2), ":%s\n", inValue);
		str = format2;

		outStr << label << str;
		bufStr += outStr.str();
	};

	//文字列追加（書式指定付きラベル対応）。
	auto addStr1 = [&](long inResID, long cntr, const std::string& inValue)->void
	{
		std::stringstream outStr;
		std::string label;
		char format[256] = {0};
		label = LoadResourceString(inResID, RESOURCE_STR);
		_snprintf(format, sizeof(format), label.c_str(), cntr);
		label = format;
		outStr << label << ":" << inValue << "\n";
		bufStr += outStr.str();
	};


	//エラー発生
	auto execption = [&](const std::string& inStr)->void
	{
		long retExp = 1;
		std::stringstream ss;
		ss << "failed :" << inStr;
		WriteDebugLog(ss.str());
		throw(retExp);
	};

	//////////////////////////////////
	bool ret = false;

	//各項目を指定の書式に変換してファイルに書き込む
		//※DEDとSESで出力項目が変わる点に注意
	std::string value;
	try
	{
		//エクスポートファイルを中間フォルダ内に作成する
		std::ofstream file(inExportFilePath.c_str(), std::ios_base::out);
		if (nullptr == file)
		{
			//ファイル出力失敗。
			std::stringstream ss;
			ss << "std::ofstream[" << getLastError()  << "]";
			execption(ss.str());
		}

		//用紙種類
		addStr(IDS_PARAM_PAPER_TYPE, inMigrationData.PaperType);

		//紙厚
		addStr(IDS_PARAM_PAPERTHICKNESS, changeTo02f((double)inMigrationData.Thickness * 0.001));
		
		//用紙幅
		addStr(IDS_PARAM_PAGEWIDTH, changeTo02f((double)inMigrationData.PaperSizeW * 0.001));

		//ページ送りサイズ
		addStr(IDS_PARAM_PAPER_FEED, getPaperFeed(inMigrationData.PaperSizeH));

		//ページ送りサイズの単位
		addStr(IDS_PARAM_PAPER_FEED_UNIT, getPaperFeedUnit(inMigrationData.PaperSizeH.UNIT));

		//段差設定
		addStr(IDS_PARAM_PRINT_TIMING, inMigrationData.LevelTuneName);


		if( true == API::getPrintModeMenuName( inMigrationData.PrintMode.RESOURCE_ID, value) )
		{
			//印刷モード
			addStr(IDS_PARAM_PRINT_MODE, value);
		}
		//印刷速度
		addStr(IDS_PARAM_PRINT_SPEED, changeToD((inMigrationData.SpeedUnit == DEF_MODE_RESO_SPEED_100) ? inMigrationData.PrintSpeed/100 : inMigrationData.PrintSpeed));

		//テンション
		addStr(IDS_PARAM_TENSION, changeToD(inMigrationData.Tension));

		//DNS
		addStr(IDS_PARAM_DNS, getTrueOrFalse(inMigrationData.DNSStatus));
		
		//パンチ
		addStr(IDS_PARAM_PAPER_OP_PUNCH, getPaperPunch(inMigrationData.PaperOption));

		//ミシン目
		addStr(IDS_PARAM_PAPER_OP_PERFORATION, getTrueOrFalse(inMigrationData.PaperOption.IS_HOLES_PERFORATIONS));

		//マーク
		addStr(IDS_PARAM_MARK, getParamMark(inMigrationData.PaperOption));

		//マークオフセット
		addStr(IDS_PARAM_MARK_OFFSET, changeTo02f((double)inMigrationData.PaperOption.PRE_MARK_OFFSET * 0.001));

		//事前印刷
		addStr(IDS_PARAM_PRE_PRINT, getTrueOrFalse(inMigrationData.PaperOption.IS_PRE_MARK_PAPER));

		//乾燥温度
		if (true == API::IsDED())
		{
			//DEDの場合、表裏設定する
			//表面乾燥温度
			addStr(IDS_PARAM_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_PART));
			//裏面乾燥温度
			addStr(IDS_PARAM_DRYER_TEMP_B, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_SELF));

			//温風アシストファン搭載
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(false) > 0 ) {
				//表面
				addStr(IDS_PARAM_HEAT_FAN_TEMP_F, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_PART));
			}
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(true) > 0 ) {
				//裏面
				addStr(IDS_PARAM_HEAT_FAN_TEMP_B, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_SELF));
			}

			//NIR搭載
			if( Stub_PrinterStateIF::IsUnitOfNir(false) ) {
				//表面
				addStr(IDS_PARAM_IR_POWER_F, changeToD(inMigrationData.DryTempPrint.NIR_POWER_PART));
			}
			if( Stub_PrinterStateIF::IsUnitOfNir(true) ) {
				//裏面
				addStr(IDS_PARAM_IR_POWER_B, changeToD(inMigrationData.DryTempPrint.NIR_POWER_SELF));
			}

			//Ranger搭載
			if( Stub_PrinterStateIF::IsUnitOfRanger() ) {
				//表面サブ乾燥温度
				addStr(IDS_PARAM_SUB_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_PART));
				//裏面サブ乾燥温度
				addStr(IDS_PARAM_SUB_DRYER_TEMP_B, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_SELF));
			}
		}
		else
		{
			//表面乾燥温度
			addStr(IDS_PARAM_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.HEAT_TEMP_SELF));

			//温風アシストファン搭載
			if( Stub_PrinterStateIF::IsLoadingHeaterFan(true) > 0 ) {
				//表面
				addStr(IDS_PARAM_HEAT_FAN_TEMP_F, changeToD(inMigrationData.DryTempPrint.FAN_TEMP_SELF));
			}

			//NIR搭載
			if( Stub_PrinterStateIF::IsUnitOfNir(true) ) {
				//表面
				addStr(IDS_PARAM_IR_POWER_F, changeToD(inMigrationData.DryTempPrint.NIR_POWER_SELF));
			}

			//Ranger搭載
			if( Stub_PrinterStateIF::IsUnitOfRanger() ) {
				//表面サブ乾燥温度
				addStr(IDS_PARAM_SUB_DRYER_TEMP_F, changeToD(inMigrationData.DryTempPrint.SUB_HEAT_TEMP_SELF));
			}
		}

		//トーンカーブ（名称）
		if (true == API::IsDED())
		{
			//DEDの場合、表裏設定する
			//表面
			if (false == getToneCurve(inMigrationData.DotgainCurve[1], value))
			{
				//エラー発生
				execption("CConvertMD5Hash err[front].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_F, value);
			}

			//裏面
			if (false == getToneCurve(inMigrationData.DotgainCurve[0], value))
			{
				//エラー発生
				execption("CConvertMD5Hash err[back].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_B, value);
			}
		}
		else
		{
			//表面
			if (false == getToneCurve(inMigrationData.DotgainCurve[0], value))
			{
				//エラー発生
				execption("CConvertMD5Hash err[front].");
			}
			if (0 != value.size())
			{
				addStr(IDS_PARAM_TONE_CURVE_F, value);
			}
		}

		//インフォメーションシート
		addStr(IDS_PARAM_INFORMATION_SHEET, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.informationSheet ? TRUE : FALSE));

		//フラッシングパターン
		addStr(IDS_PARAM_FLUSHING_PATTERN_CMYK, getFlushingPatternKind(inMigrationData.LayoutFlushingPattern.flushingPattern));

		//フラッシングパターンライン時ずらし量
		if (true == getFlushingPatternOffset(inMigrationData.LayoutFlushingPattern, value))
		{
			//Line選択時のみ、出力。
			addStr(IDS_PARAM_FLUSHING_PATTERN_OFFSET, value);
		}

		//フラッシングパターン、ラインフラッシング、副走査オフセット
		//フラッシングパターンがLine*設定時のみ､項目出力する
		if( true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern) )
		{
			if (true == API::IsDED())
			{
				//DEDの場合、表裏設定する
				// 下端設定の判定
				if(TRUE == inMigrationData.LayoutVertivalAlignmentPPFLSPTN)
				{
					// 下端設定の場合、エクスポート時のラインフラッシングのオフセット値は負の値とする
					//表面
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0] * (-1));
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
					//裏面
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[1] * (-1));
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y_BACK, value);
				}
				else
				{
					// 上端設定の場合、エクスポート時のラインフラッシングのオフセット値は正の値とする
					//表面
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0]);
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
					//裏面
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[1]);
					addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y_BACK, value);
				}
			}
			else
			{
				//表面
				// 下端設定の判定
				if(TRUE == inMigrationData.LayoutVertivalAlignmentPPFLSPTN)
				{
					// 下端設定の場合、エクスポート時のラインフラッシングのオフセット値は負の値とする
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0] * (-1));
				}
				else
				{
					// 上端設定の場合、エクスポート時のラインフラッシングのオフセット値は正の値とする
					value = changeTo02f(inMigrationData.LayoutFlushingPattern.offsetY[0]);
				}

				addStr(IDS_PARAM_FLUSHING_PATTERN_POS_Y, value);
			}
		}

		if((true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern))
			&& (TRUE == inMigrationData.LayoutFlushingPatternAdd2.enableMultiLine))
		{
			//フラッシングパターンがLine*かつ、フラッシングパターン2Lineオプション情報のenableMultiLineがTRUEの場合、
			//下記をそれぞれ設定する

			//ラインフラッシング、複数箇所に出力する
			addStr(IDS_DATA_LINE_FLUSHING_OUTPUT_TO_MULTIPLE, getTrueOrFalse(TRUE));

			if (true == API::IsDED())
			{
				//ラインフラッシング、表面挿入間隔
				std::string frontInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_INSERTION_INTERVAL, frontInsetInterval);

				//ラインフラッシング、表面ページ内補正オフセット
				std::string frontOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_OFFSET, frontOffset);

				//ラインフラッシング、裏面挿入間隔
				std::string backInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[1]);
				addStr(IDS_DATA_LINE_FLUSHING_BACK_INSERTION_INTERVAL, backInsetInterval);

				//ラインフラッシング、裏面ページ内補正オフセット
				std::string backOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[1]);
				addStr(IDS_DATA_LINE_FLUSHING_BACK_OFFSET, backOffset);
			}
			else
			{
				//ラインフラッシング、表面挿入間隔
				std::string frontInsetInterval = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.position2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_INSERTION_INTERVAL, frontInsetInterval);

				//ラインフラッシング、表面ページ内補正オフセット
				std::string frontOffset = changeTo02f(inMigrationData.LayoutFlushingPatternAdd2.offsetY2ndLine[0]);
				addStr(IDS_DATA_LINE_FLUSHING_FRONT_OFFSET, frontOffset);
			}
		}

		if( true == isFlushingPatternLine(inMigrationData.LayoutFlushingPattern.flushingPattern) )
		{
			if( DEF_LINE_OPTION2_CROSS_STITCH == inMigrationData.LayoutFlushingPatternAdd.lineOption2 )
			{
				//フラッシングパターンがLine*かつ、フラッシングパターンLineオプション情報2がDEF_LINE_OPTION2_CROSS_STITCHの場合、
				//ラインフラッシング、ライン種別を"クロスステッチ"に設定する
				addStr(IDS_DATA_LINE_FLUSHING_LINE_TYPE, LoadResourceString(IDS_DATA_CROSS_STICH, RESOURCE_STR));
			}
			addStr(IDS_PARAM_FLUSHING_LINE_OPTIONS, getFlushingPatternOption(inMigrationData.LayoutFlushingPatternAdd));
		}

		if (DEF_FLUSHING_PAGE_NONE == inMigrationData.JobBaseFlushingPage.flushingPageType)
		{
			//フラッシングページ「出力しない」設定時
			addStr(IDS_PARAM_FLUSHING_PAGE, LoadResourceString(IDS_PARAM_FLUSHING_PAGE_NAME_NONE, RESOURCE_STR));
		} else {
			//フラッシングページ印刷のときのみ、詳細項目を出力する。

			//出力パターン
			value = inMigrationData.JobBaseFlushingPage.flushingPage;
			addStr(IDS_PARAM_FLUSHING_PAGE, value);

			//ページ時の間隔
			value = changeToD(inMigrationData.JobBaseFlushingPage.interval);
			addStr(IDS_PARAM_FLUSHING_PAGE_INTERVAL, value);

			//ページ時のジョブ前
			addStr(IDS_PARAM_BEFOREJOB, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.beforeJob ? TRUE : FALSE));

			//ページ時のジョブ前回数
			value = changeToD(inMigrationData.JobBaseFlushingPage.printCountOfBeforeJob);
			addStr(IDS_PARAM_COUNT_BEFOREJOB, value);

			//ページ時のジョブ後
			addStr(IDS_PARAM_AFTERJOB, getTrueOrFalse(inMigrationData.JobBaseFlushingPage.afterJob ? TRUE : FALSE));

			//表面オフセット
			if (true == API::IsDED())
			{
				//DEDの場合、表裏設定する
				//表面
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[0]);
				addStr(IDS_PARAM_OFFSET_F, value);
				//裏面
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[1]);
				addStr(IDS_PARAM_OFFSET_B, value);
			}
			else
			{
				//表面
				value = changeTo02f(inMigrationData.JobBaseFlushingPage.offset[0]);
				addStr(IDS_PARAM_OFFSET_F, value);
			}
		}

		//主/副走査画像レイアウト
		if (true == API::IsDED())
		{
			//DEDの場合、表裏設定する
			//表面
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_Y, value);

			//裏面
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[1].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_BACK_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[1].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_BACK_Y, value);
		}
		else
		{
			//表面
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].x);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_X, value);
			value = changeTo02f(inMigrationData.LayoutImagePosition.offset[0].y);
			addStr(IDS_PARAM_IMAGE_LAYOUT_FRONT_Y, value);
		}

		//サイド検査マーク
		addStr(IDS_PARAM_SIDE_CHECK_MARK, getTrueOrFalse(inMigrationData.LayoutVerifyMark.enableVerify));

		//サイド検査マーク種類
		addStr(IDS_PARAM_SIDE_CHECK_MARK_KIND, getSideCheckMarkKind(inMigrationData.LayoutVerifyMark));

		//エラー連続ページ数
		addStr(IDS_PARAM_ERR_CONS_SHEET, changeToD(inMigrationData.LayoutVerifyMark.errorSheets));

		//主走査サイド検査マークオフセット
		if (true == API::IsDED())
		{
			//DEDの場合、表裏設定する
			//表面
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[0]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_FRONT_X, value);

			//裏面
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[1]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_BACK_X, value);
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetY);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_BACK_Y, value);
		}
		else
		{
			//表面
			value = changeTo02f(inMigrationData.LayoutVerifyMark.offsetX[0]);
			addStr(IDS_PARAM_SIDE_CHECK_MARK_OFFSET_FRONT_X, value);
		}

		//後処理マーク
		addStr(IDS_PARAM_POST_MARK_KIND, getPostMarkKind(inMigrationData.LayoutPostProcess.side));

		//後処理マークサイズ（幅）
		addStr(IDS_PARAM_POST_MARK_SIZE_X, changeTo02f(inMigrationData.LayoutPostProcess.size.cx));

		//後処理マークサイズ（高さ）
		addStr(IDS_PARAM_POST_MARK_SIZE_Y, changeTo02f(inMigrationData.LayoutPostProcess.size.cy));

		//後処理マーク表面オフセット
		addStr(IDS_PARAM_POST_MARK_OFFSET_F, changeTo02f(inMigrationData.LayoutPostProcess.offsetX));

		//後処理マーク裏面オフセット
		if (true == API::IsDED())
		{
			addStr(IDS_PARAM_POST_MARK_OFFSET_B, changeTo02f(inMigrationData.LayoutPostProcess.offsetX_back));
		}

		//後処理マーク準備シート数
		addStr(IDS_PARAM_POST_READY_SHEETS, changeToD(inMigrationData.LayoutPostProcess.sheets));

		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		addStr(IDS_PARAM_ADJUST_SHEETSIZE, getTrueOrFalse(inMigrationData.LayoutPostProcessAdd.adjustmentSheetSizeEnabled ? TRUE : FALSE));
		addStr(IDS_PARAM_ADJUST_SHEETLENGTH, changeTo02f(inMigrationData.LayoutPostProcessAdd.adjustmentSheetLength));
		if ( inMigrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob == SET_SHEET_COUNT )
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_SET, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET, label);
		}
		else if ( inMigrationData.LayoutPostProcessAdd.adjustmentSheetAfterJob == NONE )
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_NOSET, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET , label);
		}
		else
		{
			std::string label = LoadResourceString(IDS_PARAM_ADJUST_AFTERJOBSHEET_STOP, RESOURCE_STR);
			addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET , label);
		}
		addStr(IDS_PARAM_ADJUST_AFTERJOBSHEET_COUNT, changeToD(inMigrationData.LayoutPostProcessAdd.afterJobSheets));
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

		//JetInspection、フルスペック環境
		if( API::getJIoption() == 2 ) {
			//検査条件名
			addStr(IDS_PARAM_INSPECTION_PARAMATER, inMigrationData.JICondition);

			//一時マスクファイル名
			//V3.40JD014T5 2019/09/19 DB記載ファイルが実在しない場合は空文字列で出力
			std::string TempMaskFile = inMigrationData.TemporalMaskFile;
			if( !TempMaskFile.empty() )
			{
				std::string TempMaskFullPath = GetModuleDirectory() + TempMaskFile;
				if( !PathFileExists(TempMaskFullPath.c_str()) )	TempMaskFile.clear();
			}
			addStr(IDS_PARAM_TEMP_MASK_FILEPATH, TempMaskFile);
			
			if (true == API::IsDED())
			{
				//表面非検査エリア左端
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_F, changeTo02f(inMigrationData.NonInspectionLeft[1]));

				//表面非検査エリア右端
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_F, changeTo02f(inMigrationData.NonInspectionRight[1]));

				//裏面非検査エリア左端
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_B, changeTo02f(inMigrationData.NonInspectionLeft[0]));

				//裏面非検査エリア右端
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_B, changeTo02f(inMigrationData.NonInspectionRight[0]));

				if( !inMigrationData.NonInspectionArea.empty() ) {
					size_t area_cnt, f_cnt;
					if( inMigrationData.NonInspectionArea.size() > 1 ) {
						//表面非検査矩形エリア
						area_cnt = inMigrationData.NonInspectionArea[1].size();
						for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
							const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[1][f_cnt];
							addStr1(IDS_PARAM_NONINSPECT_RECT_F, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
						}
					}
					//裏面非検査矩形エリア
					area_cnt = inMigrationData.NonInspectionArea[0].size();
					for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
						const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[0][f_cnt];
						addStr1(IDS_PARAM_NONINSPECT_RECT_B, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
					}
				}
			}
			else
			{
				//表面非検査エリア左端
				addStr(IDS_PARAM_NONINSPECT_AREA_LEFT_F, changeTo02f(inMigrationData.NonInspectionLeft[0]));

				//表面非検査エリア右端
				addStr(IDS_PARAM_NONINSPECT_AREA_RIGHT_F, changeTo02f(inMigrationData.NonInspectionRight[0]));

				//表面非検査矩形エリア
				if( !inMigrationData.NonInspectionArea.empty() ) {
					size_t area_cnt, f_cnt;
					area_cnt = inMigrationData.NonInspectionArea[0].size();
					for( f_cnt = 0; f_cnt < area_cnt; f_cnt++ ) {
						const STRUCT_NON_INSPECTION_AREA& pArea = inMigrationData.NonInspectionArea[0][f_cnt];
						addStr1(IDS_PARAM_NONINSPECT_RECT_F, f_cnt+1, changeTo02f(pArea.START_POS_X, pArea.START_POS_Y, pArea.END_POS_X, pArea.END_POS_Y));
					}
				}
			}
			
			
		}

		//JetInspection デコード有効環境
		if( API::getJIoption() == 2 || Stub_PrinterStateIF::IsOCRandDECEnable() ) {
			//デコード処理
			addStr(IDS_PARAM_DECODE, getTrueOrFalse(inMigrationData.Decode.enableDecode));

			//デコード処理、エラー停止連続シート数
			addStr(IDS_PARAM_DECODE_ERR_CONS_SHEET, changeToD(inMigrationData.Decode.errorSheets));
			
			size_t d_cnt;
			static char strAngle[4][16];
			strcpy(strAngle[0],"0");
			strcpy(strAngle[1],"90");
			strcpy(strAngle[2],"180");
			strcpy(strAngle[3],"270");

			//表面、デコード詳細情報
			auto itr = inMigrationData.Decode.decodeDetailF.begin();
			for( d_cnt = 0; itr != inMigrationData.Decode.decodeDetailF.end(); itr++, d_cnt++ ) {

				//デコード設定､表面%02d、ラベル名
				addStr2(IDS_PARAM_DECODE_LABEL_NAME_F, d_cnt, itr->name);

				//デコード設定､表面%02d、種類
				addStr2(IDS_PARAM_DECODE_TYPE_F, d_cnt, getDecodeKind(itr->labelType,itr->pattern));

				//デコード設定､表面%02d、位置
				addStr1(IDS_PARAM_DECODE_POS_F, d_cnt, changeTo02f(itr->offset.x, itr->offset.y));

				//デコード設定､表面%02d、サイズ
				addStr1(IDS_PARAM_DECODE_SIZE_F, d_cnt, changeTo02f(itr->size.cx, itr->size.cy));

				if(itr->labelType == DEF_DECODE_OCR )
				{
					//デコード設定､表面%02d、角度
					addStr2(IDS_PARAM_DECODE_ANGLE_F, d_cnt, strAngle[itr->ocrAngle]);
				}

				//デコード設定､表面%02d、デコード時のエラー処理
				addStr1(IDS_PARAM_DECODE_ERROR_PROC_F, d_cnt, getTrueOrFalse(itr->checkError));
			}

			if (true == API::IsDED())
			{
				//裏面、デコード詳細情報
				auto itr_b = inMigrationData.Decode.decodeDetailB.begin();
				for( d_cnt = 0; itr_b != inMigrationData.Decode.decodeDetailB.end(); itr_b++, d_cnt++ ) {

					//デコード設定､表面%02d、ラベル名
					addStr2(IDS_PARAM_DECODE_LABEL_NAME_B, d_cnt, itr_b->name);

					//デコード設定､表面%02d、種類
					addStr2(IDS_PARAM_DECODE_TYPE_B, d_cnt, getDecodeKind(itr_b->labelType,itr_b->pattern));

					//デコード設定､表面%02d、位置
					addStr1(IDS_PARAM_DECODE_POS_B, d_cnt, changeTo02f(itr_b->offset.x, itr_b->offset.y));

					//デコード設定､表面%02d、サイズ
					addStr1(IDS_PARAM_DECODE_SIZE_B, d_cnt, changeTo02f(itr_b->size.cx, itr_b->size.cy));

					if(itr_b->labelType == DEF_DECODE_OCR )
					{
						//デコード設定､表面%02d、角度
						addStr2(IDS_PARAM_DECODE_ANGLE_B, d_cnt, strAngle[itr_b->ocrAngle]);
					}

					//デコード設定､表面%02d、デコード時のエラー処理
					addStr1(IDS_PARAM_DECODE_ERROR_PROC_B, d_cnt, getTrueOrFalse(itr_b->checkError));
				}
				
			}
		}

		// 変倍機能使用可の環境
		if(CDataSystemSetting::GetInstance()->IsMagnificationAdjustment())
		{
			// 表裏自動変倍
			addStr(
				IDS_PARAM_RIP_AUTO_MANIFICATION, 
				TRUE == inMigrationData.RipMagnification.IS_ENABLE ? LoadResourceString(IDS_DATA_TRUE, RESOURCE_STR) : LoadResourceString(IDS_DATA_FALSE, RESOURCE_STR));

			// 自動変倍基準面
			addStr(
				IDS_PARAM_RIFERENCE_FACE, 
				TRUE == inMigrationData.RipMagnification.IS_DATUM_PLANE_FRONT ? LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR) : LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR));

			// 表面主走査方向変倍値
			addStr(
				IDS_PARAM_MANIFICATION_FRONT_X, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.FRONT_X) / 100000, 5));

			// 表面副走査方向変倍値
			addStr(
				IDS_PARAM_MANIFICATION_FRONT_Y, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.FRONT_Y) / 100000, 5));

			// 裏面主走査方向変倍値
			addStr(
				IDS_PARAM_MANIFICATION_BACK_X, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.BACK_X) / 100000, 5));

			// 裏面副走査方向変倍値
			addStr(
				IDS_PARAM_MANIFICATION_BACK_Y, 
				convertNumToStringPrecision(static_cast<double>(inMigrationData.RipMagnification.BACK_Y) / 100000, 5));
		}

		// フラッシングページ=有りで
		// ノズル抜けチェック機能が有効 且つ ノズル抜けチェックが可能なフラッシングページが設定されている場合
		std::string flushingPage(inMigrationData.JobBaseFlushingPage.flushingPage);
		if (inMigrationData.JobBaseFlushingPage.flushingPageType == DEF_FLUSHING_PAGE_USER &&
			CDataSystemSetting::GetInstance()->isNozzleCheckEnabled(flushingPage))
		{
			// ジョブ前のクリーニング判定レベル
			addStr(
				IDS_PARAM_CLEANING_JUDGMENT_LEVEL_BEFORE_JOB,
				changeToD(inMigrationData.JobBaseCleaningSetting.CleaningJudgmentLevelBeforeJob));

			// ジョブ後のクリーニング判定レベル
			addStr(
				IDS_PARAM_CLEANING_JUDGMENT_LEVEL_AFTER_JOB,
				changeToD(inMigrationData.JobBaseCleaningSetting.CleaningJudgmentLevelAfterJob));

			// 印刷停止設定
			addStr(
				IDS_PARAM_PRINT_STOP_SETTING,
				getPrintStopSettingKind(inMigrationData.JobBaseCleaningSetting.PrintStopSetting));
		}

		//ファイルに書き込む。
		try
		{
			file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
			file << bufStr << std::flush;
		}
		catch (const std::ofstream::failure& /*e*/)
		{
			//エラー発生
			execption("WriteFile err[failure].");
		}
		catch (...)
		{
			//エラー発生
			execption("WriteFile err[bad].");
		}
		ret = true;
	}
	catch (long /*exp*/) {
		ret = false;
	}

	return ret;
}

//GetLastErrorの文字列取得
std::string ExportWriteTextFile::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

//数値を「"%0.2f"」書式の文字列で返す
std::string ExportWriteTextFile::changeTo02f(double inVal)
{
	std::string value;
	char format[32] = {0};

	_snprintf(format, sizeof(format), "%0.2f", roundD(inVal, 2));
	value = format;
	return value;
}

//数値を「"%0.2f"」書式の文字列で返す
std::string ExportWriteTextFile::changeTo02f(double sx, double sy, double ex, double ey)
{
	std::string value;
	char format[128] = {0};

	_snprintf(format, sizeof(format), "%0.2f, %0.2f, %0.2f, %0.2f", roundD(sx, 2), roundD(sy, 2), roundD(ex, 2), roundD(ey, 2));
	value = format;
	return value;
}

//数値を「"%0.2f"」書式の文字列で返す
std::string ExportWriteTextFile::changeTo02f(double x, double y)
{
	std::string value;
	char format[64] = {0};

	_snprintf(format, sizeof(format), "%0.2f, %0.2f", roundD(x, 2), roundD(y, 2));
	value = format;
	return value;
}

//数値->文字列変換(string)して返す
std::string ExportWriteTextFile::convertNumToStringPrecision(double inValue, UINT inPrecision)
{
	std::stringstream streaam;
	streaam << std::fixed << std::setprecision(inPrecision) << inValue;
	return streaam.str();
}

//数値を「"%d"」書式の文字列で返す
std::string ExportWriteTextFile::changeToD(long inVal)
{
	std::string value;
	char format[32] = {0};
	_snprintf(format, sizeof(format), "%d", static_cast<int>(inVal));
	value = format;
	return value;
}

//ページ送りサイズ文字列取得
std::string ExportWriteTextFile::getPaperFeed(const ST_PDB_PAPER_SIZE_H& inPaperSizeH)
{
	std::string value;
	char format[32] = {0};
	double dVal = 0.0;
	if (TRUE == inPaperSizeH.IS_SIZE_H)
	{
		dVal = inPaperSizeH.SIZE_H * 0.001;
	}
	switch (inPaperSizeH.UNIT)
	{
		case UNIT_MODE_MM:
			_snprintf(format, sizeof(format), "%0.1f", roundD(dVal, 1));
			break;
		case UNIT_MODE_INCH:
			_snprintf(format, sizeof(format), "%0.2f", roundD(dVal, 2));
			break;
		case UNIT_1PER2_INCH:
		case UNIT_1PER6_INCH:
			_snprintf(format, sizeof(format), "%0.0f", roundD(dVal, 0));
			break;
	}
	value = format;
	return value;
}

//ページ送りサイズの単位文字列取得
std::string ExportWriteTextFile::getPaperFeedUnit(const eUNIT_MODE inUnitMode)
{
	std::string value;
	switch (inUnitMode)
	{
		case UNIT_MODE_MM:
			value = LoadResourceString(IDS_DATA_UNIT_MM, RESOURCE_STR);
			break;
		case UNIT_MODE_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_INCH, RESOURCE_STR);
			break;
		case UNIT_1PER2_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_1P2INCH, RESOURCE_STR);
			break;
		case UNIT_1PER6_INCH:
			value = LoadResourceString(IDS_DATA_UNIT_1P6INCH, RESOURCE_STR);
			break;
	}
	return value;
}

//パンチ文字列取得
std::string ExportWriteTextFile::getPaperPunch(const ST_PDB_PAPER_OPTION& inPaperOption)
{
	std::string value;
	if (TRUE == inPaperOption.IS_HOLES
		|| TRUE == inPaperOption.IS_HOLES_PERFORATIONS)
	{	//画面上の「ミシン」がONの場合もONにする。（DNP蕨 NX課題 No.50）
		value = getTrueOrFalse(TRUE);
	}
	else
	{
		value = getTrueOrFalse(FALSE);
	}
	return value;
}

//ラインフラッシング、オプション文字列取得
std::string ExportWriteTextFile::getFlushingPatternOption(const ST_JOB_FLUSHING_PATTERN_ADD& inOption)
{
	std::string value;
	switch( inOption.lineOption ) {
	case DEF_LINE_OPTION_C0:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_C0_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_C0_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_NONE:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_NONE_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_NONE_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_S0:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S0_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S0_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	case DEF_LINE_OPTION_S2:
		{
			switch( inOption.lineOption2 ) {
			case DEF_LINE_OPTION2_STANDARD:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S2_STANDARD, RESOURCE_STR);
				}
				break;
			case DEF_LINE_OPTION2_CROSS_STITCH:
				{
					value = LoadResourceString(IDS_DATA_FLUSHING_OPTION_S2_CROSS_STITCH, RESOURCE_STR);
				}
				break;
			}
		}
		break;
	}
	
	return value;
}

//「true」「false」文字列取得
std::string ExportWriteTextFile::getTrueOrFalse(BOOL inVal)
{
	std::string value;
	if (TRUE == inVal)
	{
		value = LoadResourceString(IDS_DATA_TRUE, RESOURCE_STR);
	}
	else
	{
		value = LoadResourceString(IDS_DATA_FALSE, RESOURCE_STR);
	}
	return value;
}

//事前印刷マーク文字列取得
std::string ExportWriteTextFile::getParamMark(const ST_PDB_PAPER_OPTION& inPaperOption)
{
	std::string value;
	if (TRUE == inPaperOption.IS_PRE_MARK)
	{
		//事前印刷マーク有り
		if (DEF_PRINT_BACK == inPaperOption.PRE_MARK_SIDE)
		{
			//裏面
			value = LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR);
		}
		else
		{
			//表面
			value = LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR);
		}
	}
	else
	{
		//事前印刷マーク無し
		value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//トーンカーブ（名称）文字列取得
bool ExportWriteTextFile::getToneCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve, std::string& outVal)
{
	std::stringstream sStr;
	std::string Hash;

	if (0 == strlen(inDotgainCurve.DOTGAIN_NAME))
	{
		//トーンカーブ名が未設定（デフォルト）の場合
		outVal = "";
		return true;
	}
	if (false == CConvertMD5Hash::CreateHash(
		(BYTE*)inDotgainCurve.ANCHOR_POINT,
		sizeof(inDotgainCurve.ANCHOR_POINT),
		Hash))
	{
			//エラー発生
			return false;
	}
	sStr << inDotgainCurve.DOTGAIN_NAME << "[" << Hash << "]";
	outVal = sStr.str();
	return true;
}

//フラッシングパターン文字列取得
std::string ExportWriteTextFile::getFlushingPatternKind(DEF_FLUSHING_PATTERN inFlushingPatternKind)
{
	std::string value;
	switch (inFlushingPatternKind) 
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE1, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_2:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE2, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_4:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE4, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_8:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE8, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_16:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE16, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_32:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE32, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_1:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR1, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_2:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR2, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_4:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR4, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_STAR_8:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_STAR8, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE1_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE2_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE4_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE8_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE16_STAR, RESOURCE_STR);
			break;
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			value = LoadResourceString(IDS_DATA_FLUSHING_PATTERN_LINE32_STAR, RESOURCE_STR);
			break;
		default:
			value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
			break;
	}
	return value;
}

//フラッシングパターンライン時ずらし量文字列取得
bool ExportWriteTextFile::getFlushingPatternOffset(const ST_JOB_FLUSHING_PATTERN& inFlushingPattern, std::string& outVal)
{
	switch (inFlushingPattern.flushingPattern)
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
		case DEF_FLUSHING_PATTERN_LINE_2:
		case DEF_FLUSHING_PATTERN_LINE_4:
		case DEF_FLUSHING_PATTERN_LINE_8:
		case DEF_FLUSHING_PATTERN_LINE_16:
		case DEF_FLUSHING_PATTERN_LINE_32:
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			//Line選択時のみ、出力。
			outVal = changeToD(inFlushingPattern.gap);
			return true;
	}
	return false;
}

//サイド検査マーク種類
std::string ExportWriteTextFile::getSideCheckMarkKind(const ST_JOB_VERIFY_MARK& inSideCheckMarkKind)
{
	std::string value;
	if (TRUE == inSideCheckMarkKind.verifyMark
		&& TRUE == inSideCheckMarkKind.printStartMark)
	{
		//サイド検査マークと印字開始マーク
		value = LoadResourceString(IDS_DATA_CHECK_MARK_PRINT_BARCODE, RESOURCE_STR);
	}
	else if (FALSE == inSideCheckMarkKind.verifyMark
		&& TRUE == inSideCheckMarkKind.printStartMark)
	{
		//印刷開始マーク
		value = LoadResourceString(IDS_DATA_CHECK_MARK_PRINT, RESOURCE_STR);
	}
	else
	{
		//マークなし
		value = LoadResourceString(IDS_DATA_CHECK_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//後処理マーク種類文字列取得
std::string ExportWriteTextFile::getPostMarkKind(DEF_SELECT_SIDE inSide)
{
	std::string value;
	if (DEF_SELECT_SIDE_BOTH == inSide)
	{
		//両面
		value = LoadResourceString(IDS_DATA_POST_MARK_BOTH, RESOURCE_STR);
	}
	else if (DEF_SELECT_SIDE_BACK == inSide)
	{
		//裏面
		value = LoadResourceString(IDS_DATA_POST_MARK_BACK, RESOURCE_STR);
	}
	else if (DEF_SELECT_SIDE_FRONT == inSide)
	{
		//表面
		value = LoadResourceString(IDS_DATA_POST_MARK_FRONT, RESOURCE_STR);
	}
	else
	{
		//なし
		value = LoadResourceString(IDS_DATA_POST_MARK_NONE, RESOURCE_STR);
	}
	return value;
}

//小数第N位までに計算（小数第N+1位を四捨五入）して返す
double ExportWriteTextFile::roundD(double inSrc, int inLen)
{
	double adjustedValue = 0.5;
	if(inSrc < 0.0)
	{
		// 入力値が負の値の場合
		adjustedValue = (-0.5);
	}

	if (0 == inLen)
	{
		return static_cast<double>(static_cast<int>(inSrc + adjustedValue));
	}
	else
	{
		double srcValue = inSrc * pow(10.0, inLen);
		double destValue = static_cast<double>(static_cast<int>(srcValue + adjustedValue));
		return destValue * pow(10.0, -inLen);
	}
}

//フラッシングパターンがLine*かどうか取得
bool ExportWriteTextFile::isFlushingPatternLine(DEF_FLUSHING_PATTERN inFlushingPatternKind)
{
	bool isFlushingPatternLine = false;

	switch (inFlushingPatternKind)
	{
		case DEF_FLUSHING_PATTERN_LINE_1:
		case DEF_FLUSHING_PATTERN_LINE_2:
		case DEF_FLUSHING_PATTERN_LINE_4:
		case DEF_FLUSHING_PATTERN_LINE_8:
		case DEF_FLUSHING_PATTERN_LINE_16:
		case DEF_FLUSHING_PATTERN_LINE_32:
		case DEF_FLUSHING_PATTERN_LINE_1_STAR:
		case DEF_FLUSHING_PATTERN_LINE_2_STAR:
		case DEF_FLUSHING_PATTERN_LINE_4_STAR:
		case DEF_FLUSHING_PATTERN_LINE_8_STAR:
		case DEF_FLUSHING_PATTERN_LINE_16_STAR:
		case DEF_FLUSHING_PATTERN_LINE_32_STAR:
			isFlushingPatternLine = true;
			break;
		default:
			isFlushingPatternLine = false;
			break;
	}
	return isFlushingPatternLine;
}

// 印刷停止設定文字列取得
std::string ExportWriteTextFile::getPrintStopSettingKind(const EPrintStopSetting& inPrintStopSettingKind)
{
	std::string value;
	switch (inPrintStopSettingKind)
	{
	case kStopIfMissingNozzle:
		value = LoadResourceString(IDS_DATA_STOP_PRINTING, RESOURCE_STR);
		break;
	case kNotStopIfMissingNozzle:
		value = LoadResourceString(IDS_DATA_KEEP_PRINTING, RESOURCE_STR);
		break;
	default:
		break;
	}
	return value;
}
