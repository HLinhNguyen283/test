/**
 * @file	DotgainFileCreator.h
 * @brief	ドットゲインファイルを作成する
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include "PaperDBDef_OP.h"
#include "Common.h"

#define DOTGAIN_CURVE_PT_CNT	256

class CDataSystemSetting;
/**
 * @brief ドットゲインファイルを作成する
**/
class CDotgainFileCreator
{
public:
	//色種類
	enum E_COLORTYPE{
		COLOER_C,	//!< Cyan
		COLOER_M,	//!< Magenta
		COLOER_Y,	//!< Yellow
		COLOER_K	//!< Black
	};
	//ドットゲイン情報
	struct S_DOTGAIN_INFO{
		E_COLORTYPE ColorType;							//!< 色種類
		ST_PDB_ANCHOR_POINT AnChorPoint[ANCHOR_PT_CNT];	//!< アンカーポイント.

		S_DOTGAIN_INFO()
		{
			ColorType = COLOER_C;
			for(int count = 0; count < ANCHOR_PT_CNT; ++count){
				AnChorPoint[count].POSITION.x = 0;
				AnChorPoint[count].POSITION.y = 0;
				AnChorPoint[count].EDIT_TYPE = FALSE;
				AnChorPoint[count].KIND = FALSE;
			}
		};
	};

	/**
	 * @brief	constructor
	 * @param[in]	inDataSystemSetting : システムのデータ
	 * @note	共通ファイル名の決定も行う
	**/
	CDotgainFileCreator(CDataSystemSetting& inDataSystemSetting) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CDotgainFileCreator() throw();

	/**
	 * @brief	１色分のドットゲインファイルを作成する
	 * @param[in]	inData : ドットゲイン情報
	 * @param[out]	outDotgainFilePath : 作成したドットゲインファイルのフルパス
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	bool Write(S_DOTGAIN_INFO& inData, std::string& outDotgainFilePath) throw();

	/**
	 * @brief	共通ファイル名を取得する
	 * @retval	共通ファイル名
	**/
	const std::string GetCommonName() const throw();

protected:
	/**
	 * @brief	画像データに共通設定をセットする
	 * @param[out]	outImageData : 設定先画像データ
	**/
	void getCommonInfoAsImageData(ImageData& outImageData) throw();

	/**
	 * @brief	LUT変換をする
	 * @param[in]	inAnChorPoint : アンカーポイント
	 * @param[out]	outDotgainFactor : ドットゲイン値
	 * @retval	true : 成功
	 * @retval	false : 失敗
	**/
	bool convertLUT(ST_PDB_ANCHOR_POINT* inAnChorPoint, double* outDotgainFactor) throw();

	/**
	 * @brief	LUT出力値、LUT入力値を取得する
	 * @param[in]	inAnChorPoint : アンカーポイント
	 * @param[out]	outLevels : LUT入力値
	 * @param[out]	outValues : LUT出力値
	 * @param[out]	outValueLeft : 左端のLUT出力値
	 * @retval	アンカーポイント種別がカーブ描画になっている数
	**/
	int getLevelsAndValues(ST_PDB_ANCHOR_POINT* inAnChorPoint, int* outLevels, int* outValues, short& outValueLeft) const throw();

	/**
	 * @brief	入力画像に対しLUT入力値とLUT出力値で指定したLUT変換を施す
	 * @param[in]	inLevels : LUT入力値
	 * @param[in]	inValues : LUT出力値
	 * @param[in]	inValueLeft : 左端のLUT出力値
	 * @param[in]	inAnchorPointCnt : アンカーポイント種別がカーブ描画になっている数
	 * @param[out]	outDotgainFactor : LUT変換後のドットゲイン値
	 * @note	エラー発生時は例外を投げる
	**/
	void imageLUT(int* inLevels, int* inValues, short inValueLeft, int inAnchorPointCnt, double* outDotgainFactor) throw(int);

	/**
	 * @brief	ドットゲイン値を補正する
	 * @param[in]	inFirstAnchorPoint : アンカーポイント
	 * @param[out]	outDotgainFactor : ドットゲイン値
	**/
	void fixDotgainFactor(long inFirstAnchorPoint, double& outDotgainFactor) throw();

	/**
	 * @brief	ドットゲインファイルを作成する
	 * @param[in]	inColorType : 色種類
	 * @param[in]	inDotgainFactor : ドットゲイン値
	 * @note	エラー発生時は例外を投げる
	**/
	void createDotgainFile(E_COLORTYPE inColorType, double* inDotgainFactor) throw(int);

	/**
	 * @brief	書き出す値を取得する
	 * @param[in]	inDotGainFactor : ドットゲイン値
	 * @param[out]	outFactor : 値
	 * @param[out]	outShift : シフト量
	**/
	void GetWriteValue(double inDotGainFactor, long& outFactor, long& outShift) throw();

	/**
	 * @brief	ドットゲインファイルのフルパスを取得する
	 * @param[in]	inColorType : 色種類
	 * @retval	ドットゲインファイルのフルパス
	**/
	std::string GetFilePath(E_COLORTYPE inColorType) const throw();


	std::string m_CommonName;							//!< 共通ファイル名
	std::string m_DotgainFilePath;						//!< ドットゲインファイルのフルパス
	double m_dotgainRatio;								//!< アンカーポイント演算定数

	short m_srcBuff[DOTGAIN_CURVE_PT_CNT];				//!< 入力画像バッファ
	ImageData m_srcData;								//!< 入力画像データ

	short m_workBuff[DOTGAIN_CURVE_PT_CNT];				//!< ワーク画像バッファ
	ImageData m_workData;								//!< ワーク画像データ
};

