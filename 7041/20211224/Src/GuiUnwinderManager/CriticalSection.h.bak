// ===========================================================================
//
//	Copyright (C) 2019 SCREEN Graphic Solutions Co., Ltd.
//	CONFIDENTIAL Proprietary to SCREEN Graphic Solutions Co., Ltd.
//
//	本プログラムの著作権は株式会社SCREENグラフィックソリューションズに帰属するものであり、
//	同社はこれを営業秘密として管理するものです。従い、本プログラムの全て、
//	一部にかかわらず、その複製、頒布を行うことは、同社の事前の書面による
//	承諾がない限り固く禁じられるものです。
//
//	The copyright of this program shall belong to 
//	SCREEN Graphic Solutions Co., Ltd.("SCREEN") as a "work made for hire." 
//	Also, SCREEN will treat this program as its trade secret. Accordingly, 
//	no one is allowed to copy and/or distribute this program, as a whole or 
//	in part, without obtaining SCREEN' prior permission to do so in writing.
//
// ===========================================================================

/**
 * @file  CriticalSection.h
 * @brief クリティカルセクションクラス ヘッダーファイル
 * @date  20190610 CEC Miyachi 新規作成
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