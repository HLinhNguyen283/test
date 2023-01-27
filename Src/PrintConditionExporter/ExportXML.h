/*! \file	ExportXML.h
 *  \brief	ExportXML.hの処理
 *  \author	pt.ju-tanaka
 *  \date	2019/06/26 pt.ju-tanaka 新規作成
*/
#pragma once

#include "XMLElementControl.h"
#include "PrintConditionExportXML.h"
#include "PrintConditionExportXMLDef.h"

namespace export_xml
{

/**
	@brief	エクスポートファイル(XML)
**/
class ExportFile
{
	public:
		/**
			@brief	Constructor
		*/
		ExportFile();

		/**
			@brief	Destructor
		*/
		~ExportFile();

		/**
			@brief	エクスポート依頼
			@param[in]	inOutputPath		 : XMLの出力ファイルパス
			@param[in]	inPrintConditionList : XML出力用印刷条件一覧
			@retval	true : 成功
			@retval	false : 失敗
		*/
		bool onRequestExportXML(
			const text::tstring& inOutputPath,
			const export_xml::PrintCondition& inPrintConditionList);

	private:
		/**
			@brief	印刷条件名の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetPrintConditionName(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	用紙種類の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetPaperType(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	段差補正名の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetAlignment(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	印刷モードの設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetPrintMode(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	印刷速度の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetPrintSpeed(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	用紙設定情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetPaperSetting(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	乾燥温度情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetDryerTemperature(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);
			
		/**
			@brief	ICCプリセット情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetICCPreset(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	トーンカーブ情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetToneCurve(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	ジョブ基本設定情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetJobBase(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);
			
		/**
			@brief	レイアウト情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetLayout(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JI情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetJI(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JIエリア情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetJIArea(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	JIデコード情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetJIDecode(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	変倍情報の設定
			@param[in]	inPrintCondition : XML出力用印刷条件
			@param[in]	inElementPath	 : XMLエレメントのパス
			@param[out]	outElement		 : 追加されるXMLデータ
		*/
		void SetVariableMagnification(
			const export_xml::PrintCondition& inPrintCondition,
			const text::tstring& inElementPath,
			std::unique_ptr<xml_utility::XMLElementControl>& outElement);

		/**
			@brief	XMLを出力する
			@param[in]	inOutputPath : 出力先パス
			@param[in]	inElement	 : 出力するXMLデータ
		*/
		void outputXML(
			const text::tstring& inOutputPath,
			const std::unique_ptr<xml_utility::XMLElementControl>& inElement);

		/**
			@brief	指定XMLパスにXYパラメータを設定する
			@param[out]	outElement : 追加されるXMLデータ
			@param[in]	inElementPath : XMLエレメントのパス
			@param[in]	inParamX : パラメータ(X)
			@param[in]	inParamY : パラメータ(Y)
		*/
		template <class T>
		void setXYpairParameter(
			std::unique_ptr<xml_utility::XMLElementControl>& outElement,
			const text::tstring& inElementPath,
			const T inParamX,
			const T inParamY);

		/**
			@brief	ラインフラッシング設定有無を取得する
			@param[in]	inFlushingPattern : フラッシングパターン
			@retval	true : ラインフラッシング設定有り
			@retval	false : ラインフラッシング設定無し
		*/
		bool isLineFlushing(DEF_FLUSHING_PATTERN inFlushingPattern);
};

}//namespace export_xml