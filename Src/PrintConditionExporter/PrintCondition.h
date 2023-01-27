/**
 * @file	PrintCondition.h
 * @brief	紙DBの印刷条件1件分の情報
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include "Common.h"
#include "PaperDB_MigrationDef.h"

#include <list>
#include <string>

/**
 * 紙DBの印刷条件1件分の情報
**/
class CPrintCondition
{
public:
	std::string m_printConditionName;			//!< GetNext()時の印刷条件名
	std::string m_paperType;					//!< GetNext()時の用紙種類
	ST_PDB_MIGRATION_DATA m_migrationData;		//!< GetNext()時の各レコード

private:
	std::list<std::string> m_printConditionNames;	//!< 全印刷条件名(このクラスオブジェクト作成時点の一覧を持つ)
	std::list<std::string>::iterator m_it;			//!< 次にGetNext()する時のm_printConditionNamesの項目
public:
	/**
	 * コンストラクタ
	**/
	CPrintCondition();

	/**
	 * デストラクタ
	**/
	virtual ~CPrintCondition();

	/**
	 * 残りの印刷条件名がなくなったか判定
	 * @retval	true : なくなった
	 * @retval	false : まだある
	**/
	bool IsEndOfList();

	/**
	 * 次の印刷条件名の各レコードを取得する
	 * @retval	true : 成功
	 * @retval	false : 失敗(もう印刷条件名がない)
	**/
	bool GetNext();

private:
	class API;

};


