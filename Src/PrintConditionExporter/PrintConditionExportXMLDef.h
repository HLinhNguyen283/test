/*! \file PrintConditionExportXMLDef.h
 *  \brief PrintConditionExportXMLDef.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
 *  \date 2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
*/
#pragma once

// ==========================================================================
// Include files

#include <array>
#include <map>
// ==========================================================================

namespace export_xml
{
	
static const int AnchorPointColors = 4;	//!< アンカーポイント情報取得用の色数
static const int AnchorPointNum = 5;	//!< アンカーポイント数


typedef std::pair<long, long> xypair_t; //!< ２つの情報を一緒に保持する用の型定義(long型)
typedef std::pair<double, double> xypair_double; //!< ２つの情報を一緒に保持する用の型定義(double型)

//! 用紙設定情報
struct SPaperSetting
{
	double m_width;					//!< 用紙幅[単位:point]
	long m_thickness;				//!< 用紙厚み[単位:μm]
	long m_tension;					//!< テンション
	bool m_DNSStatus;				//!< DNS設定
	bool m_punchHole;				//!< パンチ穴用紙
	bool m_sewing;					//!< ミシン目用紙
	bool m_marker;					//!< 事前マーク
	bool m_prePrinted;				//!< 事前印刷
	double m_markerOffset;			//!< マークオフセット(主走査)[単位:point]
	double m_pageFeedSize;			//!< ページ送りサイズ[単位:point]
	std::string m_pageFeedSizeUnit;	//!< ページ送りサイズの単位
	std::string m_markSide;			//!< マーク印刷面
	long m_repeatSheets;			//!< ミシン目パターンの繰り返し数

	SPaperSetting()
	{
		m_width = 0.0;
		m_thickness = 0;
		m_tension = 0;
		m_punchHole = false;
		m_sewing = false;
		m_marker = false;
		m_prePrinted = false;
		m_markerOffset = 0.0;
		m_pageFeedSize = 0.0;
		m_repeatSheets = 0;
	}
};

//! 乾燥温度情報
struct SDryerTemperture
{
	long m_heatRollerSelf;	//!< ヒートローラー乾燥温度(自機)[単位:℃]
	long m_heatRollerPart;	//!< ヒートローラー乾燥温度(他機)[単位:℃]
	long m_heatFanSelf;		//!< 温風アシスト乾燥温度(自機)[単位:℃]
	long m_heatFanPart;		//!< 温風アシスト乾燥温度(他機)[単位:℃]
	long m_precoater1;		//!< 先塗り乾燥温度1[単位:℃]
	long m_precoater2;		//!< 先塗り乾燥温度2[単位:℃]
	long m_NirPowerSelf;	//!< NIR設定（自機）[kW]
	long m_NirPowerPart;	//!< NIR設定（他機）[kW]
	long m_SubHeatRollerSelf;	//!< サブヒートローラー乾燥温度(自機)[単位:℃]
	long m_SubHeatRollerPart;	//!< サブヒートローラー乾燥温度(他機)[単位:℃]

	SDryerTemperture()
	{
		m_heatRollerSelf = 0;
		m_heatRollerPart = 0;
		m_heatFanSelf = 0;
		m_heatFanPart = 0;
		m_precoater1 = 0;
		m_precoater2 = 0;
		m_NirPowerSelf = 0;
		m_NirPowerPart = 0;
		m_SubHeatRollerSelf = 0;
		m_SubHeatRollerPart = 0;
	}
};

//! ICCプリセット情報
struct SICCPreset
{
	std::string m_descriptiveName; //!< ICCプリセット名
	std::string m_presetID; 	   //!< ICCプリセットID

	SICCPreset()
	{
	}
};

//! ジョブ基本設定
struct SJobBase
{
	bool m_informationSheet;	//!< インフォメーションシートを出力するかどうか
	bool m_flushingPage;		//!< フラッシングページを挿入するかどうか
	std::string m_patternName;	//!< フラッシングページのパターン名
	long m_interval;			//!< フラッシングページの挿入間隔
	bool m_preJob;			//!< ジョブ前に挿入するかどうか
	long m_preJobCount;		//!< ジョブ前に挿入する回数
	bool m_postJob;			//!< ジョブ後に挿入するかどうか
	long m_offsetFront;		//!< フラッシングページのオフセット（表面・副走査）[単位:0.001mm]
	long m_offsetBack;		//!< フラッシングページのオフセット（裏面・副走査）[単位:0.001mm]
	long m_cleaningJudgmentLevelBeforeJob;		//!< ジョブ前のクリーニング判定レベル
	long m_cleaningJudgmentLevelAfterJob;		//!< ジョブ後のクリーニング判定レベル
	EPrintStopSetting m_printStopSetting;		//!< 印刷停止設定

	SJobBase()
	{
		m_informationSheet = false;
		m_flushingPage = false;
		m_interval = 0;
		m_preJob = false;
		m_preJobCount = 0;
		m_postJob = false;
		m_offsetFront = 0;
		m_offsetBack = 0;
		m_cleaningJudgmentLevelBeforeJob = 0;
		m_cleaningJudgmentLevelAfterJob = 0;
		m_printStopSetting = kNotStopIfMissingNozzle;
	}
};

//! レイアウト情報
struct SLayout
{
	xypair_t m_offsetFront;	//!< 画像のオフセット（表面・主副）[単位:0.001mm]
	xypair_t m_offsetBack;	//!< 画像のオフセット（裏面・主副）[単位:0.001mm]

	bool m_verification;			//!< サイド検査をするかどうか
	long m_continuousPageCount;		//!< エラーにする連続シート数
	bool m_cueMark;					//!< 印刷開始マークを出力するかどうか
	bool m_barcode;					//!< バーコードを出力するかどうか
	xypair_t m_barcodeOffsetFront;	//!< バーコードのオフセット（表面・主走査）[単位:0.001mm]
	xypair_t m_barcodeOffsetBack;	//!< バーコードのオフセット（裏面・主副）[単位:0.001mm]
	std::string m_flushingPattern;	//!< フラッシングパターン
	DEF_FLUSHING_PATTERN m_flushingPatternDef;	//!< フラッシングパターン定義
	long m_gapBetweenColors;		//!< ラインフラッシングのずらし量
	long m_lineOffsetFront;			//!< ラインフラッシングのオフセット（表面・副走査）[単位:0.001mm]
	long m_lineOffsetBack;			//!< ラインフラッシングのオフセット（裏面・副走査）[単位:0.001mm]
	bool m_LineGap;					//!< ページ内、ラインフラッシング
	long m_LineGapFront;			//!< ページ内、ラインフラッシング間隔（表面・副走査）[単位:0.001mm]
	long m_LineGapBack;				//!< ページ内、ラインフラッシング間隔（裏面・副走査）[単位:0.001mm]
	long m_LineGapOffsetFront;		//!< ページ内、ラインフラッシングの補正オフセット（表面・副走査）[単位:0.001mm]
	long m_LineGapOffsetBack;		//!< ページ内、ラインフラッシングの補正オフセット（裏面・副走査）[単位:0.001mm]
	std::string m_LinePosition;		//!< ページ内、ラインフラッシングの表示位置("Top" or "Bottom")
	std::string m_LineOptions;		//!< ページ内、ラインフラッシングのオプション情報
	long m_cutterMark;				//!< カッターマークの印刷面
	long m_cutterMarkPageCount;		//!< カッターマークの準備シート数
	xypair_t m_cutterMarkSize;		//!< カッターマークのサイズ（主副）[単位:0.001mm]
	long m_cutterMarkOffsetFront;	//!< カッターマークのオフセット（表面・主走査）[単位:0.001mm]
	long m_cutterMarkOffsetBack;	//!< カッターマークのオフセット（裏面・主走査）[単位:0.001mm]
	long m_cutterMarkAlignFront;	//!< カッターマークオフセットの基準位置（表面）
	long m_cutterMarkAlignBack;		//!< カッターマークオフセットの基準位置（裏面）

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	bool m_adjustmentSheetSizeEnabled;	//!< 後処理マークの調整シートマークサイズ有効(TRUE)/無効(FALSE)
	long m_adjustmentSheetLength;	//!< 後処理マークの調整シートマーク高さ[単位:0.001mm]
	ADJUSTMENT_SHEET_AFTER_JOB_OPTION m_adjustmentSheetAfterJob;	//!< 後処理マークのジョブ後のシート数を指定する方法を選択するオプション
	long m_afterJobSheets;			//!< 後処理マークのジョブ後シート数
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

	SLayout()
	{
		m_offsetFront.first = 0;
		m_offsetFront.second = 0;
		m_offsetBack.first = 0;
		m_offsetBack.second = 0;
		m_verification = false;
		m_continuousPageCount = 0;
		m_cueMark = false;
		m_barcode = false;
		m_barcodeOffsetFront.first = 0;
		m_barcodeOffsetFront.second = 0;
		m_barcodeOffsetBack.first = 0;
		m_barcodeOffsetBack.second = 0;
		m_flushingPatternDef = DEF_FLUSHING_PATTERN_NONE;
		m_gapBetweenColors = 0;
		m_lineOffsetFront = 0;
		m_lineOffsetBack = 0;
		m_LineGap = false;
		m_LineGapFront = 0;
		m_LineGapBack = 0;
		m_LineGapOffsetFront = 0;
		m_LineGapOffsetBack = 0;
		m_cutterMark = 0;
		m_cutterMarkPageCount = 0;
		m_cutterMarkSize.first = 0;
		m_cutterMarkSize.second = 0;
		m_cutterMarkOffsetFront = 0;
		m_cutterMarkOffsetBack = 0;
		m_cutterMarkAlignFront = 0;
		m_cutterMarkAlignBack = 0;

		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		m_adjustmentSheetLength = 0;
		m_afterJobSheets = 0;
		m_adjustmentSheetAfterJob = NONE;
		m_adjustmentSheetSizeEnabled = false;
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	}
};

//! アンカーポイント情報
struct AnchorPoint
{	
	long m_beforeConPoint;		//!< 基準濃度値
	long m_afterConPoint;		//!< カーブデータの数値
	long m_anchorPointEditType;	//!< 編集タイプ
	long m_anchorPointType;		//!< アンカーポイント種別
	AnchorPoint()
	{	
		m_beforeConPoint = 0;
		m_afterConPoint = 0;
		m_anchorPointEditType = 0;
		m_anchorPointType = 0;
	}
};

//! 1色分のアンカーポイント情報(5点分)
struct OneColorPoint
{
	std::array<AnchorPoint, AnchorPointNum> point; //!< 1色分のアンカーポイント
	OneColorPoint()
	{
	}
};

//! トーンカーブ情報
struct SToneCurve
{
	std::string m_side;				//!< 表面・裏面の区別
	std::string m_descriptiveName;	//!< トーンカーブ名
	std::map<std::string, OneColorPoint> anchorPoint;	//!< アンカーポイント情報4色×5点分

	SToneCurve()
	{
	}
};

//! JetInspection情報
struct SJetInspection
{
	std::string m_ParamaterName;		//!< 検査条件名
	std::string m_TemporalMaskFilePath;	//!< 一時マスクファイルパス

	SJetInspection()
	{
	}
};

//! JetInspection非検査矩形エリア情報
struct SJetInspectionNonInspectionRect
{
	long m_ID;					//!< 非検査矩形エリアID
	xypair_double m_StartPos;	//!< 非検査矩形エリア開始位置
	xypair_double m_EndPos;		//!< 非検査矩形エリア終了位置

	SJetInspectionNonInspectionRect()
	{
		m_ID = 0;
		m_StartPos.first = 0.0;
		m_StartPos.second = 0;
		m_EndPos.first = 0.0;
		m_EndPos.second = 0.0;
	}
};

//! JetInspectionエリア情報
struct SJetInspectionArea
{
	std::string m_side;													//!< 検査設定、対象面
	double m_NonInspectionAreaLeft;										//!< 非検査エリア左端
	double m_NonInspectionAreaRight;									//!< 非検査エリア右端
	std::vector<SJetInspectionNonInspectionRect> m_NonInspectionRect;	//!< JetInspection非検査矩形エリア情報
	
	SJetInspectionArea()
	{
		m_NonInspectionAreaLeft = 0.0;
		m_NonInspectionAreaRight = 0.0;
	}
};

//! JetInspectionデコード詳細情報
struct SJetInspectionDecodeDetails
{
	std::string m_Side;				//!< 対象面（表 or 裏）
	xypair_double m_Size;			//!< サイズ設定
	xypair_double m_Offset;			//!< 位置設定
	std::string m_Name;				//!< ラベル名称
	std::string m_Type;				//!< ラベル種類
	std::string m_Angle;			//!< （OCR限定）角度情報
	bool m_ErrorStop;				//!< デコードエラー発生時、エラー停止判定カウントの対象にするか

	SJetInspectionDecodeDetails()
	{
		m_Size.first = 0.0;
		m_Size.second = 0.0;
		m_Offset.first = 0.0;
		m_Offset.second = 0.0;
		m_ErrorStop = false;
	}
};

//! JetInspectionデコード情報
struct SJetInspectionDecode
{
	long m_ErrorSheets;											//!< 「デコード」タブ、エラー停止連続シート数
	std::string m_Unit;											//!< 数値単位系
	bool m_IsEnabled;											//!< デコード処理の有効／無効
	std::vector<SJetInspectionDecodeDetails> m_DetailsList;		//!< JetInspectionデコード詳細情報

	SJetInspectionDecode()
	{
		m_ErrorSheets = 0;
		m_IsEnabled = false;
	}
};

}//namespace export_xml