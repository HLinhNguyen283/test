/**
 * @file	DataIF.h
 * @brief	data interface class
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
**/

#pragma once

// ==========================================================================
// Include files

#include <windows.h>
#include "BaseDataIF.h"
#include "DataSystemSetting.h"
#include "DataRequestImport.h"
#include "CenterTrans_Callbacks.h"
// ==========================================================================

class ReceiveImportPaperData;

/**
 * data interface class
**/
class CDataIF : public CBaseDataIF
{
public:
	/**
	 * @brief constructor
	 * @note General process
	 * 
	 *		Initialize the member variables
	**/
	CDataIF();

	/**
	 * @brief destructor
	 * @note General process
	 * 
	 *		Remove objects which created with dynamic.
	**/
	virtual ~CDataIF();

	/**
	 * @brief Initialize.
	 * @retval TRUE success
	 * @retval FALSE fail
	 * @note General process
	 * 
	 *		called by PIM_Initialize(NULL).
	*/
	virtual BOOL Initialize();

	/**
	 * @brief Finalize.
	 * @note General process
	 * 
	 *		called by PIM_Finalize(NULL).
	*/
	virtual void Finalize();

	/**
	 * @brief	全プラグイン初期化終了イベント
	 *  
	 *  全プラグインのPIM_Initialize()コール後にコールされる。
	 *  全てのプラグインの初期化が完了し、システムとして稼働できるようになった
	 *  タイミングでコールされるイベント。この関数コール以降、他プラグインのI/Fがコールできる。
	 * @retval	TRUE : 成功
	 * @retval	FALSE : 失敗
	**/
	virtual BOOL PIM_InitSystem();

	/**
	 * @brief	最終処理実行通知
	 *
	 *  アプリケーション終了処理開始直前にコールされる。
	**/
	virtual void PIM_LastWork();

	/**
	 * @brief	指定された印刷条件名がインポート時のベースとなるものか判定する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @retval	true : ベースである
	 * @retval	false : ベースではない
	**/
	bool IsBasePrintCondition(const std::string& inPrintConditionName);

protected:
	//method

	/**
	 * @brief	インポート用のスレッド
	 * @param	pParam : CDataIFクラスポインタ
	 * @returnl	1
	**/
	static UINT WINAPI ImportProc(LPVOID pParam);

	/**
	 * @brief	CenterTransのコマンド受信時にコールしてもらう関数
	 * @param[in,out]	ioParam : 受信内容が入ったオブジェクト
	 * @param[in,out]	ioUserData : ユーザー定義データ
	**/
	static void OnReceiveJMF(ReceiveParamRef ioParam, void* ioUserData);

	//event

	//member
	size_t m_RegisterReceiveCommandID;							//!< CenterTransに登録したID
	HANDLE			m_ThreadFinish;								//!< インポート用のスレッド終了イベント
	ST_THREAD_INFO m_ImportThread;								//!< インポート用のスレッド.
	CDataSystemSetting m_DataSystemSetting;						//!< システムの情報
	std::unique_ptr<CDataRequestImport> m_DataRequestImport;	//!< 印刷条件のインポート要求に対する処理を行うクラス
	class API;
private:
	//member
	
};

