/**
 * @file	Stub_PaperDBIF.cpp
 * @brief	stub of paper DB plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include <vector>
#include "Common.h"
#include "CommonCBRProc_OP.h"
#include "PaperDB_Callbacks.h"
#include "Stub_PaperDBIF.h"

// ==========================================================================
// Local definitions
static bool GetCallbacks(SPaperDB_Callbacks* outCbr);

// ==========================================================================

/**
 * @brief	I/F取得
 * @param[out]	outCbr : I/F構造体
 * @retval	true : 成功
 * @retval	false : 失敗
**/
static bool GetCallbacks(SPaperDB_Callbacks* outCbr)
{
	if(!PaperDB_GetCallbacks(outCbr)){
		return false;
	}
	return true;	
}

CStub_PaperDBIF::CStub_PaperDBIF()
{
}

CStub_PaperDBIF::~CStub_PaperDBIF()
{
}

bool CStub_PaperDBIF::PDB_GetNonInspectionAreaList(const char* inPrintConditionName, eComSystemType inSystem, vector<STRUCT_NON_INSPECTION_AREA>& outNonInspectionArea)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetNonInspectionAreaList(inPrintConditionName, inSystem, outNonInspectionArea);
	return res;
}

bool CStub_PaperDBIF::PDB_GetNonInspectionAreaTable(const char* inPrintConditionName, eComSystemType inSystem, double& outNonInspectionLeft, double& outNonInspectionRight)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetNonInspectionAreaTable(inPrintConditionName, inSystem, outNonInspectionLeft, outNonInspectionRight);
	return res;
}

bool CStub_PaperDBIF::PDB_SetJIPhotoOnJobPrint(std::string inPrintConditionName, eComSystemType inSystem, std::string inFileName, double inResoX, double inResoY, std::string inJobName, SYSTEMTIME inDate)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetJIPhotoOnJobPrint(inPrintConditionName, inSystem, inFileName, inResoX, inResoY, inJobName, inDate);
	return res;
}

bool CStub_PaperDBIF::PDB_GetJIPhotoOnJobPrint(std::string inPrintConditionName, eComSystemType inSystem, std::string& outFileName, double& outResoX, double& outResoY, std::string& outJobName, SYSTEMTIME& outDate)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetJIPhotoOnJobPrint(inPrintConditionName, inSystem, outFileName, outResoX, outResoY, outJobName, outDate);
	return res;
}

bool CStub_PaperDBIF::PDB_CommitPaperDB()
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_CommitPaperDB();
	return res;
}

bool CStub_PaperDBIF::PDB_AddPrintConditionName(const char* inPrintConditionName, const char* inSrcPaperName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_AddPrintConditionName(inPrintConditionName, inSrcPaperName);
	return res;
}

bool CStub_PaperDBIF::PDB_DelPrintConditionName(const char* inPrintConditionName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_DelPrintConditionName(inPrintConditionName);
	return res;
}

bool CStub_PaperDBIF::PDB_GetScanFileName(const char* inPrintConditionName, eComSystemType inSystem, std::string& outScanFileName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetScanFileName(inPrintConditionName, inSystem, outScanFileName);
	return res;
}

bool CStub_PaperDBIF::PDB_DelScanFileName(const char* inPrintConditionName, eComSystemType inSystem)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_DelScanFileName(inPrintConditionName, inSystem);
	return res;
}

bool CStub_PaperDBIF::PDB_SetNonInspectionAreaTable(const char* inPrintConditionName, eComSystemType inSystem, double inNonInspectionLeft, double inNonInspectionRight)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetNonInspectionAreaTable(inPrintConditionName, inSystem, inNonInspectionLeft, inNonInspectionRight);
	return res;
}

bool CStub_PaperDBIF::PDB_SetNonInspectionAreaList(const char* inPrintConditionName, eComSystemType inSystem, const vector<STRUCT_NON_INSPECTION_AREA>& inNonInspectionArea)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetNonInspectionAreaList(inPrintConditionName, inSystem, inNonInspectionArea);
	return res;
}

bool CStub_PaperDBIF::PDB_GetLayoutImageParameterWithPrintConditionNameEx(
	const char* inPrintConditionName, 
	DEF_LENGTH_UNIT inUnit, 
	ST_JOB_IMAGE_POSITION& outLayoutImagePosition, 
	ST_JOB_FLUSHING_PATTERN& outLayoutFlushingPattern, 
	ST_JOB_FLUSHING_PATTERN_ADD& outLayoutFlushingPatternAdd, 
	ST_JOB_FLUSHING_PATTERN_ADD2& outLayoutFlushingPatternAdd2,
	ST_JOB_VERIFY_MARK& outLayoutVerifyMark, 
	ST_JOB_POSTPROCESS& outLayoutPostProcess, 
	ST_JOB_POSTPROCESS_ADD& outLayoutPostProcessAdd, 
	ST_JOB_DECODE& outLayoutDecode, 
	BOOL& outEnableAlignBottomPPFLSPTN)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetLayoutImageParameterWithPrintConditionNameEx(
		inPrintConditionName,
		inUnit,
		outLayoutImagePosition,
		outLayoutFlushingPattern,
		outLayoutFlushingPatternAdd,
		outLayoutFlushingPatternAdd2,
		outLayoutVerifyMark,
		outLayoutPostProcess,
		outLayoutPostProcessAdd,
		outLayoutDecode,
		outEnableAlignBottomPPFLSPTN);

	return res;
}

bool CStub_PaperDBIF::PDB_SetLayoutImageParameterWithPrintConditionNameEx(
	const char* inPrintConditionName,
	DEF_LENGTH_UNIT inUnit,
	const ST_JOB_IMAGE_POSITION& inLayoutImagePosition,
	const ST_JOB_FLUSHING_PATTERN& inLayoutFlushingPattern,
	const ST_JOB_FLUSHING_PATTERN_ADD& inLayoutFlushingPatternAdd,
	const ST_JOB_FLUSHING_PATTERN_ADD2& inLayoutFlushingPatternAdd2,
	const ST_JOB_VERIFY_MARK& inLayoutVerifyMark,
	const ST_JOB_POSTPROCESS& inLayoutPostProcess,
	const ST_JOB_POSTPROCESS_ADD& inLayoutPostProcessAdd,
	const ST_JOB_DECODE& inLayoutDecode,
	const BOOL& inLayoutVertivalAlignmentPPFLSPTN)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetLayoutImageParameterWithPrintConditionNameEx(
		inPrintConditionName,
		inUnit,
		inLayoutImagePosition,
		inLayoutFlushingPattern,
		inLayoutFlushingPatternAdd,
		inLayoutFlushingPatternAdd2,
		inLayoutVerifyMark,
		inLayoutPostProcess,
		inLayoutPostProcessAdd,
		inLayoutDecode,
		inLayoutVertivalAlignmentPPFLSPTN);

	return res;
}

bool CStub_PaperDBIF::PDB_GetTemporalMaskFile(const char* inPrintConditionName, std::string& outPath)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetTemporalMaskFile(inPrintConditionName, outPath);
	return res;
}

bool CStub_PaperDBIF::PDB_SetTemporalMaskFile(const char* inPrintConditionName, const std::string& inPath)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetTemporalMaskFile(inPrintConditionName, inPath);
	return res;
}


bool CStub_PaperDBIF::PDB_EnterCommonPaperDBCS()
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_EnterCommonPaperDBCS();
	return res;
}

bool CStub_PaperDBIF::PDB_LeaveCommonPaperDBCS()
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_LeaveCommonPaperDBCS();
	return res;
}

bool CStub_PaperDBIF::PDB_IsSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_IsSystemDryTempPrint(inDryTempPrint);
	return res;
}

bool CStub_PaperDBIF::PDB_AddSystemDryTempPrint(const ST_PDB_DRY_TEMP_PRINT& inDryTempPrint)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_AddSystemDryTempPrint(inDryTempPrint);
	return res;
}

bool CStub_PaperDBIF::PDB_IsSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_IsSystemDotgainCurve(inDotgainCurve);
	return res;
}

bool CStub_PaperDBIF::PDB_AddSystemDotgainCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_AddSystemDotgainCurve(inDotgainCurve);
	return res;
}

bool CStub_PaperDBIF::PDB_GetSystemDotgainCurve(int index, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetSystemDotgainCurve(index, outDotgainCurve);
	return res;
}

bool CStub_PaperDBIF::PDB_SetPrintConditionMigration(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_SetPrintConditionMigration(inPrintConditionName, inMigrationData);
	return res;
}

bool CStub_PaperDBIF::PDB_GetPrintConditionMigration(const std::string& inPrintConditionName, DEF_LENGTH_UNIT inLengthUnit, 
	eMODE_RESO_SPEED_UNIT inSpeedUnit, ST_PDB_MIGRATION_DATA& outMigrationData)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_GetPrintConditionMigration(inPrintConditionName, inLengthUnit, inSpeedUnit, outMigrationData);
	return res;
}
	
bool CStub_PaperDBIF::PDB_TryUniqueLock_forImportExport(NS_SRWLock::CUniqueTransaction<bool>& outLockObj)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	bool res = cbr.PDB_TryUniqueLock_forImportExport(outLockObj);
	return res;
}

bool CStub_PaperDBIF::PDB_IsPrintConditionName(const char* inPrintConditionName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr)){
		return false;
	}
	return cbr.PDB_IsPrintConditionName(inPrintConditionName);
}

bool CStub_PaperDBIF::PDB_OnPrintConditionImport(bool inIsStauts)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_OnPrintConditionImport(inIsStauts);
}

bool CStub_PaperDBIF::PDB_IsSystemPaperType(const char* inPaperType)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_IsSystemPaperType(inPaperType);
}

bool CStub_PaperDBIF::PDB_GetPaperType(const char* inPrintConditionName, std::string& outPaperType)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_GetPaperType(inPrintConditionName, outPaperType);
}

bool CStub_PaperDBIF::PDB_IsSystemLevelTuneName(const char* inLevelTuneName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_IsSystemLevelTuneName(inLevelTuneName);
}

bool CStub_PaperDBIF::PDB_AddSystemPaperType(const char* inPaperType)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_AddSystemPaperType(inPaperType);
}

bool CStub_PaperDBIF::PDB_AddSystemLevelTuneName(const char* inLevelTuneName)
{
	SPaperDB_Callbacks cbr;
	if(!GetCallbacks(&cbr))
	{
		return false;
	}
	return cbr.PDB_AddSystemLevelTuneName(inLevelTuneName);
}
