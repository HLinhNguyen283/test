/*! \file CtlGpMain.h
 *  \brief Main group-box
 *  \author GCS Tan Ho
 *  \date 2013/05/10 GCS Tan Ho created
*/

#if !defined(_CTL_GP_MAIN_H_)
#define _CTL_GP_MAIN_H_


// ==========================================================================
// Include files
#include "BaseCtl.h"

// ==========================================================================
// Global definitions

//! コントロールID
enum
{
	CTRLID_GB_MAIN = 0,
	//
	CTRLID_GB_MAIN_COUNT
};

// ==========================================================================

/*!
 *  \brief Main group-box
*/
class CCtlGpMain : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlGpMain();
	virtual ~CCtlGpMain();

	//イベント

protected:
	//関数
	virtual void OnSetAttribute();
	virtual void OnCreateItem();

	//イベント
	
	//部品

	//変数

private:
	//変数

	//部品
	
};


#endif // !defined(_CTL_GP_MAIN_H_)
