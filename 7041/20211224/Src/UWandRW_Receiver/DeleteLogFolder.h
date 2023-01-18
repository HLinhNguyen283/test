/**
	* @file		DeleteLogFolder.h
	* @brief	古いログフォルダ削除 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

/**
* @brief 古いログフォルダ削除クラス
*/
class CWriteDebugLog;
class CDeleteLogFolder
{
	CWriteDebugLog* m_pWriteDebugLog;
public:
	/**
		* @brief  CDeleteLogFolder コンストラクション
	**/
	CDeleteLogFolder(CWriteDebugLog* inWriteDebugLog);

	/**
		* @brief  CDeleteLogFolder デストラクション
	**/
	virtual ~CDeleteLogFolder();

	/**
		* @brief  保存期間を経過したログフォルダを削除する
		* @param  inFolderPath	: [i]トップフォルダパス
		* @param  inPeriodDate	: [i]保存期間
		* @retval TRUE=正常、FALSE=異常
	**/
	BOOL Doit( const CString& inFolderPath, UINT inPeriodDate );

	/**
		* @brief  指定フォルダ下のフォルダ一覧を取得する
		* @param  inFolderPath   : [i]指定フォルダ
		* @param  inSearchFolder : [i]指定フォルダパターン（例："*"）
		* @retval フォルダ名称一覧
	**/
	std::vector<CString> GetFolderList( const CString& inFolderPath, const CString& inSearchFolder );

	/**
		* @brief  フォルダリストのフォルダを削除する
		* @param  inFolderPath		: [i]トップフォルダパス
		* @param  inDeleteFolderList: [i]削除フォルダ名のリスト
	**/
	void DeleteFoldertoList( const CString& inFolderPath, const std::vector<CString>& inDeleteFolderList );

	/**
		* @brief  指定フォルダを削除する
		* @param  inFolderPath : [i]削除フォルダ
		* @retval TRUE=正常、FALSE=異常
	**/
	BOOL DeleteFolderOnes( const std::string& inFolderPath );

};