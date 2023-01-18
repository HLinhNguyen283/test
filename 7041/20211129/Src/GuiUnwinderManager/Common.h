/*! \file Common.h
 *  \brief プリナビプロジェクト共通ヘッダ
 *  \author TGL Sugita
 *  \date 2006/11/16 TGL Sugita TP344から分離。SDIJP専用ソース
 *  \date 2007/11/27 TGL Hamaguchi SPI専用ソースに変更.
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <STDIO.H>
//
#include "CommonDef.h"
#include "CommonUI.h"
#include <string>

//! パラメータリストの入出力情報
typedef struct _IO_INFO {
	struct	_IO_INFO	*NEXT;				//!< 次のIO_INFOへのリンク。NULLはリスト終端。
	long			IO_INFO_ID;				//!< IO_INFO用情報ブロック識別子
	long			IO_INFO_VERSION;		//!< 同一IO_INFO_IDのバージョン番号
	long			DATA_SIZE;				//!< 管理モジュールがバッファ割り当てに使用するサイズ
} IO_INFO;

//! モジュール構成情報
typedef struct _MODULE_INFO {
	long		MODULE_ID;					//!< 情報ブロックの識別子
	long		MODULE_VERSION;				//!< 同一MODULE_IDのバージョン番号
	long		MODULE_TYPE;				//!< モジュールの種類。PIM_INPUT,PIM_PROCESS,PIM_OUTPUT,PIM_UI
	long		REQUIRE_CRITSEC;			//!< 管理モジュールによる資源管理が必要な場合TRUE
	long		INSTALL_SERVICE;			//!< サービス機能要求用。TRUE:要求,FALSE:要求しない
	FARPROC		SERVICE_ADDRESS;			//!< サービス実行用関数のアドレス
	long		UI_TYPE;					//!< UI_NONE:GUIなし,UI_NONE以外:MODULE_IDが同一の専用GUIあり
	IO_INFO		*IO_INFO_LIST;				//!< パラメータリストの入出力情報。GUIの場合はNULL。
	char		MODULE_NAME[MODULE_DISP_NAME_SIZE];		//!< モジュール名
} MODULE_INFO;

//! パラメータリスト
typedef struct _IO_HEADER {
	struct	_IO_PARAMS	*NEXT;				//!< 次のIO_PARAMSへのリンク。NULLは終端。
	long				IO_INFO_ID;			//!< 情報ブロックの識別子
	long				IO_INFO_VERSION;	//!< 同一IO_INFO_IDのバージョン番号
	char				*OWNER;				//!< 初期化を依頼したモジュールへのパス
	long				FILE_DATA_SIZE;		//!< DATAをファイルに書いた際のサイズ
	long				DATA_SIZE;			//!< DATAのバイト数
	// 8バイトアライメントを遵守すること
} IO_HEADER;

//! IO_PARAMS
typedef struct _IO_PARAMS {
	IO_HEADER			HEAD;				//!< IO_PARAMSヘッダ
	long				DATA[1];			//!< パラメータの入出力情報設定値
} IO_PARAMS;

#define IO_HEAD_SIZE	sizeof(IO_HEADER)	//!< ヘッダサイズ


////! バッチJOBファイルリスト構造体
//typedef struct _ST_BAT_JOB_FILE_LIST{
//	_ST_BAT_JOB_FILE_LIST*	prev;							//!< 前
//	_ST_BAT_JOB_FILE_LIST*	next;							//!< 次
//	char					BAT_JOB_FILE[JOB_NAME_SIZE];	//!< バッチJOBファイル名
//}ST_BAT_JOB_FILE_LIST;

#define DEF_JOB_RUN_END_YES		1	//!< JOB実行終了を行う。
#define DEF_JOB_RUN_END_NO		0	//!< JOB実行終了を行わない。

//! JOB構造体
typedef struct _JOB{
	#define JOBVERSION 1
	long			VERSION;													//!< JOB構造体のバージョン
	char			NAME[JOB_NAME_SIZE];										//!< JOB名
	long			JOB_MODE;													//!< JOBモード（実行 or セットアップ）
	long			STATUS;														//!< ステータス情報
	ENUM_JOB_PAHSE	PHASE;														//!< JOBの状態
	long			INFO_FLG;													//!< JOBの汎用的な状態を表すフラグ複数設定できるので"=="禁止、"&"演算子でチェックすること
	long			PIM_COUNT;													//!< JOB実行に参加するプラグインモジュール数
	char			**PIM;														//!< プラグインモジュールへのパス（実行順）
	HMODULE			*HPIM;														//!< プラグインモジュールへのハンドル（〃）
	FARPROC			*PROC;														//!< PIM_Process関数のアドレス（〃）
	IO_PARAMS		*IO;														//!< パラメータリスト
	long			JOB_RUN_END;												//!< JOB実行終了パラメータ(Saberでは、使用しない)
} JOB, *PJOB;



//---------------------------------------------------------------------------
// 画像保存 I/Oパラメータ

//! rational型
typedef struct tagFixed {
	long numer; //!< 分子
	long denom; //!< 分母
} Fixed;

//! 画像情報構造体
typedef struct tagImageData {
	ImageFormat format;									//!< 画像の形式
	long option;										//!< 画像保存時のオプション指定
	long width, height;									//!< 画像データの幅・高さ
	long mode;											//!< 画像データの形式
	long bps;											//!< bits per sample (1, 8 or 16)
	long spp;											//!< samples per pixel (1, 3 or 4)
	long interleave;									//!< 画像データの格納方法
	Fixed reso;											//!< 解像度
	void *data;											//!< 画像データ
} ImageData, *ImagePtr;

#define IMAGE_FILENAME_SIZE	256			//!< 画像ファイルパスサイズ
#define MAX_IMAGE_COLORS	8			//!< 最大チャンネル数

//! 画像作成パスリスト用
typedef struct tagImagePath {
	char szFilePath[MAX_IMAGE_COLORS][IMAGE_FILENAME_SIZE];		//!< ファイルパス
} ImagePath;

//---------------------------------------------------------------------------

typedef struct tagFilterParam {
	int *kernel;
	SIZE kernelSize;
	POINT anchor;
	int divisor;
} FilterParam, *FilterParamPtr;
//---------------------------------------------------------------------------

#define WAIT_THREAD_SIZE 30			//!< CS取得待機中のスレッドの最大数

//! CS状態構造体
typedef struct _ST_CRITICAL_SECTION_STATUS{
	BOOL  init;						//!< 利用中フラグ 
	DWORD module_id;				//!< 作成したモジュールID
	char  name[16];					//!< CS名
	DWORD signal_thread_id;			//!< CSシグナル状態を取得中のスレッドID 
	DWORD signal_begin_time;		//!< CSシグナル状態になった時間(msec)(GetTickCount()) 
	DWORD wait_time_limit;			//!< このCS取得待機で想定している限界待機時間(msec) 
	DWORD wait_thread_id[WAIT_THREAD_SIZE];		//!< CS取得待機中のスレッドID 
	DWORD wait_begin_time[WAIT_THREAD_SIZE];	//!< CS取得待機を開始した時間(msec)(GetTickCount()) 
}ST_CRITICAL_SECTION_STATUS;

//! CS情報構造体
typedef struct _ST_CRITICAL_SECTION_INFO{
	CRITICAL_SECTION				cs;		//!< CS状態
	BOOL							init;	//!< 初期化フラグ
	ST_CRITICAL_SECTION_STATUS*		info;	//!< CS状態へのポインタ
}ST_CRITICAL_SECTION_INFO;

//---------------------------------------------------------------------------

//! スレッド情報構造体
typedef struct _ST_THREAD_INFO{
	HANDLE thread_handle;					//!< スレッドハンドル
	DWORD thread_id;						//!< スレッドID
	void (__stdcall* close_func)(void);		//!< スレッドハンドルクローズ後に実行する関数
}ST_THREAD_INFO;

//---------------------------------------------------------------------------

//! JOB Funcion Type Definitions
typedef PJOB (*CREATEJOBINFO)(long);											//!< JOB関連コールバック関数定義
typedef void (*FREEJOBINFO)(PJOB, BOOL);										//!< JOB関連コールバック関数定義
typedef PJOB (*LOADJOBINFO)(char*, long*, int, long, BOOL);						//!< JOB関連コールバック関数定義
typedef long (*SAVEJOBINFO)(PJOB, char*, BOOL, BOOL);							//!< JOB関連コールバック関数定義
typedef PJOB (*GETJOBREF)(ENUM_JOB_MODE, long);									//!< JOB関連コールバック関数定義
typedef long (*CHANGEJOBRUNPHASE)(PJOB, long);									//!< JOB関連コールバック関数定義
typedef IO_PARAMS* (*FINDIOPARAMSPROC)(PJOB, long);								//!< JOB関連コールバック関数定義
typedef long (*FORCETODELETEJOBFILE)(char*);									//!< JOB関連コールバック関数定義
typedef PJOB (*STARTJOBSETUPMODE)(char*);										//!< JOB関連コールバック関数定義
typedef long (*ENDJOBSETUPMODE)(PJOB, BOOL, BOOL);								//!< JOB関連コールバック関数定義
typedef char* (*GETJOBSETUPFILEPATH)(PJOB, long);								//!< JOB関連コールバック関数定義
typedef PJOB (*STARTJOBRUNMODE)(char*);											//!< JOB関連コールバック関数定義
typedef char* (*GETJOBRUNFILEPATH)(PJOB, long);									//!< JOB関連コールバック関数定義
typedef void (*STARTJOBRUNNING)();												//!< JOB関連コールバック関数定義
typedef void (*ENTERJOBSETUPCRITICALSECTION)();									//!< JOB関連コールバック関数定義
typedef void (*LEAVEJOBSETUPCRITICALSECTION)();									//!< JOB関連コールバック関数定義
typedef void (*SETOLUGINTIMERPAUSESTATE)(char*, BOOL);							//!< JOB関連コールバック関数定義

typedef long (*PM_CREATENEWJOB)(PJOB);
typedef long (*PM_LOADJOBFILE)(FILE*, PJOB, BOOL, long[], long);
typedef long (*PM_SAVEJOBFILE)(FILE*, PJOB, BOOL, BOOL);
typedef long (*PM_FREEJOBFILE)(PJOB, BOOL);

//! JOB Callback Proc Struct
typedef struct _JOBCBR {
#define JOBCBRVERSION	0
	long VERSION;													//!< バージョン
	CREATEJOBINFO CBR_CreateJobInfo;								//!< JOBバッファ新規作成
	FREEJOBINFO CBR_FreeJobInfo;									//!< JOBバッファ解放
	LOADJOBINFO CBR_LoadJobInfo;									//!< JOBファイル読み込み
	SAVEJOBINFO CBR_SaveJobInfo;									//!< JOBファイル書き込み
	GETJOBREF CBR_GetJobRef;										//!< JOB参照(セットアップ・実行中)
	CHANGEJOBRUNPHASE CBR_ChangeJobRunPhase;						//!< 実行中のJOBのフェーズを変更する
	FINDIOPARAMSPROC CBR_FindIOParams;								//!< IO_PARAMS参照
	FORCETODELETEJOBFILE CBR_ForceToDeleteJobFile;					//!< JOBファイル削除
	STARTJOBSETUPMODE CBR_StartJobSetupMode;						//!< JOBセットアップ開始
	ENDJOBSETUPMODE CBR_EndJobSetupMode;							//!< JOBセットアップ終了
	GETJOBSETUPFILEPATH CBR_GetJobSetupFilePath;					//!< セットアップ中のJOBファイルパス取得
	STARTJOBRUNMODE CBR_StartJobRunMode;							//!< JOB実行開始
	GETJOBRUNFILEPATH CBR_GetJobRunFilePath;						//!< 実行中のJOBファイルパス取得
	STARTJOBRUNNING CBR_StartJobRunning;							//!< タスク管理にJOB実行開始を通達
	ENTERJOBSETUPCRITICALSECTION CBR_EnterJobSetupCriticalSection;	//!< JOB関連コールバック関数定義
	LEAVEJOBSETUPCRITICALSECTION CBR_LeaveJobSetupCriticalSection;	//!< JOB関連コールバック関数定義
	SETOLUGINTIMERPAUSESTATE CBR_SetPluginTimerPauseState;			//!< PIM関係のプラグインタイマー停止・再開関数.

	PM_CREATENEWJOB CBR_PM_CreateNewJob;	//!< プラグイン管理的なジョブバッファ新規作成.
	PM_LOADJOBFILE CBR_PM_LoadJobFile;		//!< プラグイン管理的なジョブファイル読み込み.
	PM_SAVEJOBFILE CBR_PM_SaveJobFile;		//!< プラグイン管理的なジョブファイル書き込み.
	PM_FREEJOBFILE CBR_PM_FreeJobFile;		//!< プラグイン管理的なジョブバッファ解放
} JOBCBR, *LPJOBCBR;

//! LOG Funcion Type Definitions
typedef void (*WRITETOLOGPROC)(long, char*);										//!< Log関連コールバック関数定義
typedef char* (*GETFULLPATHNAMEPROC)(char*, char*);										//!< Log関連コールバック関数定義
typedef void (*FORCEWRITELOG)(void);												//!< Log関連コールバック関数定義
typedef int (*GETFIRSTDAYOFWEEKPROC) ();											//!< Log関連コールバック関数定義
typedef void (*TM_OUTPUTALLTHREADLOG)();											//!< Log関連コールバック関数定義
typedef bool (*TM_OUTPUTCURRENTTHREADLOG)(std::string, int, bool);					//!< Log関連コールバック関数定義
typedef bool (*TM_OUTPUTCURRENTTHREADLOGDETAIL)(std::string, int, bool);			//!< Log関連コールバック関数定義
typedef std::string (*TM_GETCURRENTTHREADLOG)(int);									//!< Log関連コールバック関数定義
typedef std::string (*TM_GETCURRENTTHREADLOGDETAIL)(int);							//!< Log関連コールバック関数定義

//! LOG Callback Proc Struct
typedef struct tagLOGCBR
{
#define LOGCBRVERSION	0
	long VERSION;												//!< バージョン
	WRITETOLOGPROC				CBR_WriteToLog;					//!< ログを出力する
	GETFULLPATHNAMEPROC			CBR_GetFullPathName;			//!< フルパスを取得する
	FORCEWRITELOG				CBR_ForceWriteLog;				//!< 強制ログを出力する
	GETFIRSTDAYOFWEEKPROC		CBR_GetFirstDayOfWeek;			//!< 週の始めの曜日取得する
	WRITETOLOGPROC				CBR_WriteToLogNameless;			//!< ログを出力する(プラグイン名なし)
	TM_OUTPUTALLTHREADLOG 			CBR_OutputAllThreadLog;				//!< 全スレッドのスレッドログをファイル出力する
	TM_OUTPUTCURRENTTHREADLOG		CBR_OutputCurrentThreadLog;			//!< カレントスレッドのスレッドログをファイル出力する(簡易)
	TM_OUTPUTCURRENTTHREADLOGDETAIL	CBR_OutputCurrentThreadLogDetail;	//!< カレントスレッドのスレッドログをファイル出力する(詳細)
	TM_GETCURRENTTHREADLOG			CBR_GetCurrentThreadLog;			//!< カレントスレッドのスレッドログ文字列を取得する(簡易)
	TM_GETCURRENTTHREADLOGDETAIL	CBR_GetCurrentThreadLogDetail;		//!< カレントスレッドのスレッドログ文字列を取得する(詳細)
} LOGCBR, *LPLOGCBR;

//! ColorConvert Type Definitions
typedef long (*CONVERTCOLORPIXELPROC)(long, void*, long, void*);								//!< 色変換用コールバック関数定義
typedef long (*CONVERTCOLORIMAGEPROC)(long, long, long, void*, long, long, long, void*);		//!< 色変換用コールバック関数定義
typedef long (*CONVERTCOLORIMAGEEXPROC)(long, PSIZE, PRECT, void*, long, PSIZE, void*);			//!< 色変換用コールバック関数定義
typedef int (*IMAGERESIZEPROC)(ImagePtr, ImagePtr, double, int);								//!< 色変換用コールバック関数定義
typedef int (*IMAGERESIZEXYPROC)(ImagePtr, ImagePtr, double, double, int);						//!< 色変換用コールバック関数定義
typedef int (*IMAGEMIRRORPROC)(ImagePtr, ImagePtr, int);										//!< 色変換用コールバック関数定義
typedef int (*IMAGEROTATEPROC)(ImagePtr, ImagePtr, double, double, double, int);				//!< 色変換用コールバック関数定義
typedef int (*IMAGEGAUSSFILTERPROC)(ImagePtr, ImagePtr, int);									//!< 色変換用コールバック関数定義
typedef int (*IMAGEMEDIANFILTERPROC)(ImagePtr, ImagePtr, int);									//!< 色変換用コールバック関数定義
typedef const char* (*IMAGEGETSTATUSSTRINGPROC)(int);											//!< 色変換用コールバック関数定義
typedef int (*IMAGECOVERAGEPROC)(ImagePtr src, PRECT roi, double *coverage);					//!< 色変換用コールバック関数定義
typedef int (*IMAGEPIXELTOPLANARPROC)(ImagePtr src, ImagePtr dst);								//!< 色変換用コールバック関数定義
typedef int (*IMAGEPLANARTOPIXELPROC)(ImagePtr src, ImagePtr dst);								//!< 色変換用コールバック関数定義
typedef int (*IMAGERESIZEXYROI)(ImagePtr , ImagePtr , double , double , PRECT , int );			//!< 色変換用コールバック関数定義
typedef int (*IMAGESELECTMATCHPOINT)(ImagePtr, PRECT, short, short, PPOINT);					//!< 色変換用コールバック関数定義
typedef int (*IMAGECOLORTWIST)(ImagePtr, ImagePtr, float*);										//!< 色変換用コールバック関数定義
typedef int (*IMAGEADJUSTLEVEL)(ImagePtr , ImagePtr , PRECT );									//!< 色変換用コールバック関数定義
typedef int (*IMAGECHECKAFFINITY)(ImagePtr , ImagePtr , int*, int );							//!< 色変換用コールバック関数定義
typedef int (*IMAGEFILTER)(ImagePtr , ImagePtr , FilterParamPtr );								//!< 色変換用コールバック関数定義
typedef int (*IMAGESTATISTICS)(ImagePtr , PRECT , ImageStatisticsFunc , PVOID );				//!< 色変換用コールバック関数定義
typedef int (*IMAGEDISPLAYWINDOW)(ImagePtr , PVOID , ImageDisplayFunc , int*);					//!< 色変換用コールバック関数定義
typedef int (*IMAGEDRAW)(ImagePtr , ImageDrawFunc , int*, unsigned char*, float opacity /*= 1*/);	//!< 色変換用コールバック関数定義
typedef int (*IMAGECOPY)(ImagePtr, ImagePtr, PBYTE);											//!< 色変換用コールバック関数定義

typedef char* (*STRINGFINDEXTENSION)( const char* );											//!< 色変換用コールバック関数定義
typedef int (*STRINGCREATEUNIQUEFILENAME)( const char*, const char*, char* );					//!< 色変換用コールバック関数定義
typedef int (*STRINGGETORDER)( const char*, const char* );										//!< 色変換用コールバック関数定義
typedef char* (*STRINGGETCOLOR)( const char*, int , char* );									//!< 色変換用コールバック関数定義
typedef int (*STRINGTOINT)( const char*, int*, BOOL );											//!< 色変換用コールバック関数定義
typedef char* (*STRINGFIND)( const char*, const char*, BOOL );									//!< 色変換用コールバック関数定義
typedef char* (*STRINGFINDC)( const char*, char, BOOL );										//!< 色変換用コールバック関数定義
typedef char* (*STRINGFINDCANY)( const char*, const char*, BOOL );								//!< 色変換用コールバック関数定義
typedef int (*STRINGINSERT)( char*, const char*, int, char* );									//!< 色変換用コールバック関数定義
typedef int (*STRINGREMOVE)( char*, int, int, char* );											//!< 色変換用コールバック関数定義
typedef int (*STRINGCOMPARE)( const char*, const char*, int, BOOL, BOOL );						//!< 色変換用コールバック関数定義
typedef int (*STRINGEQUAL)( const char*, const char*, int );									//!< 色変換用コールバック関数定義
typedef int (*STRINGTRIMC)( char*, char, char* );												//!< 色変換用コールバック関数定義
typedef int (*STRINGTRIMCANY)( const char*, const char*, char*, int );							//!< 色変換用コールバック関数定義
typedef int (*STRINGREPLACEC)( const char*, char*, char, char );								//!< 色変換用コールバック関数定義
typedef int (*STRINGUPPERCASE)( char*, char*, BOOL );											//!< 色変換用コールバック関数定義
typedef int (*STRINGLOWERCASE)( char*, char*, BOOL );											//!< 色変換用コールバック関数定義
typedef int (*STRINGHASH)( const char* );														//!< 色変換用コールバック関数定義
typedef int (*STRINGCONCAT)( const char*, const char*, char* );									//!< 色変換用コールバック関数定義
typedef int (*STRINGCONCATN)( const char*, int, char* );										//!< 色変換用コールバック関数定義
typedef int (*STRINGCONCATC)( const char*, int, char, char* );									//!< 色変換用コールバック関数定義
typedef int (*STRINGSPLITC)(const char*, char, char*, int, int* );								//!< 色変換用コールバック関数定義

typedef int (*IMAGECONVERT)(ImagePtr, ImagePtr);												//!< 色変換用コールバック関数定義
typedef int (*IMAGECREATESHADINGPARAM)(ImagePtr, ImagePtr, int[3], ImagePtr);					//!< 色変換用コールバック関数定義
typedef int (*IMAGEREDUCEAFTERIMAGE)(ImagePtr, ImagePtr, double*);								//!< 色変換用コールバック関数定義
typedef int (*IMAGESHADING)(ImagePtr, ImagePtr, ImagePtr);										//!< 色変換用コールバック関数定義
typedef int (*IMAGESHEAR)(ImagePtr, ImagePtr, double, double, double, double, int);				//!< 色変換用コールバック関数定義
typedef int (*IMAGELUT)(ImagePtr, ImagePtr, int*, int*, int, int, ImagePtr);					//!< 色変換用コールバック関数定義
typedef int (*IMAGESWAP)(ImagePtr, ImagePtr);													//!< 色変換用コールバック関数定義

typedef int (*IMAGECOMPRESS)(const char* , int , char* , int* );								//!< 色変換用コールバック関数定義
typedef int (*IMAGEDECOMPRESS)(const char* , int , char* , int );								//!< 色変換用コールバック関数定義

typedef long (*LOADIMAGEFILEPROC) (const char *, ImagePtr);										//!< 色変換用コールバック関数定義
typedef long (*SAVEIMAGEFILEPROC) (const char *, ImagePtr);										//!< 色変換用コールバック関数定義
typedef long (*CREATEDISPIMAGEPROC)(BYTE*, SIZE, RECT*, BYTE*, SIZE, long, long);				//!< 色変換用コールバック関数定義
typedef long (*PAINTCMYKIMAGEPROC)(BOOL, BYTE*, SIZE, COLORREF*, long, BOOL, BYTE*, BOOL);		//!< 色変換用コールバック関数定義

typedef int (*IMAGEHISTOGRAM)(ImagePtr , int*[4] , int*[4] , int );								//!< 色変換用コールバック関数定義
typedef int (*IMAGETACALERT)(ImagePtr , ImagePtr , int , unsigned char[4] );					//!< 色変換用コールバック関数定義
typedef int (*IMAGEMOSAICFILTER)(ImagePtr , ImagePtr , int);									//!< 色変換用コールバック関数定義


//! ColorConvert Callback Proc Struct
typedef struct tagCOLORCONVCBR
{
#define COLORCONVCBRVERSION		0
	long VERSION;														//!< バージョン
	CONVERTCOLORPIXELPROC		CBR_ConvertColorPixel;					//!< 指定されたピクセルの色空間を変換する
	CONVERTCOLORIMAGEPROC		CBR_ConvertColorImage;					//!< 全画素を指定された色空間に変換する
	CONVERTCOLORIMAGEEXPROC		CBR_ConvertColorImageEx;				//!< 全画素を指定された色空間に変換する(トリミング機能つき）
	IMAGERESIZEPROC				CBR_ImageResize;						//!< 画像の変倍処理。
	IMAGERESIZEXYPROC			CBR_ImageResizeXY;						//!< 画像の変倍処理(X・Yごとに倍率指定可能）
	IMAGEMIRRORPROC				CBR_ImageMirror;						//!< 画像のミラー反転処理。
	IMAGEROTATEPROC				CBR_ImageRotate;						//!< 画像の回転処理。
	IMAGEGAUSSFILTERPROC		CBR_ImageGaussFilter;					//!< 画像のガウスフィルター処理
	IMAGEMEDIANFILTERPROC		CBR_ImageMedianFilter;					//!< 画像のメディアンフィルター処理
	IMAGEGETSTATUSSTRINGPROC	CBR_ImageGetStatusString;				//!< 各種関数が返すエラーコードを文字列に変換する
	IMAGECOVERAGEPROC			CBR_ImageCoverage;						//!< 絵柄面積率計算。
	IMAGEPIXELTOPLANARPROC		CBR_ImagePixelToPlanar;					//!< ピクセル順次をプレーン順次に変換する。
	IMAGEPLANARTOPIXELPROC		CBR_ImagePlanarToPixel;					//!< プレーン順次をピクセル順次に変換する。
	IMAGERESIZEXYROI			CBR_ImageResizeXYroi;					//!< 画像の変倍処理(範囲指定あり)。
	IMAGESELECTMATCHPOINT		CBR_ImageSelectMatchPoint;				//!< 画像中の特徴点を抽出する
	IMAGECOLORTWIST				CBR_ImageColorTwist;					//!< 
	IMAGEADJUSTLEVEL			CBR_ImageAdjustLevel;					//!< 画像の明るさを調整する。
	IMAGECHECKAFFINITY			CBR_ImageCheckAffinity;					//!< 画像の類似度を比較する
	IMAGEFILTER					CBR_ImageFilter;						//!< 画像に指定のフィルター処理をする。
	IMAGESTATISTICS				CBR_ImageStatistics;					//!< 画像を分析する
	IMAGEDISPLAYWINDOW			CBR_ImageDisplayWindow;					//!< 画像をウィンドウに表示する
	IMAGEDRAW					CBR_ImageDraw;							//!< 画像上に指定した内容を描画する
	IMAGECOPY					CBR_ImageCopy;							//!< 画像の内容をコピーする
	STRINGFINDEXTENSION			CBR_StringFindExtension;				//!< 拡張子の位置を検索する
	STRINGCREATEUNIQUEFILENAME	CBR_StringCreateUniqueFileName;			//!< 固有ファイル名の設定。
	STRINGGETORDER				CBR_StringGetOrder;						//!< 刷り順文字列中の指定色の順序を返す。
	STRINGGETCOLOR				CBR_StringGetColor;						//!< 色名を返す。
	STRINGTOINT					CBR_StringToInt;						//!< 文字列を整数に変換する。
	STRINGFIND					CBR_StringFind;							//!< 文字列内の指定文字列を検索する（大文字/小文字の区別あり）
	STRINGFINDC					CBR_StringFindC;						//!< 文字列内の指定文字を検索する（大文字/小文字の区別あり）
	STRINGFINDCANY				CBR_StringFindCAny;						//!< 文字列内の指定文字群を検索する（大文字/小文字の区別あり）
	STRINGINSERT				CBR_StringInsert;						//!< 文字列内の指定位置に文字列を挿入する（格納先には十分な容量を確保しておく必要あり）
	STRINGREMOVE				CBR_StringRemove;						//!< 文字列内の指定文字数を削除する
	STRINGCOMPARE				CBR_StringCompare;						//!< 2文字列を比較する
	STRINGEQUAL					CBR_StringEqual;						//!< 2文字列の一致を検査する（大文字/小文字の区別あり）
	STRINGTRIMC					CBR_StringTrimC;						//!< 文字列の先頭、及び終端に存在する指定文字を削除する（大文字/小文字の区別あり）
	STRINGTRIMCANY				CBR_StringTrimCAny;						//!< 文字列の先頭、及び終端に存在する指定文字群を削除する（大文字/小文字の区別あり）
	STRINGREPLACEC				CBR_StringReplaceC;						//!< 文字列内の文字を置換する（大文字/小文字の区別あり）
	STRINGUPPERCASE				CBR_StringUppercase;					//!< 文字列を大文字に変換する
	STRINGLOWERCASE				CBR_StringLowercase;					//!< 文字列を小文字に変換する
	STRINGHASH					CBR_StringHash;							//!< 文字列のハッシュ値を計算する
	STRINGCONCAT				CBR_StringConcat;						//!< 2文字列を合成する。
	STRINGCONCATN				CBR_StringConcatN;						//!< 複数の文字列を合成する。
	STRINGCONCATC				CBR_StringConcatC;						//!< 区切り文字を挿入しながら複数の文字列を合成する区切り文字を挿入しながら複数の文字列を合成する
	STRINGSPLITC				CBR_StringSplitC;						//!< 文字列を区切り文字で分割する
	IMAGECONVERT				CBR_ImageConvert;						//!< 画像のビット深度を変換する
	IMAGECREATESHADINGPARAM		CBR_ImageCreateShadingParam;			//!< シェーディング補正係数を作成する
	IMAGEREDUCEAFTERIMAGE		CBR_ImageReduceAfterImage;				//!< 画像に対しCCDの残像補正を施す
	IMAGESHADING				CBR_ImageShading;						//!< 画像に対しシェーディング補正をする
	IMAGESHEAR					CBR_ImageShear;							//!< 画像に対しシアー変換する
	IMAGELUT					CBR_ImageImageLUT;						//!< 画像に対し指定したLUT変換する
	IMAGESWAP					CBR_ImageSwap;							//!< 16ビットデータのバイト順序（エンディアン）を反転させる 
	IMAGECOMPRESS				CBR_ImageCompress;						//!< 入力バッファの内容を圧縮する
	IMAGEDECOMPRESS				CBR_ImageDecompress;					//!< 入力バッファの内容を解凍する

	LOADIMAGEFILEPROC			CBR_LoadImageFile;						//!< 画像ファイルを読み込む
	SAVEIMAGEFILEPROC			CBR_SaveImageFile;						//!< 画像ファイルを書き込む
	CREATEDISPIMAGEPROC			CBR_CreateDispImage;					//!< 回転プレビューRGB画像作成ルーチン
	PAINTCMYKIMAGEPROC			CBR_PaintCMYKImage;						//!< 入力画像をCOLORREFで指定した色に着色する

	IMAGEHISTOGRAM				CBR_ImageHistogram;						//!< ヒストグラムを作成する
	IMAGETACALERT				CBR_ImageTACAlert;						//!< TAC (Total Area Coverage) 警告画素をマークする
	IMAGEMOSAICFILTER			CBR_ImageMosaicFilter;					//!< モザイクフィルターを施す
} COLORCONVCBR, *LPCOLORCONVCBR;

//! リソース管理コールバック
typedef long (*GETLOCALEFILEPROC)(char*, long);															//!< リソース関連コールバック関数定義
typedef void* (*LOADRESOURCESTRINGSPROC)(char*, char*, long);											//!< リソース関連コールバック関数定義
typedef void (*RELEASESTRINGSPROC)(void*);																//!< リソース関連コールバック関数定義
typedef char* (*STRFROMTABLEPROC)(long, void*);															//!< リソース関連コールバック関数定義
typedef void* (*LOADRESOURCEBITMAPSPROC)(char*, char*, long);											//!< リソース関連コールバック関数定義
typedef void (*RELEASEBITMAPSPROC)(void*);																//!< リソース関連コールバック関数定義
typedef HBITMAP (*BMPFROMTABLEPROC)(long, void*);														//!< リソース関連コールバック関数定義
typedef void* (*LOADRESOURCEICONSPROC)(char*, char*, long);												//!< リソース関連コールバック関数定義
typedef void (*RELEASEICONSPROC)(void*);																//!< リソース関連コールバック関数定義
typedef HICON (*ICONFROMTABLEPROC)(long, void*);														//!< リソース関連コールバック関数定義
typedef DWORD (*LOADRESPRESSERRMSGPROC)(DWORD, char*, long, char*, long, char*, long, char*, long);		//!< リソース関連コールバック関数定義
typedef long (*GETFRAMEPROC)(HBITMAP, long, long);														//!< リソース関連コールバック関数定義
typedef char* (*GETRESLANGUAGE)();																		//!< リソース関連コールバック関数定義
typedef const char* (*GETSTRINGPROC)();																	//!< リソース関連コールバック関数定義

//! Resource Callback Proc Struct
typedef struct tagRESCBR
{
#define RESCBRVERSION		0
	long VERSION;											//!< バージョン
	GETLOCALEFILEPROC 			CBR_GetLocaleFile;			//!< RESOURCEフォルダ内INIファイルパス検索。
	LOADRESOURCESTRINGSPROC 	CBR_LoadResourceStrings;	//!< 文字列リソース読込。
	RELEASESTRINGSPROC 			CBR_ReleaseStrings;			//!< 文字列リソース解放。
	STRFROMTABLEPROC 			CBR_StrFromTable;			//!< 文字列リソース取得。
	LOADRESOURCEBITMAPSPROC 	CBR_LoadResourceBitmaps;	//!< ビットマップリソース読込。
	RELEASEBITMAPSPROC 			CBR_ReleaseBitmaps;			//!< ビットマップリソース解放。
	BMPFROMTABLEPROC 			CBR_BmpFromTable;			//!< ビットマップリソース取得。
	LOADRESOURCEICONSPROC 		CBR_LoadResourceIcons;		//!< アイコンリソース読込。
	RELEASEICONSPROC 			CBR_ReleaseIcons;			//!< アイコンリソース解放。
	ICONFROMTABLEPROC 			CBR_IconFromTable;			//!< アイコンリソース取得。
	GETFRAMEPROC 				CBR_GetFrame;				//!< 統合ビットマップから指定のフレームの描画開始位置アドレスを取得する。
	GETRESLANGUAGE				CBR_GetResLanguage;			//!< 指定した言語IDの言語名を取得。
	GETSTRINGPROC				CBR_GetStringPrinterNameFront;		//!< 文字リソース取得:表機.
	GETSTRINGPROC				CBR_GetStringPrinterNameBack;		//!< 文字リソース取得:裏機.
	GETSTRINGPROC				CBR_GetStringMessageLogHeader;		//!< 文字リソース取得:ダイアログログ出力ヘッダー.
	GETSTRINGPROC				CBR_GetStringMessageLogPushButton;	//!< 文字リソース取得:ダイアログログ出力ボタン押下.
} RESCBR, *LPRESCBR;


//! ファイル管理
typedef LPCTSTR (*GETMODULEDIRECTORY)(void);													//!< ファイル関連コールバック関数定義
typedef void* (*IO_MALLOCPROC)(size_t);															//!< ファイル関連コールバック関数定義
typedef void (*IO_FREEPROC)(void*);																//!< ファイル関連コールバック関数定義
typedef FILE* (*IO_FOPENPROC)(const char*, const char*);										//!< ファイル関連コールバック関数定義
typedef FILE* (*IO_FOPENRETRYPROC)(const char*, const char*);									//!< ファイル関連コールバック関数定義
typedef int (*IO_FCLOSEPROC)(FILE*);															//!< ファイル関連コールバック関数定義
typedef size_t (*IO_FREADPROC)(void*, size_t, size_t, FILE*);									//!< ファイル関連コールバック関数定義
typedef size_t (*IO_FWRITEPROC)(const void*, size_t, size_t, FILE*);							//!< ファイル関連コールバック関数定義
typedef int (*IO_FSEEKPROC)(FILE*, long, int);													//!< ファイル関連コールバック関数定義
typedef void* (*IO_MEMORYALLOC)(size_t);														//!< ファイル関連コールバック関数定義
typedef void (*IO_MEMORYFREE)(void*);															//!< ファイル関連コールバック関数定義
typedef void (*CHECKDISKFREESPACE)(void);														//!< ファイル関連コールバック関数定義
typedef void (*INITIALIZECRITICALSECTION)( ST_CRITICAL_SECTION_INFO*, DWORD, char*, DWORD );	//!< ファイル関連コールバック関数定義
typedef void (*DELETECRITICALSECTION)( ST_CRITICAL_SECTION_INFO* );								//!< ファイル関連コールバック関数定義
typedef void (*ENTERCRITICALSECTION)( ST_CRITICAL_SECTION_INFO* );								//!< ファイル関連コールバック関数定義
typedef BOOL (*TRYENTERCRITICALSECTION)( ST_CRITICAL_SECTION_INFO* );							//!< ファイル関連コールバック関数定義
typedef void (*LEAVECRITICALSECTION)( ST_CRITICAL_SECTION_INFO* );								//!< ファイル関連コールバック関数定義
typedef ST_THREAD_INFO (*CREATETHREAD)( void*, unsigned, unsigned (__stdcall* start_address)(void*), void*, unsigned, unsigned*, char*, char*, void (__stdcall* end_handler)(void));	//!< ファイル関連コールバック関数定義
typedef void (*DELETETHREAD)( ST_THREAD_INFO* );												//!< ファイル関連コールバック関数定義
typedef void* (*GETUCONTTRANSPROCS)(void);														//!< ファイル関連コールバック関数定義
typedef HANDLE (*REGISTTHREAD)(HANDLE, DWORD);													//!< ファイル関連コールバック関数定義
typedef void (*DELETETHREAD_FORCE)( ST_THREAD_INFO* );											//!< ファイル関連コールバック関数定義
typedef int	(*IO_FPRINTFPROC)(FILE *stream, const char *format ,...);							//!< ファイル関連コールバック関数定義
typedef char* (*IO_FGETSPROC)(char *string, int n, FILE *stream);								//!< ファイル関連コールバック関数定義
typedef long (*IO_FTELLPROC)(FILE *stream);														//!< ファイル関連コールバック関数定義
typedef int	(*IO_FFLUSHPROC)(FILE *stream);														//!< ファイル関連コールバック関数定義
typedef int	(*IO_FEOFPROC)(FILE *stream);														//!< ファイル関連コールバック関数定義

typedef BOOL (*IO_WRITEPRIVATEPROFILESTRINGPROC)(LPCTSTR , LPCTSTR , LPCTSTR , LPCTSTR );		//!< ファイル関連コールバック関数定義
typedef BOOL (*IO_GETPRIVATEPROFILEINTPROC)(LPCTSTR , LPCTSTR , INT , LPCTSTR );				//!< ファイル関連コールバック関数定義
typedef BOOL (*IO_GETPRIVATEPROFILESTRINGPROC)(LPCTSTR , LPCTSTR , LPCTSTR , LPCTSTR , DWORD , LPCTSTR);//!< ファイル関連コールバック関数定義
typedef BOOL (*IO_GETPRIVATEPROFILESECTIONPROC)(LPCTSTR , LPCTSTR , DWORD , LPCTSTR );			//!< ファイル関連コールバック関数定義
typedef BOOL (*IO_GETPRIVATEPROFILESECTIONNAMESPROC)(LPCTSTR , DWORD , LPCTSTR );				//!< ファイル関連コールバック関数定義
typedef BOOL (*IO_WRITEPRIVATEPROFILESECTIONPROC)(LPCTSTR , LPCTSTR , LPCTSTR);					//!< ファイル関連コールバック関数定義

typedef long (*GETAUTOTESTMODE)();																//!< ファイル関連コールバック関数定義
typedef long (*STRATAUTOTESTMODE)();															//!< ファイル関連コールバック関数定義
typedef void (*SYSTEMSHUTDOWN)(long);															//!< ファイル関連コールバック関数定義
typedef long (*GETSYSTEMSHUTDOUWN)();															//!< ファイル関連コールバック関数定義

typedef BOOL (*FORCETODELETEFILE)(LPCTSTR);														//!< ファイル関連コールバック関数定義
typedef BOOL (*FORCETOMOVEFILE)(LPCTSTR, LPCTSTR);												//!< ファイル関連コールバック関数定義
typedef BOOL (*FORCEREMOVEDIRECTORY)(LPCTSTR);													//!< ファイル関連コールバック関数定義
typedef BOOL (*ADDSHAREDFOLDER)(char* filePath);												//!< ファイル関連コールバック関数定義
typedef BOOL (*DELETESHAREDFOLDER)(char* filePath);												//!< ファイル関連コールバック関数定義

typedef double (*GETPRIVATEPROFILEDOUBLE)( LPCTSTR, LPCTSTR, double, LPCTSTR );		//!< Log関連コールバック関数定義
typedef BOOL (*WRITEPRIVATEPROFILEDOUBLE)( LPCTSTR, LPCTSTR, double, LPCTSTR );		//!< Log関連コールバック関数定義

typedef BOOL (*ADDUSERAUTHENTICATION)(char*, char*, char*);										//!< ファイル関連コールバック関数定義
typedef long (*CHECKUSERAUTHENTICATION)(char*);													//!< ファイル関連コールバック関数定義
typedef BOOL (*DOUSERAUTHENTICATION)(char*, char*, char*);										//!< ファイル関連コールバック関数定義
typedef BOOL (*WRITEUSERAUTHENTICATIONFILE)(char*, char*, char*);								//!< ファイル関連コールバック関数定義
typedef BOOL (*READUSERAUTHENTICATIONFILE)(char*, char**, char**);								//!< ファイル関連コールバック関数定義


//! File Callback Proc Struct
typedef struct tagFILECBR
{
#define FILECBRVERSION 0
	long VERSION;													//!< バージョン
	GETMODULEDIRECTORY  CBR_GetModuleDirectory;						//!< 管理モジュールのディレクトリへのパスを取得する
	IO_MALLOCPROC		CBR_IO_malloc;								//!< メモリ確保。
	IO_FREEPROC			CBR_IO_free;								//!< CBR_IO_mallocで確保したメモリ解放。
	IO_FOPENPROC		CBR_IO_fopen;								//!< ファイルオープン。
	IO_FCLOSEPROC		CBR_IO_fclose;								//!< ファイルクローズ。
	IO_FREADPROC		CBR_IO_fread;								//!< ファイル読込。
	IO_FWRITEPROC		CBR_IO_fwrite;								//!< ファイル書込。
	IO_FSEEKPROC		CBR_IO_fseek;								//!< ファイルシーク。
	IO_MEMORYALLOC		CBR_IO_MemoryAlloc;							//!< 大きいメモリを確保する(1MB以上)
	IO_MEMORYFREE		CBR_IO_MemoryFree;							//!< CBR_IO_MemoryAllocで確保したメモリを解放する
	CHECKDISKFREESPACE	CBR_CheckDiskFreeSpace;						//!< ドライブの空き容量を取得する
	INITIALIZECRITICALSECTION	CBR_InitializeCriticalSection;		//!< クリティカルセクションを初期化し、CS管理に登録する
	DELETECRITICALSECTION		CBR_DeleteCriticalSection;			//!< クリティカルセクションを削除する
	ENTERCRITICALSECTION		CBR_EnterCriticalSection;			//!< クリティカルセクションの所有権を掴む
	TRYENTERCRITICALSECTION		CBR_TryEnterCriticalSection;		//!< クリティカルセクションの所有権を掴もうと試みる
	LEAVECRITICALSECTION		CBR_LeaveCriticalSection;			//!< クリティカルセクションの所有権を離す。
	CREATETHREAD		CBR_CreateThread;							//!< スレッドを作成し、スレッド管理に登録する
	DELETETHREAD		CBR_DeleteThread;							//!< スレッドを削除する
	REGISTTHREAD CBR_RegistThread;									//!< スレッド管理にスレッドを登録する
	DELETETHREAD_FORCE		CBR_DeleteThreadForce;					//!< スレッドを強制的に削除する(自スレッドを閉じる時用)
	IO_FPRINTFPROC		CBR_IO_fprintf;								//!< ファイルに書式付文字列を書き込む。
	IO_FGETSPROC		CBR_IO_fgets;								//!< ファイルから文字列を1行取得する
	IO_FTELLPROC		CBR_IO_ftell;								//!< ファイルポインタの現在位置を取得する
	IO_FFLUSHPROC		CBR_IO_fflush;								//!< ファイルをフラッシュする
	IO_FEOFPROC			CBR_IO_feof;								//!< eofを設定する

	IO_WRITEPRIVATEPROFILESTRINGPROC		CBR_IO_WritePrivateProfileString;		//!< INIファイルに文字列書き込み
	IO_GETPRIVATEPROFILEINTPROC				CBR_IO_GetPrivateProfileInt;			//!< INIファイルから読み込み
	IO_GETPRIVATEPROFILESTRINGPROC			CBR_IO_GetPrivateProfileString;			//!< INIファイルから読み込み
	IO_GETPRIVATEPROFILESECTIONPROC			CBR_IO_GetPrivateProfileSection;		//!< INIファイルから読み込み
	IO_GETPRIVATEPROFILESECTIONNAMESPROC	CBR_IO_GetPrivateProfileSectionNames;	//!< INIファイルから読み込み
	IO_WRITEPRIVATEPROFILESECTIONPROC		CBR_IO_WritePrivateProfileSection;		//!< INIファイルに書き込み

	GETAUTOTESTMODE		CBR_GetAutoTestMode;						//!< 自動テストモード取得
	STRATAUTOTESTMODE	CBR_StratAutoTestMode;						//!< 自動テストモード開始
	SYSTEMSHUTDOWN		CBR_SystemShutDown;							//!< システム終了依頼
	GETSYSTEMSHUTDOUWN	CBR_GetSystemShutdouwn;						//!< シャットダウン状態を取得する

	FORCETODELETEFILE	CBR_ForceToDeleteFile;						//!< ファイルを強制的に削除する
	FORCETOMOVEFILE		CBR_ForceToMoveFile;						//!< ファイルを強制的に移動する
	FORCEREMOVEDIRECTORY	CBR_ForceRemoveDirectory;				//!< フォルダを強制的に削除する
	ADDSHAREDFOLDER		CBR_AddSharedFolder;						//!< 共有フォルダのアクセス権チェックフォルダを追加する。
	DELETESHAREDFOLDER	CBR_DeleteSharedFolder;						//!< 共有フォルダのアクセス権チェックフォルダを削除する。

	GETPRIVATEPROFILEDOUBLE		CBR_GetPrivateProfileDouble;	//!< Iniファイルから浮動小数点パラメータを読み込む
	WRITEPRIVATEPROFILEDOUBLE	CBR_WritePrivateProfileDouble;	//!< Iniファイルに浮動小数点パラメータを書き込む

	ADDUSERAUTHENTICATION		CBR_AddUserAuthentication;			//!< 認証情報をパスワード管理に登録する.
	CHECKUSERAUTHENTICATION		CBR_CheckUserAuthentication;		//!< 登録済みのパスに対してユーザー認証を行う.
	DOUSERAUTHENTICATION		CBR_DoUserAuthentication;			//!< 指定データでユーザー認証を行う.
	WRITEUSERAUTHENTICATIONFILE	CBR_WriteUserAuthenticationFile;	//!< 指定ファイルに認証情報を保存する.
	READUSERAUTHENTICATIONFILE	CBR_ReadUserAuthenticationFile;		//!< 指定ファイルから認証情報を取得する.

	IO_FOPENRETRYPROC		CBR_IO_fopen_retry;		//!< リトライ機能付きファイルオープン。

}FILECBR, *LPFILECBR;

//! 管理モジュール情報
typedef struct _HOST {
	long	HOST_ID;			//!< 'PQNS'が設定される
	long	HOST_VERSION;		//!< 管理モジュールのバージョン情報
	ID_LANG	LANGID;				//!< GUI構築時の言語
	JOBCBR*	JOBPROCS;			//!< JOB関連コールバック関数
	LOGCBR*	LOGPROCS;			//!< LOG関連コールバック関数
	UICBR*	UIPROCS;			//!< GUI関連コールバック関数
	COLORCONVCBR *COLORCONVPROCS;	//!< 色変換関連コールバック関数
	RESCBR*  RESPROCS;			//!< リソース関連コールバック関数
	FILECBR* FILEPROCS;			//!< ファイル関連コールバック関数
} HOST, *PHOST;

extern JOBCBR jobCBR;				//!< JOB関連コールバック関数
extern LOGCBR logCBR;				//!< LOG関連コールバック関数
extern COLORCONVCBR colorconvCBR;	//!< 色変換関連コールバック関数
extern RESCBR resCBR;				//!< リソース関連コールバック関数
extern FILECBR fileCBR;				//!< ファイル関連コールバック関数


#endif // __COMMON_H__
