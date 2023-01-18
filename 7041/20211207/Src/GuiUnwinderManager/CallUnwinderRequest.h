/**
	* @file		CallUnwinderRequest.h
	* @brief	Unwinder要求窓口サンプル ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi 新規作成
**/

#pragma once

namespace unwinder_manager
{

class CCallUnwinderRequest
{
	/**
		* @brief  Unwionder処理無効の場合の処理
		* @retval 無効の文字列(仮)
	**/
	static std::string NotSupportedUnwinderFunction();

public:
	/**
		* @brief  Unwionder処理の有効/無効を取得する
		* @retval TRUE：Unwionder処理有効
		* @retval FALSE：Unwionder処理無効
	**/
	static BOOL IsUnwinderFunction();

	/**
		* @brief  UWに状態監視用チャネル登録を要求するサンプル
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallQueryStatus(CWnd* pWnd);

	/**
		* @brief  UWに用紙情報通知用チャネル登録を要求するサンプル
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallQueryResource(CWnd* pWnd);

	/**
		* @brief  UWにチャネル削除を要求するサンプル
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
		* @param  InType: [i]チャネル種別
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallStopPersChParams(CWnd* pWnd,int InType);

	/**
		* @brief  UWに印刷条件設定を要求するサンプル
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallCommandResource();

	/**
		* @brief  UWにREADY状態への遷移を要求するサンプル
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallCommandWakeup();

	/**
		* @brief  UWからSignalStatusを受信するサンプル
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	**/
	static void CallSignalStatus(CWnd* pWnd);

	/**
		* @brief  プロセスの状態をチェックする
		* @param  InProcessName  : [i]プロセス名称
	**/
	static void CallProcessCheck( const std::string& InProcessName );

	/**
		* @brief  プロセスが存在するかチェックする
		* @param  InProcessName  : [i]プロセス名称
	**/
	static BOOL ProcessRunningCheck( const std::string& InProcessName );

	/**
		* @brief  プロセスを起動する
		* @param  InProcessName  : [i]プロセス名称
	**/
	static BOOL ExecuteProcess( const std::string& InProcessName );

	/**
		* @brief  SignalStatus(STATUS)タイムアウトタイマー設定
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	**/
	static void SetTimerStatusReceive(CWnd* pWnd);

	/**
		* @brief  SignalStatus(PAPER)タイムアウトタイマー設定
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	**/
	static void SetTimerPaperReceive(CWnd* pWnd);

	/**
		* @brief  SignalStatus(STATUS)タイムアウトタイマー停止
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	**/
	static void KillTimerStatusReceive(CWnd* pWnd);

	/**
		* @brief  SignalStatus(PAPER)タイムアウトタイマー停止
		* @param  pWnd  : [i]親画面のウィンドウハンドル(テスト用)
	**/
	static void KillTimerPaperReceive(CWnd* pWnd);

};

};	// namespace ink_code_manager