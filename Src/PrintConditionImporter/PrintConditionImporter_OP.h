/**
 * @file	PrintConditionImporter_OP.h
 * @brief	PrintConditionImporter_OP
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
**/

#pragma once

#include "CommonDef_OP.h"
#include "DataIF.h"
#include "PrintConditionImporter.h"

#define PLUGIN_VERSION 0x0000

/**
 * @brief PrintConditionImporter
**/
class CPrintConditionImporter_OP : public CPrintConditionImporter
{
public:
	/**
	 * @brief	constructor
	**/
	CPrintConditionImporter_OP();

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionImporter_OP();

	/**
	 * @brief	指定された印刷条件名がインポート時のベースとなるものか判定する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @retval	true : ベースである
	 * @retval	false : ベースではない
	**/
	bool IsBasePrintCondition(const std::string& inPrintConditionName);

protected:
	/**
	 * @brief	UI_MODULE_INFOの構築
	 *  
	 * プラグインモジュール構成情報をセットする。
	 * @param[out]	pModData : モジュール構成情報へのポインタ
	 * @return	PLUGERR_NONE
	**/
	virtual long BuildUiModuleInfo(MODULE_INFO *pModData);

	/**
	 * @brief	システム毎のコールバック関数を設定する
	 * @param[in]	inHostInfo : ホスト情報
	**/
	virtual void SetSystemCallBackFunc(HOST inHostInfo);

	/**
	 * @brief	データI/Fクラスインスタンスを作成する
	 * @retval	TRUE : 成功
	 * @retval	FALSE : 失敗
	**/
	virtual BOOL CreateDataIF();

	/**
	 * @brief	プリンタI/Fクラスインスタンスを作成する
	 * @retval	TRUE : 成功
	 * @retval	FALSE : 失敗
	**/
	virtual BOOL CreatePrinterIF();

	CDataIF* m_dataIF;					//!< DataIFクラスポインタ.
};

