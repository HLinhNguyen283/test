/*! \file CtlScannedImage.cpp
 *  \brief The control displays the scanned image.
 *  \author GCS Dieu Dinh
 *  \date 2014/10/23 GCS Dieu Dinh created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlScannedImage.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "CommonConvertUnit.h"
#include "FlushingPatternLineWidth.h"
#include "DataPrintMode.h"

// ==========================================================================
// Local definitions
#define DEF_COLOR_WHITE					RGB(255, 255, 255)									//!< White color
#define DEF_COLOR_BLACK					RGB(0, 0, 0)										//!< Black color
#define DEF_SUBSCAN_DIRECTION_OFFSET	16													//!< Subscan direction offset is 16 mm from the top of page
#define DEF_TRACTOR_HOLE_DIAMETER		2.2													//!< The diameter of each hole (mm)
#define DEF_TRACTOR_HOLES_DISTANCE		ConvertUnit(0.5, DEF_LENGTH_INCH, DEF_LENGTH_MM)	//!< The distance between two holes (mm)(0.5 inch is converted to mm).
#define DEF_TRACTOR_HOLE_PAPER_DISTANCE	ConvertUnit(0.5, DEF_LENGTH_INCH, DEF_LENGTH_MM)	//!< The space of tractor hole on paper (mm)(0.5 inch is converted to mm).
#define DEF_PRE_PRINTED_MARK_WIDTH		3.7													//!< The width of pre-printed mark (mm)
#define DEF_PRE_PRINTED_MARK_HEIGHT		3.7													//!< The height of pre-printed mark (mm)
#define DEF_MAXIMUM_PAPER_WIDTH			520													//!< The maximum paper width is displayed (mm)
#define DEF_SPACE_EMPTY					10													//!< The space separate front and back
#define DEF_PRINT_START_MARK_WIDTH		3.7													//!< The printing start mark width
#define DEF_PRINT_START_MARK_HEIGHT		6													//!< The printing start mark height
#define DEF_SIZE_EACH_PERFORATION		8													//!< The size of each perforation (mm)
#define DEF_SPACE_BETWEEN_TWO_PERFORATIONS		4											//!< The space between two perforations (mm)
#define DEF_THICKNESS_PERFORATION		1													//!< The thickness of each perforation (mm)
#define DEF_RESO_BARCODE_BITMAP			15													//!< The resolution of barcode bitmap
#define DEF_RESO_BARCODE_MAX_BITMAP		45													//!< The maximum resolution of barcode bitmap
#define DEF_Y_COORDINATE_BARCODE_FRONT	30.0												//!< The y coordinate of barcode (Front) (mm)
#define DEF_Y_COORDINATE_BARCODE_BACK	(45.5 + 25.4)										//!< The y coordinate of barcode (Back) (mm)

// ==========================================================================

/*! \brief constructor
*/
CCtlScannedImage::CCtlScannedImage()
{
	// Set valid settings of events of the base class
	m_PreprintedMark_Offset.x = 0;
	m_PreprintedMark_Offset.y = 0;
	m_PrinterSide = DEF_PRINT_FRONT;
	m_hBitmapDisplay = NULL;
	m_imageRect.left = 0;
	m_imageRect.right = 0;
	m_imageRect.top = 0;
	m_imageRect.bottom = 0;
	m_PaperWidthMax = 0;
	m_PaperLengthMax = 0;
	m_systemUnit = DEF_LENGTH_MM;
	m_paperWidth = 0.0f;
	m_paperLength = 0.0f;
	m_preMarkOffsetFrontY = DEF_SUBSCAN_DIRECTION_OFFSET;
}

/*! \brief destructor
*/
CCtlScannedImage::~CCtlScannedImage()
{
}

/*! \fn CCtlScannedImage::UpdateControl()
 *  \brief Redraw the control image on the memmory DC.
*/
void CCtlScannedImage::UpdateControl()
{
	if(!m_hWnd){
		return;
	}

	// Initialize the all informations
	InitPaperInfo();
	// draw all into a memory DC
	DrawAll();

	// nofity redraw window
	RedrawWindow(m_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_INVALIDATE);
}

/*! \brief draw everything
*/
void CCtlScannedImage::DrawAll()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	//Draw background
	DrawBackground();

	//Draw white preview
	DrawWhiteImage();

	if (pData->IsSystemSupportJetInspection() && pData->GetOptionPreprintedPaper() && m_hBitmapDisplay) {
		//Draw scanned image
		DrawScannedImage();
	}

	//Draw all noninspection areas
	DrawAllNonInspectionAreas();

	if(pData->GetOptionTractorHolesAndPerforations()) {
		//Draw tractor holes
		DrawAllTractorHoles();

		//Draw perforations
		DrawPerforations();
	}else if(pData->GetOptionTractorHole()) {
		//Draw tractor holes
		DrawAllTractorHoles();
	}

	DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
	// Get option preprinted mark
	if(pData->GetOptionPreprintedMark(&printSide) && m_PrinterSide == printSide) {
		//Draw preprinted Mark
		DrawPrePrintedMark();
	}

	//Draw printing start mark
	DrawPrintingStartMark();

	//Draw verify mark (Barcode)
	DrawVerifyMark();
}

/*! \brief Initialize the paper information.
*/
void CCtlScannedImage::InitPaperInfo()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	//Get system length unit
	pData->GetSystemUnitLength(&m_systemUnit);

	//Paper width max
	{
		long maxWidth = 0;
		long minWidth = 0;
		long maxHeight = 0;
		long minHeight = 0;
		pData->GetMaxMinPaperSize(maxWidth, minWidth, maxHeight, minHeight);
		m_PaperWidthMax = ConvertUnit(((double)maxWidth / 1000 + 0.5), m_systemUnit, DEF_LENGTH_MM);
		m_PaperLengthMax = (double)maxHeight / 1000;
	}
	//~

	//Printer side
	m_PrinterSide = pData->GetPaperPreviewSide();
	//~

	m_preMarkOffsetFrontY = pData->GetPreMarkOffsetFrontY();

	//Pre printed mark information
	{
		//Sub scan direction
		if(DEF_PRINT_FRONT == m_PrinterSide) {
			m_PreprintedMark_Offset.y = Cnv2DisplaySize(m_preMarkOffsetFrontY);
		}
		else {
			m_PreprintedMark_Offset.y = Cnv2DisplaySize(DEF_SUBSCAN_DIRECTION_OFFSET);
		}

		//~
		//Main scan direction
		double offsetX = (double)pData->GetOptionPreprintedMarkOffset() / 1000;
		if(DEF_LENGTH_MM != m_systemUnit) {
			offsetX = ConvertUnit(offsetX, m_systemUnit, DEF_LENGTH_MM);
		}
		m_PreprintedMark_Offset.x = Cnv2DisplaySize(offsetX);
		//~
	}
	//~

	//Displayed image
	pData->GetPaperPreview(m_PrinterSide, &m_hBitmapDisplay);
	//~

	//Image rectangle
	{
		//Calculate paper length
		eUNIT_MODE lengthUnit = pData->GetPaperLengthUnit();
		bool isUsedLengthDefault = true;

		if(pData->GetUsePaperFeedLengthSetting()) {
			m_paperLength = (double)pData->GetPaperFeedLength() / 1000;

			if(m_paperLength > 0) {
				if(UNIT_MODE_MM != lengthUnit) {
					m_paperLength = ConvertUnit(m_paperLength, lengthUnit, UNIT_MODE_MM);
				}

				isUsedLengthDefault = false;
			}
		}

		//In case PrintConditionGUI is started from System settings screen:
		//	If the page feed length is off, getting the setting in ini file.
		//In case PrintConditionGUI is started form Job screen:
		//	If the page feed length is off, getting the RIP height of job to draw. If the RIP height of job can not be obtained, getting the setting in ini file.
		if(isUsedLengthDefault) {
			pData->GetRipSizeH(m_paperLength);
		}
		//~

		//Calculate paper width
		m_paperWidth = (double)pData->GetPaperWidth() / 1000;
		if(DEF_LENGTH_MM != m_systemUnit) {
			m_paperWidth = ConvertUnit(m_paperWidth, m_systemUnit, DEF_LENGTH_MM);
		}
		m_paperWidth = min(max(m_paperWidth, 0), m_PaperWidthMax);
		//~

		m_imageRect.left = 0;
		m_imageRect.right = Cnv2DisplaySize(m_paperWidth);
		m_imageRect.top = 0;
		m_imageRect.bottom = Cnv2DisplaySize(m_paperLength);
		if(DEF_PRINT_BACK == m_PrinterSide) {
			m_imageRect = Cnv2BackRect(m_imageRect);
		}
	}
	//~
}

/*! \brief Draw the background image.
*/
void CCtlScannedImage::DrawBackground()
{
	//Draw background
	RECT bgRect = { 0 };
	bgRect.left = 0;
	bgRect.top = 0;
	bgRect.right = m_size.cx;
	bgRect.bottom = m_size.cy;
	DrawFrameLayer(m_display_dc, bgRect, DEF_COLOR_BLACK);
}

/*! \brief Draw the preview image generated using GDI
*/
void CCtlScannedImage::DrawWhiteImage()
{
	//Draw the preview image generated using GDI
	DrawFrameLayer(m_display_dc, m_imageRect, DEF_COLOR_WHITE);
}

/*! \brief Draw the scanned image
*/
BOOL CCtlScannedImage::DrawScannedImage()
{
	if(!m_hBitmapDisplay) {
		return FALSE;
	}
	//Convert to pixel
	auto Cnv2Pixel = [&](double inSize, double inReso) -> LONG {
		return (LONG)((ConvertUnit(inSize, DEF_LENGTH_MM, DEF_LENGTH_INCH) * inReso) + 0.5);
	};

	//Draw bitmap on DC
	auto DrawImage = [&](HDC& inDC, RECT inDstRect, LONG inSrcWidth, LONG inSrcHeight, HBITMAP& inBmp, LONG inImageWidth) -> void {
		long sizeW = abs(inDstRect.right - inDstRect.left);
		long sizeH = abs(inDstRect.bottom - inDstRect.top);

		long offsetX = inDstRect.left;
		long offsetY = inDstRect.top;

		HDC tempDC = CreateCompatibleDC(inDC);
		HBITMAP tempOldBmp = (HBITMAP)SelectObject(tempDC, inBmp);
		long startXPos = 0;
		long startYPos = 0;
		if(DEF_PRINT_BACK == m_PrinterSide) {
			startXPos = inImageWidth - inSrcWidth;
			startYPos = 0;
		}
		SetStretchBltMode(inDC, HALFTONE);
		SetBrushOrgEx(inDC, 0, 0, NULL);
		StretchBlt(inDC, offsetX, offsetY, sizeW, sizeH, tempDC, startXPos, startYPos, inSrcWidth, inSrcHeight, SRCCOPY);

		//Release resource
		SelectObject(tempDC, tempOldBmp);
		DeleteDC(tempDC);
	};

	double resoX = 0.0f;
	double resoY = 0.0f;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(FALSE == pData->GetPaperReso(m_PrinterSide, resoX, resoY)) {
		return FALSE;
	}

	LONG srcWidth = Cnv2Pixel(m_paperWidth, resoX);
	LONG srcHeight = Cnv2Pixel(m_paperLength, resoY);
	LONG srcImageWidth = 0;

	if (m_hBitmapDisplay) {
		BITMAP bm = {0};
		::GetObject(m_hBitmapDisplay, sizeof(bm), &bm);
		srcImageWidth = bm.bmWidth;
	}

	//Draw scanned image
	DrawImage(m_display_dc, m_imageRect, srcWidth, srcHeight, m_hBitmapDisplay, srcImageWidth);

	return TRUE;
}

/*! \brief Draw the all of tractor holes
*/
void CCtlScannedImage::DrawAllTractorHoles()
{
	auto CreateCircleRect = [&](long inLeft, long inTop)->RECT{
		RECT rectHole = { 0 };
		rectHole.left = inLeft;
		rectHole.top = inTop;
		rectHole.right = rectHole.left + Cnv2DisplaySize(DEF_TRACTOR_HOLE_DIAMETER);
		rectHole.bottom = rectHole.top + Cnv2DisplaySize(DEF_TRACTOR_HOLE_DIAMETER);
		return rectHole;
	};

	long lPaperWidth = abs(m_imageRect.right - m_imageRect.left);
	long lPaperlength = abs(m_imageRect.bottom - m_imageRect.top);
	long distance = Cnv2DisplaySize(DEF_TRACTOR_HOLES_DISTANCE);

	long left1 = (Cnv2DisplaySize(DEF_TRACTOR_HOLE_PAPER_DISTANCE) - Cnv2DisplaySize(DEF_TRACTOR_HOLE_DIAMETER)) / 2;
	long left2 = lPaperWidth - left1 - Cnv2DisplaySize(DEF_TRACTOR_HOLE_DIAMETER);

	for(int pos = distance / 2; pos <= lPaperlength; pos += distance) {
		RECT circleRect1 = CreateCircleRect(left1, pos);
		RECT circleRect2 = CreateCircleRect(left2, pos);

		//If printer side is back side
		if(DEF_PRINT_BACK == m_PrinterSide) {
			circleRect1 = Cnv2BackRect(circleRect1);
			circleRect2 = Cnv2BackRect(circleRect2);
		}

		DrawTractorHole(circleRect1);
		DrawTractorHole(circleRect2);
	}
}

/*! \brief Draw the a tractor hole
 *  \param[IN] inRect: The rect of hole
*/
void CCtlScannedImage::DrawTractorHole(RECT inRect)
{
	if(inRect.bottom > m_imageRect.bottom) {
		return;
	}

	HPEN pen = CreatePen(PS_SOLID, 1, BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(m_display_dc, pen);

	Ellipse(m_display_dc, inRect.left, inRect.top, inRect.right, inRect.bottom);

	//Release resource
	SelectObject(m_display_dc, oldPen);
	DeleteObject(pen);
}

/*! \brief Draw the perforations
*/
void CCtlScannedImage::DrawPerforations()
{
	long sizeP = Cnv2DisplaySize(DEF_SIZE_EACH_PERFORATION);
	long space = Cnv2DisplaySize(DEF_SPACE_BETWEEN_TWO_PERFORATIONS);
	long firstPos = m_imageRect.left + space;
	long dstPos = firstPos + sizeP;

	//Create PEN
	long thickness = Cnv2DisplaySize(DEF_THICKNESS_PERFORATION);
	HPEN pen = CreatePen(PS_SOLID, thickness, BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(m_display_dc, pen);

	while(true) {

		//Check condition to break
		if(firstPos >= m_imageRect.right) {
			break;
		}

		if(dstPos > m_imageRect.right) {
			dstPos = m_imageRect.right;
		}

		//Top line
		MoveToEx(m_display_dc, firstPos, m_imageRect.top, nullptr);
		LineTo(m_display_dc, dstPos, m_imageRect.top);

		//Bottom line
		MoveToEx(m_display_dc, firstPos, m_imageRect.bottom - thickness, nullptr);
		LineTo(m_display_dc, dstPos, m_imageRect.bottom - thickness);

		//Check condition to break
		if(dstPos == m_imageRect.right) {
			break;
		}

		firstPos = dstPos + space;
		dstPos = firstPos + sizeP;
	}

	//Release resource
	SelectObject(m_display_dc, oldPen);
	DeleteObject(pen);
}

/*! \brief Draw the pre-printed mark
*/
void CCtlScannedImage::DrawPrePrintedMark()
{
	RECT prePrintedRect = { 0 };
	prePrintedRect.left = m_PreprintedMark_Offset.x;
	prePrintedRect.top = m_PreprintedMark_Offset.y;
	prePrintedRect.right = prePrintedRect.left + Cnv2DisplaySize(DEF_PRE_PRINTED_MARK_WIDTH);
	prePrintedRect.bottom = prePrintedRect.top + Cnv2DisplaySize(DEF_PRE_PRINTED_MARK_HEIGHT);

	if(DEF_PRINT_BACK == m_PrinterSide) {
		//it's supposed to be located at the left edge just like on the front side.
		LONG width = prePrintedRect.right - prePrintedRect.left;
		prePrintedRect.left = m_imageRect.left + prePrintedRect.left;
		prePrintedRect.right = prePrintedRect.left + width;
	}

	//Limit rectangle
	if(prePrintedRect.top >= m_imageRect.bottom) {
		return;
	}
	//Correct bottom
	if(prePrintedRect.bottom > m_imageRect.bottom) {
		prePrintedRect.bottom = m_imageRect.bottom;
	}

	//Draw preprinted mark
	DrawFrameLayer(m_display_dc, prePrintedRect, BLACK_BRUSH);
}

/*! \brief Draw the printing start mark (cue mark)
*/
void CCtlScannedImage::DrawPrintingStartMark()
{
	// 印刷条件名を取得する
	auto getPrintCondition = [](CDataIF* inDataIF, std::string& outPrintCondition)->bool
	{
		const char* printCondition = nullptr;
		bool selecting = FALSE;
		bool res = inDataIF->GetCurrentPrintCondition(&selecting, &printCondition);
		if (res) {
			outPrintCondition = printCondition;
		}
		else {
			WriteToLogBuf(LOG_DEBUG, "CCtlScannedImage::DrawPrintingStartMark failed GetCurrentPrintCondition.");
		}
		return res;
	};

	// フラッシングパターン情報を取得する
	auto getFlushingPattern = [](CDataIF* inDataIF, const std::string& inPrintCondition, ST_JOB_FLUSHING_PATTERN& outFlushingPattern, ST_JOB_FLUSHING_PATTERN_ADD& outFlushingPatternAdd, BOOL& outFlushingLineVerticalAlignment)->bool
	{
		bool res = inDataIF->GetFlushingPattern(inPrintCondition, outFlushingPattern, outFlushingPatternAdd, outFlushingLineVerticalAlignment);
		if (res) {
			WriteToLogBuf(LOG_DEBUG, "CCtlScannedImage::DrawPrintingStartMark failed GetFlushingPattern.");
		}
		return res;
	};
	
	// 出力色モードを取得する
	auto GetOutputColor = [](CDataIF* inDataIF)->DEF_OUTPUT_COLOR_MODE
	{
		long outputColor = 0;
		inDataIF->GetSelectOutputColor(&outputColor);
		return static_cast<DEF_OUTPUT_COLOR_MODE>(outputColor);
	};

	// ラインフラッシング情報を取得する
	auto getFlushingLine = [&](CDataIF* inDataIF, BOOL& outFlushingLineVerticalAlignment, double& outFlushingLineSize, double& outFlushingLineOffsetY)->void
	{
		std::string printCondition;							// 印刷条件名
		getPrintCondition(inDataIF, printCondition);

		ST_JOB_FLUSHING_PATTERN flushingPattern;			// フラッシングパターン情報
		ST_JOB_FLUSHING_PATTERN_ADD flushingPatternAdd;		// フラッシングパターンLineオプション情報
		getFlushingPattern(inDataIF, printCondition, flushingPattern, flushingPatternAdd, outFlushingLineVerticalAlignment);

		//Get resolution from print mode.
		PrintMode printMode;
		inDataIF->GetCurrentPrintMode(printMode);
		long resoX = printMode.Data.CMYK_PRN_RESO_X;
		long resoY = printMode.Data.CMYK_PRN_RESO_Y;

		// ラインフラッシングのサイズ
		outFlushingLineSize = CFlushingPatternLineWidth::CalculateForJob(
			inDataIF->IsMonochromePrint_F(),
			flushingPattern.flushingPattern,
			flushingPattern.gap,
			GetOutputColor(inDataIF) == DEF_OUTPUT_COLOR_MODE_CMYK? false : true,
			static_cast<long>(resoX),
			static_cast<long>(resoY),
			flushingPatternAdd);

		// ラインフラッシングのオフセット位置
		outFlushingLineOffsetY = flushingPattern.offsetY[0];
	};


	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(FALSE == pData->GetEnablePrintStartMark()) {
		return;
	}

	double offsetX = 0;
	double offsetY = 0;

	//Get mark informations
	double verifyMarkOffsetX = 0;
	if(FALSE == pData->GetVerifyMarkOffset(m_PrinterSide, verifyMarkOffsetX, offsetY)) {
		return;
	}

	if(DEF_PRINT_FRONT == m_PrinterSide) {
		DEF_PRINT_SIDE preprintedMarkPrintSide = DEF_PRINT_FRONT;
		bool isPreprintedMark = pData->GetOptionPreprintedMark(&preprintedMarkPrintSide);
		if(isPreprintedMark){
			if(DEF_PRINT_FRONT == preprintedMarkPrintSide) {
				offsetX = pData->GetOptionPreprintedMarkOffset() / 1000.0;
			}
			if(DEF_PRINT_BACK == preprintedMarkPrintSide) {
				offsetX = verifyMarkOffsetX;
			}
		}
		else {
			offsetX = verifyMarkOffsetX;
		}

		//Y座標
		offsetY = 0.0;
		if(isPreprintedMark){
			if(DEF_PRINT_FRONT == preprintedMarkPrintSide) {
				if (m_preMarkOffsetFrontY == DEF_PREPRINT_MARK_Y_MM) {
					offsetY = 0.0;
				}
				else {
					BOOL flushingLineVerticalAlignment = false;					// フラッシングページの位置
					double flushingLineSize = 0.0;								// フラッシングページのサイズ
					double flushingLineOffsetY = 0.0;							// フラッシングページのオフセット位置
					getFlushingLine(pData, flushingLineVerticalAlignment, flushingLineSize, flushingLineOffsetY);

					if ((flushingLineVerticalAlignment == FALSE) && ((flushingLineSize + flushingLineOffsetY) >= 6.0 && (flushingLineSize + flushingLineOffsetY) <= 8.0)) {
						// ラインフラッシングが上端 & サイズ(6～8mm)
						offsetY = ConvertUnit(12.0, DEF_LENGTH_MM, m_systemUnit);
					}
					else if ((flushingLineVerticalAlignment == TRUE) && (flushingLineSize >= 4.0 && flushingLineSize <= 8.0)) {
						// ラインフラッシングが下端 & サイズ(4～8mm)
						offsetY = ConvertUnit(12.0, DEF_LENGTH_MM, m_systemUnit);
					}
					else {
						// それ以外
						offsetY = ConvertUnit(16.0, DEF_LENGTH_MM, m_systemUnit);
					}
				}
			}
		}
	}
	else if(DEF_PRINT_BACK == m_PrinterSide) {
		if(pData->IsSystemSupportJetInspection()) {
			offsetX = verifyMarkOffsetX;
			offsetY = 0;
			//Right-end conversion later
		}
		else {
			//Do not draw.
			return;
		}
	}

	//Prepare info to draw
	if(DEF_LENGTH_MM != m_systemUnit) {
		offsetX = ConvertUnit(offsetX, m_systemUnit, DEF_LENGTH_MM);
		offsetY = ConvertUnit(offsetY, m_systemUnit, DEF_LENGTH_MM);
	}

	long lPaperWidth = abs(m_imageRect.right - m_imageRect.left);
	long sizeW = max(5, Cnv2DisplaySize(DEF_PRINT_START_MARK_WIDTH));
	long sizeH = max(5, Cnv2DisplaySize(DEF_PRINT_START_MARK_HEIGHT));

	//Calculate rect of mark
	RECT printStartRect = { 0 };
	printStartRect.left = min(lPaperWidth - sizeW, max(0, Cnv2DisplaySize(offsetX)));
	printStartRect.top = Cnv2DisplaySize(offsetY);
	printStartRect.right = printStartRect.left + sizeW;
	printStartRect.bottom = printStartRect.top + sizeH;

	if(DEF_PRINT_BACK == m_PrinterSide) {
		printStartRect = Cnv2BackRect(printStartRect);
	}

	//Draw printing start mark
	DrawFrameLayer(m_display_dc, printStartRect, BLACK_BRUSH);
}

/*! \brief Draw the verify mark (barcode)
*/
void CCtlScannedImage::DrawVerifyMark()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(FALSE == pData->GetVerifyMarkTarget()) {
		return;
	}

	//**Draw verify mark**

	//Prepare info to draw
	long sizeW = 0;
	long sizeH = 0;
	long srcSizeW = 0;
	long srcSizeH = 0;
	long offsetX = 0;
	long offsetY = 0;
	double markOffsetX = 0;
	double markOffsetY = 0;
	long imageWidth = abs(m_imageRect.right - m_imageRect.left);
	long imageHeight = abs(m_imageRect.bottom - m_imageRect.top);

	if(imageWidth <= 0 || imageHeight <= 0) {
		return;
	}

	//Get mark informations
	if(FALSE == pData->GetVerifyMarkOffset(m_PrinterSide, markOffsetX, markOffsetY)) {
		return;
	}

	//Prepare info to draw
	if(DEF_LENGTH_MM != m_systemUnit) {
		markOffsetX = ConvertUnit(markOffsetX, m_systemUnit, DEF_LENGTH_MM);
		markOffsetY = ConvertUnit(markOffsetY, m_systemUnit, DEF_LENGTH_MM);
	}

	//Load resource
	HBITMAP bmp = LoadResourceBitmap(IDB_VERIFY_MARK, RESOURCE_BMP);
	if(!bmp) {
		return;
	}
	{
		BITMAP bm = {0};
		::GetObject(bmp, sizeof(bm), &bm);
		srcSizeW = bm.bmWidth;
		srcSizeH = bm.bmHeight;
		sizeW = max(5, (long)( ((double)srcSizeW * DEF_RESO_BARCODE_BITMAP / DEF_RESO_BARCODE_MAX_BITMAP) + 0.5) );
		sizeH = max(5, (long)( ((double)srcSizeH * DEF_RESO_BARCODE_BITMAP / DEF_RESO_BARCODE_MAX_BITMAP) + 0.5) );
	}

	if(DEF_PRINT_FRONT == m_PrinterSide){
		//Calculate X coordinate
		offsetX = min( imageWidth - sizeW, max(0, Cnv2DisplaySize(markOffsetX)) );

		//Y coordinate is 30.0mm
		offsetY = Cnv2DisplaySize(DEF_Y_COORDINATE_BARCODE_FRONT);
	}else{
		//Calculate X coordinate
		offsetX = m_size.cx - sizeW - min( imageWidth - sizeW, max(0, Cnv2DisplaySize(markOffsetX)) );

		//Y coordinate is 45.5mm + 25.4mm + GUI
		offsetY = imageHeight - (Cnv2DisplaySize(DEF_Y_COORDINATE_BARCODE_BACK) + Cnv2DisplaySize(markOffsetY));
	}

	//limit drawing area
	if((offsetY + sizeH) > m_imageRect.bottom) {
		sizeH -= ((offsetY + sizeH) - m_imageRect.bottom);
	}

	//Draw bitmap
	HDC barcodeDC = CreateCompatibleDC(m_display_dc);
	HBITMAP barcodeOldBmp = (HBITMAP)SelectObject(barcodeDC, bmp);

	StretchBlt(m_display_dc, offsetX, offsetY, sizeW, sizeH, barcodeDC, 0, 0, srcSizeW, srcSizeH, SRCCOPY);

	SelectObject(barcodeDC, barcodeOldBmp);
	DeleteDC(barcodeDC);
	//~

	//**~
}

/*! \brief Draw the all noninspection areas
*/
void CCtlScannedImage::DrawAllNonInspectionAreas()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	RECT frameRect = {0};
	double elimitnatedLeft = 0.0f;
	double elimitnatedRight = 0.0f;

	// Get info of emilinated area left
	pData->GetEliminatedInfo(m_PrinterSide, true, elimitnatedLeft, elimitnatedRight);
	frameRect.left = Cnv2DisplaySize(elimitnatedLeft);
	frameRect.right = Cnv2DisplaySize(elimitnatedRight);
	frameRect.top = 0;
	frameRect.bottom = m_imageRect.bottom;

	// Draw emilinated area left
	DrawNonInspectionArea(frameRect);
	elimitnatedLeft = 0.0f;
	elimitnatedRight = 0.0f;

	// Get info of emilinated area Right
	pData->GetEliminatedInfo(m_PrinterSide, false, elimitnatedLeft, elimitnatedRight);
	frameRect.left = Cnv2DisplaySize(elimitnatedLeft);
	frameRect.right = Cnv2DisplaySize(elimitnatedRight);
	frameRect.top = 0;
	frameRect.bottom = m_imageRect.bottom;

	// Draw emilinated area Right
	DrawNonInspectionArea(frameRect);
	memset(&frameRect, 0, sizeof(frameRect));

	long itemID = 0;
	while(TRUE == pData->GetNonInspectionRect(itemID++, m_PrinterSide, frameRect)) {
		frameRect.left = Cnv2DisplaySize(frameRect.left);
		frameRect.top = Cnv2DisplaySize(frameRect.top);
		frameRect.right = Cnv2DisplaySize(frameRect.right);
		frameRect.bottom = Cnv2DisplaySize(frameRect.bottom);

		if(DEF_PRINT_BACK == m_PrinterSide) {
			//Convert to position of back side
			LONG widthArea = frameRect.right - frameRect.left;

			frameRect.left = (m_imageRect.right - m_imageRect.left) - frameRect.left;
			frameRect.right = frameRect.left + widthArea;
		}

		//Draw a non inspection area.
		DrawNonInspectionArea(frameRect);
		memset(&frameRect, 0, sizeof(frameRect));
	}
}

/*! \brief Draw the a noninspection area
 *  \param[IN] inRect: the rect of a non inspection area
*/
void CCtlScannedImage::DrawNonInspectionArea(RECT inRect)
{
	// Update position of view
	inRect.left = inRect.left + m_imageRect.left;
	inRect.right = inRect.right + m_imageRect.left;

	// Get size of rect
	SIZE frameSize = {0};
	frameSize.cx = abs(inRect.right - inRect.left);
	frameSize.cy = abs(inRect.bottom - inRect.top);
	// Draw transparent color
	{
		RECT fillRect = { 0, 0, frameSize.cx - 1, frameSize.cy - 1 };
		// Fill gray rect on base image.
		HDC transparentDC = CreateCompatibleDC(m_display_dc);
		HBITMAP hBmp = ::CreateCompatibleBitmap(m_display_dc, frameSize.cx, frameSize.cy);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(transparentDC, hBmp);

		HBRUSH colorBrush = CreateSolidBrush(GRAY_BRUSH);
		FillRect(transparentDC, &fillRect, colorBrush);
		DeleteObject(colorBrush);

		// blend the stretched image with the existing on destination DC
		BLENDFUNCTION blend = {0};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 120;

		::AlphaBlend(m_display_dc,
			inRect.left, inRect.top,
			frameSize.cx, frameSize.cy,
			transparentDC,
			0, 0,
			frameSize.cx, frameSize.cy,
			blend);
		SelectObject(transparentDC, hOldBmp);
		DeleteObject(hBmp);
		DeleteDC(transparentDC);
	}
}

/*! \brief Convert from front rect to back rect.
 *  \param inRect[IN]: The rectangle.
*/
RECT CCtlScannedImage::Cnv2BackRect(RECT inRect)
{
	RECT rect = inRect;
	rect.left = m_size.cx - inRect.left - (inRect.right - inRect.left);
	rect.right = rect.left + (inRect.right - inRect.left);
	return rect;
}

/*! \brief Convert size from mm to pixel
 *	\ param[in] inSize: input size (mm)
 *	retVal: a ouput size (pixel)
*/
long CCtlScannedImage::Cnv2DisplaySize(double inSize)
{
	return (long)( ((inSize/m_PaperWidthMax) * (m_size.cx - DEF_SPACE_EMPTY)) + 0.5 );
}

/*! \brief draw a frame layer
 *	\ param[IN] inHDC: the DC is drawed
 *	\ param[IN] inRect: rect of frame
 *	\ param[IN] inColor: color of frame
*/
void CCtlScannedImage::DrawFrameLayer(HDC inHDC, RECT inRect, COLORREF inColor)
{
	// Get size of rect
	SIZE frameSize = { 0 };
	frameSize.cx = abs(inRect.right - inRect.left);
	frameSize.cy = abs(inRect.bottom - inRect.top);
	// Draw black color
	{
		RECT fillRect = { 0, 0, frameSize.cx, frameSize.cy};
		// Fill black rect on base image.
		HDC layerDC = CreateCompatibleDC(inHDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(inHDC, frameSize.cx, frameSize.cy);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(layerDC, hBmp);


		HBRUSH colorBrush = CreateSolidBrush(inColor);
		FillRect(layerDC, &fillRect, colorBrush);
		DeleteObject(colorBrush);

		BitBlt(inHDC, inRect.left, inRect.top, frameSize.cx, frameSize.cy, layerDC, 0, 0, SRCCOPY);

		//Release resource
		SelectObject(layerDC, hOldBmp);
		DeleteObject(hBmp);
		DeleteDC(layerDC);
	}
}