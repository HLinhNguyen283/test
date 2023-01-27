/**
 * @file	ErrorLevel.h
 * @brief	エラーレベルを管理(継承して使う)
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once
#include <string>

/**
 * @brief	エラーレベルを管理(継承して使う)
**/
class CErrorLevel
{
protected:
	bool m_errorIfNotSet;		//!< 値がセット済みでなければエラーを起こすか否か(true：起こす)
	bool m_set;					//!< 値がセット済みか否か(true：値がセット済み)
	std::string m_ErrorMessage;	//!< Error() メソッド実行時にここが設定されていればこれを使う
public:

	/**
	 * @brief	constructor
	**/
	CErrorLevel() : m_errorIfNotSet(false), m_set(false)
	{
	}

	/**
	 * @brief	constructor
	 * @param[in]	inErrorIfNotSet : 値がセット済みでなければエラーを起こすか否か(true：起こす)
	**/
	CErrorLevel(bool inErrorIfNotSet) : m_errorIfNotSet(inErrorIfNotSet), m_set(false)
	{	
	}

	CErrorLevel(
		bool				inErrorIfNotSet,
		const std::string&	inMessage) : m_errorIfNotSet(inErrorIfNotSet), m_set(false), m_ErrorMessage(inMessage)
	{	
	}

	/**
	 * @brief	destructor
	**/
	virtual ~CErrorLevel()
	{	
	}

	/**
	 * @brief	値セット済みを設定
	**/
	void Set()
	{
		m_set = true;
	}

	/**
	 * @brief	値セット済みを判定
	 * @retval	true : セット設定済み
	 * @retval	false : セット未設定
	**/
	bool IsSet() const
	{
		return m_set;
	}

	/**
	 * @brief	値を設定していなくて、その場合にエラーを起こさせる設定の場合は例外を投げる
	**/
	void Error() const
	{
		if(m_set == false)
		{
			if(m_errorIfNotSet == true)
			{
				if (m_ErrorMessage.empty()) {
					throw std::runtime_error("Data are not set");
				} else {
					std::string msg("Data are not set:");
					msg.append(m_ErrorMessage);
					throw std::runtime_error(msg);
				}
			}
		}
	}
};

