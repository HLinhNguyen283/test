/*! \file CtlSideSelection.cpp
 *  \brief The control selects front side or back side for preprinted mark.
 *  \author GCS Tan Ho
 *  \date 2013/05/20 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlSideSelection.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_SIZE_BUTTON					71
//! the control id list
enum{
	CTRLID_BN_FRONT = 0,			//!< button: front side
	CTRLID_BN_BACK,					//!< button: back side

	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_ctl[CTLID_COUNT] = {"CTRLID_BN_FRONT", "CTRLID_BN_BACK"};
// ==========================================================================

/*! \brief constructor
*/
CCtlSideSelection::CCtlSideSelection()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlSideSelection::~CCtlSideSelection()
{
}


/*! \brief [event]set the control's property
*/
void CCtlSideSelection::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	//button: front side
	{
		long ctlId = CTRLID_BN_FRONT;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 0, 0, DEF_SIZE_BUTTON, DEF_SIZE_BUTTON);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_MARK_FRONT, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
	}
	
	//button: back side
	{
		long ctlId = CTRLID_BN_BACK;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_GRADATION | BNST_BITMAP | BNST_IMAGE_BLEND;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 0, DEF_SIZE_BUTTON, DEF_SIZE_BUTTON, DEF_SIZE_BUTTON + DEF_SIZE_BUTTON);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PAPER_MARK_BACK, RESOURCE_BMP);
		m_ctlAttribute[ctlId].logmsg = st_logmsg_ctl[ctlId];
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
long CCtlSideSelection::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case BN_CLICKED:
		{
			DEF_PRINT_SIDE printSide = DEF_PRINT_FRONT;

			if(true == ConvertCtlIdToPrintSideType(ctlWnd, &printSide)){
				// Change a selection side
				pData->SetSelectPreprintedMarkSide_SelectDlg(printSide);
				
				// Notify close dialog
				pData->ExitSelectPreprintedMarkSideDlg();
				return DEF_EXIT_PAGE;
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief Convert a control ID to both a print side type.
 *  \param inWnd : a window handle of the controls.
 *  \param outPrintSide : a pointer to get a print side type.
 *  \retval true : converting succeed
 *  \retval true : converting failed.(inWnd is not one of print side button)
*/
bool CCtlSideSelection::ConvertCtlIdToPrintSideType(HWND inWnd, DEF_PRINT_SIDE* outPrintSide)
{
	// Front side button
	if(inWnd == m_ctl[CTRLID_BN_FRONT]){
		*outPrintSide = DEF_PRINT_FRONT;
	}
	// Back side button
	else if(inWnd == m_ctl[CTRLID_BN_BACK]){
		*outPrintSide = DEF_PRINT_BACK;
	}
	else{
		return false;
	}

	return true;
}