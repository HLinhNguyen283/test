/*! \file DlgPageMain.h
*  \brief Main page dialog
*  \author GCS
*  \date 2014/11/13 GCS
*/
#if !defined(__DLG_PAGE_MAIN_H__)
#define __DLG_PAGE_MAIN_H__



// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/**
*	Main page dialog
**/
class CDlgPageMain : public CBaseDlg
{
public:
	CDlgPageMain();
	virtual ~CDlgPageMain();

	virtual BOOL Create(CBaseDataIF* inData, CBasePrinterIF* inPrinter);

protected:
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
};
#endif // !defined(__DLG_PAGE_MAIN_H__)
