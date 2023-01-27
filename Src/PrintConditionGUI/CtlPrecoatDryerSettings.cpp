/*! \file CtlPrecoatDryerSettings.cpp
 *  \brief The control edits a precoast setting for printer 1, printer 2.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlPrecoatDryerSettings.h"
#include "Common.h"
#include "CommonCBRProc.h"
#include "CommonUiMsg.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions
#define DEF_H_EDITBOX						62
#define DEF_W_EDITBOX						133
#define DEF_MAX_LENGTH						3
#define DEF_ICON_SIZE						45
#define DEF_PRINTER_2						2
#define DEF_PRINTER_1						1
//! the control id list
enum{
	CTRLID_SC_PRECOAT_IMAGE = 0,			//!< static-box: Precoat image
	CTRLID_EB_PRECOAT_PRINTER_2,			//!< edit-box: Precoat for printer 2
	CTRLID_EB_PRECOAT_PRINTER_1,			//!< edit-box: Precoat for printer 1
	CTRLID_SC_UNIT_PRECOAT,					//!< static-box: Unit of precoat

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlPrecoatDryerSettings::CCtlPrecoatDryerSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlPrecoatDryerSettings::~CCtlPrecoatDryerSettings()
{
}


/*! \brief [event]set the control's property
*/
void CCtlPrecoatDryerSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: Precoat image
	{
		long ctlId = CTRLID_SC_PRECOAT_IMAGE;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 210, 30 + 180, 210 + 130);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRECOAT_IMAGE, RESOURCE_BMP);
	}

	// edit-box: Precoat for printer 2
	{
		long ctlId = CTRLID_EB_PRECOAT_PRINTER_2;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 245, 250 + DEF_W_EDITBOX, 245 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// edit-box: Precoat for printer 1
	{
		long ctlId = CTRLID_EB_PRECOAT_PRINTER_1;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 393, 245, 393 + DEF_W_EDITBOX, 245 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// static-box: Unit of precoat
	{
		long ctlId = CTRLID_SC_UNIT_PRECOAT;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 530, 255, 530 + DEF_ICON_SIZE, 255 + DEF_ICON_SIZE);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_ICON_DEG, RESOURCE_BMP);
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
long CCtlPrecoatDryerSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Precoat edit-box for printer 2
			if(ctlWnd == m_ctl[CTRLID_EB_PRECOAT_PRINTER_2]){ // this will never be visited - 2014.9.11
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_PRECOAT_PRINTER_2]);

				// Change a temperature of the precoat for the printer 2
				pData->SetPrecoatTemperature_AddDlg(DEF_PRINTER_2, strTemperature);
			}
			// Precoat edit-box for printer 1
			else if(ctlWnd == m_ctl[CTRLID_EB_PRECOAT_PRINTER_1]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_PRECOAT_PRINTER_1]);

				// Change a temperature of the heat roller for the printer 1
				pData->SetPrecoatTemperature_AddDlg(DEF_PRINTER_1, strTemperature);
				// And set the same value to dryer2 - 2014.9.11
				pData->SetPrecoatTemperature_AddDlg(DEF_PRINTER_2, strTemperature);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlPrecoatDryerSettings::OnUpdateState()
{
	//CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		DWORD dwState = CST_SHOW;

		for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
			dwState = CST_SHOW;

			// Precoat edit-box for printer 2
			if(CTRLID_EB_PRECOAT_PRINTER_2 == ctlID){ // dryer2 is removed from the spec - 2014.9.11
				dwState = CST_HIDE;	// always hide the control
/*			// The system runs with SES
				if(false == pData->IsDED()){
					// Hide the control
					dwState = CST_HIDE;
				}
*/			}

			SetControlState(m_ctl[ctlID], dwState);
		}
	}
}

/*! \brief control value update event
*/
void CCtlPrecoatDryerSettings::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		// Precoat edit-box for printer 2
		{
			const char* strTemperature = pData->GetPrecoatTemperature_AddDlg(DEF_PRINTER_2);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_PRECOAT_PRINTER_2], (DWORD)strTemperature);
		}

		// Precoat edit-box for printer 1
		{
			const char* strTemperature = pData->GetPrecoatTemperature_AddDlg(DEF_PRINTER_1);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_PRECOAT_PRINTER_1], (DWORD)strTemperature);
		}
	}
}