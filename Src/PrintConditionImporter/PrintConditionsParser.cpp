/**
 * @file	PrintConditionsParser.cpp
 * @brief	PrintConditionsParser
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

#include "stdafx.h"

#include <sstream>
#include <fstream>

#include "PrintConditionsParser.h"

#include "XMLElement.h"
#include "XMLParser.h"


namespace printconditionsparser {

//#define ThrowIfNULL(x) if ((x) == NULL) { throw 1; }
#define ThrowIfNULL(x, m) if ((x) == NULL) { std::invalid_argument e(m); throw e; }
#define ThrowIfEnd(x, i, m) if ((x) == (i).end()) { std::invalid_argument e(m); throw e; }
#define ThrowIfNot(x, m) if ((x)) { std::invalid_argument e(m); throw e; }

template <typename T>
static std::pair<bool, T> str2num(
	const std::string&	inStr)
{
	T val = static_cast<T>(0);
	if (inStr.empty()) {
		return std::make_pair(false, val);
	}
	std::stringstream ss;
	ss << inStr;
	ss >> val;
	return std::make_pair(true, val);
}

static std::pair<bool, bool> str2num(
	const std::string&	inStr)
{
	static std::map<std::string, bool> s_Map;
	if (s_Map.empty()) {
		s_Map.insert(std::pair<std::string, bool>("true", true));
		s_Map.insert(std::pair<std::string, bool>("false", false));
	}
	try {
		std::stringstream ss;
		ss << "invalid contents detected for boolean attribute:" << inStr;
		auto i = s_Map.find(inStr);
		ThrowIfEnd(i, s_Map, ss.str())
	
		return std::make_pair(true, i->second);
	} catch (...) {
	}
	return std::make_pair(false, false);
}

static std::pair<bool, std::string> str2str(
	const std::string&	inStr)
{
	return std::make_pair(!inStr.empty(), inStr);
}

//str2xypair()のtemplate版
template <typename T>
static void str2xypair_t(
	const std::string&	inStr,
	std::vector<std::pair<bool, T>>& outPair)
{
	std::stringstream ss;
	ss << inStr;

	std::vector<T> vals;
	std::string s;
	while (std::getline(ss, s, ' ') != NULL) {
		vals.push_back(str2num<T>(s).second);
	}
	try {
		if (vals.size() != 2) {
			std::stringstream ss;
			ss << "cannot split to xypair:" << inStr;
			std::invalid_argument e(ss.str());
			throw e;
		}
		outPair.push_back(std::make_pair(true, vals[0]));
		outPair.push_back(std::make_pair(true, vals[1]));
	} catch (...) {
		outPair.push_back(std::make_pair(false, 0));
		outPair.push_back(std::make_pair(false, 0));
	}
}

class PaperSetting : public IPrintCondition::IPaperSetting {
public:
	PaperSetting(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement) : m_Logger(inLogger)
	{
		initialize();

		m_Width = str2num<double>(inElement.getAttribute("Width"));
		m_Thickness = str2num<long>(inElement.getAttribute("Thickness"));
		m_Tension = str2num<long>(inElement.getAttribute("Tension"));
		m_DNS = str2num(inElement.getAttribute("DNS"));
		m_IsPunchHole = str2num(inElement.getAttribute("PunchHole"));
		m_IsSewing = str2num(inElement.getAttribute("Sewing"));
		m_IsMarker = str2num(inElement.getAttribute("Marker"));
		m_IsPrePrinted = str2num(inElement.getAttribute("PrePrinted"));
		m_MarkerOffset = str2num<double>(inElement.getAttribute("MarkerOffset"));
		m_PageFeedSize = str2num<double>(inElement.getAttribute("PageFeedSize"));

		auto str2Unit = [&](const std::string&	inStr) -> std::pair<bool, IPrintCondition::IPaperSetting::EPageFeedUnit> {
			static std::map<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit> m_Map;
			if (m_Map.empty()) {
				m_Map.insert(std::pair<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit>("mm", IPrintCondition::IPaperSetting::kPageFeedUnit_mm));
				m_Map.insert(std::pair<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit>("inch", IPrintCondition::IPaperSetting::kPageFeedUnit_inch));
				m_Map.insert(std::pair<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit>("1/2inch", IPrintCondition::IPaperSetting::kPageFeedUnit_inch2));
				m_Map.insert(std::pair<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit>("1/6inch", IPrintCondition::IPaperSetting::kPageFeedUnit_inch6));
				m_Map.insert(std::pair<std::string, IPrintCondition::IPaperSetting::EPageFeedUnit>("Page", IPrintCondition::IPaperSetting::kPageFeedUnit_Page));
			}
			auto unit = m_Map.find(inStr);
			if (unit == m_Map.end()) {
				std::stringstream ss;
				ss << "PageFeedSizeUnit has unsupported contents" << inStr << ", use empty";
				m_Logger.writeToDebugLog(ss.str());
				return std::make_pair(false, IPrintCondition::IPaperSetting::kPageFeedUnit_Empty);
			}
			return std::make_pair(true, unit->second);
		};

		m_PageFeedUnit = str2Unit(inElement.getAttribute("PageFeedSizeUnit"));

		auto str2Side = [](const std::string&	inStr) -> std::pair<bool, IPrintCondition::IPaperSetting::EMarkSide> {
			if (inStr.compare("Front") == 0) {
				return std::make_pair(true, IPrintCondition::IPaperSetting::kMarkSide_Front);
			}
			if (inStr.compare("Back") == 0) {
				return std::make_pair(true, IPrintCondition::IPaperSetting::kMarkSide_Back);
			}
			return std::make_pair(false, IPrintCondition::IPaperSetting::kMarkSide_Empty);
		};

		m_MarkSide = str2Side(inElement.getAttribute("MarkSide"));

		m_RepeatSheets = str2num<long>(inElement.getAttribute("RepeatSheets"));
	}

	virtual ~PaperSetting() {}

	/**
		用紙幅を取得する
		@return 用紙幅 (pt)
	*/
	virtual std::pair<bool, double> getWidth() const { return m_Width; }

	/**
		用紙厚みを取得する
		@return 用紙厚み (μm)
	*/
	virtual std::pair<bool, long> getThickness() const { return m_Thickness; }
			
	/**
		テンションを取得する
		@return テンション
	*/
	virtual std::pair<bool, long> getTension() const { return m_Tension; }

	/**
		DNSを取得する
		@return DNS
	*/
	virtual std::pair<bool, bool> getDNS() const { return m_DNS; }

	/**
		パンチ穴有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasPunchHole() const { return m_IsPunchHole; }
			
	/**
		ミシン目有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasSewing() const { return m_IsSewing; }

	/**
		事前マーク有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasMarker() const { return m_IsMarker; }

	/**
		事前印刷有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> isPrePrinted() const { return m_IsPrePrinted; }

	/**
		マークオフセットを取得する
		@return マークオフセット (pt)
	*/
	virtual std::pair<bool, double> getMarkerOffset() const { return m_MarkerOffset; }

	/**
		ページ送りサイズを取得する
		@return ページ送りサイズ (pt)
	*/
	virtual std::pair<bool, double> getPageFeedSize() const { return m_PageFeedSize; }

	/**
		ページ送りサイズの単位を取得する
		@return ページ送りサイズ単位
	*/
	virtual std::pair<bool, EPageFeedUnit> getPageFeedUnit() const { return m_PageFeedUnit; }

	/**
		マーク印刷面を取得する
		@return マーク印刷面
	*/
	virtual std::pair<bool, EMarkSide> getMarkSide() const { return m_MarkSide; }

	/**
		ミシン目パターンの繰り返し数を取得する
		@return 繰り返し数
	*/
	virtual std::pair<bool, long> getRepeatSheets() const { return m_RepeatSheets; }

private:
	void initialize()
	{
		m_Width = std::make_pair(false, 0.0);
		m_Thickness = std::make_pair(false, 0);
		m_Tension = std::make_pair(false, 0);
		m_IsPunchHole = std::make_pair(false, false);
		m_IsSewing = std::make_pair(false, false);
		m_IsMarker = std::make_pair(false, false);
		m_IsPrePrinted = std::make_pair(false, false);
		m_MarkerOffset = std::make_pair(false, 0.0);
		m_PageFeedSize = std::make_pair(false, 0.0);
		m_PageFeedUnit = std::make_pair(false, IPaperSetting::kPageFeedUnit_Empty);
		m_MarkSide = std::make_pair(false, IPaperSetting::kMarkSide_Empty);
		m_RepeatSheets = std::make_pair(false, 0);
	}

	PrintConditionsParser::ILogger& m_Logger;

	std::pair<bool, double>	m_Width;
	std::pair<bool, long>	m_Thickness;
	std::pair<bool, long>	m_Tension;
	std::pair<bool, bool>	m_DNS;
	std::pair<bool, bool>	m_IsPunchHole;
	std::pair<bool, bool>	m_IsSewing;
	std::pair<bool, bool>	m_IsMarker;
	std::pair<bool, bool>	m_IsPrePrinted;
	std::pair<bool, double>	m_MarkerOffset;
	std::pair<bool, double>	m_PageFeedSize;
	std::pair<bool, IPaperSetting::EPageFeedUnit> m_PageFeedUnit;
	std::pair<bool, IPaperSetting::EMarkSide> m_MarkSide;
	std::pair<bool, long>	m_RepeatSheets;
};


class DryerTemperature : public IPrintCondition::IDryerTemperature {
public:
	DryerTemperature(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement) : m_Logger(inLogger), m_HeatRollerSelf(false, 0), m_HeatRollerPart(false, 0), m_HeatFanSelf(false, 0), m_HeatFanPart(false, 0),
													m_NIRPowerSelf(false, 0), m_NIRPowerPart(false, 0), m_SubHeatRollerSelf(false, 0), m_SubHeatRollerPart(false, 0)
	{
		m_HeatRollerSelf = str2num<long>(inElement.getAttribute("HeatRollerSelf"));
		m_HeatRollerPart = str2num<long>(inElement.getAttribute("HeatRollerPart"));
		
		m_HeatFanSelf = str2num<long>(inElement.getAttribute("HeatFanSelf"));
		m_HeatFanPart = str2num<long>(inElement.getAttribute("HeatFanPart"));

		m_NIRPowerSelf = str2num<long>(inElement.getAttribute("NIRPowerSelf"));
		m_NIRPowerPart = str2num<long>(inElement.getAttribute("NIRPowerPart"));

		m_Precoater.resize(2);
		m_Precoater[0] = str2num<long>(inElement.getAttribute("Precoater1"));
		m_Precoater[1] = str2num<long>(inElement.getAttribute("Precoater2"));

		m_SubHeatRollerSelf = str2num<long>(inElement.getAttribute("SubHeatRollerSelf"));
		m_SubHeatRollerPart = str2num<long>(inElement.getAttribute("SubHeatRollerPart"));
	}

	virtual~DryerTemperature() {}

	/**
		ヒートローラー乾燥温度を取得する
		@param[in] inIsSelf : 自機なら true、他機なら false
		@return 乾燥温度
	*/
	virtual std::pair<bool, long> getHeatRoller(
		bool	inIsSelf = true) const
	{
		if (inIsSelf) {
			return m_HeatRollerSelf;
		}
		return m_HeatRollerPart;
	}

	/**
		温風アシスト乾燥温度を取得する
		@param[in] inIsSelf : 自機なら true、他機なら false
		@return 乾燥温度
	*/
	virtual std::pair<bool, long> getHeatFan(
		bool	inIsSelf = true) const
	{
		if (inIsSelf) {
			return m_HeatFanSelf;
		}
		return m_HeatFanPart;
	}

	/**
		IR設定を取得する
		@param[in] inSelf : 自機なら true、他機なら false
		@return IR設定
	*/
	virtual std::pair<bool, long> getNIRPower(
		bool	inSelf = true) const
	{
		return inSelf ? m_NIRPowerSelf : m_NIRPowerPart;
	}

	/**
		先塗り乾燥温度を取得する
		@param[in] inIndex : 1 or 2
		@return 乾燥温度
	*/
	virtual std::pair<bool, long> getPrecoater(
		int	inIndex = 1) const
	{
		if ((inIndex != 1) && (inIndex != 2)) {
			std::stringstream ss;
			ss << "invalid index for getPrecoater()" << inIndex;
			m_Logger.writeToDebugLog(ss.str());
			return std::make_pair(false, -1);
		}
		return m_Precoater[inIndex - 1];
	}

	/**
		サブヒートローラー乾燥温度を取得する
		@param[in] inIsSelf : 自機なら true、他機なら false
		@return 乾燥温度
	*/
	virtual std::pair<bool, long> getSubHeatRoller(
		bool	inIsSelf = true) const
	{
		if (inIsSelf) {
			return m_SubHeatRollerSelf;
		}
		return m_SubHeatRollerPart;
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::pair<bool, long> m_HeatRollerSelf;
	std::pair<bool, long> m_HeatRollerPart;
	std::pair<bool, long> m_HeatFanSelf;
	std::pair<bool, long> m_HeatFanPart;
	std::pair<bool, long> m_NIRPowerSelf;
	std::pair<bool, long> m_NIRPowerPart;
	std::pair<bool, long> m_SubHeatRollerSelf;
	std::pair<bool, long> m_SubHeatRollerPart;

	std::vector<std::pair<bool, long> > m_Precoater;
};

class ICCPreset : public IPrintCondition::IICCPreset {
public:
	ICCPreset(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement) : m_Logger(inLogger)
	{
		m_DescriptiveName = str2str(inElement.getAttribute("DescriptiveName"));
		m_ID = str2str(inElement.getAttribute("ID"));
	}

	virtual ~ICCPreset() {}

	/**
		ICC プリセット名を取得する
		@return プリセット名
	*/
	virtual std::pair<bool, std::string> getDescriptiveName() const { return m_DescriptiveName; }

	/**
		ICC プリセット ID を取得する
		@return プリセット ID
	*/
	virtual std::pair<bool, std::string> getID() const { return m_ID; }

private:
	PrintConditionsParser::ILogger& m_Logger;

	std::pair<bool, std::string> m_DescriptiveName;
	std::pair<bool, std::string> m_ID;
};


class ToneCurve : public IPrintCondition::IToneCurve {
public:
	ToneCurve(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement) : m_Logger(inLogger)
	{
		auto getColor = [](const std::string&	inStr) -> IPrintCondition::IToneCurve::EColor {
			static std::map<std::string, EColor> s_Map;
			if (s_Map.empty()) {
				s_Map.insert(std::pair<std::string, EColor>("C", IPrintCondition::IToneCurve::kToneCurve_Color_C));
				s_Map.insert(std::pair<std::string, EColor>("M", IPrintCondition::IToneCurve::kToneCurve_Color_M));
				s_Map.insert(std::pair<std::string, EColor>("Y", IPrintCondition::IToneCurve::kToneCurve_Color_Y));
				s_Map.insert(std::pair<std::string, EColor>("K", IPrintCondition::IToneCurve::kToneCurve_Color_K));
			}
			auto i = s_Map.find(inStr);
			std::stringstream ss;
			ss << "invalid AnchorPoint#Color:" << inStr;
			ThrowIfEnd(i, s_Map, ss.str())

			return i->second;
		};

		bool foundTone = false;
		size_t elemNum = inElement.getChildCount();
		for (size_t i = 0; i < elemNum; ++i) {
			const XMLElement* elem = inElement.getChild(i);
			ThrowIfNULL(elem, "Cannot found child element")
			if (elem->getKeyWithANSI().compare("ToneCurve") != 0) {
				continue;
			}
			foundTone = true;
			size_t sideNum = elem->getChildCount();
			if ((sideNum != 1) && (sideNum != 2)) {
				// SES で表面のみの可能性を考慮し、1 or 2 個限定とする
				std::stringstream ss;
				ss << "invalid tone side num detected:" << sideNum;
				std::invalid_argument e(ss.str());
				throw e;
			}
			for (size_t side = 0; side < sideNum; ++side) {
				const XMLElement* tone = elem->getChild(side);
				ThrowIfNULL(tone, "Cannot found child ToneCurve element")
				bool isFront = tone->getAttribute("Side").compare("Front") == 0;
				OneTone& toneParam = m_Tones[isFront ? 0 : 1];
				toneParam.m_DescriptiveName = str2str(tone->getAttribute("DescriptiveName"));

				const XMLElement* rootAnchor = tone->getChild("AnchorPoint");
				ThrowIfNULL(rootAnchor, "top level ToneCurve#AnchorPoint not found")

				size_t colors = rootAnchor->getChildCount();
				if (colors != kToneCurve_ColorNum) {
					// C/M/Y/K 前提
					std::stringstream ss;
					ss << "invalid AnchorPoint array size (colors) detected:" << colors;
					std::invalid_argument e(ss.str());
					throw e;
				}

				for (size_t color = 0; color < colors; ++color) {
					const XMLElement* colorAnchor = rootAnchor->getChild(color);
					ThrowIfNULL(colorAnchor, "second level AnchorPoint not found");
					IPrintCondition::IToneCurve::EColor c = getColor(colorAnchor->getAttribute("Color"));

					size_t points = colorAnchor->getChildCount();
					if (points != 5) {
						std::stringstream ss;
						ss << "invalid AnchorPoint array size (points) detected:" << points;
						std::invalid_argument e(ss.str());
						throw e;
					}
					std::vector<OneTone::AnchorPoint> pointArray;
					pointArray.resize(5);
					for (size_t point = 0; point < points; ++point) {
						const XMLElement* pointAnchor = colorAnchor->getChild(point);
						ThrowIfNULL(pointAnchor, "Cannot found AnchorPoint element")
						std::pair<bool, long> pid = str2num<long>(pointAnchor->getAttribute("PointID"));
						if ((pid.first == false) || (pid.second < 0) || (pid.second > 4)) {
							std::stringstream ss;
							ss << "invalid AnchorPoint#PointID detected or not found";
							std::invalid_argument e(ss.str());
							throw e;
						}
						pointArray.at(pid.second).m_BeforeConPoint = str2num<long>(pointAnchor->getAttribute("BeforeConPoint"));
						pointArray.at(pid.second).m_AfterConPoint = str2num<long>(pointAnchor->getAttribute("AfterConPoint"));
						pointArray.at(pid.second).m_EditType = str2num<long>(pointAnchor->getAttribute("AnchorPointEditType"));
						pointArray.at(pid.second).m_PointType = str2num<long>(pointAnchor->getAttribute("AnchorPointType"));
					}
					toneParam.m_Points.insert(std::pair<EColor, std::vector<OneTone::AnchorPoint> >(c, pointArray));
				}
			}
		}
		if (foundTone == false) {
			std::invalid_argument e("cannot found ToneCurve element");
			throw e;
		}
	}

	virtual ~ToneCurve() {}

	/**
		トーンカーブ名（GUI 表示名）を取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return トーンカーブ名
	*/
	virtual std::pair<bool, std::string> getDescriptiveName(
		bool	inIsFront) const
	{
		if (inIsFront) {
			return m_Tones[0].m_DescriptiveName;
		}
		return m_Tones[1].m_DescriptiveName;
	}

	/**
		アンカーポイント情報を取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@param[in] inColor : 取得したい色
		@param[in] inPointID : 取得したいポイント ID (0 - 4)
		@param[out] outBeforeConPoint : 基準濃度値
		@param[out] outAfterConPooint : トーンカーブデータの数値
		@param[out] outAnchorPointEditType : 編集タイプ（実質0固定）
		@param[out] outAnchorPointType : トーンカーブデータの数値が負なら 0、それ以外なら 1
	*/
	virtual void getAnchorPoint(
		bool	inIsFront,
		EColor	inColor,
		int		inPointID,
		std::pair<bool, long>&	outBeforeConPoint,
		std::pair<bool, long>&	outAfterConPoint,
		std::pair<bool, long>&	outAnchorPointEditType,
		std::pair<bool, long>&	outAnchorPointType) const
	{
		const OneTone& tone = m_Tones[inIsFront ? 0 : 1];
		try {
			auto color = tone.m_Points.find(inColor);
			ThrowIfEnd(color, tone.m_Points, "color is not found");
			auto point = color->second.at(inPointID);
			outBeforeConPoint = point.m_BeforeConPoint;
			outAfterConPoint = point.m_AfterConPoint;
			outAnchorPointEditType = point.m_EditType;
			outAnchorPointType = point.m_PointType;
		} catch (...) {
			outBeforeConPoint = std::make_pair(false, 0);
			outAfterConPoint = std::make_pair(false, 0);
			outAnchorPointEditType = std::make_pair(false, 0);
			outAnchorPointType = std::make_pair(false, 0);
		}
	}

private:
	class OneTone {
	public:
		std::pair<bool, std::string> m_DescriptiveName;
		
		class AnchorPoint {
		public:
			std::pair<bool, long> m_BeforeConPoint;
			std::pair<bool, long> m_AfterConPoint;
			std::pair<bool, long> m_EditType;
			std::pair<bool, long> m_PointType;

			AnchorPoint() : m_BeforeConPoint(false, 0), m_AfterConPoint(false, 0), m_EditType(false, 0), m_PointType(false, 0) {}
		};
		std::map<EColor, std::vector<AnchorPoint> > m_Points;
	};

	OneTone m_Tones[2];

	PrintConditionsParser::ILogger& m_Logger;
};

class JobBase : public IPrintCondition::IJobBase {
public:
	JobBase(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger),
			m_HasInformationSheet(false, false),
			m_HasFlushingPage(false, false),
			m_Interval(false, 0),
			m_IsPreJob(false, false),
			m_PreJobCount(false, 0),
			m_IsPostJob(false, false),
			m_OffsetFront(false, 0),
			m_OffsetBack(false, 0),
			m_CleaningJudgmentLevelBeforeJob(false, 0),
			m_CleaningJudgmentLevelAfterJob(false, 0),
			m_PrintStopSetting(false, "Stop")
	{
		m_HasInformationSheet = str2num(inElement.getAttribute("InformationSheet"));
		m_HasFlushingPage = str2num(inElement.getAttribute("FlushingPage"));
		m_PatternName = str2str(inElement.getAttribute("PatternName"));
		m_Interval = str2num<long>(inElement.getAttribute("Interval"));
		m_IsPreJob = str2num(inElement.getAttribute("PreJob"));
		m_PreJobCount = str2num<long>(inElement.getAttribute("PreJobCount"));
		m_IsPostJob = str2num(inElement.getAttribute("PostJob"));
		m_OffsetFront = str2num<long>(inElement.getAttribute("OffsetFront"));
		m_OffsetBack = str2num<long>(inElement.getAttribute("OffsetBack"));
		m_CleaningJudgmentLevelBeforeJob = str2num<long>(inElement.getAttribute("CleaningJudgmentLevelBeforeJob"));
		m_CleaningJudgmentLevelAfterJob = str2num<long>(inElement.getAttribute("CleaningJudgmentLevelAfterJob"));
		m_PrintStopSetting = str2str(inElement.getAttribute("PrintStopSetting"));
	}

	virtual ~JobBase() {}

	/**
		インフォメーションシートの出力有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasInformationSheet() const { return m_HasInformationSheet; }

	/**
		フラッシングページの挿入有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasFlushingPage() const { return m_HasFlushingPage; }
			
	/**
		フラッシングページのパターン名を取得する
		@return パターン名
	*/
	virtual std::pair<bool, std::string> getPatternName() const { return m_PatternName; }

	/**
		フラッシングページの挿入間隔を取得する
		@return 挿入間隔
	*/
	virtual std::pair<bool, long> getInterval() const { return m_Interval; }

	/**
		ジョブ前に挿入するかどうかを取得する
		@return true : する
		@return false : しない 
	*/
	virtual std::pair<bool, bool> isPreJob() const { return m_IsPreJob; }

	/**
		ジョブ前に挿入する回数を取得する
		@return 挿入回数
	*/
	virtual std::pair<bool, long> getPreJobCount() const { return m_PreJobCount; }

	/**
		ジョブ後に挿入するかどうかを取得する
		@return true : する
		@return false : しない 
	*/
	virtual std::pair<bool, bool> isPostJob() const { return m_IsPostJob; }

	/**
		フラッシングページのオフセットを取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return オフセット
	*/
	virtual std::pair<bool, long> getOffset(
		bool	inIsFront = true) const
	{
		return inIsFront ? m_OffsetFront : m_OffsetBack;
	}

	// ジョブ前のクリーニング判定レベルを取得する
	virtual std::pair<bool, long> getCleaningJudgmentLevelBeforeJob() const
	{
		return m_CleaningJudgmentLevelBeforeJob;
	}

	// ジョブ後のクリーニング判定レベルを取得する
	virtual std::pair<bool, long> getCleaningJudgmentLevelAfterJob() const
	{
		return m_CleaningJudgmentLevelAfterJob;
	}

	// 印刷停止設定を取得する
	virtual std::pair<bool, std::string> getPrintStopSetting() const
	{
		return m_PrintStopSetting;
	}

private:
	PrintConditionsParser::ILogger& m_Logger;
	
	std::pair<bool, bool> m_HasInformationSheet;
	std::pair<bool, bool> m_HasFlushingPage;
	std::pair<bool, std::string> m_PatternName;
	std::pair<bool, long> m_Interval;
	std::pair<bool, bool> m_IsPreJob;
	std::pair<bool, long> m_PreJobCount;
	std::pair<bool, bool> m_IsPostJob;
	std::pair<bool, long> m_OffsetFront;
	std::pair<bool, long> m_OffsetBack;
	std::pair<bool, long> m_CleaningJudgmentLevelBeforeJob;
	std::pair<bool, long> m_CleaningJudgmentLevelAfterJob;
	std::pair<bool, std::string> m_PrintStopSetting;
		
};

class Layout : public IPrintCondition::ILayout {
public:
	Layout(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger),
			m_HasVerification(false, false), m_ContinuousPageCount(false, 0), m_IsCueMark(false, false), m_IsBarcode(false, false), m_GapBetweenColors(false, 0), m_LineOffsetFront(false, 0), m_LineOffsetBack(false, 0), m_CutterMark(false, ILayout::kCutterMarkSide_Off), m_CutterMarkOffsetFront(false, 0), m_CutterMarkOffsetBack(false, 0), m_CutterMarkAlignFront(false, ILayout::kCutterMarkAlign_Center), m_CutterMarkAlignBack(false, ILayout::kCutterMarkAlign_Center), m_CutterMarkPageCount(false, 0)
			// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
			, m_AdjustmentSheetSizeEnabled(false, false), m_AdjustmentSheetLength(false, 0), m_AdjustmentSheetAfterJob(false, 0), m_AfterJobSheets(false, 0)
			// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	{
		auto str2xypair = [](
			const std::string&	inStr,
			std::vector<std::pair<bool, long>>&	outPair)
		{
			std::stringstream ss;
			ss << inStr;

			std::vector<long> vals;
			std::string s;
			while (std::getline(ss, s, ' ') != NULL) {
				vals.push_back(str2num<long>(s).second);
			}
			try {
				if (vals.size() != 2) {
					std::stringstream ss;
					ss << "cannot split to xypair:" << inStr;
					std::invalid_argument e(ss.str());
					throw e;
				}
				outPair.push_back(std::make_pair(true, vals[0]));
				outPair.push_back(std::make_pair(true, vals[1]));
			} catch (...) {
				outPair.push_back(std::make_pair(false, 0));
				outPair.push_back(std::make_pair(false, 0));
			}
		};

		auto str2MarkSide = [](
			const std::string&	inStr) -> std::pair<bool, IPrintCondition::ILayout::ECutterMarkSide>
		{
			static std::map<std::string, ECutterMarkSide> s_Map;
			if (s_Map.empty()) {
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkSide>("0", IPrintCondition::ILayout::kCutterMarkSide_Off));
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkSide>("1", IPrintCondition::ILayout::kCutterMarkSide_Front));
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkSide>("2", IPrintCondition::ILayout::kCutterMarkSide_Back));
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkSide>("3", IPrintCondition::ILayout::kCutterMarkSide_Both));
			}
			auto side = s_Map.find(inStr);
			try {
				std::stringstream ss;
				ss << "invalid Layout#CutterMark detected:" << inStr;
				ThrowIfEnd(side, s_Map, ss.str())
				return std::make_pair(true, side->second);
			} catch (...) {
				return std::make_pair(false, IPrintCondition::ILayout::kCutterMarkSide_Off);
			}
		};

		auto str2MarkAlign = [](
			const std::string&	inStr) -> std::pair<bool, IPrintCondition::ILayout::ECutterMarkAlign>
		{
			static std::map<std::string, ECutterMarkAlign> s_Map;
			if (s_Map.empty()) {
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkAlign>("-1", IPrintCondition::ILayout::kCutterMarkAlign_Left));
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkAlign>("0", IPrintCondition::ILayout::kCutterMarkAlign_Center));
				s_Map.insert(std::pair<std::string, IPrintCondition::ILayout::ECutterMarkAlign>("1", IPrintCondition::ILayout::kCutterMarkAlign_Right));
			}
			auto align = s_Map.find(inStr);
			try {
				std::stringstream ss;
				ss << "invalid Layout#CutterMarkAlign[Front|Back] detected:" << inStr;
				ThrowIfEnd(align, s_Map, ss.str())
				return std::make_pair(true, align->second);
			} catch (...) {
				return std::make_pair(false, IPrintCondition::ILayout::kCutterMarkAlign_Left);
			}
		};

		try {
			str2xypair(inElement.getAttribute("OffsetFront"), m_OffsetFront);
			str2xypair(inElement.getAttribute("OffsetBack"), m_OffsetBack);
			m_HasVerification = str2num(inElement.getAttribute("Verification"));
			m_ContinuousPageCount = str2num<long>(inElement.getAttribute("ContinuousPageCount"));
			m_IsCueMark = str2num(inElement.getAttribute("CueMark"));
			m_IsBarcode = str2num(inElement.getAttribute("Barcode"));
			str2xypair(inElement.getAttribute("BarcodeOffsetFront"), m_BarcodeOffsetFront);
			str2xypair(inElement.getAttribute("BarcodeOffsetBack"), m_BarcodeOffsetBack);
			m_FlushingPattern = str2str(inElement.getAttribute("FlushingPattern"));
			m_GapBetweenColors = str2num<long>(inElement.getAttribute("GapBetweenColors"));
			m_LineOffsetFront = str2num<long>(inElement.getAttribute("LineOffsetFront"));
			m_LineOffsetBack = str2num<long>(inElement.getAttribute("LineOffsetBack"));
			m_LineGap = str2num(inElement.getAttribute("LineGap"));
			m_LineGapFront = str2num<long>(inElement.getAttribute("LineGapFront"));
			m_LineGapBack = str2num<long>(inElement.getAttribute("LineGapBack"));
			m_LineGapOffsetFront = str2num<long>(inElement.getAttribute("LineGapOffsetFront"));
			m_LineGapOffsetBack = str2num<long>(inElement.getAttribute("LineGapOffsetBack"));
			m_LinePosotion = str2str(inElement.getAttribute("LinePosition"));
			m_LineOptions = str2str(inElement.getAttribute("LineOptions"));
			m_CutterMark = str2MarkSide(inElement.getAttribute("CutterMark"));
			str2xypair(inElement.getAttribute("CutterMarkSize"), m_CutterMarkSize);
			m_CutterMarkOffsetFront = str2num<long>(inElement.getAttribute("CutterMarkOffsetFront"));
			m_CutterMarkOffsetBack = str2num<long>(inElement.getAttribute("CutterMarkOffsetBack"));
			m_CutterMarkAlignFront = str2MarkAlign(inElement.getAttribute("CutterMarkAlignFront"));
			m_CutterMarkAlignBack = str2MarkAlign(inElement.getAttribute("CutterMarkAlignBack"));
			m_CutterMarkPageCount = str2num<long>(inElement.getAttribute("CutterMarkPageCount"));

			// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
			m_AdjustmentSheetSizeEnabled = str2num(inElement.getAttribute("AdjustmentSheetSizeEnabled"));
			m_AdjustmentSheetLength = str2num<long>(inElement.getAttribute("AdjustmentSheetLength"));
			m_AdjustmentSheetAfterJob = str2num<long>(inElement.getAttribute("AdjustmentSheetAfterJob"));
			m_AfterJobSheets = str2num<long>(inElement.getAttribute("AfterJobSheets"));
			// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
		} catch (...) {
			throw;
		}
	}

	virtual ~Layout() {}

	/**
		オフセットを取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@param[out] outX : 主走査方向オフセット値
		@param[out] outY : 副走査方向オフセット値
	*/
	virtual void getOffset(
		bool	inIsFront,
		std::pair<bool, long>&	outX,
		std::pair<bool, long>&	outY) const
	{
		const std::vector<std::pair<bool, long> >& offset = inIsFront ? m_OffsetFront : m_OffsetBack;
		outX = offset[0];
		outY = offset[1];
	}
	
	/**
		サイド検査の有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> isVerification() const { return m_HasVerification; }

	/**
		エラーにする連続シート数を取得する
		@return シート数
	*/
	virtual std::pair<bool, long> getContinuousPageCount() const { return m_ContinuousPageCount; }


	/**
		印刷開始マークの出力の有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> hasCueMark() const { return m_IsCueMark; }

	/**
		バーコード出力の有無を取得する
		@return true : 有
		@return false : 無
	*/
	virtual std::pair<bool, bool> isBarcode() const { return m_IsBarcode; }

	/**
		バーコードのオフセット値を取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@param[out] outX : 主走査方向オフセット値
		@param[out] outY : 副走査方向オフセット値
	*/
	virtual void getBarcodeOffset(
		bool	inIsFront,
		std::pair<bool, long>&	outX,
		std::pair<bool, long>&	outY) const
	{
		const std::vector<std::pair<bool, long> >& offset = inIsFront ? m_BarcodeOffsetFront : m_BarcodeOffsetBack;
		outX = offset[0];
		outY = offset[1];
	}

	/**
		フラッシングパターンの設定を取得する
		@return フラッシングパターン設定値
	*/
	virtual std::pair<bool, std::string> getFlushingPattern() const { return m_FlushingPattern; }

	/**
		ラインフラッシングのずらし量を取得する
		@return ずらし量
	*/
	virtual std::pair<bool, long> getGapBetweenColors() const { return m_GapBetweenColors; }

	/**
		ラインフラッシングのオフセット値を取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return オフセット値
	*/
	virtual std::pair<bool, long> getLineOffset(
		bool	inIsFront) const
	{
		return inIsFront ? m_LineOffsetFront : m_LineOffsetBack;
	}
	
	/**
		ページ内、ラインフラッシングを取得する
		@return ページ内、ラインフラッシング
	*/
	virtual std::pair<bool, bool> getLayoutLineGap() const
	{
		return m_LineGap;
	}

	/**
		ページ内、ラインフラッシング間隔（副走査）
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return ページ内、ラインフラッシング間隔（副走査）
	*/
	virtual std::pair<bool, long> getLayoutLineGap(
		bool	inIsFront) const
	{
		return inIsFront ? m_LineGapFront : m_LineGapBack;
	}

	/**
		ページ内、ラインフラッシングの補正オフセット（副走査）
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return ページ内、ラインフラッシングの補正オフセット（副走査）
	*/
	virtual std::pair<bool, long> getLayoutLineGapOffset(
		bool	inIsFront) const
	{
		return inIsFront ? m_LineGapOffsetFront : m_LineGapOffsetBack;
	}

	/**
		Bottom指定時、ラインフラッシングの下端配置および後処理マークの下端配置がON
		@return ページ内、ラインフラッシングの補正オフセット（副走査）
	*/
	virtual std::pair<bool, std::string> getLayoutLinePosotion() const
	{
		return m_LinePosotion;
	}

	/**
		ラインフラッシングのオプション情報(ST_PDB_LAYOUT_IMAGE_INFOのST_JOB_FLUSHING_PATTERN_ADD情報)
		@return ページ内、ラインフラッシングの補正オフセット（副走査）
	*/
	virtual std::pair<bool, std::string> getLayoutLineOptions() const
	{
		return m_LineOptions;
	}

	/**
		カッターマークの印刷面を取得する
	*/
	virtual std::pair<bool, ECutterMarkSide> getCutterMark() const { return m_CutterMark; }

	/**
		カッターマークのサイズを取得する
		@param[out] outX : 主走査
		@param[out] outY : 副走査
	*/
	virtual void getCutterMarkSize(
		std::pair<bool, long>&	outX,
		std::pair<bool, long>&	outY) const
	{
		outX = m_CutterMarkSize[0];
		outY = m_CutterMarkSize[1];
	}

	/**
		カッターマークのオフセットを取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return オフセット
	*/
	virtual std::pair<bool, long> getCutterMarkOffset(
		bool	inIsFront = true) const
	{
		return inIsFront ? m_CutterMarkOffsetFront : m_CutterMarkOffsetBack;
	}

			
	/**
		カッターマークの基準位置を取得する
		@param[in] inIsFront : 表面なら true、裏面なら false
		@return 基準位置
	*/
	virtual std::pair<bool, ECutterMarkAlign> getCutterMarkAlign(
		bool	inIsFront = true) const
	{
		return inIsFront ? m_CutterMarkAlignFront : m_CutterMarkAlignBack;
	}

	/**
		カッターマークの準備シート数を取得する
		@return 準備シート数
	*/
	virtual std::pair<bool, long> getCutterMarkPageCount() const
	{
		return m_CutterMarkPageCount;
	}

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	virtual std::pair<bool, bool> getAdjustmentSheetSizeEnabled() const
	{
		return m_AdjustmentSheetSizeEnabled;
	}
	virtual std::pair<bool, long> getAdjustmentSheetLength() const
	{
		return m_AdjustmentSheetLength;
	}
	virtual std::pair<bool, long> getAdjustmentSheetAfterJob() const
	{
		return m_AdjustmentSheetAfterJob;
	}
	virtual std::pair<bool, long> getAfterJobSheets() const
	{
		return m_AfterJobSheets;
	}
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::vector<std::pair<bool, long> > m_OffsetFront;
	std::vector<std::pair<bool, long> > m_OffsetBack;
	std::pair<bool, bool> m_HasVerification;
	std::pair<bool, long> m_ContinuousPageCount;
	std::pair<bool, bool> m_IsCueMark;
	std::pair<bool, bool> m_IsBarcode;
	std::vector<std::pair<bool, long> > m_BarcodeOffsetFront;
	std::vector<std::pair<bool, long> > m_BarcodeOffsetBack;
	std::pair<bool, std::string> m_FlushingPattern;
	std::pair<bool, long> m_GapBetweenColors;
	std::pair<bool, long> m_LineOffsetFront;
	std::pair<bool, long> m_LineOffsetBack;
	std::pair<bool, bool> m_LineGap;
	std::pair<bool, long> m_LineGapFront;
	std::pair<bool, long> m_LineGapBack;
	std::pair<bool, long> m_LineGapOffsetFront;
	std::pair<bool, long> m_LineGapOffsetBack;
	std::pair<bool, std::string> m_LinePosotion;
	std::pair<bool, std::string> m_LineOptions;
	std::pair<bool, ECutterMarkSide> m_CutterMark;
	std::vector<std::pair<bool, long> > m_CutterMarkSize;
	std::pair<bool, long> m_CutterMarkOffsetFront;
	std::pair<bool, long> m_CutterMarkOffsetBack;
	std::pair<bool, ECutterMarkAlign> m_CutterMarkAlignFront;
	std::pair<bool, ECutterMarkAlign> m_CutterMarkAlignBack;
	std::pair<bool, long> m_CutterMarkPageCount;

	// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
	std::pair<bool, bool> m_AdjustmentSheetSizeEnabled;
	std::pair<bool, long> m_AdjustmentSheetLength;
	std::pair<bool, long> m_AdjustmentSheetAfterJob;
	std::pair<bool, long> m_AfterJobSheets;
	// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

};

//PrintCondition#scr:VerificationParams 情報クラス
class VerificationParams : public IPrintCondition::IVerificationParams {
public:
	//コンストラクタ
	VerificationParams(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{
		try
		{
			auto getKeyList = [](std::vector<std::string>& outKeyList) -> void {
				static std::vector<std::string> keyList;
				if(keyList.empty())
				{
					keyList.push_back("scr:AutoPrintStop");
					keyList.push_back("scr:AutoStopDefectCountOrange");
					keyList.push_back("scr:AutoStopDefectCountRed");
					keyList.push_back("scr:DefectAppearanceRatioOrange");
					keyList.push_back("scr:DefectAppearanceRatioRed");
					keyList.push_back("scr:DefectDetectionSensitivity");
					keyList.push_back("scr:DetectionLevelHeight");
					keyList.push_back("scr:DetectionLevelWidth");
					keyList.push_back("scr:DetectionMaskWidth");
					keyList.push_back("scr:DirtStreakBackImageRemoval");
					keyList.push_back("scr:DirtStreakFilter");
					keyList.push_back("scr:DirtStreakMaximumWidth");
					keyList.push_back("scr:DirtStreakMinimumWidth");
					keyList.push_back("scr:DirtStreakSigma");
					keyList.push_back("scr:DirtStreakValue");
					keyList.push_back("scr:IsDirtStreakDetection");
					keyList.push_back("scr:PrePrintMaskWidth");
					keyList.push_back("scr:StainDetectionSensitivity");
					keyList.push_back("scr:StreakDetectionLevelStep");
					keyList.push_back("scr:StreakDetectionLevelWidth");
					keyList.push_back("scr:StreakDetectionSensitivity");
				}
				outKeyList = keyList;
			};

			auto getAIAssistKeyList = [](std::vector<std::string>& outKeyList) -> void {
				static std::vector<std::string> keyList;
				if(keyList.empty())
				{
					keyList.push_back("scr:AssistRange");
					keyList.push_back("scr:DefectDetectionSensitivity");
					keyList.push_back("scr:DetectionLevelHeight");
					keyList.push_back("scr:DetectionLevelWidth");
					keyList.push_back("scr:DirtStreakValue");
					keyList.push_back("scr:EnableAIAssist");
					keyList.push_back("scr:EnableAIFiltering");
					keyList.push_back("scr:EnableAutoSetting");
					keyList.push_back("scr:NotificationLevel");
					keyList.push_back("scr:StainDetectionSensitivity");
					keyList.push_back("scr:StreakDetectionSensitivity");
					keyList.push_back("scr:DirtStreakMaximumWidth");
				}
				outKeyList = keyList;
			};

			m_side = str2str(inElement.getAttribute("Side"));

			const XMLElement* elem = nullptr;
			size_t elemNum = inElement.getChildCount();
			bool chkVerificationParams, chkAIAssistParams;
			chkVerificationParams = chkAIAssistParams = false;
			for (size_t i = 0; i < elemNum; ++i)
			{
				elem = inElement.getChild(i);
				ThrowIfNULL(elem, "scr:VerificationParams element(inner) is NULL")
				if (elem->getKeyWithANSI().compare("scr:VerificationParams") == 0)
				{
					//最初に見つかった要素差だけ処理する
					if( !chkVerificationParams ) {
						std::vector<std::string> keyList;
						getKeyList(keyList);
						for(auto& it = keyList.begin(); it != keyList.end(); it++)
						{
							m_scrElementMap[*it] = str2str(elem->getAttribute(*it));
						}
						if( chkAIAssistParams )	break;
						chkVerificationParams = true;
					}
				} else if (elem->getKeyWithANSI().compare("scr:AIAssistParams") == 0)
				{
					//最初に見つかった要素差だけ処理する
					if( !chkAIAssistParams ) {
						std::vector<std::string> keyList;
						getAIAssistKeyList(keyList);
						for(auto& it = keyList.begin(); it != keyList.end(); it++)
						{
							m_scrAIAssistElementMap[*it] = str2str(elem->getAttribute(*it));
						}
						if( chkVerificationParams )	break;
						chkAIAssistParams = true;
					}
				}
			}//for
		}
		catch (...) 
		{
			throw;
		}
	}

	//デストラクタ
	virtual ~VerificationParams() {}

	//対象面（表 or 裏）を取得する
	virtual std::pair<bool, std::string> getSide() const
	{
		return m_side;
	}

	//属性リストを取得する
	virtual std::map<std::string, std::pair<bool, std::string> > getScrAttributes() const
	{
		return m_scrElementMap;
	}

	//scr:AIAssistParams属性リストを取得する
	virtual std::map<std::string, std::pair<bool, std::string> > getScrAIAssistAttributes() const
	{
		return m_scrAIAssistElementMap;
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::pair<bool, std::string> m_side;									//!< 対象面（表 or 裏）
	std::map<std::string, std::pair<bool, std::string> > m_scrElementMap;	//!< scrエレメントのマップ<キー,CValue>
	std::map<std::string, std::pair<bool, std::string> > m_scrAIAssistElementMap;	//!< scr:AIAssistParams属性値のマップ<キー,CValue>
};

//PrintCondition#scr:TemporalMask 情報クラス
class TemporalMask : public IPrintCondition::ITemporalMask {
public:
	//一時マスクレコードクラス
	class TemporalMaskRecord : public ITemporalMaskRecord {
	public:
		//コンストラクタ
		TemporalMaskRecord(
			const XMLElement&				inElement)
		{
			m_type = str2str(inElement.getAttribute("Type"));
			str2xypair_t(inElement.getAttribute("Pos"), m_pos);
			str2xypair_t(inElement.getAttribute("Size"), m_size);
			str2xypair_t(inElement.getAttribute("TemporalMaskSize"), m_temporalMaskSize);
		}

		//デストラクタ
		virtual ~TemporalMaskRecord() {};
		
		//欠点種類（汚れ or スジ汚れ or 白スジ or 欠落）を取得する
		virtual std::pair<bool, std::string> getType() const
		{
			return m_type;
		}

		//欠点位置を取得する
		virtual std::vector<std::pair<bool, double> > getPos() const
		{
			return m_pos;
		}

		//欠点サイズを取得する
		virtual std::vector<std::pair<bool, double> > getSize() const
		{
			return m_size;
		}

		//一時マスクサイズを取得する
		virtual std::vector<std::pair<bool, double> > getTemporalMaskSize() const
		{
			return m_temporalMaskSize;
		}

	private:
		std::pair<bool, std::string>			m_type;				//!< 欠点種類
		std::vector<std::pair<bool, double> >	m_pos;				//!< 欠点位置
		std::vector<std::pair<bool, double> >	m_size;				//!< 欠点サイズ
		std::vector<std::pair<bool, double> >	m_temporalMaskSize;	//!< 一時マスクサイズ
	};

	//コンストラクタ
	TemporalMask(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{
		try
		{
			m_side = str2str(inElement.getAttribute("Side"));
			m_size = str2num<long>(inElement.getAttribute("Size"));

			size_t recordNum = inElement.getChildCount();
			size_t actualNum = 0;
			for (size_t j = 0; j < recordNum; ++j) {
				const XMLElement* elem = inElement.getChild(j);
				ThrowIfNULL(elem, "Cannot found child element(scr:TemporalMask)")
				if (elem->getKeyWithANSI().compare("scr:TemporalMask") != 0) {
					continue;
				}
				std::shared_ptr<TemporalMaskRecord> temporalMaskRecord;
				temporalMaskRecord.reset(new TemporalMaskRecord(*elem));
				m_TemporalMaskRecord.push_back(temporalMaskRecord);
				actualNum++;
			}//for
			ThrowIfNot((m_size.second != actualNum), "Unmatch # of scr:TemporalMask and Size");
		}
		catch (...) 
		{
			throw;
		}
	}

	//デストラクタ
	virtual ~TemporalMask() {}

	//対象面（表 or 裏）を取得する
	virtual std::pair<bool, std::string> getSide() const
	{
		return m_side;
	}

	//一時マスク個数を取得する
	virtual std::pair<bool, long> getSize() const
	{
		return m_size;
	}

	//一時マスクレコードリストを取得する
	virtual std::vector<std::shared_ptr<ITemporalMaskRecord> > getTemporalMaskRecord()  const
	{
		return m_TemporalMaskRecord;
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::pair<bool, std::string> m_side;			//!< 対象面（表 or 裏）
	std::pair<bool, long> m_size;					//!< 一時マスク個数

	std::vector<std::shared_ptr<ITemporalMaskRecord> > m_TemporalMaskRecord;	//!< 一時マスクレコード

};


//PrintCondition#scr:InspectionArea 情報 クラス
class InspectionArea : public IPrintCondition::IInspectionArea {
public:
	//非検査矩形エリア情報 クラス
	class NonInspectionRect : public INonInspectionRect {
	public:
		//コンストラクタ
		NonInspectionRect(
			const XMLElement&				inElement)
		{
			m_iD = str2str(inElement.getAttribute("ID"));
			str2xypair_t(inElement.getAttribute("StartPos"), m_startPos);
			str2xypair_t(inElement.getAttribute("EndPos"), m_endPos);

		}

		//デストラクタ
		virtual ~NonInspectionRect() {};
		
		//非検査矩形エリアIDを取得する
		virtual std::pair<bool, std::string> getID() const
		{
			return m_iD;
		}

		//非検査矩形エリア始点情報を取得する
		virtual std::vector<std::pair<bool, long> > getStartPos() const
		{
			return m_startPos;
		}

		//非検査矩形エリア終点情報を取得する
		virtual std::vector<std::pair<bool, long> > getEndPos() const
		{
			return m_endPos;
		}

	private:
		std::pair<bool, std::string>		m_iD;			//!< 非検査矩形エリアID 1～5
		std::vector<std::pair<bool, long> > m_startPos;		//!< 始点
		std::vector<std::pair<bool, long> > m_endPos;		//!< 終点

	};

	//コンストラクタ
	InspectionArea(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{

		try {
			m_side = str2str(inElement.getAttribute("Side"));
			m_nonInspectionAreaLeft = str2num<double>(inElement.getAttribute("NonInspectionAreaLeft"));
			m_nonInspectionAreaRight = str2num<double>(inElement.getAttribute("NonInspectionAreaRight"));

			const XMLElement* nonInspectionRectElem = inElement.getChild("NonInspectionRect");
			if(nonInspectionRectElem == nullptr)
			{
				return;
			}

			size_t rectNum = nonInspectionRectElem->getChildCount();
			for (size_t j = 0; j < rectNum; ++j) {
				const XMLElement* rectElem = nonInspectionRectElem->getChild(j);
				ThrowIfNULL(rectElem, "Cannot found child element(NonInspectionRect)")
				if (rectElem->getKeyWithANSI().compare("NonInspectionRect") != 0) {
					continue;
				}
				std::shared_ptr<NonInspectionRect> nonInspectionRect;
				nonInspectionRect.reset(new NonInspectionRect(*rectElem));
				m_nonInspectionRect.push_back(nonInspectionRect);
			}//for
		} catch (...) {
			throw;
		}
	}

	//デストラクタ
	virtual ~InspectionArea() {}

	//対象面（表 or 裏）を取得する
	virtual std::pair<bool, std::string> getSide() const
	{
		return m_side;
	}

	//非検査エリア(紙端)左端 を取得する
	virtual std::pair<bool, double> getNonInspectionAreaLeft() const
	{
		return m_nonInspectionAreaLeft;
	}
	
	//非検査エリア(紙端)右端 を取得する
	virtual std::pair<bool, double> getNonInspectionAreaRight() const
	{
		return m_nonInspectionAreaRight;
	}

	//非検査エリア(矩形) を取得する
	virtual std::vector<std::shared_ptr<INonInspectionRect> > getNonInspectionRect() const
	{
		return m_nonInspectionRect;
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::pair<bool, std::string>		m_side;							//!< 対象面（表 or 裏）Front / Back
	std::pair<bool, double>				m_nonInspectionAreaLeft;		//!< 非検査エリア左端
	std::pair<bool, double>				m_nonInspectionAreaRight;		//!< 非検査エリア右端

	std::vector<std::shared_ptr<INonInspectionRect> >	m_nonInspectionRect;	//!< 非検査エリア矩形
};

//PrintCondition#Inspection 情報クラス
class Inspection : public IPrintCondition::IInspection {
public:
	//コンストラクタ
	Inspection(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{
		try {
			m_paramaterName = str2str(inElement.getAttribute("ParamaterName"));
			m_temporalMaskFilePath = str2str(inElement.getAttribute("TemporalMaskFilePath"));
			
			const XMLElement* inspectionAreaElem = inElement.getChild("InspectionArea");

			if(inspectionAreaElem != nullptr)
			{
				size_t elemNum = inspectionAreaElem->getChildCount();
				for (size_t i = 0; i < elemNum; ++i) {
					const XMLElement* elem = inspectionAreaElem->getChild(i);
					ThrowIfNULL(elem, "Cannot found child element(InspectionArea)")
					if (elem->getKeyWithANSI().compare("InspectionArea") != 0) {
						continue;
					}
					std::shared_ptr<IPrintCondition::IInspectionArea> inspectionArea;
					inspectionArea.reset(new InspectionArea(inLogger, *elem));
					m_inspectionArea.push_back(inspectionArea);
				}//for
			}

			//以降はMainteJI連携部分
			size_t elemNum2 = inElement.getChildCount();
			for (size_t i = 0; i < elemNum2; ++i) {
				const XMLElement* elem = inElement.getChild(i);
				ThrowIfNULL(elem, "Cannot found child element(scr:)")
				if (elem->getKeyWithANSI().compare("scr:VerificationParams") == 0) {
					std::shared_ptr<IPrintCondition::IVerificationParams> verificationParams;
					verificationParams.reset(new VerificationParams(inLogger, *elem));
					m_verificationParams.push_back(verificationParams);
				}
				if (elem->getKeyWithANSI().compare("scr:TemporalMask") == 0) {
					std::shared_ptr<IPrintCondition::ITemporalMask> temporalMask;
					temporalMask.reset(new TemporalMask(inLogger, *elem));
					m_temporalMask.push_back(temporalMask);
				}
			}//for

		} catch (...) {
			throw;
		}
	}

	//デストラクタ
	virtual ~Inspection() {}

	//検査条件名エレメントを取得する
	virtual std::pair<bool, std::string> getParamaterName()  const
	{
		return m_paramaterName;
	}

	//IVerificationParams エレメントを取得する
	virtual const std::vector<std::shared_ptr<IPrintCondition::IVerificationParams> >& getVerificationParams() const
	{
		return m_verificationParams;
	}

	//一時マスクファイルパスエレメントを取得する
	virtual std::pair<bool, std::string> getTemporalMaskFilePath() const
	{
		return m_temporalMaskFilePath;
	}

	//ITemporalMask エレメントを取得する
	virtual const std::vector<std::shared_ptr<IPrintCondition::ITemporalMask> >& getTemporalMask() const
	{
		return m_temporalMask;
	}

	//IInspectionArea エレメントを取得する
	virtual const std::vector<std::shared_ptr<IPrintCondition::IInspectionArea> >& getInspectionArea() const
	{
		return m_inspectionArea;
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;	

	std::pair<bool, std::string>		m_paramaterName;				//!< 検査条件名
	std::pair<bool, std::string>		m_temporalMaskFilePath;			//!< 一時マスクファイルパス
	std::vector<std::shared_ptr<IPrintCondition::IVerificationParams> > m_verificationParams;	//!< 検査条件詳細情報
	std::vector<std::shared_ptr<IPrintCondition::ITemporalMask> >		m_temporalMask;			//!< 一時マスクファイルパス
	std::vector<std::shared_ptr<IPrintCondition::IInspectionArea> >		m_inspectionArea;		//!< 検査エリア
};

// 詳細情報
class DetailInfo {
public:
	//コンストラクタ
	DetailInfo() {};

	//デストラクタ
	virtual ~DetailInfo() {};

	std::pair<bool, std::string>		m_side;							//!< 対象面（表 or 裏）Front / Back
	std::vector<std::pair<bool, double> >	m_size;						//!< サイズ設定
	std::vector<std::pair<bool, double> >	m_offset;					//!< 位置設定
	std::pair<bool, std::string>		m_name;							//!< ラベル名称
	std::pair<bool, std::string>		m_type;							//!< ラベル種類
	std::pair<bool, std::string>		m_angle;						//!< （OCR限定）角度情報
	std::pair<bool, bool>				m_errorStop;					//!< デコードエラー発生時、エラー停止判定カウントの対象にするか？
};

//PrintCondition#DecodeDetails 情報クラス
class DecodeDetails : public IPrintCondition::IDecodeDetails
{
public:
	//コンストラクタ
	DecodeDetails(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{
		try {
			m_detailInfo.m_side = str2str(inElement.getAttribute("Side"));
			str2xypair_t(inElement.getAttribute("Size"), m_detailInfo.m_size);
			str2xypair_t(inElement.getAttribute("Offset"), m_detailInfo.m_offset);
			m_detailInfo.m_name  = str2str(inElement.getAttribute("Name"));
			m_detailInfo.m_type  = str2str(inElement.getAttribute("Type"));
			m_detailInfo.m_angle = str2str(inElement.getAttribute("Angle"));
			m_detailInfo.m_errorStop = str2num(inElement.getAttribute("ErrorStop"));
		} catch (...) {
			throw;
		}
	}

	//デストラクタ
	virtual ~DecodeDetails() {}

	//対象面（表 or 裏）を取得する
	virtual std::pair<bool, std::string>		 getSide()  const
	{
		return m_detailInfo.m_side;
	}

	//サイズ設定を取得する
	virtual std::vector<std::pair<bool, double> >	 getSize()  const
	{
		return m_detailInfo.m_size;
	}

	//位置設定を取得する
	virtual std::vector<std::pair<bool, double> >	 getOffset()  const
	{
		return m_detailInfo.m_offset;
	}

	//ラベル名称を取得する
	virtual std::pair<bool, std::string>		 getName()  const
	{
		return m_detailInfo.m_name;
	}

	//ラベル種類を取得する
	virtual std::pair<bool, std::string>		 getType()  const	
	{
		return m_detailInfo.m_type;
	}

	//（OCR限定）角度情報を取得する
	virtual std::pair<bool, std::string>		 getAngle()  const
	{
		return m_detailInfo.m_angle;
	}

	//（デコードエラー発生時、エラー停止判定カウントの対象にするか？を取得する
	virtual std::pair<bool, bool>				 getErrorStop()  const
	{
		return m_detailInfo.m_errorStop;
	}


private:
	PrintConditionsParser::ILogger&	m_Logger;

	DetailInfo							m_detailInfo;		//Decode詳細情報
};

//PrintCondition#Decode 情報クラス
class Decode : public IPrintCondition::IDecode
{
public:
	//コンストラクタ
	Decode(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{

		try {
			m_errorSheets = str2num<long>(inElement.getAttribute("ErrorSheets"));
			m_unit = str2str(inElement.getAttribute("Unit"));
			m_isEnabled = str2num(inElement.getAttribute("IsEnabled"));

			const XMLElement* decodeDetailsElem = inElement.getChild("DecodeDetails");
			if(decodeDetailsElem == nullptr)
			{
				return;
			}

			size_t elemNum = decodeDetailsElem->getChildCount();
			for (size_t i = 0; i < elemNum; ++i) {
				const XMLElement* elem = decodeDetailsElem->getChild(i);
				ThrowIfNULL(elem, "Cannot found child element(DecodeDetails)")
				if (elem->getKeyWithANSI().compare("DecodeDetails") != 0) {
					continue;
				}
				std::shared_ptr<IPrintCondition::IDecodeDetails> detail;
				detail.reset(new DecodeDetails(m_Logger, *elem));

				DetailInfo detailInfo;
				detailInfo.m_side		= detail->getSide();
				detailInfo.m_size		= detail->getSize();
				detailInfo.m_offset		= detail->getOffset();
				detailInfo.m_name		= detail->getName();
				detailInfo.m_type		= detail->getType();
				detailInfo.m_angle		= detail->getAngle();
				detailInfo.m_errorStop	= detail->getErrorStop();
				m_detailInfoList.push_back(detailInfo);
			} //for
		} catch (...) {
			throw;
		}
	}
	
	//デストラクタ
	virtual ~Decode() {}

	//エラー停止連続シート数を取得する
	virtual std::pair<bool, long> getErrorSheets()  const
	{
		return m_errorSheets;
	}

	//数値単位系を取得する
	virtual std::pair<bool, std::string> getUnit()  const
	{
		return m_unit;
	}

	//デコード処理の有効／無効を取得する
	virtual std::pair<bool, bool> getIsEnabled()  const
	{
		return m_isEnabled;
	}

	//Decode詳細 エレメントを取得する
	virtual bool getDetail(
		size_t		inIndex,
		std::pair<bool, std::string>&			outSide,	
		std::vector<std::pair<bool, double> >&	outSize,	
		std::vector<std::pair<bool, double> >&	outOffset,	
		std::pair<bool, std::string>&			outName,	
		std::pair<bool, std::string>&			outType,	
		std::pair<bool, std::string>&			outAngle,	
		std::pair<bool, bool>&					outErrorStop) const
	{
		if(inIndex < m_detailInfoList.size())
		{
			auto& detailInfo = m_detailInfoList[inIndex];
			outSide	     = detailInfo.m_side;
			outSize		 = detailInfo.m_size;
			outOffset	 = detailInfo.m_offset;
			outName		 = detailInfo.m_name;	
			outType		 = detailInfo.m_type;
			outAngle	 = detailInfo.m_angle;
			outErrorStop = detailInfo.m_errorStop;

			//中身がそろっているかどうか
			bool exists = 
				outSide.first &&	     
				outName.first &&
				outType.first &&
				outAngle.first &&
				outErrorStop.first;
			for(auto& itSize = outSize.begin(); itSize != outSize.end(); itSize++)
			{
				exists = exists && itSize->first;
			}
			for(auto& itOffset = outSize.begin(); itOffset != outSize.end(); itOffset++)
			{
				exists = exists && itOffset->first;
			}
			return  exists;
		}
		else
		{
			return false;
		}
	}

private:
	PrintConditionsParser::ILogger&	m_Logger;	

	std::pair<bool, long>				m_errorSheets;					//!< エラー停止連続シート数
	std::pair<bool, std::string>		m_unit;							//!< 数値単位系
	std::pair<bool, bool>				m_isEnabled;					//!< デコード処理の有効／無効

	std::vector<DetailInfo>				m_detailInfoList;				//!< 詳細情報リスト
};

/**
 * @brief	PrintCondition#Scaling 情報クラス
*/
class Scaling : public IPrintCondition::IScaling
{
public:
	/**
	 * @brief	コンストラクタ
	 * @param[in]	inLogger : ログ出力用クラス
	 * @param[in]	inElement : XMLElementクラス
	*/
	Scaling(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement)
		:	m_Logger(inLogger)
	{
		try
		{
			m_ScalingValue = str2num(inElement.getAttribute("ScalingValue"));
			m_BaseSide = str2str(inElement.getAttribute("BaseSide"));
			m_ScalingValueFrontW = str2num<double>(inElement.getAttribute("ScalingValueFrontW"));
			m_ScalingValueFrontH = str2num<double>(inElement.getAttribute("ScalingValueFrontH"));
			m_ScalingValueBackW = str2num<double>(inElement.getAttribute("ScalingValueBackW"));
			m_ScalingValueBackH = str2num<double>(inElement.getAttribute("ScalingValueBackH"));
		}
		catch (...)
		{
			throw;
		}

	}

	/**
	 * @brief	デストラクタ
	*/
	virtual ~Scaling()
	{
	}

	/**
	 * @brief	変倍機能のON/OFFを取得する
	 * @return	変倍機能のON/OFF
	*/
	virtual std::pair<bool, bool> getScalingValue() const
	{
		return m_ScalingValue;
	}

	/**
	 * @brief	基準面を取得する
	 * @return	基準面
	*/
	virtual std::pair<bool, std::string> getBaseSide() const
	{
		return m_BaseSide;
	}

	/**
	 * @brief	変倍値(表:横)を取得する
	 * @return	変倍値(表:横)
	*/
	virtual std::pair<bool, double> getScalingValueFrontW() const
	{
		return m_ScalingValueFrontW;
	}

	/**
	 * @brief	変倍値(表:縦)を取得する
	 * @return	変倍値(表:縦)
	*/
	virtual std::pair<bool, double> getScalingValueFrontH() const
	{
		return m_ScalingValueFrontH;
	}

	/*
	 * @brief	変倍値(裏:横)を取得する
	 * @return	変倍値(裏:横)
	*/
	virtual std::pair<bool, double> getScalingValueBackW() const
	{
		return m_ScalingValueBackW;
	}

	/**
	 * @brief	変倍値(裏:縦)を取得する
	 * @return	変倍値(裏:縦)
	*/
	virtual std::pair<bool, double> getScalingValueBackH() const
	{
		return m_ScalingValueBackH;
	}

private:
	PrintConditionsParser::ILogger&		m_Logger;					//!< ログ出力用クラス
	std::pair<bool, bool>				m_ScalingValue;				//!< 変倍機能のON/OFF
	std::pair<bool, std::string>		m_BaseSide;					//!< 基準面
	std::pair<bool, double>				m_ScalingValueFrontW;		//!< 変倍値(表:横)
	std::pair<bool, double>				m_ScalingValueFrontH;		//!< 変倍値(表:縦)
	std::pair<bool, double>				m_ScalingValueBackW;		//!< 変倍値(裏:横)
	std::pair<bool, double>				m_ScalingValueBackH;		//!< 変倍値(裏:縦)

};


class PrintCondition : public IPrintCondition {
public:
	PrintCondition(
		PrintConditionsParser::ILogger&	inLogger,
		const XMLElement&				inElement,
		bool							inSkipped) : m_Logger(inLogger)
	{
		m_DescriptiveName = str2str(inElement.getAttribute("DescriptiveName"));
		m_PaperType = str2str(inElement.getAttribute("PaperType"));
		m_Alignment = str2str(inElement.getAttribute("Alignment"));
		m_PrintMode = str2str(inElement.getAttribute("PrintMode"));
		m_PrintSpeed = str2num<double>(inElement.getAttribute("PrintSpeed"));

		m_IsSkipped = inSkipped;

		/**
			@note
			各エレメントが存在しない場合でも、その他のエレメントは取得しておく
		*/
		try {
			const XMLElement* papersettingElem = inElement.getChild("PaperSetting");
			ThrowIfNULL(papersettingElem, "PaperSetting element is NULL")
			m_PaperSetting.reset(new PaperSetting(m_Logger, *papersettingElem));
		} catch (...) {
		}
		try {
			const XMLElement* dryerElem = inElement.getChild("DryerTemperature");
			ThrowIfNULL(dryerElem, "DryerTemperature element is NULL")
			m_DryerTemperature.reset(new DryerTemperature(m_Logger, *dryerElem));
		} catch (...) {
		}
		try {
			const XMLElement* iccElem = inElement.getChild("ICCPreset");
			ThrowIfNULL(iccElem, "ICCPreset element is NULL")
			m_ICCPreset.reset(new ICCPreset(m_Logger, *iccElem));
		} catch (...) {
		}
		try {
			m_ToneCurve.reset(new ToneCurve(m_Logger, inElement));
		} catch (...) {
		}
		try {
			const XMLElement* jobElem = inElement.getChild("JobBase");
			ThrowIfNULL(jobElem, "JobBase element is NULL")
			m_JobBase.reset(new JobBase(m_Logger, *jobElem));
		} catch (...) {
		}
		try {
			const XMLElement* layoutElem = inElement.getChild("Layout");
			ThrowIfNULL(layoutElem, "Layout element is NULL")
			m_Layout.reset(new Layout(m_Logger, *layoutElem));
		} catch (...) {
		}
		try {
			const XMLElement* inspectionElem = inElement.getChild("Inspection");
			ThrowIfNULL(inspectionElem, "Inspection element is NULL")
			m_Inspection.reset(new Inspection(m_Logger, *inspectionElem));
		} catch (...) {
		}
		try {
			const XMLElement* decodeElem = inElement.getChild("Decode");
			ThrowIfNULL(decodeElem, "Decode element is NULL")
			m_Decode.reset(new Decode(m_Logger, *decodeElem));
		} catch (...) {
		}

		try
		{
			const XMLElement* ScalingElem = inElement.getChild("Scaling");
			ThrowIfNULL(ScalingElem, "Scaling element is NULL")
			m_Scaling.reset(new Scaling(m_Logger, *ScalingElem));
		}
		catch (...)
		{
		}

	}

	/**
		印刷条件名を取得する
		@return 印刷条件名
	*/
	virtual std::pair<bool, std::string> getDescriptiveName() const { return m_DescriptiveName; }

	/**
		用紙種類名を取得する
		@return 用紙種類名
	*/
	virtual std::pair<bool, std::string> getPaperType() const { return m_PaperType; }

	/**
		段差補正名を取得する
		@return 段差補正名
	*/
	virtual std::pair<bool, std::string> getAlignment() const { return m_Alignment; }

	/**
		PaperSetting エレメント情報を取得する
		@return PaperSetting エレメント情報
	*/
	virtual const std::shared_ptr<IPaperSetting>& getPaperSetting() const { return m_PaperSetting; }

	/**
		印刷モード名を取得する
		@return 印刷モード名
	*/
	virtual std::pair<bool, std::string> getPrintMode() const { return m_PrintMode; }

	/**
		印刷速度を取得する
		@return	印刷速度値
	*/
	virtual std::pair<bool, double> getPrintSpeed() const { return m_PrintSpeed; }

	/**
		DryerTemperature エレメントを取得する
		@return DryerTemperature エレメント情報
	*/
	virtual const std::shared_ptr<IDryerTemperature>& getDryTemperature() const { return m_DryerTemperature; }

	/**
		ICCPreset エレメントを取得する
		@return ICCPreset エレメント情報
	*/
	virtual const std::shared_ptr<IICCPreset>& getICCPreset() const { return m_ICCPreset; }

	/**
		ToneCurve エレメントを取得する
		@return ToneCurve エレメント情報
	*/
	virtual const std::shared_ptr<IToneCurve>& getToneCurve() const { return m_ToneCurve; }

	/**
		JobBase エレメントを取得する
		@return JobBase エレメント情報
	*/
	virtual const std::shared_ptr<IJobBase>& getJobBase() const { return m_JobBase; }
		
	/**
		Layout エレメント情報を取得する
		@return Layout エレメント情報
	*/
	virtual const std::shared_ptr<ILayout>& getLayout() const { return m_Layout; }

	/**
		Inspection エレメントを取得する
		@return Inspection エレメント情報
	*/
	virtual const std::shared_ptr<IInspection>& getInspection() const { return m_Inspection; }

	/**
		Inspection エレメントを取得する
		@return Inspection エレメント情報
	*/
	virtual const std::shared_ptr<IDecode>& getDecode() const { return m_Decode; }

	/**
	 * @brief	Scalingエレメントを取得する
	 * @return	Scalingエレメント情報
	*/
	virtual const std::shared_ptr<IScaling>& getScaling() const
	{
		return m_Scaling;
	}

	virtual bool isImportSkipFlag() const { return m_IsSkipped; }

private:
	PrintConditionsParser::ILogger&	m_Logger;

	std::pair<bool, std::string> m_DescriptiveName;
	std::pair<bool, std::string> m_PaperType;
	std::pair<bool, std::string> m_Alignment;
	std::pair<bool, std::string> m_PrintMode;
	std::pair<bool, double> m_PrintSpeed;
	std::shared_ptr<IPaperSetting> m_PaperSetting;
	std::shared_ptr<IDryerTemperature> m_DryerTemperature;
	std::shared_ptr<IICCPreset> m_ICCPreset;
	std::shared_ptr<IToneCurve> m_ToneCurve;
	std::shared_ptr<IJobBase> m_JobBase;
	std::shared_ptr<ILayout> m_Layout;
	std::shared_ptr<IInspection> m_Inspection;
	std::shared_ptr<IDecode> m_Decode;
	std::shared_ptr<IScaling> m_Scaling;			//!< Scalingエレメント情報
	
	bool m_IsSkipped;	//!< 取り込みスキップ指示フラグ

};

} // namespace printconditionsparser

using namespace printconditionsparser;

class PrintConditionsParserPrivate {
public:
	PrintConditionsParserPrivate(
		PrintConditionsParser::ILogger&	inLogger,
		const std::string&				inPath) : m_Logger(inLogger)
	{
		auto thrower = [](
			const std::string&	inMessage,
			const std::string&	inPath)
		{
			std::stringstream ss;
			ss << inMessage << inPath;
			std::invalid_argument e(ss.str());
			throw e;
		};

		std::ifstream ifs(inPath);
		if (ifs) {
			ifs.seekg(0, ifs.end);
			size_t s = static_cast<size_t>(ifs.tellg());
			ifs.seekg(0, ifs.beg);

			std::unique_ptr<char[]> buf(new char[s]);
			ifs.read(buf.get(), s);
			ifs.close();

			XMLParser parser;
			std::unique_ptr<XMLElement> root(parser.analyze(buf.get(), s));

			if (root.get() == NULL) {
				thrower("cannot parse file: ",  inPath);
			}
			size_t num = root->getChildCount();
			std::vector<const XMLElement*> UniqueConditions;
			size_t UniqueConditionNum;
			bool isConditionSkipped;
			for (size_t i = 0; i < num; ++i) {
				const XMLElement* conditionElem = root->getChild(i);
				ThrowIfNULL(conditionElem, "Condition element is NULL")

				UniqueConditionNum = UniqueConditions.size();
				isConditionSkipped = false;
				if( UniqueConditionNum > 0 )
				{
					for (size_t j = 0; j < UniqueConditionNum; ++j) {
						if( *conditionElem == *(UniqueConditions[j] ) )
						{
							isConditionSkipped = true;
							break;
						}
					}
				}
				if( !isConditionSkipped )	UniqueConditions.push_back(conditionElem);
				
				PrintCondition condition(m_Logger, *conditionElem, isConditionSkipped);
				m_Conditions.push_back(condition);
			}

		} else {
			thrower("cannot read file: ", inPath);
		}
	}

	size_t getNum() { return m_Conditions.size(); }

	const IPrintCondition& getCondition(
		size_t	inIndex)
	{
		return m_Conditions.at(inIndex);
	}

private:
	std::vector<PrintCondition> m_Conditions;
	PrintConditionsParser::ILogger&	m_Logger;
};


PrintConditionsParser::PrintConditionsParser(
	PrintConditionsParser::ILogger&	inLogger) : m_Logger(inLogger)
{
}

bool PrintConditionsParser::read(
	const std::string&	inPath)
{
	try {
		m_Private.reset(new PrintConditionsParserPrivate(m_Logger, inPath));
	} catch (std::exception& e) {
		m_Logger.writeToDebugLog(e.what());
		return false;
	}
	return true;
}

size_t PrintConditionsParser::getNum()
{
	if (m_Private.get() == NULL) {
		return 0;
	}
	return m_Private->getNum();
}

const IPrintCondition& PrintConditionsParser::getCondition(
	size_t	inIndex)
{
	if (inIndex >= getNum()) {
		std::stringstream ss;
		ss << "invalid index:" << inIndex << ", max:" << getNum();
		m_Logger.writeToDebugLog(ss.str());
		std::invalid_argument e(ss.str());
		throw e;
	}
	return m_Private->getCondition(inIndex);
}

