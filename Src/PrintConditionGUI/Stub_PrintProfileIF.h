/*! \file Stub_PaperDBIF.h
 *  \brief stub of paper DB plugin
 *  \author GCS Tan Ho
*/
#if !defined(_STUB_PRINT_PROFILE_IF_H_)
#define _STUB_PRINT_PROFILE_IF_H_

#ifdef __cplusplus
extern "C"  {
#endif
// ==========================================================================
// Include files
#include "PrintProfileMgrDef_OP.h"
// ==========================================================================

//bool _PrintProfile_GetCurrentHistoryInfo_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, char outRegDate[256], char outAdjName[65], bool* outIsInitial);
//bool _PrintProfile_GetCurrentHistoryWarning_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, size_t inColorID, char* outUseInitWarningMsg, size_t& ioUseInitWarningMsgSize, char* outMismatchConditionMsg, size_t& ioMismatchConditionMsgSize);
//bool _PrintProfile_CreateSnapshot_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, const char* inAdjName, const bool* inIsWarnDisable);
//HistoryInfoRef _PrintProfile_BeginFindHistoryInfo_OP(EPrintProfile_Kind	inType, const char* inMedia, const char* inPrintModeResID, const size_t* inColorIDs[2], size_t inColorIDNum[2], size_t* outHistoryNum);
//bool _PrintProfile_GetNextHistoryInfo_OP(HistoryInfoRef inType, char outRegistDate[256], char outAdjustName[65], bool& outIsWarnDisable, size_t outColorIDs[2][8], size_t outColorIDNum[2], bool& outIsUseInitial, bool& outIsMismatchConditon, bool& outIsMismatchConnection, bool& outIsCurrentHistory);
//void _PrintProfile_EndFindHistoryInfo_OP(HistoryInfoRef inType);
//void _PrintProfile_ResetCurrent_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID);
//void _PrintProfile_RemoveHistory_OP(HistoryInfoRef inHisType, size_t inIndex);
//void _PrintProfile_UpdateBasicInfo_OP(HistoryInfoRef inHisType, size_t inIndex, const char* inAdjName);
//void _PrintProfile_ApplyToCurrent_OP(HistoryInfoRef inHisType, size_t inIndex, const char* inTargetMedia, const char* inTargetPrintModeResID, bool inTargetIsMaster, const size_t inTargetColor[], size_t inTargetColorNum);
//bool _PrintProfile_GetPrintParameter_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, size_t inColorID, struct PrintProfile_PrintParam* outParam);
//const char* _PrintProfile_GetAdjustFilePath_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, bool* outIsInitialFile);
//bool _PrintProfile_CreateNewAdjustFilePath_OP(EPrintProfile_Kind	inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, char outDataPath[_MAX_PATH]);
//size_t _PrintProfile_AddNewHistory_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, const char* inAdjName, const char* inAdjFileName[2][8], const bool* inIsWarnDisable[2][8]);
//void _PrintProfile_UpdateCurrent_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID, bool inTargetIsMaster, unsigned int inColorID, size_t inIndex);
//bool _PrintProfile_IsNeedSnapshot_OP(EPrintProfile_Kind inType, const char* inMedia, const char* inPrintModeResID);
bool _PrintProfile_Duplicate_OP(EPrintProfile_Kind inType, const char* inBaseMedia, const char* inTargetMedia);
bool _PrintProfile_AddPrintMode_OP(EPrintProfile_Kind inType, const char* inMedia);
bool _PrintProfile_Remove_OP(EPrintProfile_Kind inType, const char* inBaseMedia);

#ifdef __cplusplus
}// extern "C"
#endif

#endif // !defined(_STUB_PRINT_PROFILE_IF_H_)
