/**
 *	@file	Stub_PrintProfileMgrIF.h
 *	@brief	Stub of PrintProfileMgr_HD plugin
 *	@author	
 *	@date	2022/5/16 kmatsuoka 新規作成
*/

#pragma once

#include "PrintProfile_Callbacks.h"


/**
	@brief	Stub of PrintProfileMgr_HD plugin
*/
class Stub_PrintProfileMgrIF
{
private:
	/**
		@brief	constructor
	*/
	Stub_PrintProfileMgrIF();

public:
	/**
		@brief	destructor
	*/
	~Stub_PrintProfileMgrIF();

	/**
	 * @brief		指定印刷モードの管理を追加する
	 * @param inType [IN] : 段差･シェーディング･リニアライズの種別
	 * @param inMedia [IN] : 用紙種類・段差設定名
	 * @return true : 処理成功
	 * @return false : 処理失敗（既に管理対象になっている、引数が不正）
	**/
	static bool AddPrintMode(EPrintProfile_Kind inType, const char* inMedia);
};

