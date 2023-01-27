/**
 * @file	PrintConditonForXML.cpp
 * @brief	XMLパーサーから取得した印刷条件1件分
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "PrintConditionForXML.h"
#include "PrintConditionsParser.h"
// ==========================================================================

using namespace printconditionsparser;

class CPrintConditionForXML::ToneCurve::OneTone {
public:
	bool m_IsExist;
	std::string m_DescriptiveName;
		
	class AnchorPoint {
	public:
		long m_BeforeConPoint;
		long m_AfterConPoint;
		long m_EditType;
		long m_PointType;

		AnchorPoint() : m_BeforeConPoint(0), m_AfterConPoint(0), m_EditType(0), m_PointType(0) {}
		AnchorPoint(
			long	inBeforeConPoint,
			long	inAfterConPoint,
			long	inEditType,
			long	inPointType)
				:	m_BeforeConPoint(inBeforeConPoint),
					m_AfterConPoint(inAfterConPoint),
					m_EditType(inEditType),
					m_PointType(inPointType) {}
	};
	std::map<CPrintConditionForXML::ToneCurve::EColor, std::vector<AnchorPoint> > m_Points;

	OneTone() : m_IsExist(false)
	{
	}
	OneTone(
		const OneTone&	inOneTone)
	{
		m_DescriptiveName = inOneTone.m_DescriptiveName;
		m_Points = inOneTone.m_Points;
		m_IsExist = inOneTone.m_IsExist;
	}
};

CPrintConditionForXML::ToneCurve::ToneCurve(
	const CPrintConditionForXML::ToneCurve&	inTone)
{
	m_Tones = inTone.m_Tones;
}

template<typename T>
T& operator ++ (T& value)
{
	value = static_cast<T>(value + 1);
	return value;
}

void CPrintConditionForXML::ToneCurve::set(
	const IPrintCondition&	inCondition)
{
	m_Tones.resize(2);
	std::shared_ptr<OneTone> front(new OneTone());
	m_Tones[0] = front;

	std::shared_ptr<OneTone> back(new OneTone());
	m_Tones[1] = back;

	const std::shared_ptr<IPrintCondition::IToneCurve>& toneInXML = inCondition.getToneCurve();
	if (toneInXML.get() == NULL) {
		return;
	}
	auto makeParam = [&](
		bool	inIsFront)
	{
		std::shared_ptr<OneTone>& onetone = m_Tones[inIsFront ? 0 : 1];
		std::pair<bool, std::string> descname = toneInXML->getDescriptiveName(inIsFront);
		if (descname.first == false) {
			return;
		}
		onetone->m_DescriptiveName = descname.second;
		IPrintCondition::IToneCurve::EColor incolor = IPrintCondition::IToneCurve::kToneCurve_Color_C;
		EColor outcolor = kToneCurve_Color_C;
		for (; incolor < IPrintCondition::IToneCurve::kToneCurve_ColorNum; ++incolor, ++outcolor) {
			std::vector<CPrintConditionForXML::ToneCurve::OneTone::AnchorPoint> anchors;
			anchors.resize(5);
			for (int point = 0; point <= 4; ++point) {
				std::pair<bool, long> beforeConPoint, afterConPoint, edittype, pointtype;
				toneInXML->getAnchorPoint(inIsFront, incolor, point, beforeConPoint, afterConPoint, edittype, pointtype);
				if ((beforeConPoint.first == false) || (afterConPoint.first == false) || (edittype.first == false) || (pointtype.first == false)) {
					return;
				}
				CPrintConditionForXML::ToneCurve::OneTone::AnchorPoint anchor(beforeConPoint.second, afterConPoint.second, edittype.second, pointtype.second);
				anchors[point] = anchor;
			}
			onetone->m_Points.insert(std::make_pair(outcolor, anchors));
		}
		onetone->m_IsExist = true;
		m_Tones[inIsFront ? 0 : 1] = onetone;
	};

	makeParam(true);
	makeParam(false);
}

bool CPrintConditionForXML::ToneCurve::isExist(
	bool	inIsFront) const
{
	return m_Tones[inIsFront ? 0 : 1]->m_IsExist;
}

const std::string& CPrintConditionForXML::ToneCurve::getDescriptiveName(
	bool	inIsFront) const
{
	return m_Tones[inIsFront ? 0 : 1]->m_DescriptiveName;
}

void CPrintConditionForXML::ToneCurve::getAnchorPoint(
	bool	inIsFront,
	EColor	inColor,
	int		inPointID,
	long&	outBeforeConPoint,
	long&	outAfterConPoint,
	long&	outAnchorPointEditType,
	long&	outAnchorPointType) const
{
	auto& tone = m_Tones[inIsFront ? 0 : 1];
	auto point = tone->m_Points.find(inColor);
	if (point == tone->m_Points.end()) {
		return;
	}
	auto ptdata = point->second[inPointID];
	outBeforeConPoint = ptdata.m_BeforeConPoint;
	outAfterConPoint = ptdata.m_AfterConPoint;
	outAnchorPointEditType = ptdata.m_EditType;
	outAnchorPointType = ptdata.m_PointType;
}

//非検査エリア左端 を取得する
bool CPrintConditionForXML::Inspection::getNonInspectionAreaLeft(bool inSide, double& outValue) const
{
	auto& it = m_nonInspectionAreaLeft.find(inSide);
	if(it == m_nonInspectionAreaLeft.end())
	{
		return false;
	}
	else
	{
		outValue = it->second;
		return true;
	}
}

//非検査エリア右端 を取得する
bool CPrintConditionForXML::Inspection::getNonInspectionAreaRight(bool inSide, double& outValue) const
{
	auto& it = m_nonInspectionAreaRight.find(inSide);
	if(it == m_nonInspectionAreaRight.end())
	{
		return false;
	}
	else
	{
		outValue = it->second;
		return true;
	}
}

//検査条件詳細情報 を取得する
bool CPrintConditionForXML::Inspection::getVerificationParams(
	bool inSide,
	std::map<std::string, std::pair<bool, std::string> >& outVerificationParams) const
{
	auto& it = m_verificationParams.find(inSide);
	if(it != m_verificationParams.end())
	{
		outVerificationParams = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

//AI検査条件詳細情報 を取得する
bool CPrintConditionForXML::Inspection::getAIAssistParams(
	bool inSide,
	std::map<std::string, std::pair<bool, std::string> >& outAIAssistParams) const
{
	auto& it = m_aiAssistParams.find(inSide);
	if(it != m_aiAssistParams.end())
	{
		outAIAssistParams = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

//一時マスク情報 を取得する
class CPrintConditionForXML::Inspection::TemporalMaskInfo {
public:
	std::string					m_type;							//!< 欠点種類（汚れ or スジ汚れ or 白スジ or 欠落）
	std::pair<double, double>	m_pos;							//!< 欠点位置
	std::pair<double, double>	m_size;							//!< 欠点サイズ
	std::pair<double, double>	m_temporalMaskSize;				//!< 一時マスクサイズ

	TemporalMaskInfo(){};
	TemporalMaskInfo(
		const TemporalMaskInfo&	inRhs)
	{
		clone(inRhs);
	}
	TemporalMaskInfo& operator=(const TemporalMaskInfo&	inRhs)
	{
		if(this != &inRhs)
		{
			clone(inRhs);
		}
		return *this;
	}

	void clone(const TemporalMaskInfo&	inRhs)
	{
		m_type				= inRhs.m_type;				
		m_pos				= inRhs.m_pos;					 
		m_size				= inRhs.m_size;				 
		m_temporalMaskSize	= inRhs.m_temporalMaskSize; 
	}

	virtual ~TemporalMaskInfo(){};
};

//一時マスク情報 を取得する
bool CPrintConditionForXML::Inspection::getTemporalMaskInfo(
	bool inSide,
	size_t inIndex,
	std::string& outType,
	std::pair<double, double>& outPos,
	std::pair<double, double>& outSize,
	std::pair<double, double>& outTemporalMaskSize) const
{
	auto& it = m_temporalMaskInfoList.find(inSide);
	if(it == m_temporalMaskInfoList.end())
	{
		return false;
	}
	if(inIndex < it->second.size())
	{
		auto indexPtr = it->second[inIndex].get();
		outType = indexPtr->m_type;
		outPos = indexPtr->m_pos;
		outSize = indexPtr->m_size;
		outTemporalMaskSize = indexPtr->m_temporalMaskSize;
		return true;
	}
	else
	{
		return false;
	}
}

//矩形情報 を取得する
class CPrintConditionForXML::Inspection::RectInfo {
public:
	std::string					m_iD;							//!< 非検査矩形エリア(1-5)
	std::pair<long, long>		m_startPos;						//!< 非検査矩形エリア始点
	std::pair<long, long>		m_endPos;						//!< 非検査矩形エリア終点

	RectInfo(){};
	RectInfo(const RectInfo&	inRectInfo)
	{
		clone(inRectInfo);
	}
	RectInfo& operator=(const RectInfo&	inRectInfo)
	{
		if(this != &inRectInfo)
		{
			clone(inRectInfo);
		}
		return *this;
	}

	void clone(const RectInfo&	inRectInfo)
	{
		m_iD		= inRectInfo.m_iD;		
		m_startPos	= inRectInfo.m_startPos;		 
		m_endPos	= inRectInfo.m_endPos;	 
	}

	virtual ~RectInfo(){};
};

//矩形情報 を取得する
bool CPrintConditionForXML::Inspection::getRectInfo(
	bool inSide,
	size_t inIndex,
	std::string& outID,
	std::pair<long, long>& outStartPos,
	std::pair<long, long>& outEndPos) const
{
	auto& it = m_rectInfoList.find(inSide);
	if(it == m_rectInfoList.end())
	{
		return false;
	}

	if(inIndex < it->second.size())
	{
		auto indexPtr = it->second[inIndex].get();
		outID = indexPtr->m_iD;
		outStartPos = indexPtr->m_startPos;
		outEndPos = indexPtr->m_endPos;
		return true;
	}
	else
	{
		return false;
	}
}

//コピーコンストラクタ
CPrintConditionForXML::Inspection::Inspection(
	const CPrintConditionForXML::Inspection&	inInspection)
{
	//このコピーコンストラクタは必要
	m_paramaterName 			= inInspection.m_paramaterName;
	m_temporalMaskFilePath		= inInspection.m_temporalMaskFilePath;
	m_verificationParams		= inInspection.m_verificationParams;
	m_aiAssistParams			= inInspection.m_aiAssistParams;
	m_temporalMaskInfoList		= inInspection.m_temporalMaskInfoList;
	m_nonInspectionAreaLeft		= inInspection.m_nonInspectionAreaLeft;
	m_nonInspectionAreaRight	= inInspection.m_nonInspectionAreaRight;
	m_rectInfoList				= inInspection.m_rectInfoList;
}

//XML から読み込んだ情報を反映する
bool CPrintConditionForXML::Inspection::set(
	const IPrintCondition&	inCondition)
{
	//"Front"/"Back"をboolで取得
	auto getSideBool = [](std::string inSideStr, bool& outSide) ->bool
	{
		if(inSideStr == "Front")
		{
			outSide = true;
			return true;
		}
		else if(inSideStr == "Back") 
		{
			outSide = false;
			return true;
		}
		else
		{
			return false;
		}
	};

	const std::shared_ptr<IPrintCondition::IInspection>& inspectionInXML = inCondition.getInspection();
	if (inspectionInXML.get() == NULL) {
		return false;	// タグなし
	}

	std::pair<bool, std::string> paramaterName = inspectionInXML->getParamaterName();
	m_paramaterName = paramaterName.second;

	//scr:VerificationParamsを取得
	auto verificationParams = inspectionInXML->getVerificationParams();
	for(auto& it = verificationParams.begin(); it != verificationParams.end(); it++)
	{
		auto side = it->get()->getSide();
		bool sideBool = true;
		if(getSideBool(side.second, sideBool) == true)
		{
			m_verificationParams[sideBool] = it->get()->getScrAttributes();
			m_aiAssistParams[sideBool] = it->get()->getScrAIAssistAttributes();
		}
	}

	std::pair<bool, std::string> temporalMaskFilePath = inspectionInXML->getTemporalMaskFilePath();
	m_temporalMaskFilePath = temporalMaskFilePath.second;	

	//scr:TemporalMaskを取得
	auto temporalMask = inspectionInXML->getTemporalMask();
	for(auto& it = temporalMask.begin(); it != temporalMask.end(); it++)
	{
		auto side = it->get()->getSide();
		bool sideBool = true;
		if(getSideBool(side.second, sideBool) == false)
		{
			continue;
		}

		auto size = it->get()->getSize();
		if(size.first == false)
		{	//そろっていない
			continue;
		}

		auto temporalMaskRecord = it->get()->getTemporalMaskRecord();
		std::vector<std::shared_ptr<TemporalMaskInfo> > temporalMaskInfo;
		for(auto& jt = temporalMaskRecord.begin(); jt != temporalMaskRecord.end(); jt++)
		{
			auto type = jt->get()->getType();
			auto pos = jt->get()->getPos();
			auto size = jt->get()->getSize();
			auto temporalMaskSize = jt->get()->getTemporalMaskSize();

			bool exists = type.first &&
				pos[0].first && pos[1].first &&
				size[0].first && size[1].first &&
				temporalMaskSize[0].first && temporalMaskSize[1].first;
			if(exists == false)
			{	//そろっていない
				continue;
			}

			std::shared_ptr<TemporalMaskInfo> info(new TemporalMaskInfo());
			info->m_type				= type.second;
			info->m_pos					= std::make_pair<double, double>(pos[0].second, pos[1].second);
			info->m_size				= std::make_pair<double, double>(size[0].second, size[1].second);
			info->m_temporalMaskSize	= std::make_pair<double, double>(temporalMaskSize[0].second, temporalMaskSize[1].second);

			temporalMaskInfo.push_back(info);
		}
		if(temporalMaskInfo.size() > 0)
		{
			m_temporalMaskInfoList[sideBool] = temporalMaskInfo;
		}
	}

	//InspectionAreaを取得
	std::vector<std::shared_ptr<IPrintCondition::IInspectionArea> > inspectionAreaList = inspectionInXML->getInspectionArea();
	for(auto& it = inspectionAreaList.begin(); it != inspectionAreaList.end(); it++)
	{
		auto side = it->get()->getSide();
		bool sideBool = true;
		if(getSideBool(side.second, sideBool) == false)
		{
			continue;
		}

		auto nonInspectionAreaLeft = it->get()->getNonInspectionAreaLeft();
		auto nonInspectionAreaRight = it->get()->getNonInspectionAreaRight();
		m_nonInspectionAreaLeft[sideBool] = nonInspectionAreaLeft.second;
		m_nonInspectionAreaRight[sideBool] = nonInspectionAreaRight.second;
		bool exists = nonInspectionAreaLeft.first && nonInspectionAreaRight.first;
		if(exists == false)
		{	//そろっていない
			continue;
		}

		auto nonInspectionRect = it->get()->getNonInspectionRect();
		std::vector<std::shared_ptr<RectInfo>> rectInfo;
		for(auto& jt = nonInspectionRect.begin(); jt != nonInspectionRect.end(); jt++)
		{
			auto id = jt->get()->getID();
			auto startPos = jt->get()->getStartPos();
			auto endPos = jt->get()->getEndPos();
			exists = id.first &&
				startPos[0].first && startPos[1].first &&
				endPos[0].first && endPos[1].first;
			if(exists == false)
			{	//そろっていない
				continue;
			}

			std::shared_ptr<RectInfo> info(new RectInfo());
			info->m_iD		= id.second;
			info->m_startPos = std::make_pair<long,long>(startPos[0].second, startPos[1].second);
			info->m_endPos	= std::make_pair<long,long>(endPos[0].second, endPos[1].second);
			rectInfo.push_back(info);
		}
		if(rectInfo.size() > 0)
		{
			m_rectInfoList[sideBool] = rectInfo;
		}
	}

	return true;	// タグあり
}

//コンストラクタ
class CPrintConditionForXML::Decode::DetailInfo {
public:
	std::string					m_side;							//!< 対象面（表 or 裏）Front / Back
	std::pair<double, double>	m_size;							//!< サイズ設定
	std::pair<double, double>	m_offset;						//!< 位置設定
	std::string					m_name;							//!< ラベル名称
	std::string					m_type;							//!< ラベル種類
	std::string					m_angle;						//!< （OCR限定）角度情報
	bool						m_errorStop;					//!< デコードエラー発生時、エラー停止判定カウントの対象にするか？

	DetailInfo()
	{
		m_errorStop = false;
	}

	DetailInfo(
		const DetailInfo&	inDetailInfo)
	{
		clone(inDetailInfo);
	}

	DetailInfo& operator=(const DetailInfo&	inDetailInfo)
	{
		if(this != &inDetailInfo)
		{
			clone(inDetailInfo);
		}
		return *this;
	}

	void clone(const DetailInfo&	inDetailInfo)
	{
		m_side		= inDetailInfo.m_side;		
		m_size		= inDetailInfo.m_size;		 
		m_offset	= inDetailInfo.m_offset;	 
		m_name		= inDetailInfo.m_name;		 
		m_type		= inDetailInfo.m_type;		 
		m_angle	    = inDetailInfo.m_angle;	
		m_errorStop = inDetailInfo.m_errorStop;
	}

	virtual ~DetailInfo(){};
};

//コピーコンストラクタ
CPrintConditionForXML::Decode::Decode(
	const CPrintConditionForXML::Decode&	inDecode)
{
	//このコピーコンストラクタは必要
	m_detailInfoList = inDecode.m_detailInfoList;
}

//XML から読み込んだ情報を反映する
bool CPrintConditionForXML::Decode::set(
	const IPrintCondition&	inCondition)
{
	const std::shared_ptr<IPrintCondition::IDecode>& decodeInXML = inCondition.getDecode();
	if (decodeInXML.get() == NULL) {
		return false;		// タグなし
	}

	std::pair<bool, std::string>			side;
	std::vector<std::pair<bool, double> >	size;
	std::vector<std::pair<bool, double> >	offset;
	std::pair<bool, std::string>			name;
	std::pair<bool, std::string>			type;
	std::pair<bool, std::string>			angle;
	std::pair<bool, bool>					errorStop;

	size_t index = 0;

	while(decodeInXML->getDetail(
							index,
							side,
							size,
							offset,
							name,
							type,
							angle,
							errorStop
		  ) == true)
	{
		std::shared_ptr<DetailInfo> detailInfo(new DetailInfo());
		detailInfo->m_side		= side.second;
		detailInfo->m_size		= std::make_pair(size[0].second, size[1].second);
		detailInfo->m_offset	= std::make_pair(offset[0].second, offset[1].second);
		detailInfo->m_name		= name.second;
		detailInfo->m_type		= type.second;
		detailInfo->m_angle		= angle.second;
		detailInfo->m_errorStop = errorStop.second;
		m_detailInfoList.push_back(detailInfo);
		index++;
	}

	return true;	// タグあり
}

//Decode詳細数を取得する
size_t CPrintConditionForXML::Decode::getDetailNum()
{
	return m_detailInfoList.size();
}

//Decode詳細 エレメントを取得する
bool CPrintConditionForXML::Decode::getDetail(
	size_t		inIndex,
	std::string&				outSide,	
	std::pair<double, double>&	outSize,	
	std::pair<double, double>&	outOffset,	
	std::string&				outName,	
	std::string&				outType,	
	std::string&				outAngle,	
	bool&						outErrorStop) const
{
	if(inIndex < m_detailInfoList.size())
	{
		outSide		 = m_detailInfoList[inIndex]->m_side;
		outSize		 = m_detailInfoList[inIndex]->m_size;
		outOffset	 = m_detailInfoList[inIndex]->m_offset;
		outName		 = m_detailInfoList[inIndex]->m_name;
		outType		 = m_detailInfoList[inIndex]->m_type;
		outAngle	 = m_detailInfoList[inIndex]->m_angle;
		outErrorStop = m_detailInfoList[inIndex]->m_errorStop;
		return true;
	}

	return false;
}


#define CVALUE(T, v, m) std::shared_ptr<CValue<T>>(new CValue<T>(v, m))

CPrintConditionForXML::CPrintConditionForXML(CDataSystemSetting& inDataSystemSetting)
	: m_dataSystemSetting(inDataSystemSetting)
{
	m_isSkipped = false;
	
	m_elements.resize(kNumOfTypes);

	//XMLから取得できない場合にエラーかどうかを定義.

	m_elements[kPrintConditionName]				= CVALUE(std::string,		ErrorIfNothing,	"PrintConditionName");
	m_elements[kPaperType]						= CVALUE(std::string,		OkIfNothing,	"PaperType");
	m_elements[kLevelTuneName]					= CVALUE(std::string,		OkIfNothing,	"Alignment");
	m_elements[kPaperWidth]						= CVALUE(double,			ErrorIfNothing,	"PaperWidth");
	m_elements[kPaperThickness]					= CVALUE(long,				ErrorIfNothing,	"PaperThickness");
	m_elements[kPaperTension]					= CVALUE(long,				ErrorIfNothing,	"PaperTension");
	m_elements[kDNS]							= CVALUE(bool,				OkIfNothing,	"DNS");
	m_elements[kPaperPunchHole]					= CVALUE(bool,				ErrorIfNothing,	"PaperPunchHole");
	m_elements[kPaperSewing]					= CVALUE(bool,				ErrorIfNothing,	"PaperSewing");
	m_elements[kPaperMarker]					= CVALUE(bool,				ErrorIfNothing,	"PaperMarker");
	m_elements[kPaperPrePrinted]				= CVALUE(bool,				ErrorIfNothing,	"PaperPrePrinted");
	m_elements[kPaperMarkerOffset]				= CVALUE(double,			OkIfNothing,	"PaperMarkerOffset");
	m_elements[kPaperPageFeedSize]				= CVALUE(double,			ErrorIfNothing,	"PaperPageFeedSize");
	m_elements[kPaperPageFeedSizeUnit]			= CVALUE(EPageFeedUnit,		ErrorIfNothing,	"PaperPageFeedSizeUnit");
	m_elements[kPaperMarkSide]					= CVALUE(EMarkSide,			OkIfNothing,	"PaperMarkSide");
	//m_elements[kPaperRepeatSheets]				= CVALUE(long,				OkIfNothing,	"PaperRepeatSheets");
	m_elements[kPrintMode]						= CVALUE(std::string,		ErrorIfNothing,	"PrintMode");
	m_elements[kPrintSpeed]						= CVALUE(double,			OkIfNothing,	"PrintSpeed");
	m_elements[kHeatRollerSelf]					= CVALUE(long,				OkIfNothing,	"HeatRollerSelf");
	m_elements[kHeatRollerPart]					= CVALUE(long,				OkIfNothing,	"HeatRollerPart");
	m_elements[kHeatFanSelf]					= CVALUE(long,				OkIfNothing,	"HeatFanSelf");
	m_elements[kHeatFanPart]					= CVALUE(long,				OkIfNothing,	"HeatFanPart");
	m_elements[kNIRPowerSelf]					= CVALUE(long,				OkIfNothing,	"NIRPowerSelf");
	m_elements[kNIRPowerPart]					= CVALUE(long,				OkIfNothing,	"NIRPowerPart");
	m_elements[kPrecoater1]						= CVALUE(long,				OkIfNothing,	"Precoater1");
	m_elements[kPrecoater2]						= CVALUE(long,				OkIfNothing,	"Precoater2");
	m_elements[kSubHeatRollerSelf]				= CVALUE(long,				OkIfNothing,	"SubHeatRollerSelf");
	m_elements[kSubHeatRollerPart]				= CVALUE(long,				OkIfNothing,	"SubHeatRollerPart");
	m_elements[kICCPresetName]					= CVALUE(std::string,		OkIfNothing,	"ICCPresetName");
	m_elements[kICCPresetID]					= CVALUE(std::string,		OkIfNothing,	"ICCPresetID");
	m_elements[kToneCurve]						= CVALUE(ToneCurve,			OkIfNothing,	"ToneCurve");
	m_elements[kJobBaseInformationSheet]		= CVALUE(bool,				OkIfNothing,	"JobBaseInformationSheet");
	m_elements[kJobBaseFlushingPage]			= CVALUE(bool,				OkIfNothing,	"JobBaseFlushingPage");
	m_elements[kJobBasePatternName]				= CVALUE(std::string,		OkIfNothing,	"JobBasePatternName");
	m_elements[kJobBaseInterval]				= CVALUE(long,				OkIfNothing,	"JobBaseInterval");
	m_elements[kJobBasePreJob]					= CVALUE(bool,				OkIfNothing,	"JobBasePreJob");
	m_elements[kJobBasePreJobCount]				= CVALUE(long,				OkIfNothing,	"JobBasePreJobCount");
	m_elements[kJobBasePostJob]					= CVALUE(bool,				OkIfNothing,	"JobBasePostJob");
	m_elements[kJobBaseOffsetFront]				= CVALUE(long,				OkIfNothing,	"JobBaseOffsetFront");
	m_elements[kJobBaseOffsetBack]				= CVALUE(long,				OkIfNothing,	"JobBaseOffsetBack");
	m_elements[kJobBaseCleaningJudgmentLevelBeforeJob] = CVALUE(long,		OkIfNothing,	"JobBaseCleaningJudgmentLevelBeforeJob");
	m_elements[kJobBaseCleaningJudgmentLevelAfterJob] = CVALUE(long,		OkIfNothing,	"JobBaseCleaningJudgmentLevelAfterJob");
	m_elements[kJobBasePrintStopSetting]		= CVALUE(std::string,		OkIfNothing,	"JobBasePrintStopSetting");
	m_elements[kLayoutOffsetFront]				= CVALUE(xypair_t,			OkIfNothing,	"LayoutOffsetFront");
	m_elements[kLayoutOffsetBack]				= CVALUE(xypair_t,			OkIfNothing,	"LayoutOffsetBack");
	m_elements[kLayoutVerification]				= CVALUE(bool,				OkIfNothing,	"LayoutVerification");
	m_elements[kLayoutContinuousPageCount]		= CVALUE(long,				OkIfNothing,	"LayoutContinuousPageCount");
	m_elements[kLayoutCueMark]					= CVALUE(bool,				OkIfNothing,	"LayoutCueMark");
	m_elements[kLayoutBarcode]					= CVALUE(bool,				OkIfNothing,	"LayoutBarcode");
	m_elements[kLayoutBarcodeOffsetFront]		= CVALUE(xypair_t,			OkIfNothing,	"LayoutBarcodeOffsetFront");
	m_elements[kLayoutBarcodeOffsetBack]		= CVALUE(xypair_t,			OkIfNothing,	"LayoutBarcodeOffsetBack");
	m_elements[kLayoutFlushingPattern]			= CVALUE(std::string,		OkIfNothing,	"LayoutFlushingPattern");
	m_elements[kLayoutGapBetweenColors]			= CVALUE(long,				OkIfNothing,	"LayoutGapBetweenColors");
	m_elements[kLayoutLineOffsetFront]			= CVALUE(long,				OkIfNothing,	"LayoutLineOffsetFront");
	m_elements[kLayoutLineOffsetBack]			= CVALUE(long,				OkIfNothing,	"LayoutLineOffsetBack");
	m_elements[kLayoutLineGap]					= CVALUE(bool,				OkIfNothing,	"LayoutLineGap");
	m_elements[kLayoutLineGapFront]				= CVALUE(long,				OkIfNothing,	"LayoutLineGapFront");
	m_elements[kLayoutLineGapBack]				= CVALUE(long,				OkIfNothing,	"LayoutLineGapBack");
	m_elements[kLayoutLineGapOffsetFront]		= CVALUE(long,				OkIfNothing,	"LayoutLineGapOffsetFront");
	m_elements[kLayoutLineGapOffsetBack]		= CVALUE(long,				OkIfNothing,	"LayoutLineGapOffsetBack");
	m_elements[kLayoutLinePosotion]				= CVALUE(std::string,		OkIfNothing,	"LayoutLinePosotion");
	m_elements[kLayoutLineOptions]				= CVALUE(std::string,		OkIfNothing,	"LayoutLineOptions");
	m_elements[kLayoutCutterMark]				= CVALUE(ECutterMarkSide,	OkIfNothing,	"LayoutCutterMark");
	m_elements[kLayoutCutterMarkPageCount]		= CVALUE(long,				OkIfNothing,	"LayoutCutterMarkPageCount");
	m_elements[kLayoutCutterMarkSize]			= CVALUE(xypair_t,			OkIfNothing,	 "LayoutCutterMarkSize");
	m_elements[kLayoutCutterMarkOffsetFront]	= CVALUE(long,				OkIfNothing,	"LayoutCutterMarkOffsetFront");
	m_elements[kLayoutCutterMarkOffsetBack]		= CVALUE(long,				OkIfNothing,	"LayoutCutterMarkOffsetBack");
	m_elements[kLayoutCutterMarkAlignFront]		= CVALUE(ECutterMarkAlign,	OkIfNothing,	"LayoutCutterMarkAlignFront");
	m_elements[kLayoutCutterMarkAlignBack]		= CVALUE(ECutterMarkAlign,	OkIfNothing,	"LayoutCutterMarkAlignBack");

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	m_elements[kLayoutAdjustmentSheetSizeEnabled] = CVALUE(bool,			OkIfNothing,	"LayoutAdjustmentSheetSizeEnabled");
	m_elements[kLayoutAdjustmentSheetLength]	= CVALUE(long,				OkIfNothing,	"LayoutAdjustmentSheetLength");
	m_elements[kLayoutAdjustmentSheetAfterJob]	= CVALUE(long,				OkIfNothing,	"LayoutAdjustmentSheetAfterJob");
	m_elements[kLayoutAfterJobSheets]			= CVALUE(long,				OkIfNothing,	"LayoutAfterJobSheets");
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

	m_elements[kInspection]						= CVALUE(Inspection,		OkIfNothing,	"Inspection");

	m_elements[kInspectionJICondition]			= CVALUE(std::string,		OkIfNothing,	"JICondition");
	m_elements[kInspectionTemporalMaskFile]		= CVALUE(std::string,		OkIfNothing,	"TemporalMaskFile");
	m_elements[kDecode]							= CVALUE(Decode,			OkIfNothing,	"Decode");
	m_elements[kDecodeErrorSheets]				= CVALUE(long,				OkIfNothing,	"ErrorSheets");
	m_elements[kDecodeUnit]						= CVALUE(std::string,		OkIfNothing,	"Unit");
	m_elements[kDecodeIsEnabled]				= CVALUE(bool,				OkIfNothing,	"IsEnabled");

	m_elements[kScalingValue]					= CVALUE(bool,				OkIfNothing,	"ScalingValue");
	m_elements[kScalingBaseSide]				= CVALUE(std::string,		OkIfNothing,	"BaseSide");
	m_elements[kScalingValueFrontW]				= CVALUE(double,			OkIfNothing,	"ScalingValueFrontW");
	m_elements[kScalingValueFrontH]				= CVALUE(double,			OkIfNothing,	"ScalingValueFrontH");
	m_elements[kScalingValueBackW]				= CVALUE(double,			OkIfNothing,	"ScalingValueBackW");
	m_elements[kScalingValueBackH]				= CVALUE(double,			OkIfNothing,	"ScalingValueBackH");
}

CPrintConditionForXML::~CPrintConditionForXML()
{
}

template <typename T1>
static void set(
	const std::pair<bool, T1>&	inXML,
	CErrorLevel*				ioValue)
{
	CValue<T1>* v = dynamic_cast<CValue<T1>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if (inXML.first == false) {
		return;
	}
	v->Set(inXML.second);
}

static void set(
	const IPrintCondition&	inCond,
	CErrorLevel*			ioValue)
{
	CValue<CPrintConditionForXML::ToneCurve>* v = dynamic_cast<CValue<CPrintConditionForXML::ToneCurve>*>(ioValue);
	if (v != nullptr) {
		CPrintConditionForXML::ToneCurve tone;
		tone.set(inCond);
		v->Set(tone);
	}
	else
	{
		CValue<CPrintConditionForXML::Inspection>* v = dynamic_cast<CValue<CPrintConditionForXML::Inspection>*>(ioValue);
		if (v != nullptr) {
			CPrintConditionForXML::Inspection inspection;
			if(inspection.set(inCond) == true)
			{
				v->Set(inspection);
			}
		}
		else
		{
			CValue<CPrintConditionForXML::Decode>* v = dynamic_cast<CValue<CPrintConditionForXML::Decode>*>(ioValue);
			if (v != nullptr) {
				CPrintConditionForXML::Decode decode;
				if(decode.set(inCond) == true)
				{
					v->Set(decode);
				}
			}
		}
	}
}

static void set(
	const std::pair<bool, IPrintCondition::IPaperSetting::EPageFeedUnit>&	inXML,
	CErrorLevel*															ioValue)
{
	static std::map<IPrintCondition::IPaperSetting::EPageFeedUnit, CPrintConditionForXML::EPageFeedUnit> sMap;
	if (sMap.empty()) {
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_mm, CPrintConditionForXML::kPageFeedUnit_mm));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_inch, CPrintConditionForXML::kPageFeedUnit_inch));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_inch2, CPrintConditionForXML::kPageFeedUnit_inch2));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_inch6, CPrintConditionForXML::kPageFeedUnit_inch6));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_Page, CPrintConditionForXML::kPageFeedUnit_Page));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kPageFeedUnit_Empty, CPrintConditionForXML::kPageFeedUnit_Empty));
	}
	CValue<CPrintConditionForXML::EPageFeedUnit>* v = dynamic_cast<CValue<CPrintConditionForXML::EPageFeedUnit>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if (inXML.first == false) {
		return;
	}
	v->Set(sMap.find(inXML.second)->second);
}

static void set(
	const std::pair<bool, IPrintCondition::IPaperSetting::EMarkSide>&	inXML,
	CErrorLevel*														ioValue)
{
	static std::map<IPrintCondition::IPaperSetting::EMarkSide, CPrintConditionForXML::EMarkSide> sMap;
	if (sMap.empty()) {
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kMarkSide_Front, CPrintConditionForXML::kMarkSide_Front));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kMarkSide_Back, CPrintConditionForXML::kMarkSide_Back));
		sMap.insert(std::make_pair(IPrintCondition::IPaperSetting::kMarkSide_Empty, CPrintConditionForXML::kMarkSide_Empty));
	}
	CValue<CPrintConditionForXML::EMarkSide>* v = dynamic_cast<CValue<CPrintConditionForXML::EMarkSide>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if (inXML.first == false) {
		return;
	}
	v->Set(sMap.find(inXML.second)->second);
}

static void set(
	const std::pair<bool, IPrintCondition::ILayout::ECutterMarkSide>&	inXML,
	CErrorLevel*														ioValue)
{
	static std::map<IPrintCondition::ILayout::ECutterMarkSide, CPrintConditionForXML::ECutterMarkSide> sMap;
	if (sMap.empty()) {
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkSide_Off, CPrintConditionForXML::kCutterMarkSide_Off));
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkSide_Front, CPrintConditionForXML::kCutterMarkSide_Front));
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkSide_Back, CPrintConditionForXML::kCutterMarkSide_Back));
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkSide_Both, CPrintConditionForXML::kCutterMarkSide_Both));
	}

	CValue<CPrintConditionForXML::ECutterMarkSide>* v = dynamic_cast<CValue<CPrintConditionForXML::ECutterMarkSide>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if (inXML.first == false) {
		return;
	}
	v->Set(sMap.find(inXML.second)->second);
}

static void set(
	const std::pair<bool, IPrintCondition::ILayout::ECutterMarkAlign>&	inXML,
	CErrorLevel*														ioValue)
{
	static std::map<IPrintCondition::ILayout::ECutterMarkAlign, CPrintConditionForXML::ECutterMarkAlign> sMap;
	if (sMap.empty()) {
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkAlign_Left, CPrintConditionForXML::kCutterMarkAlign_Left));
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkAlign_Center, CPrintConditionForXML::kCutterMarkAlign_Center));
		sMap.insert(std::make_pair(IPrintCondition::ILayout::kCutterMarkAlign_Right, CPrintConditionForXML::kCutterMarkAlign_Right));
	}
	CValue<CPrintConditionForXML::ECutterMarkAlign>* v = dynamic_cast<CValue<CPrintConditionForXML::ECutterMarkAlign>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if (inXML.first == false) {
		return;
	}
	v->Set(sMap.find(inXML.second)->second);
}

static void set(
	const std::pair<bool, long>&	inX,
	const std::pair<bool, long>&	inY,
	CErrorLevel*					ioValue)
{
	CValue<CPrintConditionForXML::xypair_t>* v = dynamic_cast<CValue<CPrintConditionForXML::xypair_t>*>(ioValue);
	if (v == NULL) {
		return;
	}
	if ((inX.first == false) || (inY.first == false)) {
		return;
	}
	v->Set(std::make_pair(inX.second, inY.second));
}

void CPrintConditionForXML::ReadFromXML(
	const IPrintCondition& inCondition)
{
	set(inCondition.getDescriptiveName(), m_elements[kPrintConditionName].get());

	// XML解析時に同一記述でスキップ可能と判断済みの場合、名前以外の情報設定をスキップする
	m_isSkipped = inCondition.isImportSkipFlag();
	if( m_isSkipped )	return;
	
	set(inCondition.getPaperType(), m_elements[kPaperType].get());
	set(inCondition.getAlignment(), m_elements[kLevelTuneName].get());
	if (inCondition.getPaperSetting().get() == NULL) {
		return;
	}
	set(inCondition.getPaperSetting()->getWidth(), m_elements[kPaperWidth].get());
	set(inCondition.getPaperSetting()->getThickness(), m_elements[kPaperThickness].get());
	set(inCondition.getPaperSetting()->getTension(), m_elements[kPaperTension].get());
	set(inCondition.getPaperSetting()->getDNS(), m_elements[kDNS].get());
	set(inCondition.getPaperSetting()->hasPunchHole(), m_elements[kPaperPunchHole].get());
	set(inCondition.getPaperSetting()->hasSewing(), m_elements[kPaperSewing].get());
	set(inCondition.getPaperSetting()->hasMarker(), m_elements[kPaperMarker].get());
	set(inCondition.getPaperSetting()->isPrePrinted(), m_elements[kPaperPrePrinted].get());
	set(inCondition.getPaperSetting()->getMarkerOffset(), m_elements[kPaperMarkerOffset].get());
	set(inCondition.getPaperSetting()->getPageFeedSize(), m_elements[kPaperPageFeedSize].get());
	set(inCondition.getPaperSetting()->getPageFeedUnit(), m_elements[kPaperPageFeedSizeUnit].get());
	set(inCondition.getPaperSetting()->getMarkSide(), m_elements[kPaperMarkSide].get());
	//set(inCondition.getPaperSetting()->getRepeatSheets(), m_elements[kPaperRepeatSheets].get());
	set(inCondition.getPrintMode(), m_elements[kPrintMode].get());
	set(inCondition.getPrintSpeed(), m_elements[kPrintSpeed].get());
	if (inCondition.getDryTemperature().get() == NULL) {
		return;
	}
	set(inCondition.getDryTemperature()->getHeatRoller(true), m_elements[kHeatRollerSelf].get());
	set(inCondition.getDryTemperature()->getHeatRoller(false), m_elements[kHeatRollerPart].get());
	set(inCondition.getDryTemperature()->getHeatFan(true), m_elements[kHeatFanSelf].get());
	set(inCondition.getDryTemperature()->getHeatFan(false), m_elements[kHeatFanPart].get());
	set(inCondition.getDryTemperature()->getNIRPower(true), m_elements[kNIRPowerSelf].get());
	set(inCondition.getDryTemperature()->getNIRPower(false), m_elements[kNIRPowerPart].get());
	set(inCondition.getDryTemperature()->getPrecoater(1), m_elements[kPrecoater1].get());
	set(inCondition.getDryTemperature()->getPrecoater(2), m_elements[kPrecoater2].get());
	set(inCondition.getDryTemperature()->getSubHeatRoller(true), m_elements[kSubHeatRollerSelf].get());
	set(inCondition.getDryTemperature()->getSubHeatRoller(false), m_elements[kSubHeatRollerPart].get());
	if (inCondition.getICCPreset().get() == NULL) {
		return;
	}
	set(inCondition.getICCPreset()->getDescriptiveName(), m_elements[kICCPresetName].get());
	set(inCondition.getICCPreset()->getID(), m_elements[kICCPresetID].get());
	set(inCondition, m_elements[kToneCurve].get());
	if (inCondition.getJobBase().get() == NULL) {
		return;
	}
	set(inCondition.getJobBase()->hasInformationSheet(), m_elements[kJobBaseInformationSheet].get());
	set(inCondition.getJobBase()->hasFlushingPage(), m_elements[kJobBaseFlushingPage].get());
	set(inCondition.getJobBase()->getPatternName(), m_elements[kJobBasePatternName].get());
	set(inCondition.getJobBase()->getInterval(), m_elements[kJobBaseInterval].get());
	set(inCondition.getJobBase()->isPreJob(), m_elements[kJobBasePreJob].get());
	set(inCondition.getJobBase()->getPreJobCount(), m_elements[kJobBasePreJobCount].get());
	set(inCondition.getJobBase()->isPostJob(), m_elements[kJobBasePostJob].get());
	set(inCondition.getJobBase()->getOffset(true), m_elements[kJobBaseOffsetFront].get());
	set(inCondition.getJobBase()->getOffset(false), m_elements[kJobBaseOffsetBack].get());
	// フラッシングページ=ありで、
	// ノズル抜けチェック機能が有効 且つ ノズル抜けチェックが可能なフラッシングページが設定されている場合
	if (inCondition.getJobBase()->hasFlushingPage().second &&
		m_dataSystemSetting.isNozzleCheckEnabled(inCondition.getJobBase()->getPatternName().second))
	{
		set(
			inCondition.getJobBase()->getCleaningJudgmentLevelBeforeJob(),
			m_elements[kJobBaseCleaningJudgmentLevelBeforeJob].get());
		set(
			inCondition.getJobBase()->getCleaningJudgmentLevelAfterJob(),
			m_elements[kJobBaseCleaningJudgmentLevelAfterJob].get());
		set(
			inCondition.getJobBase()->getPrintStopSetting(),
			m_elements[kJobBasePrintStopSetting].get());
	}
	if (inCondition.getLayout().get() == NULL) {
		return;
	}
	{
		std::pair<bool, long> x, y;
		inCondition.getLayout()->getOffset(true, x, y);
		set(x, y, m_elements[kLayoutOffsetFront].get());
	}
	{
		std::pair<bool, long> x, y;
		inCondition.getLayout()->getOffset(false, x, y);
		set(x, y, m_elements[kLayoutOffsetBack].get());
	}
	set(inCondition.getLayout()->isVerification(), m_elements[kLayoutVerification].get());
	set(inCondition.getLayout()->getContinuousPageCount(), m_elements[kLayoutContinuousPageCount].get());
	set(inCondition.getLayout()->hasCueMark(), m_elements[kLayoutCueMark].get());
	set(inCondition.getLayout()->isBarcode(), m_elements[kLayoutBarcode].get());
	{
		std::pair<bool, long> x, y;
		inCondition.getLayout()->getBarcodeOffset(true, x, y);
		set(x, y, m_elements[kLayoutBarcodeOffsetFront].get());
	}
	{
		std::pair<bool, long> x, y;
		inCondition.getLayout()->getBarcodeOffset(false, x, y);
		set(x, y, m_elements[kLayoutBarcodeOffsetBack].get());
	}
	set(inCondition.getLayout()->getFlushingPattern(), m_elements[kLayoutFlushingPattern].get());
	set(inCondition.getLayout()->getGapBetweenColors(), m_elements[kLayoutGapBetweenColors].get());
	set(inCondition.getLayout()->getLineOffset(true), m_elements[kLayoutLineOffsetFront].get());
	set(inCondition.getLayout()->getLineOffset(false), m_elements[kLayoutLineOffsetBack].get());
	set(inCondition.getLayout()->getLayoutLineGap(), m_elements[kLayoutLineGap].get());
	set(inCondition.getLayout()->getLayoutLineGap(true), m_elements[kLayoutLineGapFront].get());
	set(inCondition.getLayout()->getLayoutLineGap(false), m_elements[kLayoutLineGapBack].get());
	set(inCondition.getLayout()->getLayoutLineGapOffset(true), m_elements[kLayoutLineGapOffsetFront].get());
	set(inCondition.getLayout()->getLayoutLineGapOffset(false), m_elements[kLayoutLineGapOffsetBack].get());
	set(inCondition.getLayout()->getLayoutLinePosotion(), m_elements[kLayoutLinePosotion].get());
	set(inCondition.getLayout()->getLayoutLineOptions(), m_elements[kLayoutLineOptions].get());
	set(inCondition.getLayout()->getCutterMark(), m_elements[kLayoutCutterMark].get());
	set(inCondition.getLayout()->getCutterMarkPageCount(), m_elements[kLayoutCutterMarkPageCount].get());
	{
		std::pair<bool, long> x, y;
		inCondition.getLayout()->getCutterMarkSize(x, y);
		set(x, y, m_elements[kLayoutCutterMarkSize].get());
	}
	set(inCondition.getLayout()->getCutterMarkOffset(true), m_elements[kLayoutCutterMarkOffsetFront].get());
	set(inCondition.getLayout()->getCutterMarkOffset(false), m_elements[kLayoutCutterMarkOffsetBack].get());
	set(inCondition.getLayout()->getCutterMarkAlign(true), m_elements[kLayoutCutterMarkAlignFront].get());
	set(inCondition.getLayout()->getCutterMarkAlign(false), m_elements[kLayoutCutterMarkAlignBack].get());

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	set(inCondition.getLayout()->getAdjustmentSheetSizeEnabled(), m_elements[kLayoutAdjustmentSheetSizeEnabled].get());
	set(inCondition.getLayout()->getAdjustmentSheetLength(), m_elements[kLayoutAdjustmentSheetLength].get());
	set(inCondition.getLayout()->getAdjustmentSheetAfterJob(), m_elements[kLayoutAdjustmentSheetAfterJob].get());
	set(inCondition.getLayout()->getAfterJobSheets(), m_elements[kLayoutAfterJobSheets].get());
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

	set(inCondition, m_elements[kInspection].get());
	if (inCondition.getInspection().get() != nullptr) {
		set(inCondition.getInspection()->getParamaterName(), m_elements[kInspectionJICondition].get());
		set(inCondition.getInspection()->getTemporalMaskFilePath(), m_elements[kInspectionTemporalMaskFile].get());
	}

	set(inCondition, m_elements[kDecode].get());
	if (inCondition.getDecode().get() != nullptr) {
		set(inCondition.getDecode()->getErrorSheets(), m_elements[kDecodeErrorSheets].get());
		set(inCondition.getDecode()->getUnit(), m_elements[kDecodeUnit].get());
		set(inCondition.getDecode()->getIsEnabled(), m_elements[kDecodeIsEnabled].get());
	}

	if (inCondition.getScaling().get() != nullptr)
	{
		set(inCondition.getScaling()->getScalingValue(), m_elements[kScalingValue].get());
		set(inCondition.getScaling()->getBaseSide(), m_elements[kScalingBaseSide].get());
		set(inCondition.getScaling()->getScalingValueFrontW(), m_elements[kScalingValueFrontW].get());
		set(inCondition.getScaling()->getScalingValueFrontH(), m_elements[kScalingValueFrontH].get());
		set(inCondition.getScaling()->getScalingValueBackW(), m_elements[kScalingValueBackW].get());
		set(inCondition.getScaling()->getScalingValueBackH(), m_elements[kScalingValueBackH].get());
	}
}

void CPrintConditionForXML::JudgeRequiredItem()
{
	for(auto it = m_elements.begin(); it != m_elements.end(); it++)
	{
		it->get()->Error();
	}
}

const CValue<std::string>* CPrintConditionForXML::GetString(
	Type	inType) const
{
	return dynamic_cast<CValue<std::string>*>(m_elements[inType].get());
}

const CValue<double>* CPrintConditionForXML::GetDouble(
	Type	inType) const
{
	return dynamic_cast<CValue<double>*>(m_elements[inType].get());
}

const CValue<long>* CPrintConditionForXML::GetLong(
	Type	inType) const
{
	return dynamic_cast<CValue<long>*>(m_elements[inType].get());
}

const CValue<bool>* CPrintConditionForXML::GetBool(
	Type	inType) const
{
	return dynamic_cast<CValue<bool>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::ToneCurve>* CPrintConditionForXML::GetTone(
	Type	inType /* = kToneCurve*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::ToneCurve>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::EPageFeedUnit>* CPrintConditionForXML::GetPageFeedUnit(
	Type	inType /*= kPaperPageFeedSizeUnit*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::EPageFeedUnit>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::EMarkSide>* CPrintConditionForXML::GetMarkSide(
	Type	inType /*= kPaperMarkSide*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::EMarkSide>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::ECutterMarkSide>* CPrintConditionForXML::GetCutterMarkSide(
	Type	inType /*= kLayoutCutterMark*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::ECutterMarkSide>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::ECutterMarkAlign>* CPrintConditionForXML::GetCutterMarkAlign(
	Type	inType) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::ECutterMarkAlign>*>(m_elements[inType].get());
}

const CValue<CPrintConditionForXML::xypair_t>* CPrintConditionForXML::GetXYPair(
	Type	inType) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::xypair_t>*>(m_elements[inType].get());
}

//指定エレメントの値を取得する
const CValue<CPrintConditionForXML::Inspection>* CPrintConditionForXML::GetInspection(
	Type	inType /* = kInspectionArea*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::Inspection>*>(m_elements[inType].get());
}

//指定エレメントの値を取得する
const CValue<CPrintConditionForXML::Decode>* CPrintConditionForXML::GetDecode(
	Type	inType /* = kDecode*/) const
{
	return dynamic_cast<CValue<CPrintConditionForXML::Decode>*>(m_elements[inType].get());
}
