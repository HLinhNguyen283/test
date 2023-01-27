// ==========================================================================
// Include files
#include "StdAfx.h"
#include "Stub_PrintProfileIF.h"
// ==========================================================================
//bool _PrintProfile_GetCurrentHistoryInfo_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, char outRegDate[256], char outAdjName[65], bool* outIsInitial)
//{
//	_snprintf_s(outRegDate, 256, _TRUNCATE, "%s", "2012/06/23 18:00");
//	_snprintf_s(outAdjName, 65, _TRUNCATE, "%s %s %d %u", inMedia, inPrintModeResID, inTargetIsMaster, inColorID);
//	return true;
//}
//
//bool _PrintProfile_GetCurrentHistoryWarning_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, size_t inColorID, char* outUseInitWarningMsg, size_t& ioUseInitWarningMsgSize, char* outMismatchConditionMsg, size_t& ioMismatchConditionMsgSize)
//{
//	if (inMedia && inPrintModeResID && strcmp(inMedia, "Paper Type 3") == 0 && strcmp(inPrintModeResID, "PRINT_MODE_NAME_2") == 0)
//	{
//		if (inTargetIsMaster)
//		{
//			if (inColorID == 0)
//			{
//				if (outUseInitWarningMsg)
//				{
//					_snprintf_s(outUseInitWarningMsg, ioUseInitWarningMsgSize, _TRUNCATE, "%s %s %s %d %u", "outUseInitWarningMsg", inMedia, inPrintModeResID, inTargetIsMaster, (UINT)inColorID);
//				}
//				else
//				{
//					char tmp[256] = { 0 };
//					_snprintf_s(tmp, _TRUNCATE, "%s %s %s %d %u", "outUseInitWarningMsg", inMedia, inPrintModeResID, inTargetIsMaster, (UINT)inColorID);
//					ioUseInitWarningMsgSize = strnlen_s(tmp, 255);
//				}
//			}
//			else if (inColorID == 1)
//			{
//				if (outMismatchConditionMsg)
//				{
//					_snprintf_s(outMismatchConditionMsg, ioMismatchConditionMsgSize, _TRUNCATE, "%s %s %s %d %u", "outMismatchConditionMsg", inMedia, inPrintModeResID, inTargetIsMaster, (UINT)inColorID);
//				}
//				else
//				{
//					char tmp[256] = { 0 };
//					_snprintf_s(tmp, _TRUNCATE, "%s %s %s %d %u", "outMismatchConditionMsg", inMedia, inPrintModeResID, inTargetIsMaster, (UINT)inColorID);
//					ioMismatchConditionMsgSize = strnlen_s(tmp, 255);
//				}
//			}
//		}
//	}
//	return true;
//}
//
//bool _PrintProfile_CreateSnapshot_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, const char* inAdjName, const bool* inIsWarnDisable)
//{
//	return true;
//}
//
//size_t m_begin = 0;
//char m_media[256] = { 0 };
//char m_printModeResID[256] = { 0 };
//size_t m_color_front[8] = { 0 };
//size_t m_color_front_size = 0;
//size_t m_color_back[8] = { 0 };
//size_t m_color_back_size = 0;
//HistoryInfoRef _PrintProfile_BeginFindHistoryInfo_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, const size_t* inColorIDs[2], size_t inColorIDNum[2], size_t* outHistoryNum)
//{
//	if (inMedia && inPrintModeResID && strcmp(inMedia, "Paper Type 3") == 0 && strcmp(inPrintModeResID, "PRINT_MODE_NAME_3") == 0)
//	{
//		*outHistoryNum = 1;
//	}
//	else if (inMedia && inPrintModeResID && strcmp(inMedia, "Paper Type 3") == 0 && strcmp(inPrintModeResID, "PRINT_MODE_NAME_1") == 0)
//	{
//		*outHistoryNum = 4;
//	}
//	else
//	{
//		*outHistoryNum = 3;
//	}
//	_snprintf_s(m_media, _TRUNCATE, "%s", inMedia);
//	_snprintf_s(m_printModeResID, _TRUNCATE, "%s", inPrintModeResID);
//	m_color_front_size = inColorIDNum[0];
//	memset(m_color_front, 0, sizeof(m_color_front));
//	memcpy_s(m_color_front, sizeof(size_t)*8, inColorIDs[0], sizeof(size_t)*m_color_front_size);
//	memset(m_color_back, 0, sizeof(m_color_back));
//	m_color_back_size = inColorIDNum[1];
//	memcpy_s(m_color_back, sizeof(size_t)*8, inColorIDs[1], sizeof(size_t)*m_color_back_size);
//	m_begin = 0;
//	return NULL;
//}
//
//bool _PrintProfile_GetNextHistoryInfo_OP(HistoryInfoRef inType, char outRegistDate[256], char outAdjustName[65], bool& outIsWarnDisable, size_t outColorIDs[2][8], size_t outColorIDNum[2], bool& outIsUseInitial, bool& outIsMismatchConditon, bool& outIsMismatchConnection, bool& outIsCurrentHistory)
//{
//	_snprintf_s(outRegistDate, 256, _TRUNCATE, "%s %s", m_media, m_printModeResID);
//	_snprintf_s(outAdjustName, 65, _TRUNCATE, "%s %u", "Setting", (UINT)m_begin);
//	outColorIDNum[0] = m_color_front_size;
//	for (size_t i = 0; i < m_color_front_size; ++i)
//	{
//		outColorIDs[0][i] = m_color_front[i];
//	}
//	outColorIDNum[1] = m_color_back_size;
//	for (size_t i = 0; i < m_color_back_size; ++i)
//	{
//		outColorIDs[1][i] = m_color_back[i];
//	}
//	switch (m_begin)
//	{
//	case 0:
//		outIsUseInitial = true;
//		outIsMismatchConditon = false;
//		outIsMismatchConnection = false;
//		outIsCurrentHistory = false;
//		break;
//	case 1:
//		outIsUseInitial = false;
//		outIsMismatchConditon = true;
//		outIsMismatchConnection = false;
//		outIsCurrentHistory = false;
//		break;
//	case 2:
//		outIsUseInitial = false;
//		outIsMismatchConditon = false;
//		outIsMismatchConnection = false;
//		outIsCurrentHistory = true;
//		break;
//	case 3:
//		outIsUseInitial = true;
//		outIsMismatchConditon = false;
//		outIsMismatchConnection = false;
//		outIsCurrentHistory = true;
//		break;
//	}
//	++m_begin;
//	return true;
//}
//
//void _PrintProfile_EndFindHistoryInfo_OP(HistoryInfoRef inType)
//{
//}
//
//void _PrintProfile_ResetCurrent_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID)
//{
//}
//
//void _PrintProfile_RemoveHistory_OP(HistoryInfoRef inHisType, size_t inIndex)
//{
//}
//
//void _PrintProfile_UpdateBasicInfo_OP(HistoryInfoRef inHisType, size_t inIndex, const char* inAdjName)
//{
//}
//
//void _PrintProfile_ApplyToCurrent_OP(HistoryInfoRef inHisType, size_t inIndex, const char* inTargetMedia, const char* inTargetPrintModeResID, bool inTargetIsMaster, const size_t inTargetColor[], size_t inTargetColorNum)
//{
//}
//
//bool _PrintProfile_GetPrintParameter_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, size_t inColorID, struct PrintProfile_PrintParam* outParam)
//{
//	if (outParam)
//	{
//		// ADJUST_DATE
//		if (inTargetIsMaster)
//		{
//			switch (inColorID)
//			{
//			case 0:
//			case 3:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "MASTER 2013/01/16 18:30");
//				break;
//			case 1:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "MASTER 2013/02/16 18:30");
//				break;
//			case 5:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "MASTER 2013/03/16 18:30");
//				break;
//			case 2:
//			case 4:
//			case 6:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "MASTER 2013/04/16 18:30");
//				break;
//			}
//		}
//		else
//		{
//			switch (inColorID)
//			{
//			case 0:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "SLAVE 2013/01/16 18:30");
//				break;
//			case 1:
//			case 2:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "SLAVE 2013/02/16 18:30");
//				break;
//			case 3:
//			case 4:
//			case 5:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "SLAVE 2013/03/16 18:30");
//				break;
//			case 6:
//				_snprintf_s(outParam->ADJUST_DATE, _TRUNCATE, "%s", "SLAVE 2013/04/16 18:30");
//				break;
//			}
//		}
//		// PRINT_CONDITION
//		_snprintf_s(outParam->PRINT_CONDITION, _TRUNCATE, "%s | Target: %d | ID: %d", "OKHJ", inTargetIsMaster, inColorID);
//		// PRINT_MODE
//		if (strcmp("PRINT_MODE_NAME_1", inPrintModeResID) == 0)
//			outParam->PRINT_MODE = 0;
//		else if (strcmp("PRINT_MODE_NAME_2", inPrintModeResID) == 0)
//			outParam->PRINT_MODE = 1;
//		else if (strcmp("PRINT_MODE_NAME_3", inPrintModeResID) == 0)
//			outParam->PRINT_MODE = 2;
//		// PRINT_SPEED
//		outParam->PRINT_SPEED = 5000;
//		// TENSION
//		outParam->TENSION = 6;
//		// ALIGNMENT_SETTING
//		_snprintf_s(outParam->ALIGNMENT_SETTING, _TRUNCATE, "%s", inMedia);
//		// PAPER_TYPE
//		_snprintf_s(outParam->PAPER_TYPE, _TRUNCATE, "%s", inMedia);
//		// PRECOAT_USE_FRONT
//		outParam->PRECOAT_USE_FRONT = 1;
//		// PRECOAT_USE_BACK
//		outParam->PRECOAT_USE_BACK = 1;
//		// PRECOAT_AMOUNT_FRONT
//		outParam->PRECOAT_AMOUNT_FRONT = 3;
//		// PRECOAT_AMOUNT_BACK
//		outParam->PRECOAT_AMOUNT_BACK = 3;
//		// OVERCOAT_USE_FRONT
//		outParam->OVERCOAT_USE_FRONT = 1;
//		// OVERCOAT_USE_BACK
//		outParam->OVERCOAT_USE_BACK = 1;
//		// OVERCOAT_AMOUNT_FRONT
//		outParam->OVERCOAT_AMOUNT_FRONT = 2;
//		// OVERCOAT_AMOUNT_BACK
//		outParam->OVERCOAT_AMOUNT_BACK = 2;
//		// TEMPERATURE_PRECOAT_FRONT
//		outParam->TEMPERATURE_PRECOAT_FRONT = 50;
//		// TEMPERATURE_PRECOAT_BACK
//		outParam->TEMPERATURE_PRECOAT_BACK = 50;
//		// TEMPERATURE_ROLLER_FRONT
//		outParam->TEMPERATURE_ROLLER_FRONT = 100;
//		// TEMPERATURE_ROLLER_BACK
//		outParam->TEMPERATURE_ROLLER_BACK = 100;
//		// TEMPERATURE_FAN_FRONT
//		outParam->TEMPERATURE_FAN_FRONT = 0;
//		// TEMPERATURE_FAN_BACK
//		outParam->TEMPERATURE_FAN_BACK = 0;
//		// MEDIA_WIDTH
//		outParam->MEDIA_WIDTH = 520000;
//		// PAPER_LENGTH_UNIT
//		outParam->PAPER_LENGTH_UNIT = 0;
//		// PAPER_LENGTH
//		outParam->PAPER_LENGTH = 0;
//		// PAPER_THICKNESS
//		outParam->PAPER_THICKNESS = 100;
//		// PUNCH_HOLE
//		outParam->PUNCH_HOLE = 0;
//		// SEWING
//		outParam->SEWING = 0;
//		// REPEAT_SHEETS
//		outParam->REPEAT_SHEETS = 1;
//		// MARK
//		outParam->MARK = 0;
//		// MARK_SIDE
//		outParam->MARK_SIDE = 1;
//		// MARK_OFFSET
//		outParam->MARK_OFFSET = 0;
//		return true;
//	}
//	return false;
//}
//
//const char* _PrintProfile_GetAdjustFilePath_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, bool* outIsInitialFile)
//{
//	return NULL;
//}
//
//bool _PrintProfile_CreateNewAdjustFilePath_OP(EPrintProfile_Kind	inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, char outDataPath[_MAX_PATH])
//{
//	return TRUE;
//}
//
//size_t _PrintProfile_AddNewHistory_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, const char* inAdjName, const char* inAdjFileName[2][8], const bool* inIsWarnDisable[2][8])
//{
//	return 0;
//}
//
//void _PrintProfile_UpdateCurrent_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, size_t inIndex)
//{
//}
//
//bool _PrintProfile_IsNeedSnapshot_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID)
//{
//	return true;
//}

bool _PrintProfile_Duplicate_OP(EPrintProfile_Kind inType, const char* inBaseMedia, const char* inTargetMedia)
{
	return true;
}

bool _PrintProfile_AddPrintMode_OP(EPrintProfile_Kind inType, const char* inMedia)
{
	return true;
}

bool _PrintProfile_Remove_OP(EPrintProfile_Kind inType, const char* inBaseMedia)
{
	return true;
}