/**
 * @file	DataPrimer.h
 * @brief	data class: プライマー
**/

#pragma once

// ==========================================================================
// Include files
#include "DataIF.h"
#include "DataSystemSetting.h"

// ==========================================================================

/**
 * @brief	data class: プライマー
**/
class DataPrimer
{
public:
	//関数
	/**
		@brief	コンストラクタ
	**/
	DataPrimer();

	/**
		@brief	デストラクタ
	**/
	virtual ~DataPrimer();

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
		@param[in]	inPrintConditionOrSectionID : 印刷条件名orセクションID
		@retval	true : 成功
		@retval	false : 失敗
	*/
	bool EnterScreen(const char* inPrintConditionOrSectionID);

	/**
		@brief	ジョブ印刷条件設定コピーイベント
		@param[out]	outIsPrimer : プライマー
	*/
	void onCopyJobPrintCondition(bool& outIsPrimer);

	/**
		@brief	プライマーのON/OFFボタンか変更されたかどうか
		@retval	true : 変更有り
		@retval	false : 変更無し
	*/
	bool IsChanged();

	/**
		@brief	印刷条件の保存
		@retval	true : 成功
		@retval	false : 失敗
	*/
	bool Save();

	/**
		@brief	プライマーのON/OFFボタンのコントロール表示状態更新イベント
		@param[out]	outIsShow : プライマーのON/OFFボタンの表示状態
		@param[out]	outIsEnabled : プライマーのON/OFFボタンの有効状態
	*/
	void onView_Primer(bool& outIsShow, bool& outIsEnabled);

	/**
		@brief	印刷条件変更後の更新
		@param[in]	inPrintConditionOrSection : プライマーのON/OFFボタンの表示状態
	*/
	void UpdatePrintCondition(const char* inPrintConditionOrSection);

	/**
		@brief	プライマーのON/OFFボタンのボタン押下イベント
		@param[in]	inState : プライマーのON/OFFボタンの状態
	*/
	void onPush_Primer(bool inState);

	/**
		@brief	プライマーのON/OFFボタンのコントロール表示内容更新イベント
		@param[in]	outState : プライマーのON/OFFボタンの状態
	*/
	void onDraw_Primer(bool& outState);

private:
	//関数
	/**
		@brief	プライマーのON/OFFボタンの初期化
		@param[in]	inPrintConditionOrSectionID : 印刷条件名orセクションID
	*/
	void initializePrimer(const char* inPrintConditionOrSectionID);

	//変数
	CDataSystemSetting* m_SystemSetting;	//!< CDataSystemSettingクラス
	CDataIF* m_DataIF;						//!< CDataIFクラス
	string m_PrintConditionOrSectionID;		//!< 印刷条件名orセクションID

	bool m_IsPrimer;						//!< プライマーのON/OFF(true:ON, false:OFF)
	bool m_IsPrePrimer;						//!< 初期設定時のプライマーのON/OFF(true:ON, false:OFF)
};
