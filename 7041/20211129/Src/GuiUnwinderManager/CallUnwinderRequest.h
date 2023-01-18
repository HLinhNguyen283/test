/**
	* @file		CallUnwinderRequest.h
	* @brief	Unwinder要求窓口サンプル ヘッダファイル
	* @author	cec K.Miyachi
	* @date		20211119 cec K.Miyachi 新規作成
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
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallQueryStatus();

	/**
		* @brief  UWに用紙情報通知用チャネル登録を要求するサンプル
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallQueryResource();

	/**
		* @brief  UWにチャネル削除を要求するサンプル
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallStopPersChParams();

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
		* @retval 便宜上結果を文字列で戻している(仮)
	**/
	static std::string CallSignalStatus();
};

};	// namespace ink_code_manager