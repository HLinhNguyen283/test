/*! \file CtlPrintSettingsName.h
 *  \brief The control display print setting name and job name.
 *  \author GCS Vien Nguyen
 *  \date 2013/12/04 GCS Vien Nguyen created
*/

#if !defined(_CTL_PRINT_SETTINGS_NAME_H_)
#define _CTL_PRINT_SETTINGS_NAME_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control display print setting name and job name.
*/
class CCtlPrintSettingsName : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPrintSettingsName();
	virtual ~CCtlPrintSettingsName();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	//virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
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

	//関数

	// ロールタイプフラグ合成画像.
	void CreatePrintSettingsFlagImage();
	void DestroyPrintSettingsFlagImage();
	HBITMAP GetPrintSettingsFlagImage(bool, bool, bool, bool, BOOL);

	// 合成処理
	HBITMAP CreateImage(HWND hWnd, int nWidth, int nHeight, BYTE* pData);
	BOOL MergeBitmapBuff(HWND hWnd, HBITMAP srcBmp, SIZE dstSize, BYTE* dstData);
	BOOL MergeBitmap(HWND hWnd, HBITMAP* srcBmp, long srcBmpCount, HBITMAP* dstBmp);


};


#endif // !defined(_CTL_PRINT_SETTINGS_NAME_H_)

