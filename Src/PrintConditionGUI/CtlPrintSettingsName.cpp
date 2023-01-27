/*! \file CtlPrintSettingsName.cpp
 *  \brief The control display print setting name and job name.
 *  \author GCS Vien Nguyen
 *  \date 2013/12/04 GCS Vien Nguyen created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintSettingsName.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"
//
#include "CommonUiMsg_OP.h"
#include "DlgPrintConditionDeleting.h"
#include "DlgAddPrintSetting.h"

// ==========================================================================
// Local definitions
// ==========================================================================
#define LENGTH_CONDITION_JOBNAME 100	//!< length of string condition job name.
//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< static-box: icon
	CTRLID_SC_PRINT_SETTING_NAME,			//!< pull-down: print condition selection

	CTLID_COUNT
};

/*! \brief constructor
*/
CCtlPrintSettingsName::CCtlPrintSettingsName(void)
{
	m_ctlCount = CTLID_COUNT;
	memset(m_RollFlagimage, 0x00, sizeof(m_RollFlagimage));
	m_PrintSettingsStartID = 0;
}

/*! \brief desctructor
*/
CCtlPrintSettingsName::~CCtlPrintSettingsName(void)
{

}

/*! \brief [event]set the control's property
*/
void CCtlPrintSettingsName::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]
	//static-box: icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 50, 15, 50 + 60, 15 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}

	// static-box: condition - jobname.
	{
		long ctlId = CTRLID_SC_PRINT_SETTING_NAME;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_NORMAL | SCST_LEFT | SCST_TEXT;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 120, 17, 120 + 675, 17 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrintSettingsName::OnCreateItem()
{
	// create image icon for print condition
	CreatePrintSettingsFlagImage();
}
/*! \brief コントロールのアイテムを破棄するイベント
*/
void CCtlPrintSettingsName::OnDeleteItem()
{
	// Delete merged paper option bitmap object of all pattern
	DestroyPrintSettingsFlagImage();
}

/*! \brief コントロールステータス更新イベント
*/
void  CCtlPrintSettingsName::OnUpdateState()
{
	// hide all control to test 
	if(m_ctl)
	{
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		// check dialog displayed from SystemSetting hide all control 
		DWORD dwState = CST_SHOW;
		if(pData->IsStartFromSystemSetting())
		{
			dwState = CST_HIDE;
		}
		for(int ctlID = 0; ctlID < CTLID_COUNT; ctlID++)
		{
			SetControlState(m_ctl[ctlID], dwState);
		}
	}
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrintSettingsName::OnUpdateValue()
{
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

		long printSettingsID = 0;

		bool bIsHold = false;
		bool bIsHoleAndPerforat = false;
		bool bIsPreprintedMark = false;
		bool bIsPreprintedPaper = false;
		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;
		// hard code print condition id
		pData->GetPrintConditionListPaperOption(printSettingsID, &bIsHold, &bIsHoleAndPerforat, &bIsPreprintedMark, &bIsPreprintedPaper, &printSide);
		
		// set image for icon 
		SetControlData(m_ctl[CTRLID_SC_ICON],(DWORD)GetPrintSettingsFlagImage(bIsHold, bIsHoleAndPerforat, bIsPreprintedMark, bIsPreprintedPaper, printSide));

		// set text for print setting name.
		char* jobName = NULL;
		bool outSelecting = false;
		const char* currentCondition = NULL;
		// get job name.
		// get print current print condition
		if (pData->GetJobName(&jobName) && pData->GetCurrentPrintCondition(&outSelecting, &currentCondition))
		{
			char conditionJobName[LENGTH_CONDITION_JOBNAME];
			// merge name follow format
//			if (pData->IsUpdatedCurrentPrintSetting())
			if (pData->IsPrintConditionEdited())
			{
				_snprintf_s(conditionJobName, _TRUNCATE, "%s %s (%s)", currentCondition, LoadResourceString(IDS_PRINT_SETTINGS_EDITED, RESOURCE_STR), jobName);
			}
			else
			{
				_snprintf_s(conditionJobName, _TRUNCATE, "%s (%s)", currentCondition, jobName);
			}

			SetControlData(m_ctl[CTRLID_SC_PRINT_SETTING_NAME],(DWORD)conditionJobName);
		}
}

/*! \brief Create a composite image of print settings base on paper option flag.
*/
void CCtlPrintSettingsName::CreatePrintSettingsFlagImage()
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
void CCtlPrintSettingsName::DestroyPrintSettingsFlagImage()
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
HBITMAP CCtlPrintSettingsName::GetPrintSettingsFlagImage(bool inIsHole, bool inIsHoleAndPerforat, bool inIsPrePrintMark, bool inIsPrePrint, BOOL inFace)
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
HBITMAP CCtlPrintSettingsName::CreateImage(HWND hWnd, int nWidth, int nHeight, BYTE* pData)
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
BOOL CCtlPrintSettingsName::MergeBitmapBuff(HWND hWnd, HBITMAP srcBmp, SIZE dstSize, BYTE* dstData)
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
BOOL CCtlPrintSettingsName::MergeBitmap(HWND hWnd, HBITMAP* srcBmp, long srcBmpCount, HBITMAP* dstBmp)
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