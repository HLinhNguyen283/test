/*! \file CtlNirDryerSettings.cpp
 *  \brief The control edits a nir setting for printer 1, printer 2.
 *  \author SCREEN
 *  \date 2018/01/25 SCREEN created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlNirDryerSettings.h"
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
	CTRLID_EB_NIR_PRINTER_2,			//!< edit-box: NIR for printer 2
	CTRLID_EB_NIR_PRINTER_1,			//!< edit-box: NIR for printer 1
	CTRLID_SC_UNIT_NIR,					//!< static-box: Unit of NIR

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlNirDryerSettings::CCtlNirDryerSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlNirDryerSettings::~CCtlNirDryerSettings()
{
}


/*! \brief [event]set the control's property
*/
void CCtlNirDryerSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	CDataIF* dataIF = dynamic_cast<CDataIF*>(m_data);
	bool isDED = dataIF->IsDED();
	bool isHeater1 = dataIF->IsExistHeatRoller(DEF_PRINTER_1);
	bool isHeater2 = dataIF->IsExistHeatRoller(DEF_PRINTER_2);

	// edit-box: NIR for printer 2
	{
		long ctlId = CTRLID_EB_NIR_PRINTER_2;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		// In case of heaterless printer, change control position
		if ((isDED == FALSE && isHeater1 == false) || (isDED == TRUE && isHeater1 == false && isHeater2 == false)) {
			SetRect(&m_ctlAttribute[ctlId].rect, 250, 70, 250 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		} else {
			SetRect(&m_ctlAttribute[ctlId].rect, 250, 245, 250 + DEF_W_EDITBOX, 245 + DEF_H_EDITBOX);
		}
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// edit-box: NIR for printer 1
	{
		long ctlId = CTRLID_EB_NIR_PRINTER_1;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		// In case of heaterless printer, change control position
		if ((isDED == FALSE && isHeater1 == false) || (isDED == TRUE && isHeater1 == false && isHeater2 == false)) {
			SetRect(&m_ctlAttribute[ctlId].rect, 393, 70, 393 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		} else {
			SetRect(&m_ctlAttribute[ctlId].rect, 393, 245, 393 + DEF_W_EDITBOX, 245 + DEF_H_EDITBOX);
		}
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// static-box: Unit of NIR
	{
		long ctlId = CTRLID_SC_UNIT_NIR;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_TEXT | SCST_CENTER;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_UNIT_KW, RESOURCE_STR);;
		// In case of heaterless printer, change control position
		if ((isDED == FALSE && isHeater1 == false) || (isDED == TRUE && isHeater1 == false && isHeater2 == false)) {
			SetRect(&m_ctlAttribute[ctlId].rect, 530, 80, 530 + DEF_ICON_SIZE, 80 + DEF_ICON_SIZE);
		} else {
			SetRect(&m_ctlAttribute[ctlId].rect, 530, 255, 530 + DEF_ICON_SIZE, 255 + DEF_ICON_SIZE);
		}
		m_ctlAttribute[ctlId].param = NULL;
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
long CCtlNirDryerSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// NIR edit-box for printer 2
			if(ctlWnd == m_ctl[CTRLID_EB_NIR_PRINTER_2]){ // this will never be visited - 2014.9.11
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_NIR_PRINTER_2]);

				// Change a value of the nir for the printer 2
				pData->SetNirPower_AddDlg(DEF_PRINTER_2, strTemperature);
			}
			// NIR edit-box for printer 1
			else if(ctlWnd == m_ctl[CTRLID_EB_NIR_PRINTER_1]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_NIR_PRINTER_1]);

				// Change a temperature of the heat roller for the printer 1
				pData->SetNirPower_AddDlg(DEF_PRINTER_1, strTemperature);
			}
		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlNirDryerSettings::OnUpdateState()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		DWORD dwState = CST_SHOW;

		for(long ctlID = 0; ctlID < CTLID_COUNT; ++ctlID){
			dwState = CST_SHOW;

			// NIR edit-box for printer 2
			if(CTRLID_EB_NIR_PRINTER_2 == ctlID){
			// The system runs with SES
				if(false == pData->IsDED()){
					// Hide the control
					dwState = CST_HIDE;
				}
			}

			SetControlState(m_ctl[ctlID], dwState);
		}
	}
}

/*! \brief control value update event
*/
void CCtlNirDryerSettings::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		// NIR edit-box for printer 2
		{
			const char* strTemperature = pData->GetNirPower_AddDlg(DEF_PRINTER_2);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_NIR_PRINTER_2], (DWORD)strTemperature);
		}

		// NIR edit-box for printer 1
		{
			const char* strTemperature = pData->GetNirPower_AddDlg(DEF_PRINTER_1);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_NIR_PRINTER_1], (DWORD)strTemperature);
		}
	}
}