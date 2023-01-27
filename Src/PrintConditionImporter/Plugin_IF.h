/**
 * @file	Plugin_IF.h
 * @brief	機種ごとの I/F
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
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
	struct SPrintConditionImporter_Callbacks;

	/**
	 *  @brief	モジュールクラスオブジェクト作成
	 *  
	 *  @param[in] pHost ホスト情報へのポインタ
	 *  @param[out] moduleObject モジュールクラスポインタ
	 *  @retval TRUE: 成功
	 *  @retval FALSE: 失敗
	*/
	BOOL CreateModuleObject(PHOST pHost, void **moduleObject);

	/**
	 * @brief	PrintConditionImporter 公開 callbackを取得する
	 * @param[out]	outCallbacks : PrintConditionImporter で公開する callback 定義構造体
	 * @retval	true : 取得成功
	 * @retval	false : 取得失敗
	**/
	PLUGIN_MODULE_API bool _PrintConditionImporter_GetCallbacks(struct SPrintConditionImporter_Callbacks* outCallbacks);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __PLUGIN_IF_H_INCLUDE__
