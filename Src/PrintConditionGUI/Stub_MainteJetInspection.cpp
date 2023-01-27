/*! \file Stub_MainteJetInspection.h
 *  \brief Stub for Mainte JetInspection
 *  \author GCS Tan Ho
 *  \date 2013/12/17 GCST Tan Ho created
*/
#include <StdAfx.h>
#include "Stub_MainteJetInspection.h"
#include <vector>
#include <string>
using namespace std;

vector<string> JetInspectionList;

BOOL __MainteJetInspection_EnterInspectionParamsDBCS()
{
	JetInspectionList.clear();

	char strJI[64] = {0};
	for (int i = 0; i < 15; ++i)
	{
		memset(strJI, 0x00, sizeof(strJI));
		_snprintf_s(strJI, _TRUNCATE, "Jet Inspection %d", i + 1);
		JetInspectionList.push_back(strJI);
	}
	return TRUE;
}

BOOL __MainteJetInspection_LeaveInspectionParamsDBCS()
{
	return TRUE;
}

BOOL __MainteJetInspection_GetInspectionParamsName(long inIndex, char outInspectionParamsName[])
{
	if (inIndex < 0 || inIndex >= (long)JetInspectionList.size())
	{
		return FALSE;
	}

	strcpy(outInspectionParamsName, JetInspectionList.at(inIndex).c_str());

	return TRUE;
}