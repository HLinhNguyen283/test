/**
 * @file	DotgainFileCreator.cpp
 * @brief	ドットゲインファイルを作成する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "DotgainFileCreator.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <functional>
#include "CommonCBRProc.h"
#include "Shlwapi.h"
#include "DataSystemSetting.h"
// ==========================================================================

#define DEF_DOTGAIN_FACTOR_NUM		4				//!< ３次式のファクター数.

CDotgainFileCreator::CDotgainFileCreator(CDataSystemSetting& inDataSystemSetting)
{
	auto getCommonName = [&inDataSystemSetting]()->std::string
	{
		SYSTEMTIME systemTime = {0};
		GetLocalTime(&systemTime);

		std::stringstream commonName;
		commonName << std::setfill('0') << std::setw(4) << systemTime.wYear;
		commonName << std::setfill('0') << std::setw(2) << systemTime.wMonth;
		commonName << std::setfill('0') << std::setw(2) << systemTime.wDay;
		commonName << std::setfill('0') << std::setw(2) << systemTime.wHour;
		commonName << std::setfill('0') << std::setw(2) << systemTime.wMinute;
		commonName << std::setfill('0') << std::setw(2) << systemTime.wSecond;
		commonName << inDataSystemSetting.GetFileCounter();

		return commonName.str();
	};

	m_CommonName = getCommonName();

	m_dotgainRatio = inDataSystemSetting.GetDotgainRatio();

	getCommonInfoAsImageData(m_srcData);
	for (int cnt = 0; cnt < DOTGAIN_CURVE_PT_CNT; cnt++) {
		m_srcBuff[cnt] = cnt;
	}
	m_srcData.data = m_srcBuff;

	getCommonInfoAsImageData(m_workData);
	std::fill_n(m_workBuff, DOTGAIN_CURVE_PT_CNT, 0);
	m_workData.data = m_workBuff;
}

CDotgainFileCreator::~CDotgainFileCreator()
{
}

bool CDotgainFileCreator::Write(S_DOTGAIN_INFO& inData, std::string& outDotgainFilePath)
{
	try {
		//ファイルパスをクリアする
		m_DotgainFilePath.clear();

		// LUTテーブルを作成する
		double dotgainFactor[DEF_DOTGAIN_FACTOR_NUM] = {0};
		if (convertLUT(inData.AnChorPoint, dotgainFactor) == false) {
			// アンカーポイント種別がカーブ描画のものがひとつもなければドットゲインファイルを作成せずに正常終了
			return true;
		}
	
		// ドットゲインファイルを作成する
		createDotgainFile(inData.ColorType, dotgainFactor);

		// 作成済みファイルのパスを設定する
		outDotgainFilePath = m_DotgainFilePath;
		return true;
	}
	catch (int) {
		return false;
	}
}

const std::string CDotgainFileCreator::GetCommonName() const
{
	return m_CommonName;
}

void CDotgainFileCreator::getCommonInfoAsImageData(ImageData& outImageData)
{
	std::fill_n(&outImageData, 1, ImageData());
	outImageData.width = DOTGAIN_CURVE_PT_CNT;
	outImageData.height = 1;
	outImageData.bps = 16;
	outImageData.spp = 1;
}

bool CDotgainFileCreator::convertLUT(ST_PDB_ANCHOR_POINT* inAnchorPoint, double* outDotgainFactor)
{
	// LUT出力値、LUT入力値を取得する
	int levels[ANCHOR_PT_CNT] = {0};
	int values[ANCHOR_PT_CNT] = {0};
	short valueLeft = 0;
	int anchorPointCnt = getLevelsAndValues(inAnchorPoint, levels, values, valueLeft);

	// アンカーポイント種別がカーブ描画のものがひとつもなければ終了
	if (anchorPointCnt == 0) {
		return false;
	}

	// 入力画像に対しLUT入力値とLUT出力値で指定したLUT変換を施す
	imageLUT(levels, values, valueLeft, anchorPointCnt, outDotgainFactor);

	// ドットゲイン値を補正する
	// (ZZではやっているがNXのToneSettingGUIではやっていないためコメントアウト)
	//fixDotgainFactor(inAnchorPoint[ANCHOR_PT_CNT - 1].POSITION.y, outDotgainFactor[DEF_DOTGAIN_FACTOR_NUM - 1]);

	return true;
}


int CDotgainFileCreator::getLevelsAndValues(ST_PDB_ANCHOR_POINT* inAnchorPoint, int* outLevels, int* outValues, short& outValueLeft) const
{
	#define DEF_ANCHOR_PT_UNIT	1000

	int anchorPointCnt = 0;

	for (int cnt = 0; cnt < ANCHOR_PT_CNT; cnt++) {
		if (inAnchorPoint[cnt].KIND == TRUE) {
			outLevels[anchorPointCnt] = inAnchorPoint[cnt].POSITION.x / DEF_ANCHOR_PT_UNIT;
			outValues[anchorPointCnt] = (long)((double)inAnchorPoint[cnt].POSITION.y * m_dotgainRatio / (double)DEF_ANCHOR_PT_UNIT);
			++anchorPointCnt;
		}
	}

	outValueLeft = static_cast<short>(inAnchorPoint[ANCHOR_PT_CNT -1].POSITION.y / DEF_ANCHOR_PT_UNIT);

	return anchorPointCnt;
}

void CDotgainFileCreator::imageLUT(int* inLevels, int* inValues, short inValueLeft, int inAnchorPointCnt, double* outDotgainFactor)
{
	auto imageLUTerror = [](int imageFuncRet)->void
	{
		std::string status = "";

		const char* pLog = IMAGELIB_ImageGetStatusString(imageFuncRet);
		if (pLog != nullptr) {
			status = pLog;
		}

		std::stringstream log;
		log << "CDotgainFileCreator::imageLUT : Error IMAGELIB_ImageLUT [" << status << "]";
		WriteToLogBuf(LOG_DEBUG, log.str());
	};


	// 出力画像用の領域を準備
	ImageData dstData;
	getCommonInfoAsImageData(dstData);
	short dstBuff[DOTGAIN_CURVE_PT_CNT];
	std::fill_n(dstBuff, DOTGAIN_CURVE_PT_CNT, 0);
	dstData.data = dstBuff;
	dstData.option = (long)outDotgainFactor;

	// ワーク領域をクリア
	std::fill_n(m_workBuff, DOTGAIN_CURVE_PT_CNT, 0);

	// 入力画像に対しLUTの出力値とLUTの入力値で指定したLUT変換を施す
	int imageFuncRet = IMAGELIB_ImageLUT(&m_srcData, &dstData, inValues, inLevels, inAnchorPointCnt, 4, &m_workData);
	if (imageFuncRet != 0) {
		imageLUTerror(imageFuncRet);
		throw 1;
	}

	// 100％の位置を動かした場合、最後の一点がなぜか最大値になってしまうため左端のLUT入力値とする(IPPの内部でそうなっている)
	// ※ここでは係数しか使わないため補正後の値は使用しないが、表示時はズレるためこの処理を行うこと
	dstBuff[DOTGAIN_CURVE_PT_CNT - 1] = inValueLeft;
}

void CDotgainFileCreator::fixDotgainFactor(long inFirstAnchorPoint, double& outDotgainFactor)
{
	// (0, 0) が 編集されておらず、且つ D が 0 を超えている場合は、D を 0 とする
	if (inFirstAnchorPoint == 0) {
		if (outDotgainFactor > 0.0) {
			outDotgainFactor = 0.0;
		}
	}
}

void CDotgainFileCreator::createDotgainFile(E_COLORTYPE inColorType, double* inDotgainFactor)
{
	FILE* fp = nullptr;
	auto openFile = [&](const std::string& inFileName)->void
	{
		fp = IO_fopen(inFileName.c_str(), "wb");
		if (fp == nullptr) {
			std::stringstream log;
			log << "CDotgainFileCreator::createDotgainFile::openFile : Error IO_fopen [" << inFileName << "]";
			WriteToLogBuf(LOG_DEBUG, log.str());
			GetLastError_WriteLog();
			throw 1;
		}
	};

	auto closeFile = [&]()->void
	{
		IO_fclose(fp);
		fp = NULL;
	};


	auto writeValueToFile = [&](double inDotgainFactor, const char* inFactorKey, const char* inShiftKey)->void
	{
		long factor = 0;
		long shift = 0;
		GetWriteValue(inDotgainFactor, factor, shift);

		IO_fprintf(fp, "%s=%ld\n", inFactorKey, factor);
		IO_fprintf(fp, "%s=%ld\n", inShiftKey, shift);
	};

	auto writeLog = [&](const std::string& inFileName)->void
	{
		std::stringstream log;
		log << "CDotgainFileCreator::createDotgainFile : create [" << inFileName << "]";
		WriteToLogBuf(LOG_DEBUG, log.str());
	};


	// ファイル名(フルパス)を取得
	std::string fileName = GetFilePath(inColorType);

	// ファイルを開く
	openFile(fileName);
	
	// パラメータを書く
	writeValueToFile(inDotgainFactor[0], _T("A"), _T("A_shift"));
	writeValueToFile(inDotgainFactor[1], _T("B"), _T("B_shift"));
	writeValueToFile(inDotgainFactor[2], _T("C"), _T("C_shift"));
	writeValueToFile(inDotgainFactor[3], _T("D"), _T("D_shift"));

	// ファイルを閉じる
	closeFile();

	//ファイルパスを設定する
	m_DotgainFilePath = fileName;

	// ログ出力
	writeLog(fileName);
}

void CDotgainFileCreator::GetWriteValue(double inDotGainFactor, long& outFactor, long& outShift)
{
	if (inDotGainFactor == 0.0) {
		outFactor = 0;
		outShift = 0;
		return;
	}

	long shift = 0;
	long factor = (long)inDotGainFactor;
	double dotGainFactor = inDotGainFactor;

	// ドットゲインカーブの３次関数(Ax^3+Bx^2+Cx+D)の各係数を記載する
	// 各係数は16bitの範囲に収まるようなシフト量を求め、それらをセットで記載する
	while (((factor > SHRT_MIN) && (factor < SHRT_MAX)) || (factor == 0)) {
		shift++;
		dotGainFactor = dotGainFactor * 2;
		factor = static_cast<long>(dotGainFactor);
	}

	outFactor = factor / 2;
	outShift = shift - 1;
}

std::string CDotgainFileCreator::GetFilePath(E_COLORTYPE inColorType) const
{
	const char DEF_COLOR_NAME[DEF_DOTGAIN_FACTOR_NUM] = { 'C', 'M', 'Y', 'K' };
	#define DEF_DOTGAIN_LUTCURVE_FOLDER		_T("dotgain")				//!< ドットゲインカーブ保存先のフォルダ
	#define DEF_DOTGAIN_LUTCURVE_EXT		_T(".dtg")					//!< ドットゲインカーブ拡張子

	std::stringstream fileName;
	fileName << GetModuleDirectory() << PREFS_FOLDER << "\\" << DEF_DOTGAIN_LUTCURVE_FOLDER << "\\" << m_CommonName << "_" << DEF_COLOR_NAME[inColorType] << DEF_DOTGAIN_LUTCURVE_EXT;

	return fileName.str();
}