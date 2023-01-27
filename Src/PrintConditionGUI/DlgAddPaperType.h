/*! \file DlgAddPaperType.h
 *  \brief Add Paper Type Dialog
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo Created
 */

#if !defined(__DLG_ADD_PAPER_TYPE_H__)
#define __DLG_ADD_PAPER_TYPE_H__


// ==========================================================================
// Include files
#include "BaseDlg.h"

// ==========================================================================
/*!
 *  \brief Add Paper Type Dialog
 */
class DlgAddPaperType : public CBaseDlg
{
public:
	DlgAddPaperType();
	virtual ~DlgAddPaperType();

protected:
	//関数
	virtual void OnSetAttribute();

	//イベント

	//部品

	//変数

private:
	//変数
};


#endif // !defined(__DLG_ADD_PAPER_TYPE_H__)
