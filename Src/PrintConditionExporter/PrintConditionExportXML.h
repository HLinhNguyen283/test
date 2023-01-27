/*! \file PrintConditionExportXML.h
 *  \brief PrintConditionExportXML.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
*/
#pragma once

// ==========================================================================
// Include files

#include "CommonConvertUnit.h"
#include "PaperDB_MigrationDef.h"
#include "PrintConditionExportXMLDef.h"
#include "Ini_PrinterParameter.h"
// ==========================================================================

namespace export_xml
{

//! 印刷条件情報
struct SPrintConditionData
{
	std::string printConditionName;							//!< 印刷条件名
	ST_PDB_MIGRATION_DATA migrationData;					//!< 業務移行用構造体
	std::map<std::string, std::string> printModeDataList;	//!< 印刷モードの一覧 <印刷モード名, EQUIOSでの印刷モード名>
};

/**
	@brief	XML出力用印刷条件
*/
class PrintCondition
{
	private:
		
		SPrintConditionData m_PrintConditionData;	//!< 印刷条件情報

		/**
			@brief	constructor(※ 使用禁止)
		*/
		PrintCondition();

	public:

		/**
			@brief	constructor
			@param[in]	inPrintConditionData : 印刷条件データ
		*/
		PrintCondition(const SPrintConditionData& inPrintConditionData);

		/**
			@brief	destructor
		*/
		~PrintCondition();
			
		/**
			@brief	印刷条件名の取得
			@retval	印刷条件名
		*/
		std::string GetPrintConditionName() const;

		/**
			@brief	用紙種類の取得
			@retval	用紙種類
		*/
		std::string GetPaperType() const;

		/**
			@brief	段差補正名の取得
			@retval	段差補正名
		*/
		std::string GetLevelTuneName() const;

		/**
			@brief	用紙設定情報の取得
			@param[out]	outPaperSetting : 用紙設定情報
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetPaperSetting(SPaperSetting& outPaperSetting) const;

		/**
			@brief	印刷モードの取得
			@param[out]	outPrintMode : 印刷モード
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetPrintMode(std::string& outPrintMode) const;

		/**
			@brief	印刷速度の取得
			@retval	印刷速度
		*/
		double GetPrintSpeed() const;

		/**
			@brief	乾燥温度情報の取得
			@param[out]	outDryerTemperture : 乾燥温度情報
		*/
		void GetDryerTemperature(SDryerTemperture& outDryerTemperture) const;

			
		/**
			@brief	ICCプリセット情報の取得
			@param[out]	outICCPreset : ICCプリセット情報
		*/
		void GetICCPreset(SICCPreset& outICCPreset) const;


		/**
			@brief	ジョブ基本設定情報の取得
			@param[out]	outJobBase : ジョブ基本設定情報
		*/
		void GetJobBase(SJobBase& outJobBase) const;
			
		/**
			@brief	レイアウト情報の取得
			@param[out]	outLayout : レイアウト情報
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetLayout(SLayout& outLayout) const;

		/**
			@brief	トーンカーブ情報の取得
			@param[out]	outToneCurve :トーンカーブ情報
			@param[in]	inIsFront : 表面かどうか
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetToneCurve(SToneCurve& outToneCurve, bool inIsFront) const;

		/**
			@brief	JetInspection情報の取得
			@param[out]	outJetInspection : JetInspection情報
		*/
		void GetJetInspection(SJetInspection& outJetInspection) const;

		/**
			@brief	JetInspectionエリア情報の取得
			@param[out]	outJetInspectionArea : JetInspectionエリア情報
			@param[in]	inIsFront : 表面かどうか
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetJetInspectionArea(SJetInspectionArea& outJetInspectionArea, bool inIsFront) const;

		/**
			@brief	JetInspectionデコード情報の取得
			@param[out]	outJetInspectionDecode : JetInspectionデコード情報
			@param[in]	inIsFront : 表面かどうか
			@param[in]	inIsFront : 表面かどうか
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetJetInspectionDecode(SJetInspectionDecode& outJetInspectionDecode, bool inIsFront) const;

		/**
			@brief	変倍情報の取得
			@param[out]	outVariableMagnification : 変倍情報
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool GetVariableMagnification(ST_PDB_RIP_MAGNIFICATION& outVariableMagnification) const;

	private:
		/**
			@brief	エラー内容の書き込み
			@param[in]	inError : エラ―情報
		*/
		void writeErrorMessage(const std::string& inError) const;

		/**
			@brief	印刷面の取得
			@param[in]	inIsDED : DEDかどうか
			@param[in]	inIsFront : 表面かどうか
			@param[out]	outSide : 印刷面
			@param[out]	outIsPart : 自機or相手機(0:自機、1:相手機)
		*/
		void getSide(const bool inIsDED, const bool inIsFront, std::string& outSide, long& outIsPart) const;
};
}//namespace export_xml