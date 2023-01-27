/**
	* @file		SendThread.cpp
	* @brief	SignalStatus���v���O�C���֒ʒm�X���b�h �����t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi �V�K�쐬
**/

// ===========================================================================
// Include files
#include "stdafx.h"
#include <sstream>
#include "Common.h"
#include "WriteDebugLog.h"
#include "SendThread.h"
// ===========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	* @brief  CSendThread �R���X�g���N�V����
**/
CSendThread::CSendThread(const char* inDeviceName):
	m_DeviceName(inDeviceName)
{
	m_hInitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);		// �����������C�x���g
	m_hPipe = INVALID_HANDLE_VALUE;
}

/**
	* @brief  CSendThread �f�X�g���N�V����
**/
CSendThread::~CSendThread()
{
}

/**
	* @brief  �X���b�h���N������
	* @param  InWriteDebugLog : [i]���O�N���X�̃|�C���^
	* @retval �X���b�h�h�c
**/
UINT CSendThread::Start( CWriteDebugLog* InWriteDebugLog )
{
	// ����ۑ�
	m_pWriteLog = InWriteDebugLog;
	m_pWriteLog->WriteDebuglog("Start CSendThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	// �X���b�h�N��
	UINT threadID;
	m_hThread = (HANDLE)_beginthreadex( 0, 0, ThreadFunction, this, 0, &threadID );

	// �X���b�h�̏������҂��i�X���b�h�Ƀ��b�Z�[�W�L���[���o����܂ő҂j
	::WaitForSingleObject( m_hInitEvent, INFINITE );

	m_pWriteLog->WriteDebuglog("End.. CSendThread::Start", CWriteDebugLog::E_LOG_LEVEL_1);

	return threadID;
}

/**
	* @brief  �X���b�h���C������
	* @retval 0(�Œ�)
**/
UINT CSendThread::MainLoop()
{
	m_pWriteLog->WriteDebuglog("Start CSendThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	try
	{
		MSG	message;
		// ���b�Z�[�W�L���[�̍쐬
		::PeekMessage( &message, NULL, 0, 0, PM_NOREMOVE );
		// �X���b�h����������(���b�Z�[�W�L���[��������)
		::SetEvent( m_hInitEvent );

		bool loop = true;
		while ( loop )
		{
			// ���b�Z�[�W�擾
			BOOL bRet = ::GetMessage( &message, NULL, 0, 0 );
			if ( bRet == 0 || bRet == -1 )
			{
				break;
			}

			// ���b�Z�[�W����
			switch( message.message ) {
			case CCommon::WM_USER_SIGNAL_STATUS:	
				{
					CString* p = (CString*)message.wParam;
					CString s;
					s.Append( p->GetString() );
					delete p;

					// �v���O�C����SignalStatus��ʒm
					WriteSignalStatus( s );
				}
				break;
			}
		}
		m_hThread = NULL;
	}
	catch(...)
	{
		m_pWriteLog->WriteDebuglog("ERROR CSendThread::MainLoop Exception", CWriteDebugLog::E_LOG_LEVEL_1);
	}

	m_pWriteLog->WriteDebuglog("End.. CSendThread::MainLoop", CWriteDebugLog::E_LOG_LEVEL_1);
	return 0;
}

/**
	* @brief  ��M�X���b�h
	* @param  pData : [i]CSendThread�̃|�C���^
	* @retval 0(�Œ�)
**/
UINT __stdcall CSendThread::ThreadFunction( void* pData )
{
	return ((CSendThread*)pData)->MainLoop();
}

/**
	* @brief  SignalStatus���v���O�C���ɒʒm����
**/
void CSendThread::WriteSignalStatus(const CString& InData)
{
	m_pWriteLog->WriteDebuglog("Start CSendThread::WriteSignalStatus", CWriteDebugLog::E_LOG_LEVEL_1);

	int nMax = 3;
	int i = 0;
	bool isUW = (strcmp(m_DeviceName, "UW") == 0);
	for( ; i<nMax; i++ )
	{
		if ( m_hPipe == INVALID_HANDLE_VALUE )
		{
			// Pipe�쐬
			m_hPipe = CreateFile(std::string("\\\\.\\pipe\\").append(isUW ? "Unwinder": "Rewinder").c_str(),
								  GENERIC_WRITE,
								  0,
								  NULL,
								  OPEN_EXISTING, 
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);
	
			if (m_hPipe == INVALID_HANDLE_VALUE)
			{
				std::stringstream ss;
				ss << "ERROR CSendThread::WriteSignalStatus CreateFile GetLastError=" << GetLastError();
				m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
				return;
			}
		}

		BOOL nRet = FALSE;

		// �f�[�^����������
		CString SendLength;
		SendLength.Format("%08d",InData.GetLength());
		if ( WriteData( m_hPipe, (TCHAR*)SendLength.GetString(), SendLength.GetLength()) )
		{
			// �f�[�^��������
			if ( WriteData( m_hPipe, (TCHAR*)InData.GetString(), InData.GetLength()) )
			{
				nRet = TRUE;
			}
		}
		if( nRet )
		{
			break;
		}
		else
		{
			std::stringstream ss;
			ss << "ERROR CSendThread::WriteSignalStatus WriteFile count=" << i <<  " GetLastError=" << GetLastError();
			m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
			// Pipe�I��
			CloseHandle(m_hPipe);
			m_hPipe = INVALID_HANDLE_VALUE;
		}
	}
	if ( i < nMax )
	{	// Pipe�������ݐ���
		m_pWriteLog->WriteDebuglog("End.. CSendThread::WriteSignalStatus", CWriteDebugLog::E_LOG_LEVEL_1);
	}
	else
	{	// Pipe�������ُ݈�
		std::stringstream ss;
		ss << "ERROR CSendThread::WriteSignalStatus WriteFile GetLastError=" << GetLastError();
		m_pWriteLog->WriteDebuglog(ss.str().c_str(), CWriteDebugLog::E_LOG_LEVEL_1);
	}
}

/**
	* @brief  Pipe��������
	* @param  InPipe : [i]PIPE�n���h��
	* @param  InData : [i]�������݃f�[�^�̃|�C���^
	* @param  InSize : [i]�������݃f�[�^�̒���
	* @retval TRUE�F����
	* @retval FALSE�F�ُ�
**/
BOOL CSendThread::WriteData( HANDLE InPipe, TCHAR* InData, DWORD InSize )
{
	TCHAR* p = InData;

	while( InSize )
	{
		DWORD WriteSize;
		BOOL nRet = WriteFile(InPipe, p, InSize, &WriteSize, NULL);
		if ( nRet )
		{
			InSize -= WriteSize;
			InData += WriteSize;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}