/**
 * @file	Stub_PaperDBIF.h
 * @brief	stub of paper DB plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "PaperDBDef_OP.h"
#include "PaperDB_MigrationDef.h"
#include "ThreadSafeShareLock.h"
#include <string>
#include <vector>

// ==========================================================================

/**
 * @brief	stub of paper DB plugin
**/
class CStub_PaperDBIF
{
	private:
	/**
	 * @brief	コンストラクタ
	**/
	CStub_PaperDBIF();

public:

	/**
	 * @brief	デストラクタ
	**/
	~CStub_PaperDBIF();

	/**
	 * @brief	GCS Get non inspection area list
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @param[out]	outNonInspectionArea : Non inspection area list
	 * @retval	true : 処理成功 / success
	 * @retval	false : 処理失敗 / failure
	**/
	static bool PDB_GetNonInspectionAreaList(const char* inPrintConditionName, eComSystemType inSystem, std::vector<STRUCT_NON_INSPECTION_AREA>& outNonInspectionArea);

	/**
	 * @brief	GCS Get non inspection area left and right
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @param[out]	outNonInspectionLeft : Non inspection area left
	 * @param[out]	outNonInspectionRight : Non inspection area right
	 * @retval	true : 処理成功 / success
	 * @retval	false : 処理失敗 / failure
	**/
	static bool PDB_GetNonInspectionAreaTable(const char* inPrintConditionName, eComSystemType inSystem, double& outNonInspectionLeft, double& outNonInspectionRight);

	/**
	 * @brief	ジョブのJI撮影情報を設定する(JI画面の一時マスク画面用)
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inSystem : 取得したい機種(COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART).
	 * @param[in]	inFileName : 画像ファイル名(jpeg)
	 * @param[in]	inResoX : 解像度(X)
	 * @param[in]	inResoY : 解像度(Y)
	 * @param[in]	inJobName : ジョブ名
	 * @param[in]	inDate : 撮影日時
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool PDB_SetJIPhotoOnJobPrint(std::string inPrintConditionName, eComSystemType inSystem, std::string inFileName, double inResoX, double inResoY, std::string inJobName, SYSTEMTIME inDate);

	/**
	 * @brief	ジョブのJI撮影情報を取得する(JI画面の一時マスク画面用)
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inSystem : 取得したい機種(COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART).
	 * @param[out]	outFileName : 画像ファイル名(jpeg)
	 * @param[out]	outResoX : 解像度(X)
	 * @param[out]	outResoY : 解像度(Y)
	 * @param[out]	outJobName : ジョブ名
	 * @param[out]	outDate : 撮影日時
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool PDB_GetJIPhotoOnJobPrint(std::string inPrintConditionName, eComSystemType inSystem, std::string& outFileName, double& outResoX, double& outResoY, std::string& outJobName, SYSTEMTIME& outDate);

	/**
	 * @brief	紙DBをコミットする。Commit the paper DB
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_CommitPaperDB();

	/**
	 * @brief	印刷条件追加 / Add a print condition
	 * @param[in]	inPrintConditionName : 追加紙 / Print condition to be added
	 * @param[in]	inSrcPaperName : ベースとなる紙 / Print condition to be copied. NULL if adding the first print condition.
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_AddPrintConditionName(const char* inPrintConditionName, const char* inSrcPaperName);

	/**
	 * @brief	印刷条件削除 / Delete the print condition
	 * @param[in]	inPrintConditionName : 削除紙 / Print condition to be deleted
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_DelPrintConditionName(const char* inPrintConditionName);

	/**
	 * @brief	スキャン画像ファイル名を取得する. / Get the name of scanned image from the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @param[out]	outScanFileName : 取得出来た値 / Name of the scanned image
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetScanFileName(const char* inPrintConditionName, eComSystemType inSystem, std::string& outScanFileName);

	/**
	 * @brief	スキャン画像ファイルパス削除
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_DelScanFileName(const char* inPrintConditionName, eComSystemType inSystem);

	/**
	 * @brief	GCS Set non inspection area left and right
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @param[in]	inNonInspectionLeft : Non inspection area left
	 * @param[in]	inNonInspectionRight : Non inspection area right
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_SetNonInspectionAreaTable(const char* inPrintConditionName, eComSystemType inSystem, double inNonInspectionLeft, double inNonInspectionRight);

	/**
	 * @brief	GCS Set non inspection area list
	  * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	* @param[in]	inSystem : 対象号機 / Target type（COM_SYSTEM_TYPE_SELF or COM_SYSTEM_TYPE_PART）
	 * @param[in]	inNonInspectionArea : Non inspection area list
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_SetNonInspectionAreaList(const char* inPrintConditionName, eComSystemType inSystem, const std::vector<STRUCT_NON_INSPECTION_AREA>& inNonInspectionArea);

	/**
	 * @brief	レイアウトイメージ情報を取得する./ Get the layout image information from the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inUnit : 単位 / Unit （UNIT_MODE_MM or UNIT_MODE_INCH or UNIT_MODE_POINT）
	 * @param[out]	outLayoutImagePosition : 画像データ情報 / Image position information
	 * @param[out]	outLayoutFlushingPattern : フラッシングパターン情報 / Flushing pattern information
	 * @param[out]	outLayoutFlushingPatternAdd : フラッシングパターンLineオプション情報 / Flushing pattern line option information
	 * @param[out]  outLayoutFlushingPatternAdd2 : フラッシングパターンLineオプション情報2 / Flushing pattern from 2nd line information
	 * @param[out]	outLayoutVerifyMark : 検査マーク情報 / Verify mark information
	 * @param[out]	outLayoutPostProcess : 後処理マーク情報 / Post process information
	 * @param[out]	outLayoutPostProcessAdd : Additional post process information (adjustment sheet after job and adjustment sheet mark size)
	 * @param[out]	outLayoutDecode : Decode information
	 * @param[out]  outEnableAlignBottomPPFLSPTN : The status to enable bottom alignment of PostProcess and Flushing Pattern
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetLayoutImageParameterWithPrintConditionNameEx(
		const char* inPrintConditionName,
		DEF_LENGTH_UNIT inUnit,
		ST_JOB_IMAGE_POSITION& outLayoutImagePosition,
		ST_JOB_FLUSHING_PATTERN& outLayoutFlushingPattern,
		ST_JOB_FLUSHING_PATTERN_ADD& outLayoutFlushingPatternAdd,
		ST_JOB_FLUSHING_PATTERN_ADD2& outLayoutFlushingPatternAdd2,
		ST_JOB_VERIFY_MARK& outLayoutVerifyMark,
		ST_JOB_POSTPROCESS& outLayoutPostProcess,
		ST_JOB_POSTPROCESS_ADD& outLayoutPostProcessAdd,
		ST_JOB_DECODE& outLayoutDecode,
		BOOL& outEnableAlignBottomPPFLSPTN);

	/**
	 * @brief	レイアウトイメージ情報を設定する。/ Set the layout image information to the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inUnit : 単位 / Unit （UNIT_MODE_MM or UNIT_MODE_INCH or UNIT_MODE_POINT）
	 * @param[in]	inLayoutImagePosition : 画像データ情報 / Image position information
	 * @param[in]	inLayoutFlushingPattern : フラッシングパターン情報 / Flushing pattern information
	 * @param[in]	inLayoutFlushingPatternAdd : フラッシングパターンLineオプション情報 / Flushing pattern line option information
	 * @param[in]	inLayoutFlushingPatternAdd2 : フラッシングパターンLineオプション情報2 / Flushing pattern from 2nd line information
	 * @param[in]	inLayoutVerifyMark : 検査マーク情報 / Verify mark information
	 * @param[in]	inLayoutPostProcess : 後処理マーク情報 / Post process information
	 * @param[in]	inLayoutPostProcessAdd : Additional post process information (adjustment sheet after job and adjustment sheet mark size)
	 * @param[in]	inLayoutDecode : Decode information
	 * @param[in]	inLayoutVertivalAlignmentPPFLSPTN : The status to enable bottom alignment of PostProcess and Flushing Pattern
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_SetLayoutImageParameterWithPrintConditionNameEx(
		const char* inPrintConditionName,
		DEF_LENGTH_UNIT inUnit,
		const ST_JOB_IMAGE_POSITION& inLayoutImagePosition,
		const ST_JOB_FLUSHING_PATTERN& inLayoutFlushingPattern,
		const ST_JOB_FLUSHING_PATTERN_ADD& inLayoutFlushingPatternAdd,
		const ST_JOB_FLUSHING_PATTERN_ADD2& inLayoutFlushingPatternAdd2,
		const ST_JOB_VERIFY_MARK& inLayoutVerifyMark,
		const ST_JOB_POSTPROCESS& inLayoutPostProcess,
		const ST_JOB_POSTPROCESS_ADD& inLayoutPostProcessAdd,
		const ST_JOB_DECODE& inLayoutDecode,
		const BOOL& inLayoutVertivalAlignmentPPFLSPTN);

	/**
	 * @brief	Get the path to temporal mask file
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[out]	outPath : path to temporal mask file
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetTemporalMaskFile(const char* inPrintConditionName, std::string& outPath);

	/**
	 * @brief	Set the path to temporal mask file
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition
	 * @param[in]	inPath : path to temporal mask file
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_SetTemporalMaskFile(const char* inPrintConditionName, const std::string& inPath);

	/**
	 * @brief	紙DBアクセスCS取得 / Acquire the Critical Section for paper DB access
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_EnterCommonPaperDBCS();

	/**
	 * @brief	紙DBアクセスCS開放 / Release the Critical Section for paper DB access
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_LeaveCommonPaperDBCS();

	/**
	 * @brief	指定の乾燥温度の存在チェック（システム）./ Check the existence of the dryer temperature information
	 * @param[in]	inDryTempPrint : 乾燥温度 / Dryer temperature information to be checked
	 * @retval	true : 存在する / Found 
	 * @retval	false : 存在しない / Not found
	**/
	static bool PDB_IsSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint);

	/**
	 * @brief	乾燥温度追加（システム）. / Add the dryer temperature information (to the list in the system)
	 * @param[in]	inDryTempPrint : 新規乾燥温度 / Dryer temperature information to be added
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_AddSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint);

	/**
	 * @brief	指定のトーンカーブ情報の存在チェック（システム）./ Check the existence of the tone curve information
	 * @param[in]	inDotgainCurve : トーンカーブ情報 / Tone curve information to be checked
	 * @retval	true : 存在する / Found 
	 * @retval	false : 存在しない / Not found
	**/
	static bool PDB_IsSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);

	/**
	 * @brief	トーンカーブ情報追加（システム）./ Add the tone curve (to the list in the system)
	 * @param[in]	inDotgainCurve : 新規トーンカーブ情報 / Tone curve to be added
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_AddSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);

	/**
	 * @brief	トーンカーブ情報を全レコードを呼び出し順に取得（システム）./ Inquire tone curve information one by one
	 *           前後でCS取得、解放を行う事. / Acquire the Critical Section beforehand and release it afterward
	 *           必ず失敗するまで実行する必要がある / Repeat calling the function until false is returned.
	 * @param[in]	index : 0からのインデックス値 / Index (starting from 0)
	 * @param[out]	outDotgainCurve : 取得出来たトーンカーブ情報 / Tone curve information
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetSystemDotgainCurve(int index, ST_PDB_DOTGAIN_CURVE& outDotgainCurve);

	/**
	 * @brief	印刷条件の業務移行情報を設定する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inMigrationData : 業務移行情報
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_SetPrintConditionMigration(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData);

	/**
	 * @brief	印刷条件の業務移行情報を取得する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inLengthUnit : 長さ単位（DEF_LENGTH_MM or DEF_LENGTH_INCH or DEF_LENGTH_POINT）
	 * @param[in]	inSpeedUnit : 速度の単位（DEF_MODE_RESO_SPEED_M or DEF_MODE_RESO_SPEED_100）
	 * @param[out]	outMigrationData : 業務移行情報
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_GetPrintConditionMigration(const std::string& inPrintConditionName, DEF_LENGTH_UNIT inLengthUnit, 
		eMODE_RESO_SPEED_UNIT inSpeedUnit, ST_PDB_MIGRATION_DATA& outMigrationData);

	/**
	 * @brief	インポート・エクスポート用の排他処理（読み書き禁止）を開始する
	 * @param[out]	outLockObj : 排他制御のオブジェクト
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	 * @note	上位（取得先）でこの排他制御のオブジェクトが破棄されたら自動開放
	**/
	static bool PDB_TryUniqueLock_forImportExport(NS_SRWLock::CUniqueTransaction<bool>& outLockObj);

	/**
	 * @brief	指定の印刷条件の存在チェック. / Check the existence of the print condition
	 * @param[in]	inPrintConditionName : 印刷条件名 / Print condition to be checked
	 * @retval	true : 存在する / Found
	 * @retval	false : 存在しない / Not found
	**/
	static bool PDB_IsPrintConditionName(const char* inPrintConditionName);

	/**
	 * @brief	印刷条件取り込みの(開始/終了)の通知 / Print condition import notification
	 * @param[in]	inIsStauts : 開始 or 終了状態 / Start or End status
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool PDB_OnPrintConditionImport(bool inIsStauts);

	/**
	 *  @brief 指定の用紙種類の存在チェック（システム）./ Check the existence of the paper type
	 *  @param inPaperTyp	[IN]用紙種類 / Paper type to be checked
	 *  @return true：存在する / Found、false：存在しない / Not found
	 */
	static bool PDB_IsSystemPaperType(const char* inPaperType);

	/**
	 *  @brief 用紙種類を取得する。/ Get the paper type from the print condition
	 *  @param inPrintConditionName [IN]印刷条件名 / Print condition
	 *  @param outPaperTyp	[OUT]用紙種類 / Paper type
	 *  @return true：成功 / Success、false：失敗 / Failure.
	 */
	static bool PDB_GetPaperType(const char* inPrintConditionName, std::string& outPaperType);

	/**
	 *  @brief 指定の段差設定の存在チェック（システム）./ Check the existence of the printhead alignment
	 *  @param inLevelTuneName	[IN]段差設定 / Printhead alignment to be checked
	 *  @return true：存在する / Found、false：存在しない / Not found
	 */
	static bool CStub_PaperDBIF::PDB_IsSystemLevelTuneName(const char* inLevelTuneName);

	/**
	 *  @brief 用紙種類追加（システム）
	 *  @param inPaperType	[IN]新規用紙種類
	 *  @return true：成功 / Success、false：失敗 / Failure.
	 */
	static bool PDB_AddSystemPaperType(const char* inPaperType);

	/**
	 *  @brief 段差設定追加（システム）.
	 *  @param inLevelTuneName	[IN]新規段差設定
	 *  @return true：存在する / Found、false：存在しない / Not found
	 */
	static bool PDB_AddSystemLevelTuneName(const char* inLevelTuneName);

};