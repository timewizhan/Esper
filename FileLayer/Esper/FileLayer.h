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
	ST_FILE_ENC_BLOCK_DATA	m_stEncBlock;
	ST_ENC_KEY				m_stEncKey;

	std::string		m_strFilePath;
	std::string		m_strFileName;
	std::string		m_strExtension;

	DWORD ReadFileData(std::string &refstrInputFileName);
	DWORD WriteFileData(std::string &refstrOutputFileName);
	DWORD ExtractFilePath(std::string &refstrInputFileName);

	/////////////// AES ///////////////
	DWORD EncryptByAES();
	DWORD DecryptByAES();
public:
	CFileLayer();
	~CFileLayer();

	DWORD EncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFileName, std::string &refstrOutputFileName);
	DWORD DecryptFileLayer(std::string &refstrInputFileName, std::string &refstrOutputFileName);
};

void HexToByte(const char *pIn, DWORD dwLen, BYTE *pOut);


#endif