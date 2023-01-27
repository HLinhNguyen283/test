/*! \file CtlScannedImage.h
 *  \brief The control displays the scanned image.
 *  \author GCS Dieu Dinh
 *  \date 2014/10/23 GCS Dieu Dinh created
*/

#if !defined(_CTL_SCANNED_IMAGE_H_)
#define _CTL_SCANNED_IMAGE_H_



// header file
#include "BaseSpCtl.h"
#include "CommonDef_OP.h"
#include "DataIF.h"
// ==========================================================================
// Local definitions
// ==========================================================================

/*!
 *  \brief The control displays the scanned image.
*/
class CCtlScannedImage : public CBaseSpCtl
{
public:
	//定数

	//関数
	CCtlScannedImage();
	virtual ~CCtlScannedImage();
	virtual void UpdateControl();

protected:
	// member methods
	void DrawAll();
	void InitPaperInfo();
	void DrawBackground();
	void DrawWhiteImage();
	BOOL DrawScannedImage();
	void DrawAllTractorHoles();
	void DrawTractorHole(RECT inRect);
	void DrawPerforations();
	void DrawPrePrintedMark();
	void DrawPrintingStartMark(); //cue mark
	void DrawVerifyMark(); //Barcode
	void DrawAllNonInspectionAreas();
	void DrawNonInspectionArea(RECT inRect);
	RECT Cnv2BackRect(RECT inRect);
	long Cnv2DisplaySize(double inSize);
	void DrawFrameLayer(HDC inHDC, RECT inRect, COLORREF inColor);

	//Variables
	double m_PaperWidthMax;				//!< The maximum paper width
	double m_PaperLengthMax;			//!< The maximum paper length
	POINT m_PreprintedMark_Offset;		//!< Offset Y of pre-printed mark
	DEF_PRINT_SIDE m_PrinterSide;		//!< The current printer side
	RECT m_imageRect;					//!< The image rect
	HBITMAP m_hBitmapDisplay;			//!< The bitmap is displayed
	DEF_LENGTH_UNIT m_systemUnit;		//!< The current unit of system
	double m_paperWidth;				//!< The paper width
	double m_paperLength;				//!< The paper length
	long m_preMarkOffsetFrontY;			//!< 表面の事前印刷マークの副走査オフセット(mm)

private:
	//変数
};

#endif // !defined(_CTL_SCANNED_IMAGE_H_)
