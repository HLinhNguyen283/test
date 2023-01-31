/**
 * @file  CriticalSection.h
 * @brief クリティカルセクションクラス ヘッダーファイル
 * @author screen
 * @date  20210720 screen 新規作成
 **/
#pragma once

// ===========================================================================
//
// CCriticalSectionLocker クラス
//
// ===========================================================================
/**
 * @brief CCriticalSectionLockerクラス
*/
class CCriticalSectionLocker
{
public:
	CCriticalSectionLocker(LPCRITICAL_SECTION cs) : m_cs(cs) { EnterCriticalSection(m_cs); }
	~CCriticalSectionLocker() { LeaveCriticalSection(m_cs); }

private:
	LPCRITICAL_SECTION m_cs;		//!< クリティカルセクション
};

// ===========================================================================
//
// CCriticalSectionUnlocker クラス
//
// ===========================================================================
/**
 * @brief CCriticalSectionUnlockerクラス
*/
class CCriticalSectionUnlocker
{
public:
	CCriticalSectionUnlocker(LPCRITICAL_SECTION cs) : m_cs(cs) { LeaveCriticalSection(m_cs); }
	~CCriticalSectionUnlocker() { EnterCriticalSection(m_cs); }

private:
	LPCRITICAL_SECTION m_cs;		//!< クリティカルセクション
};