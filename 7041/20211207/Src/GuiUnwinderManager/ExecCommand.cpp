/**
	@file	ExecCommand.cpp
	@brief	外部コマンド実行
	@author	cec k.miyachi
	@date	2021/11/19 cec k.miyachi 新規作成
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "ExecCommand.h"
#include <string>
#include <iostream>
#include <sstream>

// ==========================================================================

namespace unwinder_manager
{

ExecCommand unwinder_manager::ExecCommand::ms_instance;				//<! インスタンス

// ==========================================================================

/**
	@brief	ハンドルクラス
*/
class Handle
{
public:
	/**
		@brief	constructor
		@param[in]	inHandle : ハンドル
	*/
	Handle(HANDLE inHandle) :
		m_handle(inHandle)
	{
	}

	/**
		@brief	destructor
	*/
	~Handle()
	{
		// インスタンス消滅時に自動クローズ
		if (m_handle != nullptr)
		{
			CloseHandle(m_handle);
			m_handle = nullptr;
		}
	}

	/**
		@brief	ハンドルを取得する
		@retval	ハンドル
	*/
	HANDLE get()
	{
		return m_handle;
	}

private:
	HANDLE m_handle;		//!< ハンドル

};

// ==========================================================================

/**
	@brief	実行結果取得クラス
*/
class Result
{
public:
	/**
		@brief	constructor
	*/
	Result()
	{
		// 実行結果ファイル名を設定する(UnwinderManagerResult_<日時(msecまで)>.txt)
		SYSTEMTIME time = {0};
		GetLocalTime(&time);
		std::stringstream resultTimeString;
		resultTimeString << time.wYear
						<< time.wMonth
						<< time.wDay
						<< "_"
						<< time.wHour
						<< time.wMinute
						<< time.wSecond
						<< "_"
						<< time.wMilliseconds;
		m_resultFileName = "UnwinderManagerResult_" + resultTimeString.str() + ".txt";

		// 実行結果のファイルパスを設定する
		const std::string moduleDirectory = GetModuleDirectory();
		m_resultFilePath = moduleDirectory + "TEMP\\" + m_resultFileName;
	}


	/**
		@brief	destructor
	*/
	~Result()
	{
		// 実行結果のファイルを消す
		deleteFile();
	}


	std::string GetModuleDirectory()
	{
		char szPath[_MAX_PATH];
		GetModuleFileName(NULL, szPath, sizeof(szPath));
		std::string s(szPath);
		int pos = s.rfind("\\");
		std::string r = s.substr(0, pos+1);

		return r;
	}


	/**
		@brief	実行結果を取得する
		@retval	実行結果
	*/
	std::string get()
	{
		try
		{
			// 実行結果のファイルを開く
			Handle fileHandle(createFile());

			// 実行結果のファイルサイズを取得する
			DWORD bufSize = getFileSize(fileHandle.get());

			// 実行結果のファイルを読む
			return  readFile(fileHandle.get(), bufSize);
		}
		catch (const std::runtime_error&)
		{
			throw;
		}
	}


	/**
		@brief	実行結果のファイルを消す
	*/
	void deleteFile()
	{
		DeleteFile(m_resultFilePath.c_str());
	}


	/**
		@brief	実行結果ファイル名を取得する
		@retval	実行結果ファイル名
	*/
	const std::string& getResultFileName()
	{
		return m_resultFileName;
	}

private:
	/**
		@brief	実行結果のファイルを開く
		@retval	ファイルハンドル
	*/
	HANDLE createFile()
	{
		HANDLE fileHandle = CreateFile(
								m_resultFilePath.c_str(),
								GENERIC_READ,
								0,
								nullptr,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								nullptr);

		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			DWORD lastError = GetLastError();
			std::stringstream error;
			error <<  "Cannot open exec result. " << m_resultFilePath << " LastError:" << lastError;
			throw std::runtime_error(error.str());
		}
		return fileHandle;
	}


	/**
		@brief	実行結果のファイルサイズを取得する
		@param[in]	inFileHandle : ファイルハンドル
		@retval	実行結果のファイルサイズ
	*/
	DWORD getFileSize(HANDLE inFileHandle)
	{
		DWORD bufSize = GetFileSize(inFileHandle, nullptr);
		if (0 == bufSize)
		{
			std::stringstream error;
			error <<  "File size error. " << m_resultFilePath;
			throw std::runtime_error(error.str());
		}
		return bufSize;
	}


	/**
		@brief	実行結果のファイルを読む
		@param[in]	inFileHandle : ファイルハンドル
		@param[in]	inBufSize : 実行結果のファイルサイズ
		@retval	実行結果
	*/
	std::string readFile(HANDLE inFileHandle, DWORD inBufSize)
	{
		char fileBuf[10240] = {0};
		if (ReadFile(inFileHandle, fileBuf, inBufSize, &inBufSize, nullptr) == FALSE)
		{
			//読み込みエラー。
			DWORD lastError = GetLastError();
			std::stringstream error;
			error <<  "Cannot read exec result. " << m_resultFilePath << " LastError:" << lastError;
			throw std::runtime_error(error.str());
		}

		return fileBuf;
	}


	std::string m_resultFilePath;					//<! 実行結果のファイルパス
	std::string m_resultFileName;					//<! 実行結果のファイル名
};

// ==========================================================================

/**
	@brief	外部コマンド実行クラス
*/
class Execute
{
public:
	/**
		@brief	constructor
		@param[in]	inExitThreadEvent : スレッド終了イベント
	*/
	Execute(HANDLE& inExitThreadEvent) :
		m_exitThreadEvent(inExitThreadEvent)
	{
	}


	/**
		@brief	destructor
	*/
	~Execute()
	{
	}

	/**
		@brief	外部コマンドを実行する
		@param[in]	inApplicationName : 実行するアプリ名
		@param[in]	inCommand : 実行するコマンド
		@retval	実行結果
	*/
	std::string exec(const std::string& inApplicationName, const std::string& inCommand)
	{
		try
		{
			Result result;
			const std::string resultFileName = result.getResultFileName();
			std::string commandLine = inApplicationName + " " + resultFileName + " " + inCommand;

			// プロセスを起動する(指定の実行コマンドを実行する)
			Handle processHandle(createProcess(commandLine));

			// プロセスの終了待ち
			waitUntilProcessEnd(processHandle.get());

			// 実行結果をファイルから取得する
			return result.get();
		}
		catch (const std::runtime_error&)
		{
			throw;
		}
	}

private:
	/**
		@brief	プロセスを起動する(指定の実行コマンドを実行する)
		@param[in]	inCommand : 実行するコマンド
		@retval	プロセスハンドル
	*/
	HANDLE createProcess(const std::string& inCommand)
	{
		STARTUPINFO startupInfo = {0};						// 表示に関する属性
		startupInfo.cb = sizeof(startupInfo);				// 構造体サイズ
		startupInfo.dwFlags = STARTF_USESHOWWINDOW;			// 構造体の有効なメンバを指定
		startupInfo.wShowWindow = SW_HIDE;					// ウィンドウ表示形式(HIDE:表示しない)

		PROCESS_INFORMATION processInformation = {0};		// プロセスとそのプライマリスレッドの情報

		// プロセスの起動
		BOOL result = CreateProcess(
						nullptr,
						const_cast<char*>(inCommand.c_str()),	// 実行するコマンド
						nullptr,
						nullptr,
						FALSE,
						0,
						nullptr,
						nullptr,
						&startupInfo,
						&processInformation);

		if (result == 0)
		{
			throw std::runtime_error("Cannot create process.");
		}

		// スレッドのハンドルはいらないのですぐ閉じる
		CloseHandle(processInformation.hThread);
		processInformation.hThread = nullptr;

		return processInformation.hProcess;
	}


	/**
		@brief	プロセスの終了待ち
		@param[in]	inProcess : プロセスハンドル
	*/
	void waitUntilProcessEnd(HANDLE inProcessHandle)
	{
		//HANDLE waitEvent[2] = {inProcessHandle, m_exitThreadEvent};
		//DWORD result = WaitForMultipleObjects(2, waitEvent, FALSE, INFINITE);
		DWORD result = WaitForSingleObject( inProcessHandle, INFINITE );
		if (result != WAIT_OBJECT_0)
		{
			// プロセスの終了待ち以外がきたら例外発生
			std::stringstream log;
			log << "Failed WaitForMultipleObjects. " << result;
			throw std::exception(log.str().c_str());
		}
	}


	HANDLE& m_exitThreadEvent;			//!< スレッド終了イベント
};

// ==========================================================================

// constructor
ExecCommand::ExecCommand() :
	m_exitThreadEvent(nullptr)
{
}


// destructor
ExecCommand::~ExecCommand()
{
}


// インスタンスを取得する(スレッド生成時)
ExecCommand& ExecCommand::getInstance_onCreateThread()
{
	return ms_instance;
}


// インスタンスを取得する(スレッド動作中)
ExecCommand& ExecCommand::getInstance_onRunning()
{
	return ms_instance;
}


// インスタンスを取得する(スレッド削除時)
ExecCommand& ExecCommand::getInstance_onDeleteThread()
{
	return ms_instance;
}


// スレッド開始時
void ExecCommand::onCreateThread_UnwinderManagerThread()
{
	WriteToLogBuf(LOG_DEBUG, "ExecCommand::onCreateThread_UnwinderManagerThread");

	if (m_exitThreadEvent != nullptr)
	{
		WriteToLogBuf(LOG_DEBUG, "m_exitThreadEvent != nullptr");

		ResetEvent(m_exitThreadEvent);

		// スレッド終了イベント破棄
		CloseHandle(m_exitThreadEvent);
		m_exitThreadEvent = nullptr;
	}

	m_exitThreadEvent = CreateEvent(nullptr, TRUE, FALSE, "ExecCommand_ExitThread");
}


// スレッド削除開始時
void ExecCommand::onDeleteThread_UnwinderManagerThread()
{
	WriteToLogBuf(LOG_DEBUG, "ExecCommand::onDeleteThread_UnwinderManagerThread");

	// スレッド終了イベントをセット
	SetEvent(m_exitThreadEvent);
}


// スレッド終了時
void ExecCommand::onThreadFinished_InkCodeManageThread()
{
	WriteToLogBuf(LOG_DEBUG, "ExecCommand::onThreadFinished_UnwinderManagerThread");

	// 終了処理をする
	if (m_exitThreadEvent != nullptr)
	{
		WriteToLogBuf(LOG_DEBUG, "m_exitThreadEvent != nullptr");

		ResetEvent(m_exitThreadEvent);

		// スレッド終了イベント破棄
		CloseHandle(m_exitThreadEvent);
		m_exitThreadEvent = nullptr;
	}
}


// 初期処理失敗時(アンワインダー管理処理)
void ExecCommand::onInitializeFailed_UnwinderManagerProcess()
{
	WriteToLogBuf(LOG_DEBUG, "ExecCommand::onInitializeFailed_UnwinderManagerProcess");

	// 終了処理をする
	if (m_exitThreadEvent != nullptr)
	{
		WriteToLogBuf(LOG_DEBUG, "m_exitThreadEvent != nullptr");

		ResetEvent(m_exitThreadEvent);

		// スレッド終了イベント破棄
		CloseHandle(m_exitThreadEvent);
		m_exitThreadEvent = nullptr;
	}
}


// 実行する
std::string ExecCommand::exec(const std::string& inApplicationName, const std::string& inCommand)
{
	try
	{
		// 外部コマンドを実行する
		Execute execute(m_exitThreadEvent);
		return execute.exec(inApplicationName, inCommand);
	}
	catch (const std::runtime_error&)
	{
		throw;
	}
}

};	// namespace ink_code_manager