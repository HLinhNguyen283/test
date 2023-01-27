/*! \file CtlCommitAddDryerSettings.h
 *  \brief The control commit/cancel to add the dryer settings.
 *  \author GCS Tan Ho
 *  \date 2013/05/06 GCS Tan Ho Created
*/

#if !defined(_CTL_COMMIT_ADD_DRYER_SETTINGS_H_)
#define _CTL_COMMIT_ADD_DRYER_SETTINGS_H_


// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

/*!
 *  \brief The control commit/cancel to add the dryer settings.
*/
class CCtlCommitAddDryerSettings : public CBaseCtl
{
public:
	//定数

	//関数
	CCtlCommitAddDryerSettings();
	virtual ~CCtlCommitAddDryerSettings();

	void SetPersonalInfo(POINT inOffset);

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

	//変数

private:
	//変数
	POINT m_ctrlOffset;		//!< position of control

};


#endif // !defined(_CTL_COMMIT_ADD_DRYER_SETTINGS_H_)
