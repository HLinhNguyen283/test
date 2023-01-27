/**
 * @file	PrintConditonForXML.h
 * @brief	XMLパーサーから取得した印刷条件1件分
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

#if !defined(__PRINT_CONDITION_FOR_XML_H_INCLUDE__)
#define __PRINT_CONDITION_FOR_XML_H_INCLUDE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include <map>
#include "Value.h"
#include "DataSystemSetting.h"

namespace printconditionsparser {
	class IPrintCondition;
}

/**
 * @brief XMLパーサーから取得した印刷条件1件分
**/
class CPrintConditionForXML
{
public:
	//! エレメントの識別子
	enum Type
	{
		kPrintConditionName = 0,		//!< 印刷条件名
		kPaperType,						//!< 用紙種類
		kLevelTuneName,					//!< 段差補正名
		kPaperWidth,					//!< 用紙幅
		kPaperThickness,				//!< 用紙厚み
		kPaperTension,				//!< テンション
		kDNS,						//!< DNS
		kPaperPunchHole,				//!< パンチ穴用紙
		kPaperSewing,				//!< ミシン目用紙
		kPaperMarker,				//!< 事前マーク
		kPaperPrePrinted,			//!< 事前印刷
		kPaperMarkerOffset,			//!< マークオフセット（主走査）
		kPaperPageFeedSize,			//!< ページ送りサイズ
		kPaperPageFeedSizeUnit,		//!< ページ送りサイズの単位
		kPaperMarkSide,				//!< マークの印刷面
		kPrintMode,					//!< 印刷モード名
		kPrintSpeed,					//!< 印刷速度
		kHeatRollerSelf,				//!< ヒートローラー乾燥温度（自機）
		kHeatRollerPart,				//!< ヒートローラー乾燥温度（他機）
		kHeatFanSelf,				//!< 温風アシスト乾燥温度（自機）
		kHeatFanPart,				//!< 温風アシスト乾燥温度（他機）
		kNIRPowerSelf,				//!< IR設定（自機）
		kNIRPowerPart,				//!< IR設定（他機）
		kPrecoater1,				//!< 先塗り乾燥温度1
		kPrecoater2,				//!< 先塗り乾燥温度2
		kSubHeatRollerSelf,				//!< サブヒートローラー乾燥温度（自機）
		kSubHeatRollerPart,				//!< サブヒートローラー乾燥温度（他機）
		kICCPresetName,				//!< ICCプリセット名
		kICCPresetID,				//!< ICCプリセット ID
		kToneCurve,					//!< トーンカーブ不明
		kJobBaseInformationSheet,	//!< インフォメーションシートを出力するかどうか
		kJobBaseFlushingPage,		//!< フラッシングページを挿入するかどうか
		kJobBasePatternName,			//!< フラッシングページのパターン名
		kJobBaseInterval,			//!< フラッシングページの挿入間隔
		kJobBasePreJob,				//!< ジョブ前に挿入するかどうか
		kJobBasePreJobCount,			//!< ジョブ前に挿入する回数
		kJobBasePostJob,				//!< ジョブ後に挿入するかどうか
		kJobBaseOffsetFront,			//!< フラッシングページのオフセット（表面・副走査）
		kJobBaseOffsetBack,			//!< フラッシングページのオフセット（裏面・副走査）
		kJobBaseCleaningJudgmentLevelBeforeJob,	//!< ジョブ前のクリーニング判定レベル
		kJobBaseCleaningJudgmentLevelAfterJob,	//!< ジョブ後のクリーニング判定レベル
		kJobBasePrintStopSetting,				//!< 印刷停止設定
		kLayoutOffsetFront,			//!< 画像のオフセット（表面・主副）
		kLayoutOffsetBack,			//!< 画像のオフセット（裏面・主副）
		kLayoutVerification,			//!< サイド検査をするかどうか
		kLayoutContinuousPageCount,	//!< エラーにする連続シート数
		kLayoutCueMark,				//!< 印刷開始マークを出力するかどうか
		kLayoutBarcode,				//!< バーコードを出力するかどうか
		kLayoutBarcodeOffsetFront,	//!< バーコードのオフセット（表面・主走査。副走査は0 固定）
		kLayoutBarcodeOffsetBack,	//!< バーコードのオフセット（裏面・主副）
		kLayoutFlushingPattern,		//!< フラッシングパターン
		kLayoutGapBetweenColors,		//!< ラインフラッシングのずらし量
		kLayoutLineOffsetFront,		//!< ラインフラッシングのオフセット（表面・副走査）
		kLayoutLineOffsetBack,		//!< ラインフラッシングのオフセット（裏面・副走査）
		kLayoutLineGap,				//!< ページ内、ラインフラッシング
		kLayoutLineGapFront,		//!< ページ内、ラインフラッシング間隔（表面・副走査）
		kLayoutLineGapBack,			//!< ページ内、ラインフラッシング間隔（裏面・副走査）
		kLayoutLineGapOffsetFront,	//!< ページ内、ラインフラッシングの補正オフセット（表面・副走査）
		kLayoutLineGapOffsetBack,	//!< ページ内、ラインフラッシングの補正オフセット（裏面・副走査）
		kLayoutLinePosotion,		//!< Bottom指定時、ラインフラッシングの下端配置および後処理マークの下端配置がON
		kLayoutLineOptions,			//!< ラインフラッシングのオプション情報(ST_PDB_LAYOUT_IMAGE_INFOのST_JOB_FLUSHING_PATTERN_ADD情報)
		kLayoutCutterMark,			//!< カッターマークの印刷面
		kLayoutCutterMarkPageCount,	//!< カッターマークの準備シート数
		kLayoutCutterMarkSize,		//!< カッターマークのサイズ（主副）
		kLayoutCutterMarkOffsetFront,//!< カッターマークのオフセット（表面・主走査）
		kLayoutCutterMarkOffsetBack,	//!< カッターマークのオフセット（裏面・主走査）
		kLayoutCutterMarkAlignFront,	//!< カッターマークオフセットの基準位置（表面）
		kLayoutCutterMarkAlignBack,	//!< カッターマークオフセットの基準位置（裏面）
		// JI追加分
		kInspection,						//! 検査情報
		kInspectionJICondition,				//! 検査情報:検査条件名
		kInspectionTemporalMaskFile,		//! 検査情報:一時マスクファイルパス
		kDecode,							//! デコード
		kDecodeErrorSheets,					//! デコード:エラー停止連続シート数
		kDecodeUnit,						//! デコード:数値単位系
		kDecodeIsEnabled,					//! デコード:デコード処理の有効／無効
		kScalingValue,						//!< 変倍パラメータ:変倍機能のON/OFF
		kScalingBaseSide,					//!< 変倍パラメータ:基準面
		kScalingValueFrontW,				//!< 変倍パラメータ:変倍値(表:横)
		kScalingValueFrontH,				//!< 変倍パラメータ:変倍値(表:縦)
		kScalingValueBackW,					//!< 変倍パラメータ:変倍値(裏:横)
		kScalingValueBackH,					//!< 変倍パラメータ:変倍値(裏:縦)
		// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		kLayoutAdjustmentSheetSizeEnabled,	//!< 後処理マークの調整シートマークサイズ有効(TRUE)/無効(FALSE)
		kLayoutAdjustmentSheetLength,		//!< 後処理マークの調整シートマーク高さ
		kLayoutAdjustmentSheetAfterJob,		//!< 後処理マークのジョブ後のシート数を指定する方法を選択するオプション
		kLayoutAfterJobSheets,				//!< 後処理マークのジョブ後シート数
		// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		kNumOfTypes					//!< 要素数
	};

private:
	//! XMLから値が得られなかった場合のエラー種類
	enum ErrorType
	{
		OkIfNothing = false,	//!< 得られなくても問題なし(選択項目)
		ErrorIfNothing = true	//!< 得られないとエラーとする(必須項目)
	};
	std::vector<std::shared_ptr<CErrorLevel>> m_elements;	//!< XMLから得た値一覧
	CDataSystemSetting& m_dataSystemSetting;		//!< システムのデータ

	bool m_isSkipped;				//!< 取り込みスキップ可能

public:

	/**
		ToneCurve パラメータ取得用クラス
	*/
	class ToneCurve {
	public:
		ToneCurve() {}
		virtual ~ToneCurve() {}
		ToneCurve(
			const ToneCurve&	inTone);

		/**
			XML から読み込んだ情報を反映する。内部でしか使わない前提
			@param[in] inCondition : XML 読み込み情報
		*/
		void set(
			const printconditionsparser::IPrintCondition&	inCondition);

		/**
			指定面の情報が存在するかどうかを取得する
			@param[in] inIsFront : 表面なら true、裏面なら false
			@return true : 存在する
			@return false : 存在しない
		*/
		bool isExist(
			bool	inIsFront) const;
		/**
			トーンカーブ名（GUI 表示名）を取得する
			@param[in] inIsFront : 表面なら true、裏面なら false
			@return トーンカーブ名
		*/
		const std::string& getDescriptiveName(
			bool	inIsFront) const;
			

		enum EColor {
			kToneCurve_Color_C, kToneCurve_Color_M, kToneCurve_Color_Y, kToneCurve_Color_K, kToneCurve_ColorNum,
		};
		/**
			アンカーポイント情報を取得する
			@param[in] inIsFront : 表面なら true、裏面なら false
			@param[in] inColor : 取得したい色
			@param[in] inPointID : 取得したいポイント ID (0 - 4)
			@param[out] outBeforeConPoint : 基準濃度値
			@param[out] outAfterConPooint : トーンカーブデータの数値
			@param[out] outAnchorPointEditType : 編集タイプ（実質0固定）
			@param[out] outAnchorPointType : トーンカーブデータの数値が負なら 0、それ以外なら 1
		*/
		void getAnchorPoint(
			bool	inIsFront,
			EColor	inColor,
			int		inPointID,
			long&	outBeforeConPoint,
			long&	outAfterConPoint,
			long&	outAnchorPointEditType,
			long&	outAnchorPointType) const;

	private:
		class OneTone;

		std::vector<std::shared_ptr<OneTone> > m_Tones;

	};

	/**
		ページ送りサイズの単位
	*/
	enum EPageFeedUnit {
		kPageFeedUnit_mm, kPageFeedUnit_inch, kPageFeedUnit_inch2, kPageFeedUnit_inch6, kPageFeedUnit_Page, kPageFeedUnit_Empty,
	};
	/**
		マークの印刷面
	*/
	enum EMarkSide {
		kMarkSide_Front, kMarkSide_Back, kMarkSide_Empty,
	};
	/**
		カッターマークの印刷面
	*/
	enum ECutterMarkSide {
		kCutterMarkSide_Off, kCutterMarkSide_Front, kCutterMarkSide_Back, kCutterMarkSide_Both,
	};
	/**
		カッターマークオフセットの基準位置
	*/
	enum ECutterMarkAlign {
		kCutterMarkAlign_Left = -1, kCutterMarkAlign_Center, kCutterMarkAlign_Right,
	};

	/**
		Inspection パラメータ取得用クラス
	*/
	class Inspection {
	public:
		class VerificationParams;
		class TemporalMaskInfo;
		class RectInfo;

		Inspection() {}
		virtual ~Inspection() {}
		Inspection(
			const Inspection&	inInspection);

		/**
			XML から読み込んだ情報を反映する。内部でしか使わない前提
			@param[in] inCondition : XML 読み込み情報
			@retval false	タグなし
			@retval true	タグあり
		*/
		bool set(
			const printconditionsparser::IPrintCondition&	inCondition);


		/**
			非検査エリア左端 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[out] outValue : 非検査エリア左端 の値
			@return	false 値がない
		*/
		bool getNonInspectionAreaLeft(bool inSide, double& outValue) const;

		/**
			非検査エリア右端 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[out] outValue : 非検査エリア右端 の値
			@return	false 値がない
		*/
		bool getNonInspectionAreaRight(bool inSide, double& outValue) const;

		/**
			検査条件詳細 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[out] outVerificationParams 検査条件詳細
			@return false:データなし
		*/
		bool getVerificationParams(
			bool inSide,
			std::map<std::string, std::pair<bool, std::string> >& outVerificationParams) const;

		/**
			AI検査条件詳細 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[out] outAIAssistParams AI検査条件詳細
			@return false:データなし
		*/
		bool getAIAssistParams(
			bool inSide,
			std::map<std::string, std::pair<bool, std::string> >& outAIAssistParams) const;

		/**
			一時マスク情報 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[in] inIndex : インデクス(0-)
			@param[out] outType 欠点種類（汚れ or スジ汚れ or 白スジ or 欠落）
			@param[out] outPos 欠点位置
			@param[out] outSize 欠点サイズ
			@param[out] outTemporalMaskSize 一時マスクサイズ
			@return false:データなし
		*/
		bool getTemporalMaskInfo(
			bool inSide,
			size_t inIndex,
			std::string& outType,
			std::pair<double, double>& outPos,
			std::pair<double, double>& outSize,
			std::pair<double, double>& outTemporalMaskSize) const;
	
		/**
			矩形情報 を取得する
			@param[in] inSide : 表面なら true、裏面なら false
			@param[in] inIndex : インデクス(0-)
			@param[out] outID 非検査矩形エリアID(1-5)
			@param[out] outStartPos 始点位置
			@param[out] outEndPos 終点位置
			@return false:データなし
		*/
		bool getRectInfo(
			bool inSide,
			size_t inIndex,
			std::string& outID,
			std::pair<long, long>& outStartPos,
			std::pair<long, long>& outEndPos) const;

	private:
		std::string	m_paramaterName;				//!< 検査条件名
		std::string	m_temporalMaskFilePath;			//!< 一時マスクファイルパス

		std::map<bool, std::map<std::string, std::pair<bool, std::string>>>		m_verificationParams;		//!< 検査条件詳細情報
		std::map<bool, std::map<std::string, std::pair<bool, std::string>>>		m_aiAssistParams;			//!< AI検査条件詳細情報
		std::map<bool, std::vector<std::shared_ptr<TemporalMaskInfo>>>			m_temporalMaskInfoList;		//!< 一時マスク
		std::map<bool, double> m_nonInspectionAreaLeft;				//!< 非検査エリア左
		std::map<bool, double> m_nonInspectionAreaRight;			//!< 非検査エリア右 
		std::map<bool, std::vector<std::shared_ptr<RectInfo>>>					m_rectInfoList;				//!< 非検査矩形エリア
	};

	/**
		Decode パラメータ取得用クラス
	*/
	class Decode {
	public:
		Decode() {}
		virtual ~Decode() {}
		Decode(
			const Decode&	inDecode);
		/**
			XML から読み込んだ情報を反映する。内部でしか使わない前提
			@param[in] inCondition : XML 読み込み情報
			@retval false	タグなし
			@retval true	タグあり
		*/
		bool set(
			const printconditionsparser::IPrintCondition&	inCondition);

		/**
			Decode詳細数を取得する
			@return	Decode詳細数
		*/
		size_t getDetailNum();

		/**
			Decode詳細 エレメントを取得する
			@param[in] inIndex : 取得したいインデクス(0-)
			@param[out] outSide,	 : 対象面（表 or 裏）Front / Back
			@param[out] outSize,	 : サイズ設定
			@param[out] outOffset,	 : 位置設定
			@param[out] outName,	 : ラベル名称
			@param[out] outType,	 : ラベル種類
			@param[out] outAngle,	 : （OCR限定）角度情報
			@param[out] outErrorStop : デコードエラー発生時、エラー停止判定カウントの対象にするか？
			@return	true: 成功 false:失敗(inIndex範囲外)
		*/
		bool getDetail(
			size_t		inIndex,
			std::string&				outSide,	
			std::pair<double, double>&	outSize,	
			std::pair<double, double>&	outOffset,	
			std::string&				outName,	
			std::string&				outType,	
			std::string&				outAngle,	
			bool&						outErrorStop) const;

	private:
		/**
			デコード詳細情報
		*/
		class DetailInfo;

		std::vector<std::shared_ptr<DetailInfo>>				m_detailInfoList;		//!< デコード詳細
	};

	/**
	 * @brief	constructor
	**/
	CPrintConditionForXML(CDataSystemSetting& inDataSystemSetting);

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionForXML();

	/**
	 * @brief	XMLパーサーから取得したデータを変換する
	 * @param[in]	inCondition : XMLパーサーから取得した １件分の PrintCondition 情報
	**/
	void ReadFromXML(const printconditionsparser::IPrintCondition& inCondition);

	/**
	 * @brief	必須項目が読み込めているか判定。読み込めていない場合は例外が出る
	**/
	void JudgeRequiredItem();

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<std::string>* GetString(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<double>* GetDouble(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<long>* GetLong(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<bool>* GetBool(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<ToneCurve>* GetTone(
		Type	inType = kToneCurve) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<EPageFeedUnit>* GetPageFeedUnit(
		Type	inType = kPaperPageFeedSizeUnit) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<EMarkSide>* GetMarkSide(
		Type	inType = kPaperMarkSide) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<ECutterMarkSide>* GetCutterMarkSide(
		Type	inType = kLayoutCutterMark) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<ECutterMarkAlign>* GetCutterMarkAlign(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	typedef std::pair<long, long> xypair_t;
	const CValue<xypair_t>* GetXYPair(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<Inspection>* GetInspection(
		Type	inType) const;

	/**
	 * @brief	指定エレメントの値を取得する
	 * @param[in]	inType : エレメントの識別子
	 * @retval	指定エレメントの値
	**/
	const CValue<Decode>* GetDecode(
		Type	inType) const;

	const bool IsImportSkipped() { return m_isSkipped; }

protected:

};

#endif // !defined(__PRINT_CONDITION_FOR_XML_H_INCLUDE__)
