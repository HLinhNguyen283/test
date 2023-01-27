/*! \file DlgAddICCPreset.h
 *  \brief Add ICC Preset Dialog
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh Created
 */

#if !defined(__DLG_ADD_ICC_PRESET_H__)
#define __DLG_ADD_ICC_PRESET_H__


// ==========================================================================
// Include files
#include "BaseDlg.h"

// ==========================================================================
/*!
 *  \brief Add Print ICC Preset Dialog
 */
class DlgAddICCPreset : public CBaseDlg
{
public:
	DlgAddICCPreset();
	virtual ~DlgAddICCPreset();

protected:
	//関数
	virtual void OnSetAttribute();

	//イベント

	//部品

	//変数
private:
	//変数
};


#endif // !defined(__DLG_ADD_ICC_PRESET_H__)
