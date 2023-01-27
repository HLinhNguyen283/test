/**
 * @file	Stub_CenterTransIF.h
 * @brief	stub of center trans plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include "PaperDBDef_OP.h"
#include <string>
#include <vector>

// ==========================================================================

/**
 * @brief	stub of center trans plugin
**/
class CStub_CenterTransIF
{
	private:
	/**
	 * @brief	コンストラクタ
	**/
	CStub_CenterTransIF();

public:

	/**
	 * @brief	デストラクタ
	**/
	~CStub_CenterTransIF();

	/**
	 * @brief	コマンド受信通知 callback 登録定義
	 * @param[in]	inProc : 通知を受ける callback
	 * @param[in]	inUserData : ユーザー定義データ
	 * @retval	登録解除時に必要になる ID
	**/
	static size_t Register_ReceiveProc(receiveCallbackProcPtr inProc, void* inUserData);

	/**
	 * @brief	コマンド受信通知 callback 登録解除定義
	 * @param[in]	inID : 登録時に返された ID
	**/
	static void Unregister_ReceiveProc(size_t inID);
};

