/*! \file PageMain.h
 *  \brief main page class
 *  \author Sugita
 *  \date 2013/04/25 Sugita new
*/

#if !defined(_PAGE_MAIN_H_)
#define _PAGE_MAIN_H_

// ==========================================================================
// Include files

#include "BasePage.h"

// ==========================================================================

/*! \class CPageMain
 *  \brief main page class
*/
class CPageMain : public CBasePage
{
public:
	//定数

	//関数
	CPageMain();
	virtual ~CPageMain();

	virtual BOOL Create(CBaseDataIF* inData, CBasePrinterIF* inPrinter, HWND inOwnerWnd);

protected:
	//関数
	virtual void OnSetAttribute();

	//イベント

	//部品

	//変数

private:
	//変数
	
};

#endif // !defined(_PAGE_MAIN_H_)
