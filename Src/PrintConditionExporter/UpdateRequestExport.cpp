/*! \file UpdateRequestExport.cpp
 *  \brief UpdateRequestExport.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/02/19 pt.ju-tanaka 新規作成
*/


// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestExport.h"
#include "ProcPluginCBRProc.h"
#include "ThreadSafe.h"
#include "UpdateRequestExport.h"
#include "Ini_PrinterDescriptor.h"
#include "DataSystemSetting.h"
#include "ThreadSafeShareLock.h"
#include "Stub_PaperDBIF.h"
#include "ExportFileUpdate.h"

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include <imagehlp.h>
#include "ResDef.h"

#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>
// ==========================================================================

const std::string st_tempEQFolderPath = "#work_ex\\ExportJDPaper_text";


// コンストラクタ
UpdateRequestExport::UpdateRequestExport() : 
	m_Requests(MOD_PRINT_CONDITION_EXPORTER, "CUpdateRequestQueue", 1000)
{
	m_IsDualSimplex= false;
	m_IsDED = false;
}

// デストラクタ
UpdateRequestExport::~UpdateRequestExport()
{
}

// 初期化イベント
bool UpdateRequestExport::onInitialize(ST_THREAD_INFO* inUpdateThread)
{
	CIni_PrinterDescriptor iniPrinterDescriptor;
	iniPrinterDescriptor.Initialize(TRUE);
	m_PCNameEQ = iniPrinterDescriptor.GetEqCenterPcName();								// EQ管理サーバーのPC名
	m_IsDualSimplex = iniPrinterDescriptor.IsEnableExchangePrinterCompositionOption();	// DualSimplex環境設定が有効かどうか
	m_IsDED = iniPrinterDescriptor.IsDedSystem();										// DED構成かどうか
	m_IpAddress = iniPrinterDescriptor.GetClientPcIPAddr(true);							// 自機のIPアドレス
	m_IpAddressPart = iniPrinterDescriptor.GetClientPcIPAddr(false);					// 相手機のIPアドレス

	CDataSystemSetting::GetInstance()->GetOutputRootFolder(m_OutputRootFolder);			// EQ制御サーバー上にある出力先のルートフォルダ
	if(m_OutputRootFolder.empty())
	{
		m_OutputRootFolder = st_tempEQFolderPath;
	}

	if( !initEQFolder() )
	{
		// EQ制御サーバ上への共有フォルダ作成失敗、またはアクセス不可時
		return false;
	}

	SRequestData pdbData;
	std::list<SRequestData> pdbDataList;
	{
		NS_SRWLock::CUniqueTransaction<bool> lockObj;
		// インポート・エクスポート用の排他処理（読み書き禁止）を開始する
		CStub_PaperDBIF::PDB_UniqueLock_forImportExport(lockObj);
		CStub_PaperDBIF::PDB_EnterCommonPaperDBCS();		

		std::string name;
		// 全印刷条件名を取得する
		while(CStub_PaperDBIF::PDB_GetPrintConditionName(name) == true)
		{
			ST_PDB_MIGRATION_DATA migrationData;
			// 紙DBから該当する印刷条件データを取得
			if(!CStub_PaperDBIF::PDB_GetPrintConditionMigration(name, UserModeGUI_GetLengthUnit(), DEF_MODE_RESO_SPEED_M, migrationData))
			{
				// 紙DBからの印刷条件データ取得失敗した場合、次の印刷条件の処理を行う
				std::stringstream strLog;
				strLog << "UpdateRequestExport::onInitialize GetPrintConditionMigration failed [ConditionName: " << name << "]";
				WriteToLogBuf(LOG_DEBUG, strLog.str());
				continue;
			}

			//SRequestData構造体メンバのisUpdateとlocalFileNameはinitialize時には使用しないため、設定不要とする
			pdbData.conditionName = name;
			pdbData.migrationData = migrationData;
			pdbDataList.push_back(pdbData);
		}	

		CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS();
		// インポート・エクスポート用の排他処理の終了はこのスコープが終わるタイミングで終了となる
	}

	// 印刷条件名と印刷条件データをスタック(m_Requests)に登録する
	for(auto itrPDBList = pdbDataList.begin(); itrPDBList != pdbDataList.end(); ++itrPDBList)
	{
		requestPrintCondionExport(itrPDBList->conditionName, itrPDBList->migrationData, true);
	}

	return true;
}

// エクスポート更新イベント
void UpdateRequestExport::onUpdateExport(ST_THREAD_INFO* inUpdateThread)
{
	ExportFileUpdate exportFileUpdate;
	std::string fileName;

	// 現状の依頼をテンポラリの一覧にコピーし、m_Requestsにある依頼を空にする.リトライが必要なものは処理後に先頭に追加する
	//  UniqueLock している期間中は、紙DBからの更新依頼を追加できない。紙DBからの更新依頼が極力止まらぬようエクスポートの実処理中はロックしない
	std::list<SRequestData> tmpRequests;
	{
		auto requests = m_Requests.UniqueLock();
		std::copy(requests->begin(), requests->end(), std::back_inserter(tmpRequests));
		requests->clear();
	}

	// 重複する印刷条件を削除
	deleteDuplicateCondition(tmpRequests);

	auto itrRequest = tmpRequests.begin();
	while( itrRequest!= tmpRequests.end() )
	{
		// 更新スレッドが終了しているかチェック
		if(inUpdateThread->thread_handle == nullptr)
		{
			// 更新スレッドが終了している場合、処理終了
			return;
		}

		// 依頼リストに依頼が有る場合
		fileName.clear();
		fileName = itrRequest->conditionName;
		if(itrRequest->isUpdate == true)
		{
			// 更新指定の場合
			std::string tmpFolderPath = exportFileUpdate.getLocalFolderPath();
			if( exportFileUpdate.IsFileExist(tmpFolderPath, itrRequest->localFileName) )
			{
				// 中間フォルダに該当ファイルがある場合、EQ制御サーバ上の共有フォルダにテキストファイルを移動する
				if( exportFileUpdate.MoveFiles(m_ExportFolder, itrRequest->localFileName) )
				{
					// EQ制御サーバ上の共有フォルダにテキストファイルを移動できた場合、
					// m_Requestsから本依頼を削除できる
					itrRequest = tmpRequests.erase(itrRequest);
					continue;
				}
			}
			else
			{
				// 中間フォルダに該当ファイルが無い場合、テキストファイルを中間フォルダに作成する
				if( exportFileUpdate.WriteFile(fileName, itrRequest->migrationData, itrRequest->localFileName) )
				{
					// EQ制御サーバ上の共有フォルダにテキストファイルを移動する
					if( exportFileUpdate.MoveFiles(m_ExportFolder, itrRequest->localFileName) )
					{
						// テキストファイルが中間ファイルに作成でき、且つEQ制御サーバ上の共有フォルダにテキストファイルを移動できた場合、
						// m_Requestsから本依頼を削除できる
						itrRequest = tmpRequests.erase(itrRequest);
						continue;
					}
				}
			}

			// テキストファイルが中間ファイルに作成できなかった or EQ制御サーバ上の共有フォルダにファイルを移動できなかった場合、
			// リトライにて再度処理をおこなうため、m_Requestsから本依頼を削除してはならない
			++itrRequest;
		}
		else
		{
			// 削除指定の場合、EQ制御サーバ上の共有フォルダにあるテキストファイルを削除する
			if( exportFileUpdate.DeleteFileEQ(m_ExportFolder, fileName) )
			{
					// EQ制御サーバ上の共有フォルダのファイル削除されている場合、m_Requestsから本依頼を削除する
					itrRequest = tmpRequests.erase(itrRequest);
					continue;
			}

			// EQ制御サーバ上の共有フォルダのファイルが削除されていない場合、
			// リトライにて再度処理をおこなうため、m_Requestsから本依頼を削除しない
			++itrRequest;
		}
	}

	//テンポラリの一覧に残ったものを、m_Requestsの先頭に追加して、リトライ可能なようにする
	{
		auto requests = m_Requests.UniqueLock();
		requests->insert(requests->begin(), tmpRequests.begin(), tmpRequests.end());
	}
}

// システムの印刷条件更新を通知する
void UpdateRequestExport::onUpdatePrintCondition(
	const std::string& inPrintConditionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData, 
	ST_THREAD_INFO* inUpdateThread)
{
	requestPrintCondionExport(inPrintConditionName, inMigrationData, true);
}

// システムの印刷条件削除を通知する
void UpdateRequestExport::onDeletePrintCondition(const std::string& inPrintConditionName, ST_THREAD_INFO* inUpdateThread)
{
	// ST_PDB_MIGRATION_DATAと用紙種類の情報は空で登録する
	ST_PDB_MIGRATION_DATA dummyMigrationData;
	requestPrintCondionExport(inPrintConditionName, dummyMigrationData, false);
}

// 印刷条件をエクスポート依頼に登録する
void UpdateRequestExport::requestPrintCondionExport(
	const std::string& inCondiotionName, 
	const ST_PDB_MIGRATION_DATA& inMigrationData, 
	const bool inIsUpdate)
{
	ExportFileUpdate exportFileUpdate;
	EReturnCode returnCode;
	//書き出し先パスが有効かチェックする
	returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder);
	if(returnCode == kReturnCode_Success)
	{
		auto requests = m_Requests.UniqueLock();

		std::string localFileName = exportFileUpdate.createLocalFileName(inCondiotionName);

		//エクスポート依頼をリストに登録する
		SRequestData requestData;
		requestData.conditionName = inCondiotionName;
		requestData.migrationData = inMigrationData;
		requestData.isUpdate = inIsUpdate;
		requestData.localFileName = localFileName;
		requests->push_back(requestData);
	}
}

// EQ制御サーバ上の共有フォルダを初期化
bool UpdateRequestExport::initEQFolder()
{
	m_ExportFolder += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\";

	std::string formMsg;
	long resourceID = 0;
	ExportFileUpdate exportFileUpdate;

	try
	{
		// EQサーバー上の共有フォルダにアクセスできるかをチェック("\\<EQのPCのIPアドレス>\#work_exExportJDPaper_text\")
		EReturnCode returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder.c_str());
		if(returnCode == kReturnCode_NotAccess)
		{
			// 共有フォルダにアクセスできない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK;
			throw 1;
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// EQサーバー上の共有フォルダパスから最後の"\"を削除する
			int pos = m_ExportFolder.find_last_of("\\");
			std::string tmpExportFolder = m_ExportFolder.substr(0, pos);
			char* tmpRootFolder = const_cast<char*>(tmpExportFolder.c_str());
			// EQサーバー上のrootフォルダパスの取得("\\<EQのPCのIPアドレス>\#work_ex")
			PathRemoveFileSpec(tmpRootFolder);

			// EQサーバー上のRootフォルダにアクセスできるかをチェック
			std::string rootFolder = tmpRootFolder;
			returnCode = exportFileUpdate.IsValidExportFolder(rootFolder);
			if(returnCode != kReturnCode_Success)
			{
				// EQサーバー上のRootフォルダが存在しない or アクセスできない場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
				resourceID = IDM_ERR_NETWORK;
				throw 1;
			}

			// 共有フォルダが存在しない場合、共有フォルダを作成
			if( MakeSureDirectoryPathExists(m_ExportFolder.c_str()) != TRUE )
			{
				// フォルダ作成に失敗した場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			// EQサーバー上の共有フォルダにアクセスできる場合は処理なし	
		}

		if(m_IpAddress.empty())
		{
			// 自機のIPアドレスが記載されていない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
			replaceString("%s", m_ExportFolder, formMsg);
			resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
			throw 1;
		}

		std::string ipAddressSelf;
		if( CDataSystemSetting::GetInstance()->IsEnableAutoUpdateFixIP() )
		{
			if( m_IsDED )
			{
				ipAddressSelf = "192.168.0.30";
			}
			else
			{
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			ipAddressSelf = m_IpAddress;
		}

		if(m_IsDED)
		{
			// DED構成時のファイルパス
			m_ExportFolder += ipAddressSelf + "-DED"+ "\\";
		}
		else
		{
			// SES構成時のファイルパス
			m_ExportFolder += ipAddressSelf + "-SES"+ "\\";
		}

		// 指定フォルダの存在をチェック
		returnCode = exportFileUpdate.IsValidExportFolder(m_ExportFolder.c_str());
		if(returnCode == kReturnCode_Success)
		{
			// フォルダが存在する場合、フォルダ内のファイルを削除
			exportFileUpdate.DeleteAllFilesEQ(m_ExportFolder);
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// フォルダが存在しない場合、フォルダを作成
			if( MakeSureDirectoryPathExists(m_ExportFolder.c_str()) != TRUE )
			{
				// フォルダ作成に失敗した場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER, RESOURCE_MSG_STR);
				replaceString("%s", m_ExportFolder, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER;
				throw 1;
			}
		}
		else
		{
			// フォルダにアクセスできない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_NETWORK, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK;
			throw 1;
		}

		// DualSimplex環境設定有効でDED構成の場合
		if( (m_IsDualSimplex) && (m_IsDED) )
		{
			std::string filePathSelf;
			filePathSelf += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\" + m_IpAddress + "-SES"+ "\\";
			// SESフォルダ(自機)内の全ファイルを削除
			exportFileUpdate.DeleteAllFilesEQ(filePathSelf);

			std::string filePathPart;
			filePathPart += "\\\\" + m_PCNameEQ + "\\" + m_OutputRootFolder + "\\" + m_IpAddressPart + "-SES"+ "\\";
			// SESフォルダ内の全ファイルを削除
			exportFileUpdate.DeleteAllFilesEQ(filePathPart);
		}
	}
	catch(int)
	{
		// エラーメッセージを表示する
		ShowMessageBox(ERR_PRINT_CONDITION_EXPORTER, resourceID, formMsg, MBST_OK | MBST_ICONERROR | MBST_MODELESS, NULL);
		WriteToLogBuf(LOG_STATUS, formMsg);
		return false;
	}

	return true;
}

// 依頼リストから重複する印刷条件を削除する
void UpdateRequestExport::deleteDuplicateCondition(std::list<SRequestData>& ioRequests)
{
	ExportFileUpdate exportFileUpdate;
	auto itrRequest = ioRequests.begin();
	std::list<SRequestData> collectiedList;

	while( itrRequest != ioRequests.end() )
	{
		auto itrCollected = collectiedList.begin();
		while( itrCollected != collectiedList.end() )
		{
			// 重複削除済み依頼リストに重複する印刷条件名があるかチェック
			if( itrRequest->conditionName.compare(itrCollected->conditionName) == 0 )
			{
				std::string tmpFolderPath = exportFileUpdate.getLocalFolderPath();
				// 中間フォルダに重複する印刷条件の一時ファイルがあるかチェック
				if( exportFileUpdate.IsFileExist(tmpFolderPath, itrCollected->localFileName) )
				{
					// 重複する印刷条件の一時ファイルを削除
					std::string tmpFilePath = tmpFolderPath + itrCollected->localFileName + ".txt";
					exportFileUpdate.DeleteFiles(tmpFilePath);
				}

				// 依頼リストから重複する印刷条件を削除する
				itrCollected = collectiedList.erase(itrCollected);
				continue;
			}

			// 印刷条件名が重複していなければ次の印刷条件へ
			++itrCollected;
		}

		// 重複削除済み依頼リストに印刷条件を登録する
		SRequestData collectiedData;
		collectiedData.conditionName = itrRequest->conditionName;
		collectiedData.migrationData = itrRequest->migrationData;
		collectiedData.isUpdate = itrRequest->isUpdate;
		collectiedData.localFileName = itrRequest->localFileName;
		collectiedList.push_back(collectiedData);
		++itrRequest;
	}

	ioRequests.clear();
	// 重複削除済み依頼リストの内容を依頼リストに設定
	for(auto itr = collectiedList.begin(); itr != collectiedList.end(); ++itr) 
	{
		SRequestData requestData;
		requestData.conditionName = itr->conditionName;
		requestData.migrationData = itr->migrationData;
		requestData.isUpdate = itr->isUpdate;
		requestData.localFileName = itr->localFileName;
		ioRequests.push_back(requestData);
	}
}

// 文字列を置換する
void UpdateRequestExport::replaceString(const std::string& inSrc, const std::string& inDst, std::string& ioLog, const size_t inPos) const
{
	size_t pos = ioLog.find(inSrc, inPos);
	if (pos != std::string::npos)
	{
		ioLog.replace(pos, 2, inDst);
	}
}
