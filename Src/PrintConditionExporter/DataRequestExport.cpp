/**
 * @file	DataRequestExport.cpp
 * @brief	印刷条件のエクスポート要求に対する処理を行う
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestExport.h"
#include "ProcPluginCBRProc.h"
#include "ThreadSafe.h"
#include "DebugLogUtil.h"

#include "ExportFile.h"
#include "PrintCondition.h"
#include "FinishFile.h"
#include "Utility.h"
#include "ResDef.h"

#include <list>
// ==========================================================================


//コンストラクタ
CDataRequestExport::CDataRequestExport() : 
	m_Requests(MOD_PRINT_CONDITION_EXPORTER, "CRequestQueue", 1000)
{
}

//デストラクタ
CDataRequestExport::~CDataRequestExport()
{
}

//JMF::ExportPaperData受信イベント
ReceiveExportPaperData::EReturnCode CDataRequestExport::OnReceiveExportPaperData(const std::string& inPaperDataURL)
{
	//書き出し先パスが有効かチェックする
	if(CExportFile::IsValidExportFolder(inPaperDataURL) == true)
	{
		//エクスポート依頼をリストに登録する
		auto requests = m_Requests.UniqueLock();
		requests->push_back(inPaperDataURL);
	
		return ReceiveExportPaperData::kReturnCode_Success;
	}
	else
	{
		//無効時はエラー終了(kReturnCode_InvalidPaperDataURL)する
		return ReceiveExportPaperData::kReturnCode_InvalidPaperDataURL;
	}
}

//エクスポート開始イベント
void CDataRequestExport::OnStartExport()
{
	///////////////////////////
	//サブ関数準備
	//先頭のリクエスト文字列取得
	auto GetRequest = [&](CThreadSafe<std::list<std::string>>& inRequests, std::string& outRequest)->bool
	{
		auto requests = m_Requests.UniqueLock();
		if(requests->empty() == false)
		{
			outRequest = requests->front();
			return true;
		}
		else
		{
			return false;
		}
	};

	//先頭のリクエストを削除
	auto PopRequest = [&](CThreadSafe<std::list<std::string>>& inRequests)->void
	{
		auto requests = m_Requests.UniqueLock();
		requests->pop_front();
	};

	///////////////////////////
	//メイン処理
	//エクスポート依頼があるだけ処理する
	std::string request;
	while(GetRequest(m_Requests, request) == true)
	{//->依頼有り
		//エクスポート処理開始
		CPrintCondition pdb;
		CExportFile exportFile;
		try
		{//->正常系
			//紙DBにある印刷条件数分繰り返す
			while(pdb.IsEndOfList() == false)
			{
				//紙DBから印刷条件のエクスポート対象の情報を取得する
				CUtility::ThrowIfFalse(pdb.GetNext(), pdb.m_printConditionName + " do not have neccessary record.");

				//ローカルの一時保存先にエクスポートファイルを作成する
				CUtility::ThrowIfFalse(exportFile.WriteFile(pdb), pdb.m_printConditionName + " failed to write an export file.");
			}

			//全印刷条件のエクスポートファイル出力に成功したらPaperDataURLにエクスポートファイルを移動する
			CUtility::ThrowIfFalse(exportFile.MoveAllFiles(request), "Export files could not move to " + request);

			//エクスポート完了のトリガーファイルを作成する(成功)
			CFinishFile finishFile;
			finishFile.CreateFile(request, true);
		}
		catch(std::exception& e)
		{//->途中の処理に失敗したらこっち
			//デバッグ用ログ出力
			WriteDebugLog(e.what());

			//全エクスポートファイルを削除する
			exportFile.DeleteAllFiles();
		
			//エクスポート完了のトリガーファイルを作成する(失敗)
			CFinishFile finishFile;
			finishFile.CreateFile(request, false);
		}

		//終了した依頼を削除する
		PopRequest(m_Requests);
	}
}

//アプリケーション終了イベント
void CDataRequestExport::OnExitSystem()
{
	////////////////////////
	//サブ関数準備
	//エクスポート依頼が空かチェック
	auto IsEmpty = [&](CThreadSafe<std::list<std::string>>& inRequests)->bool
	{
		auto requests = m_Requests.UniqueLock();
		return requests->empty();
	};

	//エクスポート依頼数を取得
	auto getRquestCnt = [&](CThreadSafe<std::list<std::string>>& inRequests)->long
	{
		auto requests = m_Requests.UniqueLock();
		return requests->size();
	};

	////////////////////////
	//処理
	//エクスポート依頼がなくなってから終わらせる
	if(IsEmpty(m_Requests) == false)
	{//->エクスポート依頼がある場合
		//進捗ダイアログ表示(TM)
		SIZE sizeProgDlg = {750, 0};
		long progMax = getRquestCnt(m_Requests);
		long progressID = ShowProgressDialog(
			sizeProgDlg,
			PROGDLG_MSG_CENTER | PROGDLG_PROGRESS_BAR,
			0,
			progMax,
			0,
			nullptr,
			(char*)LoadResourceString(IDS_EXIT_PROGRESS, RESOURCE_STR));

		//エクスポート依頼がなくなるまで待つ
		while(IsEmpty(m_Requests) == false)
		{
			//進捗バー更新(TM)
			SetProgressMessage(progressID, PROGDLG_SET_VALUE, progMax - getRquestCnt(m_Requests), nullptr);

			Sleep(1000);
		}
		//進捗バーを閉じる前に最大まで伸ばす
		SetProgressMessage(progressID, PROGDLG_SET_VALUE, progMax, nullptr);

		//進捗ダイアログ閉じる(TM)
		CloseProgressDialog(progressID);
	}
}