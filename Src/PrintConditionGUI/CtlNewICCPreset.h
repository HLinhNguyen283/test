/*! \file CtlNewICCPreset.h
 *  \brief new ICC preset name
 *  \author GCS Dieu Dinh
 *  \date 2013/12/13 GCS Dieu Dinh created
*/

#if !defined(_CTL_NEW_ICC_PRESET_H_)
#define _CTL_NEW_ICC_PRESET_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to edits to add a ICC Preset name
*/
class CCtlNewICCPreset : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlNewICCPreset();
	virtual ~CCtlNewICCPreset();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual void OnUpdateState();
	virtual void OnUpdateValue();

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
	
	// 合成処理
};


#endif // !defined(_CTL_NEW_ICC_PRESET_H_)
