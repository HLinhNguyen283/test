/*! \file WriteTextFile.h
 *  \brief WriteTextFile.h�̏���
 *  \author pt.ju-tanaka
 *  \date 2019/03/11 pt.ju-tanaka �V�K�쐬
*/

#pragma once

#include "Common.h"
#include "PaperDB_MigrationDef.h"


/**
 * @brief �G�N�X�|�[�g��Ƀe�L�X�g�t�@�C�����o�͂���
**/
class ExportWriteTextFile
{
public:
	/**
	 * �R���X�g���N�^
	**/
	ExportWriteTextFile();

	/**
	 * �f�X�g���N�^
	**/
	virtual ~ExportWriteTextFile();

	/**
	 * �e�L�X�g�t�@�C���o�̓C�x���g
	 * @param[in]	inExportFilePath : �G�N�X�|�[�g��̃t�@�C���p�X
	 * @param[in]	inMigrationData : �t�@�C���o�͑Ώۂ̋Ɩ��ڍs�p�\����
	 * @retval	true�F�o�͐���
	 * @retval	false�F���s
	**/
	bool onWriteTextFile(const std::string& inExportFilePath, const ST_PDB_MIGRATION_DATA& inMigrationData);

private:
	class API;

	/**
	 * GetLastError�̕�������擾����B
	 * @retval	GetLastError�̕�����
	**/
	std::string getLastError();

	/**
	 * ���l���u"%0.2f"�v�����̕�����ŕԂ�
	 * @param[in]	inVal :�����l�B
	 * @retval	�ϊ��㕶����
	**/
	std::string changeTo02f(double inVal);

	/**
	 * ���l���u"%0.2f"�v�����̕�����ŕԂ�
	 * @param[in]	inVal :�����l�B
	 * @retval	�ϊ��㕶����
	**/
	std::string changeTo02f(double x, double y);

	/**
	 * ���l���u"%0.2f"�v�����̕�����ŕԂ�
	 * @param[in]	inVal :�����l�B
	 * @retval	�ϊ��㕶����
	**/
	std::string changeTo02f(double sx, double sy, double ex, double ey);

	/**
	 * ���l->������ϊ�(string)���ĕԂ�
	 * @param[in]	inVal :�ϊ������l
	 * @param[in]	inPrecision�F�ϊ���̏�������
	 * @retval	�ϊ��㕶����
	**/
	std::string convertNumToStringPrecision(double inValue, UINT inPrecision);

	/**
	 * ���l���u"%d"�v�����̕�����ŕԂ�
	 * @param[in]	inVal :�����l�B
	 * @retval	�ϊ��㕶����
	**/
	std::string changeToD(long inVal);

	/**
	 * �y�[�W����T�C�Y������擾
	 * @param[in]	inPaperSizeH :�y�[�W����T�C�Y���B
	 * @retval	�ϊ��㕶����
	**/
	std::string getPaperFeed(const ST_PDB_PAPER_SIZE_H& inPaperSizeH);

	/**
	 * �y�[�W����T�C�Y�̒P�ʕ�����擾
	 * @param[in]	inUnitMode :�P�ʁB
	 * @retval	�ϊ��㕶����
	**/
	std::string getPaperFeedUnit(const eUNIT_MODE inUnitMode);

	/**
	 * �p���`������擾
	 * @param[in]	inPaperOption :����I�v�V�����B
	 * @retval	�ϊ��㕶����
	**/
	std::string getPaperPunch(const ST_PDB_PAPER_OPTION& inPaperOption);

	/**
	 * �utrue�v�ufalse�v������擾
	 * @param[in]	inVal :���l�B
	 * @retval	�ϊ��㕶����
	**/
	std::string getTrueOrFalse(BOOL inVal);

	/**
	 * ���O����}�[�N������擾
	 * @param[in]	inPaperOption :����I�v�V�����B
	 * @retval	�ϊ��㕶����
	**/
	std::string getParamMark(const ST_PDB_PAPER_OPTION& inPaperOption);

	/**
	 * �g�[���J�[�u�i���́j������擾
	 * @param[in]	inDotgainCurve :�g�[���J�[�u���B
	 * @param[out]	outVal :�ϊ��㕶����B
	 * @retval	true�F����
	 * @retval	false�F���s
	**/
	bool getToneCurve(const ST_PDB_DOTGAIN_CURVE& inDotgainCurve, std::string& outVal);

	/**
	 * �t���b�V���O�p�^�[��������擾
	 * @param[in]	inFlushingPatternKind :�t���b�V���O�p�^�[����ށB
	 * @retval	�ϊ��㕶����
	**/
	std::string getFlushingPatternKind(DEF_FLUSHING_PATTERN inFlushingPatternKind);

	/**
	 * �t���b�V���O�p�^�[�����C�������炵�ʕ�����擾
	 * @param[in]	inFlushingPattern :�t���b�V���O�p�^�[���B
	 * @param[out]	outVal :�ϊ��㕶����B
	 * @retval	true�F����
	 * @retval	false�F���s
	**/
	bool getFlushingPatternOffset(const ST_JOB_FLUSHING_PATTERN& inFlushingPattern, std::string& outVal);

	/**
	 * �T�C�h�����}�[�N��ޕ�����擾
	 * @param[in]	inSideCheckMarkKind :�T�C�h�����}�[�N���B
	 * @retval	�ϊ��㕶����
	**/
	std::string getSideCheckMarkKind(const ST_JOB_VERIFY_MARK& inSideCheckMarkKind);

	/**
	 * �㏈���}�[�N��ޕ�����擾
	 * @param[in]	inSide :�ʏ��B
	 * @retval	�ϊ��㕶����
	**/
	std::string getPostMarkKind(DEF_SELECT_SIDE inSide);

	/**
	 * ���C���t���b�V���O�A�I�v�V����������擾
	 * @param[in]	inOption :���C���t���b�V���O�A�I�v�V�����B
	 * @retval	�ϊ��㕶����
	**/
	std::string getFlushingPatternOption(const ST_JOB_FLUSHING_PATTERN_ADD& inOption);

	/**
	 * ������N�ʂ܂łɌv�Z�i������N+1�ʂ��l�̌ܓ��j���ĕԂ�
	 * @param[in]	inSrc :�����l�B
	 * @param[in]	inLen :������N�ʂ܂Ōv�Z���ĕԂ��Ăق���N�̒l�B
	 * @retval	�ϊ��㕶����
	**/
	double roundD(double inSrc, int inLen);

	/**
	 * �t���b�V���O�p�^�[����Line*���ǂ����擾
	 * @param[in]	inFlushingPatternKind :�t���b�V���O�p�^�[�����
	 * @retval	true�F�t���b�V���O�p�^�[����Line*
	 * @retval	false�F�t���b�V���O�p�^�[����Line*�ȊO
	**/
	bool isFlushingPatternLine(DEF_FLUSHING_PATTERN inFlushingPatternKind);

	/**
		@brief	�����~�ݒ蕶����擾
		@param[in]	inPrintStopSettingKind :�����~�ݒ�̗񋓒l
		@retval	�ϊ��㕶����
	*/
	std::string getPrintStopSettingKind(const EPrintStopSetting& inPrintStopSettingKind);
};