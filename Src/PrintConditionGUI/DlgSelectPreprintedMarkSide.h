/*! \file DlgSelectPreprintedMarkSide.h
 *  \brief The control to contains controls to select preprinted mark side
 *  \author GCS Tan Ho
 *  \date 2013/05/20 GCS Tan Ho created
 */

#if !defined(__DLG_SELECT_PREPRINTED_MARK_SIDE_H__)
#define __DLG_SELECT_PREPRINTED_MARK_SIDE_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control to contains controls to select preprinted mark side.
 */
class DlgSelectPreprintedMarkSide : public CBaseDlg_OpacityOption
{
public:
	DlgSelectPreprintedMarkSide();
	virtual ~DlgSelectPreprintedMarkSide();

protected:
	//関数
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//イベント

	//部品

	//変数

private:
	//変数


};


#endif // !defined(__DLG_SELECT_PREPRINTED_MARK_SIDE_H__)
