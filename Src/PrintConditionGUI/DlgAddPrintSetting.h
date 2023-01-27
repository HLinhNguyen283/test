/*! \file DlgAddPrintSetting.h
 *  \brief Add Print Setting Dialog
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo Created
 */

#if !defined(__DLG_ADD_PRINT_SETTING_H__)
#define __DLG_ADD_PRINT_SETTING_H__


// ==========================================================================
// Include files
#include "BaseDlg.h"

// ==========================================================================
/*!
 *  \brief Add Print Setting Dialog
 */
class DlgAddPrintSetting : public CBaseDlg
{
public:
	DlgAddPrintSetting();
	virtual ~DlgAddPrintSetting();

protected:
	//関数
	virtual void OnSetAttribute();

	//イベント

	//部品

	//変数

private:
	//変数
};


#endif // !defined(__DLG_ADD_PRINT_SETTING_H__)
