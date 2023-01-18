/**
 * @file  Common.h
 * @brief ���ʒ�` �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/

#pragma once


#define TEST_TOOL_INI			_T("JD_UW_Test_TOOL.ini")
#define TEST_WORK_INI			_T("JD_UW_Test_WORK.ini")
#define CONTENT_TYPE_JMF		"Content-type: application/vnd.cip4-jmf+xml"

#define CONV_TIME				_T("\\TIME\\")
#define CONV_DEVICEID			_T("\\DEVICEID\\")
#define CONV_SENDID				_T("\\SendID\\")
// Subscription
#define CONV_REPEATTIME			_T("\\REPEATTIME\\")
#define CONV_URL				_T("\\URL\\")
#define CONV_CHANNELMODE		_T("\\CHANNELMODE\\")
// ResourceQuParams
#define CONV_RESOURCEDETAILS	_T("\\RESOURCEDETAILS\\")
#define CONV_RESOURCENAME		_T("\\RESOURCENAME\\")
#define CONV_SCOPE				_T("\\SCOPE\\")
// StopPersChParams
#define CONV_MESSAGETYPE		_T("\\MESSAGETYPE\\")
#define CONV_CHANNELID			_T("\\CHANNELID\\")
#define CONV_STOP_URL			_T("\\STOP_URL\\")
// ResourceCmdParams
#define CONV_UPDATEMETHOD		_T("\\UPDATEMETHOD\\")
// ResouceSet
#define CONV_NAME				_T("\\NAME\\")
// Resource
#define CONV_DESCRIPTIVENAME	_T("\\DESCRIPTIVENAME\\")
#define CONV_EXTERNALID			_T("\\EXTERNALID\\")
// Media
#define CONV_DIMENSION			_T("\\DIMENSION\\")
#define CONV_MEDIATYPE			_T("\\MEDIATYPE\\")
#define CONV_THICKNESS			_T("\\THICKNESS\\")
#define CONV_UWTENSION			_T("\\UWTENSION\\")
#define CONV_MAXRUNSPEED		_T("\\MAXRUNSPEED\\")

#define CONV_STATUS				_T("\\STATUS\\")
#define CONV_ROLLDIAMETER		_T("\\ROLLDIAMETER\\")

/**
* @brief ����API�N���X
*/
class CCommon
{
public:
	enum
	{	// �X���b�h�ԗv��
		WM_USER_JMF_SEND = (WM_USER+100),	// JMF���M�v��
		WM_USER_JMF_SEND_END,				// JMF���M�����ʒm
		WM_USER_DSP_SEND,					// ���M�f�[�^�\���ʒm
		WM_USER_DSP_RECV,					// ��M�f�[�^�\���ʒm
	};
	enum JmfType
	{	// JMF���
		E_JMF_QueryStatus,					// ��ԊĎ�
		E_JMF_QueryResource,				// �p�����o�^
		E_JMF_StopPersChParams,				// �`���l���폜
		E_JMF_CommandResource,				// ��������ݒ�
		E_JMF_CommandWakeup,				// READY��Ԃւ̑J�ڗv��
		// 
		E_JMF_SignalStatusPaper,			// UW�����SignalStatus�iPaper�j
		E_JMF_SignalStatus,					// UW�����SignalStatus
	};

	/* ==================================================================================================== */
	/**
		* @brief  ���W���[���̃J�����g�t�H���_���擾����
		* @retval ���W���[���̃J�����g�t�H���_
	**/
	/* ==================================================================================================== */
	static CString GetCurrentDirectory();

	/* ==================================================================================================== */
	/**
		* @brief  �c�[����ini�t�@�C��(JD_UW_Test_TOOL.ini)�p�X���擾
		* @retval �c�[����ini�t�@�C���p�X
	**/
	/* ==================================================================================================== */
	static CString GetToolIniPath();

	/* ==================================================================================================== */
	/**
		* @brief  �c�[���̃��[�Nini�t�@�C��(JD_UW_Test_WORK.ini)�p�X���擾
		*�@�@�@�@ JD_UW_Test_WORK.ini�͖{�c�[�����쐬����INI�t�@�C��
		* @retval �c�[���̃��[�Nini�t�@�C���p�X
	**/
	/* ==================================================================================================== */
	static CString GetWorkIniPath();

	/* ==================================================================================================== */
	/**
		* @brief  �w��t�@�C�����������ɓǂݍ���
		* @param  inFilePath	: [i]�t�@�C���p�X
		* @param  outData		: [o]�t�@�C���̓��e
		* @retval TRUE=����AFALSE=�ُ�
	**/
	/* ==================================================================================================== */
	static BOOL ReadFile( const CString& inFilePath, CString& outData );

	/* ==================================================================================================== */
	/**
		* @brief  ���ݎ������]�[���`���Ŗ߂�
		* @retval �]�[���`���̌��ݎ���
	**/
	/* ==================================================================================================== */
	static CString CreateTimeStamp();
};