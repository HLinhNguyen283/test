/*! \file CtlPrintConditionCommitting.cpp
 *  \brief The control saves the settings of the selection print settings on the screen.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintConditionCommitting.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
#include "CommonUiMsg_OP.h"
#include "PrintConditionGUIDef.h"
#include "Ini_PrintConditionGUI.h"

#include <sstream>

// ==========================================================================
// Local definitions
enum {
	PARAM_TONECURVE_NOT_CHANGED = 0,		//!< Tone curve is NOT changed
	PARAM_TONECURVE_CHANGED,				//!< Tone curve is changed
};

#define DEF_MAX_REASONABLE_PRECOAT	3		//!< Maximum value of Precoat
#define DEF_MIN_REASONABLE_PRECOAT	1		//!< Minimum value of Precoat
#define DEF_MIN_PRINT_SPEED			15		//!< Maximum value of printing speed
#define DEF_MAX_PRINT_SPEED			75		//!< Minimum value of printing speed

//! the control id list
enum{
	CTRLID_BN_SAVE = 0,				//!< Button: Save

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_save = "CTRLID_BN_SAVE";
// ==========================================================================

/*! \brief constructor
*/
CCtlPrintConditionCommitting::CCtlPrintConditionCommitting()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;
}

/*! \brief destructor
*/
CCtlPrintConditionCommitting::~CCtlPrintConditionCommitting()
{
}

/*! \brief [event]set the control's property
*/
void CCtlPrintConditionCommitting::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button： Save
	{
		long ctlId = CTRLID_BN_SAVE;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_TEXT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_SAVE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 1010, 610, 1010 + 160, 610 + 70);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_SAVE, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_save;
	}
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPrintConditionCommitting::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	////////////////
	// サブ関数
	////////////////

	auto checkPrintSpeed = [&](long inPrintSpeed)->bool
	{
		////////////////
		// サブ関数
		////////////////

		// inMsg文字列中のinTypeFieldで指定した文字列をinValueの値に置き換える
		auto changeMessage = [](const std::string& inMsg, const std::string& inTypeField, long inValue)->const std::string
		{
			std::string msg = inMsg;
			size_t pos = msg.find(inTypeField);
			if (pos != std::string::npos) {
				msg.replace(pos, 2, std::to_string(static_cast<long long>(inValue)));
			}
			return msg;
		};

		// 必要であればテンションのエラーメッセージを返す
		auto addMsgTension = [&](CIni_PrintConditionGUI& inIni, CDataIF* inDataIF, bool& outIsShowErrorMessage)->std::string
		{
			long paperTensionMax = inIni.GetHighSpeedProperValueTensionMax();
			
			//// ※値が範囲外のときのみ出すのであればこの以下のコメントアウト部を有効とし下の処理を通す
			//if (inDataIF->GetSelectPaperTensionValue() > paperTensionMax) {
			//	outIsShowErrorMessage = true;
			//	return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_TENSION, RESOURCE_MSG_STR), "%d", paperTensionMax);
			//}
			//return "";
			if (inDataIF->GetSelectPaperTensionValue() > paperTensionMax) {
				outIsShowErrorMessage = true;
			}
			return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_TENSION, RESOURCE_MSG_STR), "%d", paperTensionMax);
		};

		// 必要であればアンダーコートのエラーメッセージを返す
		auto addMsgUnderCoat = [&](CIni_PrintConditionGUI& inIni, CDataIF* inDataIF, bool& outIsShowErrorMessage)->std::string
		{
			long underCoat1;
			inDataIF->GetSelectPrecoat(1, &underCoat1);
			long underCoat2;
			inDataIF->GetSelectPrecoat(2, &underCoat2);

			long underCoatMax = inIni.GetHighSpeedProperValueUnderCoat();

			//// ※値が範囲外のときのみ出すのであればこの以下のコメントアウト部を有効とし下の処理を通す
			//if ((inDataIF->IsExistPrecoater() == true) && ((underCoat1 > underCoatMax) || (underCoat2 > underCoatMax))) {
			//	outIsShowErrorMessage = true;
			//	return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_UNDERCOAT, RESOURCE_MSG_STR), "%d", underCoatMax);
			//}
			if (inDataIF->IsExistPrecoater() == true) {
				if ((underCoat1 > underCoatMax) || (underCoat2 > underCoatMax)) {
					outIsShowErrorMessage = true;
				}
				return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_UNDERCOAT, RESOURCE_MSG_STR), "%d", underCoatMax);
			}
			return "";
		};

		// 必要であればプロテクターコートのエラーメッセージを返す
		auto addMsgProtectorCoat = [&](CIni_PrintConditionGUI& inIni, CDataIF* inDataIF, bool& outIsShowErrorMessage)->std::string
		{
			long protectorCoat1;
			inDataIF->GetSelectOvercoat(1, &protectorCoat1);
			long protectorCoat2;
			inDataIF->GetSelectOvercoat(2, &protectorCoat2);

			long protectorCoatMax = inIni.GetHighSpeedProperValueProtectorCoat();

			//// ※値が範囲外のときのみ出すのであればこの以下のコメントアウト部を有効とし下の処理を通す
			//if (((inDataIF->IsExistOvercoater(1) == true) || (inDataIF->IsExistOvercoater(2) == true)) && ((protectorCoat1 > protectorCoatMax) || (protectorCoat2 > protectorCoatMax))) {
			//		outIsShowErrorMessage = true;
			//		return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_PROTECTORCOAT, RESOURCE_MSG_STR), "%d", protectorCoatMax);
			//	}
			//}
			if ((inDataIF->IsExistOvercoater(1) == true) || (inDataIF->IsExistOvercoater(2) == true)) {
				if ((protectorCoat1 > protectorCoatMax) || (protectorCoat2 > protectorCoatMax)) {
					outIsShowErrorMessage = true;
				}
				return changeMessage(LoadResourceString(IDM_ERR_PRINTSPEED_PROTECTORCOAT, RESOURCE_MSG_STR), "%d", protectorCoatMax);
			}
			return "";
		};

		////////////////
		// 処理
		////////////////

		if (inPrintSpeed == 150) {
			// エラーメッセージ（OKボタン）を表示する
			bool isShowErrorMessage = false;
			std::stringstream msg;
			msg << (ID_MESSAGE_ERROR + IDM_ERR_PRINTSPEED_BASE) << "\n" << LoadResourceString(IDM_ERR_PRINTSPEED_BASE, RESOURCE_MSG_STR);

			CIni_PrintConditionGUI ini_PrintConditionGUI;
			ini_PrintConditionGUI.Initialize(TRUE);
			{
				CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
				msg << addMsgTension(ini_PrintConditionGUI, pData, isShowErrorMessage);
				msg << addMsgUnderCoat(ini_PrintConditionGUI, pData, isShowErrorMessage);
				msg << addMsgProtectorCoat(ini_PrintConditionGUI, pData, isShowErrorMessage);
			}
			ini_PrintConditionGUI.Finalize();

			if (isShowErrorMessage == true) {
				ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_ICONERROR | MBST_OK, NULL);
				return false;
			}
		}

		return true;
	};

	auto checkIRDryer = [&]()->bool
	{
		////////////////
		// サブ関数
		////////////////

		// 必要であれば印刷速度のエラーメッセージを返す
		auto addMsgPrintSpeed = [&](CIni_PrintConditionGUI& inIni, CDataIF* inDataIF, bool& outIsShowErrorMessage)->std::string
		{
			long printSpeed = inDataIF->GetSelectPrintSpeedValue()/100;
			long printSpeedMin = inIni.GetIRDryerProperValuePrintSpeedMin();

			if ((printSpeedMin != 0) && (printSpeedMin > printSpeed)) {
				outIsShowErrorMessage = true;
				char msgStr[256] = {0};
				_snprintf_s(msgStr, _TRUNCATE, LoadResourceString(IDM_ERR_IRDRYER_PRINTSPEED, RESOURCE_MSG_STR), printSpeedMin);
				std::string	msg = std::string(msgStr);
				return msg;
			}
			return "";
		};

		// 必要であれば紙厚のエラーメッセージを返す
		auto addMsgPaperThickness = [&](CIni_PrintConditionGUI& inIni, CDataIF* inDataIF, bool& outIsShowErrorMessage)->std::string
		{
			long paperThickness = inDataIF->GetPaperThickness();
			long paperThicknessMin = inIni.GetIRDryerProperValueThicknessMin();

			if ((paperThicknessMin != 0) && (paperThicknessMin > paperThickness)) {
				outIsShowErrorMessage = true;
				char msgStr[256] = {0};
				_snprintf_s(msgStr, _TRUNCATE, LoadResourceString(IDM_ERR_IRDRYER_PAPERTHICKNESS, RESOURCE_MSG_STR), paperThicknessMin/1000.);
				std::string msg = std::string(msgStr);
				return msg;
			}
			return "";
		};

		////////////////
		// 処理
		////////////////

		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		
		bool		isDED = pData->IsDED();
		bool		isIRDryer = false;

		if (isDED) {
			// Self
			if(pData->IsExistNirDryerUnit(2) && (0 != pData->GetSelectNirPower(true))) {
				isIRDryer = true;
			}
			// Part
			if(pData->IsExistNirDryerUnit(1) && (0 != pData->GetSelectNirPower(false))) {
				isIRDryer = true;
			}
		} else {
			// Self
			if(pData->IsExistNirDryerUnit(1) && (0 != pData->GetSelectNirPower(true))) {
				isIRDryer = true;
			}
		}

		if (isIRDryer == true) {

			// エラーメッセージ（OKボタン）を表示する
			bool isShowErrorMessage = false;
			std::stringstream msg;
			msg << (ID_MESSAGE_ERROR + IDM_ERR_IRDRYER_BASE) << "\n" << LoadResourceString(IDM_ERR_IRDRYER_BASE, RESOURCE_MSG_STR);

			CIni_PrintConditionGUI ini_PrintConditionGUI;
			ini_PrintConditionGUI.Initialize(TRUE);
			{
				msg << addMsgPrintSpeed(ini_PrintConditionGUI, pData, isShowErrorMessage);
				msg << addMsgPaperThickness(ini_PrintConditionGUI, pData, isShowErrorMessage);
			}
			ini_PrintConditionGUI.Finalize();

			if (isShowErrorMessage == true) {
				ShowMessageBox(const_cast<char*>(msg.str().c_str()), MBST_ICONERROR | MBST_OK, NULL);
				return false;
			}
		}

		return true;
	};

	////////////////
	// 処理
	////////////////

	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam){
	case BN_CLICKED:
		{
			// Save button
			if(ctlWnd == m_ctl[CTRLID_BN_SAVE]){
				//Check blank item
				if(pData->IsBlankNotAllowed()) {
					// show message box.
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_MSG_BLANK_NOT_ALLOWED, LoadResourceString(IDM_MSG_BLANK_NOT_ALLOWED, RESOURCE_MSG_STR));
					ShowMessageBox(msg, MBST_OK | MBST_ICONERROR, NULL);
					return DEF_NONE;
				}

				DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

				// Get ON/OFF of a tractor hole and a perforations/a preprinted mark
				bool isHoleAndPer = pData->GetOptionTractorHolesAndPerforations();
				bool isPrePrintedMark = pData->GetOptionPreprintedMark(&printSide);

				// Get precoat amount and printing speed
				bool isDED = pData->IsDED();
				bool isHavePrecoater = pData->IsExistPrecoater();
				bool isShowErrorMessage = false;
				long printSpeed = pData->GetSelectPrintSpeedValue()/100;
				long nPrecoatFront = -1;
				long nPrecoatBack = -1;
				//If have precoater
				if(isHavePrecoater) {
					if(!pData->GetSelectPrecoat(DEF_PRINTER_1, &nPrecoatFront)) {
						return DEF_NONE;
					}
					if(isDED) {
						if(!pData->GetSelectPrecoat(DEF_PRINTER_2, &nPrecoatBack)) {
							return DEF_NONE;
						}
					}

					bool isPrecoatReasonableFront = (nPrecoatFront < DEF_MIN_REASONABLE_PRECOAT || nPrecoatFront > DEF_MAX_REASONABLE_PRECOAT) ? false : true;
					bool isPrecoatReasonableBack = false;
					if(isDED) {
						isPrecoatReasonableBack = (nPrecoatBack < DEF_MIN_REASONABLE_PRECOAT || nPrecoatBack > DEF_MAX_REASONABLE_PRECOAT) ? false : true;
					}

					if((isPrecoatReasonableFront || isPrecoatReasonableBack)									//If the precoat amount is between 1 and 3
						&& (printSpeed < DEF_MIN_PRINT_SPEED || printSpeed > DEF_MAX_PRINT_SPEED)) {			//and the printing speed is slower than 15 m/min (< 15) or faster than 75 m/min (> 75)
						isShowErrorMessage = true;
					}
				}
				//Display the following error message
				if(isShowErrorMessage) {
					// show message box.
					char strPrecoat[256] = {0};
					char strError[256] = {0};
					char msg[256] = {0};
					_snprintf_s(strPrecoat, _TRUNCATE, isDED ? "%d/%d" : "%d", nPrecoatFront, nPrecoatBack);
					_snprintf_s(strError, _TRUNCATE, LoadResourceString(IDM_MSG_ERROR_PRECOAT_AND_SPEED, RESOURCE_MSG_STR), printSpeed, strPrecoat);
					_snprintf_s(msg, _TRUNCATE, "%d\n%s", ID_MESSAGE_ERROR + IDM_MSG_ERROR_PRECOAT_AND_SPEED, strError);
					ShowMessageBox(msg, MBST_OK | MBST_ICONERROR, NULL);
					//prevent the changes from being saved
					return DEF_NONE;
				}

				//印刷速度をチェックする
				if (checkPrintSpeed(printSpeed) == false) {
					return DEF_NONE;
				}

				// Check conditions when IR dryer is enabled.
				if (checkIRDryer() == false) {
					return DEF_NONE;
				}

				// ジョブ印刷条件設定コピーを依頼
				bool isShowCopyMessage = false;  //!< コピー確認ダイアログ表示フラグ(true: 表示された)
				pData->OnCopyJobPrintCondition(isShowCopyMessage);

				// check if dialog started from job list screen.
				if(!pData->IsStartFromSystemSetting()) {
					// ジョブの印刷条件設定
					// コピー確認ダイアログが表示されていない場合のみ、メッセージを表示する
					if (!isShowCopyMessage) {
						char msg[256] = {0};
						_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_COMMIT_CLICK_JOB_LIST_SCREEN, LoadResourceString(IDM_COMMIT_CLICK_JOB_LIST_SCREEN, RESOURCE_MSG_STR));
						ShowMessageBox(msg, MBST_OK | MBST_ICONINFORMATION, NULL);
					}
				}
				else {
					// システムの印刷条件設定
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_COMMIT_CLICK_SYSTEM_SETTING_SCREEN, LoadResourceString(IDM_COMMIT_CLICK_SYSTEM_SETTING_SCREEN, RESOURCE_MSG_STR));
					ShowMessageBox(msg, MBST_OK | MBST_ICONINFORMATION, NULL);
				}

				//if paper size is changed and JI option is enable then show dialog to confirm the JI parameters
				if (pData->IsPaperSizeChanged() && pData->IsJIOptionEnable()){
					// show message box.
					char msg[256] = {0};
					long msgID = pData->IsIpdsMode() ? IDM_MSG_PAPER_SIZE_IS_CHANGED_IPDS : IDM_MSG_PAPER_SIZE_IS_CHANGED;
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + msgID, LoadResourceString(msgID, RESOURCE_MSG_STR));
					ShowMessageBox(msg, MBST_OK | MBST_ICONINFORMATION, NULL);
				}

				// Save the settings of the selected print settings on the screen
				pData->SavePrintCondition();
				// Display message-box when
				// - The selected paper has tractor holes and perforations.
				// - The selected paper does not have pre-printed mark.
				if(true == isHoleAndPer
					&& false == isPrePrintedMark)
				{
					//Display a message box
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_SET_CURRENT_PRINT_INFO, LoadResourceString(IDM_SET_CURRENT_PRINT_INFO, RESOURCE_MSG_STR));

					ShowMessageBox(msg, MBST_OK | MBST_ICONWARNING, NULL);
				}

				// check and show message box warning : ICC profile is not assigned to the ICC preset used in the print setting to be saved.
				if(pData->IsICCProfileNotAssignedToICCPreset())
				{
					// get current mode name 
					char * currentModeName = NULL;
					pData->GetCurrentPrintModeName(&currentModeName);
					// show message box.
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, LoadResourceString(IDM_ICC_PROFILE_IS_ASSIGNED_OR_NOT, RESOURCE_MSG_STR),currentModeName);
					ShowMessageBox(msg, MBST_OKCANCEL | MBST_ICONINFORMATION, NULL);
				}
			}
		}
		break;
	}
	
	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlPrintConditionCommitting::OnUpdateState()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	DWORD dwState = CST_SHOW;


	// Disable control when one of the next conditions met
	// - The selected print settings is locked.
	// - Started from system screen, General mode and edit print condition is prohibited.
	// - There is an item which is not set
	if(true == pData->GetLockPrintCondtion()
		|| true == pData->IsDisablePrintCondition()
		|| false == pData->IsAbleSavePrintSetting())
	{
		dwState |= CST_DISABLED;
	}
	else{
		dwState |= CST_ENABLED;
	}

	SetControlState(m_ctl[CTRLID_BN_SAVE], dwState);
}


