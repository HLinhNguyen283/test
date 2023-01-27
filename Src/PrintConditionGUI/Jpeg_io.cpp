/*! \file Jpeg_io.cpp
 *  \brief JPEG形式ファイルRead/Write
 *  \author TGL T.Fujita
 *  \date 2002/12/24 TGL T.Fujita 新規作成
 *  \date 2006/11/16 TGL Sugita SDIJPとソース共有("SWITCH_TP344_CODE" "SWITCH_SDIJP_CODE"で専用コード検索できる)
*/
#include "stdafx.h"
#include "Jpeg_io.h"
//#include "TextIniManager.h"
//! jpeglibのDLLファイル名
#define DEF_JPEG_LIB_FILE_NAME	_T("jpeg.dll")

//! DLLのハンドル
static HMODULE g_hJpegLibModule = NULL;

/* ----------------------------------------------------------------------------------------------------------- */
/* 内部関数 */
/* ----------------------------------------------------------------------------------------------------------- */

/*! \fn HMODULE load_JpegLib(void)
 *  \brief DLLのロードを行う
 *  \return DLLのハンドル
 */
HMODULE load_JpegLib(void)
{
	char szJpegLibFileName[_MAX_PATH] = {0};
	HMODULE hJpegLib = NULL;
	BOOL bRet = FALSE;

//	bRet = TINI_GetJpegLibFileName(szJpegLibFileName, _MAX_PATH);
	if (bRet) {
		hJpegLib = LoadLibrary(szJpegLibFileName);
	} else {
		hJpegLib = LoadLibrary(DEF_JPEG_LIB_FILE_NAME);
	}
	g_hJpegLibModule = hJpegLib;

	if (hJpegLib == NULL || hJpegLib == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	return hJpegLib;
}

/*! \fn close_JpegLib(HMODULE hJpegLib)
 *  \brief ロードしたDLLを解放する
 *  \param hJpegLib 解放するDLLのハンドル
 */
void close_JpegLib(HMODULE hJpegLib)
{
	if (hJpegLib != NULL && hJpegLib != INVALID_HANDLE_VALUE) {
		FreeLibrary(hJpegLib);
	}
	g_hJpegLibModule = NULL;
}

/* ----------------------------------------------------------------------------------------------------------- */
/* 外部関数 */
/* ----------------------------------------------------------------------------------------------------------- */

/*! \fn JPEG_Input(const char *inFileName, ImagePtr outImage)
 * \brief JPEG形式ファイルの読み込みを行う
 * \attention outImage->data == NULLの場合は、JPEGファイルからサイズなどの情報のみを取得する。
 * 画像データを読み込む場合は、取得した情報により必要なサイズのメモリを確保して
 * outImage->dataに割り当ててから呼び出すこと。
 * outImage->dataに割り当てたメモリは、関数の呼び出し側で解放すること。
 * \param inFileName ファイル名
 * \param outImage ImageData構造体へのポインタ
 * \return 正常 1
 * \return 異常 1以外
 */
long JPEG_Input(const char *inFileName, ImagePtr outImage)
{
	long ret = PLUGERR_NONE;

	// DLLのロード
	if(load_JpegLib() == NULL){
		return PLUGERR_LOAD_DLL;
	}
	
	if (g_hJpegLibModule == NULL) {
		return PLUGERR_LOAD_DLL;
	}

	// Jpeg_Read()の関数ポインタ取得
	typedef int (*JPEGReadProc)(char *, ImageData *);
	JPEGReadProc jpeg_read = (JPEGReadProc)GetProcAddress(g_hJpegLibModule, "Jpeg_Read");
	if (jpeg_read == NULL) {
		ret = PLUGERR_GETPROC_DLL;
		goto err_exit;
	}
	
	// jpegファイルの読み込み
	if(jpeg_read((char *)inFileName, outImage) != 1){
		ret = PLUGERR_READ_FILE;
		goto err_exit;
	}

err_exit:
	// DLLの解放
	close_JpegLib(g_hJpegLibModule);

	return ret;
}

/*! \fn JPEG_Output(const char *inFileName, ImagePtr inImage)
 * \brief JPEG形式ファイルの書込み
 * \attention inImage->dataに割り当てたメモリは、関数の呼び出し側で解放すること。
 * inImage->optionには以下の保存時のオプションを、OR演算子により組み合わせて指定することが出来る
 * いずれのオプションも指定しない場合は0を指定する。
 * [保存品質の指定]
 * 以下のうちいずれか１つを指定する
 * LIBJPGOPTION_QUALITY_MIN     = 0   (低品質)
 * LIBJPGOPTION_QUALITY_LOW     = 10
 * LIBJPGOPTION_QUALITY_MEDIUM  = 40
 * LIBJPGOPTION_QUALITY_DEFAULT = 75
 * LIBJPGOPTION_QUALITY_HIGH    = 80
 * LIBJPGOPTION_QUALITY_MAX	    = 100 (高品質)
 * [ベースラインJPEGで保存]
 * LIBJPGOPTION_FORCE_BASELINE\
 * [プログレッシブJPEGで保存]
 * LIBJPGOPTION_PROGRESSIVE
 * \param inFileName ファイル名
 * \param inImage ImageData構造体へのポインタ
 * \return 正常 1
 * \return 異常 1以外
 */
long JPEG_Output(const char *inFileName, ImagePtr inImage)
{
	long ret = PLUGERR_NONE;

	// DLLのロード
	if(load_JpegLib() == NULL){
		return PLUGERR_LOAD_DLL;
	}
	
	if (g_hJpegLibModule == NULL) {
		return PLUGERR_LOAD_DLL;
	}

	// Jpeg_Write()の関数ポインタ取得
	typedef int (*JPEGWriteProc)(char *, ImageData *);
	JPEGWriteProc jpeg_write = (JPEGWriteProc)GetProcAddress(g_hJpegLibModule, "Jpeg_Write");
	if (jpeg_write == NULL) {
		ret = PLUGERR_GETPROC_DLL;
		goto err_exit;
	}
	
	// jpegファイルの書き出し
	if(jpeg_write((char *)inFileName, inImage) != 1){
		ret = PLUGERR_WRITE_FILE;
		goto err_exit;
	}

err_exit:
	// DLLの解放
	close_JpegLib(g_hJpegLibModule);

	return ret;
}

