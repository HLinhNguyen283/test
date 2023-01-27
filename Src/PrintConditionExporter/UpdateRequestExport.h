/*! \file UpdateRequestExport.h
 *  \brief UpdateRequestExport.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/19 pt.ju-tanaka 新規作成
*/


#pragma once

#include "Common.h"
#include "ThreadSafe.h"
#include "PaperDB_MigrationDef.h"


/**
 * @brief 依頼リストデータ構造体
**/
typedef struct _SRequestData
{
	std::string				conditionName;		//!< 印刷条件名
	ST_PDB_MIGRATION_DATA	migrationData;		//!< 業務移行用構造体
	bool					isUpdate;			//!< 更新 or 削除指定
	std::string				localFileName;		//!< 一時ファイル名

	_SRequestData()
	{
		isUpdate = false;
	}
}SRequestData;


/**
 * @brief エクスポート更新要求に対する処理を行う
**/
class UpdateRequestExport
{
public:
	/**
	 * @brief	constructor
	**/
	UpdateRequestExport();

	/**
	 * @brief	destructor
	**/
	virtual ~UpdateRequestExport();

	/**
	 * @brief	初期化イベント
	**/
	bool onInitialize(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	エクスポート更新イベント
	**/
	void onUpdateExport(ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	システムの印刷条件更新を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inMigrationData : 業務移行用構造体
	**/
	void onUpdatePrintCondition(
		const std::string& inPrintConditionName, 
		const ST_PDB_MIGRATION_DATA& inMigrationData, 
		ST_THREAD_INFO* inUpdateThread);

	/**
	 * @brief	システムの印刷条件削除を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName, ST_THREAD_INFO* inUpdateThread);

private:
	/**
	 * @brief	印刷条件のエクスポート依頼
	 * @param[in]	inCondiotionName : 印刷条件名
	 * @param[in]	inMigrationData : 印刷条件データ
	 * @param[in]	inIsUpdate : 更新指定(true:更新 false:削除)
	 * @retval	true: エラー無し
	 * @retval	false: 取り込み元パスが不正
	**/
	void requestPrintCondionExport(
		const std::string& inCondiotionName, 
		const ST_PDB_MIGRATION_DATA& inMigrationData, 
		const bool inIsUpdate);

	/**
	 * @brief	EQ制御サーバ上の共有フォルダの初期化
	**/
	bool initEQFolder();
	

	/**
	 * @brief	依頼リストから重複する印刷条件を削除する
	 * @param[in,out]	ioRequests : 依頼リスト
	**/
	void deleteDuplicateCondition(std::list<SRequestData>& ioRequests);

	/**
	 * @brief	文字列を置換する
	 * @param[in]	inSrc : 変換対象の文字列
	 * @param[in]	inDst : 変換後の文字列
	 * @param[in,out]	ioLog : 文字列
	 * @param[in]	inPos : 開始位置(省略時は先頭から)
	**/
	void replaceString(const std::string& inSrc, const std::string& inDst, std::string& ioLog, const size_t inPos = 0) const;


	CThreadSafe<std::list<SRequestData>> m_Requests;	//!< 依頼一覧
	std::string m_PCNameEQ;			// EQ管理サーバーのPC名
	bool m_IsDualSimplex;			// DualSimplex環境設定が有効かどうか
	BOOL m_IsDED;					// DED構成かどうか
	std::string m_IpAddress;		// 自機のIPアドレス
	std::string m_IpAddressPart;	// 相手機のIPアドレス
	std::string m_OutputRootFolder;	// EQ制御サーバー上にある出力先のルートフォルダ
	std::string m_ExportFolder;		// EQ制御サーバー上にある出力先フォルダ
};