/**
 * @file	DataRequestExport.h
 * @brief	印刷条件のエクスポート要求に対する処理を行う
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include "Common.h"
#include "ThreadSafe.h"

#include "ReceiveExportPaperData.h"

#include <list>

/**
 * @brief 印刷条件のエクスポート要求に対する処理を行う
**/
class CDataRequestExport
{
public:
	/**
	 * @brief	constructor
	**/
	CDataRequestExport();

	/**
	 * @brief	destructor
	**/
	virtual ~CDataRequestExport();

	/**
	 * @brief	ExportPaperDataコマンド受信イベント
	 * @param[in]	inPaperDataURL : エクスポート先のルートパス(UNC)
	 * @retval	kReturnCode_Success : エラー無し
	 * @retval	kReturnCode_InvalidPaperDataURL : 取り込み元パスが不正
	**/
	ReceiveExportPaperData::EReturnCode OnReceiveExportPaperData(const std::string& inPaperDataURL);

	/**
	 * @brief	エクスポート開始イベント
	**/
	void OnStartExport();

	/**
	 * @brief	システム終了イベント
	**/
	void OnExitSystem();

protected:
	CThreadSafe<std::list<std::string>> m_Requests;	//!< 依頼一覧

};


