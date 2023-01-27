/**
 * @file	PrintConditionExporter_OP.h
 * @brief	PrintConditionExporter_OP
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/

#pragma once

#include "CommonDef_OP.h"
#include "DataIF.h"
#include "PrintConditionExporter.h"
#include "PaperDB_MigrationDef.h"

#define PLUGIN_VERSION 0x0000

/**
 * @brief PrintConditionExporter
**/
class CPrintConditionExporter_OP : public CPrintConditionExporter
{
public:
	/**
	 * @brief	constructor
	**/
	CPrintConditionExporter_OP();

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionExporter_OP();

	/**
	 * @brief	システムの印刷条件更新を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inMigrationData : 業務移行用構造体
	**/
	void onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData);

	/**
	 * @brief	システムの印刷条件削除を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName);

	/**
	 * @brief	JI、検査関連フォルダのエクスポート（ミラーリングコピー）
	 * @param[in]	検査条件フォルダ（Clientフォルダからの相対パス）
	**/
	bool onExportInspectionData(const std::string& inJIDataFolder);

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

