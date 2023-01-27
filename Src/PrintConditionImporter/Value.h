/**
 * @file	Value.h
 * @brief	エレメントの値１個分
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#if !defined(__VALUE_H_INCLUDE__)
#define __VALUE_H_INCLUDE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorLevel.h"

/**
 * @brief	エレメントの値１個分
**/
template<class T1>
class CValue : public CErrorLevel
{
private:
	T1 m_val;		//!< エレメントの値

public:
	/**
	 * @brief	constructor
	**/
	CValue() : CErrorLevel(false), m_val(T1())
	{		
	}

	/**
	 * @brief	constructor
	 * @param[in]	inErrorIfNotSet : 値がセット済みでなければエラーを起こすか否か(true：起こす)
	**/
	CValue(bool inErrorIfNotSet) : CErrorLevel(inErrorIfNotSet), m_val(T1())
	{	
	}

	CValue(
		bool				inErrorIfNotSet,
		const std::string&	inErrorMessage) : CErrorLevel(inErrorIfNotSet, inErrorMessage), m_val(T1())
	{	
	}

	/**
	 * @brief	エレメントの値をセットする
	 * @param[in]	inVal : エレメントの値
	**/
	void Set(T1 inVal)
	{
		m_val = inVal;
		//値をセット済みとする
		CErrorLevel::Set();
	}

	/**
	 * @brief	エレメントの値をゲットする
	 * @param[out]	outVal : エレメントの値
	 * @retval	true : 成功
	 * @retval	false : 失敗 (未設定の値)
	**/
	bool Get(T1& outVal) const
	{
		//セット済みなら値を返す
		if(CErrorLevel::IsSet() == true)
		{
			outVal = m_val;
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * @brief	エレメントの値をゲットする(必須項目でのみ使うこと)
	 * @retval	エレメントの値
	 * @note	本関数は値が設定されている状態でのみ使用すること。
	**/
	const T1& Get() const
	{
		//値を返す
		if(CErrorLevel::IsSet() == true)
		{
			return m_val;
		}
		else
		{
			//呼ぶ前にCErrorLevel::Error()を呼んで例外が出ないことを確定させておくこと
			throw std::runtime_error("呼んではいけない");
		}
	}
};


#endif // !defined(__VALUE_H_INCLUDE__)
