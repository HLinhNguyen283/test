/*! \file DataPreOverCoat.h
 *  \brief data class: Precoat and Overcoat data
 *  \author GCS Tan Ho
 *  \date 2013/12/11 GCS Tan Ho created
*/

#if !defined(_DATA_PRE_OVER_COAT_H_)
#define _DATA_PRE_OVER_COAT_H_

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
class CDataPreOverCoat
{
public:
	//method
	CDataPreOverCoat();
	virtual ~CDataPreOverCoat();
	bool Initialize(CDataSystemSetting* inSystemSetting);
	bool Finalize();

	bool IsChanged();
	bool Save();

	void GetPrecoatRange(long* outMin, long* outMax);
	void SetSelectPrecoat(long inPrinterID, long inItemID);
	bool GetSelectPrecoat(long inPrinterID, long* outItemID);

	void GetOvercoatRange(long* outMin, long* outMax);
	void SetSelectOvercoat(long inPrinterID, long inItemID);
	bool GetSelectOvercoat(long inPrinterID, long* outItemID);

	void UpdateCurrentPreOverCoat(const char* inKey);

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

	long m_precoatMin;								//!< Min value of Precoat
	long m_precoatMax;								//!< Max value of Precoat
	long m_selectedPrecoatID;						//!< Selected precoat id

	long m_overcoatMin;								//!< Min value of Overcoat
	long m_overcoatMax;								//!< Max value of Overcoat
	long m_selectedOvercoatID;						//!< Selected Overcoat id

	ST_PDB_PRE_OVER_COAT m_preOverCoat;				//!< Precoat and overcoat info
	ST_PDB_PRE_OVER_COAT m_currPreOverCoat;			//!< The current setting precoat, overcoat of Print Condition Name

private:
	//member


};


#endif // !defined(_DATA_PRE_OVER_COAT_H_)
