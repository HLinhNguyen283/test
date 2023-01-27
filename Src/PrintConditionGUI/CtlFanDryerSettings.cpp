/*! \file CtlFanDryerSettings.cpp
 *  \brief Fan dryer settings control for Add dryer settings dialog
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/
// ==========================================================================
// Include files
#include "stdafx.h"
#include "CtlFanDryerSettings.h"
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
	CTRLID_SC_DRYER_IMAGE = 0,				//!< static-box: Dryer image
	CTRLID_SC_ICON_PRINTER_2,				//!< static-box: Printer 2 icon
	CTRLID_SC_ICON_PRINTER_1,				//!< static-box: Printer 1 icon
	CTRLID_EB_HEAT_ROLLER_PRINTER_2,		//!< edit-box: Heat roller for printer 2
	CTRLID_EB_HEAT_ROLLER_PRINTER_1,		//!< edit-box: Heat roller for printer 1
	CTRLID_SC_UNIT_HEAT_ROLLER,				//!< static-box: Unit of heat roller
	CTRLID_EB_DRYER_FUN_PRINTER_2,			//!< edit-box: Dryer fun for printter 2
	CTRLID_EB_DRYER_FUN_PRINTER_1,			//!< edit-box: Dryer fun for printer 1
	CTRLID_SC_UNIT_DRYER_FUN,				//!< static-box: Unit of dryer fun
	CTLID_EB_SUB_HEAT_ROLLER_PRINT_2,		//!< edit-box: SubHeatRoller for printer 2
	CTLID_EB_SUB_HEAT_ROLLER_PRINT_1,		//!< edit-box: SubHeatRoller for printer 1
	CTRLID_SC_UNIT_SUB_HEAT_ROLLER,			//!< static-box: Unit of Sub Heat Roller

	CTLID_COUNT
};

// ==========================================================================

/*! \brief constructor
*/
CCtlFanDryerSettings::CCtlFanDryerSettings()
{
	//set to create the number of control.
	m_ctlCount = CTLID_COUNT;

}

/*! \brief destructor
*/
CCtlFanDryerSettings::~CCtlFanDryerSettings()
{
}


/*! \brief [event]set the control's property
*/
void CCtlFanDryerSettings::OnSetAttribute()
{
	//Set the control's property to m_ctlAttribute[0 to m_ctlCount-1]

	// static-box: Dryer image
	{
		long ctlId = CTRLID_SC_DRYER_IMAGE;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 30, 70, 30 + 210, 70 + 140);
		m_ctlAttribute[ctlId].param = NULL;
	}

	// static-box: Printer 2 icon
	{
		long ctlId = CTRLID_SC_ICON_PRINTER_2;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 293, 23, 293 + DEF_ICON_SIZE, 23 + DEF_ICON_SIZE);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINTER_2_ICON, RESOURCE_BMP);
	}

	// static-box: Printer 1 icon
	{
		long ctlId = CTRLID_SC_ICON_PRINTER_1;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 435, 23, 435 + DEF_ICON_SIZE, 23 + DEF_ICON_SIZE);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_PRINTER_1_ICON, RESOURCE_BMP);
	}

	// edit-box: Heat roller for printer 2
	{
		long ctlId = CTRLID_EB_HEAT_ROLLER_PRINTER_2;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 145, 250 + DEF_W_EDITBOX, 145 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// edit-box: Heat roller for printer 1
	{
		long ctlId = CTRLID_EB_HEAT_ROLLER_PRINTER_1;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 393, 145, 393 + DEF_W_EDITBOX, 145 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// static-box: Unit of heat roller
	{
		long ctlId = CTRLID_SC_UNIT_HEAT_ROLLER;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 530, 155, 530 + DEF_ICON_SIZE, 155 + DEF_ICON_SIZE);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_ICON_DEG, RESOURCE_BMP);
	}

	// edit-box: heat fan for printer 2
	{
		long ctlId = CTRLID_EB_DRYER_FUN_PRINTER_2;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 70, 250 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// edit-box: heat fan for printer 1
	{
		long ctlId = CTRLID_EB_DRYER_FUN_PRINTER_1;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_EXIT, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 393, 70, 393 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}

	// static-box: Unit of heat fan
	{
		long ctlId = CTRLID_SC_UNIT_DRYER_FUN;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 530, 80, 530 + DEF_ICON_SIZE, 80 + DEF_ICON_SIZE);
		m_ctlAttribute[ctlId].param = (DWORD)LoadResourceBitmap(IDB_ICON_DEG, RESOURCE_BMP);
	}

	// edit-box: SubHeatRoller for printer 2
	{
		long ctlId = CTLID_EB_SUB_HEAT_ROLLER_PRINT_2;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 250, 70, 250 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}
	// edit-box: SubHeatRoller for printer 1
	{
		long ctlId = CTLID_EB_SUB_HEAT_ROLLER_PRINT_1;
		m_ctlAttribute[ctlId].type = CT_EDITBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | EBST_NUMERIC | EBST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 393, 70, 393 + DEF_W_EDITBOX, 70 + DEF_H_EDITBOX);
		m_ctlAttribute[ctlId].param = DEF_MAX_LENGTH;
	}
	// static-box: static-box: Unit of Sub Heat Roller
	{
		long ctlId = CTRLID_SC_UNIT_SUB_HEAT_ROLLER;
		m_ctlAttribute[ctlId].type = CT_STATICBOX;
		m_ctlAttribute[ctlId].style = CST_HIDE | SCST_BITMAP | SCST_IMAGE_BLEND | SCST_NORMAL;
		m_ctlAttribute[ctlId].text = NULL;
		SetRect(&m_ctlAttribute[ctlId].rect, 530, 80, 530 + DEF_ICON_SIZE, 80 + DEF_ICON_SIZE);
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
long CCtlFanDryerSettings::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//[event]edit-box inputed : wParam == UWM_EDIT_KEYBOARD_CLOSED
	//[event]pulldown-menu selected : wParam == CBN_SELCHANGE
	//[event]listbox selected : wParam == LBN_SELCHANGE
	HWND ctlWnd = (HWND)lParam;
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);
	
	switch(wParam){
	case UWM_EDIT_KEYBOARD_CLOSED:
		{
			// Heat roller edit-box for printer 2
			if(ctlWnd == m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_2]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_2]);

				// Change a temperature of the heat roller for the printer 2
				pData->SetHeatRollerTemperature_AddDlg(DEF_PRINTER_2, strTemperature);
			}
			// Heat roller edit-box for printer 1
			else if(ctlWnd == m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_1]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_1]);

				// Change a temperature of the heat roller for the printer 1
				pData->SetHeatRollerTemperature_AddDlg(DEF_PRINTER_1, strTemperature);
			}
			// Dryer fun edit-box for printer 2
			else if(ctlWnd == m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_2]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_2]);

				// Change a temperature of the dryer fun for the printer 2
				pData->SetDryerFunTemperature_AddDlg(DEF_PRINTER_2, strTemperature);
			}
			// Dryer fun edit-box for printer 1
			else if(ctlWnd == m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_1]){
				char* strTemperature = (char*)GetControlData(m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_1]);

				// Change a temperature of the dryer fun for the printer 1
				pData->SetDryerFunTemperature_AddDlg(DEF_PRINTER_1, strTemperature);
			}
			// Sub Heat Roller edit-box for printer 1
			else if(ctlWnd == m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_1])
			{
				char* strTemperature = (char*)GetControlData(m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_1]);
				// Change a temperature of the SubHeatRoller for the printer 1
				pData->SetDryerTemp_SubHeatRoller(DEF_PRINTER_1, strTemperature);
			
			}
			// Sub Heat Roller edit-box for printer 2
			else if(ctlWnd == m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_2])
			{
				char* strTemperature = (char*)GetControlData(m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_2]);
				// Change a temperature of the SubHeatRoller for the printer 2
				pData->SetDryerTemp_SubHeatRoller(DEF_PRINTER_2, strTemperature);
			}

		}
		break;
	}

	return DEF_NONE;
}

/*! \brief control state update event
*/
void CCtlFanDryerSettings::OnUpdateState()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		DWORD dwState = CST_SHOW;

		// Get system mode
		bool isDED = pData->IsDED();

		// Printer 1/2 has a dryer fun unit or not.
		bool isExistDryerFunPrinter1 = pData->IsExistDryerUnit(DEF_PRINTER_1);
		bool isExistDryerFunPrinter2 = pData->IsExistDryerUnit(DEF_PRINTER_2);
		
		bool isExistHeatRollerPrinter1 = pData->IsExistHeatRoller(DEF_PRINTER_1);
		bool isExistHeatRollerPrinter2 = pData->IsExistHeatRoller(DEF_PRINTER_2);
		
		bool isUnitOfRanger = pData->IsUnitOfRanger();

		bool hideCtl[CTLID_COUNT] = {false};
		hideCtl[CTRLID_SC_DRYER_IMAGE] = (!isExistHeatRollerPrinter1 && !isDED) || (!isExistHeatRollerPrinter1 && !isExistHeatRollerPrinter2 && isDED);
		hideCtl[CTRLID_SC_ICON_PRINTER_2] = !isDED;
		hideCtl[CTRLID_SC_ICON_PRINTER_1] = false;
		hideCtl[CTRLID_EB_HEAT_ROLLER_PRINTER_2] = !isDED || !isExistHeatRollerPrinter2;
		hideCtl[CTRLID_EB_HEAT_ROLLER_PRINTER_1] = !isExistHeatRollerPrinter1;
		hideCtl[CTRLID_SC_UNIT_HEAT_ROLLER] = (!isExistHeatRollerPrinter1 && !isDED) || (!isExistHeatRollerPrinter1 && !isExistHeatRollerPrinter2 && isDED);
		hideCtl[CTRLID_EB_DRYER_FUN_PRINTER_2] = !isDED || !isExistDryerFunPrinter2;
		hideCtl[CTRLID_EB_DRYER_FUN_PRINTER_1] = !isExistDryerFunPrinter1;
		hideCtl[CTRLID_SC_UNIT_DRYER_FUN] = (!isExistDryerFunPrinter1 && !isDED) || (!isExistDryerFunPrinter1 && !isExistDryerFunPrinter2 && isDED);

		hideCtl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_2] = !isDED || !isUnitOfRanger || !isExistHeatRollerPrinter2;
		hideCtl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_1] = !isUnitOfRanger || !isExistHeatRollerPrinter1;
		hideCtl[CTRLID_SC_UNIT_SUB_HEAT_ROLLER] = !isUnitOfRanger || (!isExistHeatRollerPrinter1 && !isDED) || (!isExistHeatRollerPrinter1 && !isExistHeatRollerPrinter2 && isDED);

		for(long ctlId = 0; ctlId < CTLID_COUNT; ++ctlId){
			dwState = hideCtl[ctlId] ? CST_HIDE : CST_SHOW;
			SetControlState(m_ctl[ctlId], dwState);
		}
	}
}

/*! \brief control value update event
*/
void CCtlFanDryerSettings::OnUpdateValue()
{
	CDataIF* pData = dynamic_cast<CDataIF*>(m_data);

	if(m_ctl){
		// Dryer image
		{
			long bitmapID = IDB_DRYER_IMAGE_FAN_ON;
			bool isDED = pData->IsDED();
			bool isExistDryerFunPrinter1 = pData->IsExistDryerUnit(DEF_PRINTER_1);
			bool isExistDryerFunPrinter2 = pData->IsExistDryerUnit(DEF_PRINTER_2);
			bool isUnitOfRanger = pData->IsUnitOfRanger();

			if((!isExistDryerFunPrinter1 && !isDED && !isUnitOfRanger) || (!isExistDryerFunPrinter1 && !isExistDryerFunPrinter2 && isDED && !isUnitOfRanger) ){
				bitmapID = IDB_DRYER_IMAGE_FAN_OFF;
			}

			if((!isDED && !isExistDryerFunPrinter1 && isUnitOfRanger) || (isDED && !isExistDryerFunPrinter1 && !isExistDryerFunPrinter2 && isUnitOfRanger))
			{
				bitmapID = IDB_SUB_HEAT_ROLLER; 
			}

			SetControlData(m_ctl[CTRLID_SC_DRYER_IMAGE], (DWORD)LoadResourceBitmap(bitmapID, RESOURCE_BMP));
		}

		// Heat roller edit-box for printer 2
		{
			const char* strTemperature = pData->GetHeatRollerTemperature_AddDlg(DEF_PRINTER_2);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_2], (DWORD)strTemperature);
		}

		// Heat roller edit-box for printer 1
		{
			const char* strTemperature = pData->GetHeatRollerTemperature_AddDlg(DEF_PRINTER_1);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_HEAT_ROLLER_PRINTER_1], (DWORD)strTemperature);
		}

		// Dryer fun edit-box for printer 2
		{
			const char* strTemperature = pData->GetDryerFunTemperature_AddDlg(DEF_PRINTER_2);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_2], (DWORD)strTemperature);
		}

		// Dryer fun edit-box for printer 1
		{
			const char* strTemperature = pData->GetDryerFunTemperature_AddDlg(DEF_PRINTER_1);
			// Display the temperature
			SetControlData(m_ctl[CTRLID_EB_DRYER_FUN_PRINTER_1], (DWORD)strTemperature);
		}

		// Sub Heat roller edit-box for printer 1
		{
			const char* strTemperature = pData->GetDryerTemp_SubHeatRoller(DEF_PRINTER_1);
			SetControlData(m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_1], (DWORD)strTemperature);
		
		}
		// Sub Heat roller edit-box for printer 2
		{
			const char* strTemperature = pData->GetDryerTemp_SubHeatRoller(DEF_PRINTER_2);
			SetControlData(m_ctl[CTLID_EB_SUB_HEAT_ROLLER_PRINT_2], (DWORD)strTemperature);
		}


	}
}