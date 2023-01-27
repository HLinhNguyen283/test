/**
 *	@file	UnwinderManager.cpp
 *	@brief	UnwinderManager
**/

#if !defined(__UNWINDER_MANAGER__)
#define __UNWINDER_MANAGER__

#include "ProcPlugin_Common.h"

#define PLUGIN_VERSION 0x0000

/**
 * @brief	HeadCleaningSetting
**/
class CUwRwManager : public CProcPlugin_Common
{
public:
	/**
	 * @brief	constructor
	**/
	CUwRwManager();

	/**
	 * @brief	destructor
	**/
	virtual ~CUwRwManager();

protected:

};

#endif // !defined(__UNWINDER_MANAGER__)