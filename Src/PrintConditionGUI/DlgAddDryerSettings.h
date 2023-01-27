/*! \file DlgAddDryerSettings.h
 *  \brief The control edits a heat roller setting/dryer fun setting for printer 1 and 2.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho created
 */

#if !defined(__DLG_ADD_DRYER_SETTINGS_H__)
#define __DLG_ADD_DRYER_SETTINGS_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control edits a heat roller setting/dryer fun setting for printer 1 and 2.
 */
class DlgAddDryerSettings : public CBaseDlg
{
public:
	DlgAddDryerSettings();
	virtual ~DlgAddDryerSettings();

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


#endif // !defined(__DLG_ADD_DRYER_SETTINGS_H__)
