/*! \file PrinterIF.h
 *  \brief printer interface class
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/

#if !defined(_PRINTER_IF_H_)
#define _PRINTER_IF_H_


// ==========================================================================
// Include files

#include <windows.h>
#include "BasePrinterIF.h"
#include "DataIF.h"

// ==========================================================================
//! 

/*! \brief printer interface class
*/
class CPrinterIF : public CBasePrinterIF
{
public:
	//method
	CPrinterIF();
	virtual ~CPrinterIF();
	
	void SetInterface(void* inData);
	virtual BOOL UI_Initialize();
	virtual void UI_Finalize();
	virtual long UI_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	  
	bool IsPrinterStop();

	/**
		@brief	調整印刷中かどうか取得する
		@retval	true : 調整印刷中の状態
		@retval	false : 調整印刷中ではない状態
	*/
	bool IsDuringAdjustPrinting();
protected:
	//method
	//event

	//parts

	//member
	CDataIF* m_data;			//!< class of data interface

private:
	//member
	
};


#endif // !defined(_PRINTER_IF_H_)
