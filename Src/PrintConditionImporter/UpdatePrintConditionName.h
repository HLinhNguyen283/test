/**
 * @file	UpdatePrintConditionName.h
 * @brief	印刷条件名を追加/上書きする
 * @author	pt.ta-mukai
 * @date	2019/02/05 pt.ta-mukai 新規作成
**/

#pragma once

//warning C4290: C++ の例外の指定は無視されます。関数が __declspec(nothrow) でないことのみ表示されます。 を抑制
#pragma warning( disable : 4290 )

#include "CommonDef_OP.h"
#include "PaperDB_MigrationDef.h"
#include <vector>

class CDataSystemSetting;

/**
 * @brief 印刷条件名を追加/上書きする
**/
class CUpdatePrintConditionName
{
public:
	/**
	 * @brief	constructor
	 * @param[in]	inDataSystemSetting : システムのデータ
	**/
	CUpdatePrintConditionName(CDataSystemSetting& inDataSystemSetting, ST_PDB_MIGRATION_DATA* inMigrationData, std::pair<bool,bool>& inHasElements) throw();

	/**
	 * @brief	destructor
	**/
	virtual ~CUpdatePrintConditionName() throw();

	/**
	 * @brief	指定の印刷条件名を新規に追加する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @note	エラー発生時は例外を投げる
	**/
	void Add(const std::string& inPrintConditionName) throw (int);

	/**
	 * @brief	引き継ぎが必要な設定を複製する
	 * @param[out]	outFileList : 複製したファイル名をリストに追加する(フルパス)
	 * @note	エラー発生時は例外を投げる
	**/
	void CopyNecessaryFiles(std::vector<std::string>& outFileList) throw(int);

	/**
	 * @brief	上位クラスで持っている用紙上のパンチ有無情報の設定
	 * @note	この情報に基づいて「非検査エリア(紙端)のデフォルト値」が切り替わる
	**/
	void SetPunchedPaperFlag(bool isPunched);

	/**
	 * @brief	印刷条件追加/上書前に別途設定が必要な項目に対して処理を行う
	**/
	void preProc();

protected:
	//! 複製したファイル名
	typedef struct _ST_COPIED_FILE_NAMES{
		std::string JIPhotoOnJobPrintSelf;		//!< ジョブ印刷時のJI撮影画像ファイル名(自機)
		std::string JIPhotoOnJobPrintPart;		//!< ジョブ印刷時のJI撮影画像ファイル名(相手機)

		// constructor
		_ST_COPIED_FILE_NAMES() :
			JIPhotoOnJobPrintSelf(""),
			JIPhotoOnJobPrintPart("")
		{
		}
	} ST_COPIED_FILE_NAMES;


	// デバッグログ出力時にstringstreamを使えるようにする
	#define WRITE_TO_LOG_BUF(inType, inString)	std::stringstream log; log << inString; WriteToLogBuf(inType, log.str());


	/**
	 * @brief	一時マスクファイルを複製する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[out]	outFileList : 複製したファイル名をリストに追加する(フルパス)
	 * @retval	複製した一時マスクファイル名
	 * @note	エラー発生時は例外を投げる
	**/
	std::string copyTemporalMaskFile(const std::string& inPrintConditionName, std::vector<std::string>& outFileList) throw(int);

	/**
	 * @brief	ジョブ印刷時のJI撮影画像ファイルを複製する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inSystemType : システムタイプ
	 * @param[out]	outFileList : 複製したファイル名をリストに追加する(フルパス)
	 * @retval	複製したジョブ印刷時のJI撮影画像ファイル名
	 * @note	エラー発生時は例外を投げる
	**/
	std::string copyJIPhotoOnJobPrint(const std::string& inPrintConditionName, eComSystemType inSystemType, std::vector<std::string>& outFileList) throw(int);

	/**
	 * @brief	指定の印刷条件名を新規に追加する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @note	エラー発生時は例外を投げる
	**/
	void addPrintConditionName(const std::string& inPrintConditionName) throw(int);

	/**
	 * @brief	JI系の情報の引き継ぎが必要かどうか
	 * @return true:引継ぎが必要
	**/
	bool isNeededJiTakeOver();
	
	/**
	 * @brief	印刷条件追加時に別途設定が必要な項目に対して処理を行う
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void additionalProcOnAdd(const std::string& inPrintConditionName) throw();

	/**
	 * @brief	スキャン画像ファイル名の追加時の別途設定をする
	 * @param[in]	inPrintConditionName : 印刷条件名
	**/
	void additionalProcForScanFileName(const std::string& inPrintConditionName) const throw();

	/**
	 * @brief	非検査エリア(紙端)の追加時の別途設定をする 
	**/
	void additionalProcForNonInspectionAreaTable() const throw();

	/**
	 * @brief	非検査エリア(矩形)の追加時の別途設定をする
	**/
	void additionalProcForNonInspectionAreaList() const throw();

	/**
	 * @brief	レイアウト画面設定のデコード情報の追加時の別途設定をする
	**/
	void additionalProcForLayoutImageParameterDecodeInfo() const throw();

	/**
	 * @brief	一時マスクファイル名の追加時の別途設定をする
	 * @param[in]	inCopiedFileName : 複製したファイル名
	**/
	void additionalProcForTemporalMaskFileName(const std::string& inCopyFileName) throw();

	/**
	 * @brief	ジョブ印刷時のJI撮影画像ファイルの追加時の別途設定をする
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[in]	inCopiedFileNameSelf : 複製したファイル名(自機)
	 * @param[in]	inCopiedFileNamePart : 複製したファイル名(相手機)
	**/
	void additionalProcForJIPhotoOnJobPrint(const std::string& inPrintConditionName, const std::string& inCopyFileNameSelf, const std::string& inCopyFileNamePart) const throw();

	/**
	 * @brief	エラーコードの文字列を取得する
	 * @param[in]	inErrID : エラーコード
	 * @retval	エラーコードの文字列
	**/
	const std::string getLastErrorString(DWORD inErrID) throw();

	/**
	 * @brief	紙DBの項目に紐づいたファイル名を取得する
	 * @param[in]	inPrintConditionName : 印刷条件名
	 * @param[out]	outDeleteFiles : 削除するファイル名
	**/
	void getDeleteFiles(const std::string& inPrintConditionName, std::vector<const std::string>& outDeleteFiles);

	/**
	 * @brief	ジョブ印刷時のJI撮影画像ファイルのパスを取得する
	 * @param[in]	inSystemType : システムタイプ
	**/
	const std::string getJIPhotoOnJobPrintPath(eComSystemType inSystemType);


	CDataSystemSetting& m_DataSystemSetting;	//!< システムのデータ
	bool m_DED;									//!< DEDかどうか(true:DED)
	bool m_IsRegisterJI;						//!< JI系の情報の登録が必要かどうか(true:必要)
	std::string m_BasePrintConditionName;		//!< インポート時のベースとなる印刷条件名
	ST_COPIED_FILE_NAMES m_CopiedFileNames;		//!< 複製したファイル名
	bool m_IsPunchedPaper;						//!< XML用紙情報上でのパンチ有無情報
	ST_PDB_MIGRATION_DATA* m_DataForPDB;		//!< 業務移行構造体
	bool m_hasInspectionElement;				//!< Inspectionエレメントが存在するかどうか
	bool m_hasDecodeElement;					//!< Decodeエレメントが存在するかどうか

	class API;
};

