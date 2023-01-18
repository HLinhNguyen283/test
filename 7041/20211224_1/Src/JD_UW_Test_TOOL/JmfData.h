/**
 * @file  JmfData.h
 * @brief JMFの雛形データ ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/
#pragma once

/* ==================================================================================================== */
/**
	* @brief  送信データの雛形を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval  送信データの雛形
**/
/* ==================================================================================================== */
CString GetJmfData(UINT nJmfType);

/* ==================================================================================================== */
/**
	* @brief  送信レスポンスデータの雛形を取得する
	* @param  nJmfType	: [i]JMF種別
	* @retval  送信レスポンスデータの雛形
**/
/* ==================================================================================================== */
CString GetJmfResponseData(UINT nJmfType);

extern TCHAR Jmf_QueryStatus[];				// 状態監視
extern TCHAR Jmf_QueryResource[];			// 用紙情報登録
extern TCHAR Jmf_StopPersChParams[];		// チャネル削除
extern TCHAR Jmf_CommandResource[];			// 印刷条件設定
extern TCHAR Jmf_CommandWakeup[];			// READY状態への遷移要請

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 試験対向用
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern TCHAR Jmf_SignalStatus [];			// UWからのSignalStatus(データ固定)
extern TCHAR Jmf_SignalStatusPaper[];
extern TCHAR Jmf_QueryStatusRes[];			// 状態監視のレスポンス(データ固定)
extern TCHAR Jmf_QueryResourceRes[];		// 用紙情報登録のレスポンス(データ固定)
extern TCHAR Jmf_StopPersChParamsRes[];		// チャネル削除のレスポンス(データ固定)
extern TCHAR Jmf_CommandResourceRes[];		// 印刷条件設定のレスポンス(データ固定)
extern TCHAR Jmf_CommandWakeupRes[];		// READY状態への遷移要請のレスポンス(データ固定)

