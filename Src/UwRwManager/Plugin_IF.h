/**
 *	@file	Plugin_IF.h
 *	@brief	I / F for each model
**/

#if !defined(__PLUGIN_IF_H_INCLUDE__)
#define __PLUGIN_IF_H_INCLUDE__

#include "Common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef PLUGINMODULE_EXPORTS
#define PLUGIN_MODULE_API __declspec(dllexport)
#else
#define PLUGIN_MODULE_API __declspec(dllimport)
#endif

	/**
	 *	Module class object creation
	 *	@param[in]	pHost : Pointer to host information
	 *	@param[out]	moduleObject : Module class pointer
	 *	@retval true : Success
	 *	@retval false : Failure
	**/
	BOOL CreateModuleObject(PHOST pHost, void **moduleObject);

	struct SUwRwManager_Callbacks;

	/**
	 *  @brief	Get the UnwinderManager callback
	 *  @param[out]	outCallbacks : callback struct
	 *  @retval true : Success
	 *  @retval false : Failure
	**/
	PLUGIN_MODULE_API bool _UnwinderManager_GetCallbacks(struct SUwRwManager_Callbacks* outCallbacks);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __PLUGIN_IF_H_INCLUDE__
