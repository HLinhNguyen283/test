/*! \file CommonDef.h
 *  \brief プリナビ　定数一覧
 *  \author TGL Sugita
 *  \date 2002/10/29 TGL Sugita 新規作成
 *  \date 2006/11/16 TGL Sugita TP344から分離。SDIJP専用ソース
 *  \date 2012/03/26 TGL Hamaguchi Saberに向けて一部定義移動.
*/

#ifndef	COMMON_DEF_H
#define	COMMON_DEF_H




#ifndef __PRNV_PRAGMA_MESSAGE__
#define __PRNV_PRAGMA_MESSAGE__
	/**
		VS2010 で #warning に近いことを実現するためのマクロ
		以下のように使う
		#pragma message ( __MSG__ "my message" )
	*/
	// stringised version of line number (must be done in two steps)
	#define STRINGISE(N) #N
	#define EXPAND_THEN_STRINGISE(N) STRINGISE(N)
	#define __LINE_STR__ EXPAND_THEN_STRINGISE(__LINE__)

	// MSVC-suitable routines for formatting <#pragma message>
	#define __LOC__ __FILE__ "(" __LINE_STR__ ")"
	#define __OUTPUT_FORMAT__(type) __LOC__ " : ■■ " type " : "

	// specific message types for <#pragma message>
	#define __WARN__ __OUTPUT_FORMAT__("warning")
	#define __ERR__ __OUTPUT_FORMAT__("error")
	#define __MSG__ __OUTPUT_FORMAT__("programmer's message")
	#define __TODO__ __OUTPUT_FORMAT__("to do")
	#define __TEST__ __OUTPUT_FORMAT__("test code")
#endif


//! プラグインフォルダ
#define PLUGIN_FOLDER _T("PLUGIN")			//!< フォルダ名：プラグイン
#define PREFS_FOLDER _T("PREFERENCES")		//!< フォルダ名：各種設定ファイル

//! インク色名.
#define INK_NAME_CYAN				_T("Cyan")			//!< インク名：Cyan
#define INK_NAME_MAGENTA			_T("Magenta")		//!< インク名：Magenta
#define INK_NAME_YELLOW				_T("Yellow")		//!< インク名：Yellow
#define INK_NAME_BLACK				_T("Black")			//!< インク名：Black
#define INK_NAME_COATING			_T("Coating")		//!< インク名：Coating
#define INK_NAME_MICR				_T("MICR")			//!< インク名：MICR

//! ファイル禁止文字列
#define	FILE_NAME_WORD	_T("\\/.<>:;\"|*?")
#define	FILE_NAME_WORD_REGEX	_T("[\\\\/\\.<>:;\"\\|\\*\\?]")		//!< 正規表現版

//! 共通の設定値
#define JOB_NAME_SIZE				256			//!< JOB名サイズ(PrintNav, JobBaseGUI, JobSelectGUI)
#define MAX_COLOR_COUNT				4			//!< 1JOBに登録できる最大色数を定義(PrintNav)
#define INVALID_MODULE_ID			0			//!< 無効なモジュールID(PrintNav)
#define MODULE_DISP_NAME_SIZE		32			//!< 表示上のプラグインGUI名(PrintNav)
#define PQNS_HOST_ID				'PQNS'		//!< ホストID(PrintNav)
#define MAX_FONTNAME				32			//!< フォント名サイズ(PrintNav)
#define DEFAULT_FONT				_T("ヒラギノ丸ゴ Pro W4")	//!< デフォルトフォント名(PrintNav)
#define DEFAULT_ENCODE				_T("x-sjis")	//!< デフォルトエンコード名(PrintNav)
#define MAX_INK_NAME_SIZE			32			//!< インキ名最大文字数(PeriodicMaintenanceGUI)
#define DEF_INK_TOTAL_COUNT			10			//!< インキタンクの数。(PeriodicMaintenanceGUI)
#define JMF_ID_SIZE_JET				32			//!< JMFでJetDriveから送信するIDサイズ。(JetInspector)
#define JMF_ID_SIZE_CENTER			52			//!< JMFでCenterから受信するIDサイズ。(JetInspector)
#define JMF_DATA_SIZE_CENTER		512			//!< JMFでCenterから受信するデータサイズ。(JetInspector)
#define MAX_NETWORK_PATH_SIZE		(_MAX_PATH + 100)	//!< ネットワークパスの最大値(マシン名の上限は１５文字・共有名の上限は８０文字由来)(PrintNav)
#define DEF_PAPER_NAME_SIZE			256			//!< 用紙名サイズ。(HeadCleaning, PaperDB)
#define DEF_PAPER_TYPE_SIZE			256			//!< 用紙種類名サイズ。(HeadCleaning)
#define JOB_SECURITY_PASSWORD_SIZE	32			//!< ジョブセキュリティパスワード最大文字数(JobSelectGUI)

#pragma warning(disable:4616)	//このワーニングはでるのが仕方ないのでカットする
#pragma warning(disable:1899)	//このワーニングはでるのが仕方ないのでカットする

//! MODULE_ID の一覧
#define MOD_PRINAVI			'PRNV'	//!< モジュールID：管理モジュール
#define MOD_STATUSBAR		'STAT'	//!< モジュールID：ステータスバー
#define MOD_STARTUP			'STUP'	//!< モジュールID：起動画面
#define MOD_LAYOUTIMAGE		'LAYI'	//!< モジュールID：レイアウト画面
#define MOD_MONITOR_MAIL	'MOML'	//!< モジュールID：モニタリングメール
#define MOD_PERIODIC_MAINT	'PERM'	//!< モジュールID：定期メンテナンスプラグ
#define MOD_JOB_SELECT_GUI	'JOBS'	//!< モジュールID：ジョブ選択画面プラグ。
#define MOD_LOG_VIEW		'LOVI'	//!< モジュールID:ログ表示画面プラグ
#define	MOD_HOT_FOLDER		'HOFO'	//!< モジュールID：JOBの入力プラグ。
#define	MOD_SAVE_JOB		'SAJO'	//!< モジュールID：JOBの保存プラグ。
#define MOD_USER_MODE		'USMO'	//!< モジュールID：操作環境設定プラグ。
#define MOD_GET_SETUP		'GESE'	//!< モジュールID：一般設定ベースプラグ。
#define MOD_MAINTENACE_GUI	'MGUI'	//!< モジュールID：特別調整画面プラグ。
#define MOD_FEEDER_PANEL	'FDPL'	//!< モジュールID：給紙パネル。
#define MOD_JOBSETUP        'JOSE'	//!< モジュールID：JobSetup
#define MOD_HEAD_CLEANING	'HECL'	//!< モジュールID: ヘッドクリーニング画面プラグ。
#define MOD_JOBBASE			'JOBA'	//!< モジュールID: 基本情報設定画面。
#define MOD_OUTPUT 			'OUTP'	//!< モジュールID：OutputPlug
#define MOD_SCANINPUT		'SCIN'	//!< モジュールID：InputPlug
#define MOD_CLIENT_TRANS	'CLTR'	//!< モジュールID：ClientTrans
#define MOD_PRINT_ORDER		'PROR'	//!< モジュールID：PrintOrder
#define MOD_JMF_CLIENT		'JMCL'	//!< モジュールID：JMF_ClientTrans
#define MOD_CONTPRINT_SETUP	'CPSE'	//!< モジュールID：連続印刷設定画面プラグ.
#define MOD_DEVELOP_TEST	'DPTE'	//!< モジュールID：開発テスト用画面プラグ.
#define MOD_REMOTE_CTRL		'RECL'	//!< モジュールID：リモート制御コントロールプラグ.
#define MOD_BARCODE         'BRCD'  //!< モジュールID : バーコード照合データ収集プラグ
#define MOD_JET_INSPECTOR   'JTIP'  //!< モジュールID：JetInspectorプラグ
#define MOD_INPUTDEV		'INDV'	//!< モジュールID：InputDevicePlug
#define MOD_PRINTPROFILEMGR 'PPMG'  //!< モジュールID：PrintProfileMgrプラグ
#define MOD_SV_COMMAND_ANA	'SVCA'	//!< モジュールID：SVコマンド解析プラグ
#define MOD_SV_TRANS		'SVTR'	//!< モジュールID：SV通信プラグ
#define MOD_SV_COOPERATION	'SCOP'	//!< モジュールID：SV110連携プラグ.
#define MOD_OCR_DECODE		'OCRD'	//!< モジュールID：OCRデコード画面プラグ.
#define MOD_TIFF_MANAGER	'TIFM'	//!< モジュールID：Tiff管理プラグ.
#define MOD_PRINT_START		'PRST'	//!< モジュールID：刷り出し速度プラグ.
#define MOD_RPPS_TRANS		'RPTR'	//!< モジュールID：RPPS通信プラグ.
#define MOD_GEN_DUMMY		'GDMY'	//!< モジュールID：一般設定画面ダミー画面プラグ.
#define MOD_OBSTACLE_INFO	'OBIF'	//!< モジュールID：障害情報監視プラグ.
#define MOD_MAINTE_FIRMWARE	'MFRM'	//!< Module ID : firmware screen.
#define MOD_ROLLEXCHANGE_GUI	'RolE'	//!< モジュールID: ロール交換画面.
#define MOD_PAPER_FEED				'PPFD'	//!< モジュールID: 用紙搬送画面.
#define MOD_MAINTE_MOTOR_ACTION		'MMTA'	//!< Module ID: motor action screen
#define MOD_MAINTE_INK_HEAD			'MINK'		//!< Module ID: ink head screen
#define MOD_MAINTE_PRINT_IO			'MPIO'	//!< Module ID: print IO screen
#define MOD_MAINTE_PRINT_PARAMETER	'MPPM'	//!< Module ID: print parameter screen
#define MOD_MAINTE_INK_CARTRIDGE	'MINP'		//!< Module ID: ink pouch screen
#define MOD_MAINTE_JET_INSPECTION	'MJIN'	//!< Module ID: jet inspection screen
#define MOD_MAINTE_PRINT_ADJUSTMENT 'MPAD'	//!< Module ID: print adjustment screen
#define MOD_MAINTE_BARCODE_READER	'MBCR'	//!< Module ID: barcode reader screen
#define MOD_FEEDING_ADJUSTMENTS		'FDAD'	//!< Module ID: feeding adjustments screen
#define MOD_MAINTE_CCD_MONITOR 'MCCD'	//!< Module ID: CCD Monitor
#define MOD_PAPER_DB		'PADB'	//!< Module ID: PaperDB
#define MOD_PRINTER_STATE		'PTSA'	//!< Module ID: PrinterState.
#define MOD_INK_SUPPLY		'ISPY'	//!< Module ID: Ink Supply screen.
#define MOD_JOB_MANAGER		'JBMG'	//!< Module ID: JobManager
#define MOD_MANUAL_SHADING	'MNSD'	//!< Module ID: Manual Shading.
#define MOD_MANUAL_LEVELTUNE	'MNLT'	//!< Module ID: Manual LevelTune.
#define MOD_PRINT_CONDITION	'PTCN'	//!< Module ID: Print Condition screen.
#define MOD_CIS_ADJUSTMENT	'CISA'	//!< Module ID: CIS adjustment screen
#define MOD_TONE_SETTINGS	'TCST'	//!< Module ID: Tone setting screen
#define MOD_PRINT_UNIT		'PRUN'	//!< Module ID: PrintUnitAdjustment.
#define MOD_HEAD_ADJUST_DATAFILE	'HADF'	//!< Module ID : HeadAdjustDataFile
#define MOD_PRINT_MONITOR	'PTMO'	//!< Module ID: Print monitor screen.
#define MOD_CENTER_TRANS 'CtTr'		//!< Module ID : CenterTrans
#define MOD_HEAD_POS	'HDPS'		//!< Module ID: HeadPosAdjustment.
#define MOD_MIST_FILTER	'MFLT'		//!< Module ID: MistFilter.
#define MOD_PRINT_DENSITY	'PDEN'	//!< Module ID: Print density screen.
#define MOD_IBT_LOG	'IBTL'			//!< Module ID: IBT log manager
#define MOD_IPCODE	'IPCD'			//!< Module ID: IPCode
#define MOD_CLIENT_CIT_TRANS	'CITT'	//!< Module ID: Client CIT Trans.
#define MOD_JOB_PRINT_SEQUENCE	'JPSQ'	//!< Moduke ID: Job Print Sequence.
#define MOD_JOBLOG	'JLMN'			//!< Moduke ID: Job Log Manager
#define MOD_PRINT_HIST_LOG	'PTHL'	//!< Moduke ID: Print history log
#define MOD_CLIENT_RIP_JD_TRANS	'CRJT'	//!< Module ID: Client Rip-JD Trans.
#define MOD_MAINTE_SHADING_GUI		'MTSD'	//!< Module ID: Alignment GUI for 520HD.
#define MOD_MAINTE_ALIGNMENT_GUI	'MTAL'	//!< Module ID: Alignment GUI for 520HD.
#define MOD_PRESSERR_MNG	'PEMG'	//!< モジュールID：エラー警告管理プラグ.
#define MOD_WEB_UP			'WEUP'	//!< Module ID: web up screen.
#define MOD_SPOT_COLOR_CHART	'SPCC'	//!< Module ID : Spot Color Chart(特色チャート印刷)
#define MOD_ACCUMULATION_LIGHT	'ACLT'	//!< Module ID : 累積点灯表示画面 (L350)
#define MOD_DNS_SETTINGS		'DNSS'	//!< Module ID : DNS Settings GUI
#define MOD_MICR_SETTINGS		'MICR'	//!< Module ID : MICR Settings GUI
#define MOD_JOB_PRINT_HELPER	'JPHL'	//!< Module ID : Job Print Helper
#define MOD_EQ_LICENSE			'EQLC'	//!< Module ID : EGライセンスプラグ.
#define MOD_EXTRIP_TRANS	'ERTR'	//!< モジュールID：外部RIP(IPDS)通信プラグ(ZZ/HD)
#define MOD_JI_EVENT_MNG		'JIEM'	//!< Module ID: JIEventManager.
#define MOD_AUTO_ECO_MODE		'AECO'	//!< Module ID: AutoEcoMode.
#define MOD_PRE_HEATER			'PRHT'	//!< Module ID: PreHeater.
#define MOD_IR_SETTINGS			'NIRS'	//!< Module ID: N-IR Settings GUI.
#define MOD_SAVE_MONITOR_IMAGE	'SVMI'	//!< Module ID：印刷モニター画像保存(IPDS)プラグ
#define MOD_MODE_CHANGE_GUI		'MDCE'	//!< Module ID：動作モード切替画面(HD)
#define MOD_INK_CODE_MANAGER	'INCM'	//!< Module ID：インクコード管理
#define MOD_CLICK_CHARGE_OF_JOB_PRINT	'CCJP'	//!<  Module ID：ジョブ印刷用クリックチャージプラグ
#define MOD_CLICKCHARGE			'PRCT'	//!< Module ID: Click charge.
#define MOD_AUTO_SPLICE			'ATSP'	//!< Module ID：オートスプライス
#define MOD_PRINT_CONDITION_IMPORTER	'PRCI'	//!< Module ID:印刷条件インポートプラグ(HD)
#define MOD_PRINT_CONDITION_EXPORTER	'PRCE'	//!< Module ID:印刷条件エクスポートプラグ(HD)
#define MOD_TWEAK				'TWEA'	//!< Module ID：Tweak
#define MOD_SUB_HEAT_ROLLER		'SRHT'	//!< Module ID: サブヒートローラー設定

#pragma warning(default:1899)
#pragma warning(default:4616)

//! エラーコード
enum ENUM_ERR_CODE {
    ERR_PQNS_BASE					= 0,		//!< 管理モジュール用 0 ～ 999
	ERR_STARTUP						= 1000,		//!< 起動画面用 1000 ～ 1099
	obs_ERR_HOT_FOLDER				= 1100,		//!< JOBの入力プラグ用 1100 ～ 1199
	ERR_SAVE_JOB					= 1200,		//!< JOBの保存プラグ用 1200 ～ 1299
	ERR_USER_MODE					= 1300,		//!< 操作環境設定プラグ用 1300 ～ 1399
	obs_ERR_GET_SETUP				= 1400,		//!< 一般設定ベースプラグ用 1400 ～ 1499
	ERR_LOG_VIEW					= 1500,		//!< ログ表示画面 1500 ～ 1599
	ERR_PERIODIC_MAINT				= 1600,		//!< 定期メンテナンス 1600 ～ 1699
	ERR_LOG_ARCHIVE					= 1700,		//!< ログ回収プラグ 1700 ～ 1799
	obs_ERR_MONITORMAIL_GUI_BASE	= 1800,		//!< リモートモニタリングメール送信GUIプラグ用 1800～1899
	obs_ERR_MAINTENANCE_GUI			= 1900,		//!< 特別調整画面 1900～1999
	obs_ERR_FEEDER_PANEL			= 2000,		//!< 給紙パネル 2000～2499
	//次は2500.
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_JOBSETUP					= 2500,		//!< JOBセットアップ画面共通部 2500～2599
	ERR_MAINTE_INK_HEAD				= 2600,		//!< ヘッドメンテナンス画面 2600～2699
//#pragma message("■■■リファクタリング後に削除。新規に500件程度確保する■■■")
	ERR_JOB_SELECT					= 2700,		//!< JOB選択画面 2700～2799
//#pragma message("■■■リファクタリング後に削除。新規に500件程度確保する■■■")
	ERR_STATUSBAR_BASE				= 2800,		//!< ステータスバー用 2800 ～ 2899
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_JOBBASE						= 2900,		//!< 基本情報設定画面 2900 ～ 2999
	ERR_MAINT_MOTOR_ACTION			= 3000,		//!< モーター動作 3000 ～ 3099
	ERR_MAINT_FIRMWARE				= 3100,		//!< ファームウェア画面 3100 ～ 3199
	ERR_MAINT_PRINT_PARAM			= 3200,		//!< CanNodeパラメーター画面 3200～3299
	ERR_LAYOUTIMAGE					= 3300,		//!< レイアウトイメージモジュール用 3300 ～ 3399
	ERR_MAINT_PRINT_IO				= 3400,		//!< CanNodeIO画面 3400～3499
	ERR_OUTPUT						= 3500,		//!< 出力プラグ 3500～3599
	ERR_INPUT						= 3600,		//!< 入力プラグ 3600～3699	
	ERR_MAIN_SERVER					= 3700,		//!< メインサーバー 3700～3999
	ERR_SUB_SERVER					= 4000,		//!< サブサーバー 4000～4299
	ERR_CLIENT_TRANS				= 4300,		//!< サーバー通信プラグ用 4300～4499
	ERR_PRINT_ORDER					= 4500,		//!< 印刷情報作成プラグ 4500～4599
	ERR_JMF_CLIENT_TRANS			= 4600,		//!< JMF通信プラグ用 4600～4699
	obs_ERR_CONTPRINT_SETUP			= 4700,		//!< 連続印刷設定画面プラグ用 4700～4799
	ERR_DEVELOP_TEST				= 4800,		//!< 開発テスト用画面プラグ用 4800～4899
	obs_ERR_MAINT_PRINT_TIMING		= 4900,		//!< 紙頭検出位置調整画面プラグ用 4900～4999
	ERR_BARCODE						= 5000,      //!< バーコード照合データ収集プラグ用 5000～5099
	ERR_JET_INSPECTOR				= 5100,		//!< JetInspectorプラグ用 5100～5199
	ERR_INPUT_DEVICE				= 5200,		//!< 入力デバイスプラグ用 5200～5299
	ERR_PRINTPROFILEMGR				= 5300,		//!< PrintProFileプラグ用 5300～5399
	obs_ERR_SV_TRANS				= 5400,		//!< SV通信プラグ用 5400～5499
	ERR_PRINT_START_SPEED			= 5500,		//!< ZZ刷り出し速度指定 5500～5599
	ERR_RPPS_TRANS					= 5600,		//!< RPPS通信プラグ用 5600～5699
	ERR_IFSPK						= 5700,		//!< IFSPK用 5700～5799
	ERR_INK_CARTRIDGE				= 5800,		//!< インクカートリッジ情報画面 5800～5899
	ERR_MAINTE_JETINSPECTION		= 5900,		//!< JetInspection調整画面 5900～5999
	obs_ERR_MAINTE_PRINTER_ZZ		= 6000,		//!< Mainte_PrinterZZ用 6000～6099
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_LAYOUTIMAGE_OP				= 6100,		//!< レイアウトイメージモジュール用(共通部用) 6100 ～ 6199
	obs_ERR_LAYOUTIMAGE_BASE3			= 6200,		//!< レイアウトイメージモジュール用(JI専用) 6200 ～ 6299
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_STATUSBAR_BASE2				= 6300,		//!< ステータスバー用 6300 ～ 6399
	obs_ERR_USER_MODE2				= 6400,		//!< 操作環境設定プラグ用(共通部用) 6400 ～ 6499
	obs_ERR_TIFFMANAGER				= 6500,		//!< Tiff管理プラグ用 6500 ～ 6599
	obs_ERR_STARTUP2				= 6600,		//!< 起動画面用(共通部) 6600 ～ 6699
	obs_ERR_MAINTENANCE_GUI2		= 6700,		//!< 特別調整画面 6700～6799
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_JOBSETUP2					= 6800,		//!< JOBセットアップ画面(共通部) 6800～6999
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_JOBBASE2					= 7000,		//!< 基本情報設定画面(共通部) 7000 ～ 7099
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_JOB_SELECT_OP				= 7100,		//!< JOB選択画面(共通部) 7100～7199
	obs_ERR_PRINT_ORDER2			= 7200,		//!< 印刷情報作成プラグ 7200～7299
	obs_ERR_MONITOR_MAIL_OP			= 7300,		//!< リモートモニタリングメールプラグOP専用 7300 ～ 7399
	ERR_BCR							= 7400,		//!< バーコードリーダー画面用 7400 ～ 7499
	ERR_MONITORING_DISK				= 7500,		//!< 障害情報監視ツール 7500 ～ 7599
	ERR_OCR_DECODE					= 7600,		//!< OCRデコード画面 7600 ～ 7699
//#pragma message("■■■リファクタリング後に削除■■■")
	ERR_STATUSBAR_OP2				= 7700,		//!< ステータスバー用 7700 ～ 7799
	//
	ERR_HEAD_CLEANING				= 7800,		//!< クリーニング画面 7800 ～ 7899
	ERR_PAPER_FEED					= 7900,		//!< 用紙搬送画面 7900 ～ 7999
	ERR_INK_SUPPLY					= 8000,		//!< インク供給画面 8000 ～ 8099
	ERR_ROLLEXCHANGE		 		= 8100,		//!< ロール交換画面 8100 - 8199
	ERR_JOB_MANAGER		 			= 8200,		//!< ジョブ管理 8200 - 8699
	ERR_PAPER_DB		 			= 8700,		//!< 紙DB管理 8700 - 8799
	ERR_PRINTER_STATE	 			= 8800,		//!< 印刷機状態管理 8800 - 8899
	ERR_CONTROL_ERR					= 8900,		//!< ControlErr.ini 8900-9099
	ERR_MANUAL_SHADING_ERR			= 9100,		//!< 手動シェーディング画面 9100-9199
	ERR_FEEDING_ADJUSTMENT			= 9200,		//!< 搬送調整画面 9200 - 9299
	ERR_MANUAL_LEVELTUNE_ERR		= 9300,		//!< 手動段差補正画面 9300 - 9399
	ERR_HEAD_CLEANING_L350_ERR		= 9400,		//!< ヘッドクリーニング画面(L350用) 9400 - 9499
	ERR_PRINT_CONDITION				= 9500,		//!< L350用の印刷条件画面 9500-9599
	ERR_CIS_ADJUSTMENT				= 9600,		//!< CIS調整画面 9600-9699
	ERR_STATUSBAR_NEW				= 9700,		//!< ステータスバー 9700-9999
	ERR_JOBBASE_NEW					= 10000,	//!< 基本情報設定画面 10000-10199
	ERR_LAYOUTIMAGE_NEW				= 10200,	//!< レイアウト画面 10200-10399
	ERR_SIS_REMOTE_CTRL	   			= 10400,	//!< SISリモコン 10400-10499
	ERR_PRINT_UNIT					= 10500,	//!< 印刷ユニット調整画面 10500 - 10599
	ERR_TONE_SETTINGS				= 10600,	//!< トーン調整画面 10600-10699
	ERR_PRINTPROFILEMGR_L350		= 10700,	//!< プロファイル管理 10700-10799
	ERR_HEADADJUSTDATAFILE_L350		= 10800,	//!< 調整データ作成モジュール 10800-10899
	ERR_HEADADJUSTDATAFILE_HD		= 10900,	//!< 調整データ作成モジュール 10900-10999	
	ERR_PRINT_MONITOR				= 11000,	//!< 印刷モニター画面 11000-11099
	ERR_EXPORTPAPERDB				= 11100,	//!< 用紙設定エクスポートプラグ 11100 - 11199
	ERR_HEAD_POS_ADJUST				= 11200,	//!< ヘッド上下位置調整(L350UV用) 112000 - 11299
	ERR_MIST_FILTER					= 11300,	//!< ミストフィルター(L350UV用) 111300 - 11399
	ERR_PRINT_DENSITY				= 11400,	//!< 印刷濃度管理画面 11400 - 11499
	ERR_JOB_SELECT_NEW				= 11500,	//!< ジョブ選択画面 11500 - 11699
	ERR_PRINT_CONDITION_SABER		= 11700,	//!< Saber用の印刷条件画面 11700-11799
	ERR_PRINT_HEAD_UNIFORM			= 11800,	//!< 印刷濃度均一化画面 11800-11899
	ERR_IBT_LOG_MANAGER				= 12000,	//!< IBTログ管理 12000-12099(L350/HDで共通)
	ERR_CENTERTRANS					= 12100,	//!< CenterTrans 12100 - 12199
	ERR_SYNC_LINE_SETTING			= 12200,	//!< ライン同期プラグ 12200 - 12299(ZZ用)
	ERR_IPCODE						= 12300,	//!< Error ID for IPCode.dll
	ERR_CLIENT_CITTRANS				= 12400,	//!< Client CIT通信 12400 - 12499(HD用)	
	ERR_JOB_PRINT_SEQUENCE			= 12500,	//!< ジョブ印刷シーケンスプラグ 12500 - 12599(HD用)
	ERR_CLIENT_RIPJDTRANS			= 12600,	//!< Client Rip-JD通信 12600 - 12699(HD用)	
	ERR_PRINTPROFILEMGR_HD			= 12700,	//!< Error ID for PrintProfileMgr_HD.dll 12700 - 12799 (HD)
	ERR_MAINTE_SHADINGGUI			= 12800,	//!< Error ID for Mainte_ShadingGUI.dll 12800 - 12899 (HD)
	ERR_MAINTE_ALIGNMENTGUI			= 12900,	//!< Error ID for Manual_AdjustmentGUI.dll 12900 - 12999 (HD)
	ERR_PRESSERR_MNG				= 13000,	//!< エラー警告管理プラグ 13000 - 13099(HD用)
	ERR_JOBLOG_MANAGER				= 13100,	//!< JobLogManager (13100 - 13199)(HD用)
	ERR_WEB_UP						= 13200,	//!< WebUp screen 13200-13299(HD用)
	ERR_SPOT_COLOR_CHART			= 13300,	//!< Spot Color Chart(特色チャート印刷) 13300-13399(HD用)
	ERR_ACCUMULATION_LIGHT			= 13400,	//!< 累積点灯時間表示画面 (L350用)
	ERR_DNS_SETTINGS				= 13500,	//!< DNS Settings 13500-13599(HD)
	ERR_MICR_SETTINGS				= 13600,	//!< MICR Settings 13600-13699(HD)
	ERR_EXTERNALRIP					= 13700,	//!< ExternalRIP 13700 - 13799(ZZ/HD)
	ERR_EXTRIPTRANS					= 13800,	//!< ExtRIPTrans 13800 - 13899(ZZ/HD)
	ERR_CLIENT_TRANS_ZZ			    = 13900,	//!< サーバー通信プラグZZ用 13900 - 13999(ZZ)
	ERR_VOICE_INSTRUCTION			= 14000,	//!< Voice instruction screen(L350/HD)
	ERR_JOB_PRINT_HELPER			= 14100,	//!< Job Print Helper 13700-13799(HD)
	ERR_JOB_INK_AMOUNT_PREDICT		= 14200,	//!< Job Ink Amount Predict(L350/将来的には他機種も)
	ERR_JOBMNLSHD_GUI				= 14300,	//!< JOBManualShadingGUI Error(L350)
    ERR_JOBMNLSHD					= 14400,	//!< JOBManualShading Error(L350)
	ERR_HEADADJUSTDATAFILE_NX		= 14500,	//!< 調整データ作成モジュール for NX 14500-14599
	ERR_PRE_HEATER					= 14600,	//!< プレヒートプラグ(NX)
	ERR_AUTO_ECO_MODE				= 14700,	//!< Auto Eco Mode 14700 - 14799(NX)
	ERR_JI_EVENT_MANAGERE			= 14800,	//!< JIイベント管理プラグ 14800 - 14899(NX)
	ERR_EQLICENSE					= 14900,	//!< EGライセンスプラグ 14900 - 14999(NX)
	ERR_HEADINFO					= 15000,	//!< HeadInfo.dll 15000 - 15099(NX)
	ERR_PRINT_CONDITION_IMPORTER	= 15100,	//!< 印刷条件インポートプラグ15100 - 15199(NX)
	ERR_PRINT_CONDITION_EXPORTER	= 15200,	//!< 印刷条件エクスポートプラグ15200 - 15299(NX)
	ERR_TIMER_SETTING_GUI			= 15300,	//!< Timer Setting GUI 15300 - 15399(L350)
	ERR_MAINTE_HEADOVERLAP			= 15400,	//!< Mainte_HeadOverlap.dll 15400 - 15499(NX)
	ERR_EQUIOS_COOPERATION			= 15500,	//!< EQUIOS Cooperation 15500 - 15599(L350)
	ERR_SYSTEM_CONFIG_CHANGE		= 15600,	//!< SystemConfigChange.dll 15600 - 15699(NX)
	ERR_MAINTE_FAN_SPEED			= 15700,	//!< Mainte_FanSpeed.dll 15700 - 15799(NX)
	ERR_AUTO_SPLICE					= 15800,	//!< AutoSpliceプラグ 15800 - 15899(ZZ用)
	ERR_HEAD_CONNECTION_CHECKER		= 15900,	//!< HeadConnectionChecker.dll 15900 - 15999(NX)
	ERR_IR_SETTINGS					= 16000,	//!< IR Settings 16000-16099(HD)
	ERR_MODE_CHANGE_GUI				= 16100,	//!< 動作モード切替画面 16100 - 16199(HD)
	ERR_SAVE_MONITOR_IMAGE			= 16200,	//!< SavePrintMonitorImage.dll 16200 - 16299(NX)
	ERR_INK_CODE_MANAGER			= 16300,	//!< インクコード管理 16300 - 16399(HD)
	ERR_EXECUTION_CONTROL_OF_PRIS	= 16400,	//!< ExecutionControlOfPriS.dll 16400 - 16499(NX)
	ERR_CLICK_CHARGE			    = 16500,	//!< ClickCharge.dll 16500 - 16599(HD)
	ERR_AUTO_JOB_INK_AMOUNT_PREDICT	= 16600,	//!< Auto Job Ink Amount Predict 16600 - 16699(L350)
	ERR_BE_HEAD_CLEANING_CHECKER	= 16700,	//!< BEHeadCleaningChecker.dll (16700 - 16799) (NX)
	ERR_TWEAK						= 16800,	//!< Tweakプラグ 16800 - 16899(HD/NX)
	ERR_SUB_HEAT_ROLLER				= 16900,	//!< サブヒートローラー設定プラグ 16900 - 16999(HD)
	//次は17000.
	//★メッセージを追加した場合、他機種にも同様に追加すること。
	//他機種：ZZ、L350、HD、NX

	ERR_EQUIOS_BASE					= 50000		//!<  EQUIOS本体用 50000～ 
};

//! プラグインエラー用設定値
enum {
	PLUGERR_NONE = 0,			//!< エラーなし
	PLUGERR_CREATE_BUF,			//!< バッファ作成関連エラー
	PLUGERR_READ_BUF,			//!< バッファ読込み関連エラー
	PLUGERR_WRITE_BUF,			//!< バッファ書込み関連エラー
	PLUGERR_OPEN_FILE,			//!< ファイルオープンエラー
	PLUGERR_READ_FILE,			//!< ファイル読込みエラー
	PLUGERR_WRITE_FILE,			//!< ファイル書込みエラー
	PLUGERR_LOAD_DLL,			//!< DLLロードエラー
	PLUGERR_GETPROC_DLL,		//!< DLL関数取得エラー
	PLUGERR_INIT_ALREADY,		//!< すでに初期化されているものに2回目の初期化を行った
	PLUGERR_NOT_INIT,			//!< 未初期化エラー
	PLUGERR_END_ALREADY,		//!< すでに終了しているものに2回目の終了を行った
	PLUGERR_PROC_ALREADY,		//!< すでに実行したものに2回目の実行を行った
	PLUGERR_INVALID_STRUCT,		//!< 無効な構造体
	PLUGERR_STOP_PROCESS,		//!< その他プロセスをストップする必要があるエラー
	PLUGERR_USING_OUTPUT,		//!< 出力ファイルは使用中
	PLUGERR_INVALID_PARAM,		//!< パラメータが無効
	PLUGERR_ALREADY_EXEC,		//!< 指定された入出力機のJOBはすでに実行されている
	PLUGERR_WARNING,			//!< その他のワーニング
	PLUGERR_FORCE_JOBSTOP,		//!< プラグインから強制的にJOBを停止したい場合のエラー
	PLUGERR_RUN_END,			//!< JOB実行を正常に終了する(JOBSに保存)(ステータスバー以外使用禁止)
	PLUGERR_RUN_ALL_END,		//!< JOB実行を正常に終了する(JOBSに保存)(連続実行しない)(ステータスバー以外使用禁止)
	PLUGERR_RUN_SELECT_END,		//!< 指定JOBのJOB実行を正常に終了する
	PLUGERR_PROC_RETRY,			//!< 同関数をもう一度コールすることを依頼した.(PIM_InitSystem/PIM_ExitOuterSystem以外使用禁止)
	PLUGERR_CANCEL				//!< GUIのキャンセルボタンが押下された
};

//! UI_Process戻り値用設定値
enum {
	UIPROC_LOOPOUT			= 0,		//!< メッセージループが不要となった
	UIPROC_NEED_PROC		= 1,		//!< メッセージ処理が必要
	UIPROC_SAVE_ERROR		= 2		//!< UWM_PLUGIN_PAGE_SAVEでエラー発生
};

//! UI_TYPE 用設定値
enum {
	UI_NONE				= 0,				//!< GUIなし
	UI_GENPREFS			= 1 << 1,			//!< 一般設定用GUI
	UI_JOBSETUP			= 1 << 2,			//!< JOB設定用GUI
	UI_STARTUP			= 1 << 3,			//!< 起動画面
	UI_STATUSBAR		= 1 << 4,			//!< ステータスバー
	UI_FEEDER_PANEL		= 1 << 5,			//!< 給紙パネル
	UI_JOB_SELECT		= 1 << 6,			//!< JOB選択画面
	UI_LOG_VIEW			= 1 << 7,			//!< ログ画面
	UI_INVALID_SHOW_JOBRUNNING	= 1 << 8,	//!< JOB実行中に表示不可能な画面
	UI_SERVICE_SHOW_JOBRUNNING	= 1 << 9,	//!< JOB実行中にサービス以外は表示不可能な画面
	UI_CALL_PROC_BACKGROUND = 1 << 10,		//!< ページ非表示時にもメッセージプロシージャをコールする
	UI_INVALID_SHOW_PRINTERRUNNING = 1<<11,	//!< 印刷動作中に表示不可能な画面.
	UI_JOBRUN_SETUP		= 1 << 12,			//!< 実行中ジョブの設定画面
	UI_DUMMY			= 1 << 13,			//!< 一般設定画面ダミー画面.
	UI_MAINTENACE		= 1 << 14,			//!< メインテナンス系画面.
	UI_WAKEUP			= 1 << 15,			//!< ウェイクアップの必要な画面.
	UI_EMERGENCY		= 1 << 16,			//!< 画面入退時に緊急停止発行が必要な画面.
	UI_INVALID_SHOW_AGITATIONRUNNING = 1 << 17,	//!< 攪拌中に表示不可能な画面
	UI_INVALID_SHOW_POWER_SAVING = 1 << 18,	//!< 省電力中に表示不可能な画面
	UI_INVALID_SHOW_PRPS_RUNNING = 1 << 19,	//!< 印刷準備開始中に表示不可能な画面
	UI_ENTER_FORCE_DOWNLOAD = 1 << 20,		//!< 強制ダウンロード中に遷移可能な画面
	UI_INVALID_SHOW_EXCEPT_OFFLINE_POWERSAVING = 1 << 21,	//!< オフライン、停止中、省電力中以外に表示不可能な画面
	UI_CHECK_ONLY_SELF_PRINTER_STATUS	= 1 << 22,	//!< 自機の印刷機ステータスのみを参照する
	UI_INVALID_SHOW_PAPERFEEDING = 1 << 23,	//!< screen can not be displayed during paper transport
	UI_INVALID_SHOW_CLEANNING	 = 1 << 24	//!< screen can not be displayed during head cleaning
};

//! プラグイン・モジュールの種類
enum {
	PIM_PROCESS,	//!< 演算モジュール
	PIM_UI			//!< GUI モジュール
};

//! JOBの状態一覧
enum ENUM_JOB_STATUS{
    STATUS_NONE = 0,		//!< 状態なし
    STATUS_PRINTING,		//!< 印刷中
    STATUS_FORCE_STOP,		//!< 強制停止中
    STATUS_COMPLETE,		//!< 完了
	STATUS_WARN,			//!< 警告
    STATUS_ERROR			//!< エラー（未使用）
};


//! 登録済みのパスに対してのユーザー認証結果.
enum {
	USERAUTHENTICATION_OK = 0,	//!< 認証成功
	USERAUTHENTICATION_NG,		//!< 認証失敗
	USERAUTHENTICATION_NODATA	//!< 認証情報未登録
};

//! 言語コード
typedef enum _ID_LANG {
	ID_ENGLISH	= 0,			//!< 英語
	ID_JAPANESE	= 1,			//!< 日本語
	ID_FRENCH,					//!< フランス語
	ID_GERMAN,					//!< ドイツ語
	ID_ITALIAN,					//!< イタリア語
	ID_PORTUGUESE,				//!< ポルトガル語
	ID_SPANISH,					//!< スペイン語
	ID_CHINESE,					//!< 中国語
	ID_KOREAN,					//!< 韓国語
	ID_DUTCH,					//!< オランダ語
	ID_POLISH,					//!< ポーランド語
	ID_DEFAULT	= ID_ENGLISH	//!< デフォルト値：英語
} ID_LANG;

//! グループボックスの種類
enum {
	GBOX_PRIMARY		= 0,		//!< 未使用：一次グループ
	GBOX_SECONDARY,					//!< 未使用：二次グループ
	GBOX_IMAGEWELL,					//!< 未使用：画像表示用ボックス
	GBOX_HIDDEN,					//!< 未使用：非表示ボックス
	GBOX_RADIOGROUP		= 0x10000	//!< 未使用：
};

//! ヒント画面タイプ
enum
{
	HINT_NORMAL = 0,			//!< 吹出しなし
	HINT_TOPLEFT,				//!< 吹出し左上
	HINT_TOPRIGHT,				//!< 吹出し右上
	HINT_BOTTOMLEFT,			//!< 吹出し左下
	HINT_BOTTOMRIGHT			//!< 吹出し右下
};

//! 進捗ダイアログタイプ 
#if defined(_WIN32_WINNT)
#if _WIN32_WINNT < 0x0501
enum
{
	PROGDLG_NORMAL				= 0,	//!< メッセージのみ表示
	PROGDLG_PROGRESS_BAR		= 1,	//!< プログレスバーを表示
	PROGDLG_MSG_MULTILINE		= 2,	//!< テキストを複数行モードで表示
	PROGDLG_MSG_CENTER			= 4,	//!< テキストを中央に表示
	PROGDLG_WAIT_DISPLAY		= 8,	//!<  時間と進捗の度合いにより表示を調整する
	PROGDLG_MODALMODE   		=16,	//!< モーダルでキャンセルボタンなし
	PROGDLG_BACKGROUND			=32,	//!< バックグラウンドボタン付き
	PROGDLG_INDEFINITE_BAR		=64,	//!< 不定値表示プログレスバーを表示
	PROGDLG_FILLED_FILTER_HOLE	=128	//!< フィルターウィンドウの穴をふさぐ
};
#else

enum
{
	PROGDLG_PROGRESS_BAR		= 1,	//!<  プログレスバーを表示
	PROGDLG_MSG_MULTILINE		= 2,	//!<  テキストを複数行モードで表示
	PROGDLG_MSG_CENTER			= 4,	//!<  テキストを中央に表示
	PROGDLG_WAIT_DISPLAY		= 8,	//!<  時間と進捗の度合いにより表示を調整する
	PROGDLG_MODALMODE			=16,	//!<  モーダルでキャンセルボタンなし
	PROGDLG_BACKGROUND			=32,	//!< バックグラウンドボタン付き
	PROGDLG_INDEFINITE_BAR		=64,	//!< 不定値表示プログレスバーを表示
	PROGDLG_FILLED_FILTER_HOLE	=128	//!< フィルターウィンドウの穴をふさぐ
};
#endif //_WIN32_WINNT < 0x0501
#else

#ifndef PROGDLG_NORMAL
#define PROGDLG_NORMAL 0
#endif

enum
{
	PROGDLG_PROGRESS_BAR	= 1,	//!<  プログレスバーを表示
	PROGDLG_MSG_MULTILINE	= 2,	//!<  テキストを複数行モードで表示
	PROGDLG_MSG_CENTER		= 4,	//!<  テキストを中央に表示
	PROGDLG_WAIT_DISPLAY	= 8,	//!<  時間と進捗の度合いにより表示を調整する
	PROGDLG_MODALMODE		=16,	//!<  モーダルでキャンセルボタンなし
	PROGDLG_BACKGROUND		=32,	//!< バックグラウンドボタン付き
	PROGDLG_INDEFINITE_BAR	=64		//!< 不定値表示プログレスバーを表示
};
#endif //defined(_WIN32_WINNT)

//! 進捗ダイアログ設定モード
enum
{
	PROGDLG_SET_VALUE	= 1,	//!< プログレスバーの値を設定
	PROGDLG_SET_MESSAGE	= 2,	//!< メッセージを設定
	PROGDLG_SET_MAXVALUE	= 4	//!< プログレスバーの最大値を設定 
};

//! ログ出力形式
enum {
	LOG_STATUS = 0,		//!< 通常の記録
	LOG_ERROR,			//!< エラー情報
	LOG_WARNING,		//!< 警告情報
	LOG_DEBUG,			//!< デバッグ情報
	LOG_HTML,			//!< HTML 直接出力
	LOG_OPERATION		//!< オペレーション情報
};

//! RGB/CMYK変換値
typedef struct _InputRGBandCMYK {
	short alpha;				//!< チャンネル：アルファ
	short red;					//!< チャンネル：レッド
	short green;				//!< チャンネル：グリーン
	short blue;					//!< チャンネル：ブルー
	unsigned char cyan;			//!< チャンネル：シアン
	unsigned char magenta;		//!< チャンネル：マゼンタ
	unsigned char yellow;		//!< チャンネル：イエロー
	unsigned char black;		//!< チャンネル：ブラック
} InputRGBandCMYK;

//! 色空間
enum { 
	PQNS_INPUTRGB = 0,			//!< プリナビ標準入力RGB
								// ・16ビットピクセル順次αＲＧＢ：short aRGB[4]の配列 
								// ・ピクセルは上から下に並び、左上を原点とする 
	PQNS_DISPLAYRGB,			//!< 表示用RGB
								// ・8ビットピクセル順次ＢＧＲｘ：unsigned char RGBx[4]の配列 
								// ・ピクセルは下から上に並び、左下を原点とする 
								// ・各行は32ビット境界（DWORD aligned）とする 
	PQNS_CMYK,					//!<  8ビットピクセル順次ＣＭＹＫ 
								// ・unsigned char CMYK[4]の配列 
								// ・ピクセルは上から下に並び、左上を原点とする 
//	PQNS_DENSITY,				//!< ＹＭＣ濃度 
								// ・float YMC[3]の配列 
	PQNS_GRAY,					// グレースケール
								// ・8bit 1Ch
	PQNS_INPUT_RGB_AND_CMYK,	//!< プリナビ標準入力RGBとCMYKの組み合わせ 
	PQNS_INPUTRGB_NO_ALPHA		//!< プリナビ標準入力RGB
								// ・16ビットピクセル順次ＲＧＢ：short RGB[3]の配列 
								// ・ピクセルは上から下に並び、左上を原点とする 
};

//! 色変換コールバック関数の戻り値
enum { 
	CONVERR_NONE = 0,		//!< 正常終了 
	CONVERR_INVALIDPARAM,	//!< 不当なパラメータ値が設定された 
	CONVERR_UNSUPPORTED,	//!< 定義されていない変換が指定された 
	CONVERR_MEMORYFULL,		//!< 作業領域が確保できない
	CONVERR_GENERIC			//!< 上記以外のエラー
};

//! JOBモード
enum ENUM_JOB_MODE{
	JOB_MODE_SETUP = 0,				//!< セットアップ中
//	JOB_MODE_SETUP_OTHER_SIDE,		//!< セットアップ中(反対側)
	JOB_MODE_RUN,					//!< 実行中
//	JOB_MODE_RUN_OTHER_SIDE,		//!< 実行中(反対側)
	JOB_MODE_NORMAL					//!< 通常
};

//! JOB PAHSE
enum ENUM_JOB_PAHSE{
    PHASE_BORN = 0,		//!< JOB作成時
    PHASE_SETUP_READY,	//!< JOB設定準備完了
    PHASE_PRINT_READY,	//!< 印刷準備完了
    PHASE_JOBRUNNING,	//!< JOB実行中
    PHASE_PRINTING,		//!< 印刷中(このフェーズにはならない)
    PHASE_PRINTED		//!< 印刷完了
};

//! JOB INFO_FLG
#define JOB_INFO_DONT_SAVE			0x00000001	//!< 保存しない(PrintNav)

//! 画像形式
typedef enum tagImageFormat {
	fileTIFF = 0,				//!< tiff
	fileBMP,					//!< bmp
	fileJPEG,					//!< jpeg
	filePNG,					//!< png
	filePhotoshop,				//!< psd
	fileRAW,					//!< raw
	fileJPEG2000				//!< jp2
} ImageFormat;

//! 画像データの種類
enum {
	modeBitmap = 1,				//!< モノクロ２値(WHITE is ZERO)
	modeGrayscale,				//!< モノクロ多値(WHITE is ZERO)
	modeRGBColor,				//!< ＲＧＢカラー
	modeCMYKColor,				//!< ＣＭＹＫカラー
//	2014.10.7 tmatsuda
	modeBitmapInv,				//!< モノクロ２値(BLACK is ZERO)
	modeGrayscaleInv			//!< モノクロ多値(BLACK is ZERO)
};

//! 画像データの格納方法
enum {
	imageChunky,				//!< ピクセル順次
	imagePlanar					//!< プレーン順次
};

//! jpeg形式で画像保存する際のオプション指定

//! 保存品質(0～100の範囲を6段階に区分)
enum {
	LIBJPGOPTION_QUALITY_MIN		= 0,		//!< 0
	LIBJPGOPTION_QUALITY_LOW		= 10,		//!< 10
	LIBJPGOPTION_QUALITY_QUARTER	= 25,		//!< 25
	LIBJPGOPTION_QUALITY_MEDIUM		= 40,		//!< 40
	LIBJPGOPTION_QUALITY_DEFAULT	= 75,		//!< 75
	LIBJPGOPTION_QUALITY_HIGH		= 80,		//!< 80
	LIBJPGOPTION_QUALITY_MAX		= 100		//!< 100
};

//! その他のオプション
#define LIBJPGOPTION_QUALITY_MASK	0x000000ff  //!< 保存品質（0～100の範囲で指定）
#define LIBJPGOPTION_FORCE_BASELINE 0x80000000	//!< ベースライン
#define LIBJPGOPTION_PROGRESSIVE    0x40000000	//!< プログレッシブjpeg

//! rotate_mode
enum {
	ROTATE_NONE = 0,	//!<  回転しない
	ROTATE_LEFT,		//!<  左へ90度回転
	ROTATE_RIGHT,		//!<  右へ90度回転
	ROTATE_REVERSE		//!<  180度回転
};


//! 音の種類
typedef enum _SOUND_TYPES {
	SOUND_TYPE_STOP,		//!< 音停止
	SOUND_TYPE_OK,			//!< OK音
	SOUND_TYPE_NG,			//!< NG音
	SOUND_TYPE_LOOP,		//!< Run/Print音
	SOUND_TYPE_TIMEOUT	//!< タイムアウト音
} SOUND_TYPES;


//! ユーザーID
enum DEF_USER_MODE_ID
{
	USER_ID_GENERAL = 1,		//!< ユーザ：一般
	USER_ID_EXPERT,				//!< ユーザ：エキスパート
	USER_ID_SERVICE,			//!< ユーザ：サービス
	USER_ID_RPPS_EXPERT			//!< ユーザ：RPPSエキスパート
};

//! ImageStatistics func
typedef enum {
	imageSum,	//!< dataに指定領域の合計値を返す（doubleのポインタを設定） 
	imageMean	//!< dataに指定領域の平均値を返す（doubleのポインタを設定） 
} ImageStatisticsFunc;

//! ImageDisplay func
typedef enum {
	imageDisplay,	//!< options: title (char*), fullscreen flag, closed flag
	imageQuit,		//!< no options
	imageResize,	//!< options: new width, new height, redraw, force
	/* redraw: If true, the current displayed image in the display window will be bloc-interpolated to fit the new dimensions.
			   If false, a black image will be drawn in the resized window.
	   force:  If true, the window size if effectively set to the specified dimensions.
			   If false, only internal data buffer to display images is resized, not the window itself. */
	imageShow,		//!< no options
	imageClose		//!< no options
} ImageDisplayFunc;

//! ImageDraw func
typedef enum {
	imageDrawPoint,		//!< options: x-coord, y-coord
	imageDrawLine,		//!< options: x-coord of start, y-coord of start, x-coord of end, y-coord of end, pattern
	imageDrawArrow,		//!< options: x-coord of start (tail), y-coord of start (tail), x-coord of end (head), y-coord of end (head), aperture angle of the arrow head, length of the arrow head (if < 0: described as a percentage of the arrow length), pattern
	imageDrawTriangle,	//!< options: x-coord of pt1, y-coord of pt1, x-coord of pt2, y-coord of pt2, x-coord of pt3, y-coord of pt3
	imageDrawRectangle,	//!< options: x-coord of upper-left, y-coord of upper-left, x-coord of lower-right, y-coord of lower-right
	imageDrawEllipse,	//!< options: x-coord of center, y-coord of center, first radius, second radius, x-coord of the orientation vector, y-coord of the orientation vector
	imageDrawCircle,	//!< options: x-coord of center, y-coord of center, radius
	imageDrawText,		//!< options: text (char*), x-coord, y-coord
	imageDrawAxeX,		//!< options: lower bound of the x-range, upper bound of the x-range, y-coord, precision
	imageDrawAxeY,		//!< options: x-coord, lower bound of the y-range, upper bound of the y-range, precision
	imageDrawAxeXY		//!< options: lower bound of the x-range, upper bound of the x-range, lower bound of the y-range, upper bound of the y-range, precision x, precision y
} ImageDrawFunc;

//! 単位系
enum eUNIT_MODE{
	UNIT_MODE_DEFAULT = 0,	//!< 操作環境設定画面で選択した単位(UserModeGUI)
	UNIT_MODE_MM,			//!< ミリメートル(PrintOrder, UserModeGUI, HeadCleaning, PaperFeed, FeedingAdjustments)
	UNIT_MODE_INCH,			//!< インチ(PrintOrder, UserModeGUI, PaperFeed, FeedingAdjustments)
	UNIT_MODE_POINT,		//!< ポイント(UserModeGUI)

	UNIT_1PER2_INCH,		//!< 1/2インチ(UserModeGUI)
	UNIT_1PER6_INCH			//!< 1/6インチ(UserModeGUI)
};


//! CreateDispImage()用
enum {
	PREVOPT_PLANE_SRC = 1 << 0,			//!< プレーン順次画像を入力
	PREVOPT_PIXEL_SRC = 1 << 1			//!< ピクセル順次画像を入力
};


//! 印刷面
enum{
	DEF_PRINT_SIDE_FRONT = 0,		//!< 表面(PrintOrder, Mainte_InkHead)
	DEF_PRINT_SIDE_BACK,			//!< 裏面(PrintOrder, Mainte_InkHead)
	DEF_PRINT_SIDE_COUNT			//!< カウント
};

//網種類の列挙子。
enum SCREENING_TYPE{
	SCREENING_TYPE_IMAGE_PHOTOGRAPHIC = 0,	//!< 画像
	SCREENING_TYPE_TEXT,					//!< 文字
	SCREENING_TYPE_LINE,					//!< 線
	SCREENING_TYPE_OTHER,					//!< その他
	//
	SCREENING_TYPE_COUNT					//!< 網種類数
};

//印刷機状態管理プラグのイベントIDの列挙子。
enum {
	PRINTERSTATE_EVT_ID_BUTTONSTAT = 0,		//!< ステータスとボタン情報。
	PRINTERSTATE_EVT_ID_HELTHCHECK,			//!< ヘルスチェック情報。
	PRINTERSTATE_EVT_ID_JOBNUMLIST,			//!< ジョブ一覧情報。
	PRINTERSTATE_EVT_ID_SLAVE_PRINTERINFO	//!< スレーブ機の情報をマスターに通知。
};

//印刷機状態管理プラグのボタンの有効無効の列挙子。
enum {
	PRINTERSTATE_BUTTONSTAT_ENABLED = 0,		//!< 有効。
	PRINTERSTATE_BUTTONSTAT_DISABLED,			//!< 無効。
	PRINTERSTATE_BUTTONSTAT_NONE				//!< 不明。
};

//印刷機状態管理プラグのボタンの実行中／停止中の列挙子。
enum {
	PRINTERSTATE_BUTTONSTAT_RUN = 0,		//!< 実行中。
	PRINTERSTATE_BUTTONSTAT_STOP,			//!< 停止中。
	PRINTERSTATE_BUTTONSTAT_RUN_NONE		//!< 不明。
};

//印刷機状態管理プラグの更新カウンタIDの列挙子。
enum {
	PRINTERSTATE_UPD_ID_BUTTON = 0,			//!< ボタン情報。
	PRINTERSTATE_UPD_ID_HELTH_CHECK,		//!< ヘルスチェック。
	PRINTERSTATE_UPD_ID_JOBNUMLIST,			//!< ジョブ一覧情報。
	PRINTERSTATE_UPD_ID_STAT,				//!< 印刷機ステータス。
	PRINTERSTATE_UPD_ID_INTERRUPT,			//!< インタラプトエラー。
	PRINTERSTATE_UPD_ID_P_ONOFF				//!< 電源ON/OFF。
};

//! 画面領域の種類。
typedef enum {
	WINDOWAREA_FULL	= 0,				//!< 全画面領域。
	WINDOWAREA_MAIN,					//!< メインウィンドウ。
	WINDOWAREA_STATUSBAR,				//!< ステータスバーウィンドウ。
	WINDOWAREA_FEED						//!< 給紙パネルウィンドウ。
} eWINDOWAREA;

//!< IBTログ用(segment).
#define IBT_SEG_P						_T("P")							//!< Event related to printer.
#define IBT_SEG_P1						_T("P1")						//!< Event related to printer 1.
#define IBT_SEG_P2						_T("P2")						//!< Event related to printer 2.
#define IBT_SEG_JD						_T("JD")						//!< Event related to JetDrive.
#define IBT_SEG_JD1						_T("JD1")						//!< Event related to JetDrive.
#define IBT_SEG_JD2						_T("JD2")						//!< Event related to JetDrive.
#define IBT_SEG_EQ						_T("EQ")						//!< Event related to EQUIOS.
#define IBT_SEG_V						_T("V")							//!< Event related to version.

//!< IBTログ用(subject).
#define IBT_SUBJECT_JOB					_T("JOB")						//!< Job Printing completed.
#define IBT_SUBJECT_STAT				_T("STAT")						//!< Printer State.
#define IBT_SUBJECT_ADJ					_T("ADJ")						//!< Printing (test charts/etc) for adjustment completed.
#define IBT_SUBJECT_ERR					_T("ERR")						//!< Error occurred.
#define IBT_SUBJECT_WARN				_T("WARN")						//!< Warning occurred.
#define IBT_SUBJECT_MNT					_T("MNT")						//!< Update serial number.
#define IBT_SUBJECT_MTN					_T("MTN")						//!< Maintenance information 
#define IBT_SUBJECT_VERSION				_T("VERSION")					//!< EQVerion.
#define IBT_SUBJECT_COMPO				_T("COMPO")						//!< COMPO
#define IBT_EVENT_SPLICE				_T("SPLICE")					//!< SPLICE

//!< IBTログ用(event).
#define IBT_EVENT_PRINT					_T("PRINT")						//!< Job Printing completed.
#define IBT_EVENT_SHUTDOWN				_T("SHUTDOWN")					//!< JetDrive shutdown.
#define IBT_EVENT_PHCLN					_T("PHCLN")						//!< Cleaning completed.
#define IBT_EVENT_SLEEP					_T("SLEEP")						//!< Change sleep mode.
#define IBT_EVENT_WAKE					_T("WAKE")						//!< Change Wake up mode.
#define IBT_EVENT_PHUNI					_T("PHUNI")						//!< Select shading data.
#define IBT_EVENT_PHALN					_T("PHALN")						//!< Select level tune data.
#define IBT_EVENT_FWDL					_T("FWDL")						//!< Firmware download completed.
#define IBT_EVENT_ON					_T("ON")						//!< Connection to MCPU (PIM_DeviceChange(arrival)) was detected.
#define IBT_EVENT_OFF					_T("OFF")						//!< Connection to MCPU was lost. 
#define IBT_EVENT_PHLIN					_T("PHLIN")						//!< Print head linearize completed.
#define IBT_EVENT_RIPSTART				_T("RIPSTART")					//!< RIP started.
#define IBT_EVENT_RIPEND				_T("RIPEND")					//!< RIP completed. 
#define IBT_EVENT_RIPABORT				_T("RIPABORT")					//!< RIP aborted. 
#define IBT_EVENT_OPEN					_T("OPEN")						//!< Screen's opened.
#define IBT_EVENT_CLOSE					_T("CLOSE")						//!< Screen's closed. 
#define IBT_EVENT_STOP					_T("STOP")						//!< Printer stopped.
#define IBT_EVENT_CHME					_T("CHME")						//!< During Self-Diagnosis.
#define IBT_EVENT_SCAN					_T("SCAN")						//!< Printer's scanning.
#define IBT_EVENT_END					_T("END")						//!< End job.
#define IBT_EVENT_FEED					_T("FEED")						//!< Feeding.
#define IBT_EVENT_PRINT					_T("PRINT")						//!< Printing.
#define IBT_EVENT_TENREADY				_T("TENREADY")					//!< Initial Tension.
#define IBT_EVENT_READY					_T("READY")						//!< Printer's ready.
#define IBT_EVENT_CLEAN					_T("CLEAN")						//!< Cleaning.
#define IBT_EVENT_TRAN					_T("TRAN")						//!< Transferring.
#define IBT_EVENT_TEMP					_T("TEMP")						//!< Temperature controlling.
#define IBT_EVENT_MNTE					_T("MNTE")						//!< Maintenance.
#define IBT_EVENT_RCOV					_T("RCOV")						//!< Recovering.
#define IBT_EVENT_INIT					_T("INIT")						//!< Starting.
#define IBT_EVENT_IDLE					_T("IDLE")						//!< No status.
#define IBT_EVENT_LAUNCH				_T("LAUNCH")					//!< Launch
#define IBT_EVENT_HEADREPLACE			_T("HEADREPLACE")				//!< Replace head
#define IBT_EVENT_INFO					_T("INFO")						//!< Job Information
#define IBT_EVENT_MONITOR				_T("MONIT")						//!< monitoring
#define IBT_EVENT_HEAD					_T("HEAD")						//!< Head
#define IBT_EVENT_HCLN					_T("HCLN")						//!< Head cleaning
#define IBT_EVENT_SECURITY				_T("SECURITY")					//!< SECURITY
#define IBT_EVENT_PRINTREADY			_T("PRINTREADY")				//!< PRINTREADY
#define IBT_EVENT_STARTUP				_T("STARTUP")					//!< Printer startup
#define IBT_EVENT_SPEED					_T("SPEED")						//!< SPEED
#define IBT_EVENT_START					_T("START")						//!< START
#define IBT_EVENT_EMGSTOP				_T("EMGSTOP")					//!< Emergency Stop
#define IBT_EVENT_ROLLER				_T("ROLLER")					//!< Roller
#define IBT_EVENT_INKCODE				_T("INKCODE")					//!< Ink code
#define IBT_EVENT_INKTANK				_T("INKTANK")					//!< Ink tank

#define IBT_GUI_LIN_TEST				_T("LIN-TEST")					//!< ID for test printing of PrintDensityGUI
#define IBT_GUI_LIN_ADJ					_T("LIN-ADJ")					//!< ID for adjust printing of PrintDensityGUI
#define IBT_GUI_UNI_TEST				_T("UNI-TEST")					//!< ID for test printing of ShadingGUI
#define IBT_GUI_UNI_ADJ					_T("UNI-ADJ")					//!< ID for adjust printing of ShadingGUI
#define IBT_GUI_ALN_TEST				_T("ALN-TEST")					//!< ID for test printing of HeadAlignmentGUI
#define IBT_GUI_ALN_ADJ					_T("ALN-ADJ")					//!< ID for adjust printing of HeadAlignmentGUI
#define IBT_GUI_ALNVL_TEST				_T("ALNVL-TEST")				//!< ID for test printing of Auto Adjustment Head Voltage
#define IBT_GUI_ALNVL_ADJ				_T("ALNVL-ADJ")					//!< ID for adjust printing of Manual Adjustment Head Voltage
#define IBT_GUI_CLN_TEST				_T("CLN-TEST")					//!< ID for test printing of HeadCleaningGUI
#define IBT_GUI_CLN_CHK					_T("CLN-CHK")					//!< ID for nozzle check of HeadCleaningGUI
#define IBT_GUI_CLN_HICHK				_T("CLN-HICHK")					//!< ID for High speed Nozzle check
#define IBT_GUI_SPTCOL					_T("SPTCOL")					//!< ID for spot color chart
#define IBT_GUI_DNS_TEST				_T("DNS-TEST")					//!< ID for test printing of DNS
#define IBT_GUI_JI_TEST					_T("JI-TEST")					//!< ID for test printing of JetInspection
#define IBT_GUI_NIR_TEST				_T("NIR-TEST")					//!< ID for test printing of NIR
#define IBT_GUI_ALN_HIADJ				_T("ALN-HIADJ")					//!< ID for High speed Head Aliment adjust printing
#define IBT_GUI_UNI_HIADJ				_T("UNI-HIADJ")					//!< ID for High speed shading adjust printing

#endif	// COMMON_DEF_H


