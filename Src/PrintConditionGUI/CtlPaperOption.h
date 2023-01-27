/*! \file CtlPaperOption.h
 *  \brief Paper option control
 *  \author GCS Tan Ho
 *  \date 2013/05/16 GCS Tan Ho created
*/

#if !defined(_CTL_PAPER_OPTION_H_)
#define _CTL_PAPER_OPTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief Paper option control
*/
class CCtlPaperOption : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlPaperOption();
	virtual ~CCtlPaperOption();

	//イベント
	virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品

	//変数

private:
	//変数
	time_t m_MarkedRepTime;		//!< start time repeater
	bool m_buttonPreprintedMarkDownFlg;	//!< True while L button is downing.

};


#endif // !defined(_CTL_PAPER_OPTION_H_)
