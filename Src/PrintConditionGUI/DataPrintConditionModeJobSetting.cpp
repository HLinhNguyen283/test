/*! \file DataPrintConditionModeJobSetting.cpp
 *  \brief 印刷条件モードジョブ設定データクラス
 *  \author okada
 *  \date 2014/11/21 okada 新規作成
 */

#include "StdAfx.h"
#include "DataPrintConditionModeJobSetting.h"

#include "DataIF.h"
#include "DataSystemSetting.h"
#include "PaperDB_Callbacks.h"

#include "CommonCBRProc.h"
#include "CommonCBRProc_OP.h"

#include "DlgDummy.h"
#include <string>
#include <sstream>

#include "ResDef.h"
#include "PluginCBRProc.h"

// 画面表示のためのパラメータ.
#include "JobBaseGUI_IFStruct.h"
#include "LayoutImageDef_OP.h"

 /*!
  * \brief コンストラクタ
  */
 CDataPrintConditionModeJobSetting::CDataPrintConditionModeJobSetting()
	: m_PaperDBCallbacks(NULL)
		, m_DataIF(NULL)
		, m_DataSstemSetting(NULL)
 {
 }

 /*!
  * \brief デストラクタ
  */
 CDataPrintConditionModeJobSetting::~CDataPrintConditionModeJobSetting()
 {
 }

/*!
 * \brief ジョブ設定を開始
 * \return 処理結果
 * \note 印刷条件設定画面でジョブ設定ボタン押下時に印刷条件モードでジョブ設定を開始する
 */
bool CDataPrintConditionModeJobSetting::StartJobSetting()
{
	// 処理開始ログ.
	WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::StartJobSetting START. ");

	// 未保存のパラメータがあるかチェック
	if (!m_DataIF->IsSavedPrintSetting()) {
		// 未保存パラメータがある場合

		std::stringstream errorMessage;
		errorMessage << ID_MESSAGE_ERROR + IDM_CHANGE_NOTSAVED_PREVIEW_NOT_CORRECT_SIZE << "\n" << LoadResourceString(IDM_CHANGE_NOTSAVED_PREVIEW_NOT_CORRECT_SIZE, RESOURCE_MSG_STR);
		if (ShowMessageBox(const_cast<char*>(errorMessage.str().c_str()), MBST_YESNO | MBST_ICONINFORMATION, NULL) == IDNO) {
			return true;
		}
	}

	const char* printerPrintCondition = NULL;
	// Get Selected print condition
	long selectedIndex = 0;
	if (!m_DataIF->GetSelectPrintCondition(&selectedIndex)) {
		// 取得失敗.
		// デバッグログ
		WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::StartJobSetting GetSelectPrintCondition failed. ");

		return false;
	}
	if (!m_DataIF->GetPrintConditionList(selectedIndex, &printerPrintCondition)) {
		// 取得失敗.
		// デバッグログ
		WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::StartJobSetting GetPrintConditionList failed. ");

		return false;
	}

	// 選択されている印刷条件名に紐づくジョブパラメータを紙DBに登録を依頼
	const std::string currentPrintConditionName = printerPrintCondition ? printerPrintCondition : "";
	if (!m_PaperDBCallbacks->PDB_InitializeParamFromPrintConditionGUI(currentPrintConditionName)) {
		// 取得失敗.
		// デバッグログ
		WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::StartJobSetting. PDB_InitializeParamFromPrintConditionGUI failed.");

		return false;
	}

	// ダミーダイアログ作成
	CDlgDummy dummyDialog;
	dummyDialog.Create(NULL, NULL);

	// 表示のためのパラメータ.
	const SJobBaseGUIShowInfo jobbase_param(currentPrintConditionName);

	SLayoutImageShowInfo layout_param;
	layout_param.printConditionName = currentPrintConditionName;

	bool isIpdsMode = m_DataSstemSetting->IsIpdsMode();
	if(!isIpdsMode){
		// 前へ/次へで基本情報設定画面/レイアウト画面を表示するためのループ.
		while (1) {
			// 基本情報設定画面表示依頼.
			if (!JobBaseGUI_ShowDialog_PrintCondition(jobbase_param)) {
				// 印刷条件設定画面を表示する場合.繰り返しを抜ける
				break;
			}
			// レイアウト設定画面表示依頼.
			if (!LayoutImage_ShowDialog_PrintCondition(layout_param)) {
				// 印刷条件設定画面を表示する場合.繰り返しを抜ける
				break;
			}
		}
	}else{
		// IPDSモード時はレイアウト画面だけを表示する
		LayoutImage_ShowDialog_PrintCondition(layout_param);
	}

	// ダミーダイアログ破棄
	dummyDialog.Destroy();

	// 処理終了ログ.
	WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::StartJobSetting END. ");

	return true;
}

/*!
 * \brief 初期化
 * \return 処理結果
 */
bool CDataPrintConditionModeJobSetting::Initialize(CDataIF* inDataIF, CDataSystemSetting* inSDataystemSetting)
{
	if (!inDataIF) {
		WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::Initialize failed. invalid parameter[CDataIF].");

		return false;
	}
	if (!inSDataystemSetting) {
		WriteToLogBuf(LOG_DEBUG, "CDataPrintConditionModeJobSetting::Initialize failed. invalid parameter[CDataSystemSetting].");

		return false;
	}

	m_DataIF = inDataIF;
	m_DataSstemSetting = inSDataystemSetting;

	m_PaperDBCallbacks = m_DataSstemSetting->GetPaperDB_Callbacks();

	return true;
}
