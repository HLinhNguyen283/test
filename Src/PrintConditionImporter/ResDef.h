/**
 * @file	ResDef.h
 * @brief	PrintConditionImporter用リソースID定義ファイル
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
**/

#pragma once

#include "CommonDef_OP.h"

// ==========================================================================
// Global definitions
#define ID_MESSAGE_ERROR		ERR_PRINT_CONDITION_IMPORTER			//!< エラーコード = ID_MESSAGE_ERROR + リソースID

#define RESOURCE_STR_FILE	_T("strings_PrintConditionImporter.ini")	//! リソースINIファイル名 strings_PrintConditionImporter.ini
#define RESOURCE_STR	_T("STRING")	//! 読む込むリソースのセクション名
//! [STRING]セクションリソースID列挙
//! IDの並びはINIファイルの該当セクションと完全に一致している必要がある。
/* string identifiers */
enum
{
	IDS_STAT_IMPORT_NOT_DEFINED_PAPERTYPE = 1,
	IDS_STAT_IMPORT_NOT_DEFINED_PRINTMODE,
	IDS_STAT_IMPORT_NOT_DEFINED_PRINTSPEED,
	IDS_STAT_IMPORT_NOT_SUPPORTED_FLUSHPATTERN,
	IDS_STAT_IMPORT_NOT_EXECUTE,
	IDS_STAT_IMPORT_NOT_DEFINED_ALLIGNMENT,
	IDS_STAT_IMPORT_NOT_DEFINED_FLUSHPAGE,
	IDS_STAT_IMPORT_EXIST_INSPECTSETTING,
	IDS_STAT_IMPORT_NOT_APPEND_PAPERTYPE,
	IDS_STAT_IMPORT_NOT_APPEND_PAPERTYPE_PROFILE,
	IDS_STAT_IMPORT_NOT_APPEND_ALLIGNMENT,
	IDS_STAT_IMPORT_NOT_MODIFIED_ALLIGNMENT,
	IDS_STAT_IMPORT_NOT_APPEND_ALLIGNMENT_PROFILE,
	IDS_STAT_IMPORT_NOT_MODIFIED_ALLIGNMENT_PROFILE,
	IDS_STAT_IMPORT_ADD_PAPERTYPE,
	IDS_STAT_IMPORT_ADD_ALLIGNMENT,
	IDS_PROFILE_SHADING_AND_LINEARIZE,
	IDS_PROFILE_SHADING,
	IDS_PROFILE_LINEARIZE,

	/* リソースID数 */
	IDS_MAX
};

//! リソースINIファイル名
#define RESOURCE_MSG_STR_FILE	_T("strings_PrintConditionImporter.ini")
#define RESOURCE_MSG_STR	_T("MSG")			//!< 読む込むリソースのセクション名
//! [MSG]セクションリソースID列挙
//! IDの並びはINIファイルの該当セクションと完全に一致している必要がある。
enum
{
	IDM_EXIT_PROGRESS = 1,
	IDM_ERR_IMPORT_TIMEOUT,
	/* リソースID数 */
	IDM_MAX
};

