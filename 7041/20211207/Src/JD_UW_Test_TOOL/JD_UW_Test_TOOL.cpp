/**
 * @file  JD_UW_Test_TOOL.cpp
 * @brief TP-UW�C���^�[�t�F�[�X�m�F�p�A�v�����C�� �����t�@�C��
 * @details (TP��UW�̃C���^�[�t�F�[�X�m�F�p�A�v��)
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

// ===========================================================================
// Include files
// ===========================================================================
#include "stdafx.h"
#include "Common.h"
#include "GetTime.h"
#include "WriteDebugLog.h"
#include "JD_UW_Test_TOOL.h"
#include "JD_UW_Test_TOOLDlg.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CJD_UW_Test_TOOLApp

BEGIN_MESSAGE_MAP(CJD_UW_Test_TOOLApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLApp �R���X�g���N�V����
	* @retval FALSE(�Œ�)
**/
/* ==================================================================================================== */
CJD_UW_Test_TOOLApp::CJD_UW_Test_TOOLApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

// �B��� CJD_UW_Test_TOOLApp �I�u�W�F�N�g�ł��B
CJD_UW_Test_TOOLApp theApp;

/* ==================================================================================================== */
/**
	* @brief CJD_UW_Test_TOOLApp ����������
	* @retval FALSE(�Œ�)
**/
/* ==================================================================================================== */
BOOL CJD_UW_Test_TOOLApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	CString FileName(_T("JD_UW_Test_TOOL_"));
	FileName.Append(CGetTime::GetStrDate() + _T(".txt"));

	CWriteDebugLog sWriteDebugLog;
	sWriteDebugLog.Initialize(FileName);
	sWriteDebugLog.WriteDebuglog(_T("TP-UW �C���^�[�t�F�[�X�m�F �J�n"), CWriteDebugLog::E_LOG_LEVEL_1);

	CJD_UW_Test_TOOLDlg dlg(&sWriteDebugLog);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}

	sWriteDebugLog.WriteDebuglog(_T("TP-UW �C���^�[�t�F�[�X�m�F �I��"), CWriteDebugLog::E_LOG_LEVEL_1);

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

