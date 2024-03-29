/**
 * @file  JmfAnalys.h
 * @brief JMFを解析する ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/
#pragma once

#import <msxml3.dll> raw_interfaces_only
using namespace MSXML2;

/**
* @brief JMF解析クラス
*/
class CJmfAnalys
{
public:
	/* ==================================================================================================== */
	/**
		* @brief CJmfAnalys コンストラクション
		* @param  JmfData : [i]XMLデータ
	**/
	/* ==================================================================================================== */
	CJmfAnalys( const CString& JmfData );

	/* ==================================================================================================== */
	/**
		* @brief CJmfAnalys デストラクション
	**/
	/* ==================================================================================================== */
	virtual ~CJmfAnalys();

	/* ==================================================================================================== */
	/**
		* @brief XMLデータのパース
		* @retval ERROR_SUCCESS(正常[形式正常])、その他(異常)
	**/
	/* ==================================================================================================== */
	LONG Perth();

	/* ==================================================================================================== */
	/**
		* @brief XMLデータのパース
		* @param  OutPath : [i]出力パス
		* @retval ERROR_SUCCESS(正常)、その他(異常)
	**/
	/* ==================================================================================================== */
	LONG Save(const CString& OutPath);

	int GetRecvJmfType();

private:
	CString m_JmfData;
	MSXML2::IXMLDOMDocumentPtr m_pDocument;
	MSXML2::IXMLDOMElementPtr m_pRootElement;
};