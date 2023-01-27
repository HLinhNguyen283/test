/**
 * @file	PrintCondition.cpp
 * @brief	紙DBの印刷条件1件分の情報
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintCondition.h"
#include "ThreadSafeShareLock.h"
#include "Stub_PaperDBIF.h"
#include "Utility.h"

#include <list>
#include <string>
// ==========================================================================
/**
 * @brief	CPrintConditionが使うAPI
**/
class CPrintCondition::API
{
public:
	//業務移行用データ取得
	static bool GetPrintConditionMigration
		(	
			const std::string&		inPrintConditionName,
			DEF_LENGTH_UNIT			inLengthUnit, 
			eMODE_RESO_SPEED_UNIT	inSpeedUnit, 
			ST_PDB_MIGRATION_DATA&	outMigrationData,
			std::string&			outPaperType
		)
	{
		bool res1 = CStub_PaperDBIF::PDB_GetPrintConditionMigration(inPrintConditionName, inLengthUnit, inSpeedUnit, outMigrationData);

		bool res2 = CStub_PaperDBIF::PDB_GetPaperType(inPrintConditionName.c_str(), outPaperType);


		return res1 && res2;
	}
	
	//インポート・エクスポート用の排他処理（読み書き禁止）を開始する
	static NS_SRWLock::CUniqueTransaction<bool> UniqueLock()
	{
		NS_SRWLock::CUniqueTransaction<bool> lockObj;
		CStub_PaperDBIF::PDB_UniqueLock_forImportExport(lockObj);
		
		return lockObj;
	}

	//全印刷条件名取得
	static std::list<std::string> GetAllPrintConditionNames()
	{
		std::list<std::string> printConditionNames;
		CStub_PaperDBIF::PDB_EnterCommonPaperDBCS();
		
		std::string name;
		while(CStub_PaperDBIF::PDB_GetPrintConditionName(name) == true)
		{
			printConditionNames.push_back(name);
		}
	
		CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS();

		return std::move(printConditionNames);
	}

};

//コンストラクタ
CPrintCondition::CPrintCondition() : 
	m_printConditionNames(API::GetAllPrintConditionNames()),
	m_it(m_printConditionNames.begin())
{


}

//デストラクタ
CPrintCondition::~CPrintCondition()
{

}

//残りの印刷条件名がなくなったか判定
bool CPrintCondition::IsEndOfList()
{
	bool res = (m_it == m_printConditionNames.end());
	return res;
}


//次の印刷条件名の各レコードを取得する
bool CPrintCondition::GetNext()
{
	if (IsEndOfList() == false)		//残りの印刷条件名があれば処理する
	{//->ある
		//インポート・エクスポート用の排他制御を開始する
		auto lockObj = API::UniqueLock();

		ST_PDB_MIGRATION_DATA migrationData;
		//m_itで紙DBから業務移行用データ取得
		if (false == API::GetPrintConditionMigration(
			*m_it,
			DEF_LENGTH_MM,
			DEF_MODE_RESO_SPEED_M,
			migrationData,
			m_paperType)) {
				//取得失敗時は失敗とする
				WriteDebugLog("failed GetPrintConditionMigration");
				return false;
		}

		//取得したデータをメンバ変数に保持する
		m_printConditionName = m_it->data();		// GetNext()時の印刷条件名
		m_migrationData = migrationData;			// 内部にvectorがあるので

		//m_it進める
		++m_it;

		return true;
	}
	else
	{//->ない
		//ここに来るまでコールしてはいけない
		WriteDebugLog("failed IsEndOfList");
		return false;	
	}
}
