/**
 * @file	Stub_JobManagerIF.h
 * @brief	stub of JobManager plugin
**/


#pragma once
#include "Ini_FlushingPageListData.h"

class Stub_JobManagerIF
{

public:

	/**
	 * @brief	コンストラクタ
	**/
	Stub_JobManagerIF();

	/**
	 * @brief	デストラクタ
	**/
	~Stub_JobManagerIF();


	/**
		@brief  フラッシングページの詳細情報一覧を取得する
		@param[out] outItems : フラッシングページの詳細情報一覧
		@retval true : 成功
		@retval false : 失敗
	*/
	static bool GetDetailedFlushingPageList(std::vector<flushing_page_list_ini::SItem>& outItems);

};