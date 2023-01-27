/*! \file DlgDeletePrintHeadAlignment.h
 *  \brief The control to contains controls to delete Print head alignment.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */

#if !defined(__DLG_DELETE_PRINT_HEAD_ALIGNMENT_H__)
#define __DLG_DELETE_PRINT_HEAD_ALIGNMENT_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control to contains controls to delete Print head alignment.
 */
class DlgDeletePrintHeadAlignment : public CBaseDlg
{
public:
	DlgDeletePrintHeadAlignment();
	virtual ~DlgDeletePrintHeadAlignment();

protected:
	//関数
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	//イベント

	//部品

	//変数

private:
	//変数


};


#endif // !defined(__DLG_DELETE_PRINT_HEAD_ALIGNMENT_H__)
