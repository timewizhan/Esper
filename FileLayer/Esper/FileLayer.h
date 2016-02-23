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
	ST_FILE_LAYER			m_stFileLayer;
	ST_ENC_KEY				m_stEncKey;

	E_OP_TYPE				m_eOPType;
	ST_FILE_ATTR			m_stFileAttr;

	DWORD ReadRawFileData(std::string &refstrInputFileName);
	DWORD ReadEncryptedFileData(std::string &refstrInputFileName);

	DWORD EncryptByAES();
	DWORD DecryptByAES();

	DWORD WriteEncryptFileData(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName);
	DWORD WriteDecryptFileData(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName);
	
public:
	CFileLayer();
	~CFileLayer();

	inline VOID SetOPType(E_OP_TYPE eOPType) { m_eOPType = eOPType; }
	inline VOID SetFileAttr(ST_FILE_ATTR &refstFileAttr) { m_stFileAttr = refstFileAttr; }
	inline VOID SetFileHeader(ST_FILE_LAYER_HEADER &refstFileLayerHeader) { m_stFileLayer.stFileLayerHeader = refstFileLayerHeader; }

	DWORD ReadRequestedFile(std::string &refstrInputFileName);
	DWORD ExecuteCryptoOperation();
	DWORD WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName);
};

void HexToByte(const char *pIn, DWORD dwLen, BYTE *pOut);
 
DWORD GetFileName(std::string &refstrInputFileName, std::string &refstrFileName, std::string &refstrFileExtension);
DWORD ExceuteEncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath);
DWORD ExceuteDecryptFileLayer(ST_FILE_ATTR &refstFileAttr, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath);


#endif