/*! \file DlgDummy.cpp
 *  \brief システムの印刷条件画面からジョブ設定画面・レイアウト画面を表示する際のダミーダイアログ
 *  \author okada
 *  \date 2014/11/25 okada 新規作成
 */

#include "StdAfx.h"
#include "DlgDummy.h"
#include "CommonCBRProc.h"

#define DEF_DLG_W		1920	//!< ダイアログの幅.
#define DEF_DLG_H		926		//!< ダイアログの高さ.

 /*!
  * \brief コンストラクタ
  */
 CDlgDummy::CDlgDummy()
 {
 }

 /*!
  * \brief デストラクタ
  */
 CDlgDummy::~CDlgDummy()
 {
 }

/*! \brief コントロールリストにコントロールクラスインスタンスを登録するイベント
*/
void CDlgDummy::OnSetAttribute()
{
	RECT jobSelect_ClientRect = {0};
	GetClientArea(WINDOWAREA_MAIN, &jobSelect_ClientRect);

	int Width = jobSelect_ClientRect.right - jobSelect_ClientRect.left + 1;
	int Height = jobSelect_ClientRect.bottom - jobSelect_ClientRect.top + 1;

	RECT DlgRect =
	{
		(Width -  DEF_DLG_W) / 2,
		(Height - DEF_DLG_H) / 2,
		((Width - DEF_DLG_W) / 2) + DEF_DLG_W,
		((Height - DEF_DLG_H) / 2) + DEF_DLG_H
	};

	// オフセットの加算
	OffsetRect(&DlgRect, jobSelect_ClientRect.left, jobSelect_ClientRect.top);

	SetAttribute(&DlgRect, TRUE, PathFindFileName(__FILE__));
}
