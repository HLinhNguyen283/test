/*! \file Stub_MainteJetInspection.h
 *  \brief Stub for Mainte JetInspection
 *  \author GCS Tan Ho
 *  \date 2013/12/17 GCST Tan Ho created
*/
#include "StdAfx.h"
#include <Windows.h>

/*!
 * \brief 検査条件DBアクセスCSを取得する./ Acquire Critical Section for DB access
 * \return TRUE:Success/FALSE:Failure
 */
BOOL __MainteJetInspection_EnterInspectionParamsDBCS();


/*!
 * \brief 検査条件DBアクセスCSを解放する./ Release Critical Section
 * \return TRUE:Success/FALSE:Failure
 */
BOOL __MainteJetInspection_LeaveInspectionParamsDBCS();


/*!
 * \brief 指定されたインデックスの検査条件名を取得する./ Obtain the inspection setting specified by the index
          前後でCS取得、解放を行う必要がある. / Acquire Critical Section before calling the function and release it after processing is done
 * \param inIndex [IN]インデックス./ index
 * \param outInspectionParamsName [OUT]取得できた検査条件名./ inspection setting
 * \return TRUE:Success/FALSE:Failure
 */
BOOL __MainteJetInspection_GetInspectionParamsName(long inIndex, char outInspectionParamsName[]);
