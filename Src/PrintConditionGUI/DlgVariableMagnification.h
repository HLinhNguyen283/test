/*! \file DlgVariableMagnification.h
 *  \brief 変倍ダイアログ
 */

#pragma once

// ==========================================================================
// Include files
#include "BaseDlg_OP.h"

// ==========================================================================


/**
 *  \brief 変倍ダイアログ
**/
class DlgVariableMagnification : public CBaseDlg
{
public:
	/**
		@brief	コンストラクタ
	**/
	DlgVariableMagnification();

	/**
		@brief	デストラクタ
	**/
	virtual ~DlgVariableMagnification();

protected:
	//関数

	//イベント
	
	/**
		@brief	コントロール属性設定イベント
	**/
	virtual void OnSetAttribute();

	/**
		@brief	コントロール生成イベント
	**/
	//virtual void OnCreateItem();

	//部品

	//変数

private:
	//変数
};

