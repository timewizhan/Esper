#ifndef _FILE_LAYER_STRUCT_
#define _FILE_LAYER_STRUCT_

#include "Common.h"

#define BUF_BLOCK_SIZE 4096

struct ST_FILE_LAYER_HEADER
{
	DWORD dwUserId;
	DWORD dwServerId;
};

struct ST_BLOCK_DATA
{
	char szBuf[BUF_BLOCK_SIZE];
	DWORD dwUsedSize;
};

struct ST_FILE_LAYER_BODY
{
	std::vector<ST_BLOCK_DATA> vecstBlockData;
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