#ifndef _FILE_LAYER_
#define _FILE_LAYER_

#include "Common.h"
#include "FileLayerStruct.h"

#include <iostream>
#include <iomanip>

#include "cryptlib.h"
#include "modes.h"
#include "aes.h"
#include "filters.h"
#include "base64.h"

class CFileLayer
{
protected:
	ST_FILE_LAYER			m_stFileLayer;
	ST_ENC_KEY				m_stEncKey;

	ST_FILE_ATTR			m_stFileAttr;

	bool isBase64(unsigned char c);

public:
	CFileLayer();
	~CFileLayer();

	virtual DWORD ReadRequestedFile(std::string &refstrInputFileName) { return 1; }
	virtual DWORD ExecuteCryptoOperation() { return 1; }
	virtual DWORD WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName) { return 1; }
};

class CEncrypt : public CFileLayer
{
	VOID EncryptHeader();
	VOID EncryptByAES(std::string &refstrEncode, ST_BLOCK_ENC_DATA &refstBlockEncData);
	std::string EncodeByBase64(unsigned char const* bytes_to_encode, unsigned int in_len);

public:
	CEncrypt();

	inline VOID SetFileAttr(ST_FILE_ATTR &refstFileAttr) { m_stFileAttr = refstFileAttr; }
	inline VOID SetFileHeader(ST_FILE_LAYER_HEADER &refstFileLayerHeader) { m_stFileLayer.stFileLayerHeader = refstFileLayerHeader; }

	DWORD ReadRequestedFile(std::string &refstrInputFileName) override;
	DWORD ExecuteCryptoOperation() override;
	DWORD WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName) override;
};

class CDecrypt : public CFileLayer
{
	std::vector<std::string> m_vecstrDecode;

	VOID DecryptByAES(std::string &refstrDecrypt, ST_BLOCK_ENC_DATA &refstBlockEncData);
	std::string DecodeByBase64(std::string const& encoded_string);

public:
	CDecrypt();

	DWORD ReadRequestedFile(std::string &refstrInputFileName) override;
	DWORD ExecuteCryptoOperation() override;
	DWORD WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName) override;
};

void HexToByte(const char *pIn, DWORD dwLen, BYTE *pOut);

DWORD GetFileName(std::string &refstrInputFileName, std::string &refstrFileName, std::string &refstrFileExtension);
DWORD EncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath);
DWORD DecryptFileLayer(ST_FILE_ATTR &refstFileAttr, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath);


#endif