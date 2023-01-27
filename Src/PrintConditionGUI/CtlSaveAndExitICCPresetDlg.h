/*! \file CtlSaveAndExitICCPresetDlg.h
 *  \brief Commit new ICC preset name control
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

#if !defined(_CTL_SAVE_AND_EXIT_ICC_PRESET_H_)
#define _CTL_SAVE_AND_EXIT_ICC_PRESET_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to commit/cancel to add the ICC Preset name
*/
class CCtlSaveAndExitICCPresetDlg : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlSaveAndExitICCPresetDlg();
	virtual ~CCtlSaveAndExitICCPresetDlg();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	//virtual void OnUpdateValue();

protected:
	//関数
	
	//イベント
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数

	//関数
};


#endif // !defined(_CTL_SAVE_AND_EXIT_ICC_PRESET_H_)
