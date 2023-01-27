/**
 * @file	Plugin_IF.h
 * @brief	機種ごとの I/F
 * @author pt.ju-tanaka
 * @date 2019/2/5 pt.ju-tanaka new
**/

#if !defined(__PLUGIN_IF_H_INCLUDE__)
#define __PLUGIN_IF_H_INCLUDE__

#include "Common.h"
#include "CommonDef_OP.h"

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
	 *  @brief	モジュールクラスオブジェクト作成
	 *  
	 *  @param[in] pHost ホスト情報へのポインタ
	 *  @param[out] moduleObject モジュールクラスポインタ
	 *  @retval TRUE: 成功
	 *  @retval FALSE: 失敗
	*/
	BOOL CreateModuleObject(PHOST pHost, void **moduleObject);

	PLUGIN_MODULE_API bool _PrintConditionExporter_GetCallbacks(struct SPrintConditionExporter_Callbacks*	outCallbacks);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __PLUGIN_IF_H_INCLUDE__
