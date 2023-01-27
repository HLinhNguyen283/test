/**
 * @file	ConvertMD5Hash.cpp
 * @brief	MD5ハッシュ値変換クラス
 * @author	pt.ju-tanaka
 * @date	2019/2/5 pt.ju-tanaka 新規作成
**/

// ==========================================================================
// Include files
#include "stdafx.h"
#include "ConvertMD5Hash.h"
#include "Wincrypt.h"
#include <sstream>
#include <iomanip>
#include "CommonCBRProc.h"

// ==========================================================================
bool CConvertMD5Hash::m_isOutputDebugFile = false;	//!< デバッグファイル出力をするかどうか(true:する, false:しない)
// ==========================================================================

CConvertMD5Hash::CConvertMD5Hash()
{
}

CConvertMD5Hash::~CConvertMD5Hash()
{
}

void CConvertMD5Hash::SetOutputDebugFile(bool inIsOutput)
{
	m_isOutputDebugFile = inIsOutput;
}

bool CConvertMD5Hash::CreateHash(const BYTE* inData, DWORD inDataSize, std::string& outHashedData)
{
	bool res = false;
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	DWORD errNo = 0;
	try{
		// キーコンテナの準備.
		if(CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE){
			errNo = GetLastError();
			throw std::runtime_error("CryptAcquireContext");
		}

		// MD5ハッシュ計算のインスタンスを生成.
		if(CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE){
			errNo = GetLastError();
			throw std::runtime_error("CryptCreateHash");
		}

		// MD5ハッシュ値を計算.
		if(CryptHashData(hHash, inData, inDataSize, 0) == FALSE){
			errNo = GetLastError();
			throw std::runtime_error("CryptHashData");
		}

		// MD5ハッシュ値を取得.
		{
			// ハッシュサイズを取得.
			DWORD hashLen = 0;
			if(CryptGetHashParam(hHash, HP_HASHVAL, 0, &hashLen, 0) == FALSE){
				errNo = GetLastError();
				throw std::runtime_error("CryptGetHashParam:get hash size");
			}
			// ハッシュを入れるメモリを用意.
			BYTE* hash = (BYTE*)IO_malloc(hashLen + 1);
			if(hash == NULL){
				throw std::runtime_error("CryptGetHashParam:malloc");
			}
			memset(hash, 0, hashLen+1);

			// ハッシュを取得.
			if(CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0) == FALSE){
				errNo = GetLastError();
				IO_free(hash);
				throw std::runtime_error("CryptGetHashParam:get hash");
			}

			// 文字列に変換.
			std::stringstream ss;
			ss << std::hex;
			for(DWORD i = 0; i < hashLen; ++i){
				ss << std::setw( 2 ) << std::setfill( '0' ) << (int)hash[i];
			}
			outHashedData = ss.str();
			IO_free(hash);
		}
		res = true;

	}catch(std::exception& e){
		OutputCreateHashFailedLog(e.what(), errNo);
	}

	//後始末.
	if(hHash != NULL){
		if(CryptDestroyHash(hHash) == FALSE){
			DWORD errNo = GetLastError();
			OutputCreateHashFailedLog("CryptDestroyHash", errNo);
		}
	}

	if(hProv != NULL) {
		if(CryptReleaseContext(hProv, 0) == FALSE) {
			DWORD errNo = GetLastError();
			OutputCreateHashFailedLog("CryptReleaseContext", errNo);
		}
	}

	if(m_isOutputDebugFile){
		OutputDebugFile(inData, inDataSize);
	}
	return res;
}

void CConvertMD5Hash::OutputCreateHashFailedLog(const std::string& inMsg, DWORD inErrNo)
{
	std::stringstream log;
	log << "Failed CreateHash::" << inMsg;
	if(inErrNo != 0){
		LPVOID msgBuf = GetLastErrorString(inErrNo);
		if(msgBuf != NULL){
			log << "[ErrNo: "<< inErrNo << "]"<<(LPTSTR)msgBuf;
			LocalFree(msgBuf);
		}
	}
	OutputDebugLog(log.str());
}

void CConvertMD5Hash::OutputDebugLog(const std::string& inMsg)
{
	WriteToLogBuf(LOG_DEBUG, const_cast<char*>(inMsg.c_str()));
}

void CConvertMD5Hash::OutputDebugFile(const BYTE* inData, DWORD inDataSize)
{
	static DWORD st_count = 0;
	++st_count;

	std::stringstream path;
	path << GetModuleDirectory() << "Log\\md5_" << st_count << ".dat";
	FILE* fp = IO_fopen(path.str().c_str(), "wb");
	if(fp){
		IO_fwrite(inData, inDataSize, 1, fp);
		IO_fclose(fp);
	}
}