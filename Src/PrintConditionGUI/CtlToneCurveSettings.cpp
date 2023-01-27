/*! \file CtlToneCurveSettings.cpp
 *  \brief The control displays Tone settings dialog.
 *  \author GCS Tan Ho
 *  \date 2013/05/15 GCS Tan Ho created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlToneCurveSettings.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_ALL_COLOR						RGB(128, 128, 128)		//!< Gray color of "all color" curve
#define DEF_COLOR_NAME_C					_T("C")					//!< Cyan color name
#define DEF_COLOR_NAME_M					_T("M")					//!< Magenta color name
#define DEF_COLOR_NAME_Y					_T("Y")					//!< Yellow color name
#define DEF_COLOR_NAME_K					_T("K")					//!< Black color name
#define DEF_COLOR_NAME_R					_T("R")					//!< Red color name
#define DEF_COLOR_NAME_O					_T("O")					//!< Orange color name
#define STR_EMPTY							_T("")					//!< Empty string
//! the control id list
enum{
	CTRLID_BN_TONE_SETTINGS = 0,				//!< Button: Tone settings

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_tone_setting = "CTRLID_BN_TONE_SETTINGS";
// ==========================================================================

/*! \brief constructor
*/
CCtlToneCurveSettings::CCtlToneCurveSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

	m_dotgain_size.cx = 0;
	m_dotgain_size.cy = 0;
	m_dotgain_bmp = NULL;
	m_dotgain_dc = NULL;
}

/*! \brief destructor
*/
CCtlToneCurveSettings::~CCtlToneCurveSettings()
{
	FinDotGainBitmap();
}

/*! \brief [event]set the control's property
*/
void CCtlToneCurveSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button： Tone settings
	{
		long ctlId = CTRLID_BN_TONE_SETTINGS;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_TEXT | BNST_SBAR | BNST_GRADATION | BNST_DISABLE_BLACK;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 1040, 280, 1040 + 130, 280 + 70);
		m_ctlAttribute[ctlId].param = NULL;
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_tone_setting;
	}
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlToneCurveSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam){
	case BN_CLICKED:
		{
			// Tone settings button
			if(ctlWnd == m_ctl[CTRLID_BN_TONE_SETTINGS]){
				// Display Tone settings dialog
				pData->InitToneSettingDlg();
				pData->SetChangeParam();
			}
		}
		break;
	}
	
	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlToneCurveSettings::OnUpdateState()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	DWORD dwState = CST_SHOW;

	// Disable control when the selected print settings is locked.
	if(true == pData->GetLockPrintCondtion() || pData->IsDisablePrintCondition()){
		dwState |= CST_DISABLED;
	}
	else{
		dwState |= CST_ENABLED;
	}

	SetControlState(m_ctl[CTRLID_BN_TONE_SETTINGS], dwState);
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlToneCurveSettings::OnUpdateValue()
{
	//Draw dot gain curve
	SIZE size = {100, 50};
	InitDotGainBitmap(m_ctl[CTRLID_BN_TONE_SETTINGS], size);
	DrawDotGainBitmap();

	BNITEMINFO bn_item = {0};
	bn_item.nMask = OT_BITMAP | OT_TEXT;
	bn_item.hImage = m_dotgain_bmp;
	bn_item.lpszText = (char*)LoadResourceString(IDS_TONE, RESOURCE_STR);

	SetControlItem(m_ctl[CTRLID_BN_TONE_SETTINGS], 0, &bn_item);
}

/*! \fn void InitDotGainBitmap(HWND inHWnd, SIZE inSizeImage)
 * \brief Initialize the parameter for dot gain curve
 * \param hWnd Window hanlde.
 * \param inSizeImage Image size
*/
void CCtlToneCurveSettings::InitDotGainBitmap(HWND inHWnd, SIZE inSizeImage)
{
	m_dotgain_size = inSizeImage;
	//Create bitmap
	{
		char* pData = new char[m_dotgain_size.cx*m_dotgain_size.cy*4];
		memset(pData, 0x00, m_dotgain_size.cx*m_dotgain_size.cy*4);
		if(m_dotgain_bmp){
			::DeleteObject(m_dotgain_bmp);
			m_dotgain_bmp = NULL;
		}

		m_dotgain_bmp = CreateImage(inHWnd, m_dotgain_size.cx, m_dotgain_size.cy, pData);
		delete [] pData;
		pData = NULL;
	}

	//Create DC
	{
		HDC hdc = ::GetDC(inHWnd);
		if(m_dotgain_dc){
			::DeleteDC(m_dotgain_dc); 
			m_dotgain_dc = NULL;
		}

		m_dotgain_dc = CreateCompatibleDC(hdc);
		::ReleaseDC(inHWnd, hdc);
	}
}

/*! \fn void CCtlToneCurveSettings::FinDotGainBitmap()
 * \brief Delete the parameter for dot gain curve
*/
void CCtlToneCurveSettings::FinDotGainBitmap()
{
	if(m_dotgain_dc){
		::DeleteDC(m_dotgain_dc); 
		m_dotgain_dc = NULL;
	}
	if(m_dotgain_bmp){
		::DeleteObject(m_dotgain_bmp);
		m_dotgain_bmp = NULL;
	}
}

/*! \fn void CCtlToneCurveSettings::DrawDotGainBitmap()
 * \brief Draw bitmap of dot gain curve
*/
void CCtlToneCurveSettings::DrawDotGainBitmap()
{
	auto MappingAnchorColorWithInkColor = [](int anchorColorID)->const char *{
		switch (anchorColorID) {
		case ANCHOR_COL_CYAN:
			{
				//Cyan
				return DEF_COLOR_NAME_C;
			}
		case ANCHOR_COL_MAGENTA:
			{
				//Magenta
				return DEF_COLOR_NAME_M;
			}
		case ANCHOR_COL_YELLOW:
			{
				//Yellow
				return DEF_COLOR_NAME_Y;
			}
		case ANCHOR_COL_BLACK:
			{
				//Black
				return DEF_COLOR_NAME_K;
			}
		case ANCHOR_COL_RED:
			{
				//Red
				return DEF_COLOR_NAME_R;
			}
		case ANCHOR_COL_ORANGE:
			{
				//Red
				return DEF_COLOR_NAME_O;
			}
		default :
			return STR_EMPTY;
		}
	};
	
	ST_PDB_DOTGAIN_CURVE dotGainCurve = {0};
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	bool bNotDefault = !pData->IsDotgainCurveDefault_ForDrawing();
	//Check printer side
	DEF_PRINT_SIDE printerSide = (pData->IsDED() && pData->IsMaster()) ? DEF_PRINT_BACK :DEF_PRINT_FRONT;

	if(!pData->GetDotgainCurve_ForDrawing(dotGainCurve)){
		for (long lIndx = 0; lIndx < ANCHOR_COL_MAX; ++lIndx) {
			pData->GetLinearDotGainData_ForDrawing(dotGainCurve.ANCHOR_POINT[lIndx]);
		}
		bNotDefault = false;
	}

	HDC hdc = m_dotgain_dc;
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hdc, m_dotgain_bmp);

	//get tone frame color from ini file
	COLORREF colorRefSetting  = pData->GetToneFrameColor_ForDrawing();

	//	Obtain RGB color and frame width information here
	RECT rect = {0, 0, m_dotgain_size.cx, m_dotgain_size.cy};
	//Fill
	{
		COLORREF colorRef = bNotDefault ? colorRefSetting: RGB(204,204,204);
		HBRUSH brush = CreateSolidBrush(colorRef);	// bNotDefault is true if tone curve of either side is not default
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}

	if(bNotDefault){
		//get tone frame width from ini file
		long frameWidth = pData->GetToneFrameWidth_ForDrawing();

		RECT rect_inner = {frameWidth, frameWidth, m_dotgain_size.cx-frameWidth, m_dotgain_size.cy-frameWidth};
		//Fill
		{
			HBRUSH brush = CreateSolidBrush(RGB(204,204,204));
			FillRect(hdc, &rect_inner, brush);
			DeleteObject(brush);
		}
	}
	RECT rect_grah = {5, 5, m_dotgain_size.cx -5, m_dotgain_size.cy -5};
	{
		HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
		FrameRect(hdc, &rect_grah, brush);
		DeleteObject(brush);
	}

	HPEN hPen = (HPEN)::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	//Draw the line
	MoveToEx(hdc, rect_grah.left, rect_grah.bottom -1, NULL);
	LineTo(hdc, rect_grah.right, rect_grah.top); 

	::SelectObject(hdc, hOldPen);
	if(hPen){
		::DeleteObject(hPen);
		hPen = NULL;
	}

	short dotgain_curve[DOTGAIN_CURVE_PT_CNT] = {0};
	//Display the curve on the button
	SIZE sizeGraph = {rect_grah.right - rect_grah.left, rect_grah.bottom - rect_grah.top};
	long tmp_y = 0;
	POINT draw_curve[DOTGAIN_CURVE_PT_CNT] = {0};

	for (long lIndx = ANCHOR_COL_MAX-1; lIndx > -1; --lIndx) {
		COLORREF outColor = 0;
		if((ANCHOR_COL_ALL != lIndx) && (!pData->GetDisplayColor(printerSide, MappingAnchorColorWithInkColor(lIndx), outColor))) {
			continue;
		}
		memset(dotgain_curve, 0x00, sizeof(dotgain_curve));
		CreateDotGainCurve(dotGainCurve.ANCHOR_POINT[lIndx], dotgain_curve);
		memset(draw_curve, 0x00, sizeof(draw_curve));
		tmp_y = 0;

		for(int x = 0; x < DOTGAIN_CURVE_PT_CNT; ++x){
			draw_curve[x].x = x * sizeGraph.cx / MAX_DOTGAIN_CURVE_PT_X + 5;
			tmp_y = dotgain_curve[x];
			//Out of range values
			if(tmp_y < MIN_DOTGAIN_CURVE_PT_Y){
				tmp_y = MIN_DOTGAIN_CURVE_PT_Y;
			}else if(tmp_y > MAX_DOTGAIN_CURVE_PT_Y){
				tmp_y = MAX_DOTGAIN_CURVE_PT_Y;
			}
			draw_curve[x].y = sizeGraph.cy - (tmp_y * sizeGraph.cy / MAX_DOTGAIN_CURVE_PT_Y) + 4;
		}
		//Create color for anchor curve.
		outColor = (ANCHOR_COL_ALL == lIndx) ? DEF_ALL_COLOR : outColor;
		HPEN hPenGraph = (HPEN)::CreatePen(PS_SOLID, 1, outColor);
		hOldPen = (HPEN)::SelectObject(hdc, hPenGraph);
		::Polyline(hdc, draw_curve, DOTGAIN_CURVE_PT_CNT);
		::SelectObject(hdc, hOldPen);
		if(hPenGraph){
			::DeleteObject(hPenGraph);
			hPenGraph = NULL;
		}
	}

	::SelectObject(hdc, hOldBitmap);
	if(hdc){
		::DeleteDC(hdc);
		hdc = NULL;
	} 
	if(hOldBitmap){
		::DeleteObject(hOldBitmap);
		hOldBitmap = NULL;
	}
}

/*! \fn HBITMAP CCtlToneCurveSettings::CreateImage(HWND inHWnd, int inWidth, int inHeight, char* inData)
 *  \brief Create bitmap
 *  \param inHWnd	Window handle
 *  \param inWidth	Bitmap width
 *  \param inHeight	Bitmap height
 *  \param inData	Original data
 *  \return Bitmap
*/
HBITMAP CCtlToneCurveSettings::CreateImage(HWND inHWnd, int inWidth, int inHeight, char* inData)
{
	if (inWidth <= 0 || inHeight <= 0 || inData == NULL || inHWnd == NULL)
	{
		return NULL;
	}

	HDC hDC = GetDC(inHWnd);
 
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = inWidth;
	bmi.bmiHeader.biHeight = inHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HBITMAP hBitmap = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT, inData, &bmi, DIB_RGB_COLORS);
	::ReleaseDC(inHWnd, hDC);

	return hBitmap;
}

/*! \fn void CCtlToneCurveSettings::CreateDotGainCurve(DOTGAIN_ANCHOR* inAnchor, short* outDotgainCurve)
 *  \brief Create a dot gain curve
 *  \param inAnchor [IN]Anchor point information of 1ch
 *  \param outDotgainCurve [OUT]Dot gain curve
*/
void CCtlToneCurveSettings::CreateDotGainCurve(ST_PDB_ANCHOR_POINT* inAnchor, short* outDotgainCurve)
{
	if((inAnchor == NULL) || (outDotgainCurve == NULL)){
		return;
	}

	int anchor_cnt = 0;
	int cnt = 0;
	int index = 0;
	int pt_x[ANCHOR_PT_CNT] = {0};
	int pt_y[ANCHOR_PT_CNT] = {0};
	short dst_buff[DOTGAIN_CURVE_PT_CNT] = {0};
	short src_buff[DOTGAIN_CURVE_PT_CNT] = {0};
	short work_buff[DOTGAIN_CURVE_PT_CNT] = {0};

	short pt_y_left = 0;
	int mode = 4;
	int nImageFuncRet = 0;

	for(cnt = 0; cnt < ANCHOR_PT_CNT; ++cnt){
		if(inAnchor[cnt].KIND == ANCHOR_KIND_CURVEDRAW){
			anchor_cnt++;
		}
	}

	//there is no information to create a dot gain curve
	if(anchor_cnt == 0){
		char pLog[256] = {0};
		_snprintf(pLog, sizeof(pLog) -1, "%s [Line:%d]", "CCtlToneCurveSettings::CreateDotGainCurve Func Err", __LINE__);
		WriteToLogBuf(LOG_DEBUG, pLog);
		return;
	}

	for(cnt = 0, index = 0; cnt < ANCHOR_PT_CNT; ++cnt){
		if(inAnchor[cnt].KIND == ANCHOR_KIND_CURVEDRAW){
			pt_x[index] = inAnchor[cnt].POSITION.x / DEF_ANCHOR_PT_UNIT;
			pt_y[index] = inAnchor[cnt].POSITION.y / DEF_ANCHOR_PT_UNIT;
			++index;
		}
	}

	//Save the Y coordinate of the left edge
	{
		pt_y_left = (short)(inAnchor[ANCHOR_PT_CNT -1].POSITION.y / DEF_ANCHOR_PT_UNIT);
	}

	for(cnt = 0; cnt < DOTGAIN_CURVE_PT_CNT; cnt++){
		src_buff[cnt] = cnt;
	}

	ImageData srcdata, dstdata, workdata;
	memset(&srcdata, 0x00, sizeof(srcdata));
	memset(&dstdata, 0x00, sizeof(dstdata));
	memset(&workdata, 0x00, sizeof(workdata));
	srcdata.width = DOTGAIN_CURVE_PT_CNT;
	srcdata.height = 1;
	srcdata.bps = 16;
	srcdata.spp = 1;
	dstdata = srcdata;
	workdata = dstdata;
	srcdata.data = src_buff;
	dstdata.data = dst_buff;
	workdata.data = work_buff;

	nImageFuncRet = IMAGELIB_ImageLUT(&srcdata, &dstdata, pt_y, pt_x, anchor_cnt, mode, &workdata);
	if(nImageFuncRet != 0){
		//failure
		char pLogMsg[256] = {0};
		_snprintf(pLogMsg, sizeof(pLogMsg) -1, "%s [Line:%d]", "CCtlToneCurveSettings::CreateDotGainCurve Func Err", __LINE__);
		WriteToLogBuf(LOG_DEBUG, pLogMsg);

		char* pLog = (char*)IMAGELIB_ImageGetStatusString(nImageFuncRet);
		if(pLog){
			char log_text[256] = {0};
			_snprintf(log_text, sizeof(log_text) -1, "IMAGELIB_ImageLUT Func Err [%s]", pLog);
			WriteToLogBuf(LOG_DEBUG, log_text);
		}
		return;
	}
	dst_buff[DOTGAIN_CURVE_PT_CNT -1] = pt_y_left;

	memcpy(outDotgainCurve, dst_buff, sizeof(short) * DOTGAIN_CURVE_PT_CNT);
}