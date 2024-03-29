/**
	* @file		MakeComposeUnwinderData.h
	* @brief	UWandRW_Compose_Xml起動パラメータ作成 ヘッダーファイル
	* @author	cec K.Miyachi
	* @date		2021/12/24 cec K.Miyachi 新規作成
**/

#pragma once

#include <map>

namespace unwinder_manager
{

typedef std::map<std::string, std::string> UwXjmfDataMap;

//
// 印刷条件情報
//
class CUnwinderPaper
{
public:
	std::string DescriptiveName;		// 印刷条件名
	std::string Dimension;				// 用紙幅と残量
	std::string MediaType;				// メディア名
	std::string Thickness;				// 紙厚
	std::string UWTension;				// テンション
	std::string MaxRunSpeed;			// 印刷最高速度
	std::string ExternalID;				// 外部ID
};

/**
* @brief UWandRW_Compose_Xml起動パラメータ作成クラス
*/
class CMakeComposeUnwinderData
{
public:
	//
	// UWへのXJMF要求
	//
	enum TestNo
	{ 
		E_Compose_QueryStatus,				// 状態監視用チャネル登録
		E_Compose_QueryResource,			// 用紙情報通知用チャネル登録
		E_Compose_StopPersChParams,			// チャネル削除
		E_Compose_CommandResource,			// 印刷条件設定
		E_Compose_CommandWakeup,			// READY状態への遷移
	};

public:
	/**
		* @brief  CMakeComposeUnwinderDataコンストラクタ
	**/
	CMakeComposeUnwinderData();

	/**
		* @brief  CMakeComposeUnwinderDataデストラクタ
	**/
	virtual ~CMakeComposeUnwinderData();

	/**
		* @brief  状態監視用チャネル登録（QueryStatus）の送信データを作成する
		* @retval XML送信データMAP
	**/
	UwXjmfDataMap ParameterQueryStatus();

	/**
		* @brief  用紙情報通知用チャネル登録（QueryResource）の送信データを作成する
		* @retval XML送信データMAP
	**/
	UwXjmfDataMap ParameterQueryResource();

	/**
		* @brief  チャネル削除（StopPersChParams）の送信データを作成する
		* @param  inType : [i]削除チャネル種別(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval XML送信データMAP
	**/
	UwXjmfDataMap ParameterStopPersChParams(int inType);

	/**
		* @brief  印刷条件設定（CommandResource）の送信データを作成する
		* @param  inUnwinderPaper : [i]印刷条件情報
		* @retval XML送信データMAP
	**/
	UwXjmfDataMap ParameterCommandResource(const CUnwinderPaper& inUnwinderPaper);

	/**
		* @brief  READY状態への遷移（CommandWakeup）の送信データを作成する
		* @retval XML送信データMAP
	**/
	UwXjmfDataMap ParameterCommandWakeup();

protected:
	/**
		* @brief  現在時刻をゾーン形式で取得する
		* @retval ゾーン形式の現在時刻
	**/
	std::string GetTimeStamp();

	/**
		* @brief  XJMFに設定するDeviceIDを取得する
		* @retval DeviceID
	**/
	std::string GetDeviceID();

	/**
		* @brief  XJMFの種別文字列を取得する
		* @param  inType : [i]XJMFの種別
		* @retval XJMFの種別文字列
	**/
	std::string GetType(int inType);

	/**
		* @brief  XJMFに設定するリピートタイムを取得する
		* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval リピートタイム
	**/
	std::string GetRepeatTime(int inType);

	/**
		* @brief  XJMFに設定するURLを取得する
		* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval URL
	**/
	std::string GetURL(int inType);

	/**
		* @brief  XJMFに設定するChannelModeを取得する
		* @retval ChannelMode
	**/
	std::string GetChannelMode();

	/**
		* @brief  XJMFに設定するResourceDetailsを取得する
		* @retval ResourceDetails
	**/
	std::string GetResourceDetails();

	/**
		* @brief  XJMFに設定するResourceNameを取得する
		* @retval ResourceName
	**/
	std::string GetResourceName();

	/**
		* @brief  XJMFに設定するScopeを取得する
		* @retval Scope
	**/
	std::string GetScope();

	/**
		* @brief  XJMFに設定するチャネルＩＤを取得する
		* @param  inType : [i]JMF種別(E_Compose_QueryStatus/E_Compose_QueryResource)
		* @retval チャネルＩＤ
	**/
	std::string GetChannelID(int inType);

	/**
		* @brief  XJMFに設定するMessageTypeを取得する
		* @retval MessageType
	**/
	std::string GetMessageType();

	/**
		* @brief  XJMFに設定するUpdateMethodを取得する
		* @retval UpdateMethod
	**/
	std::string GetUpdateMethod();

	/**
		* @brief  XJMFに設定するNameを取得する
		* @retval Name
	**/
	std::string GetName();

	/**
		* @brief  XJMFに設定するIDを取得する
		*         ＩＤは、接頭文字_日付(YYYYMMDD)_日単位・JMF単位の連番(1〜n)
		* @param  inType : [i]JMF種別
		* @retval ID
	**/
	std::string GetID(int inType);

	/**
		* @brief  JMF種別毎の前回要求した送信IDを取得する
		* @param  inType : [i]JMF種別
		* @retval 送信ID
	**/
	UINT GetSendID(int inType);

	/**
		* @brief  JMF種別毎の送信IDを保存する
		* @param  inType   : [i]JMF種別
		* @param  inSendID : [i]送信ＩＤ

		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL SetSendID(int inType, UINT inSendID);

	/**
		* @brief  JMF種別毎の送信IDをリセットする
	**/
	void ResetSendID();

	/**
		* @brief  JMF種別毎のIDの接頭文字を取得する
		* @param  inType : [i]JMF種別
		* @retval 接頭文字
	**/
	std::string GetPrefix(int inType);

	/**
		* @brief  前回JMF要求した日付を取得する
		* @retval 日付
	**/
	std::string GetLastSendData();

	/**
		* @brief  状態監視用チャネル登録の送信IDを取得する
		* @retval 状態監視用チャネル登録の送信ID
	**/
	UINT GetQueryStatusID();

	/**
		* @brief  用紙情報通知用チャネル登録の送信IDを取得する
		* @retval 用紙情報通知用チャネル登録の送信ID
	**/
	UINT GetQueryResourceID();

	/**
		* @brief  チャネル削除の送信IDを取得する
		* @retval チャネル削除の送信ID
	**/
	UINT GetStopPersChParamsID();

	/**
		* @brief  印刷条件設定の送信IDを取得する
		* @retval 印刷条件設定の送信ID
	**/
	UINT GetCommandResourceID();

	/**
		* @brief  READY状態への遷移の送信IDを取得する
		* @retval READY状態への遷移の送信ID
	**/
	UINT GetCommandWakeupID();

	/**
		* @brief  JMF要求した日付を更新する
		* @param  inDate : [i]日付(YYYYMMDD)
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutLastSendData(const std::string& inDate);

	/**
		* @brief  状態監視用チャネル登録の送信IDを更新する
		* @param  inID : 状態監視用チャネル登録の送信ID
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutQueryStatusID(const std::string& inID);

	/**
		* @brief  用紙情報通知用チャネル登録を更新する
		* @param  inID : 用紙情報通知用チャネル登録の送信ID
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutQueryResourceID(const std::string& inID);

	/**
		* @brief  チャネル削除の送信IDを更新する
		* @param  inID : チャネル削除の送信ID
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutStopPersChParamsID(const std::string& inID);

	/**
		* @brief  印刷条件設定の送信IDを更新する
		* @param  inID : 印刷条件設定の送信ID
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutCommandResourceID(const std::string& inID);

	/**
		* @brief  READY状態への遷移の送信IDを更新する
		* @param  inID : READY状態への遷移の送信ID
		* @retval TRUE：正常
		* @retval FALSE：異常
	**/
	BOOL PutCommandWakeupID(const std::string& inID);
};

};	// namespace unwinder_manager