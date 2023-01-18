/*! \file CommonUI.h
 *  \brief  GUI メイン定義ヘッダ
 *  \author TGL Sugita
 *  \date 2002/10/29 TGL Sugita 新規作成
 *  \date 2006/11/16 TGL Sugita TP344から分離。SDIJP専用ソース
 *  \date 2006/12/04 TGL Hamaguchi 数値計算ソフトウェアキーボード追加対応:要求番号000153
 *  \date 2007/12/04 TGL Hamaguchi SPI用に変更.
 *  \date 2008/07/03 TGL Hamaguchi ダイアログのフィルターウィンドウのリージョンを設定関数追加:TODO 1682
 *  \date 2008/07/11 TGL Hamaguchi 微調整用メッセージ追加:要求番号000229
 *  \date 2008/11/10 TGL Hamaguchi ヒント同時表示用IF関数追加：TPJS74:0056
 *  \date 2008/11/17 TGL Hamaguchi ボタンスタイル3つ追加と拡張スタイル対応：要求232
 *  \date 2009/09/29 TGL Hamaguchi メッセージを分離.
*/

#ifndef __COMMON_UI_H__
#define __COMMON_UI_H__

#include <winuser.h>

// ==========================================================================
// Global definitions

//! Control Types
enum
{
	CT_UNKNOWN = 0,		//!< 未使用：unknown
	CT_BUTTON,			//!< button
	CT_CHECKBOX,		//!< checkbox
	CT_RADIOBUTTON,		//!< 未使用：radiobutton
	CT_GROUPBOX,		//!< groupbox
	CT_PULLDOWN,		//!< pulldown menu
	CT_LISTBOX,			//!< listbox
	CT_EDITBOX,			//!< editbox
	CT_STATICBOX,		//!< staticbox
	CT_TARGET,			//!< 未使用：target
	CT_TABCONTROL,		//!< tab control
	CT_MENUCONTROL,		//!< menu control
	CT_COLORBUTTON,		//!< color button
	CT_LINECONTROL,		//!< line control
	CT_JOBLISTBOXBACK,	//!< 未使用：joblistboxback control
	CT_PROGRESS,		//!< progress control
	CT_BADGE			//!< badge control
};

//! Control Styles
#define CST_ENABLED 		0x80000000L		//!< 共通スタイル：有効
#define CST_DISABLED		0x40000000L		//!< 共通スタイル：無効
#define CST_SHOW			0x20000000L		//!< 共通スタイル：表示
#define CST_HIDE			0x10000000L		//!< 共通スタイル：非表示

//! Checkbox Control Styles
#define CKST_DISABLE_BLACK	0x00000001L		//!< チェックボックススタイル：無効時に黒っぽくなる(指定しないと無効時は白っぽくなる)

//! Button Control Styles
#define BNST_NORMAL			0x00000004L		//!< ボタンスタイル：プッシュボタン。
#define BNST_STICKY			0x00000001L		//!< ボタンスタイル：状態記憶ボタン。
#define BNST_STATIC			0x00000002L		//!< ボタンスタイル：無反応ボタン。
#define BNST_GRADATION_SIDE	0x00000008L		//!< ボタンスタイル：グラデーション(横)
#define BNST_TEXT			0x00000010L		//!< ボタンスタイル：文字列表示
#define BNST_BITMAP			0x00000020L		//!< ボタンスタイル：ビットマップ表示
#define BNST_ICON			0x00000040L		//!< ボタンスタイル：アイコン表示
#define BNST_COLORFRAME		0x00000080L		//!< ボタンスタイル：縁付きボタン。
#define BNST_AUTOREP		0x00000100L		//!< ボタンスタイル：押し続けたときに繰り返しコマンドメッセージを発行する。
#define BNST_PUSHONLY		0x00000200L		//!< ボタンスタイル：プッシュのみ可能なボタン。
#define BNST_IMAGE			0x00000400L		//!< ボタンスタイル：選択時、イメージを反転描画しない
#define BNST_MAX_SIZE		0x00001000L		//!< ボタンスタイル：画像を縦横比を保って伸縮表示する
#define BNST_GRADATION		0x00002000L		//!< ボタンスタイル：外観がグラデーションになる。
#define BNST_LED_LIGHT		0x00004000L		//!< ボタンスタイル：BNST_GRADATIONスタイルの時だけ有効で、ボタンがUPされる度に点灯状態が変わる。
#define BNST_SBAR			0x00008000L		//!< ボタンスタイル：ステータスバー専用ボタン
#define BNST_IMAGE_BLEND	0x00010000L		//!< ボタンスタイル：透過BMP対応スタイル
#define BNST_LED_AUTO		0x00020000L		//!< ボタンスタイル：LED点灯手動スタイル
#define BNST_PUSH_NOSET		0x00040000L		//!< ボタンスタイル：押下時にデータをセットできないスタイル
#define BNST_PUSH_IMAGE_SET	0x00080000L		//!< ボタンスタイル：押下時のイメージを指定できるスタイル
#define BNST_REVERSE_IMAGE  0x00100000L		//!< ボタンスタイル：押下時にイメージを反転するスタイル
#define BNST_FLAT			0x00200000L		//!< ボタンスタイル：フラットボタン。
#define BNST_UNITED_IMAGE	0x00400000L		//!< ボタンスタイル：統合されたビットマップを表示
#define BNST_FIXEDFONTSIZE	0x01000000L		//!< ボタンスタイル：固定フォントを使用する
#define BNST_LBUTTON_NOTIFY	0x02000000L		//!< ボタンスタイル：マウス左ボタンダウン/アップでイベント送信（給紙パネル用）
#define BNST_STARTUP		0x04000000L		//!< ボタンスタイル：スタートアップ専用ボタン.
#define BNST_DISABLE_BLACK	0x08000000L		//!< ボタンスタイル：無効時に黒っぽくなる(指定しないと無効時は白っぽくなる)


#define BNST_2STATE         (BNST_GRADATION | BNST_STICKY)		//!< ボタンスタイル：グラデーショの外観で状態記憶ボタン
#define BNST_FRAME_2STATE	BNST_GRADATION						//!< ボタンスタイル：グラデーションの外観のボタン
#define BNST_LED			(BNST_LED_LIGHT | BNST_LED_AUTO)	//!< ボタンスタイル：自動制御されたLEＤボタン
#define BNST_LED_MANUAL		BNST_LED_LIGHT						//!< ボタンスタイル：LED付のボタン
#define BNST_UNITED_BITMAP	(BNST_BITMAP | BNST_UNITED_IMAGE)	//!< ボタンスタイル：統合されたビットマップ

//! Button Control Extension Styles
#define BNST_EX_GROUP_L		0x00000001L		//!< ボタン拡張スタイル：グループボタン(左)。
#define BNST_EX_GROUP_C		0x00000002L		//!< ボタン拡張スタイル：グループボタン(中)。
#define BNST_EX_GROUP_R		0x00000004L		//!< ボタン拡張スタイル：グループボタン(右)。

//! GroupBox Control Styles
#define GBST_NORMAL				0x00000000L		//!< グループボックススタイル：フレームなしグループボックス。
#define GBST_PRIMARY			0x00000001L		//!< グループボックススタイル：一次グループボックス。
#define GBST_SECONDARY			0x00000002L		//!< グループボックススタイル：二次グループボックス。
#define GBST_SUB				0x00000004L		//!< グループボックススタイル：サブグループボックス。
#define GBST_NOTEXT				0x00000010L		//!< グループボックススタイル：テキストなしグループボックス。
#define GBST_SMALL_WINDOW		0x00000020L		//!< グループボックススタイル：窓付きグループボックス
#define GBST_SMALL_WINDOW_GOLD	0x00000040L		//!< グループボックススタイル：窓付きグループボックス(金)
#define GBST_SMALL_WINDOW_GRAY	0x00000080L		//!< グループボックススタイル：窓付きグループボックス(グレー)
#define GBST_UNEVEN				0x00000100L		//!< グループボックススタイル：凸凹グループボックス


//! ListBox Control Styles
#define LBST_VSCROLL				0x00000001L		//!< リストボックススタイル：垂直スクロール用リストボックス。
#define LBST_HSCROLL				0x00000002L		//!< リストボックススタイル：水平スクロール用リストボックス。
#define LBST_TEXT					0x00000010L		//!< リストボックススタイル：テキスト表示リストボックス。
#define LBST_BITMAP					0x00000020L		//!< リストボックススタイル：ビットマップ表示リストボックス。
#define LBST_ICON					0x00000040L		//!< リストボックススタイル：アイコン表示リストボックス。
#define LBST_NOTE					0x00000100L		//!< リストボックススタイル：注釈表示リストボックス。
#define LBST_TRANSPARENT			0x00000200L		//!< リストボックススタイル：選択項目の色を反転しないリストボックス。 
#define LBST_FIXEDFONTSIZE			0x01000000L		//!< リストボックススタイル：固定サイズのフォントを使用するリストボックス
#define LBST_BOXDISPLAY				0x00001000L		//!< リストボックススタイル：項目のテキストとイメージを縦に並べて表示。
#define LBST_LINEDISPLAY			0x00002000L		//!< リストボックススタイル：項目のテキストとイメージを横に並べて表示。
//<--要求番号 000232対応.
#define LBST_4LINEDISPLAY			0x00004000L		//!< リストボックススタイル：項目のテキスト(４行)とイメージを横に並べて表示。
//-->
#define LBST_2LINEDISPLAY			0x00008000L		//!< リストボックススタイル：項目のテキスト(2行)とイメージを横に並べて表示。
#define LBST_MULTISELECT			0x00010000L		//!< リストボックススタイル：複数項目選択可能リストボックス
#define LBST_NOCONV_NEW_PARAGRAPH	0x00020000L		//!< リストボックススタイル：「\n」を改行コードに変換しない
#define LBST_IMAGENORESIZE			0x00040000L		//!< リストボックススタイル：画像をリサイズしない

//! EditBox Control Styles
#define EBST_NORMAL				0x00000000L		//!< エディットボックススタイル：標準エディットボックス。
#define EBST_READONLY			0x00000001L		//!< エディットボックススタイル：読取専用エディットボックス。
#define EBST_NUMERIC			0x00000002L		//!< エディットボックススタイル：数値入力用エディットボックス。
#define EBST_FLAT				0x00000004L		//!< エディットボックススタイル：平面状のエディットボックス。
#define EBST_PASSWORD			0x00000010L		//!< エディットボックススタイル：パスワード入力用エディットボックス。
#define EBST_IME				0x00000020L		//!< エディットボックススタイル：IME使用
#define EBST_NOKEYBOARD			0x00000040L		//!< エディットボックススタイル：ソフトキーボードなし
#define EBST_MULTILINEVIEW		0x00000080L		//!< エディットボックススタイル：複数行表示(リードオンリー)
//要求番号000153対応.
#define EBST_CALCULATOR			0x00000100L		//!< エディットボックススタイル：数値計算.
#define EBST_RANGE				0x00000200L		//!< エディットボックススタイル：範囲指定エディットボックス.
#define EBST_QRCODE				0x00000400L		//!< エディットボックススタイル：QRコード
#define EBST_DISABLE_BLACK		0x00000800L		//!< エディットボックススタイル：無効時に黒っぽくなる(指定しないと無効時は白っぽくなる)
//<--要求番号000351追加--
#define EBST_SEARCH_ICON		0x00001000L		//!< エディットボックススタイル：検索アイコン.
//->

//! Static Control Styles
#define SCST_NORMAL					0x00000000L		//!< スタティックスタイル：境界線を持たないスタティックボックス。
#define SCST_SUNKEN					0x00000001L		//!< スタティックスタイル：窪んだ境界線を持つスタティックボックス。
#define SCST_RAISED					0x00000002L		//!< スタティックスタイル：盛り上がった境界線を持つスタティックボックス。
#define SCST_FLAT					0x00000004L		//!< スタティックスタイル：平面状のスタティックボックス。
#define SCST_OWNER_DRAW				0x00000008L		//!< スタティックスタイル：WM_PAINT処理時、親ウィンドウにUWM_OWNER_DRAW_PAINTイベントを発行する。
#define SCST_TEXT					0x00000010L		//!< スタティックスタイル：テキスト表示用ボックス。
#define SCST_BITMAP					0x00000020L		//!< スタティックスタイル：ビットマップ表示用ボックス。
#define SCST_ICON					0x00000040L		//!< スタティックスタイル：アイコン表示用ボックス。
#define SCST_SBAR					0x00000080L		//!< スタティックスタイル：ステータスバー専用.
#define SCST_MULTILINE				0x00000100L		//!< スタティックスタイル：複数行の文字列を表示するスタティックボックス。
#define SCST_NOTIFY					0x00000200L		//!< スタティックスタイル：イベント通知リストボックス
#define SCST_FIXEDFONTNAME			0x00000400L		//!< スタティックスタイル：固定フォント
#define SCST_DISABLE_BLACK			0x00000800L		//!< スタティックスタイル：無効時に黒っぽくなる(指定しないと無効時は白っぽくなる)
#define SCST_LEFT					0x00000000L		//!< スタティックスタイル：テキストを左揃えで表示する。
#define SCST_CENTER					0x00001000L		//!< スタティックスタイル：テキストを中央揃えで表示する。
#define SCST_RIGHT					0x00002000L		//!< スタティックスタイル：テキストを右揃えで表示する。
#define SCST_STRETCH				0x00004000L		//!< スタティックスタイル：画像をコントロール全体に伸縮表示する。
#define SCST_MAX_SIZE				0x00008000L		//!< スタティックスタイル：画像を縦横比を保ってコントロール全体に伸縮表示する
#define SCST_BUTTON					0x00010000L 	//!< スタティックスタイル：スタティックボタン
#define SCST_STICKY					0x00020000L		//!< スタティックスタイル：状態記憶ボタン。
#define SCST_AUTOREP				0x00040000L		//!< スタティックスタイル：押し続けたときに繰り返しコマンドメッセージを発行
#define SCST_FEEDER_PANEL			0x00080000L		//!< スタティックスタイル：給紙パネル用新デザイン
#define SCST_SINGLE_HINT			0x00100000L		//!< スタティックスタイル：ヒント用の1行表示
#define SCST_BK_NOGRAY				0x00200000L		//!< スタティックスタイル：無効時に背景をグレーにしない
#define SCST_IMAGE_BLEND			0x00400000L		//!< スタティックスタイル：透過BMP対応スタイル
#define SCST_UNITED_IMAGE			0x00800000L		//!< スタティックスタイル：統合イメージスタイル
#define SCST_FIXEDFONTSIZE			0x01000000L		//!< スタティックスタイル：固定フォント
#define SCST_NOCONV_NEW_PARAGRAPH	0x02000000L		//!< スタティックスタイル：「\n」を改行コードに変換しない
#define SCST_READONLY_EDIT_FRAME	0x04000000L		//!< スタティックスタイル：エディットボックスのフレーム
#define SCST_FIXEDFONTSIZE_S		0x08000000L		//!< スタティックスタイル：固定フォント(小さいフォント)

#define SCST_UNITED_BITMAP	(SCST_BITMAP | SCST_UNITED_IMAGE)//!< 統合されたビットマップ

//! Static Control Extension Styles
#define SCST_EX_NOCLIPSIBLINGS					0x00000001L		//!< スタティックボックス拡張スタイル：互いに関連する兄弟ウィンドウをクリップしない.
#define SCST_EX_TEXT_ASPECT_RATIO_LIMITED		0x00000002L		//!< スタティックボックス拡張スタイル：文字列縦横比制限有り(3:1～6:1)
#define SCST_EX_MID_ELLIPSIS					0x00000004L		//!< スタティックボックス拡張スタイル：文字列の中央省略符号(文字列の途中を...に置き換え)

//! Target Control Styles (必要に応じて対応する)
#define TGST_NORMAL				0x00000000L		//!< ターゲットスタイル：標準ターゲット
#define TGST_NOTIFY				0x00000200L		//!< ターゲットスタイル：イベント通知ターゲット。

//! PullDownMenu Control Styles
#define PMST_TEXT				0x00000010L		//!< プルダウンメニュースタイル：テキスト表示
#define PMST_BITMAP				0x00000020L		//!< プルダウンメニュースタイル：ビットマップ表示
#define PMST_ICON				0x00000040L		//!< プルダウンメニュースタイル：アイコン表示
#define PMST_SBAR				0x00000100L		//!< プルダウンメニュースタイル：ステータスバー専用
#define PMST_IMAGE_BLEND		0x00000200L		//!< プルダウンメニュースタイル：透過イメージ対応
#define PMST_JOBRUN   			0x00000400L		//!< プルダウンメニュースタイル：JOBRUNGUI専用
#define PMST_DISABLE_BLACK		0x00000800L		//!< プルダウンメニュースタイル：無効時に黒っぽくなる(指定しないと無効時は白っぽくなる)
//<--要求番号000351追加--
#define PMST_QUERY				0x00001000L		//!< プルダウンメニュースタイル：絞り込みエディットボックス表示
//->

//! Line Control Styles
#define LNST_NORMAL				0x00000000L		//!< ラインスタイル：標準

//! Tab Control Styles
#define TCST_NORMAL				0x00000000L		//!< タブスタイル：通常
#define TCST_TEXT				0x00000010L		//!< タブスタイル：テキスト表示
#define TCST_BITMAP				0x00000020L		//!< タブスタイル：ビットマップ表示
#define TCST_ICON				0x00000040L		//!< タブスタイル：アイコン表示
#define TCST_IMAGE_BLEND		0x00000080L		//!< タブスタイル：透過イメージ対応
#define TCST_FEEDERPANEL		0x00000100L		//!< タブスタイル：給紙パネル専用の外観を持つスタイル 
#define TCST_TABDOWN			0x00000200L		//!< タブスタイル：下付きのタブをもつタブコントロール
//<--要求番号 000232対応.
#define TCST_TABUP				0x00000400L		//!< タブスタイル：上にタブをもつタブコントロール
#define TCST_BUTTON				0x00000800L		//!< タブスタイル：タブ部分にボタンが乗ったタブコントロール
//->

//! Progress Control Styles
#define PCST_NORMAL				0x00000000L		//!< プログレススタイル：通常

//! Badge Control Styles
#define BDST_LEFT				0x00000000L		//!< バッジスタイル：バッジを左揃えで表示する。
#define BDST_CENTER				0x00001000L		//!< バッジスタイル：バッジを中央揃えで表示する。
#define BDST_RIGHT				0x00002000L		//!< バッジスタイル：バッジを右揃えで表示する。

//! ShowMessageBox() Flags
#define MBST_YES				0x00000001L		//!< メッセージボックススタイル：メッセージボックスに [Yes] のプッシュボタンを表示
#define MBST_OK					0x00000002L		//!< メッセージボックススタイル：メッセージボックスに [OK] のプッシュボタンを表示
#define MBST_ABORT				0x00000004L		//!< メッセージボックススタイル：メッセージボックスに [Abort] のプッシュボタンを表示
#define MBST_RETRY				0x00000008L		//!< メッセージボックススタイル：メッセージボックスに [Retry] のプッシュボタンを表示
#define MBST_NO					0x00000010L		//!< メッセージボックススタイル：メッセージボックスに [No] のプッシュボタンを表示 
#define MBST_CANCEL				0x00000020L		//!< メッセージボックススタイル：メッセージボックスに [Cancel] のプッシュボタンを表示
#define MBST_IGNORE				0x00000040L		//!< メッセージボックススタイル：メッセージボックスに[Ignore] のプッシュボタンを表示 
#define MBST_HELP				0x00000080L		//!< メッセージボックススタイル：メッセージボックスに [Help] のプッシュボタンを表示
#define MBST_DEBUGBREAK			0x00000100L		//!< メッセージボックススタイル：メッセージボックスに [Debug] のプッシュボタンを表示
#define MBST_NONEBUTTON			0x00000200L		//!< メッセージボックススタイル：ボタン無し
#define MBST_ABORTRETRYIGNORE	0x0000004CL		//!< メッセージボックススタイル：メッセージボックスに [Abort]、 [Retry]、[Ignore] の各プッシュボタンを表示  
#define MBST_OKCANCEL			0x00000022L		//!< メッセージボックススタイル：メッセージボックスに [OK]、[Cancel] の各プッシュボタンを表示
#define MBST_RETRYCANCEL		0x00000028L		//!< メッセージボックススタイル：メッセージボックスに [Retry]、[Cancel]の各プッシュボタンを表示
#define MBST_YESNO				0x00000011L		//!< メッセージボックススタイル：メッセージボックスに [Yes]、[No]の各プッシュボタンを表示
#define MBST_YESNOCANCEL		0x00000031L		//!< メッセージボックススタイル：メッセージボックスに [Yes]、[No]、[Cancel]の各プッシュボタンを表示
#define MBST_DEBUGOK			0x00000102L		//!< メッセージボックススタイル：メッセージボックスに [Debug][OK] のプッシュボタンを表示
#define MBST_ICONINFORMATION	0x10000000L		//!< メッセージボックススタイル：情報(i) アイコンを表示する。
#define MBST_ICONWARNING		0x20000000L		//!< メッセージボックススタイル：感嘆符(!) アイコンを表示する
#define MBST_ICONERROR			0x40000000L		//!< メッセージボックススタイル：停止アイコンを表示する。
#define MBST_ICONDEBUG			0x80000000L		//!< メッセージボックススタイル：デバッグアイコンを表示する。
#define MBST_FPDIALOG			0x01000000L		//!< メッセージボックススタイル：給紙側でも同様のダイアログを表示する。
#define MBST_MODELESS			0x00010000L		//!< メッセージボックススタイル：モードレスダイアログで表示する
#define MBST_USER_CLOSE_EVENT	0x00020000L		//!< メッセージボック数スタイル：ボタン押下時のダイアログ破棄を指定関数アドレスの戻り値で判断する.

//-- 安全確認(必要に応じて対応)--//
/*
#define MBST_SAFETY_CHECK		0x00020000L															//!< メッセージボックススタイル：安全確認オプション
#define MBST_YES_WITH_SAFETY_CHECK					(MBST_YES | MBST_SAFETY_CHECK)					//!< メッセージボックススタイル：[YES]ボタンを安全確認ボタンにする。	
#define MBST_OK_WITH_SAFETY_CHECK					(MBST_OK | MBST_SAFETY_CHECK)					//!< メッセージボックススタイル：[OK]ボタンを安全確認ボタンにする。
#define MBST_RETRY_WITH_SAFETY_CHECK				(MBST_RETRY	| MBST_SAFETY_CHECK)				//!< メッセージボックススタイル：[RETRY]ボタンを安全確認ボタンにする。
#define MBST_OKCANCEL_WITH_SAFETY_CHECK				(MBST_OKCANCEL	 | MBST_SAFETY_CHECK)			//!< メッセージボックススタイル：[OK][CANCEL]ボタンを表示し、[OK]ボタンを安全確認ボタンにする。
#define MBST_ABORTRETRYIGNORE_WITH_SAFETY_CHECK		(MBST_ABORTRETRYIGNORE	| MBST_SAFETY_CHECK)	//!< メッセージボックススタイル：[ABORT][RETRY][IGNORE]ボタンを表示し、[RETRY]ボタンを安全確認ボタンにする。
#define MBST_RETRYCANCEL_WITH_SAFETY_CHECK			(MBST_RETRYCANCEL | MBST_SAFETY_CHECK)			//!< メッセージボックススタイル：[RETRY][CANCEL]ボタンを表示し、[RETRY]ボタンを安全確認ボタンにする。
#define MBST_YESNO_WITH_SAFETY_CHECK				(MBST_YESNO | MBST_SAFETY_CHECK)				//!< メッセージボックススタイル：[YES][NO]ボタンを表示し、[YES]ボタンを安全確認ボタンにする。
#define MBST_YESNOCANCEL_WITH_SAFETY_CHECK			(MBST_YESNOCANCEL | MBST_SAFETY_CHECK)			//!< メッセージボックススタイル：[YES][NO][CANCEL]ボタンを表示し、[YES]ボタンを安全確認ボタンにする。
*/

//! Object Types(SetControlItem()用)
#define OT_TEXT					0x0001L		//!< アイテムタイプ：文字列がセットされている
#define OT_BITMAP				0x0002L		//!< アイテムタイプ：ビットマップがセットされている
#define OT_ICON					0x0004L		//!< アイテムタイプ：アイコンがセットされている
#define OT_SEPARATOR			0x0010L		//!< アイテムタイプ：セパレータがセットされている
#define OT_COLOR				0x0020L		//!< 未使用
#define OT_NOTE					0x0040L		//!< アイテムタイプ：注釈がセットされている
#define OT_PARAM				0x0100L		//!< アイテムタイプ：不可情報がセットされている
#define OT_CONST				0x0200L		//!< アイテムタイプ：リソースを読み取り専用で使う
#define OT_LED					0x0400L		//!< 未使用
#define OT_CONST_ALPHA			0x0800L		//!< アイテムタイプ：α値(定数)

//! Timer event ids
enum
{
	EVENTID_PLUGINTIMER = 1,
	EVENTID_PLUGINTIMER_1,
	EVENTID_PLUGINTIMER_2,
	EVENTID_PLUGINTIMER_3,
	EVENTID_PLUGINTIMER_4,
	EVENTID_PLUGINTIMER_5,
	EVENTID_PLUGINTIMER_6,
	EVENTID_PLUGINTIMER_7,
	EVENTID_PLUGINTIMER_8,
	EVENTID_PLUGINTIMER_9,
	EVENTID_PLUGINTIMER_EMA,
	EVENTID_PLUGINTIMER_EMA_1,
	EVENTID_PLUGINTIMER_EMA_2,
	EVENTID_PLUGINTIMER_EMA_3,
	EVENTID_PLUGINTIMER_EMA_4,
	EVENTID_PLUGINTIMER_EMA_5,
	EVENTID_PLUGINTIMER_EMA_6,
	EVENTID_PLUGINTIMER_EMA_7,
	EVENTID_PLUGINTIMER_EMA_8,
	EVENTID_PLUGINTIMER_EMA_9,
	EVENTID_MAINT_DUMP_PARAM,
	EVENTID_BASE_SP_CTL_1,
	EVENTID_BASE_SP_CTL_2,
	EVENTID_UI_PROCESS,
	EVENTID_SET_CHANGE_PARAM,
	EVENTID_MAINTE_JI_TIMER_1,
	EVENTID_WIN_TOUCH_1,
	EVENTID_WIN_MOUSE_1,
	EVENTID_PIM_TIMER_PROC	//!< PIM_TimerProc用タイマー

//
//	EVENTID_LAYOUTIMAGEGUI_AREA_VIEW_MAGRECT_BLINK,				//!< レイアウト画面 拡大表示範囲点滅タイマー用
//
//	EVENTID_JOBLIST_SORT_1,	//!< ジョブ選択リストソート用タイマー。
//	EVENTID_JOBLIST_SORT_2,	//!< ジョブ選択リストソート用タイマー。
//	EVENTID_JOBLIST_SORT_3,	//!< ジョブ選択リストソート用タイマー。
};

//!  G/SetControlColor() Flags
enum
{
	CC_FACE,			//!< コントロールの色：表面
	CC_TEXT,			//!< コントロールの色：テキスト
	CC_FRAME_LINE,		//!< コントロールの色：フレーム
	CC_IMAGE_BLEND,		//!< コントロールの色：イメージブレンド
	CC_FACE_CENTER,		//!< コントロールの色：表面真ん中のみ
	CC_COUNT
};

//! Listbox Command Type
enum
{
	LBCT_LIST = 0,		//!< リストボックスイベント種類：リスト項目が選択された
	LBCT_SCROLLUP,		//!< 未使用
	LBCT_SCROLLDOWN		//!< 未使用
};

//! UWM_OUTPUTPLUG_CONNECTのWPARAM
enum
{
	WP_USB_DISCONNECTD	= 0,	//!< 未接続.
	WP_USB_CONNECT				//!< 接続.
};

//! UWM_OUTPUTPLUG_****_GROUP_CHANGEのWPARAM
enum
{
	WP_GROUP_CHANGE_NORMAL	= 0,	//!< ボタン状態変化通知.
	WP_GROUP_CHANGE_DISABLED		//!< ボタン状態強制無効化通知.
};


//--要求番号000153--
//! Software KeyBoard Result
#define DEF_KEYBOARD_RESULT_OTHER	0x00000000 //!< ボタン押下以外 
#define DEF_KEYBOARD_RESULT_ENT		0x00000001 //!< [ENT]ボタン
#define DEF_KEYBOARD_RESULT_ESC		0x00000002 //!< [ESC]ボタン
#define DEF_KEYBOARD_RESULT_RIGHT	0x00000004 //!< [→|]
#define DEF_KEYBOARD_RESULT_LEFT	0x00000008 //!< [|←]ボタン

//コントロールの無効色定義
#define SBAR_CONTROL_DISABLE_COLOR_BK				-100				//!< 無効色の指定.
#define SBAR_CONTROL_DISABLE_COLOR_WT				100					//!< 無効色の指定.

//フィルターウィンドウの不透明度
#define LAYERED_ALPHA_DELIVERY_PANEL	120	//!< 排紙側レイヤードの不透明度
#define LAYERED_ALPHA_FEEDER_PANEL		120	//!< 給紙側レイヤードの不透明度 

//BUTTON(BNST_COLORFRAME)の指定可能なフレーム色
#define PUSHBUTTON_FRAMECOLOR_BLACK		RGB(   0,   0,   0)		//!< フレーム色：黒
#define PUSHBUTTON_FRAMECOLOR_BLUE		RGB(   0,   0, 255)		//!< フレーム色：青
#define PUSHBUTTON_FRAMECOLOR_GREEN		RGB(   0, 255,   0)		//!< フレーム色：緑
#define PUSHBUTTON_FRAMECOLOR_ORANGE	RGB( 255, 125,   0)		//!< フレーム色：橙
// ==========================================================================
// Global type definitions

#define DEF_CTRL_ITEM_VER		'VER0'	//!<ITEM構造体バージョン

//!  StaticBoxl Item Struct
typedef struct tagSCITEMINFO
{
	long Version;				//!< バージョン
	UINT nMask;					//!< マスク情報
	BYTE nConstAlpha;			//!< α値
	UINT nUnitedImageCount;		//!< 統合イメージ数
	UINT nUnitedImageIndex;		//!< 統合イメージ番号
} SCITEMINFO, *LPSCITEMINFO;


//!  Button Control Item Struct
typedef struct tagBNITEMINFO
{
	long Version;		//!< バージョン
	UINT nMask;			//!< マスク情報
	char* lpszText;		//!< 文字列
	HANDLE hImage;		//!< 画像
	HANDLE hSelImage;	//!< 選択時の画像
	BOOL    bLED_On;	//!< TRUE：LED表示をON
} BNITEMINFO, *LPBNITEMINFO;

//!  Color Button Control Item Struct
typedef struct tagCBITEMINFO
{
	long Version;		//!< バージョン
	UINT nMask;			//!< マスク情報
	char* lpszText;		//!< 文字列
	HANDLE hImage;		//!< 画像
	COLORREF crLine;	//!< 表示色

} CBITEMINFO, *LPCBITEMINFO;

//! プルダウンメニュー用プロパティデータ(各項目情報)
typedef struct tagPDITEMINFO
{
	long Version;		//!< バージョン
	char* text;			//!< 文字列
	HANDLE hImage;		//!< 画像
	BOOL disable;		//!< TRUE：無効
	void* data;			//!< 任意のパラメータ
	BOOL crTextEnabled;	//!< 文字色指定フラグ	
	COLORREF crText;	//!< 文字色
//--要求番号000351追加--
	BOOL no_query;		//!< 絞込み対象外アイテム指定.
}PDITEMINFO;

//! ListBox Control Item Struct
typedef struct tagLBITEMINFO
{
	long Version;	//!< バージョン
	UINT nMask;		//!< マスク情報
	char* lpszText;	//!< 文字列
	char* lpszNote;	//!< 注釈文字列
	HANDLE hImage;	//!< 画像
	DWORD dwParam;	//!< 拡張パラメータ
	BOOL disable;	//!< 無効フラグ
	BOOL lock;		//!< ロックマークフラグ
} LBITEMINFO, *LPLBITEMINFO;

//! Tab Control Item Struct
typedef struct tagTCITEMINFO
{
	long Version;	//!< バージョン
   	UINT nMask;		//!< マスク情報
  	char* lpszText;	//!< 文字列
   	HANDLE hImage;	//!< 画像
//--要求番号000232追加--
	BOOL bLED_Disp;	//!< TRUE:LED表示有.
	BOOL bLED_On;	//!< TRUE:LED点灯.

} TCITEMINFO, *LPTCITEMINFO;

//! グループボックス用アイテム構造体
typedef struct tagGBITEMINFO
{
	long Version;		//!< バージョン
	RECT rect;	//!< グループボックスの表示領域
}GBITEMINFO;

//--要求番号000153対応--
//! Edit Control Item Struct
typedef struct tagEDITEMINFO
{
	long Version;		//!< バージョン
	double	dValue;		//!< 現在設定数値<※必須>
	double* dMaxValue;	//!< 設定上限値
	double* dMinValue;	//!< 設定下限値
	long nDecimalPt;	//!< 小数点以下の表示桁数 (0:整数、1以上:実数、-1:桁数指定なし実数)<※必須>
} EDITEMINFO, *LPEDITEMINFO;

//! Chk Box Item Struct
typedef struct tagCHKITEMINFO
{
	long Version;		//!< バージョン
	char* text;			//!< 文字列
} CHKITEMINFO;

//! プログレスバーコントロール用アイテム構造体
typedef struct tagPSITEMINFO
{
	long Version;		//!< バージョン
	int nMax;	//!< 最大値
	int nMin;	//!< 最小値
	int nValue;	//!< 現在値
}PSITEMINFO, *LPPSITEMINFO;

//! Badge Control Styles
typedef struct tagBDITEMINFO
{
	long Version;	//!< バージョン(互換性維持用) / Version (Used to maintain compatibility)
	long max;		//!< 最大値 / max value
	long min;		//!< 最小値 / min value
}BDITEMINFO, *LPBDITEMINFO;

//! キーボード結果構造体 
typedef struct tagKEYBOARDRESULT
{
	HWND hWnd;		//!< 対象エディットボックスハンドル
	char* lpszText;	//!< 確定文字列バッファアドレス
	long nSizeText;	//!< 文字列バッファサイズ
	double dValue;	//!< 設定数値
	long nResult;	//!< どのボタンが押されたか
} KEYBOARDRESULT, *LPKEYBOARDRESULT;

//! リストボックスタッチ操作通知用構造体 
typedef struct tagLIST_BOX_TOUCH_CHANGE_ITEM
{
	DWORD dwIndex;	//!< 変更のあったアイテムのインデックス.
	BOOL bSelect;	//!< 変更後の選択状態.
} LISTBOX_TOUCH_CHANGE_ITEM, *LPLISTBOX_TOUCH_CHANGE_ITEM;

typedef const BNITEMINFO *LPCBNITEMINFO;	//!< ボタンアイテム構造体
typedef const PDITEMINFO *LPCPDITEMINFO;	//!< プルダウンメニューアイテム構造体
typedef const LBITEMINFO *LPCLBITEMINFO;	//!< リストボックスアイテム構造体
typedef const TCITEMINFO *LPCTCITEMINFO;	//!< タブアイテム構造体
typedef const GBITEMINFO *LPCGBITEMINFO;	//!< グループボックスアイテム構造体
typedef const EDITEMINFO *LPCEDITEMINFO;	//!< エディットボックスアイテム構造体
typedef const PSITEMINFO *LPCPSITEMINFO;	//!< プログレスバーアイテム構造体

//! コントロール情報
typedef struct tagCREATECONTROLSTRUCT
{
	HWND	hWndOwner;		//!< 親ウィンドウ
	DWORD	dwType;			//!< コントロールタイプ
	DWORD	dwStyle;		//!< コントロールスタイル
	INT		nStringID;		//!< 描画文字列リソースID
	RECT	rectItem;		//!< コントロール領域
	DWORD	dwParam;		//!< 追加情報

} CREATECONTROLSTRUCT, *LPCREATECONTROLSTRUCT;

//! コントロール情報へのポインタ
typedef const CREATECONTROLSTRUCT *LPCCREATECONTROLSTRUCT;

//! 拡張コントロール情報
typedef struct tagCREATECONTROLSTRUCTEX
{
	HWND	hWndOwner;		//!< 親ウィンドウ
	DWORD	dwType;			//!< コントロールタイプ
	DWORD	dwStyle;		//!< コントロールスタイル
	DWORD	dwExStyle;		//!< 拡張コントロールスタイル
	INT		nStringID;		//!< 描画文字列リソースID
	RECT	rectItem;		//!< コントロール領域
	DWORD	dwParam;		//!< 追加情報
} CREATECONTROLSTRUCTEX, *LPCREATECONTROLSTRUCTEX;

//! 拡張コントロール情報へのポインタ
typedef const CREATECONTROLSTRUCTEX *LPCCREATECONTROLSTRUCTEX;

/* Function Type Definitions */
typedef HWND (*GETMAINWINDOWPROC)(void);													//!< GUI関連コールバック関数定義
typedef BOOL (*CREATECONTROLSPROC)(LPCCREATECONTROLSTRUCT, HWND*, int, void*);				//!< GUI関連コールバック関数定義
typedef BOOL (*DESTROYCONTROLSPROC)(HWND*, int);											//!< GUI関連コールバック関数定義

typedef UINT (*CREATECONTROLPROC)(HWND, DWORD, DWORD, LPCTSTR, LPCRECT, DWORD);				
typedef void (*DESTROYCONTROLPROC)(UINT);													//!< GUI関連コールバック関数定義
typedef DWORD (*SETCONTROLDATAPROC)(UINT, DWORD);											//!< GUI関連コールバック関数定義
typedef DWORD (*GETCONTROLDATAPROC)(UINT);													//!< GUI関連コールバック関数定義
typedef int (*SETCONTROLITEMPROC)(UINT, int, LPCVOID);										//!< GUI関連コールバック関数定義
typedef LPCVOID (*GETCONTROLITEMPROC)(UINT, int);											//!< GUI関連コールバック関数定義
typedef BOOL (*SETCONTROLSTATEPROC)(UINT, DWORD);											//!< GUI関連コールバック関数定義
typedef DWORD (*GETCONTROLSTATEPROC)(UINT);													//!< GUI関連コールバック関数定義
typedef void (*SETCONTROLCOLORPROC)(UINT, int, DWORD);										//!< GUI関連コールバック関数定義
typedef int (*SETCONTROLOPTIONPROC)(UINT, int, void*);										//!< GUI関連コールバック関数定義.
typedef void* (*GETCONTROLOPTIONPROC)(UINT, int);											//!< GUI関連コールバック関数定義.
typedef int (*SETCONTROLSCROLLPROC)(UINT, int);												//!< GUI関連コールバック関数定義.
typedef int (*SHOWMESSAGEBOXPROC)(LPCTSTR, DWORD, FARPROC);									//!< GUI関連コールバック関数定義
typedef BOOL (*GETCLIENTAREAPROC)(UINT, LPRECT);											//!< GUI関連コールバック関数定義
typedef LPCTSTR (*SHOWFOLDERDIALOGPROC)(LPCTSTR);											//!< GUI関連コールバック関数定義
typedef BOOL (CALLBACK *LPDLGFILEHOOKPROC)(char* filePath);									//!< GUI関連コールバック関数定義
typedef LPCTSTR (*SHOWFILEDIALOGPROC)(LPCTSTR , LPCTSTR , LPDLGFILEHOOKPROC );				//!< GUI関連コールバック関数定義

typedef UINT (*CREATEBASEDIALOGBOX)(UINT, LPCRECT, void*, BOOL, char*);						//!< GUI関連コールバック関数定義
typedef UINT (*CREATEDIALOGBOXPROC)(UINT, LPCRECT, WNDPROC, BOOL, char*);					//!< GUI関連コールバック関数定義
typedef void (*DESTROYDIALOGBOXPROC)(UINT);													//!< GUI関連コールバック関数定義
typedef long (*PROCPLUGINDIALOGBOX)(UINT, UINT, WPARAM, LPARAM);							//!< GUI関連コールバック関数定義

typedef long (*SHOWHINTDIALOGPROC)(RECT*, long, char*);										//!< GUI関連コールバック関数定義
typedef void (*UNINSTALLHINTDIALOGPROC)( long );											//!< GUI関連コールバック関数定義
typedef long (*SHOWMODALHINTDIALOG)(RECT*, long, char*);									//!< GUI関連コールバック関数定義
typedef long (*SHOWHINTBLUEDIALOGPROC)(RECT*, long);										//!< GUI関連コールバック関数定義
typedef long (*ADDMSGHINTBLUEDIALOGPROC)(long, RECT*, char*);								//!< GUI関連コールバック関数定義
typedef long (*SHOWPROGRESSDIALOGPROC)(SIZE, long, long, long, long, FARPROC, char*);		//!< GUI関連コールバック関数定義
typedef void (*SETPROGRESSMESSAGEPROC)(long, long, long, char*);							//!< GUI関連コールバック関数定義
typedef void (*CLOSEPROGRESSDIALOGPROC)(long);												//!< GUI関連コールバック関数定義
typedef long (*ENTERFULLSCREENMODE)(void);													//!< GUI関連コールバック関数定義
typedef long (*EXITFULLSCREENMODE)(void);													//!< GUI関連コールバック関数定義
typedef BOOL (*SETCONTROLBKWINDOWPROC)(UINT, UINT);											//!< GUI関連コールバック関数定義
typedef HWND (*SETSTATUSBARWINDOWPROC)(void);												//!< GUI関連コールバック関数定義
typedef BOOL (*CANCELMESSAGEBOXPROC)(HWND);													//!< GUI関連コールバック関数定義

typedef long (*PCPLAYSOUNDFILE)(char* , long , UINT);										//!< GUI関連コールバック関数定義
typedef char* (*GETFONTNAMEPROC)(void);														//!< GUI関連コールバック関数定義
typedef void (*SHOWMESSAGEBOXMODELESSPROC)(long);											//!< GUI関連関数定義(メッセージボックス：モードレス用関数アドレス)
typedef BOOL (*SHOWMESSAGEBOX_USERCLOSEEVENTPROC)(HWND, long);								//!< GUI関連関数定義(メッセージボックス：MBST_USER_CLOSE_EVENT用関数アドレス)		
typedef long (*SHOWSHUTDOWNDIALOG)(char*);													//!< GUI関連コールバック関数定義
typedef void (*CLOSESHUTDOWNDIALOG)(long);													//!< GUI関連コールバック関数定義

typedef BOOL (*ISPROGRESSDIALOGHIDDEN)(long);												//!< GUI関連コールバック関数定義
typedef void (*SHOWHIDDENPROGRESS)(long);													//!< GUI関連コールバック関数定義
typedef long (*GETPROGRESSID)(long);														//!< GUI関連コールバック関数定義

typedef BOOL (*BLOCKINPUT_PROC_DP)(BOOL);													//!< GUI関連コールバック関数定義
typedef HWND (*SETCAPTURE_PROC_DP)(HWND);													//!< GUI関連コールバック関数定義
typedef BOOL (*RELEASECAPTURE_PROC_DP)(VOID);												//!< GUI関連コールバック関数定義
typedef int (*SHOW_IMAGE_MESSAGEBOXPROC)(LPCTSTR, DWORD, FARPROC, char*);					//!< GUI関連コールバック関数定義

typedef UINT (*CREATESETCAPTUREDIALOGBOXPROC)(UINT, LPCRECT, WNDPROC, BOOL, char*);			//!< GUI関連コールバック関数定義
/* 拡張コントロール作成関数 */
typedef BOOL (*CREATECONTROLSEXPROC)(LPCCREATECONTROLSTRUCTEX, HWND*, int, void*);				//!< GUI関連コールバック関数定義
typedef UINT (*CREATECONTROLEXPROC)(HWND, DWORD, DWORD, DWORD, LPCTSTR, LPCRECT, DWORD);		//!< GUI関連コールバック関数定義	
			
//FP用//
typedef HWND (*GETFEEDERPANELWINDOWPROC)(void);												//!< GUI関連コールバック関数定義
typedef BOOL (*CREATECONTROLSPROC_FP)(LPCCREATECONTROLSTRUCT, HWND*, int, void*);			//!< GUI関連コールバック関数定義
typedef UINT (*CREATECONTROLPROC_FP)(HWND, DWORD, DWORD, LPCTSTR, LPCRECT, DWORD);			//!< GUI関連コールバック関数定義
typedef UINT (*CREATEDIALOGBOXPROC_FP)(UINT, LPCRECT, WNDPROC, BOOL, char*);				//!< GUI関連コールバック関数定義
typedef int (*SHOWMESSAGEBOXPROC_FP)(LPCTSTR, DWORD, FARPROC);								//!< GUI関連コールバック関数定義
typedef long (*SHOWPROGRESSDIALOGPROC_FP)(SIZE, long, long, long, long, FARPROC, char*);	//!< GUI関連コールバック関数定義
typedef long (*SHOWHINTDIALOGPROC_FP)(RECT*, long, char*);									//!< GUI関連コールバック関数定義
typedef long (*SHOWMODALHINTDIALOG_FP)(RECT*, long, char*);									//!< GUI関連コールバック関数定義
typedef BOOL (*BLOCKINPUT_PROC_FP)(BOOL);													//!< GUI関連コールバック関数定義
typedef HWND (*SETCAPTURE_PROC_FP)(HWND);													//!< GUI関連コールバック関数定義
typedef BOOL (*RELEASECAPTURE_PROC_FP)(VOID);												//!< GUI関連コールバック関数定義
typedef long (*SHOWSHUTDOWNDIALOG_FP)(char*);												//!< GUI関連コールバック関数定義
typedef void (*CLOSESHUTDOWNDIALOG_FP)(long);												//!< GUI関連コールバック関数定義
typedef void (*SHOWWNDPRESTARTUP_FP)(VOID);													//!< GUI関連コールバック関数定義
typedef void (*HIDEWNDPRESTARTUP_FP)(VOID);													//!< GUI関連コールバック関数定義
typedef UINT (*CREATESETCAPTUREDIALOGBOXPROC_FP)(UINT, LPCRECT, WNDPROC, BOOL, char*);		//!< GUI関連コールバック関数定義
/* 拡張コントロール作成関数 */
typedef BOOL (*CREATECONTROLSEX_FPPROC)(LPCCREATECONTROLSTRUCTEX, HWND*, int, void*);				//!< GUI関連コールバック関数定義
typedef UINT (*CREATECONTROLEX_FPPROC)(HWND, DWORD, DWORD, DWORD, LPCTSTR, LPCRECT, DWORD);		//!< GUI関連コールバック関数定義				

//ページ管理
typedef long (*OPENPAGE)(long , long , BOOL );												//!< GUI関連コールバック関数定義
typedef long (*NEXTPAGE)();																	//!< GUI関連コールバック関数定義
typedef long (*PREVPAGE)();																	//!< GUI関連コールバック関数定義
typedef long (*PREVUITYPEPAGE)();															//!< GUI関連コールバック関数定義
typedef BOOL (*GETPAGEINFO)(long , long , char** , long* , long*, char**);					//!< GUI関連コールバック関数定義
typedef long (*GETNOWPAGEID)();																//!< GUI関連コールバック関数定義
typedef long (*GETNOWPAGEUITYPE)();															//!< GUI関連コールバック関数定義

//透過フレームダイアログ
typedef UINT (*CREATETRANSFRAMEDLG)(UINT, LPCRECT, WNDPROC, BOOL, BYTE, char*);					//!< GUI関連コールバック関数定義
typedef UINT (*CREATEBASETRANSFRAMEDLG)(UINT, LPCRECT, void*, BOOL, BYTE, char*);					//!< GUI関連コールバック関数定義
typedef void (*DESTROYTRANSFRAMEDLG)(UINT);													//!< GUI関連コールバック関数定義
typedef UINT (*CREATETRANSFRAMEDLG_FP)(UINT, LPCRECT, WNDPROC, BOOL, BYTE, char*);				//!< GUI関連コールバック関数定義

//ヒントのID取得
typedef HWND (*GETHINTDIALOG)(void);														//!< GUI関連コールバック関数定義
typedef HWND (*GETHINTDIALOG_FP)(void);														//!< GUI関連コールバック関数定義

//最大サイズを指定できるヒント
typedef long (*SHOWSETRANGEHINTDIALOGPROC)(RECT*, long, char*, long);						//!< GUI関連コールバック関数定義
typedef long (*SHOWSETRANGEHINTDIALOGPROC_FP)(RECT*, long, char*, long);					//!< GUI関連コールバック関数定義
typedef void (*SHOWSETRANGEHINTDIALOGPROC_DPFP)(RECT*, long, long*, RECT*, long, long*, long, char*);					//!< GUI関連コールバック関数定義

//リモートデスクトップダイアログ
typedef long (*SHOWREMOTEDESKTOPDLG)(char* );												//!< GUI関連コールバック関数定義
typedef void (*CLOSEREMOTEDESKTOPDLG)(long );												//!< GUI関連コールバック関数定義

//不透明度を指定できるダイアログを作成する
typedef UINT (*CLEATEDIALOGBOXOPACITYOPTION)(UINT, LPCRECT, WNDPROC, BOOL, BYTE, char*);	//!< GUI関連コールバック関数定義

//指定ダイアログのフィルターウィンドウのリージョンを設定する
typedef void (*SETDIALOGFILTERRGN)(UINT, HRGN, POINT);										//!< GUI関連コールバック関数定義

typedef HWND (*GETTOPSHOWDLG)(void);														//!< GUI関連コールバック関数定義

//ユーザー認証Dlgを表示する.
typedef BOOL (*SHOWUSERAUTHENTDLG)(char**, char**);											//!< GUI関連コールバック関数定義

typedef HWND (*SETCAPTURECTRL_PROC_DP)(HWND);													//!< GUI関連コールバック関数定義
typedef BOOL (*RELEASECAPTURECTRL_PROC_DP)(VOID);												//!< GUI関連コールバック関数定義	
typedef HWND (*SETCAPTURECTRL_PROC_FP)(HWND);													//!< GUI関連コールバック関数定義
typedef BOOL (*RELEASECAPTURECTRL_PROC_FP)(VOID);												//!< GUI関連コールバック関数定義	
typedef BOOL (*GETSTARTWARINGPROC)(void);													//!< GUI関連コールバック関数定義												//!< GUI関連コールバック関数定義
typedef void (*PAGESETREDRAWENABLEPROC)(BOOL);												//!< GUI関連コールバック関数定義.

typedef void (*SHOWPRINTNAVWINDOW_PROC)(BOOL);													//!< GUI関連コールバック関数定義.

typedef HWND (*GETSTARTUPWINDOWHANDLE_PROC)(void);											//!< GUI関連コールバック関数定義.
typedef void (*SETSHOWBUTTONTEXTSTATEPROC)(BOOL);											//!< GUI関連コールバック関数定義.
//アニメーションダイアログ作成関数.
typedef UINT (*CREATEANIMATIONDIALOG)(UINT, WNDPROC, char*);							//!< GUI関連コールバック関数定義.
typedef UINT (*CREATEANIMATIONDIALOG_FP)(UINT, WNDPROC, char*);							//!< GUI関連コールバック関数定義.
typedef UINT (*CREATEBASEANIMATIONDIALOG)(UINT, void*, char*);							//!< GUI関連コールバック関数定義.
//ダイアログ表示中のヒント許可設定関数.
typedef void (*SETENABLEHINT_DURINGSHOWINGDIALOGPROC)(BOOL);							//!< GUI関連コールバック関数定義.

//一般設定Dlg
typedef BOOL (*SHOWGENSETUPDIALOG)(SIZE*);		//!< GUI関連コールバック関数定義.
typedef void (*CLOSEGENSETUPDIALOG)();			//!< GUI関連コールバック関数定義.
typedef HWND (*GETGENSETUPDIALOG)();			//!< GUI関連コールバック関数定義.

typedef BOOL (*CANSHOWMESSAGEBOX)(DWORD);													//!< GUI関連コールバック関数定義.

typedef void (*WRITELOGFORSHOWMESSAGEBOXPROC)(char*, DWORD, DWORD);				//!< GUI関連コールバック関数定義.

typedef time_t (*GETLASTOPERATIONTIMEPROC)();											//!< GUI関連コールバック関数定義.

//! UI Callback Proc Struct
typedef struct tagUICBR
{
#define UICBRVERSION			0
	long VERSION;
	GETMAINWINDOWPROC			CBR_GetMainWindow;							//!< メインウィンドウハンドルゲット。
	CREATECONTROLSPROC			CBR_CreateControls;							//!< まとめてコントロールを作成。
	DESTROYCONTROLSPROC			CBR_DestroyControls;						//!< まとめてコントロールを削除。
	CREATECONTROLPROC			CBR_CreateControl;							//!< コントロール作成。
	DESTROYCONTROLPROC			CBR_DestroyControl;							//!< コントロール削除。
	SETCONTROLDATAPROC			CBR_SetControlData;							//!< コントロールにデータをセットする。
	GETCONTROLDATAPROC			CBR_GetControlData;							//!< コントロールからデータをゲットする。
	SETCONTROLITEMPROC			CBR_SetControlItem;							//!< コントロールにアイテムをセットする。
	GETCONTROLITEMPROC			CBR_GetControlItem;							//!< コントロールからアイテムをゲットする。
	SETCONTROLSTATEPROC			CBR_SetControlState;						//!< コントロールにステータスをセットする。
	GETCONTROLSTATEPROC			CBR_GetControlState;						//!< コントロールからステータスをゲットする。
	SETCONTROLCOLORPROC			CBR_SetControlColor;						//!< コントロールに色をセットする。
	SHOWMESSAGEBOXPROC			CBR_ShowMessageBox;							//!< メッセージボックスを表示する。
	GETCLIENTAREAPROC			CBR_GetClientArea;							//!< 指定したメインウィンドウのクライアントエリアを取得する。
	SHOWFOLDERDIALOGPROC		CBR_ShowFolderDialog;						//!< フォルダ選択ダイアログを表示する。
	CREATEDIALOGBOXPROC			CBR_CreateDialogBox;						//!< ダイアログを作成する。
	DESTROYDIALOGBOXPROC		CBR_DestroyDialogBox;						//!< ダイアログを破棄する。
	PROCPLUGINDIALOGBOX			CBR_ProcPluginDialogBox;					//!< CBR_CreateDialogBox()で作成したダイアログの基本メッセージプロシージャ。
	SHOWHINTDIALOGPROC			CBR_ShowHint;								//!< ヒントを表示する。
	UNINSTALLHINTDIALOGPROC 	CBR_HideHint;								//!< ヒントを非表示にする。
	SHOWPROGRESSDIALOGPROC		CBR_ShowProgressDialog;						//!< プログレスダイアログを表示する。
	SETPROGRESSMESSAGEPROC		CBR_SetProgressMessage;						//!< プログレスダイアログに各種値をセットする。
	CLOSEPROGRESSDIALOGPROC 	CBR_CloseProgressDialog;					//!< プログレスダイアログを閉じる。
	ENTERFULLSCREENMODE			CBR_EnterFullScreenMode;					//!< フルスクリーンモードを開始する。
	EXITFULLSCREENMODE			CBR_ExitFullScreenMode;						//!< フルスクリーンモードを終了する。
	SETCONTROLBKWINDOWPROC		CBR_SetControlBkWindow;						//!< コントロールの背景のウィンドウをセットする。
	SETSTATUSBARWINDOWPROC		CBR_GetStatusBarWindow;						//!< ステータスバーウィンドウハンドルを取得する。
	CANCELMESSAGEBOXPROC		CBR_CancelMessageBox;						//!< メッセージボックスをキャンセルする(モードレスのみ)。
	PCPLAYSOUNDFILE				CBR_PCPlaySoundFile;						//!< 音ファイルを再生する
	GETFONTNAMEPROC				CBR_GetFontName;							//!< フォント名を取得する。
	SHOWFILEDIALOGPROC			CBR_ShowFileDialog;							//!< ファイルダイアログを表示する。
	SHOWMODALHINTDIALOG			CBR_ShowModalHint;							//!< モーダルのヒントを表示する。(使用禁止)
	SHOWSHUTDOWNDIALOG			CBR_ShowShutDownDialog;						//!< シャットダウンダイアログを表示する。(使用禁止)
	CLOSESHUTDOWNDIALOG			CBR_CloseShutDownDialog;					//!< シャットダウンダイアログを閉じる。(使用禁止)
	ISPROGRESSDIALOGHIDDEN		CBR_IsProgressDialogHidden;					//!< プログレスダイアログが隠れているかチェック。
	SHOWHIDDENPROGRESS			CBR_ShowHiddenProgress;						//!< 隠れている進捗ダイアログを表示状態にする。
	GETPROGRESSID				CBR_GetProgressID;							//!< プログレスダイアログのIDを取得(ダイアログのウィンドウハンドル)。
	BLOCKINPUT_PROC_DP CBR_BlockInput_DP;									//!< 排紙側のGUI操作を規制する。
	SETCAPTURE_PROC_DP CBR_SetCapture_DP;									//!< 排紙側のフォーカスを固定する。
	RELEASECAPTURE_PROC_DP CBR_ReleaseCapture_DP;							//!< 排紙側のフォーカスを解除する。
	SHOW_IMAGE_MESSAGEBOXPROC	CBR_ShowImageMessageBox;					//!< 画像付きメッセージボックスを表示する。
	CREATESETCAPTUREDIALOGBOXPROC	CBR_CreateSetCaptureDialogBox;			//!< SetCaptureダイアログを作成する。(使用禁止)

	GETFEEDERPANELWINDOWPROC	CBR_GetFeederPanelWindow;					//!< 給紙パネルウィンドウをハンドルを取得する。
	CREATECONTROLSPROC_FP		CBR_CreateControls_FP;						//!< 給紙パネルのコントロールをまとめて作成。
	CREATECONTROLPROC_FP		CBR_CreateControl_FP;						//!< 給紙パネルのコントロールを作成。
	CREATEDIALOGBOXPROC_FP		CBR_CreateDialogBox_FP;						//!< 給紙パネルのダイアログを作成。
	SHOWMESSAGEBOXPROC_FP		CBR_ShowMessageBox_FP;						//!< 給紙パネルのメッセージボックスを表示。
	SHOWPROGRESSDIALOGPROC_FP	CBR_ShowProgressDialog_FP;					//!< 給紙パネルのプログレスダイアログを表示。
	SHOWHINTDIALOGPROC_FP		CBR_ShowHint_FP;							//!< 給紙パネルのヒントを表示。
	SHOWMODALHINTDIALOG_FP		CBR_ShowModalHint_FP;						//!< 給紙パネルのモーダルのヒントを表示。(使用禁止)
	BLOCKINPUT_PROC_FP			CBR_BlockInput_FP;							//!< 給紙パネルのGUI操作を規制する。
	SETCAPTURE_PROC_FP			CBR_SetCapture_FP;							//!< 給紙パネルのフォーカスを固定する。
	RELEASECAPTURE_PROC_FP		CBR_ReleaseCapture_FP;						//!< 給紙パネルのフォーカスを解除する。
	SHOWSHUTDOWNDIALOG_FP		CBR_ShowShutDownDialog_FP;					//!< 給紙パネルのシャットダウンダイアログを表示する。(使用禁止)
	CLOSESHUTDOWNDIALOG_FP		CBR_CloseShutDownDialog_FP;					//!< 給紙パネルのシャットダウンダイアログを閉じる。(使用禁止)
	SHOWWNDPRESTARTUP_FP		CBR_ShowWndPreStartup_FP;					//!< 給紙パネルの起動中画面を表示する。
	HIDEWNDPRESTARTUP_FP		CBR_HideWndPreStartup_FP;					//!< 給紙パネルの起動中画面を閉じる。
	CREATESETCAPTUREDIALOGBOXPROC_FP	CBR_CreateSetCaptureDialogBox_FP;	//!< 給紙パネルのSetCaptureダイアログを作成する。(使用禁止)

	OPENPAGE CBR_OpenPage;													//!< ページを開く
	NEXTPAGE CBR_NextPage;													//!< 次のページを開く
	PREVPAGE CBR_PrevPage;													//!< 前のページを開く
	GETPAGEINFO  CBR_GetPageInfo;											//!< ページ情報を取得する
	GETNOWPAGEID CBR_GetNowPageID;											//!< 現在表示中のページIDを取得する
	GETNOWPAGEUITYPE CBR_GetNowPageUiType;									//!< 現在のページのUIタイプを取得する

	CREATETRANSFRAMEDLG			CBR_CreateTransFrameDlg;					//!< 透過フレームダイアログを作成する
	DESTROYTRANSFRAMEDLG		CBR_DestroyTransFrameDlg;					//!< 透過フレームダイアログを破棄する
	CREATETRANSFRAMEDLG_FP		CBR_CreateTransFrameDlg_FP;					//!< 透過フレームダイアログを作成する(給紙パネル用)

	GETHINTDIALOG				CBR_GetHintDialog;							//!< 表示されているヒントダイアログのハンドルを取得する.
	GETHINTDIALOG_FP			CBR_GetHintDialog_FP;						//!< 給紙パネルの表示されているヒントダイアログのハンドルを取得する.

	SHOWSETRANGEHINTDIALOGPROC		CBR_ShowSetRangeHint;					//!< 最大サイズを指定できるヒントダイアログ
	SHOWSETRANGEHINTDIALOGPROC_FP	CBR_ShowSetRangeHint_FP;				//!< 最大サイズを指定できるヒントダイアログ(給紙パネル用)

	SHOWREMOTEDESKTOPDLG		CBR_ShowRemoteDesktopDlg;					//!< リモートデスクトップダイアログを表示する
	CLOSEREMOTEDESKTOPDLG		CBR_CloseRemoteDesktopDlg;					//!< リモートデスクトップダイアログを閉じる
	CLEATEDIALOGBOXOPACITYOPTION	CBR_CreateDialogBoxOpacityOption;		//!< 不透明度を指定できるダイアログを作成する
	SETDIALOGFILTERRGN			CBR_SetDialogFilterRgn;						//!< 指定ダイアログのフィルターウィンドウのリージョンを設定する

	SHOWSETRANGEHINTDIALOGPROC_DPFP	CBR_ShowSetRangeHintDialog_DPFP;		//!< 最大サイズを指定できるヒントダイアログ(排紙給紙同時表示)			

	CREATECONTROLEXPROC			CBR_CreateControlEx;						//!< コントロール作成(拡張)。
	CREATECONTROLSEXPROC		CBR_CreateControlsEx;						//!< まとめてコントロールを作成(拡張)。
	CREATECONTROLEX_FPPROC		CBR_CreateControlEx_FP;						//!< コントロール作成(拡張)(給紙パネル用)。
	CREATECONTROLSEX_FPPROC		CBR_CreateControlsEx_FP;					//!< まとめてコントロールを作成(拡張)(給紙パネル用)。

	SETCAPTURE_PROC_DP			CBR_SetCaptureThrough_DP;					//!< 排紙側のフォーカスを固定する。
	SETCAPTURE_PROC_FP			CBR_SetCaptureThrough_FP;					//!< 給紙パネルのフォーカスを固定する。

	GETTOPSHOWDLG				CBR_GetTopShowDlg_DP;						//!< 排紙側の一番上のDlgを取得する。
	GETTOPSHOWDLG				CBR_GetTopShowDlg_FP;						//!< 給紙側の一番上のDlgを取得する。

	SETCAPTURECTRL_PROC_DP			CBR_SetCapture_Ctrl_DP;						//!< マウスイベントを指定ウィンドウにキャプチャーする(排紙側)(コントロール用)
	RELEASECAPTURECTRL_PROC_DP		CBR_ReleaseCapture_Ctrl_DP;					//!< ウィンドウキャプチャーを解除(排紙側)(コントロール用)
	SETCAPTURECTRL_PROC_FP			CBR_SetCapture_Ctrl_FP;						//!< マウスイベントを指定ウィンドウにキャプチャーする(給紙側)(コントロール用)
	RELEASECAPTURECTRL_PROC_FP		CBR_ReleaseCapture_Ctrl_FP;					//!< ウィンドウキャプチャーを解除(給紙側)(コントロール用)

	GETSTARTWARINGPROC			CBR_GetStartWarning;						//!< 起動中に警告が表示されたかチェック.
	PAGESETREDRAWENABLEPROC		CBR_Page_SetRedrawEnable;					//!< ページ描画の再描画許可設定.

	PREVUITYPEPAGE CBR_PrevUITypePage;										//!< UI_TYPEの違う前のページを開く

	SHOWMESSAGEBOXPROC			CBR_ShowMessageBox_TopMost;					//!< 最前面専用メッセージボックスを表示する。
	SHOWUSERAUTHENTDLG			CBR_ShowUserAuthenticationDialog;			//!< ユーザー認証ダイアログを表示する.

	SHOWPRINTNAVWINDOW_PROC		CBR_ShowPrintNavWindow;						//!< プリナビの画面の表示・非表示切り替え.
	GETSTARTUPWINDOWHANDLE_PROC	CBR_GetStartupWindowHandle;					//!< 起動中画面のウインドウハンドル取得。
	SETSHOWBUTTONTEXTSTATEPROC	CBR_SetShowButtonNamesState;				//!< 画像と文字列同時指定ボタンの文字列表示有無を設定する.

	SETCONTROLOPTIONPROC		CBR_SetControlOption;						//!< コントロールにオプションデータをセットする.
	GETCONTROLOPTIONPROC		CBR_GetControlOption;						//!< コントロールからオプションデータをゲットする.

	CREATEANIMATIONDIALOG		CBR_CreateAnimationDialog;					//!< アニメーションダイアログ作成関数(排紙側)
	CREATEANIMATIONDIALOG_FP	CBR_CreateAnimationDialog_FP;				//!< アニメーションダイアログ作成関数(給紙側)

	SETENABLEHINT_DURINGSHOWINGDIALOGPROC	CBR_SetEnableHint_DuringShowingDialog;		//!< ダイアログ表示中のヒント表示許可/不許可設定(排紙側)(使用禁止)
	SETENABLEHINT_DURINGSHOWINGDIALOGPROC	CBR_SetEnableHint_DuringShowingDialog_FP;	//!< ダイアログ表示中のヒント表示許可/不許可設定(給紙側)(使用禁止)

	SHOWHINTBLUEDIALOGPROC		CBR_ShowHintBlue;							//!< 青ヒントを表示する。
	ADDMSGHINTBLUEDIALOGPROC	CBR_AddMsgHintBlue;							//!< 青ヒントに文字列を設定する。

	SHOWGENSETUPDIALOG		CBR_ShowGenSetupDialog;							//!< 一般設定Dlgを表示する
	CLOSEGENSETUPDIALOG		CBR_CloseGenSetupDialog;						//!< 一般設定Dlgを閉じる
	GETGENSETUPDIALOG		CBR_GetGenSetupDialog;							//!< 一般設定Dlgのウィンドウハンドル取得。

	CREATEBASEDIALOGBOX		CBR_CreateBaseDialogBox;						//!< 基本ダイアログを表示する
	CREATEBASETRANSFRAMEDLG		CBR_CreateBaseTransFrameDlg;				//!< 透過フィルターダイアログを表示する(新フレームワーク用)

	SETCONTROLSCROLLPROC		CBR_SetControlScroll;						//!< コントロールをスクロールする.

	CANSHOWMESSAGEBOX			CBR_CanShowMessageBox;						//!< メッセージボックスの表示可否取得
	WRITELOGFORSHOWMESSAGEBOXPROC	CBR_WriteLogForShowMessageBox;			//!< メッセージボックスのログ出力.

	CREATEBASEANIMATIONDIALOG		CBR_CreateBaseAnimationDialog;			//!< アニメーションダイアログを表示する(新フレームワーク用)

	GETLASTOPERATIONTIMEPROC		CBR_GetLastOperationTime;				//!< 最終操作日時を取得.
} UICBR, *LPUICBR;

extern UICBR uiCBR;
//#define UICBR_VERSION 0;

#endif // __COMMON_UI_H__
