/**
	* @file		UWandRW_Receiver.cpp
	* @brief	SignalStatus��M�������C�� �����t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <vector>
#include "Common.h"
#include "GetTime.h"
#include "WriteDebugLog.h"
#include "Ini_UwRwManager.h"
#include "SendThread.h"
#include "ReceiveThread.h"
#include "DeleteLogFolder.h"
#include "UWandRW_Receiver.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
	* @brief  UWandRW_Receiver�N������
	* @retval 0(�Œ�)
**/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC �����������āA�G���[�̏ꍇ�͌��ʂ�������܂��B
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("�v���I�ȃG���[: MFC �̏��������ł��܂���ł����B\n"));
			nRetCode = 1;
		}
		else
		{
			// �N��������
			HANDLE hMutex = ::CreateMutex( NULL, TRUE, "UWandRW_Receiver" );
			if( GetLastError() == ERROR_ALREADY_EXISTS)
			{
				// ����PC����̓�d�N���i�c�[���͂��łɋN������Ă��܂��B�j
				// ::MessageBox( NULL, "UWandRW_Receiver is already running.", "UWandRW_Receiver", MB_OK | MB_ICONHAND );
				::ReleaseMutex( hMutex );
				CloseHandle( hMutex );
				return FALSE;
			}

			// �又��
			CMainFunction sMainFunction;
			sMainFunction.Doit();

			// �I��������
			::ReleaseMutex( hMutex );
			CloseHandle( hMutex );
		}
	}
	else
	{
		_tprintf(_T("�v���I�ȃG���[: GetModuleHandle �����s���܂���\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

/**
	* @brief  CMainFunction �R���X�g���N�^
**/
CMainFunction::CMainFunction()
	: m_pSendThread()
	, m_pReceiveThread()
{
}

/**
	* @brief  CMainFunction �f�X�g���N�^
**/
CMainFunction::~CMainFunction()
{
	for (int index = 0; index < DEF_MAX_NUMBER_DEVICE; index++) {
		if (m_pSendThread[index])
		{
			delete m_pSendThread[index];
		}
		if (m_pReceiveThread[index])
		{
			delete m_pReceiveThread[index];
		}
	}
}

/**
	* @brief  UWandRW_Receiver�̎又��
**/
void CMainFunction::Doit()
{
	// ���O�̏�����
	CString LogFileName = "UWandRW_Receiver_" + CGetTime::GetStrDate() + ".log";
	CWriteDebugLog sWriteDebugLog;
	sWriteDebugLog.Initialize(LogFileName);
	sWriteDebugLog.WriteDebuglog("Start UWandRW_Receiver.exe", CWriteDebugLog::E_LOG_LEVEL_1);
	sWriteDebugLog.WriteDebuglog("Start CMainFunction::Doit", CWriteDebugLog::E_LOG_LEVEL_1);

	try
	{
		// �v���O�C����SignalStatus��ʒm����X���b�h���N��
		m_pSendThread[0] = new CSendThread("UW");
		m_nSendThreadID[0] = m_pSendThread[0]->Start(&sWriteDebugLog);

		// UW����SignalStatus����M����X���b�h���N��
		m_pReceiveThread[0] = new CReceiveThread("UW");
		m_nReceiveThreadID[0] = m_pReceiveThread[0]->Start(m_nSendThreadID[0],&sWriteDebugLog);

		// �v���O�C����SignalStatus��ʒm����X���b�h���N��
		m_pSendThread[1] = new CSendThread("RW");
		m_nSendThreadID[1] = m_pSendThread[1]->Start(&sWriteDebugLog);

		// UW����SignalStatus����M����X���b�h���N��
		m_pReceiveThread[1] = new CReceiveThread("RW");
		m_nReceiveThreadID[1] = m_pReceiveThread[1]->Start(m_nSendThreadID[1], &sWriteDebugLog);

		// �Â����O���폜
		DeleteLogFolder(&sWriteDebugLog, "UW");
		DeleteLogFolder(&sWriteDebugLog, "RW");

		// �X���b�h�̏I����҂�(�v���I�ȃG���[���������Ȃ�����I�����Ȃ�)
		HANDLE waitEvent[4] = {m_pSendThread[0]->GetThreadHandle(), m_pReceiveThread[0]->GetThreadHandle(), m_pSendThread[1]->GetThreadHandle(), m_pReceiveThread[1]->GetThreadHandle() };
		DWORD result = WaitForMultipleObjects(4, waitEvent, FALSE, INFINITE);

		sWriteDebugLog.WriteDebuglog("End.. CMainFunction::Doit **********\n", CWriteDebugLog::E_LOG_LEVEL_1);
	}
	catch(...)
	{
		sWriteDebugLog.WriteDebuglog("ERROR Exeption CMainFunction::Doit *********\n", CWriteDebugLog::E_LOG_LEVEL_1);
	}
}

/**
	* @brief  �Â����O���폜����
**/
void CMainFunction::DeleteLogFolder(CWriteDebugLog* inWriteDebugLog, const char* inDeviceName)
{
	CString LogTopFoler = CCommon::GetModuleDirectory() + "\\Log\\" + inDeviceName + "_CONNECT\\";

	CIni_UwRwManager iniUnwinderManager;
	iniUnwinderManager.Initialize(TRUE);

	CDeleteLogFolder sDeleteLogFolder(inWriteDebugLog);
	sDeleteLogFolder.Doit( LogTopFoler, iniUnwinderManager.getPeriod_day() );
}
