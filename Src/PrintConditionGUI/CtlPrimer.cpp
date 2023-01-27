/**
 * @file	CtlPrimer.cpp
 * @brief	�v���C�}�[�̃R���g���[��
**/

// ==========================================================================
// Include files

#include "stdafx.h"
#include "CtlPrimer.h"
#include "PluginCBRProc.h"
#include "ResDef.h"
#include "DataIF.h"

// ==========================================================================
// Local definitions

//! �E�B���h�EID�ꗗ
enum
{
	CTRLID_BN_PRIMER = 0,	//!< �v���C�}�[�{�^��
	CTLID_COUNT
};

// Control operation log name define
static const char* st_logmsg_btn_primer = "CTRLID_BN_PRIMER";
// ==========================================================================

// �R���X�g���N�^
CCtlPrimer::CCtlPrimer()
{
	//���̃N���X�ō쐬����R���g���[�������Z�b�g
	m_ctlCount = CTLID_COUNT;
}

// �f�X�g���N�^
CCtlPrimer::~CCtlPrimer()
{
}

// �R���g���[�������ݒ�C�x���g
void CCtlPrimer::OnSetAttribute()
{
	//�쐬����R���g���[���̑�����m_ctlAttribute[0]�`[m_ctlCount-1]�ɓo�^����

	// Button: �v���C�}�[�{�^��
	{
		long ctlId = CTRLID_BN_PRIMER;
		m_ctlAttribute[ctlId].type = CT_BUTTON;
		m_ctlAttribute[ctlId].style = CST_HIDE | BNST_BITMAP | BNST_IMAGE_BLEND | BNST_STICKY | BNST_LED_MANUAL | BNST_TEXT | BNST_GRADATION;
		m_ctlAttribute[ctlId].text = LoadResourceString(IDS_PRIMER, RESOURCE_STR);
		SetRect(&m_ctlAttribute[ctlId].rect, 1045, 150, 1045 + 70, 150 + 70);
		m_ctlAttribute[ctlId].param = reinterpret_cast<DWORD>(LoadResourceBitmap(IDB_PRIMER, RESOURCE_BMP));
		m_ctlAttribute[ctlId].logmsg = st_logmsg_btn_primer;
	}
}

// �R���g���[������C�x���g
long CCtlPrimer::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND ctlWnd = reinterpret_cast<HWND>(lParam);
	CDataIF* data = dynamic_cast<CDataIF*>(m_data);

	switch(wParam)
	{
	case BN_CLICKED:
		{
			if (ctlWnd == m_ctl[CTRLID_BN_PRIMER])
			{
				long isON = GetControlData(m_ctl[CTRLID_BN_PRIMER]);
				// �f�[�^���Ƀ{�^��������Ԃ�ʒm����
				data->onPush_Primer(isON == 0 ? false : true);
			}
		}
		break;
	}
	return DEF_NONE;
}

// �R���g���[���\���l�X�V�C�x���g
void CCtlPrimer::OnUpdateValue()
{
	if(m_ctl)
	{
		CDataIF* data = dynamic_cast<CDataIF*>(m_data);
		bool isShow = false;
		bool isEnabled = false;

		// �f�[�^������\����ԂƗL����Ԃ��擾����
		data->onView_Primer(isShow, isEnabled);

		// �\����Ԃ̐ݒ�
		{
			if(isShow)
			{
				SetControlState(m_ctl[CTRLID_BN_PRIMER], CST_SHOW);
			}
		}

		// �L����Ԃ̐ݒ�
		{
			DWORD dwState = isEnabled ? CST_ENABLED : CST_DISABLED;
			SetControlState(m_ctl[CTRLID_BN_PRIMER], dwState);
		}
	}
}

// �R���g���[���X�e�[�^�X�C�x���g
void CCtlPrimer::OnUpdateState()
{
	CDataIF* data = dynamic_cast<CDataIF*>(m_data);

	bool isOn = false;
	// �f�[�^������{�^����Ԃ��擾����
	data->onDraw_Primer(isOn);

	BNITEMINFO bn_item = {0};
	bn_item.bLED_On = isOn;
	SetControlItem(m_ctl[CTRLID_BN_PRIMER], 0, &bn_item);
	SetControlData(m_ctl[CTRLID_BN_PRIMER], static_cast<DWORD>(isOn));
}
