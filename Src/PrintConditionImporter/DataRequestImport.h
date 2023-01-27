/**
 * @file	DataRequestImport.h
 * @brief	印刷条件のインポート要求に対する処理を行う
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

#include "Common.h"
#include <memory>

class CDataSystemSetting;

/**
 * @brief 印刷条件のインポート要求に対する処理を行う
**/
class CDataRequestImport
{
public:
	//! 追加時のエラー種類
	enum E_ADD_RESULT_TYPE
	{
		ADD_SUCCESS = 0,				//!< 追加成功
		ADD_ERROR_FILE_COPY,			//!< 追加失敗[元ファイルのコピー失敗(元ファイルのパスが無効でコピーできないも含む)]
		ADD_ERROR_INVALID_BASE_DATA,	//!< 追加失敗[ベースとなる印刷条件が無効]
	};

	/**
	 * @brief	constructor
	 * @param[in]	inDataSystemSetting : システムのデータ
	**/
	CDataRequestImport(CDataSystemSetting& inDataSystemSetting) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CDataRequestImport() throw();

	/**
	 * @brief	ImportPaperDataコマンド受信イベント
	 * @param[in]	inJmfCommandId : JMFのコマンドID
	 * @param[in]	inPaperDataURL : 取り込み元のパス(UNC)
	 * @param[in]	inIsOverwritePaper : 同名異内容の印刷条件があった場合上書きするかどうか
	 * @param[in]	inIsAppendPaper : 同名の印刷条件がない場合に追加するかどうか
	 * @retval	ADD_SUCCESS : 追加成功
	 * @retval	ADD_ERROR_FILE_COPY : 追加失敗[元ファイルのコピー失敗(元ファイルのパスが無効でコピーできないも含む)]
	 * @retval	ADD_ERROR_INVALID_BASE_DATA : 追加失敗[ベースとなる印刷条件が無効]
	**/
	E_ADD_RESULT_TYPE OnReceiveImportPaperData(const std::string& inJmfCommandId, const std::string& inPaperDataURL, bool inIsOverwritePaper, bool inIsAppendPaper) throw();

	/**
	 * @brief	インポート開始イベント
	**/
	void OnStartImport() throw();

	/**
	 * @brief	システム終了イベント
	**/
	void OnExitSystem() throw();

protected:

	/**
	 * @brief	1件分の依頼を行う
	**/
	void OneImport() throw();

	CDataSystemSetting& m_DataSystemSetting;		//!< システムのデータ

	class CRequestQueue;
	std::unique_ptr<CRequestQueue> m_RequestQueue;	//!< 依頼受付

	class API;
};


