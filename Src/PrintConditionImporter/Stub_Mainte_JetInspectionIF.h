/**
	@file	Stub_Mainte_JetInspectionIF.h
	@brief	stub of Mainte_JetInspection plugin
	@author	K.Matsuoka
	@date	2019/07/17 K.Matsuoka 新規作成
*/

#pragma once

// ==========================================================================
// Include files
#include "StructJetInspector.h"
#include <vector>


// ==========================================================================

/**
	stub of Mainte_JetInspection plugin
*/
class CStub_Mainte_JetInspectionIF
{
private:
	/**
		constructor
	*/
	CStub_Mainte_JetInspectionIF();

public:

	/**
		destructor
	*/
	~CStub_Mainte_JetInspectionIF();

	/*!
	* \brief  ライン間同期処理用、一時マスクファイルの作成処理
	* \param ioFilePath				[IN/OUT]一時マスクファイルパス（Clientフォルダからの相対パス）
	*								指定パスが存在し、設定指示と同内容の場合は既存ファイルを流用
	*								指定パスが存在しない場合は指定名で保存
	*								指定パスが存在するが内容が異なる場合は､別名保存
	* \param inJIFlawInfoListFront	[IN]一時マスク設定情報（表面）
	* \param inJIFlawInfoListBack	[IN]一時マスク設定情報（裏面）
	* \return TRUE:Success/FALSE:Failure
	*/
	static BOOL MainteJetInspection_SaveJIFlawInfoListToTempMaskPath(std::string& ioFilePath, std::vector<STRUCT_JI_FLAW_INFO> inJIFlawInfoListFront, std::vector<STRUCT_JI_FLAW_INFO> inJIFlawInfoListBack);
	
	/*!
	* \brief  ライン間同期処理用、検査条件の作成処理
	* \param inInspectionName		[IN/OUT]検査条件名
	* \param inVerifyParamsFront	[IN]検査条件設定情報（表面）
	* \param inVerifyParamsBack		[IN]検査条件設定情報（裏面）
	* \return	 0:既存検査条件と設定内容同一のため既存ファイルを流用しました。
				 1:検査条件の追加に成功しました。
				 2:指定名で内容の異なる検査条件が存在するため､条件名を変更して保存しました。
				-1:検査条件の追加に失敗しました。
				-2:指定名で内容の異なる検査条件が存在するため､保存できません。
	*/
	static long MainteJetInspection_SaveInspectionParameter(std::string& inInspectionName, STRUCT_VERIFICATION_PARAMS& inVerifyParamsFront, STRUCT_VERIFICATION_PARAMS& inVerifyParamsBack);
	
	/*!
	* \brief  ライン間同期処理用、AIAssist対応版、検査条件の作成処理
	* \param inInspectionName		[IN/OUT]検査条件名
	* \param inVerifyParamsFront	[IN]検査条件設定情報（表面）
	* \param inVerifyParamsBack		[IN]検査条件設定情報（裏面）
	* \param inAIAssistParmsFront	[IN]AI検査条件設定情報（表面）
	* \param inAIAssistParmsBack	[IN]AI検査条件設定情報（裏面）
	* \return	 0:既存検査条件と設定内容同一のため既存ファイルを流用しました。
				 1:検査条件の追加に成功しました。
				 2:指定名で内容の異なる検査条件が存在するため､条件名を変更して保存しました。
				-1:検査条件の追加に失敗しました。
				-2:指定名で内容の異なる検査条件が存在するため､保存できません。
	*/
	static long MainteJetInspection_SaveInspectionAIAssistParameter(std::string& inInspectionName, STRUCT_VERIFICATION_PARAMS& inVerifyParamsFront, STRUCT_VERIFICATION_PARAMS& inVerifyParamsBack, STRUCT_AIASSIST_PARAMS* inAIAssistParmsFront, STRUCT_AIASSIST_PARAMS* inAIAssistParmsBack);

};
