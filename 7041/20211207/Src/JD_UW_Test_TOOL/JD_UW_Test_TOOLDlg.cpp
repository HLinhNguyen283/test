/**
 * @file  JD_UW_Test_TOOLDlg.cpp
 * @brief ダイアログ画面 実装ファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "WriteDebugLog.h"
#include "GetIniFileParam.h"
#include "ClientThread.h"
#include "ServerThread.h"
#include "JD_UW_Test_TOOL.h"
#include "JD_UW_Test_TOOLDlg.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg コンストラクション
	* @param  pWriteDebugLog : [i]ログクラスポインタ
	* @param  pParent        : [i]呼び出し元クラスポインタ
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLDlg::CJD_UW_Test_TOOLDlg( CWriteDebugLog *pWriteDebugLog, CWnd* pParent /*=NULL*/)
	: CDialogEx(CJD_UW_Test_TOOLDlg::IDD, pParent)
	, m_pClientThread(NULL)
	, m_pServerThread(NULL)
	, m_pWriteLog(pWriteDebugLog)
	, m_Send1st(TRUE)
	, m_Recv1st(TRUE)
	, m_nSendFunction(-1)
	, m_nAutoStatus(FALSE)
	, m_nAutoPaper(FALSE) 

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg デストラクション
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLDlg::~CJD_UW_Test_TOOLDlg()
{
	if ( m_pClientThread )
	{
		//! スレッド終了要求
		PostThreadMessage( m_nClientThreadID, WM_QUIT, (WPARAM)0, (LPARAM)0 );
		//! スレッドの終了を待つ
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
	* @brief CJD_UW_Test_TOOLDlg データ交換
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
	* @brief CJD_UW_Test_TOOLDlg メッセージマップ
**/
/* ==================================================================================================== */
BEGIN_MESSAGE_MAP(CJD_UW_Test_TOOLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CCommon::WM_USER_JMF_SEND_END, OnJmfSendEnd)
	ON_MESSAGE(CCommon::WM_USER_DSP_SEND, OnDispSend)
	ON_MESSAGE(CCommon::WM_USER_DSP_RECV, OnDispRecv)
	ON_BN_CLICKED(IDCANCEL, &CJD_UW_Test_TOOLDlg::OnBnEndButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CJD_UW_Test_TOOLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CJD_UW_Test_TOOLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CJD_UW_Test_TOOLDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CJD_UW_Test_TOOLDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CJD_UW_Test_TOOLDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CJD_UW_Test_TOOLDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CJD_UW_Test_TOOLDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CJD_UW_Test_TOOLDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CJD_UW_Test_TOOLDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CJD_UW_Test_TOOLDlg::OnBnClickedButton10)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLDlg 初期化
	* @retval TRUE(固定)
**/
/* ==================================================================================================== */
BOOL CJD_UW_Test_TOOLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	m_pClientThread = new CClientThread();
	m_nClientThreadID = m_pClientThread->Start( this, m_pWriteLog);

	m_pServerThread = new CServerThread();
	m_nServerThreadID = m_pServerThread->Start( this, m_pWriteLog);

	// 対向用の試験モジュール
	m_pWriteLog->WriteDebuglog(_T("試験用対向UW側"), CWriteDebugLog::E_LOG_LEVEL_1);
	GetDlgItem(IDC_STATIC_1)->SetWindowText(_T("試験用対向UW 送信(UW-->TP)"));
	GetDlgItem(IDC_STATIC_2)->SetWindowText(_T("試験用対向UW 受信(TP-->UW)"));

	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。
void CJD_UW_Test_TOOLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CJD_UW_Test_TOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* ==================================================================================================== */
/**
	* @brief 「状態監視」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton1()
{
	m_pWriteLog->WriteDebuglog(_T("状態監視ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_QueryStatus;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「用紙情報登録」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton2()
{
	m_pWriteLog->WriteDebuglog(_T("用紙情報登録ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_QueryResource;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「チャネル削除」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton3()
{
	m_pWriteLog->WriteDebuglog(_T("チャネル削除ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_StopPersChParams;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「印刷条件設定」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton4()
{
	m_pWriteLog->WriteDebuglog(_T("印刷条件設定ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_CommandResource;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「READY状態への遷移」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton5()
{
	m_pWriteLog->WriteDebuglog(_T("READY状態への遷移ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_CommandWakeup;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「送受信ログクリア」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton6()
{
	m_pWriteLog->WriteDebuglog(_T("送受信ログクリアボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	m_DispSendMessage.Empty();
	m_DispRecvMessage.Empty();
	m_editSendMessage.SetWindowText(m_DispSendMessage);
	m_editRecvMessage.SetWindowText(m_DispRecvMessage);
	m_Send1st = TRUE;
	m_Recv1st = TRUE;
}

/* ==================================================================================================== */
/**
	* @brief 「用紙情報通知」ボタン押下（対向試験用）
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnClickedButton7()
{
	m_pWriteLog->WriteDebuglog(_T("用紙情報通知ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_SignalStatusPaper;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}

/* ==================================================================================================== */
/**
	* @brief 「終了」ボタン押下
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::OnBnEndButton()
{
	m_pWriteLog->WriteDebuglog(_T("終了ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	CDialogEx::OnCancel();
}

/* ==================================================================================================== */
/**
	* @brief 「Client処理終了」イベント受信
	* @param  wParam : [i]未使用
	* @param  lParam : [i]未使用
**/
/* ==================================================================================================== */
afx_msg LRESULT CJD_UW_Test_TOOLDlg::OnJmfSendEnd(WPARAM wParam, LPARAM lParam)
{
	ButtonControl(TRUE);		// 送信関連ボタンを有効にする

	// 送信ボタンにフォーカスを戻す
	switch(m_nSendFunction) {
	case CCommon::E_JMF_QueryStatus:
		GetDlgItem(IDC_BUTTON1)->SetFocus();
		break;
	case CCommon::E_JMF_QueryResource:
		GetDlgItem(IDC_BUTTON2)->SetFocus();
		break;
	case CCommon::E_JMF_StopPersChParams:
		GetDlgItem(IDC_BUTTON3)->SetFocus();
		break;
	case CCommon::E_JMF_CommandResource:
		GetDlgItem(IDC_BUTTON4)->SetFocus();
		break;
	case CCommon::E_JMF_CommandWakeup:
		GetDlgItem(IDC_BUTTON5)->SetFocus();
		break;
	case CCommon::E_JMF_SignalStatusPaper:
		GetDlgItem(IDC_BUTTON7)->SetFocus();
		break;
	case CCommon::E_JMF_SignalStatus:
		GetDlgItem(IDC_BUTTON8)->SetFocus();
		break;
	}
	return 0;
}

/* ==================================================================================================== */
/**
	* @brief 「送信データ表示」イベント受信
	* @param  wParam : [i]送信データ(CStringのポインタ)
	* @param  lParam : [i]未使用
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
	* @brief 「受信データ表示」イベント受信
	* @param  wParam : [i]受信データ(CStringのポインタ)
	* @param  lParam : [i]未使用
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
	* @brief 送信関連ボタンを有効/無効にする(送信中は無効)
	* @param  Enable : [i]有効(TRUE)/無効(FALSE)
**/
/* ==================================================================================================== */
void CJD_UW_Test_TOOLDlg::ButtonControl(BOOL Enable)
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(Enable);
}



void CJD_UW_Test_TOOLDlg::OnBnClickedButton8()
{
	m_pWriteLog->WriteDebuglog(_T("ステータス通知ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
	ButtonControl(FALSE);
	m_nSendFunction = CCommon::E_JMF_SignalStatus;
	PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)m_nSendFunction, (LPARAM)0 );
}


void CJD_UW_Test_TOOLDlg::OnBnClickedButton9()
{
	if ( m_nAutoStatus )
	{
		m_pWriteLog->WriteDebuglog(_T("自動ステータス通知停止ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
		KillTimer(TIMER_ID_STATUS);
		GetDlgItem(IDC_BUTTON9)->SetWindowText("自動ステータス通知開始");
		m_nAutoStatus = FALSE;
	}
	else
	{
		m_pWriteLog->WriteDebuglog(_T("自動ステータス通知開始ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
		SetTimer(TIMER_ID_STATUS,CGetIniFileParam::GetStatusRepeattime() * 1000,NULL);
		GetDlgItem(IDC_BUTTON9)->SetWindowText("自動ステータス通知停止");
		m_nAutoStatus = TRUE;
	}
}


void CJD_UW_Test_TOOLDlg::OnBnClickedButton10()
{
	if ( m_nAutoPaper )
	{
		m_pWriteLog->WriteDebuglog(_T("自動用紙情報通知停止ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
		KillTimer(TIMER_ID_PAPER);
		GetDlgItem(IDC_BUTTON10)->SetWindowText("自動用紙情報通知開始");
		m_nAutoPaper = FALSE;
	}
	else
	{
		m_pWriteLog->WriteDebuglog(_T("自動用紙情報通知開始ボタン押下"), CWriteDebugLog::E_LOG_LEVEL_1);
		SetTimer(TIMER_ID_PAPER,CGetIniFileParam::GetPaperRepeattime() * 1000,NULL);
		GetDlgItem(IDC_BUTTON10)->SetWindowText("自動用紙情報通知停止");
		m_nAutoPaper = TRUE;
	}
}


void CJD_UW_Test_TOOLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	switch(nIDEvent) {
	case TIMER_ID_STATUS:
		PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)CCommon::E_JMF_SignalStatus, (LPARAM)0 );
		break;
	case TIMER_ID_PAPER:
		PostThreadMessage( m_nClientThreadID, CCommon::WM_USER_JMF_SEND, (WPARAM)CCommon::E_JMF_SignalStatusPaper, (LPARAM)0 );
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
