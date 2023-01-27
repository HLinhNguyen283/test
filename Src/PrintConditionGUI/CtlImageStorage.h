/*! \file CtlImageStorage.h
 *  \brief CtlImageStorage.h�̏���
 *  \author omori
 *  \date 2019/02/28 omori �V�K�쐬
*/

#pragma once

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================

class CCtlImageStorage : public CBaseCtl
{
public:

	CCtlImageStorage();

	virtual ~CCtlImageStorage();
	//�C�x���g
	//virtual long Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	//virtual BOOL OnCreate(CCtlBase** inControlList);
	//virtual void OnDestroy();
	virtual long OnCommand(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateState();
	virtual void OnUpdateValue();

protected:
	//�֐�
	//�C�x���g
	virtual void OnSetAttribute();
	//virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//���i

	//�ϐ�

private:
	//�ϐ�

};