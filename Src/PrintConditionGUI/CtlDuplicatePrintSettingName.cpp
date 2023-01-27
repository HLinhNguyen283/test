/*! \file CtlDuplicatePrintSettingName.cpp
 *  \brief duplicate print setting name control for adding print settings
 *  \author GCS Quang Vo
 *  \date 2013/05/17 GCS Quang Vo created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlDuplicatePrintSettingName.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
//! the control id list
enum{
	CTRLID_ST_NAME = 0,					//!< label
	CTRLID_PD_DUPLICATE,				//!< duplicate print setting name menu pull-down

	CTLID_COUNT
};
// ==========================================================================

/*! \brief constructor
*/
CCtlDuplicatePrintSettingName::CCtlDuplicatePrintSettingName()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

	memset(m_RollFlagimage, 0x0, sizeof(m_RollFlagimage));
}

/*! \brief destructor
*/
CCtlDuplicatePrintSettingName::~CCtlDuplicatePrintSettingName()
{
}


/*! \brief [event]set the control's property
*/
void CCtlDuplicatePrintSettingName::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//label: name
	{
		long ctlId = CTRLID_ST_NAME;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_TEXT | SCST_LEFT;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_LABLE_PRINT_SETTING_NAME_DUPLICATE, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 15, 105, 15 + 95, 105 + 40);
		m_ctlAttribute[ctlId].param = NULL;
	}

	//pull-down: print setting name
	{
		long ctlId = CTRLID_PD_DUPLICATE;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_BITMAP | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 120,  95, 120 + 670, 95 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlDuplicatePrintSettingName::OnCreateItem()
{
	// Create merged paper option bitmap object of all ON/OFF pattern
	CreatePrintSettingsFlagImage();

	// registers items
	this->RegisterPrintSettingItem();
}

/*! \brief コントロールのアイテムを破棄するイベント
*/
void CCtlDuplicatePrintSettingName::OnDeleteItem()
{
	// Delete merged paper option bitmap object of all pattern
	DestroyPrintSettingsFlagImage();
}

/*! \brief [event]the control operated
 *  \param hWnd window handle
 *  \param Message messag id
 *  \param wParam parameter 1
 *  \param lParam parameter 2
 *  \return DEF_NONE : continue showing this screen.
 *  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlDuplicatePrintSettingName::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;

	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case CBN_SELCHANGE:
		{
			// duplicate print setting name menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_DUPLICATE]){
				// change the selection print setting name
				long lItemID = GetControlData(m_ctl[CTRLID_PD_DUPLICATE]);
				pData->SetSelectPrintCondition_AddDlg(lItemID);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロールステータス更新イベント
*/
void CCtlDuplicatePrintSettingName::OnUpdateState()
{
	if(m_ctl){
		// always show all controls
		for(long ctlId = 0; ctlId < m_ctlCount; ++ctlId){
			SetControlState(m_ctl[ctlId], CST_SHOW);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlDuplicatePrintSettingName::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	// display the selection print setting name
	long lItemID = 0;
	if(pData->GetSelectPrintCondition_AddDlg(&lItemID)){
		SetControlData(m_ctl[CTRLID_PD_DUPLICATE], lItemID);
	}
}

/*! \brief used to registers item for menu pull-down
*/
void CCtlDuplicatePrintSettingName::RegisterPrintSettingItem()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Registers all print settings to the menu pull-down
	{
		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
		bool bIsHole = false;
		bool bIsHoleAndPerforat = false;
		bool bIsPreprintedMark = false;
		bool bIsPreprintedPaper = false;

		// current print condition
		bool bIsSelected = false;
		const char* strCurPrintConditionName = NULL;
		bool bHasCurPrintCondition = pData->GetCurrentPrintCondition_AddDlg(&bIsSelected, &strCurPrintConditionName);

		// loop to get a list of print condition name
		PDITEMINFO pd_item = {0};
		pd_item.crTextEnabled = TRUE;

		long lItemId = 0;
		const char* strPrintConditionName = NULL;
		while(pData->GetPrintConditionList_AddDlg(lItemId, &strPrintConditionName)){
			// add a print condition name
			pd_item.text = (char*)strPrintConditionName;

			// when the print condition is the same as the current one
			if(bHasCurPrintCondition && 0 == strcmp(strCurPrintConditionName, strPrintConditionName)){
				// set text color to blue
				pd_item.crText = RGB(0, 0, 255);
			}else{
				// set text color to black
				pd_item.crText = RGB(0, 0, 0);
			}

			// set a merged paper option image of a print setting
			if(pData->GetPrintConditionListPaperOption_AddDlg(lItemId, &bIsHole, &bIsHoleAndPerforat, &bIsPreprintedMark, &bIsPreprintedPaper, &printSide)){
				pd_item.hImage = GetPrintSettingsFlagImage(bIsHole, bIsHoleAndPerforat, bIsPreprintedMark, bIsPreprintedPaper, printSide);
			}

			SetControlItem(m_ctl[CTRLID_PD_DUPLICATE], lItemId++, &pd_item);

			// reset data
			strPrintConditionName = NULL;
		}
	}
}

/*! \brief Create a composite image of print settings base on paper option flag.
*/
void CCtlDuplicatePrintSettingName::CreatePrintSettingsFlagImage()
{
	// Synthesized all the images of the pattern paper option flag.
	HBITMAP bmp_None = LoadResourceBitmap(IDB_ROLLTYPE_NONE, RESOURCE_BMP);				// None
	
	HBITMAP bmp_pt1 = LoadResourceBitmap(IDB_ROLLTYPE_PREPRINTMARK_B, RESOURCE_BMP);	// Preprinted Mark: Back
	HBITMAP bmp_pt2 = LoadResourceBitmap(IDB_ROLLTYPE_PREPRINTMARK_F, RESOURCE_BMP);	// Preprinted Mard: Front
	HBITMAP bmp_pt3 = LoadResourceBitmap(IDB_ROLLTYPE_HOLE, RESOURCE_BMP);				// Hole
	HBITMAP bmp_pt4 = LoadResourceBitmap(IDB_ROLLTYPE_HOLE_PERF, RESOURCE_BMP);			// Hole and Preforation
	HBITMAP bmp_pt5 = LoadResourceBitmap(IDB_ROLLTYPE_PREPRINT, RESOURCE_BMP);			// Preprinted Paper

	// Synthesize None bitmap with 4 type bitmap (paper option flag)

	HBITMAP src[2] = {0};
	HBITMAP tmp[5] = {0};

	BOOL isMerge = FALSE;

	// 5 paper option type -> 32 pattern

	// Priority of bitmap will be synthesized (from low to high)
	// 1) Preprinted paper
	// 2) Hole and perforat, Hole
	// 3) Preprinted mark (front, back side)
	for (int i = 0; i < 32; ++i) {
		memset(tmp, 0x00, sizeof(tmp));
		src[0] = bmp_None;
		{
			if ((i & 1) == 1) {	// Preprinted paper
				// Synthesized
				src[1] = bmp_pt5;
				isMerge = TRUE;
				MergeBitmap(GetGenSetupDialog(), src, 2, &tmp[0]);
				src[1] = NULL;
			}
			if (isMerge) {
				src[0] = tmp[0];
				isMerge = FALSE;
			}

			if ((i & 2) == 2) {	// Hole and perforat.
				// Synthesized.
				src[1] = bmp_pt4;
				isMerge = TRUE;
				MergeBitmap(GetGenSetupDialog(), src, 2, &tmp[1]);
				src[1] = NULL;
			}
			if (isMerge) {
				src[0] = tmp[1];
				isMerge = FALSE;
			}

			if ((i & 4) == 4) {		// Hole.
				// Synthesized.
				src[1] = bmp_pt3;
				isMerge = TRUE;
				MergeBitmap(GetGenSetupDialog(), src, 2, &tmp[2]);
				src[1] = NULL;
			}
			if (isMerge) {
				src[0] = tmp[2];
				isMerge = FALSE;
			}

			if ((i & 8) == 8) {		// Preprinted mark: Front
				// Synthesized.
				src[1] = bmp_pt2;
				isMerge = TRUE;
				MergeBitmap(GetGenSetupDialog(), src, 2, &tmp[3]);
				src[1] = NULL;
			}
			if (isMerge) {
				src[0] = tmp[3];
				isMerge = FALSE;
			}

			if ((i & 16) == 16) {		// Preprinted mark: Back
				// Synthesized.
				src[1] = bmp_pt1;
				isMerge = TRUE;
				MergeBitmap(GetGenSetupDialog(), src, 2, &tmp[4]);
				src[1] = NULL;
			}
			if (isMerge) {
				src[0] = tmp[4];
				isMerge = FALSE;
			}

			// Save the result images
			m_RollFlagimage[i] = src[0];

			// Release temporary object
			for (int idx = 0; idx < 5; ++idx) {
				if(tmp[idx] && (m_RollFlagimage[i] != tmp[idx])){
					DeleteObject(tmp[idx]);
					tmp[idx] = NULL;
				}
			}
		}
	}
}

/*! \brief Detroy the composite images of print settings
*/
void CCtlDuplicatePrintSettingName::DestroyPrintSettingsFlagImage()
{
	// Release all composite images.
	HBITMAP bmp_None = LoadResourceBitmap(IDB_ROLLTYPE_NONE, RESOURCE_BMP);
	long imageSize = (sizeof(m_RollFlagimage) / sizeof(HBITMAP));
	for (int i = 0; i < imageSize; ++i) {	
		if(m_RollFlagimage[i] && (bmp_None != m_RollFlagimage[i])){
			DeleteObject(m_RollFlagimage[i]);
			m_RollFlagimage[i] = NULL;
		}
	}	
}

/*! \brief Get the print settings composite image base on the paper option flag.
 *  \param inIsHole: Hole flag
 *  \param inIsHoleAndPerforat: Hole and Perforat flag
 *  \param inIsPrePrintMark: Preprinted mark flag
 *  \param inIsPrePrint: Preprinted paper flag
 *  \param inFace: 0 is Front, 1 is Back
 *  \return Composite image HBITMAP. If can not get image return NULL
*/
HBITMAP CCtlDuplicatePrintSettingName::GetPrintSettingsFlagImage(bool inIsHole, bool inIsHoleAndPerforat, bool inIsPrePrintMark, bool inIsPrePrint, BOOL inFace)
{
	// The preprinted mark has two case: front or back
	int marked = 0;
	
	if (inFace == 0 || inFace == 1) {
		if (inIsPrePrintMark) {
			// If inFace is Front side, the marked is 8.
			// If inFace is Back side, the marked is 16.
			marked = ((inFace == 0) ? 8 : 16);
		}
	}

	int index = (inIsHole ? 4:0) + (inIsHoleAndPerforat ? 2 : 0) + marked + (inIsPrePrint ? 1: 0);

	if (index < 0 || index > 23) {
		return NULL;
	}
	return m_RollFlagimage[index];
}

/*! \brief バッファからビットマップ作成
 *  \param hWnd			[IN]ウィンドウハンドル
 *  \param nWidth		[IN]作成するビットマップの幅
 *  \param nHeight		[IN]作成するビットマップの高さ
 *  \param pData		[IN]バッファ
 *  \return ビットマップオブジェクト
*/
HBITMAP CCtlDuplicatePrintSettingName::CreateImage(HWND hWnd, int nWidth, int nHeight, BYTE* pData)
{
	if (nWidth <= 0 || nHeight <= 0 || pData == NULL)
	{
		return NULL;
	}

	HDC hDC = GetDC(hWnd);
 
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nWidth;
	bmi.bmiHeader.biHeight = nHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* bitData = NULL;
	HBITMAP hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &bitData, NULL, NULL);
	if(bitData){
		if(pData){
			memcpy(bitData, pData, nWidth * nHeight * bmi.bmiHeader.biPlanes * bmi.bmiHeader.biBitCount / 8);
		}else{
			memset(bitData, 0x00, nWidth * nHeight * bmi.bmiHeader.biPlanes * bmi.bmiHeader.biBitCount / 8);
		}
	}
	::ReleaseDC(hWnd, hDC);

	return hBitmap;
}

/*! \brief ビットマップバッファを合成する
 *  \param hWnd			[IN]ウィンドウハンドル
 *  \param srcBmp		[IN]合成元ビットマップオブジェクトハンドル
 *  \param dstSize		[IN]dstDataの画像サイズ
 *  \param dstData		[OUT]合成後のバッファ
 *  \return TRUE：成功
*/
BOOL CCtlDuplicatePrintSettingName::MergeBitmapBuff(HWND hWnd, HBITMAP srcBmp, SIZE dstSize, BYTE* dstData)
{
	//元画像バッファを取得する
	SIZE srcSize = {0};
	BYTE* srcData = NULL;
	{
		//サイズ取得
		BITMAP bm = {0};
		if(!::GetObject(srcBmp, sizeof(bm), &bm)){
			return FALSE;
		}
		srcSize.cx = bm.bmWidth;
		srcSize.cy = bm.bmHeight;
		//バッファ作成
		srcData = new BYTE[srcSize.cx * srcSize.cy * 4];
		if(!srcData){
			return FALSE;
		}
		memset(srcData, 0x00, srcSize.cx * srcSize.cy * 4);

		//絵柄取得
		HDC hDC = GetDC(hWnd);
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = srcSize.cx;
		bmi.bmiHeader.biHeight = srcSize.cy;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		::GetDIBits(hDC, srcBmp, 0, srcSize.cy, srcData, &bmi, DIB_RGB_COLORS);

		ReleaseDC(hWnd, hDC);
	}

	//画像バッファを合成する(Xチャンネルでマスクする)
	{
		DWORD srcPos = 0;
		DWORD dstPos = 0;
		//1行あたりの余りサイズ算出
		SIZE minSize = {min(srcSize.cx, dstSize.cx), min(srcSize.cy, dstSize.cy)};
		DWORD srcLineRestSize = (srcSize.cx - minSize.cx) * 4;
		DWORD dstLineRestSize = (dstSize.cx - minSize.cx) * 4;
		for(long y = 0; y < minSize.cy; y++){
			for(long x = 0; x < minSize.cx; x++){
				//マスクされてなければ合成後バッファの画素を上書きする
				if(srcData[srcPos + 3] != 0){
					memcpy(&dstData[dstPos], &srcData[srcPos], 4);
				}
				srcPos += 4;
				dstPos += 4;
			}
			srcPos += srcLineRestSize;
			dstPos += dstLineRestSize;
		}
	}

	//バッファ解放
	delete [] srcData;

	return TRUE;
}

/*! \brief 複数のビットマップを合成する
 *  \param hWnd			[IN]ウィンドウハンドル
 *  \param srcBmp		[IN]合成元ビットマップオブジェクトハンドル配列
 *  \param srcBmpCount	[IN]srcBmpの配列数
 *  \param dstBmp		[OUT]合成後のビットマップオブジェクトハンドルへのポインタ
 *  \return TRUE：成功
*/
BOOL CCtlDuplicatePrintSettingName::MergeBitmap(HWND hWnd, HBITMAP* srcBmp, long srcBmpCount, HBITMAP* dstBmp)
{
	if(!srcBmp || srcBmpCount <= 0 || !dstBmp){
		return FALSE;
	}
	//合成用バッファを作成する
	SIZE dstSize = {0};
	BYTE* dstData = NULL;
	{
		BITMAP bm = {0};
		if(!::GetObject(srcBmp[0], sizeof(bm), &bm)){
			return FALSE;
		}
		dstSize.cx = bm.bmWidth;
		dstSize.cy = bm.bmHeight;

		dstData = new BYTE[dstSize.cx * dstSize.cy * 4];
		memset(dstData, 0x00, dstSize.cx * dstSize.cy * 4);
	}

	//合成する
	for(int i = 0; i < srcBmpCount; i++){
		if(!MergeBitmapBuff(hWnd, srcBmp[i], dstSize, dstData)){
			delete[] dstData;
			return FALSE;
		}
	}

	//合成後のバッファからビットマップオブジェクトを作成する(オブジェクトの解放は呼び出し側に任せる)
	*dstBmp = CreateImage(hWnd, dstSize.cx, dstSize.cy, dstData);
	delete [] dstData;
	
	return TRUE;
}