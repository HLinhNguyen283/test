/*! \file DataRequestExportXML.h
 *  \brief DataRequestExportXML.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
*/

#pragma once

// ==========================================================================
// Include files
#include "Common.h"
#include "ThreadSafe.h"
#include "PaperDB_MigrationDef.h"
#include "ExportUtility.h"
#include <list>

// ==========================================================================

/*** @brief XMLエクスポート更新要求に対する処理を行う
**/
class DataRequestExportXML
{
public:
	/**
	 * @brief	constructor
	**/
	DataRequestExportXML();

	/**
	 * @brief	destructor
	**/
	virtual ~DataRequestExportXML();

	/**
	 * @brief	初期化イベント
	**/
	bool onInitialize();

	/**
	 * @brief	エクスポート更新イベント
	**/
	void onUpdateXMLExport(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	システムの印刷条件更新を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inPaperType : 用紙種類
	 * @param[in]	inMigrationData : 業務移行用構造体
	**/
	void onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData);

	/**
	 * @brief	システムの印刷条件削除を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName);

	/**
	 * @brief	JI、検査関連フォルダのミラーリングコピー
	 * @param[in]	JI検査関連フォルダ（Clientフォルダからの相対パス）
	**/
	bool MirroringInspectionData(const std::string& inJIDataFolder);

private:
	/**
	 * @brief	EQ制御サーバ上の共有フォルダの初期化
	**/
	bool initEQFolder();

	/**
	 * @brief	依頼リストから重複する印刷条件を削除する
	 * @param[in,out]	ioRequests : 依頼リスト
	**/
	void deleteDuplicateCondition(std::list<SRequestConditionData>& ioRequests);

	/**
	 * @brief	印刷条件のエクスポート依頼
	 * @param[in]	inRequestConditionData : 依頼リストデータ構造体
	 * @retval	true: エラー無し
	 * @retval	false: 取り込み元パスが不正
	**/
	void requestXMLPrintCondionExport(SRequestConditionData& inRequestConditionData);

	DWORD runSystemCmd(const std::string& inCmd);
	bool mirrorFolder(const std::string& inSrcPath, const std::string& inDestPath);
	std::string getLastError();
	
	CThreadSafe<std::list<SRequestConditionData>> m_RequestsXML;	//!< 依頼一覧
};