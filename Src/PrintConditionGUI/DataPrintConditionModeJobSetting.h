/*! \file DataPrintConditionModeJobSetting.h
 *  \brief 印刷条件モードジョブ設定データクラス
 *  \author okada
 *  \date 2014/11/21 okada 新規作成
*/

#if !defined(__DATA_PRINTCONDITIONMODE_JOBSETTING_H__)
#define __DATA_PRINTCONDITIONMODE_JOBSETTING_H__

class CDataIF;
class CDataSystemSetting;
struct SPaperDB_Callbacks;

/*!
 *  \brief 印刷条件モードジョブ設定データクラス
*/
class CDataPrintConditionModeJobSetting
{
public:
	CDataPrintConditionModeJobSetting();
	virtual ~CDataPrintConditionModeJobSetting();

	bool Initialize(CDataIF* inDataIF, CDataSystemSetting* inSDataystemSetting);

	// ジョブ設定開始
	bool StartJobSetting();

protected:
	CDataIF* m_DataIF;
	CDataSystemSetting* m_DataSstemSetting;

	SPaperDB_Callbacks*	m_PaperDBCallbacks;

};

#endif // !defined(__DATA_PRINTCONDITIONMODE_JOBSETTING_H__)
