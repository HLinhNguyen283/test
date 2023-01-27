/*! \file Jpeg_io.h
 *  \brief JPEG形式ファイルRead/Write
 *  \author TGL T.Fujita
 *  \date 2002/12/24 TGL T.Fujita 新規作成
 *  \date 2006/11/16 TGL Sugita SDIJPとソース共有("SWITCH_TP344_CODE" "SWITCH_SDIJP_CODE"で専用コード検索できる)
*/
#include "Common.h"

#ifndef __JPEG_IO_H_
#define __JPEG_IO_H_


long JPEG_Input(const char *inFileName, ImagePtr outImage);		// JPEG読み込み
long JPEG_Output(const char *inFileName, ImagePtr inImage);		// JPEG書込み

//==========================================================================
HMODULE load_JpegLib(void);										// DLLロード
void close_JpegLib(HMODULE hJpegLib);							// DLL解放


#endif // __JPEG_IO_H
