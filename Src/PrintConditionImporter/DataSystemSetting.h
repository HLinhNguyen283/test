/**
 * @file	DataSystemSetting.h
 * @brief	システムのデータ
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

#include "Common.h"
#include <string>
#include <map>
#include <list>

struct ST_PDB_PRINT_MODE;

/**
 * @brief システムのデータ
**/
class CDataSystemSetting
{
public:
	/**
	 * @brief	constructor
	**/
	CDataSystemSetting() throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CDataSystemSetting() throw();

	/**
	 * @brief	初期化処理を行う.
	**/
	void Initialize() throw();

	/**
	 * @brief	DEDかどうか
	 * @retval	true : DEDである
	 * @retval	false : DEDではない
	**/
	bool IsDED() const throw();

	/**
	 * @brief	JI系の情報の登録が必要かどうかを取得する
	 * @retval	true : 必要
	 * @retval	false : 不要
	**/
	bool IsRegisterJetInspection() const throw();

	/**
	 * @brief	インポート時のベースとなる印刷条件名を取得する
	 * @retval	インポート時のベースとなる印刷条件名
	**/
	const std::string& GetBasePrintConditionName() const throw();

	/**
	 * @brief	非検査エリア(紙端)のデフォルト値を取得する
	 * @param[in]	isPunched : 用紙情報上のパンチ有無（true:パンチあり）
	 * @retval	非検査エリア(紙端)のデフォルト値（単位 mm）
	 * @note	パンチ有無情報で返却値を切り替える
	**/
	double GetNonInspectionAreaDefault(
		bool isPunched) const throw();

	/**
	 * @brief	指定の印刷モード名に対応する印刷モード情報を取得する
	 * @param[in]	inPrintModeName : 印刷モード名（PrinterParameter.ini での EQUIOS_NAME の値）
	 * @param[out]	outPrintMode : 印刷モード情報
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	bool GetPrintMode(
		const std::string&	inPrintModeName,
		ST_PDB_PRINT_MODE&	outPrintMode) const throw();

	/**
	 * @brief	指定の印刷モードで指定の印刷速度が有効値であるかチェックする
	 * @param[in]	inPrintModeName : 印刷モード名
	 * @param[in]	inSpeedValue : 印刷速度[単位:0.01 m/min]
	 * @retval	true : 有効値である
	 * @retval	false : 有効値ではない
	**/
	bool CheckEffectiveSpeed(
		const std::string&	inPrintModeName,
		long				inSpeedValue) const throw();

	/**
	 * @brief	指定印刷モードの最高速度設定を取得
	 * @param[in]	inPrintModeName : 印刷モード名
	 * @retval	最高印刷速度[単位:0.01 m/min]
	**/
	long GetMaxSpeed(
		const std::string&	inPrintModeName) const throw();

	/**
	 * @brief	デバッグ用のファイルを出力するか
	 * @retval	true : する
	 * @retval	false : しない
	**/
	bool IsDebugLog() const throw();

	/**
	 * @brief	ファイルを新規作成するときにファイル名につける数値を取得する
	 * @retval	ファイルを新規作成するときにファイル名につける数値
	 * @note	同時刻(秒まで一致)に同名ファイルが作成されるのを防ぐ
	**/
	unsigned long GetFileCounter() throw();

	/**
	 * @brief	アンカーポイント演算定数を取得する
	 * @retval	アンカーポイント演算定数
	**/
	double GetDotgainRatio() throw();

	/**
	 * @brief	インポートのタイムアウト時間を取得する
	 * @retval	インポートのタイムアウト時間[単位:msec]
	**/
	DWORD GetTimeoutImport();

	/**
	 * @brief	フラッシングページが登録済みか（解像度、JIチェックあり）
	 * @param[in] inFlushingPageName	フラッシングページ名
	 * @param[in] inResoX				主走査解像度
	 * @param[in] inResoY				副走査解像度
	 * @retval	true : 登録済み
	 * @retval	false : 未登録
	**/
	bool isSystemFlushingPage(
		const std::string& inFlushingPageName, 
		const long inResoX, 
		const long inResoY) const throw();

	/**
	 * @brief	JetInspectorとの接続設定
	 * @retval	接続設定（0：未接続、1：デコードのみ、2：デコード＋絵柄検査(フルスペック)）
	**/
	long GetJetInspectionOption() const;

	/**
	 * @brief	プリンターの印刷モード取得
	 * @param inPrintModeName プリンターの印刷モード名
	 * @return	プリンターの印刷モード文字列
	**/
	std::string getPrintModeName(const std::string&	inPrintModeName);

	/**
	 * @brief	自動変倍機能が使用可能か否かを返す
	 * @retval	true : 使用可能
	 * @retval	false : 使用不可
	**/
	bool isAvailableAutoMagnification();

	/**
		@brief	ノズルチェック機能が可能かどうか
		@param[in]	inFlushingPageName フラッシングページ名
		@retval	true : 可能
		@retval	false : 不可能
	*/
	bool isNozzleCheckEnabled(const std::string& inFlushingPageName) const;

protected:
	bool m_IsDED;								//!< DEDかどうか

	bool m_IsRegisterJI;						//!< JI系の情報の登録が必要かどうか
	std::string m_BasePrintConditionName;		//!< インポート時のベースとなる印刷条件名
	bool m_IsDebugLog;							//!< デバッグ用のファイルを出力するか
	double m_NonInspectionAreaDefault;			//!< 非検査エリア(紙端)のデフォルト値（パンチなし用紙用）
	double m_NonInspectionAreaDefault_Punch;	//!< 非検査エリア(紙端)のデフォルト値（パンチあり用紙用）
	bool m_IsHaveJetInspection;					//!< JetInspectionがあるかどうか

	class CPrintModeData;
	std::map<std::string, CPrintModeData> m_PrintModeList;	//!< 印刷モードに紐づく情報一覧(印刷モード文字列（PrinterParameter.ini での EQUIOS_NAME の値）をキーとする)
	unsigned long m_FileNameCounter;					//!< ファイル名につけるカウンター (0-100000の繰り返し)
	ST_CRITICAL_SECTION_INFO m_CsFileNameCounter;		//!< ファイル名につけるカウンターの排他制御用
	double m_dotgainRatio;								//!< アンカーポイント演算定数
	DWORD m_importTimeout;								//!< インポートのタイムアウト時間[単位:msec]
	long m_JIOption;									//!< JetInspectorとの接続設定
	bool m_MagnificationAdjustment;						//!< 表裏倍率調整用のフラッシングページを出力する(true:ON/false:OFF)
	std::list<std::string> m_NozzleCheckEnabledFlushingPage;	//!< ノズルチェック機能が可能なフラッシングページ一覧
};


