/**
 * @file	PrintConditionExporter.h
 * @brief	PrintConditionExporter
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/

#pragma once

#include "ProcPlugin_Common.h"

#define PLUGIN_VERSION 0x0000

/**
 * @brief	PrintConditionExporter
**/
class CPrintConditionExporter : public CProcPlugin_Common
{
public:
	/**
	 * @brief	constructor
	**/
	CPrintConditionExporter();

	/**
	 * @brief	destructor
	**/
	virtual ~CPrintConditionExporter();

protected:

};

