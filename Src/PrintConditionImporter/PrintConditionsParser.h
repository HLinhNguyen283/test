/**
 * @file	PrintConditionsParser.h
 * @brief	PrintConditionsParser
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai new
 * @date	2022/04/27 cec.k-miyachi #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
**/

#pragma once

#include <string>
#include <vector>
#include <map>

namespace printconditionsparser {
	/**
		PrintCondition 情報。
		抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
	*/
	class IPrintCondition {
	public:
		virtual ~IPrintCondition() {}

		/**
			PrintCondition#PaperSetting 情報。
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IPaperSetting {
		public:
			virtual ~IPaperSetting() {}

			/**
				用紙幅を取得する
				@return 用紙幅 (pt)
			*/
			virtual std::pair<bool, double> getWidth() const = 0;

			/**
				用紙厚みを取得する
				@return 用紙厚み (μm)
			*/
			virtual std::pair<bool, long> getThickness() const = 0;
			
			/**
				テンションを取得する
				@return テンション
			*/
			virtual std::pair<bool, long> getTension() const = 0;

			/**
				DNSを取得する
				@return DNS
			*/
			virtual std::pair<bool, bool> getDNS() const = 0;

			/**
				パンチ穴有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasPunchHole() const = 0;
			
			/**
				ミシン目有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasSewing() const = 0;

			/**
				事前マーク有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasMarker() const = 0;

			/**
				事前印刷有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> isPrePrinted() const = 0;

			/**
				マークオフセットを取得する
				@return マークオフセット (pt)
			*/
			virtual std::pair<bool, double> getMarkerOffset() const = 0;

			/**
				ページ送りサイズを取得する
				@return ページ送りサイズ (pt)
			*/
			virtual std::pair<bool, double> getPageFeedSize() const = 0;

			enum EPageFeedUnit {
				kPageFeedUnit_mm, kPageFeedUnit_inch, kPageFeedUnit_inch2, kPageFeedUnit_inch6, kPageFeedUnit_Page, kPageFeedUnit_Empty,
			};
			/**
				ページ送りサイズの単位を取得する
				@return ページ送りサイズ単位
			*/
			virtual std::pair<bool, EPageFeedUnit> getPageFeedUnit() const = 0;

			enum EMarkSide {
				kMarkSide_Front, kMarkSide_Back, kMarkSide_Empty,
			};
			/**
				マーク印刷面を取得する
				@return マーク印刷面
			*/
			virtual std::pair<bool, EMarkSide> getMarkSide() const = 0;

			/**
				ミシン目パターンの繰り返し数を取得する
				@return 繰り返し数
			*/
			virtual std::pair<bool, long> getRepeatSheets() const = 0;
		};

		/**
			PrintCondition#DryerTemperature 情報
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IDryerTemperature {
		public:
			virtual ~IDryerTemperature(){}

			/**
				ヒートローラー乾燥温度を取得する
				@param[in] inIsSelf : 自機なら true、他機なら false
				@return 乾燥温度
			*/
			virtual std::pair<bool, long> getHeatRoller(
				bool	inIsSelf = true) const = 0;

			/**
				温風アシスト乾燥温度を取得する
				@param[in] inIsSelf : 自機なら true、他機なら false
				@return 乾燥温度
			*/
			virtual std::pair<bool, long> getHeatFan(
				bool	inIsSelf = true) const = 0;

			/**
				IR設定を取得する
				@param[in] inSelf : 自機なら true、他機なら false
				@return IR設定
			*/
			virtual std::pair<bool, long> getNIRPower(
				bool	inSelf = true) const = 0;

			/**
				先塗り乾燥温度を取得する
				@param[in] inIndex : 1 or 2
				@return 乾燥温度
			*/
			virtual std::pair<bool, long> getPrecoater(
				int	inIndex = 1) const = 0;

			/**
				サブヒートローラー乾燥温度を取得する
				@param[in] inIsSelf : 自機なら true、他機なら false
				@return 乾燥温度
			*/
			virtual std::pair<bool, long> getSubHeatRoller(
				bool	inIsSelf = true) const = 0;
		};

		/**
			PrintCondition#ICCPreset 情報
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IICCPreset {
		public:
			virtual ~IICCPreset() {}

			/**
				ICC プリセット名を取得する
				@return プリセット名
			*/
			virtual std::pair<bool, std::string> getDescriptiveName() const = 0;

			/**
				ICC プリセット ID を取得する
				@return プリセット ID
			*/
			virtual std::pair<bool, std::string> getID() const = 0;
		};

		/**
			PrintCondition#ToneCurve 情報
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IToneCurve {
		public:
			virtual ~IToneCurve(){}

			/**
				トーンカーブ名（GUI 表示名）を取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return トーンカーブ名
			*/
			virtual std::pair<bool, std::string> getDescriptiveName(
				bool	inIsFront) const = 0;

			enum EColor {
				kToneCurve_Color_C, kToneCurve_Color_M, kToneCurve_Color_Y, kToneCurve_Color_K, kToneCurve_ColorNum,
			};
			/**
				アンカーポイント情報を取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@param[in] inColor : 取得したい色
				@param[in] inPointID : 取得したいポイント ID (0 - 4)
				@param[out] outBeforeConPoint : 基準濃度値
				@param[out] outAfterConPoint : トーンカーブデータの数値
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
				std::pair<bool, long>&	outAnchorPointType) const = 0;
		};

		/**
			PrintCondition#JobBase 情報
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IJobBase {
		public:
			virtual ~IJobBase(){}

			/**
				インフォメーションシートの出力有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasInformationSheet() const = 0;

			/**
				フラッシングページの挿入有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasFlushingPage() const = 0;
			
			/**
				フラッシングページのパターン名を取得する
				@return パターン名
			*/
			virtual std::pair<bool, std::string> getPatternName() const = 0;

			/**
				フラッシングページの挿入間隔を取得する
				@return 挿入間隔
			*/
			virtual std::pair<bool, long> getInterval() const = 0;

			/**
				ジョブ前に挿入するかどうかを取得する
				@return true : する
				@return false : しない 
			*/
			virtual std::pair<bool, bool> isPreJob() const = 0;

			/**
				ジョブ前に挿入する回数を取得する
				@return 挿入回数
			*/
			virtual std::pair<bool, long> getPreJobCount() const = 0;

			/**
				ジョブ後に挿入するかどうかを取得する
				@return true : する
				@return false : しない 
			*/
			virtual std::pair<bool, bool> isPostJob() const = 0;

			/**
				フラッシングページのオフセットを取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return オフセット
			*/
			virtual std::pair<bool, long> getOffset(
				bool	inIsFront = true) const = 0;

			/**
				@brief	ジョブ前のクリーニング判定レベルを取得する
				@retval	ジョブ前のクリーニング判定レベル
			*/
			virtual std::pair<bool, long> getCleaningJudgmentLevelBeforeJob() const = 0;

			/**
				@brief	ジョブ後のクリーニング判定レベルを取得する
				@retval	ジョブ後のクリーニング判定レベル
			*/
			virtual std::pair<bool, long> getCleaningJudgmentLevelAfterJob() const = 0;

			/**
				@brief	印刷停止設定を取得する
				@retval	0 : 停止する
				@retval	1 : 停止しない
			*/
			virtual std::pair<bool, std::string> getPrintStopSetting() const = 0;
		};

		/**
			PrintCondition#Layout 情報
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class ILayout {
		public:
			virtual ~ILayout(){}

			/**
				オフセットを取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@param[out] outX : 主走査方向オフセット値
				@param[out] outY : 副走査方向オフセット値
			*/
			virtual void getOffset(
				bool	inIsFront,
				std::pair<bool, long>&	outX,
				std::pair<bool, long>&	outY) const = 0;
			
			/**
				サイド検査の有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> isVerification() const = 0;

			/**
				エラーにする連続シート数を取得する
				@return シート数
			*/
			virtual std::pair<bool, long> getContinuousPageCount() const = 0;

			/**
				印刷開始マークの出力の有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> hasCueMark() const = 0;

			/**
				バーコード出力の有無を取得する
				@return true : 有
				@return false : 無
			*/
			virtual std::pair<bool, bool> isBarcode() const = 0;

			/**
				バーコードのオフセット値を取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@param[out] outX : 主走査方向オフセット値
				@param[out] outY : 副走査方向オフセット値
			*/
			virtual void getBarcodeOffset(
				bool	inIsFront,
				std::pair<bool, long>&	outX,
				std::pair<bool, long>&	outY) const = 0;

			/**
				フラッシングパターンの設定を取得する
				@return フラッシングパターン設定値
			*/
			virtual std::pair<bool, std::string> getFlushingPattern() const = 0;

			/**
				ラインフラッシングのずらし量を取得する
				@return ずらし量
			*/
			virtual std::pair<bool, long> getGapBetweenColors() const = 0;

			/**
				ラインフラッシングのオフセット値を取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return オフセット値
			*/
			virtual std::pair<bool, long> getLineOffset(
				bool	inIsFront) const = 0;

			/**
				ページ内、ラインフラッシングを取得する
				@return ページ内、ラインフラッシング
			*/
			virtual std::pair<bool, bool> getLayoutLineGap() const = 0;
			
			/**
				ページ内、ラインフラッシング間隔（副走査）
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return ページ内、ラインフラッシング間隔（副走査）
			*/
			virtual std::pair<bool, long> getLayoutLineGap(
				bool	inIsFront) const = 0;

			/**
				ページ内、ラインフラッシングの補正オフセット（副走査）
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return ページ内、ラインフラッシングの補正オフセット（副走査）
			*/
			virtual std::pair<bool, long> getLayoutLineGapOffset(
				bool	inIsFront) const = 0;

			/**
				Bottom指定時、ラインフラッシングの下端配置および後処理マークの下端配置がON
				@return ページ内、ラインフラッシングの補正オフセット（副走査）
			*/
			virtual std::pair<bool, std::string> getLayoutLinePosotion() const = 0;

			/**
				ラインフラッシングのオプション情報(ST_PDB_LAYOUT_IMAGE_INFOのST_JOB_FLUSHING_PATTERN_ADD情報)
				@return ページ内、ラインフラッシングの補正オフセット（副走査）
			*/
			virtual std::pair<bool, std::string> getLayoutLineOptions() const = 0;

			enum ECutterMarkSide {
				kCutterMarkSide_Off, kCutterMarkSide_Front, kCutterMarkSide_Back, kCutterMarkSide_Both,
			};
			/**
				カッターマークの印刷面を取得する
			*/
			virtual std::pair<bool, ECutterMarkSide> getCutterMark() const = 0;

			/**
				カッターマークのサイズを取得する
				@param[out] outX : 主走査
				@param[out] outY : 副走査
			*/
			virtual void getCutterMarkSize(
				std::pair<bool, long>&	outX,
				std::pair<bool, long>&	outY) const = 0;

			/**
				カッターマークのオフセットを取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return オフセット
			*/
			virtual std::pair<bool, long> getCutterMarkOffset(
				bool	inIsFront = true) const = 0;

			
			enum ECutterMarkAlign {
				kCutterMarkAlign_Left = -1, kCutterMarkAlign_Center, kCutterMarkAlign_Right,
			};
			/**
				カッターマークの基準位置を取得する
				@param[in] inIsFront : 表面なら true、裏面なら false
				@return 基準位置
			*/
			virtual std::pair<bool, ECutterMarkAlign> getCutterMarkAlign(
				bool	inIsFront = true) const = 0;

			/**
				カッターマークの準備シート数を取得する
				@return 準備シート数
			*/
			virtual std::pair<bool, long> getCutterMarkPageCount() const = 0;

			// ADD --> #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）
			virtual std::pair<bool, bool> getAdjustmentSheetSizeEnabled() const = 0;
			virtual std::pair<bool, long> getAdjustmentSheetLength() const = 0;
			virtual std::pair<bool, long> getAdjustmentSheetAfterJob() const = 0;
			virtual std::pair<bool, long> getAfterJobSheets() const = 0;
			// ADD <-- #9545: 印刷条件のインポート／エクスポート（自動ダイバート設定）

		};

		/**
			PrintCondition#scr:VerificationParams 情報クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IVerificationParams {
		public:
			virtual ~IVerificationParams() {}

			/**
				対象面（表 or 裏）を取得する
				@return 対象面（表 or 裏）
			*/
			virtual std::pair<bool, std::string> getSide() const = 0;

			/**
				属性リストを取得する
				@return 属性リスト
			*/
			virtual std::map<std::string, std::pair<bool, std::string> > getScrAttributes() const = 0;

			/**
				scr:AIAssistParams属性リストを取得する
				@return 属性リスト
			*/
			virtual std::map<std::string, std::pair<bool, std::string> > getScrAIAssistAttributes() const = 0;

		};

		/**
			PrintCondition#scr:TemporalMask 情報クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class ITemporalMask {
		public:
			/**
				一時マスクレコード クラス
			*/
			class ITemporalMaskRecord{
			public:
				//デストラクタ
				virtual ~ITemporalMaskRecord() {};

				/**
					欠点種類（汚れ or スジ汚れ or 白スジ or 欠落）を取得する
					@return 欠点種類
				*/
				virtual std::pair<bool, std::string> getType() const = 0;

				/**
					欠点位置を取得する
					@return 欠点位置
				*/
				virtual std::vector<std::pair<bool, double> > getPos() const = 0;

				/**
					欠点サイズを取得する
					@return 欠点サイズ
				*/
				virtual std::vector<std::pair<bool, double> > getSize() const = 0;

				/**
					一時マスクサイズを取得する
					@return 一時マスクサイズ
				*/
				virtual std::vector<std::pair<bool, double> > getTemporalMaskSize() const = 0;
			};
			virtual ~ITemporalMask() {}

			/**
				対象面（表 or 裏）を取得する
				@return 対象面（表 or 裏）
			*/
			virtual std::pair<bool, std::string> getSide() const = 0;

			/**
				一時マスク個数を取得する
				@return 一時マスク個数
			*/
			virtual std::pair<bool, long> getSize() const = 0;

			/**
				一時マスクレコードリストを取得する
				@return 一時マスクレコードリスト
			*/
			virtual std::vector<std::shared_ptr<ITemporalMaskRecord> > getTemporalMaskRecord()  const = 0;
		};

		/**
			PrintCondition#scr:InspectionArea 情報 クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IInspectionArea {
		public:
			/**
				非検査矩形エリア情報 クラス
				抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
			*/
			class INonInspectionRect{
			public:
				virtual ~INonInspectionRect() {};

				/**
					非検査矩形エリアIDを取得する
					@return 非検査矩形エリアID
				*/
				virtual std::pair<bool, std::string> getID()  const = 0;

				/**
					非検査矩形エリア始点情報を取得する
					@return 非検査矩形エリア始点情報
				*/
				virtual std::vector<std::pair<bool, long> > getStartPos()  const = 0;

				/**
					非検査矩形エリア終点情報を取得する
					@return 非検査矩形エリア終点情報
				*/
				virtual std::vector<std::pair<bool, long> > getEndPos()  const = 0;
			};		

			virtual ~IInspectionArea() {};

			/**
				対象面（表 or 裏）を取得する
				@return 対象面（表 or 裏）
			*/
			virtual std::pair<bool, std::string> getSide()  const = 0;

			/**
				非検査エリア(紙端)左端 を取得する
				@return 非検査エリア左端
			*/
			virtual std::pair<bool, double> getNonInspectionAreaLeft()  const = 0;

			/**
				非検査エリア(紙端)右端 を取得する
				@return 非検査エリア右端
			*/
			virtual std::pair<bool, double> getNonInspectionAreaRight()  const = 0;

			/**
				非検査エリア(矩形) を取得する
				@return 非検査エリア右端
			*/
			virtual std::vector<std::shared_ptr<INonInspectionRect> > getNonInspectionRect()  const = 0;

		};

		/**
			PrintCondition#DecodeDetails 情報クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IDecodeDetails {
		public:
			virtual ~IDecodeDetails(){}

			/**
				対象面（表 or 裏）を取得する
				@return 対象面（表 or 裏）
			*/
			virtual std::pair<bool, std::string>		 getSide()  const = 0;

			/**
				サイズ設定を取得する
				@return サイズ設定
			*/
			virtual std::vector<std::pair<bool, double> >	 getSize()  const = 0;

			/**
				位置設定を取得する
				@return 位置設定
			*/
			virtual std::vector<std::pair<bool, double> >	 getOffset()  const = 0;

			/**
				ラベル名称を取得する
				@return ラベル名称
			*/
			virtual std::pair<bool, std::string>		 getName()  const = 0;

			/**
				ラベル種類を取得する
				@return ラベル種類
			*/
			virtual std::pair<bool, std::string>		 getType()  const = 0;

			/**
				（OCR限定）角度情報を取得する
				@return （OCR限定）角度情報
			*/
			virtual std::pair<bool, std::string>		 getAngle()  const = 0;

			/**
				（デコードエラー発生時、エラー停止判定カウントの対象にするか？を取得する
				@return （デコードエラー発生時、エラー停止判定カウントの対象にするか？
			*/
			virtual std::pair<bool, bool>				 getErrorStop()  const = 0;

			/// @remark DetailInfoのような型を使用するとインタフェース定義の意義が薄れる
		};

		/**
			PrintCondition#Decode 情報クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IDecode {
		public:
			virtual ~IDecode(){}

			/**
				エラー停止連続シート数を取得する
				@return エラー停止連続シート数
			*/
			virtual std::pair<bool, long> getErrorSheets()  const = 0;

			/**
				数値単位系を取得する
				@return 数値単位系
			*/
			virtual std::pair<bool, std::string> getUnit()  const = 0;

			/**
				デコード処理の有効／無効を取得する
				@return 数値単位系
			*/
			virtual std::pair<bool, bool> getIsEnabled()  const = 0;

			/**
				Decode詳細 エレメントを取得する
				@param[in] inIndex : 取得したいインデクス(0-)
				@param[out] outSide,	 : 対象面（表 or 裏）Front / Back
				@param[out] outSize,	 : サイズ設定
				@param[out] outOffset,	 : 位置設定
				@param[out] outName,	 : ラベル名称
				@param[out] outType,	 : ラベル種類
				@param[out] outAngle,	 : （OCR限定）角度情報
				@param[out] outErrorStop : デコードエラー発生時、エラー停止判定カウントの対象にするか？
			*/
			virtual bool getDetail(
				size_t		inIndex,
				std::pair<bool, std::string>&			outSide,	
				std::vector<std::pair<bool, double> >&	outSize,	
				std::vector<std::pair<bool, double> >&	outOffset,	
				std::pair<bool, std::string>&			outName,	
				std::pair<bool, std::string>&			outType,	
				std::pair<bool, std::string>&			outAngle,	
				std::pair<bool, bool>&					outErrorStop) const = 0;
		};

		/**
			PrintCondition#Inspection 情報 クラス
			抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IInspection {
		public:
			virtual ~IInspection(){}
			/**
				検査条件名エレメントを取得する
				@return 検査条件名
			*/
			virtual std::pair<bool, std::string> getParamaterName() const = 0;

			/**
				IVerificationParams エレメントを取得する
				@return IVerificationParams エレメント情報
			*/
			virtual const std::vector<std::shared_ptr<IPrintCondition::IVerificationParams> >& getVerificationParams() const = 0;

			/**
				一時マスクファイルパスエレメントを取得する
				@return 一時マスクファイルパス
			*/
			virtual std::pair<bool, std::string> getTemporalMaskFilePath() const = 0;

			/**
				ITemporalMask エレメントを取得する
				@return ITemporalMask エレメント情報
			*/
			virtual const std::vector<std::shared_ptr<IPrintCondition::ITemporalMask> >& getTemporalMask() const = 0;

			/**
				IInspectionArea エレメントを取得する
				@return IInspectionArea エレメント情報
			*/
			virtual const std::vector<std::shared_ptr<IPrintCondition::IInspectionArea> >& getInspectionArea() const = 0;
		
		};

		/**
		 * @brief	PrintCondition#Scaling 情報クラス
		 * @note	抽象クラスとして、getter I/F のみを公開しておき、実体は隠蔽化
		*/
		class IScaling {
		public:
			virtual ~IScaling(){}

			/**
			 * @brief	変倍機能のON/OFFを取得する
			 * @return	変倍機能のON/OFF
			*/
			virtual std::pair<bool, bool> getScalingValue() const = 0;

			/**
			 * @brief	基準面を取得する
			 * @return	基準面
			*/
			virtual std::pair<bool, std::string> getBaseSide() const = 0;

			/**
			 * @brief	変倍値(表:横)を取得する
			 * @return	変倍値(表:横)
			*/
			virtual std::pair<bool, double> getScalingValueFrontW() const = 0;

			/**
			 * @brief	変倍値(表:縦)を取得する
			 * @return	変倍値(表:縦)
			*/
			virtual std::pair<bool, double> getScalingValueFrontH() const = 0;

			/*
			 * @brief	変倍値(裏:横)を取得する
			 * @return	変倍値(裏:横)
			*/
			virtual std::pair<bool, double> getScalingValueBackW() const = 0;

			/**
			 * @brief	変倍値(裏:縦)を取得する
			 * @return	変倍値(裏:縦)
			*/
			virtual std::pair<bool, double> getScalingValueBackH() const = 0;
		};

		/**
			印刷条件名を取得する
			@return 印刷条件名
		*/
		virtual std::pair<bool, std::string> getDescriptiveName() const = 0;

		/**
			用紙種類名を取得する
			@return 用紙種類名
		*/
		virtual std::pair<bool, std::string> getPaperType() const = 0;

		/**
			段差補正名を取得する
			@return 段差補正名
		*/
		virtual std::pair<bool, std::string> getAlignment() const = 0;

		/**
			PaperSetting エレメント情報を取得する
			@return PaperSetting エレメント情報
		*/
		virtual const std::shared_ptr<IPaperSetting>& getPaperSetting() const = 0;

		/**
			印刷モード名を取得する
			@return 印刷モード名
		*/
		virtual std::pair<bool, std::string> getPrintMode() const = 0;

		/**
			印刷速度を取得する
			@return	印刷速度値
		*/
		virtual std::pair<bool, double> getPrintSpeed() const = 0;

		/**
			DryerTemperature エレメントを取得する
			@return DryerTemperature エレメント情報
		*/
		virtual const std::shared_ptr<IDryerTemperature>& getDryTemperature() const = 0;

		/**
			ICCPreset エレメントを取得する
			@return ICCPreset エレメント情報
		*/
		virtual const std::shared_ptr<IICCPreset>& getICCPreset() const = 0;

		/**
			ToneCurve エレメントを取得する
			@return ToneCurve エレメント情報
		*/
		virtual const std::shared_ptr<IToneCurve>& getToneCurve() const = 0;

		/**
			JobBase エレメントを取得する
			@return JobBase エレメント情報
		*/
		virtual const std::shared_ptr<IJobBase>& getJobBase() const = 0;
		
		/**
			Layout エレメント情報を取得する
			@return Layout エレメント情報
		*/
		virtual const std::shared_ptr<ILayout>& getLayout() const = 0;

		/**
			Inspection エレメント情報を取得する
			@return Inspection エレメント情報
		*/
		virtual const std::shared_ptr<IInspection>& getInspection() const = 0;

		/**
			Decode エレメントを取得する
			@return Decode エレメント情報
		*/
		virtual const std::shared_ptr<IDecode>& getDecode() const = 0;


		/*
		 * @brief	Scalingエレメント情報を取得する
		 * @return	Scalingエレメント情報
		*/
		virtual const std::shared_ptr<IScaling>& getScaling() const = 0;

		virtual bool isImportSkipFlag() const = 0;
	};


}; // namespace printconditionsparser


class PrintConditionsParserPrivate;


/**
	PrintConditions.xml パーサークラス本体
*/
class PrintConditionsParser {
public:

	/**
		ログ出力用抽象クラス
	*/
	class ILogger {
	public:
		virtual ~ILogger() {}

		/**
			指定メッセージをデバッグログとして出力する
			@param[in] inMessage : メッセージ
		*/
		virtual void writeToDebugLog(
			const std::string&	inMessage) = 0;
	};

	PrintConditionsParser(
		ILogger&	inLogger);

	virtual ~PrintConditionsParser() {}

	/**
		PrintConditions.xml ファイル読み込み関数。
		@param[in] inPath : 対象ファイルへのパス
		@return true : 読み込み成功
		@return false : 読み込み失敗
	*/
	bool read(
		const std::string&	inPath);

	/**
		PrintCondition の個数を取得する
		@return 個数
	*/
	size_t getNum();

	/**
		指定インデックスの PrintCondition エレメント情報を取得する
		@param[in] inIndex : インデックス
		@return PrintCondition エレメント情報
		@throw inIndex >= getNum() の場合、std::invalid_argument を送出する
	*/
	const printconditionsparser::IPrintCondition& getCondition(
		size_t	inIndex);


private:
	std::shared_ptr<PrintConditionsParserPrivate> m_Private;
	ILogger&	m_Logger;
};

