/*! \file DataDotgainCurve.cpp
 *  \brief data class: Dot gain curve
 *  \author GCS Dieu Dinh
 *  \date 2013/12/12 GCS Dieu Dinh created
*/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"
#include "DataDotgainCurve.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include <sstream>

// ==========================================================================
// Local definitions
#define DOTGAIN_LUTCURVE_FOLDER		_T("dotgain")				//!< The folder stores dot gain file
#define DOTGAIN_LUTCURVE_EXT		_T(".dtg")					//!< Dot gain curve extension.
// ==========================================================================

/**
 * constructor
 * @note Normal process
 * 
 *		Initialize the member variables
 *	@note 
 *	
 *		
**/
CDataDotgainCurve::CDataDotgainCurve()
{
	ResetMember();
}

/**
 * destructor
 * @note Normal process
 * 
 *		Remove objects which created with dynamic.
 *	@note 
 *	
 *		
**/
CDataDotgainCurve::~CDataDotgainCurve()
{
	ResetMember();
}

/**
 * Initialzie menber variables in the class
 * @retval	true: success
 * @retval	false: failure
 * @note Notes
 * 
 *		The methods to initialize each member variables are written at each getter methods.
 *	@note 
 *	
 *		
**/
bool CDataDotgainCurve::Initialize(CDataSystemSetting* inSystemSetting, CDataIF* inDataIF)
{
	if(!inSystemSetting || !inDataIF || FALSE == m_iniPrintConditionGUI.Initialize(TRUE)) {
		return false;
	}

	m_systemSetting = inSystemSetting;
	m_dataIF = inDataIF;

	return true;
}

/**
 *	Remove menber variables in the class
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CDataDotgainCurve::Finalize()
{
	m_iniPrintConditionGUI.Finalize();
	return true;
}

/**
 *	Get a dot gain curve
 *	@param[out]	outDotgainCurve : a dot gain curve
 *	@return true : success, false: fail
**/
bool CDataDotgainCurve::GetDotgainCurve_ForDrawing(ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	if (m_isInitSuccess)
	{
		ST_PDB_DOTGAIN_CURVE lDotgainCurve;
		//get dotgain curve setting from back side
		lDotgainCurve = m_dotgainCurve[COM_SYSTEM_TYPE_SELF];
		bool isDotgainCurveDefaultOnSelf = (0 == strcmp(lDotgainCurve.DOTGAIN_NAME, ""));
		//At DED mode, use dotgain curve setting of front side when Job only has front side or dotgain curve setting on back side is set default
		if( m_systemSetting->IsDED() && (m_dataIF->IsSimplexJob() || isDotgainCurveDefaultOnSelf)) {
			//Determine system type
			lDotgainCurve = m_dotgainCurve[COM_SYSTEM_TYPE_PART];
		}
		outDotgainCurve = lDotgainCurve;
	}
	return m_isInitSuccess;
}


/*  \brief Get the anchor point of the linear
 *  \outAnchor	Anchor point information
*/
void CDataDotgainCurve::GetLinearDotGainData_ForDrawing(ST_PDB_ANCHOR_POINT* outAnchor)
{
	if(!outAnchor) {
		return;
	}

	outAnchor[0].EDIT_TYPE = ANCHOR_EDITTYPE_DEFAULT;
	outAnchor[0].KIND = ANCHOR_KIND_CURVEDRAW;
	outAnchor[0].POSITION.x = 0;
	outAnchor[0].POSITION.y = 0;

	outAnchor[1].EDIT_TYPE = ANCHOR_EDITTYPE_DEFAULT;
	outAnchor[1].KIND = ANCHOR_KIND_DISPLAY;
	outAnchor[1].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 25) / 100;
	outAnchor[1].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 25) / 100;

	outAnchor[2].EDIT_TYPE = ANCHOR_EDITTYPE_DEFAULT;
	outAnchor[2].KIND = ANCHOR_KIND_DISPLAY;
	outAnchor[2].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 50) / 100;
	outAnchor[2].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 50) / 100;

	outAnchor[3].EDIT_TYPE = ANCHOR_EDITTYPE_DEFAULT;
	outAnchor[3].KIND = ANCHOR_KIND_DISPLAY;
	outAnchor[3].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 75) / 100;
	outAnchor[3].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 75) / 100;

	outAnchor[4].EDIT_TYPE = ANCHOR_EDITTYPE_DEFAULT;
	outAnchor[4].KIND = ANCHOR_KIND_CURVEDRAW;
	outAnchor[4].POSITION.x = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_X * 100) / 100;
	outAnchor[4].POSITION.y = (DEF_ANCHOR_PT_UNIT * MAX_DOTGAIN_CURVE_PT_Y * 100) / 100;
}

/**
 *	Initialize dot gain curve member
 *	@param[in]	inKeyName : key name value
 *	@retval	true: success
 *	@retval	false: failure
 *	@note Notes
 *	
 *		In case the Print Condition GUI is started from:
 *			-	Job List Screen
 *					inKeyName = section ID
 *			-	System Setting Screen
 *					inKeyName = Print Condition Name
**/
bool CDataDotgainCurve::InitDotgainCurve(const char* inKeyName)
{
	_snprintf_s(m_printConditionName, _TRUNCATE, "%s", inKeyName);
	m_isInitSuccess = false;
	eComSystemType eSystemType = COM_SYSTEM_TYPE_SELF;

	if(m_systemSetting->IsStartedFromJobListScreen()) {
		//get dotgain curve setting from COM_SYSTEM_TYPE_SELF
		memset(&m_dotgainCurve[COM_SYSTEM_TYPE_SELF], 0x00, sizeof(ST_PDB_DOTGAIN_CURVE));
		if(!m_systemSetting->GetJobManager_Callbacks()->JM_GetDotgainCurve(inKeyName, eSystemType, m_dotgainCurve[COM_SYSTEM_TYPE_SELF])) {
			return false;
		}
		//At DED mode
		if(m_systemSetting->IsDED()) {
			//Determine system type
			eSystemType = COM_SYSTEM_TYPE_PART;
			memset(&m_dotgainCurve[COM_SYSTEM_TYPE_PART], 0x00, sizeof(ST_PDB_DOTGAIN_CURVE));
			if(!m_systemSetting->GetJobManager_Callbacks()->JM_GetDotgainCurve(inKeyName, eSystemType, m_dotgainCurve[COM_SYSTEM_TYPE_PART])) {
				return false;
			}
		}
	}else {
		//get dotgain curve setting from COM_SYSTEM_TYPE_SELF
		memset(&m_dotgainCurve[COM_SYSTEM_TYPE_SELF], 0x00, sizeof(ST_PDB_DOTGAIN_CURVE));
		if(!m_systemSetting->GetPaperDB_Callbacks()->PDB_GetDotgainCurve(inKeyName, eSystemType, m_dotgainCurve[COM_SYSTEM_TYPE_SELF])) {
			return false;
		}
		//at DED mode
		if(m_systemSetting->IsDED()) {
			//Determine system type
			eSystemType = COM_SYSTEM_TYPE_PART;
			memset(&m_dotgainCurve[COM_SYSTEM_TYPE_PART], 0x00, sizeof(ST_PDB_DOTGAIN_CURVE));
			if(!m_systemSetting->GetPaperDB_Callbacks()->PDB_GetDotgainCurve(inKeyName, eSystemType, m_dotgainCurve[COM_SYSTEM_TYPE_PART])) {
				return false;
			}
		}
	}
	memcpy(m_initDotgainCurve, m_dotgainCurve, sizeof(m_dotgainCurve));
	m_isInitSuccess = true;
	return true;
}

/**
 *	Check if Dot gain curve is changed or not
 *	@return true : changed, false: not changed
**/
bool CDataDotgainCurve::IsDotgainCurveChanged()
{
	if (CompareTwoDotgainCurve(m_dotgainCurve[COM_SYSTEM_TYPE_SELF], m_initDotgainCurve[COM_SYSTEM_TYPE_SELF]) ||
		(m_systemSetting->IsDED() && CompareTwoDotgainCurve(m_dotgainCurve[COM_SYSTEM_TYPE_PART], m_initDotgainCurve[COM_SYSTEM_TYPE_PART])) ) {
			return true;
	}
	return false;
}

/*!
 *  \brief Set the Dotgain curve to the print condition
 *  \return true：Success, false：Failure.
 */
bool CDataDotgainCurve::Save()
{
	// Check dotgain curve files
	for (int sideID = DEF_PRINT_FRONT; sideID < DEF_PRINT_BOTH; ++sideID) {
		DEF_PRINT_SIDE printSide = (DEF_PRINT_SIDE)sideID;

		if (sideID == DEF_PRINT_BACK && !m_systemSetting->IsDED()) {
			// SES case
			break;
		}

		//Check Master's printer
		int isSelf = (DEF_PRINT_FRONT == printSide && m_systemSetting->IsDED())? COM_SYSTEM_TYPE_PART : COM_SYSTEM_TYPE_SELF;
		if (0 != strcmp(m_dotgainCurve[isSelf].DOTGAIN_NAME, "")) {
			//Create file path with DOTGAIN_SHAR_NAME and the folder path contains dot gain files
			TCHAR pFilePath[_MAX_PATH] = {0};
			_snprintf_s(pFilePath, sizeof(pFilePath), _TRUNCATE, "%s%s\\%s\\%s_", GetModuleDirectory(), PREFS_FOLDER, DOTGAIN_LUTCURVE_FOLDER, m_dotgainCurve[isSelf].DOTGAIN_SHAR_NAME);
			size_t filePathSize = strlen(pFilePath);

			UINT colorSize = m_systemSetting->GetDisplayColorSize(printSide);
			for(UINT colorID = 0; colorID < colorSize; ++colorID) {
				const char* bufColorName = m_systemSetting->GetCBufColorName(colorID, printSide);
				if(!isToneCurveFile(pFilePath, filePathSize, bufColorName)) {
					// Show error message box when not found dot gain file.
					char strResource[_MAX_PATH] = {0};
					char msgWarning[_MAX_PATH] = {0};
					_snprintf_s(strResource, _TRUNCATE, LoadResourceString(IDM_ERR_NO_TONECURVE_FILE, RESOURCE_MSG_STR), m_dotgainCurve[isSelf].DOTGAIN_NAME);
					_snprintf_s(msgWarning, sizeof(msgWarning), _TRUNCATE, "%d\n%s", IDM_ERR_NO_TONECURVE_FILE + ID_MESSAGE_ERROR, strResource);
					ShowMessageBox(msgWarning, MBST_OK | MBST_ICONERROR, NULL);
					return false;
				}
			}
		}

		// ジョブのトーン設定を編集するとき、カラーのジョブにモノクロ設定用トーンが選択された場合は、エラーとする。
		if (m_systemSetting->IsStartedFromJobListScreen()){

			DEF_PRINT_SIDE nJobPrintSide = DEF_PRINT_FRONT;
			m_systemSetting->GetJobManager_Callbacks()->JM_GetSidePrint(m_printConditionName, nJobPrintSide, false);

			//!< DED 構成の場合は常に両面（２面）がチェック対象、 SES の場合は常に片面（１面）をチェック対象とする。
			long idx_max = (m_systemSetting->IsDED()? 2:1);

			long idx = 0;
			for (idx = 0; idx < idx_max; idx++) {
				
				bool			bSelf = ((idx == 0)? true:false);
				eComSystemType	nSide = ((!bSelf && m_systemSetting->IsDED())? COM_SYSTEM_TYPE_PART:COM_SYSTEM_TYPE_SELF);

				//!< DED かつ片面ジョブの場合、裏面 (COM_SYSTEM_TYPE_SELF) のチェックは不要。
				if (m_systemSetting->IsDED() && (nJobPrintSide == DEF_PRINT_FRONT) && (nSide == COM_SYSTEM_TYPE_SELF)) {
					continue;
				}

				if (strlen(m_dotgainCurve[nSide].DOTGAIN_SHAR_NAME) == 0) {
					//共通ファイル名がなければデフォルト設定なので判定不要
					continue;
				}

				//!< 印刷面がカラーかモノクロか
				long	nJobColorCountOfPrintSide = 0;
				m_systemSetting->GetJobManager_Callbacks()->JM_GetColorCount(m_printConditionName, bSelf, nJobColorCountOfPrintSide);

				//!< 選択されているトーンがモノクロ用の設定かどうか
				bool			bMonoToneSelected = false;
				std::string		sSelectedToneName = m_dotgainCurve[nSide].DOTGAIN_NAME;
				std::vector<std::string>	vMonoTonePrefix;
				vMonoTonePrefix.push_back("mono@");
				vMonoTonePrefix.push_back(" mono@");
				for (auto itr = vMonoTonePrefix.begin(); itr != vMonoTonePrefix.end(); ++itr) {
					std::string		sMonoTonePrefix = (*itr);
					if (sSelectedToneName.size() >= sMonoTonePrefix.size() &&
						std::equal(std::begin(sMonoTonePrefix), std::end(sMonoTonePrefix), std::begin(sSelectedToneName))) {
							// モノクロ用のトーンカーブが設定されていなければ、判定不要。
							bMonoToneSelected = true;
							break;
					}
				}

				// カラーの印刷面にモノクロ用のトーンが設定されている
				if ((nJobColorCountOfPrintSide != 1) && (bMonoToneSelected == true)) {
					// Show error dialog
					char strResource[_MAX_PATH] = {0};
					char msg[_MAX_PATH] = {0};
					_snprintf_s(strResource, _TRUNCATE, LoadResourceString(IDM_ERR_CANNOT_SET_MONOTONE, RESOURCE_MSG_STR), m_dotgainCurve[nSide].DOTGAIN_NAME);
					_snprintf_s(msg, sizeof(msg), _TRUNCATE, "%d\n%s", IDM_ERR_CANNOT_SET_MONOTONE + ID_MESSAGE_ERROR, strResource);
					ShowMessageBox(msg, MBST_OK | MBST_ICONERROR, NULL);
					return false;
				}
			}
		}
	}

	// Set data
	if (!SaveDotGainCurveToJobOrSystem()) {
		return false;
	}
	InitDotgainCurve(m_printConditionName);
	return true;
}

/**
 *	Compare two ST_PDB_DOTGAIN_CURVE structs
 *	@param[in]	inDotgainCurve1 : struct 1
 *	@param[in]	inDotgainCurve2 : struct 2
 *	@return true : two structs are not the same, false: otherwise
**/
bool CDataDotgainCurve::CompareTwoDotgainCurve(ST_PDB_DOTGAIN_CURVE inDotgainCurve1, ST_PDB_DOTGAIN_CURVE inDotgainCurve2)
{
	long resultCmpAnchorPoint = memcmp(inDotgainCurve1.ANCHOR_POINT, inDotgainCurve2.ANCHOR_POINT, sizeof(inDotgainCurve1.ANCHOR_POINT));
	long resultCmpDotgainName = memcmp(inDotgainCurve1.DOTGAIN_NAME, inDotgainCurve2.DOTGAIN_NAME, sizeof(inDotgainCurve1.DOTGAIN_NAME));
	long resultCmpDotgainSharName = memcmp(inDotgainCurve1.DOTGAIN_SHAR_NAME, inDotgainCurve2.DOTGAIN_SHAR_NAME, sizeof(inDotgainCurve1.DOTGAIN_SHAR_NAME));

	if(0 != resultCmpAnchorPoint || 0 != resultCmpDotgainName || 0 != resultCmpDotgainSharName) {
		return true;
	}

	return false;
}

/**
 *	Reset menber variables in the class
 *	@note General process
 *	
 *		Reset the member variables in the class
 *		Remove objects which created with dynamic.
**/
void CDataDotgainCurve::ResetMember()
{
	m_systemSetting = NULL;
	m_dataIF = NULL;
	memset(m_dotgainCurve, 0x00, sizeof(m_dotgainCurve));
	memset(m_initDotgainCurve, 0x00, sizeof(m_initDotgainCurve));

	m_isInitSuccess = false;
	memset(m_printConditionName, 0x00, sizeof(m_printConditionName));
	memset(m_dotgainCurve, 0x00, sizeof(m_dotgainCurve));
}

bool CDataDotgainCurve::EnterScreen(const char* inPrintConditionOrSectionID)
{
	// Reset member
	m_isInitSuccess = false;

	return InitDotgainCurve(inPrintConditionOrSectionID);
}

bool CDataDotgainCurve::ExitScreen()
{
	return true;
}

bool CDataDotgainCurve::UpdatePrintCondition(const char* inPrintConditionOrSectionID)
{
	return InitDotgainCurve(inPrintConditionOrSectionID);
}

/**
 *	Check if dotgain curve is default or not
 *	@note General process
 *	
 *
 *	@return true: is default, false : otherwise
**/
bool CDataDotgainCurve::IsDotgainCurveDefault_ForDrawing()
{
	ST_PDB_DOTGAIN_CURVE lDotgainCurve;
	if (GetDotgainCurve_ForDrawing(lDotgainCurve)) {
		return 0 == strcmp(lDotgainCurve.DOTGAIN_NAME, "");
	}
	return false;
}

/**
 *	Get tone frame width
 *	@retval	tone frame width
**/
long CDataDotgainCurve::GetToneFrameWidth_ForDrawing()
{
	return m_iniPrintConditionGUI.GetToneFrameWidth();
}

/**
 *	Get tone frame color
 *	@retval	tone frame color (R,G,B)
**/
COLORREF CDataDotgainCurve::GetToneFrameColor_ForDrawing()
{	
	//get tone frame color string from ini file
	string colors = m_iniPrintConditionGUI.GetToneFrameColor();
	std::string delimiter = ",";
	std::string token;
	size_t pos = 0;

	//get value of red color
	pos = colors.find(delimiter);
	token = colors.substr(0, pos);	
	long redColor = atoi(token.c_str());
	colors.erase(0, pos + delimiter.length());

	//get value of green color
	pos = colors.find(delimiter);
	token = colors.substr(0, pos);	
	long greenColor = atoi(token.c_str());
	colors.erase(0, pos + delimiter.length());

	//get value of blue color
	long blueColor = atoi(colors.c_str());

	return RGB(redColor, greenColor, blueColor);
}

/**
 *	Check existence of dot gain file before save
 *	@param[in]	inFilePath : Path of file
 *	@param[in]	inFilePathSize : Size of path
 *	@param[in]	inTargetColorName : Color file name
 *	@retval	true: Exist
 *	@retval	false: Not Exist
**/
bool CDataDotgainCurve::isToneCurveFile(TCHAR *inFilePath, long inFilePathSize, const char *inTargetColorName)
{
	if(!inFilePath || !inTargetColorName || inFilePathSize == 0) {
		return false;
	}
	TCHAR *pFileName = &inFilePath[inFilePathSize];
	{
		_snprintf_s(pFileName, _MAX_PATH - inFilePathSize, _TRUNCATE, "%s%s", inTargetColorName, DOTGAIN_LUTCURVE_EXT);
		if (!PathFileExists(inFilePath)) {
			std::stringstream logSS;
			logSS << "CDataDotgainCurve::isToneCurveFile error.";
			logSS << " FileName:[" << pFileName << "]";
			WriteToLogBuf(LOG_DEBUG, const_cast<char*>(logSS.str().c_str()));
			return false;
		}
	}
	return true;
}

/**
 *	Set current dotgain curve value to PrintConditionGUI
 *	@param[in]	inSystem : input eComSystemType
 *	@param[in]	inDotgainCurve : dotgain curve value
**/
void CDataDotgainCurve::SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve)
{
	if (COM_SYSTEM_TYPE_SELF == inSystem) {
		m_dotgainCurve[COM_SYSTEM_TYPE_SELF] = inDotgainCurve;
	} else {
		m_dotgainCurve[COM_SYSTEM_TYPE_PART] = inDotgainCurve;
	}
}

/**
 *	Get current dotGain curve value from PrintconditionGUI
 *	@param[in]	inSystem : eComSystemType value
 *	@param[out]	outDotgainCurve : dotgain curve value
 *	@retval	true: success
 *	@retval	false: failure
**/
bool CDataDotgainCurve::GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& outDotgainCurve)
{
	if (COM_SYSTEM_TYPE_SELF == inSystem) {
		outDotgainCurve = m_dotgainCurve[COM_SYSTEM_TYPE_SELF];
	} else {
		outDotgainCurve = m_dotgainCurve[COM_SYSTEM_TYPE_PART];
	}
	return true;
}

/**
 *	Save dotgain curve to Job or PaperDB
**/
bool CDataDotgainCurve::SaveDotGainCurveToJobOrSystem()
{
	bool retValue = true;
	if(m_systemSetting->IsStartedFromJobListScreen()) {
		// Save dotgain curve to Job data
		retValue &= m_systemSetting->GetJobManager_Callbacks()->JM_SetDotgainCurve(m_printConditionName, COM_SYSTEM_TYPE_SELF, m_dotgainCurve[COM_SYSTEM_TYPE_SELF]);
		if (m_systemSetting->IsDED()) {
			retValue &= m_systemSetting->GetJobManager_Callbacks()->JM_SetDotgainCurve(m_printConditionName, COM_SYSTEM_TYPE_PART, m_dotgainCurve[COM_SYSTEM_TYPE_PART]);
		}
	} else {
		// Save dotgain curve value to PaperDB
		retValue &= m_systemSetting->GetPaperDB_Callbacks()->PDB_SetDotgainCurve(m_printConditionName, COM_SYSTEM_TYPE_SELF, m_dotgainCurve[COM_SYSTEM_TYPE_SELF]);
		if (m_systemSetting->IsDED()) {
			retValue &= m_systemSetting->GetPaperDB_Callbacks()->PDB_SetDotgainCurve(m_printConditionName, COM_SYSTEM_TYPE_PART, m_dotgainCurve[COM_SYSTEM_TYPE_PART]);
		}
	}
	return retValue;
}
