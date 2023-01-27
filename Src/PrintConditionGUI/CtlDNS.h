/*! \file CtlDNS.h
 *  \brief The control edits a dynamic nozzle setting of the selected print settings on the screen.
 *  \author Screen Matsuda
 *  \date 2014/03/20 Screeh Matsuda created
*/

#if !defined(_CTL_DNS_H_)
#define _CTL_DNS_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control edits dynamic nozzle shift setting of the selected print settings on the screen.
*/
class CCtlDNS : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlDNS();
	virtual ~CCtlDNS();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	
	//イベント
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数

	//関数
	
};


#endif // !defined(_CTL_DNS_H_)
