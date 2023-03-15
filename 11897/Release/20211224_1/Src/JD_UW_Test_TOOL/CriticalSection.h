/**
 * @file  CriticalSection.h
 * @brief �N���e�B�J���Z�N�V�����N���X �w�b�_�[�t�@�C��
 * @author screen
 * @date  20210720 screen �V�K�쐬
 **/
#pragma once

// ===========================================================================
//
// CCriticalSectionLocker �N���X
//
// ===========================================================================
/**
 * @brief CCriticalSectionLocker�N���X
*/
class CCriticalSectionLocker
{
public:
	CCriticalSectionLocker(LPCRITICAL_SECTION cs) : m_cs(cs) { EnterCriticalSection(m_cs); }
	~CCriticalSectionLocker() { LeaveCriticalSection(m_cs); }

private:
	LPCRITICAL_SECTION m_cs;		//!< �N���e�B�J���Z�N�V����
};

// ===========================================================================
//
// CCriticalSectionUnlocker �N���X
//
// ===========================================================================
/**
 * @brief CCriticalSectionUnlocker�N���X
*/
class CCriticalSectionUnlocker
{
public:
	CCriticalSectionUnlocker(LPCRITICAL_SECTION cs) : m_cs(cs) { LeaveCriticalSection(m_cs); }
	~CCriticalSectionUnlocker() { EnterCriticalSection(m_cs); }

private:
	LPCRITICAL_SECTION m_cs;		//!< �N���e�B�J���Z�N�V����
};