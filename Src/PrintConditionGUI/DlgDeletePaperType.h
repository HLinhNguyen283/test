/*! \file DlgDeletePaperType.h
 *  \brief The control to contains controls to delete Paper type.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */

#if !defined(__DLG_DELETE_PAPER_TYPE_H__)
#define __DLG_DELETE_PAPER_TYPE_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control to contains controls to delete Paper type.
 */
class DlgDeletePaperType : public CBaseDlg
{
public:
	DlgDeletePaperType();
	virtual ~DlgDeletePaperType();

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


#endif // !defined(__DLG_DELETE_PAPER_TYPE_H__)
