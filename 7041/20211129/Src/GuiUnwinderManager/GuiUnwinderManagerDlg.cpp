/**
 * @file  GuiUnwinderManagerDlg.cpp
 * @brief �_�C�A���O��� �����t�@�C��
 * @author screen
 * @date  20211119 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "ClientThread.h"
#include "ServerThread.h"
#include "GuiUnwinderManager.h"
#include "GuiUnwinderManagerDlg.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg �R���X�g���N�V����
	* @param  pWriteDebugLog : [i]���O�N���X�|�C���^
	* @param  pParent        : [i]�Ăяo�����N���X�|�C���^
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLDlg::CJD_UW_Test_TOOLDlg( CWnd* pParent /*=NULL*/)
	: CDialogEx(CJD_UW_Test_TOOLDlg::IDD, pParent)
	, m_pClientThread(NULL)
	, m_pServerThread(NULL)
	, m_Send1st(TRUE)
	, m_Recv1st(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg �f�X�g���N�V����
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLDlg::~CJD_UW_Test_TOOLDlg()
{
	if ( m_pClientThread )
	{
		//! �X���b�h�I���v��
		PostThreadMessage( m_nClientThreadID, WM_QUIT, (WPARAM)0, (LPARAM)0 );
		//! �X���b�h�̏I����҂�
		WaitForSingleObject( m_pClientThread->GetThreadHandle(), INFINITE );

		delete m_pClientThread;
		m_pClientThread = NULL;
	}
	if ( m_pServerThread )
	{
		delete m_pServerThread;
		m_pServerThread = NULL;
	}
}

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg �f�[�^����
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editSendMessage);
	DDX_Control(pDX, IDC_EDIT3, m_editRecvMessage);
}

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg ���b�Z�[�W�}�b�v
**/
/* ==================================================================================================== */
BEGIN_MESSAGE_MAP(CJD_UW_Test_TOOLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_JMF_QueryStatus_END, OnJmfQueryStatusEnd)
	ON_MESSAGE(WM_USER_JMF_QueryResource_END, OnJmfQueryResourceEnd)
	ON_MESSAGE(WM_USER_JMF_StopPersChParams_END, OnJmfStopPersChParamsEnd)
	ON_MESSAGE(WM_USER_JMF_CommandResource_END, OnJmfCommandResourceEnd)
	ON_MESSAGE(WM_USER_JMF_CommandWakeup_END, OnJmfCommandWakeupEnd)
	ON_MESSAGE(WM_USER_DSP_SEND, OnDispSend)
	ON_MESSAGE(WM_USER_DSP_RECV, OnDispRecv)
	ON_BN_CLICKED(IDCANCEL, &CJD_UW_Test_TOOLDlg::OnBnEndButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CJD_UW_Test_TOOLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CJD_UW_Test_TOOLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CJD_UW_Test_TOOLDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CJD_UW_Test_TOOLDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CJD_UW_Test_TOOLDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CJD_UW_Test_TOOLDlg::OnBnClickedButton6)
END_MESSAGE_MAP()

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg ������
	* @retval TRUE(�Œ�)
**/
/* ==================================================================================================== */
BOOL CJD_UW_Test_TOOLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	m_pClientThread = new CClientThread();
	m_nClientThreadID = m_pClientThread->Start(this);

	m_pServerThread = new CServerThread();
	m_nServerThreadID = m_pServerThread->Start(this);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B
void CJD_UW_Test_TOOLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CJD_UW_Test_TOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* ==================================================================================================== */
/**
	* @brief �u��ԊĎ��v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton1()
{
	WriteToLogBuf(LOG_DEBUG,"�u��ԊĎ��v�{�^������");
	ButtonControl(FALSE);
	PostThreadMessage( m_nClientThreadID, WM_USER_JMF_QueryStatus, (WPARAM)0, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief �u�p�����o�^�v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton2()
{
	WriteToLogBuf(LOG_DEBUG,"�u�p�����o�^�v�{�^������");
	ButtonControl(FALSE);
	PostThreadMessage( m_nClientThreadID, WM_USER_JMF_QueryResource, (WPARAM)0, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief �u�`���l���폜�v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton3()
{
	WriteToLogBuf(LOG_DEBUG,"�u�`���l���폜�v�{�^������");
	ButtonControl(FALSE);
	PostThreadMessage( m_nClientThreadID, WM_USER_JMF_StopPersChParams, (WPARAM)0, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief �u��������ݒ�v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton4()
{
	WriteToLogBuf(LOG_DEBUG,"�u��������ݒ�v�{�^������");
	ButtonControl(FALSE);
	PostThreadMessage( m_nClientThreadID, WM_USER_JMF_CommandResource, (WPARAM)0, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief �uREADY��Ԃւ̑J�ځv�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton5()
{
	WriteToLogBuf(LOG_DEBUG,"�uREADY��Ԃւ̑J�ځv�{�^������");
	ButtonControl(FALSE);
	PostThreadMessage( m_nClientThreadID, WM_USER_JMF_CommandWakeup, (WPARAM)0, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief �u����M���O�N���A�v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton6()
{
	WriteToLogBuf(LOG_DEBUG,"�u����M���O�N���A�v�{�^������");
	m_DispSendMessage.Empty();
	m_DispRecvMessage.Empty();
	m_editSendMessage.SetWindowText(m_DispSendMessage);
	m_editRecvMessage.SetWindowText(m_DispRecvMessage);
	m_Send1st = TRUE;
	m_Recv1st = TRUE;
}

/* ==================================================================================================== */
/**
	* @brief �u�I���v�{�^������
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnEndButton()
{
	CDialogEx::OnCancel();
}

/* ==================================================================================================== */
/**
	* @brief �uClient�����I���v�C�x���g��M
	* @param  wParam : [i]���g�p
	* @param  lParam : [i]���g�p
**/
/* ==================================================================================================== */
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfQueryStatusEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// ���M�֘A�{�^����L���ɂ���
	GetDlgItem(IDC_BUTTON1)->SetFocus();
	return 0;
}
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfQueryResourceEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// ���M�֘A�{�^����L���ɂ���
	GetDlgItem(IDC_BUTTON2)->SetFocus();
	return 0;
}
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfStopPersChParamsEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// ���M�֘A�{�^����L���ɂ���
	GetDlgItem(IDC_BUTTON3)->SetFocus();
	return 0;
}
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfCommandResourceEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// ���M�֘A�{�^����L���ɂ���
	GetDlgItem(IDC_BUTTON4)->SetFocus();
	return 0;
}
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfCommandWakeupEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// ���M�֘A�{�^����L���ɂ���
	GetDlgItem(IDC_BUTTON5)->SetFocus();
	return 0;
}

/* ==================================================================================================== */
/**
	* @brief �u���M�f�[�^�\���v�C�x���g��M
	* @param  wParam : [i]���M�f�[�^(CString�̃|�C���^)
	* @param  lParam : [i]���g�p
**/
/* ==================================================================================================== */
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnDispSend(WPARAM wParam, LPARAM lParam)
{
	if ( wParam != NULL )
	{
		CString *p = (CString*)wParam;

		if ( !m_Send1st )
		{
			m_DispSendMessage.Append(_T("\r\n"));
		}
		m_Send1st = FALSE;
		m_DispSendMessage.Append(*p);
		m_editSendMessage.SetWindowText(m_DispSendMessage);
		m_editSendMessage.LineScroll(m_editSendMessage.GetLineCount());

		delete p;
	}
	return 0;
}

/* ==================================================================================================== */
/**
	* @brief �u��M�f�[�^�\���v�C�x���g��M
	* @param  wParam : [i]��M�f�[�^(CString�̃|�C���^)
	* @param  lParam : [i]���g�p
**/
/* ==================================================================================================== */
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnDispRecv(WPARAM wParam, LPARAM lParam)
{
	if ( wParam != NULL )
	{
		CString *p = (CString*)wParam;

		if ( !m_Recv1st )
		{
			m_DispRecvMessage.Append(_T("\r\n"));
		}
		m_Recv1st = FALSE;
		m_DispRecvMessage.Append(*p);
		m_editRecvMessage.SetWindowText(m_DispRecvMessage);
		m_editRecvMessage.LineScroll(m_editRecvMessage.GetLineCount());

		delete p;
	}
	return 0;
}

/* ==================================================================================================== */
/**
	* @brief ���M�֘A�{�^����L��/�����ɂ���(���M���͖���)
	* @param  Enable : [i]�L��(TRUE)/����(FALSE)
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::ButtonControl(BOOL Enable)
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(Enable);
}
