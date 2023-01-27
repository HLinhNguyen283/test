/**
 * @file	FinishFile.h
 * @brief	完了トリガーファイル
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

#include "Common.h"
#include <string>

class CPrintCondition;

/**
 * 完了トリガーファイル
**/
class CFinishFile
{
public:
	/**
	 * コンストラクタ
	**/
	CFinishFile();

	/**
	 * デストラクタ
	**/
	virtual ~CFinishFile();

	/**
	 * ファイル作成する
	 * @param[in]	inDestRootFolderPath : 作成先のルートパス(UNC)
	 * @param[in]	inSuccess : true：成功ファイル作成、false：失敗ファイル作成
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	bool CreateFile(const std::string& inDestRootFolderPath, bool inSuccess);


private:

};


