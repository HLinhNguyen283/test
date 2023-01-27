/**
 * @file	Stub_JetInspectorIF.h
 * @brief	stub of JetInspector plugin
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

// ==========================================================================
// Include files


// ==========================================================================

/**
 * @brief	stub of JetInspector plugin
**/
class CStub_JetInspectorIF
{
	private:
	/**
	 * @brief	コンストラクタ
	**/
	CStub_JetInspectorIF();

public:

	/**
	 * @brief	デストラクタ
	**/
	~CStub_JetInspectorIF();

	/**
	 * @brief	JIの状態(Online/Offline)を取得する
	 * @retval	TRUE : Online
	 * @retval	FALSE : Offline
	**/
	static BOOL JI_IsJIOnline();
	
	/**
	 * @brief	JIプラグは、JIに調整印字結果の削除を通知する
	 * @param	pPaperName : 削除する用紙名.
	 * @param	enSystem : プリンタの構成(SES,DED).
	**/
	static void JI_DeleteAdjustPrint(char* pPaperName, ENUM_SYSTEM_CONFIG enSystem);
};
