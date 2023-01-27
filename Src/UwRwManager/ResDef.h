/**
 *	@file	ResDef.h
 *	@brief	Head Cleaning Setting GUI resource ID definition file
 *	@author	GCS	Cong Le
 *	@date	2021/12/03 Cong Le new
**/

#ifndef __RES_DEF_H_INCLUDED__
#define __RES_DEF_H_INCLUDED__

#include "CommonDef_OP.h"

// ==========================================================================
// Global definitions
#define ID_MESSAGE_UNWINDERMANAGER		ERR_UNWINDER_MANAGER

#define RESOURCE_STR_FILE	_T("strings_UnwinderManager.ini")	//<! Resource INI file name strings_UnwinderManager.ini
#define RESOURCE_STR	_T("STRING")	//<! Section name of the resource to read
//<! [STRING] Section Resource ID enumeration
//<! The ID sequence must exactly match the relevant section of the INI file.
/* string identifiers */
enum
{
	/* string id count */
	IDS_MAX
};

#define RESOURCE_MSG_STR_FILE	_T("strings_UnwinderManager.ini")	//<! Resource INI file name strings_UnwinderManager.ini
#define RESOURCE_MSG_STR			_T("MSG")	//!< Section name of the resource to read

enum
{
	IDM_NOTIFY_RECEIVER_STATUS = 1,
	IDM_UPDATE_PAPER_THICKNESS_TO_CURRENT_PRINT_CONDITION,
	IDM_UPDATE_PAPER_THICKNESS_TO_JOB,
	IDM_NOTIFY_UW_STATUS,
	IDM_MAX
};

#endif // __RES_DEF_H_INCLUDED__
