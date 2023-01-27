/**
 * @file	Stub_RemoteControlIF.h
 * @brief	stub of RemoteControl plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

// ==========================================================================
// Include files


// ==========================================================================

/**
 * @brief	stub of RemoteControl plugin
**/
class CStub_RemoteControlIF
{
	private:
	/**
	 * @brief	コンストラクタ
	**/
	CStub_RemoteControlIF();

public:

	/**
	 * @brief	デストラクタ
	**/
	~CStub_RemoteControlIF();

	/**
	 * @brief	スレーブ機の調整印字結果を削除する.
	 * @param	pPaperName : 削除する用紙名.
	 * @param	enSystem : プリンタの構成(SES,DED)
	**/
	static void Remote_JI_DeleteAdjustPrint(char* pPaperName, ENUM_SYSTEM_CONFIG enSystem);
};