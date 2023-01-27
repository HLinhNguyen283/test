/*! \file	ExportXML.cpp
 *  \brief	ExportXML.cppの処理
 *  \author	pt.ju-tanaka
 *  \date	2019/06/26 pt.ju-tanaka 新規作成
 *  \date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
*/

#include "stdafx.h"
#include "CommonCBRProc.h"
#include "fileText.h"
#include "DataSystemSetting.h"
#include "ExportXML.h"
#include "Stub_PrinterStateIF.h"

namespace export_xml
{
 //コンストラクタ
ExportFile::ExportFile()
{
}

// デストラクタ
ExportFile::~ExportFile()
{
}

// XML形式での印刷条件エクスポート依頼
bool ExportFile::onRequestExportXML(
	const text::tstring& inOutputPath,
	const export_xml::PrintCondition& inPrintConditionList)
{
	bool isXmlExportLog = CDataSystemSetting::GetInstance()->IsDebugXmlExportLog();
	std::unique_ptr<xml_utility::XMLElementControl> elementXML;
	elementXML.reset(new xml_utility::XMLElementControl(_T("<PrintConditions>")));

	try
	{
		std::unique_ptr<xml_utility::XMLElementControl> elementXMLSub;
		text::tstring elementPath = _T("PrintCondition");
		elementXMLSub.reset(new xml_utility::XMLElementControl(_T("<") + elementPath + _T(">")));

		// 印刷条件名の設定
		SetPrintConditionName(inPrintConditionList, elementPath, elementXMLSub);

		// 用紙種類の設定
		SetPaperType(inPrintConditionList, elementPath, elementXMLSub);

		// 段差補正名の設定
		SetAlignment(inPrintConditionList, elementPath, elementXMLSub);

		// 印刷モードの設定
		SetPrintMode(inPrintConditionList, elementPath, elementXMLSub);

		// 印刷速度の設定
		SetPrintSpeed(inPrintConditionList, elementPath, elementXMLSub);

		// 用紙設定情報の設定
		SetPaperSetting(inPrintConditionList, elementPath, elementXMLSub);

		// 乾燥温度情報
		SetDryerTemperature(inPrintConditionList, elementPath, elementXMLSub);

		// ICCプリセット情報の設定
		SetICCPreset(inPrintConditionList, elementPath, elementXMLSub);

		// トーンカーブ情報の設定
		SetToneCurve(inPrintConditionList, elementPath, elementXMLSub);

		// ジョブ基本設定情報情報の設定
		SetJobBase(inPrintConditionList, elementPath, elementXMLSub);

		// レイアウト情報の設定
		SetLayout(inPrintConditionList, elementPath, elementXMLSub);

		if(2 == CDataSystemSetting::GetInstance()->GetJetInspectionOption())
		{
			// JI関連情報の設定(JetInspectorとの接続されていればエレメント出力対象とする)
			SetJI(inPrintConditionList, elementPath, elementXMLSub);

			// JIエリア情報の設定
			SetJIArea(inPrintConditionList, elementPath, elementXMLSub);
		}

		if((Stub_PrinterStateIF::IsOCRandDECEnable()) || (2 == CDataSystemSetting::GetInstance()->GetJetInspectionOption()))
		{
			// JIデコード情報の設定
			SetJIDecode(inPrintConditionList, elementPath, elementXMLSub);
		}

		if(CDataSystemSetting::GetInstance()->IsMagnificationAdjustment())
		{
			// 変倍情報の設定
			SetVariableMagnification(inPrintConditionList, elementPath, elementXMLSub);
		}

		elementXML->addElement(elementXMLSub);

		if(isXmlExportLog)
		{
			// XMLエクスポートログ出力が有効時
			std::stringstream exportlog;
			exportlog << "XML Exportable" << " Name:[" << inPrintConditionList.GetPrintConditionName() << "]";
			WriteToLogBuf(LOG_DEBUG, exportlog.str());
		}
	}
    catch(std::exception& e)
    {
        std::stringstream errorlog;
		errorlog << "XML Export failure Name:[" << inPrintConditionList.GetPrintConditionName() << "] Method:[" << e.what() << "]";
		WriteToLogBuf(LOG_DEBUG, errorlog.str());

		return false;
    }

	// XMLファイルへ出力
	outputXML(inOutputPath, elementXML);

	return true;
}

// 印刷条件名の設定
void ExportFile::SetPrintConditionName(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 印刷条件名を取得しXMLデータに設定
	outElement->setValue(_T(inElementPath + "/DescriptiveName"), inPrintCondition.GetPrintConditionName());
}

// 用紙種類の設定
void ExportFile::SetPaperType(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 用紙種類を取得しXMLデータに設定
	outElement->setValue(_T(inElementPath + "/PaperType"), inPrintCondition.GetPaperType());
}

// 段差補正名の設定
void ExportFile::SetAlignment(
	const export_xml::PrintCondition& inPrintCondition, 
	const text::tstring& inElementPath, 
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 段差補正名を取得しXMLデータに設定
	outElement->setValue(inElementPath + _T("/Alignment"), inPrintCondition.GetLevelTuneName());
}

// 印刷モードの設定
void ExportFile::SetPrintMode(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 印刷モードの取得
	std::string printMode;
	if(!inPrintCondition.GetPrintMode(printMode))
	{
		// 印刷モード取得失敗時は処理終了
		throw std::runtime_error("SetPrintMode");
	}

	// 印刷モードをXMLデータに設定
	outElement->setValue(inElementPath + _T("/PrintMode"), printMode);
}

// 印刷速度の設定
void ExportFile::SetPrintSpeed(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 印刷速度を取得しXMLデータに設定
	outElement->setValue(inElementPath + _T("/PrintSpeed"), text::convertNumToString<double>(inPrintCondition.GetPrintSpeed()));
}

// 用紙設定情報の設定
void ExportFile::SetPaperSetting(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 用紙設定情報を取得
	SPaperSetting paperSetting;
	if(!inPrintCondition.GetPaperSetting(paperSetting))
	{
		// 用紙設定情報取得失敗時は処理終了
		throw std::runtime_error("SetPaperSetting");
	}

	text::tstring paperSettingPath = inElementPath + _T("/PaperSetting");

	// 用紙幅をXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/Width"), text::convertNumToStringPrecision<double>(paperSetting.m_width,4));

	// 用紙厚みをXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/Thickness"), text::convertNumToString<long>(paperSetting.m_thickness));

	// テンションをXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/Tension"), text::convertNumToString<long>(paperSetting.m_tension));

	// DNSをXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/DNS"), true == paperSetting.m_DNSStatus ? _T("true") : _T("false"));

	// パンチ穴用紙をXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/PunchHole"), true == paperSetting.m_punchHole ? _T("true") : _T("false"));

	// ミシン目用紙をXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/Sewing"), true == paperSetting.m_sewing ? _T("true") : _T("false"));

	// 事前マークをXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/Marker"), true == paperSetting.m_marker ? _T("true") : _T("false"));

	// 事前印刷をXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/PrePrinted"), true == paperSetting.m_prePrinted ? _T("true") : _T("false"));

	if(paperSetting.m_marker)
	{
		// マークオフセット(主走査)をXMLデータに設定(事前マークがtrueの場合のみエレメント出力する)
		outElement->setValue(paperSettingPath + _T("/MarkerOffset"), text::convertNumToStringPrecision<double>(paperSetting.m_markerOffset,4));
	}

	// ページ送りサイズをXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/PageFeedSize"), text::convertNumToStringPrecision<double>(paperSetting.m_pageFeedSize,4));

	// ページ送りサイズの単位をXMLデータに設定
	outElement->setValue(paperSettingPath + _T("/PageFeedSizeUnit"), paperSetting.m_pageFeedSizeUnit);

	if(paperSetting.m_marker)
	{
		// マークの印刷面をXMLデータに設定(事前マークがtrueの場合のみエレメント出力する)
		outElement->setValue(paperSettingPath + _T("/MarkSide"), paperSetting.m_markSide);
	}
}

// 乾燥温度情報の設定
void ExportFile::SetDryerTemperature(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 乾燥温度情報を取得
	SDryerTemperture dryerTemperture;
	inPrintCondition.GetDryerTemperature(dryerTemperture);
	text::tstring dryerTemperaturePath = inElementPath + _T("/DryerTemperature");

	// ヒートローラー乾燥温度(自機)をXMLデータに設定
	outElement->setValue(dryerTemperaturePath + _T("/HeatRollerSelf"), text::convertNumToString<long>(dryerTemperture.m_heatRollerSelf));

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// ヒートローラー乾燥温度(相手機)をXMLデータに設定(DED構成の場合のみエレメント出力する)
		outElement->setValue(dryerTemperaturePath + _T("/HeatRollerPart"), text::convertNumToString<long>(dryerTemperture.m_heatRollerPart));
	}

	long heaterFanSelf = Stub_PrinterStateIF::IsLoadingHeaterFan(true);
	long heaterFanPart = Stub_PrinterStateIF::IsLoadingHeaterFan(false);
	if((0 != heaterFanSelf) || (0 != heaterFanPart))
	{
		// 温風アシスト乾燥温度(自機)をXMLデータに設定(自機or相手機のどちらか1つでも温風ファン搭載であればエレメント出力する))
		outElement->setValue(dryerTemperaturePath + _T("/HeatFanSelf"), text::convertNumToString<long>(dryerTemperture.m_heatFanSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// 温風アシスト乾燥温度(相手機)をXMLデータに設定(DED構成の場合エレメント出力する)
			outElement->setValue(dryerTemperaturePath + _T("/HeatFanPart"), text::convertNumToString<long>(dryerTemperture.m_heatFanPart));
		}
	}

	bool isUnitOfNirSelf = Stub_PrinterStateIF::IsUnitOfNir(true);
	bool isUnitOfNirPart = Stub_PrinterStateIF::IsUnitOfNir(false);
	if((isUnitOfNirSelf) || (isUnitOfNirPart))
	{
		// NIR設定(自機)をXMLデータに設定(自機or相手機のどちらか1つでもNIR搭載であればエレメント出力する))
		outElement->setValue(dryerTemperaturePath + _T("/NIRPowerSelf"), text::convertNumToString<long>(dryerTemperture.m_NirPowerSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// NIR設定(相手機)をXMLデータに設定(DED構成の場合エレメント出力する)
			outElement->setValue(dryerTemperaturePath + _T("/NIRPowerPart"), text::convertNumToString<long>(dryerTemperture.m_NirPowerPart));
		}
	}

	bool isPreCoat = Stub_PrinterStateIF::IsLoadingPreCoat();
	if(isPreCoat)
	{
		// 先塗り乾燥温度１をXMLデータに設定(プレコート搭載であればエレメント出力する))
		outElement->setValue(dryerTemperaturePath + _T("/Precoater1"), text::convertNumToString<long>(dryerTemperture.m_precoater1));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// NIR設定(相手機)をXMLデータに設定(DED構成の場合エレメント出力する)
			outElement->setValue(dryerTemperaturePath + _T("/Precoater2"), text::convertNumToString<long>(dryerTemperture.m_precoater2));
		}
	}

	bool isUnitOfRanger = Stub_PrinterStateIF::IsUnitOfRanger();
	if(isUnitOfRanger)
	{
		// サブヒートローラー乾燥温度(自機)をXMLデータに設定
		outElement->setValue(dryerTemperaturePath + _T("/SubHeatRollerSelf"), text::convertNumToString<long>(dryerTemperture.m_SubHeatRollerSelf));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// サブヒートローラー乾燥温度(相手機)をXMLデータに設定(DED構成の場合のみエレメント出力する)
			outElement->setValue(dryerTemperaturePath + _T("/SubHeatRollerPart"), text::convertNumToString<long>(dryerTemperture.m_SubHeatRollerPart));
		}
	}
}

// ICCプリセット情報の設定
void ExportFile::SetICCPreset(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ICCプリセット情報を取得
	SICCPreset iccPreset;
	inPrintCondition.GetICCPreset(iccPreset);
	text::tstring iccPresetPath = inElementPath + _T("/ICCPreset");

	// ICCプリセット名をXMLデータに設定
	outElement->setValue(iccPresetPath + _T("/DescriptiveName"), iccPreset.m_descriptiveName);

	// ICCプリセットIDをXMLデータに設定
	outElement->setValue(iccPresetPath + _T("/ID"), iccPreset.m_presetID);
}

// トーンカーブ情報の設定
void ExportFile::SetToneCurve(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// アンカーポイント情報設定処理
	auto setAnchorPoint = [](
		std::unique_ptr<xml_utility::XMLElementControl>& outElement,
		const export_xml::PrintCondition& inPrintCondition,
		const text::tstring& inElementPath,
		const SToneCurve& inToneCurve,
		const text::tstring& inToneCurvePath)
	{
		text::tstring colorRootPath = inToneCurvePath + _T("/AnchorPoint");
		// PartIDKeysをXMLデータに設定
		outElement->setValue(colorRootPath + _T("/PartIDKeys"), _T("Color PointID"));

		// 各色のアンカーポイント情報を設定
		for(auto itr = inToneCurve.anchorPoint.begin(); itr != inToneCurve.anchorPoint.end(); ++itr)
		{
			size_t index = std::distance(inToneCurve.anchorPoint.begin(), itr);
			text::tstring colorPath = inToneCurvePath + _T("/AnchorPoint/AnchorPoint<") + text::convertNumToTString<size_t>(index) + _T(">");

			// 色名をXMLデータに設定
			outElement->setValue(colorPath + _T("/Color"), itr->first);

			// アンカーポイント情報を設定
			for(int cnt = 0; cnt < AnchorPointNum; ++cnt)
			{
				text::tstring anchorPointPath = colorPath + _T("/AnchorPoint<") + text::convertNumToTString<size_t>(cnt) + _T(">");

				// Point IDをXMLデータに設定
				outElement->setValue(_T(anchorPointPath + "/PointID"), text::convertNumToString<int>(cnt));

				// 基準濃度値をXMLデータに設定
				outElement->setValue(_T(anchorPointPath + "/BeforeConPoint"), text::convertNumToString<long>(itr->second.point[cnt].m_beforeConPoint));

				// カーブデータの数値をXMLデータに設定
				outElement->setValue(_T(anchorPointPath + "/AfterConPoint"), text::convertNumToString<long>(itr->second.point[cnt].m_afterConPoint));

				// 編集タイプをXMLデータに設定
				outElement->setValue(_T(anchorPointPath + "/AnchorPointEditType"), text::convertNumToString<long>(itr->second.point[cnt].m_anchorPointEditType));

				// カーブデータの符号をXMLデータに設定
				outElement->setValue(_T(anchorPointPath + "/AnchorPointType"), text::convertNumToString<long>(itr->second.point[cnt].m_anchorPointType));
			}
		}
	};

	//	メイン処理

	// 表面のトーンカーブ情報を取得
	SToneCurve toneCurveFront;
	if(!inPrintCondition.GetToneCurve(toneCurveFront, true))
	{
		// トーンカーブ情報の取得失敗時
		return;
	}

	text::tstring toneCurveFrontRootPath = inElementPath + _T("/ToneCurve");
	// PartIDKeysをXMLデータに設定
	outElement->setValue(toneCurveFrontRootPath + _T("/PartIDKeys"), _T("Side"));

	text::tstring toneCurveFrontPath = inElementPath + _T("/ToneCurve/ToneCurve<") + text::convertNumToTString<int>(0) + _T(">");

	// 表面・裏面の区別をXMLデータに設定
	outElement->setValue(toneCurveFrontPath + _T("/Side"), toneCurveFront.m_side);
	// 表面のトーンカーブ名をXMLデータに設定
	outElement->setValue(toneCurveFrontPath + _T("/DescriptiveName"), toneCurveFront.m_descriptiveName);
	// 表面の各色のアンカーポイント情報を設定
	setAnchorPoint(outElement, inPrintCondition, inElementPath, toneCurveFront, toneCurveFrontPath);


	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// 裏面のトーンカーブ情報を取得
		SToneCurve toneCurveBack;
		if(!inPrintCondition.GetToneCurve(toneCurveBack, false))
		{
			// トーンカーブ情報の取得失敗時
			return;
		}
		text::tstring toneCurveBackPath = inElementPath + _T("/ToneCurve/ToneCurve<") + text::convertNumToTString<int>(1) + _T(">");

		// 表面・裏面の区別をXMLデータに設定
		outElement->setValue(toneCurveBackPath + _T("/Side"), toneCurveBack.m_side);
		// 裏面のトーンカーブ名をXMLデータに設定
		outElement->setValue(toneCurveBackPath + _T("/DescriptiveName"), toneCurveBack.m_descriptiveName);
		// 裏面の各色のアンカーポイント情報を設定
		setAnchorPoint(outElement, inPrintCondition, inElementPath, toneCurveBack, toneCurveBackPath);
	}
}

// ジョブ基本設定情報の設定
void ExportFile::SetJobBase(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// ジョブ基本設定情報を取得
	SJobBase jobBase;
	inPrintCondition.GetJobBase(jobBase);
	text::tstring jobBasePath = inElementPath + _T("/JobBase");

	// インフォメーションシートの出力有無をXMLデータに設定
	outElement->setValue(jobBasePath + _T("/InformationSheet"), true == jobBase.m_informationSheet ? _T("true") : _T("false"));

	// フラッシングページの挿入有無をXMLデータに設定
	outElement->setValue(jobBasePath + _T("/FlushingPage"), true == jobBase.m_flushingPage ? _T("true") : _T("false"));

	if(jobBase.m_flushingPage)
	{
		// フラッシングページのパターン名をXMLデータに設定(フラッシングページの挿入有の場合のみエレメント出力する)
		outElement->setValue(jobBasePath + _T("/PatternName"), jobBase.m_patternName);

		// フラッシングページの挿入間隔をXMLデータに設定(フラッシングページの挿入有の場合のみエレメント出力する)
		outElement->setValue(jobBasePath + _T("/Interval"), text::convertNumToString<long>(jobBase.m_interval));
	}

	// ジョブ前の挿入有無をXMLデータに設定
	outElement->setValue(jobBasePath + _T("/PreJob"), true == jobBase.m_preJob ? _T("true") : _T("false"));

	if(jobBase.m_preJob)
	{
		// ジョブ前に挿入する回数をXMLデータに設定(ジョブ前に挿入する設定の場合のみエレメント出力する)
		outElement->setValue(jobBasePath + _T("/PreJobCount"), text::convertNumToString<long>(jobBase.m_preJobCount));
	}

	// ジョブ後の挿入有無をXMLデータに設定
	outElement->setValue(jobBasePath + _T("/PostJob"), true == jobBase.m_postJob ? _T("true") : _T("false"));

	if(jobBase.m_flushingPage)
	{
		// フラッシングページのオフセット(表面・副走査)をXMLデータに設定(フラッシングページの挿入有の場合のみエレメント出力する)
		outElement->setValue(jobBasePath + _T("/OffsetFront"), text::convertNumToString<long>(jobBase.m_offsetFront));
	}

	if((jobBase.m_flushingPage) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// フラッシングページのオフセット(裏面・副走査)をXMLデータに設定(フラッシングページの挿入有かつ、DED構成の場合のみエレメント出力する)
		outElement->setValue(jobBasePath + _T("/OffsetBack"), text::convertNumToString<long>(jobBase.m_offsetBack));
	}

	// フラッシングページ=有りで
	// ノズル抜けチェック機能が有効 且つ ノズル抜けチェックが可能なフラッシングページが設定されている場合
	if (jobBase.m_flushingPage &&
		CDataSystemSetting::GetInstance()->isNozzleCheckEnabled(jobBase.m_patternName))
	{
		// ジョブ前のクリーニング判定レベル
		outElement->setValue(jobBasePath + _T("/CleaningJudgmentLevelBeforeJob"), text::convertNumToString<long>(jobBase.m_cleaningJudgmentLevelBeforeJob));
		// ジョブ後のクリーニング判定レベル
		outElement->setValue(jobBasePath + _T("/CleaningJudgmentLevelAfterJob"), text::convertNumToString<long>(jobBase.m_cleaningJudgmentLevelAfterJob));
		// 印刷停止設定
		outElement->setValue(jobBasePath + _T("/PrintStopSetting"), jobBase.m_printStopSetting == kStopIfMissingNozzle ? _T("Stop") : _T("NotStop"));
	}
}

// レイアウト情報の設定
void ExportFile::SetLayout(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// レイアウト情報を取得
	SLayout layout;
	if(!inPrintCondition.GetLayout(layout))
	{
		// レイアウト情報取得失敗時は処理終了
		throw std::runtime_error("SetLayout");
	}

	text::tstring layoutPath = inElementPath + _T("/Layout");

	// 画像のオフセット(表面・主副)をXMLデータに設定
	setXYpairParameter(outElement, layoutPath + _T("/OffsetFront"), layout.m_offsetFront.first, layout.m_offsetFront.second);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// 画像のオフセット(裏面・主副)をXMLデータに設定(DED構成の場合のみエレメント出力する)
		setXYpairParameter(outElement, layoutPath + _T("/OffsetBack"), layout.m_offsetBack.first, layout.m_offsetBack.second);
	}

	// サイド検査の有無をXMLデータに設定
	outElement->setValue(layoutPath + _T("/Verification"), true == layout.m_verification ? _T("true") : _T("false"));

	if(layout.m_verification)
	{
		// エラーにする連続シート数をXMLデータに設定(サイド検査をする設定の場合のみエレメント出力する)
		outElement->setValue(layoutPath + _T("/ContinuousPageCount"), text::convertNumToString<long>(layout.m_continuousPageCount));
	}

	// 印刷開始マークの出力有無をXMLデータに設定
	outElement->setValue(layoutPath + _T("/CueMark"), true == layout.m_cueMark ? _T("true") : _T("false"));

	// バーコードの出力有無をXMLデータに設定
	outElement->setValue(layoutPath + _T("/Barcode"), true == layout.m_barcode ? _T("true") : _T("false"));

	if(layout.m_barcode)
	{
		// バーコードのオフセット(表面・主走査。副走査は0 固定)をXMLデータに設定(バーコード出力する設定の場合のみエレメント出力する)
		setXYpairParameter(outElement, layoutPath + _T("/BarcodeOffsetFront"), layout.m_barcodeOffsetFront.first, layout.m_barcodeOffsetFront.second);
	}

	if((layout.m_barcode) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// バーコードのオフセット(裏面・主副)をXMLデータに設定(バーコード出力する設定かつ、DED構成の場合のみエレメント出力する)
		setXYpairParameter(outElement, layoutPath + _T("/BarcodeOffsetBack"), layout.m_barcodeOffsetBack.first, layout.m_barcodeOffsetBack.second);
	}

	// フラッシングパターンをXMLデータに設定
	outElement->setValue(layoutPath + _T("/FlushingPattern"), layout.m_flushingPattern);

	// ラインフラッシングが設定されているか判断する
	if(isLineFlushing(layout.m_flushingPatternDef))
	{
		// ラインフラッシング設定時の場合に各エレメント出力する

		// ラインフラッシングのずらし量をXMLデータに設定
		outElement->setValue(layoutPath + _T("/GapBetweenColors"), text::convertNumToString<long>(layout.m_gapBetweenColors));

		// ラインフラッシングのオフセット(表面・副走査)をXMLデータに設定
		outElement->setValue(layoutPath + _T("/LineOffsetFront"), text::convertNumToString<long>(layout.m_lineOffsetFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// ラインフラッシングのオフセット(裏面・副走査)をXMLデータに設定
			outElement->setValue(layoutPath + _T("/LineOffsetBack"), text::convertNumToString<long>(layout.m_lineOffsetBack));
		}

		// ページ内、ラインフラッシングをXMLデータに設定
		outElement->setValue(layoutPath + _T("/LineGap"), true == layout.m_LineGap ? _T("true") : _T("false"));

		// ページ内、ラインフラッシング間隔（表面・副走査）をXMLデータに設定
		outElement->setValue(layoutPath + _T("/LineGapFront"), text::convertNumToString<long>(layout.m_LineGapFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// ページ内、ラインフラッシング間隔（裏面・副走査）をXMLデータに設定
			outElement->setValue(layoutPath + _T("/LineGapBack"), text::convertNumToString<long>(layout.m_LineGapBack));
		}

		// ページ内、ラインフラッシングの補正オフセット（表面・副走査）をXMLデータに設定
		outElement->setValue(layoutPath + _T("/LineGapOffsetFront"), text::convertNumToString<long>(layout.m_LineGapOffsetFront));

		if(CDataSystemSetting::GetInstance()->IsDED())
		{
			// ページ内、ラインフラッシングの補正オフセット（裏面・副走査）をXMLデータに設定
			outElement->setValue(layoutPath + _T("/LineGapOffsetBack"), text::convertNumToString<long>(layout.m_LineGapOffsetBack));
		}

		// ページ内、ラインフラッシングのオプション情報をXMLデータに設定
		outElement->setValue(layoutPath + _T("/LineOptions"), layout.m_LineOptions);
	}

	// ページ内、ラインフラッシングの表示位置をXMLデータに設定
	outElement->setValue(layoutPath + _T("/LinePosition"), layout.m_LinePosition);

	// カッターマークの印刷面をXMLデータに設定
	outElement->setValue(layoutPath + _T("/CutterMark"), text::convertNumToString<long>(layout.m_cutterMark));

	if(0 != layout.m_cutterMark)
	{
		// CutterMarkがoff(0)以外の場合に各エレメント出力する

		// カッターマークの準備シート数をXMLデータに設定
		outElement->setValue(layoutPath + _T("/CutterMarkPageCount"), text::convertNumToString<long>(layout.m_cutterMarkPageCount));

		// カッターマークのサイズ(主副)をXMLデータに設定
		setXYpairParameter(outElement, layoutPath + _T("/CutterMarkSize"), layout.m_cutterMarkSize.first, layout.m_cutterMarkSize.second);

		// カッターマークのオフセット(表面・主走査)をXMLデータに設定
		outElement->setValue(layoutPath + _T("/CutterMarkOffsetFront"), text::convertNumToString<long>(layout.m_cutterMarkOffsetFront));
	}

	if((0 != layout.m_cutterMark) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// カッターマークのオフセット(裏面・主走査)をXMLデータに設定(CutterMarkがoff(0)以外かつ、DED構成の場合にエレメント出力する)
		outElement->setValue(layoutPath + _T("/CutterMarkOffsetBack"), text::convertNumToString<long>(layout.m_cutterMarkOffsetBack));
	}

	if(0 != layout.m_cutterMark)
	{
		// カッターマークオフセットの基準位置(表面)をXMLデータに設定(CutterMarkがoff(0)以外の場合にエレメント出力する)
		outElement->setValue(layoutPath + _T("/CutterMarkAlignFront"), text::convertNumToString<long>(layout.m_cutterMarkAlignFront));
	}

	if((0 != layout.m_cutterMark) && (CDataSystemSetting::GetInstance()->IsDED()))
	{
		// カッターマークオフセットの基準位置(裏面)をXMLデータに設定(CutterMarkがoff(0)以外かつ、DED構成の場合にエレメント出力する)
		outElement->setValue(layoutPath + _T("/CutterMarkAlignBack"), text::convertNumToString<long>(layout.m_cutterMarkAlignBack));
	}

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	if(0 != layout.m_cutterMark) {

		// 「後処理マーク、調整シートマークサイズ」有効(TRUE)/無効(FALSE)を出力する
		outElement->setValue(layoutPath + _T("/AdjustmentSheetSizeEnabled"), true == layout.m_adjustmentSheetSizeEnabled ? _T("true") : _T("false"));
		if ( layout.m_adjustmentSheetSizeEnabled )
		{	// 「処理マーク、調整シートマークサイズ」が有効の場合に「後処理マーク、調整シートマーク高さ」を出力する
			outElement->setValue(layoutPath + _T("/AdjustmentSheetLength"), text::convertNumToString<long>(layout.m_adjustmentSheetLength));
		}
		// 「ジョブ後のシート数を指定する方法を選択するオプション」を出力する
		outElement->setValue(layoutPath + _T("/AdjustmentSheetAfterJob"), text::convertNumToString<long>(layout.m_adjustmentSheetAfterJob));
		if ( layout.m_adjustmentSheetAfterJob == SET_SHEET_COUNT )
		{	// 「ジョブ後のシート数を指定する方法を選択するオプション」が「シート数設定」の場合に「後処理マーク、ジョブ後シート数」を出力する
			outElement->setValue(layoutPath + _T("/AfterJobSheets"), text::convertNumToString<long>(layout.m_afterJobSheets));
		}
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	}
}

// JI情報の設定
void ExportFile::SetJI(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// JI情報を取得
	SJetInspection jetInspection;
	inPrintCondition.GetJetInspection(jetInspection);

	text::tstring jetInspectionPath = inElementPath + _T("/Inspection");
	// 検査条件名をXMLデータに設定
	outElement->setValue(jetInspectionPath + _T("/ParamaterName"), jetInspection.m_ParamaterName);
	// 一時マスクファイルパスをXMLデータに設定
	outElement->setValue(jetInspectionPath + _T("/TemporalMaskFilePath"), jetInspection.m_TemporalMaskFilePath);
}

// JIエリア情報の設定
void ExportFile::SetJIArea(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	auto setJIArea = [&](bool inIsFront, text::tstring& inJetInspectionAreaFrontPath)
	{
		// JIエリア情報を取得
		SJetInspectionArea jetInspectionArea;
		if(!inPrintCondition.GetJetInspectionArea(jetInspectionArea, inIsFront))
		{
			std::stringstream errorlog;
			errorlog << "ExportFile::SetJIArea failure inIsFront = " << inIsFront;
			WriteToLogBuf(LOG_DEBUG, errorlog.str());
			return;
		}

		// 検査設定、対象面をXMLデータに設定
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/Side"), jetInspectionArea.m_side);
		// 非検査エリア左端
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/NonInspectionAreaLeft"), text::convertNumToTStringPrecision<double>(jetInspectionArea.m_NonInspectionAreaLeft, 2));
		// 非検査エリア右端
		outElement->setValue(inJetInspectionAreaFrontPath + _T("/NonInspectionAreaRight"), text::convertNumToTStringPrecision<double>(jetInspectionArea.m_NonInspectionAreaRight, 2));

		text::tstring nonInspectionRootPath = inJetInspectionAreaFrontPath + _T("/NonInspectionRect");
		// PartIDKeysをXMLデータに設定
		outElement->setValue(nonInspectionRootPath + _T("/PartIDKeys"), _T("ID"));

		for(auto it = jetInspectionArea.m_NonInspectionRect.begin(); it != jetInspectionArea.m_NonInspectionRect.end(); ++it)
		{
			size_t index = std::distance(jetInspectionArea.m_NonInspectionRect.begin(), it);
			text::tstring nonInspectionPath = inJetInspectionAreaFrontPath + _T("/NonInspectionRect/NonInspectionRect<") + text::convertNumToTString<size_t>(index) + _T(">");

			// IDをXMLデータに設定
			outElement->setValue(nonInspectionPath + _T("/ID"), text::convertNumToTString<long>(it->m_ID));
			// 非検査矩形エリア開始位置をXMLデータに設定
			outElement->setValue(
				nonInspectionPath + _T("/StartPos"), 
				text::convertNumToTStringPrecision<double>(it->m_StartPos.first, 2) + _T(" ") + text::convertNumToTStringPrecision<double>(it->m_StartPos.second, 2));
			// 非検査矩形エリア終了位置をXMLデータに設定
			outElement->setValue(
				nonInspectionPath + _T("/EndPos"), 
				text::convertNumToTStringPrecision<double>(it->m_EndPos.first, 2) + _T(" ") + text::convertNumToTStringPrecision<double>(it->m_EndPos.second, 2));
		}		
	};

	text::tstring jetInspectionAreaRootPath = inElementPath + _T("/InspectionArea");
	// PartIDKeysをXMLデータに設定
	outElement->setValue(jetInspectionAreaRootPath + _T("/PartIDKeys"), _T("Side"));

	// 表面のJIエリア情報を設定
	text::tstring jetInspectionAreaFrontPath = inElementPath + _T("/InspectionArea/InspectionArea<")+ text::convertNumToTString<int>(0) + _T(">");
	setJIArea(true, jetInspectionAreaFrontPath);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		// 裏面のJIエリア情報を設定
		text::tstring jetInspectionAreaBackPath = inElementPath + _T("/InspectionArea/InspectionArea<")+ text::convertNumToTString<int>(1) + _T(">");
		setJIArea(false, jetInspectionAreaBackPath);
	}
}

// JIデコード情報の設定
void ExportFile::SetJIDecode(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	auto setJIDecodeDetails = [&](SJetInspectionDecode& inJetInspectionDecode, text::tstring& ioJetInspectionDecodePath, long& ioIndex)
	{
		for(auto it = inJetInspectionDecode.m_DetailsList.begin(); it != inJetInspectionDecode.m_DetailsList.end(); ++it)
		{
			text::tstring decodeDetailsPath = ioJetInspectionDecodePath + _T("/DecodeDetails/DecodeDetails<") + text::convertNumToTString<size_t>(ioIndex++) + _T(">");

			// 表面・裏面の区別をXMLデータに設定
			outElement->setValue(decodeDetailsPath + _T("/Side"), it->m_Side);
			// サイズ設定をXMLデータに設定
			outElement->setValue(
				decodeDetailsPath + _T("/Size"), 
				text::convertNumToTStringPrecision<double>(it->m_Size.first, 1)+ _T(" ") + text::convertNumToTStringPrecision<double>(it->m_Size.second, 1));
			// 位置設定をXMLデータに設定
			outElement->setValue(
				decodeDetailsPath + _T("/Offset"),
				text::convertNumToTStringPrecision<double>(it->m_Offset.first, 1)+ _T(" ") + text::convertNumToTStringPrecision<double>(it->m_Offset.second, 1));
			// ラベル名称をXMLデータに設定
			outElement->setValue(decodeDetailsPath + _T("/Name"), it->m_Name);
			// ラベル種類をXMLデータに設定
			outElement->setValue(decodeDetailsPath + _T("/Type"), it->m_Type);
			// （OCR限定）角度情報をXMLデータに設定
			outElement->setValue(decodeDetailsPath + _T("/Angle"), it->m_Angle);
			// デコードエラー発生時、エラー停止判定カウントの対象にするかをXMLデータに設定
			outElement->setValue(decodeDetailsPath + _T("/ErrorStop"), true == it->m_ErrorStop ? _T("true") : _T("false"));
		}
	};

	// JIデコード情報を取得
	SJetInspectionDecode jetInspectionDecode;
	if(!inPrintCondition.GetJetInspectionDecode(jetInspectionDecode, true))
	{
		std::string errorlog = "ExportFile::SetJIDecode failure Front";
		WriteToLogBuf(LOG_DEBUG, errorlog);
		return;
	}

	long indexDecodeDetails = 0;
	text::tstring jetInspectionDecodePath = inElementPath + _T("/Decode");
	// 「デコード」タブ、エラー停止連続シート数をXMLデータに設定
	outElement->setValue(jetInspectionDecodePath + _T("/ErrorSheets"), text::convertNumToTString<long>(jetInspectionDecode.m_ErrorSheets));
	// 「数値単位系をXMLデータに設定
	outElement->setValue(jetInspectionDecodePath + _T("/Unit"), jetInspectionDecode.m_Unit);

	// デコード処理の有効／無効
	outElement->setValue(jetInspectionDecodePath + _T("/IsEnabled"), true == jetInspectionDecode.m_IsEnabled ? _T("true") : _T("false"));

	text::tstring decodeDetailsFullPath = jetInspectionDecodePath + _T("/DecodeDetails");
	// PartIDKeysをXMLデータに設定
	outElement->setValue(decodeDetailsFullPath + _T("/PartIDKeys"), _T("Name"));

	// 表面のJIデコード詳細情報を設定
	setJIDecodeDetails(jetInspectionDecode, jetInspectionDecodePath, indexDecodeDetails);

	if(CDataSystemSetting::GetInstance()->IsDED())
	{
		SJetInspectionDecode jetInspectionDecodeBack;
		if(!inPrintCondition.GetJetInspectionDecode(jetInspectionDecodeBack, false))
		{
			std::string errorlog = "ExportFile::SetJIDecode failure Back";
			WriteToLogBuf(LOG_DEBUG, errorlog);
			return;
		}

		// 裏面のJIデコード詳細情報を設定
		setJIDecodeDetails(jetInspectionDecodeBack, jetInspectionDecodePath, indexDecodeDetails);
	}
}

// 変倍情報の設定
void ExportFile::SetVariableMagnification(
	const export_xml::PrintCondition& inPrintCondition,
	const text::tstring& inElementPath,
	std::unique_ptr<xml_utility::XMLElementControl>& outElement)
{
	// 変倍情報を取得
	ST_PDB_RIP_MAGNIFICATION ripMagnification;
	if(!inPrintCondition.GetVariableMagnification(ripMagnification))
	{
		// 変倍情報取得失敗時は処理終了
		throw std::runtime_error("SetVariableMagnification");
	}

	text::tstring scalingPath = inElementPath + _T("/Scaling");

	// 変倍機能のON/OFFをXMLデータに設定
	outElement->setValue(scalingPath + _T("/ScalingValue"), TRUE == ripMagnification.IS_ENABLE ? _T("true") : _T("false"));

	// 基準面をXMLデータに設定
	outElement->setValue(scalingPath + _T("/BaseSide"), TRUE == ripMagnification.IS_DATUM_PLANE_FRONT ? _T("Front ") : _T("Back"));

	// 変倍値(表・横)をXMLデータに設定
	outElement->setValue(scalingPath + _T("/ScalingValueFrontW"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.FRONT_X) / 1000, 3));

	// 変倍値(表・縦)をXMLデータに設定
	outElement->setValue(scalingPath + _T("/ScalingValueFrontH"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.FRONT_Y) / 1000, 3));

	// 変倍値(裏・横)をXMLデータに設定
	outElement->setValue(scalingPath + _T("/ScalingValueBackW"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.BACK_X) / 1000, 3));

	// 変倍値(裏・縦)をXMLデータに設定
	outElement->setValue(scalingPath + _T("/ScalingValueBackH"), text::convertNumToTStringPrecision<double>(static_cast<double>(ripMagnification.BACK_Y) / 1000, 3));
}

// XMLを出力する
void ExportFile::outputXML(
	const text::tstring& inOutputPath,
	const std::unique_ptr<xml_utility::XMLElementControl>& inElement)
{
	// 出力するXMLデータを文字データに変換する
	text::tstring outXMLData = inElement->getTString();

	// XMLファイルを出力する (UTF-8 Bom無し)
	file_utility::OutTextFileStream outTextFileStream(inOutputPath, false, false);
	outTextFileStream.writeLine(outXMLData);
}

// 指定XMLパスにXYパラメータを設定する
template <class T>
void ExportFile::setXYpairParameter(
	std::unique_ptr<xml_utility::XMLElementControl>& outElement,
	const text::tstring& inElementPath,
	const T inParamX,
	const T inParamY)
{
	// 文字列に変換
	text::tstring paramX = text::convertNumToTString<T>(inParamX);
	text::tstring paramY = text::convertNumToTString<T>(inParamY);
	text::tstring param = paramX + _T(" ") + paramY;

	// XMLデータに設定
	outElement->setValue(inElementPath, param);
}

// ラインフラッシング設定有無を取得する
bool ExportFile::isLineFlushing(DEF_FLUSHING_PATTERN inFlushingPattern)
{
	bool ret = true;

	switch(inFlushingPattern)
	{
	case DEF_FLUSHING_PATTERN_NONE:
	case DEF_FLUSHING_PATTERN_STAR_1:
	case DEF_FLUSHING_PATTERN_STAR_2:
	case DEF_FLUSHING_PATTERN_STAR_4:
	case DEF_FLUSHING_PATTERN_STAR_8:
		ret = false;
		break;
	}

	return ret;
}

}//namespace export_xml