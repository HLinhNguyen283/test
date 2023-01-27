/*! \file DlgAddDryerSettingsWithPrecoat.h
 *  \brief Add dryer settings dialog with pre-coater
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho created
 */

#if !defined(__DLG_ADD_DRYER_SETTINGS_PRECOAT_H__)
#define __DLG_ADD_DRYER_SETTINGS_PRECOAT_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief Add dryer settings dialog with pre-coater
 */
class DlgAddDryerSettingsWithPrecoat : public CBaseDlg
{
public:
	DlgAddDryerSettingsWithPrecoat();
	virtual ~DlgAddDryerSettingsWithPrecoat();

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


#endif // !defined(__DLG_ADD_DRYER_SETTINGS_PRECOAT_H__)
