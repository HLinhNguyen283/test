/*! \file CtlOutputColorSelection.h
 *  \brief CtlOutputColorSelection.h�̏���
 *  \author SCREEN
 *  \date 2017/11/20 SCREEN �V�K�쐬
*/

#if !defined(_CTL_OUTPUTCOLOR_SELECTION_H_)
#define _CTL_OUTPUTCOLOR_SELECTION_H_

// ==========================================================================
// Local definitions
#include "BaseCtl.h"

// ==========================================================================
/*!
 *  \brief The control selects a tension of the selection print settings on the screen.
*/
class CCtlOutputColorSelection : public CBaseCtl
{
public:
	//�萔

	//�֐�
	CCtlOutputColorSelection();
	virtual ~CCtlOutputColorSelection();

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
	virtual void OnCreateItem();
	//virtual void OnDeleteItem();

	//���i
	void RegisterOutputColorItems();
	//�ϐ�

private:
	//�ϐ�

	//�֐�
	
};

#endif // !defined(_CTL_OUTPUTCOLOR_SELECTION_H_)
