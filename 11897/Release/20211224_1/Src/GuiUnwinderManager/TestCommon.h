/**
	* @file		TestCommon.h
	* @brief	試験用共通定義 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

#pragma once


enum JmfType
{	// JMF種別
	WM_USER_JMF_QueryStatus = (WM_USER+100),	// 状態監視
	WM_USER_JMF_QueryResource,					// 用紙情報登録
	WM_USER_JMF_StopPersChParams,				// チャネル削除
	WM_USER_JMF_CommandResource,				// 印刷条件設定
	WM_USER_JMF_CommandWakeup,					// READY状態への遷移要請
	WM_USER_CHECK_PROCESS,						// UWandRW_Receiver.exe存在チェック

	WM_USER_JMF_QueryStatus_END,				// 状態監視完了通知
	WM_USER_JMF_QueryResource_END,				// 用紙情報登録完了通知
	WM_USER_JMF_StopPersChParams_END,			// チャネル削除完了通知
	WM_USER_JMF_CommandResource_END,			// 印刷条件設定完了通知
	WM_USER_JMF_CommandWakeup_END,				// READY状態への遷移要請完了通知

	WM_USER_DSP_SEND,							// 送信データ表示通知
	WM_USER_DSP_RECV,							// 受信データ表示通知
};

//! ログ出力形式
enum {
	LOG_STATUS = 0,				//!< 通常の記録
	LOG_ERROR,					//!< エラー情報
	LOG_WARNING,				//!< 警告情報
	LOG_DEBUG,					//!< デバッグ情報
	LOG_HTML,					//!< HTML 直接出力
	LOG_OPERATION				//!< オペレーション情報
};

void WriteToLogBuf(long type, char* pString);

//! タイマーＩＤ
enum {
	TIMER_STATUS_RECEIVE = 1,	// SignalStatus(STATUS)受信
	TIMER_PAPER_RECEIVE,		// SignalStatus(PAPER)受信
	TIMER_PROCESS_CHECK,		// UWandRW_Receiver.exe存在チェック
};