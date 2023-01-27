/*! \file CtlToneCurveSettings.h
 *  \brief The control displays Tone settings dialog.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/

#if !defined(_CTL_TONE_CURVE_SETTINGS_H_)
#define _CTL_TONE_CURVE_SETTINGS_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"
#include "PaperDBDef_OP.h"
#include "PrintConditionGUIDef.h"

// ==========================================================================

/*!
 *  \brief The control displays Tone settings dialog.
*/
class CCtlToneCurveSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlToneCurveSettings();
	virtual ~CCtlToneCurveSettings();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	virtual void InitDotGainBitmap(HWND hWnd, SIZE sizeImage);
	virtual void DrawDotGainBitmap();
	virtual void FinDotGainBitmap();
	virtual HBITMAP CreateImage(HWND hWnd, int nWidth, int nHeight, char* pData);
	virtual void CreateDotGainCurve(ST_PDB_ANCHOR_POINT* anchor, short* dotgain_curve);

	//部品

	//変数
	SIZE	m_dotgain_size;							//!< Dotgain size
	HBITMAP m_dotgain_bmp;							//!< Dotgain bitmap
	HDC m_dotgain_dc;								//!< Dotgain DC

private:
	//変数
	
};


#endif // !defined(_CTL_TONE_CURVE_SETTINGS_H_)
