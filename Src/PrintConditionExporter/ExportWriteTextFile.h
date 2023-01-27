/*! \file WriteTextFile.h
 *  \brief WriteTextFile.hの処理
 *  \author pt.ju-tanaka
 *  \date 2019/03/11 pt.ju-tanaka 新規作成
*/

#pragma once

#include "Common.h"
#include "PaperDB_MigrationDef.h"


/**
 * @brief エクスポート先にテキストファイルを出力する
**/
class ExportWriteTextFile
{
public:
	/**
	 * コンストラクタ
	**/
	ExportWriteTextFile();

	/**
	 * デストラクタ
	**/
	virtual ~ExportWriteTextFile();

	/**
	 * テキストファイル出力イベント
	 * @param[in]	inExportFilePath : エクスポート先のファイルパス
	 * @param[in]	inMigrationData : ファイル出力対象の業務移行用構造体
	 * @retval	true：出力成功
	 * @retval	false：失敗
	**/
	bool onWriteTextFile(const std::string& inExportFilePath, const ST_PDB_MIGRATION_DATA& inMigrationData);

private:
	class API;

	/**
	 * GetLastErrorの文字列を取得する。
	 * @retval	GetLastErrorの文字列
	**/
	std::string getLastError();

	/**
	 * 数値を「"%0.2f"」書式の文字列で返す
	 * @param[in]	inVal :元数値。
	 * @retval	変換後文字列
	**/
	std::string changeTo02f(double inVal);

	/**
	 * 数値を「"%0.2f"」書式の文字列で返す
	 * @param[in]	inVal :元数値。
	 * @retval	変換後文字列
	**/
	std::string changeTo02f(double x, double y);

	/**
	 * 数値を「"%0.2f"」書式の文字列で返す
	 * @param[in]	inVal :元数値。
	 * @retval	変換後文字列
	**/
	std::string changeTo02f(double sx, double sy, double ex, double ey);

	/**
	 * 数値->文字列変換(string)して返す
	 * @param[in]	inVal :変換元数値
	 * @param[in]	inPrecision：変換後の少数桁数
	 * @retval	変換後文字列
	**/
	std::string convertNumToStringPrecision(double inValue, UINT inPrecision);

	/**
	 * 数値を「"%d"」書式の文字列で返す
	 * @param[in]	inVal :元数値。
	 * @retval	変換後文字列
	**/
	std::string changeToD(long inVal);

	/**
	 * ページ送りサイズ文字列取得
	 * @param[in]	inPaperSizeH :ページ送りサイズ情報。
	 * @retval	変換後文字列
	**/
	std::string getPaperFeed(const ST_PDB_PAPER_SIZE_H& inPaperSizeH);

	/**
	 * ページ送りサイズの単位文字列取得
	 * @param[in]	inUnitMode :単位。
	 * @retval	変換後文字列
	**/
	std::string getPaperFeedUnit(const eUNIT_MODE inUnitMode);

	/**
	 * パンチ文字列取得
	 * @param[in]	inPaperOption :印刷オプション。
	 * @retval	変換後文字列
	**/
	std::string getPaperPunch(const ST_PDB_PAPER_OPTION& inPaperOption);

	/**
	 * 「true」「false」文字列取得
	 * @param[in]	inVal :元値。
	 * @retval	変換後文字列
	**/
	std::string getTrueOrFalse(BOOL inVal);

	/**
	 * 事前印刷マーク文字列取得
	 * @param[in]	inPaperOption :印刷オプション。
	 * @retval	変換後文字列
	**/
	std::string getParamMark(const ST_PDB_PAPER_OPTION& inPaperOption);

	/**
	 * トーンカーブ（名称）文字列取得
	 * @param[in]	inDotgainCurve :トーンカーブ情報。
	 * @param[out]	outVal :変換後文字列。
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	bool getToneCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve, std::string& outVal);

	/**
	 * フラッシングパターン文字列取得
	 * @param[in]	inFlushingPatternKind :フラッシングパターン種類。
	 * @retval	変換後文字列
	**/
	std::string getFlushingPatternKind(DEF_FLUSHING_PATTERN inFlushingPatternKind);

	/**
	 * フラッシングパターンライン時ずらし量文字列取得
	 * @param[in]	inFlushingPattern :フラッシングパターン。
	 * @param[out]	outVal :変換後文字列。
	 * @retval	true：成功
	 * @retval	false：失敗
	**/
	bool getFlushingPatternOffset(const ST_JOB_FLUSHING_PATTERN& inFlushingPattern, std::string& outVal);

	/**
	 * サイド検査マーク種類文字列取得
	 * @param[in]	inSideCheckMarkKind :サイド検査マーク情報。
	 * @retval	変換後文字列
	**/
	std::string getSideCheckMarkKind(const ST_JOB_VERIFY_MARK& inSideCheckMarkKind);

	/**
	 * 後処理マーク種類文字列取得
	 * @param[in]	inSide :面情報。
	 * @retval	変換後文字列
	**/
	std::string getPostMarkKind(DEF_SELECT_SIDE inSide);

	/**
	 * ラインフラッシング、オプション文字列取得
	 * @param[in]	inOption :ラインフラッシング、オプション。
	 * @retval	変換後文字列
	**/
	std::string getFlushingPatternOption(const ST_JOB_FLUSHING_PATTERN_ADD& inOption);

	/**
	 * 小数第N位までに計算（小数第N+1位を四捨五入）して返す
	 * @param[in]	inSrc :元数値。
	 * @param[in]	inLen :小数第N位まで計算して返してほしいNの値。
	 * @retval	変換後文字列
	**/
	double roundD(double inSrc, int inLen);

	/**
	 * フラッシングパターンがLine*かどうか取得
	 * @param[in]	inFlushingPatternKind :フラッシングパターン種類
	 * @retval	true：フラッシングパターンがLine*
	 * @retval	false：フラッシングパターンがLine*以外
	**/
	bool isFlushingPatternLine(DEF_FLUSHING_PATTERN inFlushingPatternKind);

	/**
		@brief	印刷停止設定文字列取得
		@param[in]	inPrintStopSettingKind :印刷停止設定の列挙値
		@retval	変換後文字列
	*/
	std::string getPrintStopSettingKind(const EPrintStopSetting& inPrintStopSettingKind);
};