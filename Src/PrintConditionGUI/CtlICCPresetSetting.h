/*! \file CtlICCPresetSetting.h
 *  \brief ICC preset setting control
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

#if !defined(_CTL_ICC_PRESET_SETTING_H_)
#define _CTL_ICC_PRESET_SETTING_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief Used to selects the ICC preset
*/
class CCtlICCPresetSetting : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlICCPresetSetting();
	virtual ~CCtlICCPresetSetting();

	//イベント
	virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	
	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品
	void RegisterICCPresetItems();
	//変数

private:
	//変数

	//関数

};


#endif // !defined(_CTL_ICC_PRESET_SETTING_H_)