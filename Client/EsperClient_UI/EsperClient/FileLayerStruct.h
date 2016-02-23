#ifndef _FILE_LAYER_STRUCT_
#define _FILE_LAYER_STRUCT_

#include "Common.h"

#define BUF_BLOCK_SIZE 512
#define BUF_BLOCK_ENC_SIZE 714

enum E_OP_TYPE
{
	E_OP_ENCRYPT = 0,
	E_OP_DECRYPT
};

struct ST_FILE_ATTR
{
	std::string strFilePath;
	std::string strFileName;
	std::string strFileOriginExt;
	std::string strFileEncExt;

	void operator=(const ST_FILE_ATTR stFileAttr) {
		this->strFilePath = stFileAttr.strFilePath;
		this->strFileName = stFileAttr.strFileName;
		this->strFileOriginExt = stFileAttr.strFileOriginExt;
		this->strFileEncExt = stFileAttr.strFileEncExt;
	}
};

struct ST_FILE_LAYER_HEADER
{
	/*
	To do ..
	*/
	DWORD dwUserId;
	DWORD dwServerId;
};

struct ST_BLOCK_RAW_DATA
{
	unsigned char szBuf[BUF_BLOCK_SIZE];
	DWORD dwUsedSize;

	ST_BLOCK_RAW_DATA() {
		::memset(szBuf, 0x00, sizeof(szBuf));
	}
};

struct ST_BLOCK_ENC_DATA
{
	unsigned char szBuf[BUF_BLOCK_ENC_SIZE];
	DWORD dwUsedSize;

	ST_BLOCK_ENC_DATA() {
		::memset(szBuf, 0x00, sizeof(szBuf));
	}
};

struct ST_FILE_LAYER_BODY
{
	std::vector<ST_BLOCK_RAW_DATA> vecstBlockRawData;
	std::vector<ST_BLOCK_ENC_DATA> vecstBlockEncData;
};

struct ST_FILE_LAYER
{
	ST_FILE_LAYER_HEADER	stFileLayerHeader;
	ST_FILE_LAYER_BODY		stFileLayerBody;
};

struct ST_FILE_ENC_BLOCK_DATA
{
	std::vector<std::string> vecstrCipherEncodedText;
};

struct ST_ENC_KEY
{
	CHAR *pRawKey;
	CHAR *pRawIV;
};

#endif