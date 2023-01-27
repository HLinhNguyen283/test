/*! \file CtlPrintSettingsSelection.h
 *  \brief The control selects to edit print settings
 *  \author GCS Tan Ho
 *  \date 2013/06/14 GCS Tan Ho created
*/

#if !defined(_CTL_PRINT_SETTINGS_SELECTION_H_)
#define _CTL_PRINT_SETTINGS_SELECTION_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects to edit print settings
*/
class CCtlPrintSettingsSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintSettingsSelection();
	virtual ~CCtlPrintSettingsSelection();

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
	virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	HBITMAP m_RollFlagimage[32];	//!< 合成画像.
	long m_PrintSettingsStartID;	//!< The start id of print settings

	long m_CurrentSelection;		//!< item ID of the current selection

	//関数

	// methods
	void RegisterPrintConditionItems();

	// ロールタイプフラグ合成画像.
	void CreatePrintSettingsFlagImage();
	void DestroyPrintSettingsFlagImage();
	HBITMAP GetPrintSettingsFlagImage(bool, bool, bool, bool, BOOL);

	// 合成処理
	HBITMAP CreateImage(HWND hWnd, int nWidth, int nHeight, BYTE* pData);
	BOOL MergeBitmapBuff(HWND hWnd, HBITMAP srcBmp, SIZE dstSize, BYTE* dstData);
	BOOL MergeBitmap(HWND hWnd, HBITMAP* srcBmp, long srcBmpCount, HBITMAP* dstBmp);

};


#endif // !defined(_CTL_PRINT_SETTINGS_SELECTION_H_)
