/*! \file DlgAddDryerSettingsWithNir.h
 *  \brief Add dryer settings dialog with nir
 *  \author SCREEN
 *  \date 2018/01/25 SCREEN created
 */

#if !defined(__DLG_ADD_DRYER_SETTINGS_NIR_H__)
#define __DLG_ADD_DRYER_SETTINGS_NIR_H__


// ==========================================================================
// Include files

#include "BaseDlg_OP.h"

// ==========================================================================
// Global definitions

// ==========================================================================


/*!
 *  \brief Add dryer settings dialog with Nir
 */
class DlgAddDryerSettingsWithNir : public CBaseDlg
{
public:
	DlgAddDryerSettingsWithNir();
	virtual ~DlgAddDryerSettingsWithNir();

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


#endif // !defined(__DLG_ADD_DRYER_SETTINGS_NIR_H__)
