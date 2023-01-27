/*! 
 * @file	PrinterIF.h
 * @brief	printer interface class
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
*/

#pragma once

// ==========================================================================
// Include files

#include <windows.h>
#include "BasePrinterIF.h"
#include "DataIF.h"

// ==========================================================================

/**
 * @brief printer interface class
**/
class CPrinterIF : public CBasePrinterIF
{
public:
	/**
	 * @brief constructor
	 * @note General process
	 * 
	 *		Initialize the member variables
	**/
	CPrinterIF();

	/**
	 * @brief destructor
	 * @note General process
	 * 
	 *		Remove objects which created with dynamic.
	**/
	virtual ~CPrinterIF();
	
	/**
	 * @brief Set interface
	 * @param[in]	inData : class of data interface
	**/
	virtual void SetInterface(void* inData);

	/**
	 *  @brief UI_Initialize.
	 *  @retval TRUE : success
	 *  @retval FALSE : fail
	*/
	virtual BOOL UI_Initialize();

	/**
	 * @brief UI_Finalize
	*/
	virtual void UI_Finalize();

	/**
	 * @brief window message procedure
	 * @param[in] hWnd : window handle
	 * @param[in] message : message id
	 * @param[in] wParam : parameter 1
	 * @param[in] lParam : parameter 2
	 * @retval DEF_NONE : continue showing the screen
	 * @retval DEF_EXIT_PAGE : close the screen
	 * @retval DEF_UPDATE_STATE : update status of all controls
	 * @retval DEF_UPDATE_VALUE : update data of all controls
	**/
	virtual long UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



protected:
	//method

	//event

	//parts

	//member
	CDataIF* m_data;			//!< class of data interface

private:
	//member
	
};

