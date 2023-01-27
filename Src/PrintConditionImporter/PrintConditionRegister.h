/**
 * @file	PrintConditionRegister.h
 * @brief	１件分の印刷条件を登録する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

#pragma once

//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include "Value.h"
#include <string>
#include <vector>
#include "Stub_PaperDBIF.h"
#include "PrintConditionForXML.h"

class CDataSystemSetting;
/**
	@brief 情報ログ抑制用例外
 */
class LogSupressException : public std::runtime_error
{
public:
	LogSupressException(const std::string& _Message) : std::runtime_error(_Message) {};
	// XML取り込み中にこの例外が発生したら、throw元で情報ログを出力する
	// それ以外はcatch側で情報ログを出力する
};


/**
 * @brief １件分の印刷条件をインポートする
**/
class CPrintConditionRegister
{
public:
	/**
	 * @brief	constructor
	 * @param[in]	inPrintConditionName : インポート対象の印刷条件名
	 * @param[in]	inDataSystemSetting : システムのデータ
	**/
	CPrintConditionRegister(const std::string& inPrintConditionName, CDataSystemSetting& inDataSystemSetting) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionRegister() throw();

	/**
	 * @brief	ベースとなる印刷条件の全レコードを読み込み初期値とする
	 * @param[in]	inIsAppend : 追加かどうか
	 * @note	エラー発生時は例外を投げる
	**/
	void LoadInitValue(bool inIsAppend) throw (std::runtime_error);

	/**
	 * @brief	インポート対象の印刷条件がシステムの印刷条件に登録済みかどうかを判断する
	 * @retval	true : あり
	 * @retval	false : なし
	**/
	bool IsRegisteredPrintConditionName() throw();

	/**
	 * @brief	インポート処理を実行する
	 * @param[in]	inIsAppend : 新規追加かどうか(true:新規追加, false:上書き) 
	 * @param[in]	inXML : XMLパーサーから取得した印刷条件1件分のデータ
	 * @note	インポートに失敗したときは、例外を投げる
	**/
	void Import(bool inIsAppend, CPrintConditionForXML& inXML) throw (std::runtime_error);

	/**
	 * @brief	用紙種類の新規追加確認
	 * @param[out]	PaperTypeName : 新規追加した用紙種類名
	 * @retval	true : 新規追加実施
	 * @retval	false : 追加なし
	**/
	bool IsAppendPaperType(std::string& PaperTypeName);

	/**
	 * @brief	段差補正の新規追加確認
	 * @param[out]	Name : 新規追加した段差補正名
	 * @retval	true : 新規追加実施
	 * @retval	false : 追加なし
	**/
	bool IsAppendAllignment(std::string& AllignmentName);

private:

	//! デバッグ出力するDB
	enum E_DEBUGOUT_DB {
		E_DEBUGOUT_DB_BASE = 0,			//!< ベースDB
		E_DEBUGOUT_DB_UPDATE			//!< 追加/上書きしたDB
	};

	/**
	 * @brief	各要素を紙DBに登録するフォーマットに変換する
	 * @param[in]	inXML : インポート用データ
	 * @note	変換に失敗したときは例外を投げる
	**/
	void setDataForPDB(const CPrintConditionForXML& inXML) throw (std::runtime_error);

	/**
	 * @brief	印刷条件名設定用パラメータを決定する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPrintConditionName(
		const CValue<std::string>* inPrintConditionName
	) throw (std::runtime_error);

	/**
	 * @brief	用紙種類パラメータを決定する
	 * @param[in]	inPaperType : 用紙種類
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperType(
		const CValue<std::string>* inPaperType
	) throw (std::runtime_error);

	/**
	 * @brief	段差補正名パラメータを決定する
	 * @param[in]	inLevelTuneName : 段差補正名
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportLevelTuneName(
		const CValue<std::string>* inLevelTuneName
	) throw (std::runtime_error);

	/**
	 * @brief	用紙幅設定用パラメータを決定する
	 * @param[in]	inPaperWidth : 用紙幅 [単位:point]
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperSizeW(
		const CValue<double>* inPaperWidth
	) throw (std::runtime_error);

	/**
	 * @brief	紙厚設定用パラメータを決定する
	 * @param[in]	inThickness : 紙厚 [単位:0.001mm]
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperThickness(
		const CValue<long>* inThickness
	) throw (std::runtime_error);

	/**
	 * @brief	テンション設定用パラメータを決定する
	 * @param[in]	inTension : テンション
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperTension(
		const CValue<long>* inTension
	) throw (std::runtime_error);

	/**
	 * @brief	DNS設定用パラメータを決定する
	 * @param[in]	inDns : DNS
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportDNS(
		const CValue<bool>* inDns
	) throw (std::runtime_error);

	/**
	 * @brief	紙オプション設定用パラメータを決定する
	 * @param[in]	inPaperPunchHole : パンチ穴用紙
	 * @param[in]	inPaperSewing : ミシン目用紙
	 * @param[in]	inPaperMarker : 事前マーク
	 * @param[in]	inPaperPrePrinted : 事前印刷
	 * @param[in]	inPaperMarkerOffset : マークオフセット（主走査）
	 * @param[in]	inPaperMarkSide : マークの印刷面
	 * @note	変換に失敗したときは例外を投げる
	**/
	void CPrintConditionRegister::ImportPaperOption(
		const CValue<bool>* inPaperPunchHole,
		const CValue<bool>* inPaperSewing,
		const CValue<bool>* inPaperMarker,
		const CValue<bool>* inPaperPrePrinted,
		const CValue<double>* inPaperMarkerOffset,
		const CValue<CPrintConditionForXML::EMarkSide>* inPaperMarkSide//,
	) throw (std::runtime_error);

	/**
	 * @brief	紙送り設定用パラメータを決定する
	 * @param[in]	inPaperPageFeedSize : 
	 * @param[in]	inPaperPageFeedSizeUnit : 
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperSizeH(
		const CValue<double>* inPaperPageFeedSize,
		const CValue<CPrintConditionForXML::EPageFeedUnit>* inPaperPageFeedSizeUnit
	) throw (std::runtime_error);


	/**
	 * @brief	紙DBへの印刷モード設定用データを決定する
	 * @param[in]	inPrintMode : 印刷モード名
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPrintMode(
		const CValue<std::string>* inPrintMode
	) throw (std::runtime_error);

	/**
	 * @brief	印刷速度設定用データを決定する
	 * @param[in]	inPrintSpeed : 印刷速度
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportModeResoSpeed(
		const CValue<double>* inPrintSpeed
	) throw (std::runtime_error);

	/**
	 * @brief	用紙節約モード用データを決定する
	 * @param[in]	inPaperSavingMode :用紙節約モード
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportPaperSavingMode(
		const CValue<bool>* inPaperSavingMode
	) throw (std::runtime_error);

	/**
	 * @brief	乾燥温度設定用データを決定する
	 * @param[in]	inHeatRollerSelf : 乾燥温度(自機)
	 * @param[in]	inHeatRollerPart : 乾燥温度(相手機)
	 * @param[in]	inHeatFanSelf : 温風アシスト乾燥温度（自機）
	 * @param[in]	inHeatFanPart : 温風アシスト乾燥温度（他機）
	 * @param[in]	inNIRPowerSelf : IR設定（自機）
	 * @param[in]	inNIRPowerPart : IR設定（他機）
	 * @param[in]	inNIRPowerSelf : 先塗り乾燥温度１
	 * @param[in]	inNIRPowerPart : 先塗り乾燥温度２
	 * @param[in]	inHeatRollerSelf : サブ乾燥温度(自機)
	 * @param[in]	inHeatRollerPart : サブ乾燥温度(相手機)
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportDryTempPrint(
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
	) throw (std::runtime_error);

	/**
	/**
	 * @brief	ICCプリセット設定用データを決定する
	 * @param[in]	inICCPresetName : ICCプリセット名
	 * @param[in]	inICCPresetID : ICCプリセットID
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportICCPreset(
		const CValue<std::string>* inICCPresetName,
		const CValue<std::string>* inICCPresetID
	) throw (std::runtime_error);

	/**
	 * @brief	トーンカーブ情報設定用データを決定する
	 * @param[in]	inToneCurve : トーンカーブ情報
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportDotgainCurve(
		const CValue<CPrintConditionForXML::ToneCurve>* inToneCurve
	) throw (std::runtime_error);

	/**
	 * @brief	紙DBへのジョブ基本情報設定用データを決定する
	 * @param[in]	inInformationSheet : インフォメーションシートを出力するかどうか
	 * @param[in]	inFlushingPage : フラッシングページを挿入するかどうか
	 * @param[in]	inPatternName : フラッシングページのパターン名
	 * @param[in]	inInterval : フラッシングページの挿入間隔
	 * @param[in]	inPreJob : ジョブ前に挿入するかどうか
	 * @param[in]	inPreJobCount : ジョブ前に挿入する回数
	 * @param[in]	inPostJob : ジョブ後に挿入するかどうか
	 * @param[in]	inOffsetFront : フラッシングページのオフセット（表面・副走査）[単位:0.001mm]
	 * @param[in]	inOffsetBack : フラッシングページのオフセット（裏面・副走査）[単位:0.001mm]
	 * @param[in]	inCleaningJudgmentLevelBeforeJob : ジョブ前のクリーニング判定レベル
	 * @param[in]	inCleaningJudgmentLevelAfterJob : ジョブ後のクリーニング判定レベル
	 * @param[in]	inPrintStopSetting : 印刷停止設定
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportJobBaseParameterWithPrintConditionName(
		const CValue<bool>* inInformationSheet,
		const CValue<bool>* inFlushingPage,
		const CValue<std::string>* inPatternName,
		const CValue<long>* inInterval,
		const CValue<bool>* inPreJob,
		const CValue<long>* inPreJobCount,
		const CValue<bool>* inPostJob,
		const CValue<long>* inOffsetFront,
		const CValue<long>* inOffsetBack,
		const CValue<long>* inCleaningJudgmentLevelBeforeJob,
		const CValue<long>* inCleaningJudgmentLevelAfterJob,
		const CValue<std::string>* inPrintStopSetting
	) throw (std::runtime_error);

	/**
	 * @brief	レイアウトイメージ情報設定用データを決定する
	 * @param[in]	inLayoutOffsetFront : 画像のオフセット（表面・主副）
	 * @param[in]	inLayoutOffsetBack : 画像のオフセット（裏面・主副）
	 * @param[in]	inLayoutVerification : サイド検査をするかどうか
	 * @param[in]	inLayoutContinuousPageCount : エラーにする連続シート数
	 * @param[in]	inLayoutCueMark : 印刷開始マークを出力するかどうか
	 * @param[in]	inLayoutBarcode : バーコードを出力するかどうか
	 * @param[in]	inLayoutBarcodeOffsetFront : バーコードのオフセット（表面・主走査。副走査は0 固定）
	 * @param[in]	inLayoutBarcodeOffsetBack : バーコードのオフセット（裏面・主副）
	 * @param[in]	inLayoutFlushingPattern : フラッシングパターン
	 * @param[in]	inLayoutGapBetweenColors : ラインフラッシングのずらし量
	 * @param[in]	inLayoutLineOffsetFront : ラインフラッシングのオフセット（表面・副走査）
	 * @param[in]	inLayoutLineOffsetBack : ラインフラッシングのオフセット（裏面・副走査）
	 * @param[in]	inLayoutCutterMark : カッターマークの印刷面
	 * @param[in]	inLayoutCutterMarkPageCount : カッターマークの準備シート数
	 * @param[in]	inLayoutCutterMarkSize : カッターマークのサイズ（主副
	 * @param[in]	inLayoutCutterMarkOffsetFront : カッターマークのオフセット（表面・主走査）
	 * @param[in]	inLayoutCutterMarkOffsetBack : カッターマークのオフセット（裏面・主走査）
	 * @param[in]	inLayoutCutterMarkAlignFront : カッターマークオフセットの基準位置（表面）
	 * @param[in]	inLayoutCutterMarkAlignBack : カッターマークオフセットの基準位置（裏面）
	 * @param[in]	inLayoutAdjustmentSheetSizeEnabled : 後処理マークの調整シートマークサイズ有効(TRUE)/無効(FALSE)
	 * @param[in]	inLayoutAdjustmentSheetLength : 後処理マークの調整シートマーク高さ
	 * @param[in]	inLayoutAdjustmentSheetAfterJob : 後処理マークのジョブ後のシート数を指定する方法を選択するオプション
	 * @param[in]	inLayoutAfterJobSheets : 後処理マークのジョブ後シート数
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportLayoutImageParameterWithPrintConditionName(
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
	) throw (std::runtime_error);

	/**
	 * @brief	検査パラメータを決定する
	 * @param[in]	inInspection : 検査条件詳細情報
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportInspection(
		const CValue<CPrintConditionForXML::Inspection>* inInspection,
		const CValue<std::string>* inJICondition,
		const CValue<std::string>* inTemporalMaskFile
	) throw (std::runtime_error);

	/**
	 * @brief	検査条件名パラメータを決定する
	 * @param[in]	inJICondition : 検査条件名
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportJICondition(
		const CValue<std::string>* inJICondition
	) throw (std::runtime_error);

	/**
	 * @brief	一時マスクファイルパスパラメータを決定する
	 * @param[in]	inTemporalMaskFile : 一時マスクファイルパス
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportTemporalMaskFile(
		const CValue<std::string>* inTemporalMaskFile
	) throw (std::runtime_error);

	/**
	 * @brief	デコードパラメータを決定する
	 * @param[in]	inDecode : デコード
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportDecode(
		const CValue<CPrintConditionForXML::Decode>* inDecode,
		const CValue<long>* inErrorSheets,
		const CValue<std::string>* inUnit,
		const CValue<bool>* inIsEnabled
	) throw (std::runtime_error);

	/**
	 * @brief	検査条件詳細情報をインポートする
	 * @param[in]		inVerificationParams : 検査条件詳細情報
	 * @param[in,out]	ioJiCondition : 検査条件名
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportVerificationParams(
		const CPrintConditionForXML::Inspection& inInspection,
		std::string& ioJiCondition
	) throw (std::runtime_error);

	/**
	 * @brief	一時マスクをエクスポートする
	 * @param[in]	inTemporalMask : 一時マスク
	 * @param[in,out]	ioTemporalMaskRelPath : 一時マスクファイルパス
	 * @note	変換に失敗したときは例外を投げる
	**/
	void ImportTemporalMask(
		const CPrintConditionForXML::Inspection& inInspection,
		std::string& ioTemporalMaskRelPath
	) throw (std::runtime_error);
	
	/**
	 * @brief	JI関連エレメントが存在するかどうか
	 * @param[in]	inXML : XML
	**/
	std::pair<bool,bool> hasJiElements(
		const CPrintConditionForXML& inXML
	);

	/**
	 * @brief	パンチフラグが有効かどうか
	 * @param[in]	inXML : XML
	**/
	bool getPunchPaperFlag(
		const CPrintConditionForXML& inXML
	);

	/**
	 * @brief	トーンカーブファイル作成する。既に登録済みのトーンの場合はファイルを作成せず情報を上書きする
	 * @param[in]	inSystemType : システムタイプの指標
	 * @note	エラー発生時は例外を投げる
	**/
	void createDotgainFile(eComSystemType inSystemType) throw (std::runtime_error);

	/**
	 * @brief	システムに未登録の項目があれば設定する
	**/
	void addSystemItem() throw();

	/**
	 * @brief	トーンカーブ情報設定用データを決定する
	 * @param[in]	inToneCurve : トーンカーブ情報
	 * @param[in]	inSystemType : システムタイプ
	 * @param[in]	inIsFront : 表面なら true、裏面なら false
	 * @note	エラー発生時は例外を投げる
	**/
	void importDotgainCurve(const CPrintConditionForXML::ToneCurve& inToneCurve, eComSystemType inSystemType, bool inIsFront) throw (std::runtime_error);

	/**
	 * @brief	トーンカーブのAllの値をセットする
	 * @param[in]	inSystemType : システムタイプの指標
	**/
	void setToneColorAll(eComSystemType inSystemType) throw();

	/**
	 * @brief	紙DBへ反映する
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	bool CommitToPaperDB() throw (int);

	/**
	 * @brief	デバッグ出力
	 * @param[in]	inDebugoutDB : 出力するDB
	 * @param[in]	inIsAppend : true/追加 false/上書き 
	**/
	void debugout(E_DEBUGOUT_DB inDebugoutDB, bool inIsAppend = true) throw(std::runtime_error);

	/**
	 * @brief	変倍パラメータを決定する
	 * @param[in]	inScalingValue : 変倍機能のON/OFF
	 * @param[in]	inScalingBaseSide : 基準面
	 * @param[in]	inScalingValueFrontW : 変倍値(表:横)
	 * @param[in]	inScalingValueFrontH : 変倍値(表:縦)
	 * @param[in]	inScalingValueBackW : 変倍値(裏:横)
	 * @param[in]	inScalingValueBackH : 変倍値(裏:縦)
	 * @param[in]	inIsDED : DEDかSESか(true:DED/false:SES)
	 * @param[out]	outRipMagnification : 値をセットする変倍パラメータ構造体
	 * @note	エラー時は例外を投げる
	*/
	void ImportScaling(
		const CValue<bool>* inScalingValue,
		const CValue<std::string>* inScalingBaseSide,
		const CValue<double>* inScalingValueFrontW,
		const CValue<double>* inScalingValueFrontH,
		const CValue<double>* inScalingValueBackW,
		const CValue<double>* inScalingValueBackH,
		const bool& inIsDED,
		ST_PDB_RIP_MAGNIFICATION& outRipMagnification
	) throw (std::runtime_error);

	std::string m_PrintConditionName;			//!< インポート対象の印刷条件名
	CDataSystemSetting& m_DataSystemSetting;	//!< システムのデータ
	std::vector<std::string> m_TempFileList;	//!< 作成したファイルの一覧(成功時はリストをクリアする。失敗時はデストラクタでファイル削除) 

	ST_PDB_MIGRATION_DATA m_DataForPDB;			//!< 紙DBとのやり取り用のデータ(初期値取得/印刷条件設定)

	std::string m_PrintModeName;				//!< 印刷モード名
	class API;
	bool m_isAppend;							//!< true:新規 false:上書き

	std::stringstream m_DebugoutFile;			//!< デバッグ用ファイル名

	bool m_isAddPaperType;						//!< true:用紙種類の新規追加 false:既存用紙種類を使用
	bool m_isAddLevelTuneName;						//!< true:段差補正の新規追加 false:既存段差補正を使用
};

