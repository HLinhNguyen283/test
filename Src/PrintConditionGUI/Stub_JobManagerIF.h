/**
	@file	Stub_JobManagerIF.h
	@brief	stub of jobmanager plugin
	@author	to-hamaguchi
	@date	2017/09/13 to-hamaguchi 新規作成
*/

#pragma once
#include "JobDef_PrintCondition_OP.h"

/**
	@brief	stub of jobmanager plugin
*/
class Stub_JobManagerIF
{
private:
	/**
		@brief	constructor
	*/
	Stub_JobManagerIF();
public:
	/**
		@brief	destructor
	*/
	~Stub_JobManagerIF();

	/**
		@brief	ジョブ印刷条件設定コピー依頼
		@param[in]	inCopyTargetJobPrintCondition : コピー対象のジョブ印刷条件設定
		@param[out]	outIsShowCopyMessage : コピー確認ダイアログ表示結果(true: 表示された, false: 表示されていない)
	*/
	static void OnBeforeSaveJobPrintCondition(PrintConditionGUI::ST_COPY_TARGET_JOB_PRINT_CONDITION inCopyTargetJobPrintCondition, bool &outIsShowCopyMessage);

	/**
	@brief	JI検査条件を設定する。
	@param[in]	inSectionID : セクションID
	@param[in]	inJICondition : JI検査条件
	@param[in]	inIsImageStorage : 画像保管
	@retval	true : 成功
	@retval	false : 失敗
	*/
	static bool SetJICondition(const char* inSectionID, std::string inJICondition, const bool inIsImageStorage);


	/**
	@brief	JI検査条件を取得する。
	@param[in]	inSectionID : セクションID
	@param[out]	outJICondition : JI検査条件
	@param[out]	outIsImageStorage : 画像保管
	@retval	true : 成功
	@retval	false : 失敗
	*/
	static bool GetJICondition(const char* inSectionID, std::string& outJICondition, bool& outIsImageStorage);

	/**
		@brief	プライマーの接続状態を設定する
		@param[in]	inSectionID : セクションID
		@param[in]	inIsPrimer : プライマーの接続状態（true: ON、false: OFF）
	*/
	static void SetPrimerStatus(const std::string& inSectionID, bool inIsPrimer);

	/**
		@brief	プライマーの接続状態を取得する
		@param[in] : inSectionID : セクションID
		@param[out] : outIsPrimer : プライマーの接続状態（true: ON、false: OFF）
		@return true : 成功 / Success
		@return false : 失敗 / Failure
	*/
	static bool GetPrimerStatus(const std::string& inSectionID, bool& outIsPrimer);

	/**
		@brief	印刷予約ジョブかどうか取得する
		@param[in]	inSectionID : セクションID
		@param[in]	inIsSetupEditingData : セットアップ編集用ジョブを操作するかどうか.
		@retval RipState
	**/
	static bool GetReservePrintMode(const char* inSectionID, bool inIsSetupEditingData);

	/**
		@brief	事前RIPステータスを取得する
		@param[in]	inSectionID : 対象ジョブのセクションID / Section ID
		@param[out]	outPreRIPStatus : 事前RIPステータス。
		@param[in]	inIsSetupEditingData : セットアップ編集用ジョブを操作するかどうか.
		@retval	true : 成功 / Success
		@retval	false : 失敗 / Failure
	**/
	static bool GetPreRIPStatus(const char* inSectionID, ENUM_PRE_RIP_PROGRESS& outPreRIPStatus, bool inIsSetupEditingData);

	/**
	 * @brief	ジョブから変倍パラメータを取得する
	 * @param[in] inSectionID : セクションID
	 * @param[out]	outRipMagnificationData : 変倍パラメータのデータ
	 * @return	処理の成否(ジョブから取得できたかどうか)を返す
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool GetRipMagnification(const std::string& inSectionID, ST_JOB_RIP_MAGNIFICATION& outRipMagnificationData);

	/**
	 * @brief	ジョブの変倍パラメータを更新する
	 * @param[in] inSectionID : セクションID
	 * @param[in] inRipMagnificationData : 設定する変倍パラメータのデータ
	 * @return	処理の成否(ジョブに設定できたかどうか)を返す
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool UpdateRipMagnification(const std::string& inSectionID, const ST_JOB_RIP_MAGNIFICATION& inRipMagnificationData);

	/*
	 * 印刷面を取得する
	 * @param inSectionID [IN] セクションID / Section ID
	 * @param outSidePrint[OUT] 印刷面 / PrintSide
	 * @param[in]	inIsSetupEditingData : セットアップ編集用ジョブを操作するかどうか.
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	*/
	static bool GetSidePrint(const char* inSectionID, DEF_PRINT_SIDE& outSidePrint, bool inIsSetupEditingData);

	/**
	 * ジョブの印刷条件設定の編集済みフラグ設定。
	 * @param[in]	inSectionID : セクションID
	 * @param[out]	inEditFlag : 編集フラグ(TRUE:編集済み, FALSE:未編集)
	 * @retval	true : 成功 / Success
	 * @retval	false : 失敗 / Failure
	**/
	static bool SetPrintConditionEditFlag(const char* inSectionID, BOOL inEditFlag);

	/**
	 * ジョブファイル更新
	 * @param[in]	inSectionID : 対象ジョブのセクションID / Section ID of the target job
	 * @retval	PLUGERR_NONE : 成功 / Success
	 * @retval	PLUGERR_NONE以外 : 失敗 / Failure
	**/
	static long SaveJobFile(const char* inSectionID);
};
