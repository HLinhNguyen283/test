/**
 * @file	DataVariableMagnification.h
 * @brief	data class: 変倍
**/

#pragma once

// ==========================================================================
// Include files
#include "DataSystemSetting.h"
#include "PaperDBDef_OP.h"

// ==========================================================================

/**
 * @brief	data class: 変倍
**/
class DataVariableMagnification
{
public:
	//関数
	/**
		@brief	コンストラクタ
	**/
	DataVariableMagnification();

	/**
		@brief	デストラクタ
	**/
	virtual ~DataVariableMagnification();

	/**
		@brief	初期化
		@param[in]	inSystemSetting : CDataSystemSettingクラスインスタンス
		@param[in]	inDataIF : CDataIFクラスインスタンス
		@retval	true : 成功
		@retval	false : 失敗
	*/
	bool Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF);

	/**
		@brief	印刷条件画面表示
		@param[in]	inPrintConditionOrSectionID : 印刷条件名 or セクションID
		@retval	true : 成功
		@retval	false : 失敗
	*/
	bool EnterScreen(const char* inPrintConditionOrSectionID);

	/**
		@brief	印刷条件変更後の更新
		@param[in]	inPrintConditionOrSection : 印刷条件名 or セクションID
	*/
	void UpdatePrintCondition(const char* inPrintConditionOrSection);

	/**
		@brief	変倍ボタンのコントロール表示状態更新イベント
		@param[out]	outIsShow : 変倍ボタンの表示状態
		@param[out]	outIsEnabled : 変倍ボタンの有効状態
	*/
	void onView_VariableMagnification(bool& outIsShow, bool& outIsEnabled);

	/**
		@brief	変倍値表示フィールドのコントロール表示状態更新イベント
		@param[out]	outIsShowFront : 表面の変倍値フィールドの表示状態
		@param[out]	outIsShowBack :  裏面の変倍値フィールドの表示状態
	*/
	void onView_VariableMagnificationField(bool& outIsShowFront, bool& outIsShowBack);

	/**
		@brief	RIP変倍値設定のコントロール表示状態更新イベント
		@param[out]	outIsShow : 変倍入力の表示状態
	*/
	void onView_RipVariableMagnification(bool& outIsShow);

	/**
		@brief	変倍入力のコントロール表示状態更新イベント
		@param[out]	outIsShowFront : 表面の変倍入力の表示状態
		@param[out]	outIsShowBack  : 裏面の変倍入力の表示状態
		@param[out]	outIsEnabled   : 変倍入力の有効状態
	*/
	void onView_VariableMagnificationInput(bool& outIsShowFront, bool& outIsShowBack, bool& outIsEnabled);

	/**
		@brief	基準面指定のコントロール表示状態更新イベント
		@param[out]	outIsEnabled : 基準面指定の表示状態
	*/
	void onView_VariableMagnificationReferencePlane(bool& outIsShow);

	/**
		@brief	登録ボタンのコントロール表示状態更新イベント
		@param[out]	outIsEnabled : 登録ボタンの有効状態
	*/
	void onView_VariableMagnificationCommit(bool& outIsEnabled);

	/**
		@brief	変倍値表示フィールドのコントロール表示内容更新イベント
		@param[out]	outFrontX : 横の変倍値(表面)
		@param[out]	outFrontY : 縦の変倍値(表面)
		@param[out]	outBackX : 横の変倍値(裏面)
		@param[out]	outBackY : 縦の変倍値(裏面)
	*/
	void onDraw_VariableMagnificationField(std::string& outFrontX, std::string& outFrontY, std::string& outBackX, std::string& outBackY);

	/**
		@brief	RIP変倍値設定のコントロール表示内容更新イベント
		@param[out]	outIsRipVariableMagnification : 編集中のRIP変倍値設定状態(ture:ON、false:OFF)
	*/
	void onDraw_RipVariableMagnification(bool& outIsRipVariableMagnification);

	/**
		@brief	基準面指定のコントロール表示内容更新イベント
		@param[out]	outIsEditDatumPlaneFront : 編集中の基準面が表面かどうか(true:表面、false:裏面)
	*/
	void onDraw_VariableMagnificationReferencePlane(bool& outIsEditDatumPlaneFront);

	/**
		@brief	変倍入力のコントロール表示内容更新イベント
		@param[out]	outFrontX : 編集中の横の変倍値(表面)
		@param[out]	outFrontY : 編集中の縦の変倍値(表面)
		@param[out]	outBackX : 編集中の横の変倍値(裏面)
		@param[out]	outBackY : 編集中の縦の変倍値(裏面)
		@param[out]	outIsEditRipVariableMagnification : 編集中のRIP変倍値指定(true:ON、false:OFF)
	*/
	void onDraw_VariableMagnificationInput(
		std::string& outFrontX, 
		std::string& outFrontY, 
		std::string& outBackX, 
		std::string& outBackY, 
		bool& outIsEditRipVariableMagnification);

	/**
		@brief	変倍ボタンのコントロール操作イベント
		@param[in]	inData : CBaseDataIFクラスインスタンス
		@param[in]	inPrinter : CBasePrinterIFクラスインスタンス
	*/
	void onPush_VariableMagnification(CBaseDataIF* inData, CBasePrinterIF* inPrinter);

	/**
		@brief	RIP変倍値設定のコントロール操作イベント
		@param[in]	inIsRipVariableMagnification : 編集中のRIP変倍値設定(true:ON、false:OFF)
	*/
	void onPush_RipVariableMagnification(bool inIsRipVariableMagnification);

	/**
		@brief	基準面設定のコントロール操作イベント
	*/
	void onPush_VariableMagnificationReferencePlane();

	/**
		@brief	変倍入力のコントロール操作イベント
		@param[out]	inFrontX : 編集中の横の変倍値(表面)
		@param[out]	inFrontY : 編集中の縦の変倍値(表面)
		@param[out]	inBackX : 編集中の横の変倍値(裏面)
		@param[out]	inBackY : 編集中の縦の変倍値(裏面)
	*/
	void onPush_VariableMagnificationInput(
		const long long& inFrontX, 
		const long long& inFrontY, 
		const long long& inBackX, 
		const long long& inBackY);

	/**
		@brief	変倍登録ボタンのコントロール操作イベント
	*/
	void onPush_VariableMagnificationCommit();

	/**
		@brief	変倍終了ボタンのコントロール操作イベント
		@retval	true : 未保存の設定有り
		@retval	false : 未保存の設定無し
	*/
	bool onPush_VariableMagnificationClose();

	/**
		@brief	変倍ダイアログを初期化する
	*/
	void initializeDlgVariableMagnification();

	/**
		@brief	編集中の変倍値を取得する
		@param[out]	outFrontX : 編集中の横の変倍値(表面)
		@param[out]	outFrontY : 編集中の縦の変倍値(表面)
		@param[out]	outBackX : 編集中の横の変倍値(裏面)
		@param[out]	outBackY : 編集中の縦の変倍値(裏面)
	*/
	void getVariableMagnificationInput(long long& outFrontX, long long& outFrontY, long long& outBackX, long long& outBackY);

private:
	//関数
	/**
		@brief	自動変倍機能が有効か取得する
		@retval	true : 有効
		@retval	false : 無効
	*/
	bool isAutoVariableMagnification();

	/**
		@brief	変倍パラメータを初期化する
	*/
	void initializeVariableMagnificationValue();

	/**
		@brief	変倍入力値を範囲内(10.000%～999.999%)に補正する
		@param[out]	inInputValue : 補正前の値
		@retval	: 補正後の値
	*/
	long inputCorrection(long long inInputValue);

	//変数
	CDataSystemSetting* m_SystemSetting;					//!< CDataSystemSettingクラス
	CDataIF* m_DataIF;										//!< CDataIFクラス

	std::string m_PrintConditionOrSectionID;				//!< 印刷条件名 or セクションID
	ST_PDB_RIP_MAGNIFICATION m_PDBRipMagnificationInfo;		//!< 変倍パラメータ構造体(PDB)
	ST_JOB_RIP_MAGNIFICATION m_JobRipMagnificationInfo;		//!< 変倍パラメータ構造体(Job)
	bool m_IsEditRipVariableMagnification;					//!< 編集中のRIP変倍値指定(true:ON、false:OFF)
	long m_EditFrontX;										//!< 編集中の横の変倍値(表面)
	long m_EditFrontY;										//!< 編集中の縦の変倍値(表面)
	long m_EditBackX;										//!< 編集中の横の変倍値(裏面)
	long m_EditBackY;										//!< 編集中の縦の変倍値(裏面)
	bool m_IsEditDatumPlaneFront;							//!< 編集中の基準面が表面かどうか(true:表面、false:裏面)
};
