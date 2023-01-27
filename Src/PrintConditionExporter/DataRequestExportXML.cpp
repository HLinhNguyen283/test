/*! \file DataRequestExportXML.cpp
 *  \brief DataRequestExportXML.cppの処理
 *  \author pt.ju-tanaka
 *  \date 2019/06/26 pt.ju-tanaka 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DataRequestExportXML.h"
#include "ProcPluginCBRProc.h"
#include "ThreadSafe.h"
#include "DataSystemSetting.h"
#include "ThreadSafeShareLock.h"
#include "ResDef.h"
#include "PrintConditionExportXML.h"
#include "ExportXML.h"
#include "Utility.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <imagehlp.h>

// ==========================================================================
// Local Definition
const std::string st_TempFolderPath = "Temp\\PrintConditionExportXml";
const std::string st_EQFolderXMLPath = "#work_ex\\ExportJDPaper_xml";
std::string st_ExportFolderPath = "";

// ==========================================================================

// コンストラクタ
DataRequestExportXML::DataRequestExportXML() : 
	m_RequestsXML(MOD_PRINT_CONDITION_EXPORTER, "CRequestQueueXML", 1000)
{
}

// デストラクタ
DataRequestExportXML::~DataRequestExportXML()
{
}

// 初期化イベント
bool DataRequestExportXML::onInitialize()
{
	if( !initEQFolder() )
	{
		// EQ制御サーバ上への共有フォルダ作成失敗、またはアクセス不可時
		return false;
	}

	std::list<SRequestConditionData> pdbDataList;
	// 紙DBから印刷条件情報を取得する
	ExportUtility::getPDBGetPrintConditionInfo(pdbDataList);

	// 印刷条件名と印刷条件データをスタック(m_RequestsXML)に登録する
	for(auto itrPDBList = pdbDataList.begin(); itrPDBList != pdbDataList.end(); ++itrPDBList)
	{
		SRequestConditionData requestConditionData;
		requestConditionData.conditionName = itrPDBList->conditionName;
		requestConditionData.migrationData = itrPDBList->migrationData;
		requestConditionData.isUpdate = true;
		requestXMLPrintCondionExport(requestConditionData);
	}

	std::string outputFolder = GetModuleDirectory() + st_TempFolderPath;
	// 中間フォルダの作成
	if(FALSE == CreateDirectory(outputFolder.c_str(), NULL))
	{
		return false;
	}

	return true;
}

// エクスポート更新イベント
void DataRequestExportXML::onUpdateXMLExport(ST_THREAD_INFO* inUpdateThread)
{
	std::string fileName;

	// 現状の依頼をテンポラリの一覧にコピーし、m_RequestsXMLにある依頼を空にする.リトライが必要なものは処理後に先頭に追加する
	//  UniqueLockしている期間中は、紙DBからの更新依頼を追加できない。紙DBからの更新依頼が極力止まらぬようエクスポートの実処理中はロックしない
	std::list<SRequestConditionData> tmpRequestsList;
	{
		auto requests = m_RequestsXML.UniqueLock();
		std::copy(requests->begin(), requests->end(), std::back_inserter(tmpRequestsList));
		requests->clear();
	}

	// 重複する印刷条件を削除
	deleteDuplicateCondition(tmpRequestsList);

	auto itrRequest = tmpRequestsList.begin();
	while( itrRequest!= tmpRequestsList.end() )
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
			std::string tmpFolderPath = ExportUtility::getLocalFolderPath(st_TempFolderPath);
			if( ExportUtility::IsFileExist(tmpFolderPath, itrRequest->localFileName, st_ExtensionXML) )
			{
				// 中間フォルダに該当ファイルがある場合、EQ制御サーバ上の共有フォルダにXMLファイルを移動する
				if( ExportUtility::MoveFiles(st_ExportFolderPath, itrRequest->localFileName, st_ExtensionXML, st_TempFolderPath) )
				{
					// EQ制御サーバ上の共有フォルダにXMLファイルを移動できた場合、
					// m_RequestsXMLから本依頼を削除できる
					itrRequest = tmpRequestsList.erase(itrRequest);
					continue;
				}
			}
			else
			{
				export_xml::SPrintConditionData printConditionData;
				printConditionData.printConditionName = fileName;
				printConditionData.migrationData = itrRequest->migrationData;
				CDataSystemSetting::GetInstance()->onExportXML(printConditionData.printModeDataList);

				export_xml::PrintCondition printCondition(printConditionData);
				std::string outputXMLPath = GetModuleDirectory() + st_TempFolderPath + "\\" + itrRequest->localFileName + st_ExtensionXML;
				export_xml::ExportFile exportFile;

				// 中間フォルダに該当ファイルが無い場合、XMLファイルを中間フォルダに作成する
				if(exportFile.onRequestExportXML(outputXMLPath, printConditionData))
				{
					// EQ制御サーバ上の共有フォルダにXMLファイルを移動する
					if( ExportUtility::MoveFiles(st_ExportFolderPath, itrRequest->localFileName, st_ExtensionXML, st_TempFolderPath) )
					{
						// XMLファイルが中間ファイルに作成でき、且つEQ制御サーバ上の共有フォルダにXMLファイルを移動できた場合、
						// m_RequestsXMLから本依頼を削除できる
						itrRequest = tmpRequestsList.erase(itrRequest);
						continue;
					}
				}
			}

			// XMLファイルが中間ファイルに作成できなかった or EQ制御サーバ上の共有フォルダにファイルを移動できなかった場合、
			// リトライにて再度処理をおこなうため、m_RequestsXMLから本依頼を削除してはならない
			++itrRequest;
		}
		else
		{
			// 削除指定の場合、EQ制御サーバ上の共有フォルダにあるXMLファイルを削除する
			if( ExportUtility::DeleteFileEQ(st_ExportFolderPath, fileName, st_ExtensionXML) )
			{
					// EQ制御サーバ上の共有フォルダのファイル削除されている場合、m_RequestsXMLから本依頼を削除する
					itrRequest = tmpRequestsList.erase(itrRequest);
					continue;
			}

			// EQ制御サーバ上の共有フォルダのファイルが削除されていない場合、
			// リトライにて再度処理をおこなうため、m_RequestsXMLから本依頼を削除しない
			++itrRequest;
		}
	}

	//テンポラリの一覧に残ったものを、m_RequestsXMLの先頭に追加して、リトライ可能なようにする
	{
		auto requests = m_RequestsXML.UniqueLock();
		requests->insert(requests->begin(), tmpRequestsList.begin(), tmpRequestsList.end());
	}
}

// システムの印刷条件更新を通知する
void DataRequestExportXML::onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	SRequestConditionData requestConditionData;
	requestConditionData.conditionName = inPrintConditionName;
	requestConditionData.migrationData = inMigrationData;
	requestConditionData.isUpdate = true;

	requestXMLPrintCondionExport(requestConditionData);
}

// システムの印刷条件削除を通知する
void DataRequestExportXML::onDeletePrintCondition(const std::string& inPrintConditionName)
{
	ST_PDB_MIGRATION_DATA dummyMigrationData;
	SRequestConditionData requestConditionData;
	requestConditionData.conditionName = inPrintConditionName;
	// 印刷条件削除の場合ST_PDB_MIGRATION_DATAの情報は空で登録する
	requestConditionData.migrationData = dummyMigrationData;
	requestConditionData.isUpdate = false;

	requestXMLPrintCondionExport(requestConditionData);
}

// 印刷条件のエクスポート依頼
void DataRequestExportXML::requestXMLPrintCondionExport(SRequestConditionData& inRequestConditionData)
{
	//書き出し先パスが有効かチェックする
	EReturnCode returnCode = ExportUtility::isValidExportFolder(st_ExportFolderPath);
	if(returnCode == kReturnCode_Success)
	{
		auto requests = m_RequestsXML.UniqueLock();

		inRequestConditionData.localFileName = ExportUtility::createLocalFileName(inRequestConditionData.conditionName);

		//エクスポート依頼をリストに登録する
		requests->push_back(inRequestConditionData);
	}
}

// EQ制御サーバ上の共有フォルダの初期化
bool DataRequestExportXML::initEQFolder()
{
	std::string pcNameEQ;
	CDataSystemSetting::GetInstance()->GetEqCenterPcName(pcNameEQ);
	std::string outputRootFolderXML;
	CDataSystemSetting::GetInstance()->GetOutputRootFolderXML(outputRootFolderXML);
	if(outputRootFolderXML.empty())
	{
		outputRootFolderXML = st_EQFolderXMLPath;
	}

	std::string exportFolderPath = "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\";

	std::string formMsg;
	long resourceID = 0;

	try
	{
		// EQサーバー上の共有フォルダにアクセスできるかをチェック("\\<EQのPCのIPアドレス>\#work_ex\ExportJDPaper_xml\")
		EReturnCode returnCode = ExportUtility::isValidExportFolder(exportFolderPath.c_str());
		if(returnCode == kReturnCode_NotAccess)
		{
			// 共有フォルダにアクセスできない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK_XML;
			throw 1;
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// EQサーバー上の共有フォルダパスから最後の"\"を削除する
			int pos = exportFolderPath.find_last_of("\\");
			std::string tmpExportFolder = exportFolderPath.substr(0, pos);
			char* tmpRootFolder = const_cast<char*>(tmpExportFolder.c_str());
			// EQサーバー上のrootフォルダパスの取得("\\<EQのPCのIPアドレス>\#work_ex")
			PathRemoveFileSpec(tmpRootFolder);

			// EQサーバー上のRootフォルダにアクセスできるかをチェック
			std::string rootFolder = tmpRootFolder;
			returnCode = ExportUtility::isValidExportFolder(rootFolder);
			if(returnCode != kReturnCode_Success)
			{
				// EQサーバー上のRootフォルダが存在しない or アクセスできない場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
				resourceID = IDM_ERR_NETWORK_XML;
				throw 1;
			}

			// 共有フォルダが存在しない場合、共有フォルダを作成
			if( MakeSureDirectoryPathExists(exportFolderPath.c_str()) != TRUE )
			{
				// フォルダ作成に失敗した場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
				ExportUtility::replaceString("%s", exportFolderPath, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
				throw 1;
			}
		}
		else
		{
			// EQサーバー上の共有フォルダにアクセスできる場合は処理なし	
		}

		bool isFixIP = CDataSystemSetting::GetInstance()->IsEnableAutoUpdateFixIP();
		bool isDED = CDataSystemSetting::GetInstance()->IsDED();

		std::string ipAddressSelf;
		if( isFixIP )
		{
			if( isDED ) ipAddressSelf = "192.168.0.30";
		} else {
			CDataSystemSetting::GetInstance()->GetClientPcIPAddrSelf(ipAddressSelf);
		}
		if(ipAddressSelf.empty())
		{
			// 自機のIPアドレスが記載されていない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
			ExportUtility::replaceString("%s", exportFolderPath, formMsg);
			resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
			throw 1;
		}

		if(isDED)
		{
			// DED構成時のファイルパス
			exportFolderPath += ipAddressSelf + "-DED"+ "\\";
		}
		else
		{
			// SES構成時のファイルパス
			exportFolderPath += ipAddressSelf + "-SES"+ "\\";
		}

		// 指定フォルダの存在をチェック
		returnCode = ExportUtility::isValidExportFolder(exportFolderPath.c_str());
		if(returnCode == kReturnCode_Success)
		{
			// フォルダが存在する場合、フォルダ内のファイルを削除
			ExportUtility::DeleteAllFilesEQ(exportFolderPath);
		}
		else if(returnCode == kReturnCode_NotExists)
		{
			// フォルダが存在しない場合、フォルダを作成
			if( MakeSureDirectoryPathExists(exportFolderPath.c_str()) != TRUE )
			{
				// フォルダ作成に失敗した場合、エラーメッセージのパラメータをセットする
				formMsg = LoadResourceString(IDM_ERR_CRATE_OUTPUT_FOLDER_XML, RESOURCE_MSG_STR);
				ExportUtility::replaceString("%s", exportFolderPath, formMsg);
				resourceID = IDM_ERR_CRATE_OUTPUT_FOLDER_XML;
				throw 1;
			}
		}
		else
		{
			// フォルダにアクセスできない場合、エラーメッセージのパラメータをセットする
			formMsg = LoadResourceString(IDM_ERR_NETWORK_XML, RESOURCE_MSG_STR);
			resourceID = IDM_ERR_NETWORK_XML;
			throw 1;
		}

		st_ExportFolderPath = exportFolderPath;

		bool isDualSimplex = CDataSystemSetting::GetInstance()->IsDualSimplex();
		// DualSimplex環境設定有効でDED構成の場合
		if( (isDualSimplex) && (isDED) )
		{
			std::string filePathSelf;
			filePathSelf += "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\" + ipAddressSelf + "-SES"+ "\\";
			// SESフォルダ(自機)内の全ファイルを削除
			ExportUtility::DeleteAllFilesEQ(filePathSelf);

			std::string ipAddressPart;
			CDataSystemSetting::GetInstance()->GetClientPcIPAddrPart(ipAddressPart);
			std::string filePathPart;
			filePathPart += "\\\\" + pcNameEQ + "\\" + outputRootFolderXML + "\\" + ipAddressPart + "-SES"+ "\\";
			// SESフォルダ内の全ファイルを削除
			ExportUtility::DeleteAllFilesEQ(filePathPart);
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
void DataRequestExportXML::deleteDuplicateCondition(std::list<SRequestConditionData>& ioRequests)
{
	auto itrRequest = ioRequests.begin();
	std::list<SRequestConditionData> collectiedList;

	while( itrRequest != ioRequests.end() )
	{
		auto itrCollected = collectiedList.begin();
		while( itrCollected != collectiedList.end() )
		{
			// 重複削除済み依頼リストに重複する印刷条件名があるかチェック
			if( itrRequest->conditionName.compare(itrCollected->conditionName) == 0 )
			{
				std::string tmpFolderPath = ExportUtility::getLocalFolderPath(st_TempFolderPath);
				// 中間フォルダに重複する印刷条件の一時ファイルがあるかチェック
				if( ExportUtility::IsFileExist(tmpFolderPath, itrCollected->localFileName, st_ExtensionXML) )
				{
					// 重複する印刷条件の一時ファイルを削除
					std::string tmpFilePath = tmpFolderPath + itrCollected->localFileName + ".txt";
					ExportUtility::DeleteFiles(tmpFilePath);
				}

				// 依頼リストから重複する印刷条件を削除する
				itrCollected = collectiedList.erase(itrCollected);
				continue;
			}

			// 印刷条件名が重複していなければ次の印刷条件へ
			++itrCollected;
		}

		// 重複削除済み依頼リストに印刷条件を登録する
		collectiedList.push_back(*itrRequest);
		++itrRequest;
	}

	ioRequests.clear();
	// 重複削除済み依頼リストの内容を依頼リストに設定
	for(auto itr = collectiedList.begin(); itr != collectiedList.end(); ++itr) 
	{
		ioRequests.push_back(*itr);
	}
}


//JI、検査関連フォルダのミラーリングコピー
bool DataRequestExportXML::MirroringInspectionData(const std::string& inJIDataFolder)
{
	if( !(CDataSystemSetting::GetInstance()->IsEnableAutoUpdatePrintConditionXML()) )	return	true;
	
	std::string pcNameEQ;
	CDataSystemSetting::GetInstance()->GetEqCenterPcName(pcNameEQ);
	std::string outputRootFolderJIData;
	CDataSystemSetting::GetInstance()->GetOutputRootFolderInspectionData(outputRootFolderJIData);
	if(outputRootFolderJIData.empty())	return true;

	std::string srcPath = GetModuleDirectory();
	srcPath += inJIDataFolder;
	std::string dstPath = "\\\\" + pcNameEQ + "\\" + outputRootFolderJIData + "\\" + inJIDataFolder;
	if( srcPath.back() == '\\' )	srcPath.pop_back();
	if( dstPath.back() == '\\' )	dstPath.pop_back();

	return	mirrorFolder(static_cast<const std::string&>(srcPath), static_cast<const std::string&>(dstPath));
}

//GetLastErrorの文字列取得。
std::string DataRequestExportXML::getLastError()
{
	DWORD errID = GetLastError();
	LPVOID errBuf = GetLastErrorString(errID);
	std::string errMsg = (LPTSTR)errBuf;
	FreeLastErrorString(errBuf);
	return errMsg;
}

//コマンドプロンプトで実行する。
DWORD DataRequestExportXML::runSystemCmd(const std::string& inCmd)
{
	STARTUPINFO  si = {0};
	PROCESS_INFORMATION pi = {0};
	HANDLE hWndmain = nullptr;

	if (TRUE == CreateProcess(nullptr, const_cast<char*>(inCmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		//robocopyの終了コードをチェック。
		DWORD exitCode = 0;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return exitCode;
	} else {
		//エラー発生。
		std::stringstream ss;
		ss << "failed CreateProcess[" << getLastError()  << "]";
		WriteDebugLog(ss.str());
		return 0;
	}
}

//フォルダのミラーリング。
bool DataRequestExportXML::mirrorFolder(const std::string& inSrcPath, const std::string& inDestPath)
{
	//CopyFile(SrcPath.str().c_str(), DestPath.str().c_str(), TRUE);

	if( CDataSystemSetting::GetInstance()->IsDebugLog() )
	{
		WriteDebugLog(inSrcPath);
		WriteDebugLog(inDestPath);
	}
	
	//---generate logfile name
	SYSTEMTIME tm = { 0 };
	GetLocalTime(&tm);
	char logname[64] = { 0 };
	_snprintf_s(logname, _TRUNCATE, "MirrorFailed%04d%02d%02d%02d%02d%02d%03d.txt",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	std::stringstream logStr;
	logStr << GetModuleDirectory() <<"\\Log\\" << std::string(logname);
	
	std::stringstream comStr;
	comStr << "cmd /c robocopy \"" << inSrcPath << "\" \"" << inDestPath << "\" /MIR /R:1 /W:1 /LOG:" << logStr.str();
	/* 参考～robocopyの終了コード（8以上がエラー）。
		if errorlevel 16  echo  ***FATAL ERROR***  & goto end
		if errorlevel 15  echo FAIL MISM XTRA COPY & goto end
		if errorlevel 14  echo FAIL MISM XTRA      & goto end
		if errorlevel 13  echo FAIL MISM      COPY & goto end
		if errorlevel 12  echo FAIL MISM           & goto end
		if errorlevel 11  echo FAIL      XTRA COPY & goto end
		if errorlevel 10  echo FAIL      XTRA      & goto end
		if errorlevel  9  echo FAIL           COPY & goto end
		if errorlevel  8  echo FAIL                & goto end
		if errorlevel  7  echo      MISM XTRA COPY & goto end
		if errorlevel  6  echo      MISM XTRA      & goto end
		if errorlevel  5  echo      MISM      COPY & goto end
		if errorlevel  4  echo      MISM           & goto end
		if errorlevel  3  echo           XTRA COPY & goto end
		if errorlevel  2  echo           XTRA      & goto end
		if errorlevel  1  echo                COPY & goto end
		if errorlevel  0  echo    --no change--    & goto end
	*/
	int errNum = runSystemCmd(comStr.str());
	if (7 < errNum) {
		//エラー発生。
		std::stringstream ss;
		ss << "failed robocopy[" << errNum << "]";
		WriteDebugLog(ss.str());
		return false;
	} else {
		std::remove((logStr.str()).c_str());
	}

	return true;
}
