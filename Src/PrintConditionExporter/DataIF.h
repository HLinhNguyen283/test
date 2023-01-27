/*! @file DataIF.h
 *  @brief data interface class
 *  @author pt.ju-tanaka
 *  @date 2019/2/5 pt.ju-tanaka new
*/

#pragma once

// ==========================================================================
// Include files

#include <windows.h>
#include "BaseDataIF.h"
#include "DataSystemSetting.h"
#include "DataRequestExport.h"
#include "CenterTrans_Callbacks.h"
#include "PaperDB_MigrationDef.h"
#include "UpdateRequestExport.h"
#include "DataRequestExportXML.h"
// ==========================================================================

class ReceiveExportPaperData;

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
	 * @brief	全プラグイン終了開始前イベント
	 *  
	 *  全プラグインのPIM_Finalize()コール前にコールされる。
	 *  全てのプラグインの終了処理が行われる前にコールされる。
	 *  この関数コール以降、他プラグインのI/Fがコールできない。
	 * @retval	TRUE : 成功
	 * @retval	FALSE : 失敗
	**/
	virtual BOOL PIM_ExitSystem();

	/**
	 * @brief	最終処理実施イベント
	**/
	virtual void PIM_LastWork();

	/**
	 * @brief	システムの印刷条件更新を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inMigrationData : 業務移行用構造体
	**/
	void onUpdatePrintCondition(const std::string& inPrintConditionName, const ST_PDB_MIGRATION_DATA& inMigrationData);

	/**
	 * @brief	システムの印刷条件削除を通知する
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void onDeletePrintCondition(const std::string& inPrintConditionName);

	/**
	 * @brief	JI、検査関連フォルダのエクスポート（ミラーリングコピー）
	 * @param[in]	検査条件フォルダ（Clientフォルダからの相対パス）
	**/
	bool onExportInspectionData(const std::string& inJIDataFolder);

protected:
	//method

	/**
	 * @brief	エクスポート用のスレッド
	 * @param	pParam : CDataIFクラスポインタ
	 * @returnl	1
	**/
	static UINT WINAPI ExportProc(LPVOID pParam);

	/**
	 * @brief	テキスト更新用のスレッド
	 * @param	pParam : CDataIFクラスポインタ
	 * @returnl	1
	**/
	static UINT WINAPI updateProc(LPVOID pParam);

	/**
	 * @brief	XML更新用のスレッド
	 * @param	pParam : CDataIFクラスポインタ
	 * @returnl	1
	**/
	static UINT WINAPI updateXMLProc(LPVOID pParam);

	/**
	 * @brief	CenterTransのコマンド受信時にコールしてもらう関数
	 * @param[in,out]	ioParam : 受信内容が入ったオブジェクト
	 * @param[in,out]	ioUserData : ユーザー定義データ
	**/
	static void OnReceiveJMF(ReceiveParamRef ioParam, void* ioUserData);

	//event

	//member
	size_t m_RegisterReceiveCommandID;							//!< CenterTransに登録したID
	HANDLE			m_ThreadFinish;								//!< エクスポート用のスレッド終了イベント
	ST_THREAD_INFO m_ExportThread;								//!< エクスポート用のスレッド.
	std::unique_ptr<CDataRequestExport> m_DataRequestExport;	//!< 印刷条件のエクスポート要求に対する処理を行うクラス
	class API;
	
private:
	//method
	/**
	 * @brief	テキスト更新依頼の実施
	**/
	void onUpdateThred();

	/**
	 * @brief	XML更新依頼の実施
	**/
	void onUpdateXMLThred();

	//member
	std::unique_ptr<UpdateRequestExport> m_UpdateRequestExport;		//!< エクスポート更新要求に対する処理を行うクラス
	std::unique_ptr<DataRequestExportXML> m_DataRequestExportXML;	//!< XMLエクスポート更新要求に対する処理を行うクラス
};

