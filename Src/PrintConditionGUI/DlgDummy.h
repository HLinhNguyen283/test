/*! \file DlgDummy.h
 *  \brief ダミーダイアログ
 *  \author okada
 *  \date 2014/11/25 okada 新規作成
*/
#pragma once
#include "BaseDlg_OP.h"

/*!
 *  \brief ダミーダイアログクラス
*/
class CDlgDummy : public CBaseDlg
{
public:
	CDlgDummy();
	virtual ~CDlgDummy();

	// イベント
	virtual void OnSetAttribute();

};

