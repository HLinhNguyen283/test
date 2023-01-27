/**
 * @file	PrintConditionImporter.h
 * @brief	PrintConditionImporter
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
**/

#pragma once

#include "ProcPlugin_Common.h"

#define PLUGIN_VERSION 0x0000

/**
 * @brief	PrintConditionImporter
**/
class CPrintConditionImporter : public CProcPlugin_Common
{
public:
	/**
	 * @brief	constructor
	**/
	CPrintConditionImporter();

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionImporter();

protected:

};

