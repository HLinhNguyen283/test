/*! \file CtlPrintSettingsSelection.cpp
 *  \brief The control selects to edit print settings
 *  \author GCS Tan Ho
 *  \date 2013/06/14 GCS Tan Ho created
*/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrintSettingsSelection.h"
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
#define DEF_COLOR_BLUE			RGB(0, 0, 255)
#define DEF_COLOR_BLACK			RGB(0, 0, 0)
// ==========================================================================

//! the control id list
enum{
	CTRLID_SC_ICON = 0,					//!< static-box: icon
	CTRLID_PD_PRINT_SETTINGS,			//!< pull-down: print condition selection

	CTLID_COUNT
};

//! Print settings operation
typedef enum{
	DEF_PRINT_SETTINGS_OPERATOR_ADD = 1,		//!< Add operator
	DEF_PRINT_SETTINGS_OPERATOR_DELETE			//!< Delete operator
}DEF_PRINT_SETTINGS_OPERATION;

// ==========================================================================

/*! \brief constructor
*/
CCtlPrintSettingsSelection::CCtlPrintSettingsSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

	memset(m_RollFlagimage, 0x0, sizeof(m_RollFlagimage));
	m_PrintSettingsStartID = 0;
	m_CurrentSelection = 0;
}

/*! \brief destructor
*/
CCtlPrintSettingsSelection::~CCtlPrintSettingsSelection()
{
}


/*! \brief [event]set the control's property
*/
void CCtlPrintSettingsSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]
	//static-box: icon
	{
		long ctlId = CTRLID_SC_ICON;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_STRETCH;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 35, 17, 35 + 60, 17 + 60);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINT_SETTING, RESOURCE_BMP);
	}

	//pull-down: Print condition menu

	{
		long ctlId = CTRLID_PD_PRINT_SETTINGS;
		m_ctlAttribute[ctlId].type = CT_PULLDOWN;
		m_ctlAttribute[ctlId].style = CST_HIDE | PMST_TEXT | PMST_BITMAP | PMST_QUERY;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 105, 17, 105 + 675, 17 + 60);
		m_ctlAttribute[ctlId].param = NULL;
	}
}

/*! \brief コントロールのプロパティを設定するイベント
*/
void CCtlPrintSettingsSelection::OnCreateItem()
{
	// Create the merged paper option bitmap objects of all ON/OFF pattern
	CreatePrintSettingsFlagImage();

	// Registers items for print condition menu pull-down
	this->RegisterPrintConditionItems();
}

/*! \brief コントロールのアイテムを破棄するイベント
*/
void CCtlPrintSettingsSelection::OnDeleteItem()
{
	// Delete merged paper option bitmap object of all pattern
	DestroyPrintSettingsFlagImage();
}

/*! \brief ウィンドウプロシージャ
*  \param hWnd ウィンドウハンドル
*  \param Message メッセージ番号
*  \param wParam パラメータ
*  \param lParam パラメータ
*  \return DEF_NONE
*/
long CCtlPrintSettingsSelection::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case UWM_PRINT_CONDITION_LIST:
		{
			CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

			PDITEMINFO pd_item = {0};

			const char* strPrintCondition = NULL;
			bool bIsHold = false;
			bool bIsHoleAndPerforat = false;
			bool bIsPreprintedMark = false;
			bool bIsPreprintedPaper = false;
			DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

			switch(wParam){
			case CDataIF::DEF_UPDATE_ADD_ITEM:
				{
					// add the print condition item
					if(TRUE == pData->GetPrintConditionList(lParam, &strPrintCondition)){
						pd_item.text = (char*)strPrintCondition;

						if(pData->GetPrintConditionListPaperOption(lParam, &bIsHold, &bIsHoleAndPerforat, &bIsPreprintedMark, &bIsPreprintedPaper, &printSide)){
							pd_item.hImage = GetPrintSettingsFlagImage(bIsHold, bIsHoleAndPerforat, bIsPreprintedMark, bIsPreprintedPaper, printSide);
						}

						SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], m_PrintSettingsStartID + lParam, &pd_item);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_EDIT_ITEM:
				{
					// change the print condition item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], lParam);
					if(selectedItem){
						if(TRUE == pData->GetPrintConditionList(lParam, &strPrintCondition)){
							selectedItem->text = (char*)strPrintCondition;
							if(pData->GetPrintConditionListPaperOption(lParam, &bIsHold, &bIsHoleAndPerforat, &bIsPreprintedMark, &bIsPreprintedPaper, &printSide)){
								pd_item.hImage = GetPrintSettingsFlagImage(bIsHold, bIsHoleAndPerforat, bIsPreprintedMark, bIsPreprintedPaper, printSide);
							}
							// change and update item at selected index
							SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], m_PrintSettingsStartID + lParam, selectedItem);
						}
					}
				}
				break;
			case CDataIF::DEF_UPDATE_DEL_ITEM:
				{
					// delete the prnt condition item
					PDITEMINFO* selectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], lParam);
					if(selectedItem){
						// delete the selected item
						SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], m_PrintSettingsStartID + lParam, NULL);
					}
				}
				break;
			case CDataIF::DEF_UPDATE_REREGISTER_ALL_ITEM:
				{
					// Clear all items
					while(NULL != (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], 0)){
						SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], 0, NULL);
					}

					// re-regiseter all items of print condition
					this->RegisterPrintConditionItems();
				}
				break;
			}
		}

		break;
	}

	return DEF_NONE;
}

/*! \brief [event]the control operated
*  \param hWnd window handle
*  \param Message messag id
*  \param wParam parameter 1
*  \param lParam parameter 2
*  \return DEF_NONE : continue showing this screen.
*  \return DEF_EXIT_PAGE : close this screen.
*/
long CCtlPrintSettingsSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// Print condition menu pull-down
			if(ctlWnd == m_ctl[CTRLID_PD_PRINT_SETTINGS]){
				// The selected print settings is not save yet
				if(!pData->IsSavedPrintSetting()){
					//Display a dialog-box to confirm
					char msg[256] = {0};
					_snprintf(msg, sizeof(msg) - 1, "%d\n%s", ID_MESSAGE_ERROR + IDM_DISCARD_CHANGES_CONFIRMATION, LoadResourceString(IDM_DISCARD_CHANGES_CONFIRMATION, RESOURCE_MSG_STR));

					if(IDNO == ShowMessageBox(msg, MBST_YESNO | MBST_ICONINFORMATION, NULL)){
						pData->SetChangeParam();
						return DEF_NONE;
					}
				}

				long nSelectedIndex = GetControlData(m_ctl[CTRLID_PD_PRINT_SETTINGS]);
				PDITEMINFO* pdSelectedItem = (PDITEMINFO*)GetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], nSelectedIndex);
				if(pdSelectedItem){
					// if the current selection is a print condition
					if(NULL == pdSelectedItem->data){
						// set the selected print condition
						pData->SetSelectPrintCondition(nSelectedIndex - m_PrintSettingsStartID);

					}else{
						long nOperation = (long)pdSelectedItem->data;
						// if the current selection is Add operator
						if(DEF_PRINT_SETTINGS_OPERATOR_ADD == nOperation){
							// Notify to display Add print settings dialog tho DataIF
							pData->InitAddPrintConditionDlg();
							// Display dialog
							DlgAddPrintSetting dlg;
							dlg.Create(m_data, m_printer);
						}
						// if the current selection is Delete operator
						else if(DEF_PRINT_SETTINGS_OPERATOR_DELETE == nOperation){
							// initialize to display delete print condition dialog
							pData->InitDeletePrintConditionDlg();

							// show the delete print condition dialog
							DlgPrintConditionDeleting dlg;
							dlg.Create(m_data, m_printer);
						}
					}
				}
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief コントロール表示値更新イベント
*/
void CCtlPrintSettingsSelection::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	// Set a default selection to print condition pull-down menu
	long nSelection = 0;
	if(pData->GetSelectPrintCondition(&nSelection)){
		SetControlData(m_ctl[CTRLID_PD_PRINT_SETTINGS], m_PrintSettingsStartID + nSelection);
	}
}

/*! \brief コントロールステータス更新イベント
*/
void  CCtlPrintSettingsSelection::OnUpdateState()
{
	if(m_ctl)
	{
		CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
		// check dialog displayed from SystemSetting show all control 
		DWORD dwState = CST_HIDE;
		if(pData->IsStartFromSystemSetting())
		{
			dwState = CST_SHOW;
		}
		
		for(int ctlID = 0; ctlID < CTLID_COUNT; ctlID++)
		{
			SetControlState(m_ctl[ctlID],dwState);
		}
	}
}

/*! \brief Used to register print condition items
*/
void CCtlPrintSettingsSelection::RegisterPrintConditionItems()
{
	// Registers items for print condition menu pull-down
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	long nItemId = 0;

	// Add and Delete item
	{
		PDITEMINFO pd_item = {0};
		pd_item.hImage = NULL;
		pd_item.no_query = TRUE;

		BOOL bIsAllowAdd;
		BOOL bIsAllowDelete;
		// The user mode of the system is General
		if(USER_ID_GENERAL == pData->GetSystemUserMode()){
			bIsAllowAdd = pData->IsPermissionAddPrintCondition();
			bIsAllowDelete = pData->IsPermissionDeletePrintCondition();
		} else {
			bIsAllowAdd = true;
			bIsAllowDelete = true;
		}

		// add an add operator
		if(bIsAllowAdd){
			pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_ADD, RESOURCE_STR);
			pd_item.data = (void*)DEF_PRINT_SETTINGS_OPERATOR_ADD;
			SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], nItemId++, &pd_item);
		}

		// add a delete operator
		if(bIsAllowDelete){
			pd_item.text = (char*)LoadResourceString(IDS_OPERATOR_DELETE, RESOURCE_STR);
			pd_item.data = (void*)DEF_PRINT_SETTINGS_OPERATOR_DELETE;
			SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], nItemId++, &pd_item);
		}

		// add a seperator
		if(bIsAllowAdd || bIsAllowDelete){
			char chSeparator[4] = {0};
			_snprintf(chSeparator, sizeof(chSeparator) - 1, "-");

			pd_item.text = chSeparator;
			pd_item.data = NULL;
			SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], nItemId++, &pd_item);
		}
	}

	// All print settings
	{
		// Backup the index of the first print setting item.
		m_PrintSettingsStartID = nItemId;

		const char* strPrintSettings = NULL;
		PDITEMINFO pd_item = {0};
		pd_item.crTextEnabled = TRUE;

		long printSettingsID = 0;

		bool bIsHold = false;
		bool bIsHoleAndPerforat = false;
		bool bIsPreprintedMark = false;
		bool bIsPreprintedPaper = false;
		DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

		// Get current print condition
		bool bIsCurrPrintCondition = false;
		const char* strCurrPrintCondition = NULL;
		bool bIsSucceedGetCurrPrintCondition = pData->GetCurrentPrintCondition(&bIsCurrPrintCondition, &strCurrPrintCondition);

		while(TRUE){
			if(FALSE == pData->GetPrintConditionList(printSettingsID, &strPrintSettings)){
				break;
			}

			// add a condition item
			pd_item.text = (char*)strPrintSettings;

			// when the print condition is the same as printer's current print condition
			if(true == bIsSucceedGetCurrPrintCondition
				&& 0 == strcmp(strPrintSettings, strCurrPrintCondition)){
				// set text color to blue
				pd_item.crText = DEF_COLOR_BLUE;
			}else{
				// set text color to black
				pd_item.crText = DEF_COLOR_BLACK;
			}

			// 画像.
			if(pData->GetPrintConditionListPaperOption(printSettingsID++, &bIsHold, &bIsHoleAndPerforat, &bIsPreprintedMark, &bIsPreprintedPaper, &printSide)){
				pd_item.hImage = GetPrintSettingsFlagImage(bIsHold, bIsHoleAndPerforat, bIsPreprintedMark, bIsPreprintedPaper, printSide);
			}
			SetControlItem(m_ctl[CTRLID_PD_PRINT_SETTINGS], nItemId++, &pd_item);

			strPrintSettings = NULL;
		}
	}

	OnUpdateValue();
}

/*! \brief Create a composite image of print settings base on paper option flag.
*/
void CCtlPrintSettingsSelection::CreatePrintSettingsFlagImage()
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
void CCtlPrintSettingsSelection::DestroyPrintSettingsFlagImage()
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
HBITMAP CCtlPrintSettingsSelection::GetPrintSettingsFlagImage(bool inIsHole, bool inIsHoleAndPerforat, bool inIsPrePrintMark, bool inIsPrePrint, BOOL inFace)
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
HBITMAP CCtlPrintSettingsSelection::CreateImage(HWND hWnd, int nWidth, int nHeight, BYTE* pData)
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
BOOL CCtlPrintSettingsSelection::MergeBitmapBuff(HWND hWnd, HBITMAP srcBmp, SIZE dstSize, BYTE* dstData)
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
BOOL CCtlPrintSettingsSelection::MergeBitmap(HWND hWnd, HBITMAP* srcBmp, long srcBmpCount, HBITMAP* dstBmp)
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