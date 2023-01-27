/**
 * @file	DataSystemSetting.h
 * @brief	システムのデータ
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

#pragma once

// ==========================================================================
// Include files
#include <memory>
#include <map>
#include <list>

// ==========================================================================

struct ST_PDB_PRINT_MODE;

/**
 * @brief システムのデータ
**/
class CDataSystemSetting
{
public:
	/**
	 * @brief	シングルトンオブジェクト作成
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	static bool CreateInstance();

	/**
	 * @brief	シングルトンオブジェクト破棄
	**/
	static void DeleteInstance();

	/**
	 * @brief	シングルトンオブジェクト取得
	 * @retval	CDataSystemSetting
	**/
	static std::shared_ptr<CDataSystemSetting> GetInstance();

	/**
	 * @brief	constructor
	**/
	CDataSystemSetting();

	/**
	 * @brief	destructor
	**/
	virtual ~CDataSystemSetting();

	/**
	 * @brief	初期化処理を行う.
	**/
	void Initialize();

	/**
	 * @brief	DEDかどうか
	 * @retval	true : DEDである
	 * @retval	false : DEDではない
	**/
	bool IsDED() const;

	/**
	 * @brief	Log.htmlに出力するかどうか
	 * @retval	true : 出力する
	 * @retval	false : 出力しない
	**/
	bool IsDebugLog() const;

	/**
	 * @brief	Log.htmlにXMLエクスポート処理のログを出力するかどうか
	 * @retval	true : 出力する
	 * @retval	false : 出力しない
	**/
	bool IsDebugXmlExportLog() const;

	/**
	 * @brief	EQ制御サーバー上にあるテキスト出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にあるテキスト出力先のルートフォルダ名
	**/
	void GetOutputRootFolder(std::string& outPath) const;

	/**
	 * @brief	EQ制御サーバー上にあるXML出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にある出力先のルートフォルダ名
	**/
	void GetOutputRootFolderXML(std::string& outPath) const;

	/**
	 * @brief	EQ制御サーバー上にあるJI関連情報出力先のルートフォルダ
	 * @param[out]	outPath : EQ制御サーバー上にある出力先のルートフォルダ名
	**/
	void GetOutputRootFolderInspectionData(std::string& outPath) const;

	/**
	 * @brief	EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔
	 * @retval	リトライ間隔(単位：秒)
	**/
	long GetAutoUpdatePrintConditionRetryInterval() const;

	/**
	 * @brief	EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔
	 * @retval	リトライ間隔(単位：秒)
	**/
	long GetAutoUpdatePrintConditionRetryIntervalXML() const;

	/**
	 * @brief	EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能の有効状態
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdatePrintConditionText() const;

	/**
	 * @brief	EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能の有効状態
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdatePrintConditionXML() const;

	/**
		@brief	印刷モードの情報
		@param[out]	outPrintModeDataList : 印刷モードの情報 <印刷モード名, 印刷モード>
	*/
	void onExportXML(std::map<std::string,std::string>& outPrintModeDataList) const;

	/**
	 * @brief	印刷モードメニュー名の取得
	 * @param[in]	inModeDataName : 印刷モード名
	 * @param[in]	outModeMenuName : 印刷モードメニュー名
	 * @retval	true : 印刷モード名有効
	 * @retval	false : 印刷モード名無効
	*/
	bool GetPrintModeMenuName(const char* inModeName, std::string& outModeMenuName) const;

	/**
	 * @brief	XML/TEXT出力先の<IPAddress>-<構成名>フォルダを192.168.0.30-<構成名>で処理するか？
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsEnableAutoUpdateFixIP() const;

	/**
	 * @brief	EQUIOS Center PCのPC名
	 * @param[out]	outPCNameEQ : EQUIOS Center PCのPC名
	**/
	void GetEqCenterPcName(std::string& outPCNameEQ) const;

	/**
	 * @brief	自機のIPアドレス
	 * @param[out]	outIpAddressSelf : 自機のIPアドレス
	**/
	void GetClientPcIPAddrSelf(std::string& outIpAddressSelf) const;

	/**
	 * @brief	相手機のIPアドレス
	 * @param[out]	outIpAddressSelf : 自機のIPアドレス
	**/
	void GetClientPcIPAddrPart(std::string& outIpAddressPart) const;

	/**
	 * @brief	JetInspectorとの接続設定
	 * @retval	接続設定（0：未接続、1：デコードのみ、2：デコード＋絵柄検査(フルスペック)）
	**/
	long GetJetInspectionOption() const;

	/**
	 * @brief	DualSimplex環境設定の有効状態
	 * @retval	true : 有効
	 * @retval	false : 無効
	**/
	bool IsDualSimplex() const;

	/**
	 * @brief	表裏倍率調整用のフラッシングページを出力するかどうか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsMagnificationAdjustment() const;

	/**
		@brief	ノズルチェック機能が可能かどうか
		@param[in]	inFlushingPageName フラッシングページ名
		@retval	true : 可能
		@retval	false : 不可能
	*/
	bool isNozzleCheckEnabled(const std::string& inFlushingPageName) const;

protected:
	bool m_IsDED;									//!< DEDかどうか
	bool m_IsDebugLog;								//!< Log.htmlに出力するかどうか
	bool m_IsDebugXmlExportLog;						//!< Log.htmlにXMLエクスポート処理のログを出力するかどうか
	std::string m_OutputRootFolder;					//!< EQ制御サーバー上にあるテキスト出力先のルートフォルダ
	std::string m_OutputRootFolderXML;				//!< EQ制御サーバー上にあるXML出力先のルートフォルダ
	long m_AutoUpdatePrintConditionRetryInterval;	//!< EQ制御サーバー上へのテキスト形式印刷条件ファイル、自動出力のリトライ間隔(単位：秒)
	long m_AutoUpdatePrintConditionRetryIntervalXML;//!< EQ制御サーバー上へのXML形式印刷条件ファイル、自動出力のリトライ間隔(単位：秒)
	bool m_EnableAutoUpdatePrintConditionText;		//!< EQ制御サーバー上にあるテキスト出力先のルートフォルダの更新機能の有効状態(false:無効, true:有効,デフォルト:0)
	bool m_EnableAutoUpdatePrintConditionXML;		//!< EQ制御サーバー上にあるXML出力先のルートフォルダの更新機能の有効状態(false:無効, true:有効,デフォルト:0)
	std::map<std::string, std::string> m_PrintModeDataList; //!< 印刷モードの一覧 <印刷モード名, EQUIOSでの印刷モード名>
	std::map<std::string, std::string> m_PrintModeMenuList; //!< 印刷モードの一覧 <印刷モード名, 印刷モードMENU表示名>
	std::string m_PCNameEQ;							//!< EQUIOS Center PCのPC名
	std::string m_IpAddressSelf;					//!< 自機のIPアドレス
	std::string m_IpAddressPart;					//!< 相手機のIPアドレス
	long m_JIOption;								//!< JetInspectorとの接続設定
	bool m_IsDualSimplex;							//!< DualSimplex環境設定が有効かどうか
	bool m_EnableAutoUpdateFixIP;					//!< XML/TEXT出力先の<IPAddress>-<構成名>フォルダを192.168.0.30-<構成名>で処理するか？
	std::string m_OutputRootFolderJIData;			//!< EQ制御サーバー上にあるJI関連情報出力先のルートフォルダ
	bool m_isMagnifigationAdustment;				//!< 表裏倍率調整用のフラッシングページを出力するかどうか(true:する、 false:しない)
	std::list<std::string> m_NozzleCheckEnabledFlushingPage;	//!< ノズルチェック機能が可能なフラッシングページ一覧
};


