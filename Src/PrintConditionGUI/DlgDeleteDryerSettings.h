/*! \file DlgDeleteDryerSettings.h
 *  \brief The control to contains controls to delete dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
 */

#if !defined(__DLG_DELETE_DRYER_SETTINGS_H__)
#define __DLG_DELETE_DRYER_SETTINGS_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief The control to contains controls to delete dryer settings.
 */
class DlgDeleteDryerSettings : public CBaseDlg
{
public:
	DlgDeleteDryerSettings();
	virtual ~DlgDeleteDryerSettings();

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


#endif // !defined(__DLG_DELETE_DRYER_SETTINGS_H__)
