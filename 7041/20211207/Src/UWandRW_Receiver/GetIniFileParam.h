/**
	* @file		GetIniFileParam.h
	* @brief	INI�t�@�C���l�擾 �w�b�_�[�t�@�C��
	* @author	cec K.Miyachi
	* @date		2021/12/07 cec K.Miyachi �V�K�쐬
**/
#pragma once

/**
* @brief INI���擾/�X�V�N���X
*/
class CGetIniFileParam
{
	// TP-UW_Communication.ini�̃t�@�C���p�X�擾
	static CString GetUnwinderIniFilePath();
	// PrinterDescriptor.ini�̃t�@�C���p�X�擾
	static CString GetPrinterDescriptorIniFilePath();
public:
	// Unwindwe�@�\�̗L��/����
	static BOOL GetUnwinderOption();
	// UW�����SignalStatus����M����|�[�g
	static UINT GetRecvPortNumber();
	// UW�����SignalStatus����M����IP�A�h���X
	static CString GetRecvIpAddress();
	// Status��M�t�H���_��
	static CString GetRecvStatusFolder();
	// Paper��M�t�H���_��
	static CString GetRecvPaperFolder();
	// Status��MURL��
	static CString GetRecvStatusURL();
	// Paper��MURL��
	static CString GetRecvPaperURL();
	// ���O�ۑ�����
	static BOOL GetLogRetentionPeriod();
	// ���O���x��
	static UINT GetDebugLogLevel();
};