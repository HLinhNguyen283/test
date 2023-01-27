/*! \file Plugin_IF.h
 *  \brief 機種ごとのI/F
 *  \author Sugita
 *  \date 2013/04/25 Sugita 新規作成
*/


#if !defined(__PLUGIN_IF_H_INCLUDE__)
#define __PLUGIN_IF_H_INCLUDE__

#include "Common.h"
#include "CommonDef_OP.h"
#include "PaperDBDef_OP.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef PLUGINMODULE_EXPORTS
#define PLUGIN_MODULE_API __declspec(dllexport)
#else
#define PLUGIN_MODULE_API __declspec(dllimport)
#endif

	BOOL CreateModuleObject(PHOST pHost, void **moduleObject);
	PLUGIN_MODULE_API BOOL _ShowDialogForMaintenance();
	PLUGIN_MODULE_API bool _SetCurrentDotGainCurve(eComSystemType inSystem, const ST_PDB_DOTGAIN_CURVE& inDotgainCurve);
	PLUGIN_MODULE_API bool _GetCurrentDotGainCurve(eComSystemType inSystem, ST_PDB_DOTGAIN_CURVE& inDotgainCurve);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __PLUGIN_IF_H_INCLUDE__
