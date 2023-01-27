/**
 * @file	OparationAddProfileTypeList.h
 * @brief	AddPaperTypeList.txtおよびAddAlignmentList.txtの操作(作成・削除等)クラス
**/

#pragma once
//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include "Common.h"
#include <list>

/**
	@brief	ImportProgress.txtの操作(作成・削除等)を行う
*/

class COperationAddProfileTypeList
{
public:

	/**
	 * @brief	constructor
	 * @param[in]	inOutputURL : 出力先のURLパス(UNC)
	 * @param[in]	inOutputFname : 出力先のファイル名
	**/
	COperationAddProfileTypeList(const std::string& inOutputURL, const std::string inOutputFname) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~COperationAddProfileTypeList() throw();

	/**
	 * @brief	成功時の取り込み結果を出力する
	**/
	void WriteSuccess(const std::string& inProfileName) throw();

private:

	/**
	 * @brief	ファイルに書く
	 * @param[in]	inString : 文字列
	 * @note	エラー発生時は例外を投げる
	**/
	void writeFile(const std::string& inString) throw(std::runtime_error);

	/**
	 * @brief	エラーコードの文字列を取得する
	 * @param[in]	inErrID : エラーコード
	 * @retval	エラーコードの文字列
	**/
	const std::string getLastErrorString(DWORD inErrID) throw();


	std::string m_OutputFilePath;	//!< 出力ファイルのフルパス
	std::list<std::string> m_FaileList;	//!< 結果一覧(名前+取り込み結果)

};

class COperationAddPaperTypeList : public COperationAddProfileTypeList
{
public:
	COperationAddPaperTypeList(const std::string& inOutputURL);
	virtual ~COperationAddPaperTypeList() throw();
};


class COperationAddAllignmentList : public COperationAddProfileTypeList
{
public:
	COperationAddAllignmentList(const std::string& inOutputURL);
	virtual ~COperationAddAllignmentList() throw();
};

