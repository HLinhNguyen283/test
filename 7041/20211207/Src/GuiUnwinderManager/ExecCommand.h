/**
	@file	ExecCommand.h
	@brief	外部コマンド実行
	@author	cec k.miyachi
	@date	2021/11/19 cec k.miyachi 新規作成
*/

#pragma once

#include <windows.h>
#include <string>

namespace unwinder_manager
{

/**
	@brief	外部コマンド実行クラス
	@note	どこからでも使えるようにシングルトン
*/
class ExecCommand
{
private:
	/**
		@brief	constructor
	*/
	ExecCommand();

	/**
		@brief	destructor
	*/
	~ExecCommand();

	/**
		@brief	constructor
		@note	シングルトンなので使用禁止
	*/
	ExecCommand(const ExecCommand&);

	/**
		@brief	operator
		@note	シングルトンなので使用禁止
	*/
	ExecCommand& operator=(const ExecCommand&);

public:
	/**
		@brief	インスタンスを取得する(スレッド生成時)
		@retval	インスタンス
		@note	スレッド終了していたら例外をスローする
	*/
	static ExecCommand& ExecCommand::getInstance_onCreateThread();

	/**
		@brief	インスタンスを取得する(スレッド動作中)
		@retval	インスタンス
		@note	スレッド終了していたら例外をスローする
	*/
	static ExecCommand& ExecCommand::getInstance_onRunning();

	/**
		@brief	インスタンスを取得する(スレッド削除時)
		@retval	インスタンス
		@note	スレッド終了していたら例外をスローする
	*/
	static ExecCommand& ExecCommand::getInstance_onDeleteThread();

	/**
		@brief	スレッド開始時
		@note	スレッド開始にしていいのはインクコード管理スレッドのみ
		@note	すでにスレッド生成済みなら例外スロー
	*/
	void onCreateThread_UnwinderManagerThread();

	/**
		@brief	スレッド削除開始時
		@note	スレッド削除開始していいのはインクコード管理スレッドのみ
	*/
	void onDeleteThread_UnwinderManagerThread();

	/**
		@brief	スレッド終了時
		@note	スレッド終了にしていいのはインクコード管理スレッドのみ
	*/
	void onThreadFinished_InkCodeManageThread();

	/**
		@brief	初期処理失敗時(インクコード管理処理)
	*/
	void onInitializeFailed_UnwinderManagerProcess();

	/**
		@brief	実行する
		@param[in]	inApplicationName : 実行するアプリ名
		@param[in]	inCommand : 実行するコマンド
		@retval	実行結果の文字列
	*/
	std::string exec(const std::string& inApplicationName, const std::string& inCommand);

private:
	/**
		@brief	プロセスを起動する(指定の実行ファイルを実行する)
		@param[in]	inCommand : 実行するコマンド
		@retval	起動したプロセスハンドル
	*/
	HANDLE createProcess(const std::string& inCommand);

	/**
		@brief	プロセスの終了待ち
		@param[in]	inProcess : プロセス
	*/
	void waitUntilProcessEnd(HANDLE inProcess);

	/**
		@brief	実行結果をファイルから取得する
		@retval	実行結果の文字列
	*/
	std::string getResultString(HANDLE& outFileHandle);

	/**
		@brief	ファイルを開く
		@retval	ファイルハンドル
	*/
	HANDLE createFile();

	/**
		@brief	ファイルサイズを取得する
		@param[in]	inFileHandle : ファイルハンドル
		@retval	ファイルサイズ
	*/
	DWORD getFileSize(HANDLE inFileHandle);

	/**
		@brief	ファイルを読む
		@param[in]	inFileHandle : ファイルハンドル
		@param[in]	inBufSize : ファイルサイズ
		@retval	文字列
	*/
	std::string readFile(HANDLE inFileHandle, DWORD inBufSize);


	static ExecCommand ms_instance;					//!< インスタンス

	HANDLE m_exitThreadEvent;						//!< スレッド終了イベント

};

};	// namespace unwinder_manager