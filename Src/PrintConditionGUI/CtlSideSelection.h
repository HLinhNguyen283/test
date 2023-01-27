/*! \file CtlSideSelection.h
 *  \brief The control selects front side or back side for preprinted mark.
 *  \author GCS Tan Ho
 *  \date 2013/05/20 GCS Tan Ho Created
*/

#if !defined(_CTL_SIDE_SELECTION_H_)
#define _CTL_SIDE_SELECTION_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"
#include "DataIF.h"

// ==========================================================================

/*!
 *  \brief The control selects front side or back side for preprinted mark.
*/
class CCtlSideSelection : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlSideSelection();
	virtual ~CCtlSideSelection();

	//イベント
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual void OnUpdateState();
	//virtual void OnUpdateValue();

protected:
	//関数
	//イベント
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//部品
	bool ConvertCtlIdToPrintSideType(HWND inWnd, DEF_PRINT_SIDE* outPrintSide);
	//変数

private:
	//変数
	
};


#endif // !defined(_CTL_SIDE_SELECTION_H_)
