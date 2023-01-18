/**
 * @file  Common.h
 * @brief 共通定義 ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/

#pragma once


#define TEST_TOOL_INI			_T("JD_UW_Test_TOOL.ini")
#define TEST_WORK_INI			_T("JD_UW_Test_WORK.ini")
#define CONTENT_TYPE_JMF		"Content-type: application/vnd.cip4-jmf+xml"

#define CONV_TIME				_T("\\TIME\\")
#define CONV_DEVICEID			_T("\\DEVICEID\\")
#define CONV_SENDID				_T("\\SendID\\")
// Subscription
#define CONV_REPEATTIME			_T("\\REPEATTIME\\")
#define CONV_URL				_T("\\URL\\")
#define CONV_CHANNELMODE		_T("\\CHANNELMODE\\")
// ResourceQuParams
#define CONV_RESOURCEDETAILS	_T("\\RESOURCEDETAILS\\")
#define CONV_RESOURCENAME		_T("\\RESOURCENAME\\")
#define CONV_SCOPE				_T("\\SCOPE\\")
// StopPersChParams
#define CONV_MESSAGETYPE		_T("\\MESSAGETYPE\\")
#define CONV_CHANNELID			_T("\\CHANNELID\\")
#define CONV_STOP_URL			_T("\\STOP_URL\\")
// ResourceCmdParams
#define CONV_UPDATEMETHOD		_T("\\UPDATEMETHOD\\")
// ResouceSet
#define CONV_NAME				_T("\\NAME\\")
// Resource
#define CONV_DESCRIPTIVENAME	_T("\\DESCRIPTIVENAME\\")
#define CONV_EXTERNALID			_T("\\EXTERNALID\\")
// Media
#define CONV_DIMENSION			_T("\\DIMENSION\\")
#define CONV_MEDIATYPE			_T("\\MEDIATYPE\\")
#define CONV_THICKNESS			_T("\\THICKNESS\\")
#define CONV_UWTENSION			_T("\\UWTENSION\\")
#define CONV_MAXRUNSPEED		_T("\\MAXRUNSPEED\\")

#define CONV_STATUS				_T("\\STATUS\\")
#define CONV_ROLLDIAMETER		_T("\\ROLLDIAMETER\\")

/**
* @brief 共通APIクラス
*/
class CCommon
{
public:
	enum
	{	// スレッド間要求
		WM_USER_JMF_SEND = (WM_USER+100),	// JMF送信要求
		WM_USER_JMF_SEND_END,				// JMF送信完了通知
		WM_USER_DSP_SEND,					// 送信データ表示通知
		WM_USER_DSP_RECV,					// 受信データ表示通知
	};
	enum JmfType
	{	// JMF種別
		E_JMF_QueryStatus,					// 状態監視
		E_JMF_QueryResource,				// 用紙情報登録
		E_JMF_StopPersChParams,				// チャネル削除
		E_JMF_CommandResource,				// 印刷条件設定
		E_JMF_CommandWakeup,				// READY状態への遷移要請
		// 
		E_JMF_SignalStatusPaper,			// UWからのSignalStatus（Paper）
		E_JMF_SignalStatus,					// UWからのSignalStatus
	};

	/* ==================================================================================================== */
	/**
		* @brief  モジュールのカレントフォルダを取得する
		* @retval モジュールのカレントフォルダ
	**/
	/* ==================================================================================================== */
	static CString GetCurrentDirectory();

	/* ==================================================================================================== */
	/**
		* @brief  ツールのiniファイル(JD_UW_Test_TOOL.ini)パスを取得
		* @retval ツールのiniファイルパス
	**/
	/* ==================================================================================================== */
	static CString GetToolIniPath();

	/* ==================================================================================================== */
	/**
		* @brief  ツールのワークiniファイル(JD_UW_Test_WORK.ini)パスを取得
		*　　　　 JD_UW_Test_WORK.iniは本ツールが作成するINIファイル
		* @retval ツールのワークiniファイルパス
	**/
	/* ==================================================================================================== */
	static CString GetWorkIniPath();

	/* ==================================================================================================== */
	/**
		* @brief  指定ファイルをメモリに読み込む
		* @param  inFilePath	: [i]ファイルパス
		* @param  outData		: [o]ファイルの内容
		* @retval TRUE=正常、FALSE=異常
	**/
	/* ==================================================================================================== */
	static BOOL ReadFile( const CString& inFilePath, CString& outData );

	/* ==================================================================================================== */
	/**
		* @brief  現在時刻をゾーン形式で戻す
		* @retval ゾーン形式の現在時刻
	**/
	/* ==================================================================================================== */
	static CString CreateTimeStamp();
};