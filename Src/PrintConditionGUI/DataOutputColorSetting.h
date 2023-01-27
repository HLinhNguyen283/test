/*! \file DataOutputColorSetting.h
 *  \brief DataOutputColorSetting.hÇÃèàóù
 *  \author SCREEN
 *  \date 2017/11/20 SCREEN êVãKçÏê¨
*/

#if !defined(_DATA_OUTPUT_COLOR_SETTING_H_)
#define _DATA_OUTPUT_COLOR_SETTING_H_

// ==========================================================================
// Include files

#include <windows.h>
#include "DataSystemSetting.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// ==========================================================================

/*! \brief data class: Precoat and Overcoat
*/
class CDataOutputColorSetting
{
public:
	//method
	CDataOutputColorSetting();
	virtual ~CDataOutputColorSetting();
	bool Initialize(CDataSystemSetting* inSystemSetting);
	bool Finalize();

	bool IsChanged();
	bool Save();

	void SetSelectOutputColor(long inItemID);
	bool GetSelectOutputColor(long* outItemID);

	void UpdateCurrentOutputColor(const char* inKey);

	bool EnterScreen(const char* inPrintConditionOrSectionID);
	bool ExitScreen();


protected:
	//method
	void ResetMember();


	//event

	//parts

	//member
	SPaperDB_Callbacks* m_PaperDB_Callbacks;			//!< Callbacks for Paper DB
	SJobManager_Callbacks* m_JM_Callbacks;			//!< Callbacks for Job Manager

	CDataSystemSetting* m_systemSetting;			//!< Data System setting handle

	string m_printConditionName;					//!< In case the GUI is started from Job List Screen : SectionID
													//!< In case the GUI is started from System Setting Screen : Print Condition Name

	DEF_OUTPUT_COLOR_MODE m_preOutputColor;			//!< The select output color
	DEF_OUTPUT_COLOR_MODE m_curOutputColor;			//!< The current setting output color of Print Condition Name

private:
	//member


};


#endif // !defined(_DATA_OUTPUT_COLOR_SETTING_H_)
