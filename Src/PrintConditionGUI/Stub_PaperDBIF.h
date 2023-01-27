/*! \file Stub_PaperDBIF.h
 *  \brief Stub_PaperDBIF.hの処理
 *  \author omori
 *  \date 2019/03/04 omori 新規作成
*/


#pragma once
#include "Stub_PaperDBIF.h"
/**
	@brief	stub of jobmanager plugin
*/
class Stub_PaperDBIF
{
private:
	/**
		@brief	constructor
	*/
	Stub_PaperDBIF();
public:
	/**
		@brief	destructor
	*/
	~Stub_PaperDBIF();

	/**
	@brief	JI検査条件を取得する
	@param[in]	inPrintConditionName : 印刷条件名
	@param[out]	outJICondition : JI印刷条件
	@retval	true : 成功
	@retval	false : 失敗
	*/
	static bool GetJICondition(const char* inPrintConditionName, std::string& outJICondition);

	/**
	@brief	画像保管を設定する
	@param[in]	inPrintConditionName : 印刷条件名
	@param[in]	inIsImageStorage : 画像保管の有効無効(true:On、false:Off)
	@retval	true : 成功
	@retval	false : 失敗
	*/
	static bool SetImageStorage(const std::string& inPrintConditionName, const bool inIsImageStorage);


	/**
	@brief	画像保管を取得する
	@param[in]	inPrintConditionName : 印刷条件名
	@retval	true : On
	@retval	false : Off
	*/
	static bool IsImageStorage(const std::string& inPrintConditionName);

	/**
	 * @brief	システムの印刷条件から変倍パラメータを取得する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[out]	outRipMagnificationData : 変倍パラメータのデータ
	 * @return	処理の成否(紙DBから取得できたかどうか)を返す
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool GetRipMagnification(const std::string& inPrintConditionName, ST_PDB_RIP_MAGNIFICATION& outRipMagnificationData);

	/**
	 * @brief	システムの印刷条件に変倍パラメータを設定する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inRipMagnificationData : 設定する変倍パラメータのデータ
	 * @return	処理の成否(紙DBに保存できたかどうか)を返す
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool SetRipMagnification(const std::string& inPrintConditionName, const ST_PDB_RIP_MAGNIFICATION& inRipMagnificationData);

	/**
	 * @brief	紙DBをコミットする
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool CommitPaperDB();

	/**
		@brief	システムの印刷条件にプライマーのON/OFFを設定する
		@param[in]	inPrintConditionName : 印刷条件名
		@param[in]	inIsPrimerSetting : プライマーのON/OFF設定(true:ON、false:OFF)
		@retval	true : 成功
		@retval	false : 失敗
	**/
	static bool SetPrimerStatus(const std::string& inPrintConditionName, const bool& inIsPrimerSetting);

	/**
		@brief	システムの印刷条件からプライマーのON/OFFを取得する
		@param[in]	inPrintConditionName : 印刷条件名
		@param[out]	outIsPrimerSetting : プライマーのON/OFF設定(true:ON、false:OFF)
		@retval	true : 成功
		@retval	false : 失敗
	**/
	static bool GetPrimerStatus(const std::string& inPrintConditionName, bool& outIsPrimerSetting);
};
