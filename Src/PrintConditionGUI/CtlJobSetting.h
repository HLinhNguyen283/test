/*! \file CtlJobSetting.h
 *  \brief ジョブ設定ボタンコントロール
 *  \author TGL okada
 *  \date 2014/11/21 okada 新規作成
*/

#if !defined(__CTL_JOB_SETTING_H__)
#define __CTL_JOB_SETTING_H__


#include "BaseCtl.h"

/*!
 *  \brief ジョブ設定ボタンコントロールクラス.
*/
class CCtlJobSetting : public CBaseCtl
{
public:
	CCtlJobSetting();
	virtual ~CCtlJobSetting();

	// イベント
	virtual void OnUpdateState();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

protected:
	// イベント
	virtual void OnSetAttribute();

};


#endif // !defined(__CTL_JOB_SETTING_H__)
