/*! \file DlgPrintConditionDeleting.h
 *  \brief The control to contains controls to delete Print settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/17 GCS Tan Ho created
 */

#if !defined(__DLG_PRINT_CONDITION_DELETING_H__)
#define __DLG_PRINT_CONDITION_DELETING_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control to contains controls to delete Print settings.
 */
class DlgPrintConditionDeleting : public CBaseDlg
{
public:
	DlgPrintConditionDeleting();
	virtual ~DlgPrintConditionDeleting();

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


#endif // !defined(__DLG_PRINT_CONDITION_DELETING_H__)
