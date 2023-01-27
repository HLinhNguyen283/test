/*! \file CtlDuplicatePrintSettingName.h
 *  \brief duplicate print setting name control for adding print settings
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/

#if !defined(_CTL_DUPLICATE_PRINT_SETTING_NAME_H_)
#define _CTL_DUPLICATE_PRINT_SETTING_NAME_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief used to selects a print setting name for duplicating
*/
class CCtlDuplicatePrintSettingName : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDuplicatePrintSettingName();
	virtual ~CCtlDuplicatePrintSettingName();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	void RegisterPrintSettingItem();

	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	HBITMAP m_RollFlagimage[32];	//!< 合成画像.

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


#endif // !defined(_CTL_DUPLICATE_PRINT_SETTING_NAME_H_)
