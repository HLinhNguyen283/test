/**
 * @file	UpdatePrintConditionName.cpp
 * @brief	印刷条件名を追加/上書きする
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "UpdatePrintConditionName.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "PaperDBDef_OP.h"
#include "Stub_PaperDBIF.h"
#include <sstream>
#include <iomanip>
#include "Ini_PrintConditionGUI.h"
#include "DataSystemSetting.h"
#include "Stub_JetInspectorIF.h"
#include "Stub_RemoteControlIF.h"
// ==========================================================================


CUpdatePrintConditionName::CUpdatePrintConditionName(CDataSystemSetting& inDataSystemSetting, ST_PDB_MIGRATION_DATA* inMigrationData, std::pair<bool,bool>& inHasElements) 
	: m_DataSystemSetting(inDataSystemSetting)
{
	m_DED = inDataSystemSetting.IsDED();
	m_IsRegisterJI = inDataSystemSetting.IsRegisterJetInspection();
	m_BasePrintConditionName = inDataSystemSetting.GetBasePrintConditionName();
	m_IsPunchedPaper = false;
	m_DataForPDB = inMigrationData;
	m_hasInspectionElement = inHasElements.first;
	m_hasDecodeElement = inHasElements.second;
}

CUpdatePrintConditionName::~CUpdatePrintConditionName()
{
}

void CUpdatePrintConditionName::Add(const std::string& inPrintConditionName)
{
	// Note:
	//   印刷条件設定画面の印刷条件名追加処理を参考に作成
	//     PrintConditionGUI\CDataPrintSettings::SaveAddPrintSettingDlg

	try {
		// 印刷条件名を追加する
		addPrintConditionName(inPrintConditionName);
	}
	catch (int) {
		WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::Add : Error. Failed to add a print condition." << inPrintConditionName);
		throw;
	}
}

void CUpdatePrintConditionName::CopyNecessaryFiles(std::vector<std::string>& outFileList)
{
	try {
		if (isNeededJiTakeOver() == false) {
			// JI系の情報の引き継ぎが不要なときはコピーしない
			return;
		}
		const std::string printConditionName = m_DataSystemSetting.GetBasePrintConditionName();
		// 一時マスクファイルを複製する
		m_DataForPDB->TemporalMaskFile = copyTemporalMaskFile(printConditionName, outFileList);

		// ジョブ印刷時のJI撮影画像ファイルを複製する
		m_CopiedFileNames.JIPhotoOnJobPrintSelf = copyJIPhotoOnJobPrint(printConditionName, COM_SYSTEM_TYPE_SELF, outFileList);
		m_CopiedFileNames.JIPhotoOnJobPrintPart = copyJIPhotoOnJobPrint(printConditionName, COM_SYSTEM_TYPE_PART, outFileList);
	}
	catch (int) {
		throw;
	}
}

std::string CUpdatePrintConditionName::copyTemporalMaskFile(const std::string& inPrintConditionName, std::vector<std::string>& outFileList)
{
	// パスのみ抜き出す
	auto extractPath = [](const std::string& inSrcFileName)->std::string
	{
		size_t fileStartPos = inSrcFileName.find_last_of("\\");
		if (fileStartPos == std::string::npos) {
			WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::copyTemporalMaskFile : Error. Cannot get file path.\n" << inSrcFileName);
			throw 1;
		}
		return inSrcFileName.substr(0, fileStartPos + 1);
	};

	// 新しいファイル名を生成する
	auto getDstTemporalMaskFileName = [&]()->std::string
	{
		SYSTEMTIME systemTime = {0};
		GetLocalTime(&systemTime);

		std::stringstream newFileName;
		newFileName << "PDB_"; 
		newFileName << std::setfill('0') << std::setw(4) << systemTime.wYear;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wMonth;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wDay;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wHour;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wMinute;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wSecond;
		newFileName << m_DataSystemSetting.GetFileCounter();
		newFileName << ".xml";

		return newFileName.str();
	};


	// 一時マスクファイル名を取得する
	std::string srcFileName = "";
	if (CStub_PaperDBIF::PDB_GetTemporalMaskFile(inPrintConditionName.c_str(), srcFileName) == false) {
		return "";
	}

	if (srcFileName.empty() == true) {
		// 一時マスクファイル名が設定されていなければ複製せずに終了
		return "";
	}

	// パスのみ抜き出す
	std::string path = extractPath(srcFileName);

	// 新しいファイル名を生成する
	std::string dstFileName = path + getDstTemporalMaskFileName();

	// 新しいファイル名で元ファイルの複製を生成する
	std::string moduleDirectory = GetModuleDirectory();
	if (CopyFile((moduleDirectory + srcFileName).c_str(), (moduleDirectory + dstFileName).c_str(), TRUE) == 0) {
		const std::string lastErr = getLastErrorString(GetLastError());
		WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::copyTemporalMaskFile : Error. Cannot copy a temporal mask file.\nsrc:" << srcFileName << "\ndst:" << dstFileName << "\n" << lastErr);
		throw 1;
	}

	outFileList.push_back(moduleDirectory + dstFileName);

// 新しいファイル名は Preference\TemporalMask\<SES/DED>\PDB_YYYYMMDDHHMMSS.xml
	// 新しいファイル名を返す
	return dstFileName;
}

std::string CUpdatePrintConditionName::copyJIPhotoOnJobPrint(const std::string& inPrintConditionName, eComSystemType inSystemType, std::vector<std::string>& outFileList)
{
	// 新しいファイル名を生成する
	auto getDstJIPhotoOnJobPrintFileName = [&]()->std::string
	{
		SYSTEMTIME systemTime = {0};
		GetLocalTime(&systemTime);

		std::stringstream newFileName;
		newFileName << std::setfill('0') << std::setw(4) << systemTime.wYear;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wMonth;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wDay;
		newFileName << "_";
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wHour;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wMinute;
		newFileName << std::setfill('0') << std::setw(2) << systemTime.wSecond;
		newFileName << std::setfill('0') << std::setw(3) << systemTime.wMilliseconds;
		newFileName << m_DataSystemSetting.GetFileCounter();
		newFileName << ".jpg";

		return newFileName.str();
	};


	// ジョブ印刷時のJI撮影画像ファイルを取得する
	std::string srcFileName = "";
	double resoX;
	double resoY;
	std::string jobName;
	SYSTEMTIME date;
	if (CStub_PaperDBIF::PDB_GetJIPhotoOnJobPrint(inPrintConditionName.c_str(), inSystemType, srcFileName, resoX, resoY, jobName, date) == false) {
		return "";
	}

	if (srcFileName.empty() == true) {
		// ジョブ印刷時のJI撮影画像ファイルが設定されていなければ複製せずに終了
		return "";
	}

	// 新しいファイル名を生成する
	std::string dstFileName = getDstJIPhotoOnJobPrintFileName();

	// フルパスにする
	std::string path = getJIPhotoOnJobPrintPath(inSystemType);
	std::string srcPath = path + srcFileName;
	std::string dstPath = path + dstFileName;

	if (PathFileExists(srcPath.c_str()) == 0) {
		// ジョブ印刷時のJI撮影画像ファイルが存在しない場合は複製せずに終了
		return "";
	}

	// 新しいファイル名で元ファイルのコピーを生成する
	if (CopyFile(srcPath.c_str(), dstPath.c_str(), TRUE) == 0) {
		const std::string lastErr = getLastErrorString(GetLastError());
		WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::copyJIPhotoOnJobPrint : Error. Cannot copy a JI Photo file." << (inSystemType == COM_SYSTEM_TYPE_SELF? " SELF" : " PART") << "\nsrc:" << srcPath << "\ndst:" << dstPath << "\n" << lastErr);
		throw 1;
	}

	outFileList.push_back(dstPath);

	// 新しいファイル名を返す
	return dstFileName;
}

void CUpdatePrintConditionName::addPrintConditionName(const std::string& inPrintConditionName)
{
	// 印刷条件名を追加する
	if (CStub_PaperDBIF::PDB_AddPrintConditionName(inPrintConditionName.c_str(), m_BasePrintConditionName.c_str()) == false) {
		WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::addPrintConditionName : Error. PDB_AddPrintConditionName. " << inPrintConditionName << " based:" << m_BasePrintConditionName);
		throw 1;
	}

	// 印刷条件追加時に別途設定が必要な項目に対して処理を行う
	additionalProcOnAdd(inPrintConditionName);
}

// JI系の情報の引き継ぎが必要かどうか
bool CUpdatePrintConditionName::isNeededJiTakeOver()
{
	return(m_IsRegisterJI == true && m_hasInspectionElement == false);
}

void CUpdatePrintConditionName::preProc()
{
	// JI系の情報の引き継ぎが必要か判定
	if (isNeededJiTakeOver() == false)
	{
		// 非検査エリア(紙端)を削除する
		additionalProcForNonInspectionAreaTable();

		// 非検査エリア(矩形)を削除する
		additionalProcForNonInspectionAreaList();

		// 一時マスクファイル名を削除する
		additionalProcForTemporalMaskFileName("");
	}

	// Decodeエレメントが存在するときは削除する
	if (m_IsRegisterJI == false || m_hasDecodeElement == true) 
	{
		// レイアウト画面設定のデコード情報を削除する
		additionalProcForLayoutImageParameterDecodeInfo();
	}
}

void CUpdatePrintConditionName::additionalProcOnAdd(const std::string& inPrintConditionName)
{
	// スキャン画像ファイル名を削除する
	additionalProcForScanFileName(inPrintConditionName);

	// 複製したジョブ印刷時のJI撮影画像ファイルを設定する
	// 複製しないときは空文字を設定する
	additionalProcForJIPhotoOnJobPrint(inPrintConditionName, m_CopiedFileNames.JIPhotoOnJobPrintSelf, m_CopiedFileNames.JIPhotoOnJobPrintPart);
}

void CUpdatePrintConditionName::additionalProcForScanFileName(const std::string& inPrintConditionName) const
{
	// スキャン画像ファイル名を削除する
	auto delScanFileName = [](const std::string& inPrintConditionName, eComSystemType inSystemType)->void
	{
		// スキャン画像ファイル名を取得する	
		std::string scanFileName = "";
		if (CStub_PaperDBIF::PDB_GetScanFileName(inPrintConditionName.c_str(), inSystemType, scanFileName) == false) {
			// 取得に失敗しても正常終了
			return;
		}

		if (scanFileName.empty() == true) {
			// 取得できなければ削除せずに正常終了
			return;
		}

		// 紙DBの当該項目を削除する
		if (CStub_PaperDBIF::PDB_DelScanFileName(inPrintConditionName.c_str(), inSystemType) == false) {
			WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::additionalProcForScanFileName : Error. PDB_DelScanFileName. " << inPrintConditionName);
		}
	};


	// 自機のスキャン画像ファイル名を削除する
	delScanFileName(inPrintConditionName, COM_SYSTEM_TYPE_SELF);

	if (m_DED == true) {
		// DEDなら相手機も削除する
		delScanFileName(inPrintConditionName, COM_SYSTEM_TYPE_PART);
	}
}

void CUpdatePrintConditionName::additionalProcForNonInspectionAreaTable() const
{
	//デフォルト値
	double NonInspectionAreaDefault = m_DataSystemSetting.GetNonInspectionAreaDefault(m_IsPunchedPaper);

	// 自機の非検査エリア(紙端)を削除する
	m_DataForPDB->NonInspectionLeft[COM_SYSTEM_TYPE_SELF] = NonInspectionAreaDefault;
	m_DataForPDB->NonInspectionRight[COM_SYSTEM_TYPE_SELF] = NonInspectionAreaDefault;

	if (m_DED == true) {
		// DEDなら相手機も削除する
		m_DataForPDB->NonInspectionLeft[COM_SYSTEM_TYPE_PART] = NonInspectionAreaDefault;
		m_DataForPDB->NonInspectionRight[COM_SYSTEM_TYPE_PART] = NonInspectionAreaDefault;
	}
}

void CUpdatePrintConditionName::additionalProcForNonInspectionAreaList() const
{
	// 非検査エリア(矩形)を削除する
	m_DataForPDB->NonInspectionArea.clear();
	// resizeしないとPDBがat()でアクセスするのに具合が悪い
	m_DataForPDB->NonInspectionArea.resize(m_DED == true ? 2 : 1);
}

void CUpdatePrintConditionName::additionalProcForLayoutImageParameterDecodeInfo() const
{
	//Decode情報
	ST_JOB_DECODE emptyDecode;
	m_DataForPDB->Decode = emptyDecode;
}

void CUpdatePrintConditionName::additionalProcForTemporalMaskFileName(const std::string& inCopyFileName)
{
	// 新しい一時マスクファイル名で上書きする()
	m_DataForPDB->TemporalMaskFile = inCopyFileName;
}

void CUpdatePrintConditionName::additionalProcForJIPhotoOnJobPrint(const std::string& inPrintConditionName, const std::string& inCopyFileNameSelf, const std::string& inCopyFileNamePart) const
{
	// ジョブ印刷時のJI撮影画像ファイルを設定する
	//   ※設定するファイルは作成済みであること
	//   ※ファイル名が空ならレコードを初期設定にする
	auto setJIPhotoOnJobPrint = [&](const std::string& inPrintConditionName, eComSystemType inSystemType, const std::string& inCopyFileName)->void
	{
		// ジョブ印刷時のJI撮影画像ファイルを取得する
		double resoX = 0.0;
		double resoY = 0.0;
		std::string jobName = "";
		SYSTEMTIME date = {0};
		std::string fileName = "";
		if (CStub_PaperDBIF::PDB_GetJIPhotoOnJobPrint(inPrintConditionName.c_str(), inSystemType, fileName, resoX, resoY, jobName, date) == false) {
			// 取得できなければ上書きしない
			return;
		}

		if (inCopyFileName.empty() == true) {
			// ファイル名が空ならレコードを初期設定にする
			resoX = 0.0;
			resoY = 0.0;
			jobName = "";
			SYSTEMTIME emptyDate = {0};
			date = emptyDate;
		}

		// 紙DBの当該項目を新しいファイル名で上書きする
		if (CStub_PaperDBIF::PDB_SetJIPhotoOnJobPrint(inPrintConditionName.c_str(), inSystemType, inCopyFileName, resoX, resoY, jobName, date) == false) {
			WRITE_TO_LOG_BUF(LOG_DEBUG, "CUpdatePrintConditionName::additionalProcForTemporalMaskFile : Error. PDB_SetJIPhotoOnJobPrint. " << inPrintConditionName << " " << inCopyFileName);
		}
	};


	// 自機のジョブ印刷時のJI撮影画像ファイルを設定する
	setJIPhotoOnJobPrint(inPrintConditionName, COM_SYSTEM_TYPE_SELF, inCopyFileNameSelf);

	if (m_DED == true) {
		// DEDなら相手機も削除する
		setJIPhotoOnJobPrint(inPrintConditionName, COM_SYSTEM_TYPE_PART, inCopyFileNamePart);
	}
}

const std::string CUpdatePrintConditionName::getLastErrorString(DWORD inErrID)
{
	LPVOID errBuf = GetLastErrorString(inErrID);
	if(errBuf == nullptr){
		return "";
	}

	const std::string errStr = static_cast<LPTSTR>(errBuf);
	FreeLastErrorString(errBuf);
	return errStr;
}

const std::string CUpdatePrintConditionName::getJIPhotoOnJobPrintPath(eComSystemType inSystemType)
{
	std::stringstream path;
	path << GetModuleDirectory();

	if (m_DED == true) {
		if (inSystemType == COM_SYSTEM_TYPE_PART) {
			path << _T("Preferences\\PrintMonitor\\DED\\Slave\\");
			return path.str();
		}
		else {
			path << _T("Preferences\\PrintMonitor\\DED\\Master\\");
			return path.str();
		}
	}
	else {
		path << _T("Preferences\\PrintMonitor\\SES\\");
		return path.str();
	}
}

void CUpdatePrintConditionName::SetPunchedPaperFlag(bool isPunched)
{
	m_IsPunchedPaper = isPunched;
}

