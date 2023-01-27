/**
	* @file		ReceiveSignalStatus.h
	* @brief	SignalStatus受信 ヘッダファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include <map>

#define ErrorLabel		"[ERROR]"
#define THICKNESS_INVALID_VALUE (-1)

class CDataIF;
namespace uwrw_manager
{
typedef std::map<std::string, std::string> UwRwXjmfDataMap;

class CRequestUwRwThread;
class CReceiveSignalStatus
{
public:
	/**
	 * @brief Get instance of the class
	**/
	static CReceiveSignalStatus& GetInstance(const char* inDeviceName)
	{
		static CReceiveSignalStatus receiveUWSignalStatusInstance;
		static CReceiveSignalStatus receiveRWSignalStatusInstance;
		return (strcmp(inDeviceName, "UW")? receiveRWSignalStatusInstance: receiveUWSignalStatusInstance);
	}

	/**
		* @brief  CReceiveSignalStatus destructor
	**/
	virtual ~CReceiveSignalStatus();

	/**
	 * @brief  Receive signal status info from UW
	**/
	void ReceiveSignalStatusInfo(CDataIF* inDataIF);

	/**
	 * @brief Receive signal status info notified from UWandRW_Receiver
	**/
	BOOL ReceiveInfo();

	/**
	 * @brief Set a CRequestUwRwThread object
	 * @param[in] inRequestThread CRequestUwRwThread object
	**/
	void SetRequestThread(CRequestUwRwThread* inRequestThread);

	/**
	 * @brief Get the paper info receiving status (whether or not the info has been received)
	**/
	void HasPaperInfoReceived(bool &outHasPaperInfoReceived);
	/**
	 * @brief Get value of paper thickness which is notified from UW
	**/
	void GetUWPaperThickness(long &outThickness);
	/**
	 * @brief Get value of paper remaining amount which is notified from UW
	**/
	void GetUWPaperRemainingAmount(long &outPaperRemainingAmount);
	/**
	 * @brief Set value of paper remaining amount
	**/
	void SetUWPaperRemainingAmount(long inPaperRemainingAmount);
	/**
	 * @brief Set status of paper info receiving
	**/
	void SetStatusOfPaperInfoReceiving(bool status);
	/**
	 * @brief Set the status for starting of paper info receiving (status is true if it is the first time paper info received from UW from when the controller is started, else it is false)
	**/
	void SetRecvPaperInfoStartingStatus(bool status);
	/**
	 * @brief Call from CReceiveSignalStatusThread to set/reset break loop condition
	 * @param[in] inVal Value to set
	**/
	void SetExit(bool inVal);

private:
	/**
	 * @brief  CReceiveSignalStatus constructor
	**/
	CReceiveSignalStatus();

	/**
	 * @brief  pipe reading
	**/
	BOOL ReadData(HANDLE inPipe, char* outData, DWORD inSize);

	/**
	 * @brief  analyze signal status info notified from UWandRW_Receiver
	**/
	BOOL AnalyzeData(const std::string& inXmldata);

	/**
	 * @brief  parse xml data
	**/
	std::string ExecuteParseXml( const std::string& inSignalData, UwRwXjmfDataMap& outUwXjmfDataMap );

	/**
	 * @brief  processing when the status info is received
	**/
	BOOL ReceiveStatusInfo(const std::string& inStatus);

	/**
	 * @brief  processing when the paper info is received
	**/
	BOOL ReceivePaperInfo(const std::string& inDescriptiveName,
		const std::string& inDimension,
		const std::string& inMediaType,
		const std::string& inRollDiameter,
		const std::string& inThickness,
		const std::string& inWeight,
		const std::string& inStopRollDiameter,
		const std::string& inUWWebTension,
		const std::string& inRWWebTension,
		const std::string& inRWRollTension);

	CRequestUwRwThread* m_RequestThread;
	CDataIF* m_dataIF;

	bool m_paperInfoReceivingStatus; // status of paper info receiving (if there is any error when receiving of paper info from UW, it is false, otherwise it is true)
	bool m_receivedPaperInfo; // whether or not the paper info has been received
	bool m_isRecvPaperInfoStarting; // whether it is the first time of paper info receiving from UW since the controller was started
	long m_UWThickness; // Value of thickness notified from UW.
	long m_paperRemainingAmount; // Value of paper remaining amount notified from UW.
	bool m_IsExit; // set when thread exit
};

};	// namespace uwrw_manager