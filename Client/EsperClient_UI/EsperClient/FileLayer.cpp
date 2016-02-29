#include "FileLayer.h"

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

//////////////////////////////////////////////////////////////////////////////////////////////////
CFileLayer::CFileLayer()
{
	m_stEncKey.pRawKey = "f4150d4a1ac5708c29e437749045a39a";
	m_stEncKey.pRawIV = "86afc43868fea6abd40fbf6d5ed50905";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
CFileLayer::~CFileLayer()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool CFileLayer::isBase64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

CEncrypt::CEncrypt() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
VOID CEncrypt::EncryptByAES(std::string &refstrEncode, ST_BLOCK_ENC_DATA &refstBlockEncData)
{
	// Initialize Key Value
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	HexToByte(m_stEncKey.pRawKey, strlen(m_stEncKey.pRawKey), key);

	// Initialize IV(Initial Vector) Value
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	HexToByte(m_stEncKey.pRawIV, strlen(m_stEncKey.pRawIV), iv);

	// AES Encryption
	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);


	std::string strCipherText;
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(strCipherText));
	stfEncryptor.Put((const byte *)refstrEncode.c_str(), refstrEncode.length() + 1);
	stfEncryptor.MessageEnd();


	refstBlockEncData.dwUsedSize = strCipherText.size();
	memcpy(refstBlockEncData.szBuf, strCipherText.c_str(), sizeof(refstBlockEncData.szBuf));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
std::string CEncrypt::EncodeByBase64(unsigned char const* bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CEncrypt::ReadRequestedFile(std::string &refstrInputFileName)
{
	FILE *pFile = NULL;
	DWORD dwErr;
	dwErr = ::fopen_s(&pFile, refstrInputFileName.c_str(), "rb");
	if (dwErr != 0) {
		printf("Cannot read file [%s]\n", refstrInputFileName.c_str());
		return 0;
	}

	fseek(pFile, 0, SEEK_END);
	DWORD dwSize = ftell(pFile);
	printf("File Data Size : [%d]\n", dwSize);

	fseek(pFile, 0, SEEK_SET);

	int readn = 0;
	DWORD dwTotal = 0;
	do
	{
		ST_BLOCK_RAW_DATA stBlockRawData;
		readn = fread(stBlockRawData.szBuf, 1, sizeof(stBlockRawData.szBuf), pFile);
		if (readn < 0) {
			printf("Cannot read data from file\n");
			break;
		}
		stBlockRawData.dwUsedSize = readn;
		m_stFileLayer.stFileLayerBody.vecstBlockRawData.push_back(stBlockRawData);

		dwTotal += readn;
	} while (dwTotal < dwSize);

	::fclose(pFile);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CEncrypt::ExecuteCryptoOperation()
{
	{
		ST_BLOCK_RAW_DATA stBlockData;
		memcpy(stBlockData.szBuf, &m_stFileLayer.stFileLayerHeader, sizeof(stBlockData.szBuf));

		std::string strEncode = EncodeByBase64(stBlockData.szBuf, sizeof(stBlockData.szBuf));

		ST_BLOCK_ENC_DATA stBlockEncData;
		EncryptByAES(strEncode, stBlockEncData);
		m_stFileLayer.stFileLayerBody.vecstBlockEncData.push_back(stBlockEncData);
	}

	std::vector<ST_BLOCK_RAW_DATA>::iterator vecIter;
	for (vecIter = m_stFileLayer.stFileLayerBody.vecstBlockRawData.begin(); vecIter != m_stFileLayer.stFileLayerBody.vecstBlockRawData.end(); vecIter++) {
		ST_BLOCK_RAW_DATA stBlockRawData = (*vecIter);

		std::string strEncode = EncodeByBase64(stBlockRawData.szBuf, sizeof(stBlockRawData.szBuf));

		ST_BLOCK_ENC_DATA stBlockEncData;
		EncryptByAES(strEncode, stBlockEncData);
		m_stFileLayer.stFileLayerBody.vecstBlockEncData.push_back(stBlockEncData);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CEncrypt::WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName)
{
	FILE *pFile = NULL;
	DWORD dwErr;

	std::string strOutputFileName;
	strOutputFileName = refstrOutputFilePath + "\\" + m_stFileAttr.strFileName + "." + m_stFileAttr.strFileEncExt;

	dwErr = ::fopen_s(&pFile, strOutputFileName.c_str(), "wb");
	if (dwErr != 0) {
		printf("Fail to write file\n");
		return 0;
	}

	DWORD i;
	int nRet = 0;
	for (i = 0; i < m_stFileLayer.stFileLayerBody.vecstBlockEncData.size(); i++) {
		ST_BLOCK_ENC_DATA stBlockEncData = m_stFileLayer.stFileLayerBody.vecstBlockEncData[i];
		DWORD dwSizeOfEncodedText = sizeof(stBlockEncData.szBuf);
		nRet = fwrite(stBlockEncData.szBuf, 1, dwSizeOfEncodedText, pFile);
		if (nRet != dwSizeOfEncodedText) {
			printf("Cannot write encoded data to file\n");
			break;
		}
	}

	if (nRet < 0) {
		::fclose(pFile);
		return 0;
	}

	refstrOutputFileName = strOutputFileName;
	::fclose(pFile);
	return 1;
}

CDecrypt::CDecrypt() {}

VOID CDecrypt::DecryptByAES(std::string &refstrDecrypt, ST_BLOCK_ENC_DATA &refstBlockEncData)
{
	// Initialize Key Value
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	HexToByte(m_stEncKey.pRawKey, strlen(m_stEncKey.pRawKey), key);

	// Initialize IV(Initial Vector) Value
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	HexToByte(m_stEncKey.pRawIV, strlen(m_stEncKey.pRawIV), iv);

	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	std::string strDecryptedText;
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(strDecryptedText));
	stfDecryptor.Put((const byte *)refstBlockEncData.szBuf, sizeof(refstBlockEncData.szBuf));
	stfDecryptor.MessageEnd();

	refstrDecrypt = strDecryptedText;
}

std::string CDecrypt::DecodeByBase64(std::string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && isBase64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

DWORD CDecrypt::ReadRequestedFile(std::string &refstrInputFileName)
{
	FILE *pFile = NULL;
	DWORD dwErr;
	dwErr = ::fopen_s(&pFile, refstrInputFileName.c_str(), "rb");
	if (dwErr != 0) {
		printf("Cannot read file [%s]\n", refstrInputFileName.c_str());
		return 0;
	}

	fseek(pFile, 0, SEEK_END);
	DWORD dwSize = ftell(pFile);
	printf("File Data Size : [%d]\n", dwSize);

	fseek(pFile, 0, SEEK_SET);

	int readn = 0;
	DWORD dwTotal = 0;
	do
	{
		ST_BLOCK_ENC_DATA stBlockEncData;
		readn = fread((unsigned char *)stBlockEncData.szBuf, 1, sizeof(stBlockEncData.szBuf), pFile);
		if (readn < 0) {
			printf("Cannot read data from file\n");
			break;
		}
		stBlockEncData.dwUsedSize = readn;
		m_stFileLayer.stFileLayerBody.vecstBlockEncData.push_back(stBlockEncData);

		dwTotal += readn;
	} while (dwTotal < dwSize);

	::fclose(pFile);
	return 1;
}

DWORD CDecrypt::ExecuteCryptoOperation()
{
	BOOL bFirst = TRUE;
	std::vector<ST_BLOCK_ENC_DATA>::iterator vecIter;
	for (vecIter = m_stFileLayer.stFileLayerBody.vecstBlockEncData.begin(); vecIter != m_stFileLayer.stFileLayerBody.vecstBlockEncData.end(); vecIter++) {
		/*
		To do..
		first vecIter indicate header in file layer
		*/
		if (bFirst) {
			bFirst = FALSE;
			continue;
		}

		ST_BLOCK_ENC_DATA stBlockEncData = (*vecIter);

		std::string strDecrypt;
		DecryptByAES(strDecrypt, stBlockEncData);

		if (strDecrypt.size() < 1) {
			continue;
		}

		std::string strDecode;
		strDecode = DecodeByBase64(strDecrypt);
		m_vecstrDecode.push_back(strDecode);
	}
	return 1;
}

DWORD CDecrypt::WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName)
{
	FILE *pFile = NULL;
	DWORD dwErr;

	std::string strOutputFileName;
	strOutputFileName = refstrOutputFilePath + "\\" + refstrOutputFileName + "." + m_stFileAttr.strFileOriginExt;

	dwErr = ::fopen_s(&pFile, strOutputFileName.c_str(), "wb");
	if (dwErr != 0) {
		printf("Fail to write file\n");
		return 0;
	}

	DWORD i;
	int nRet = 0;
	for (i = 0; i < m_vecstrDecode.size(); i++) {
		std::string strDecode = m_vecstrDecode[i];
		if (strDecode.size() < 1) {
			continue;
		}

		nRet = fwrite(&strDecode[0], 1, strDecode.size(), pFile);
		if (nRet != strDecode.size()) {
			printf("Cannot write encoded data to file\n");
			break;
		}
	}

	if (nRet < 0) {
		::fclose(pFile);
		return 0;
	}

	refstrOutputFileName = strOutputFileName;
	::fclose(pFile);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void HexToByte(const char *pIn, DWORD dwLen, BYTE *pOut)
{
	DWORD i;
	for (i = 0; i < dwLen; i += 2) {
		char c0 = pIn[i + 0];
		char c1 = pIn[i + 1];

		BYTE b = (
			((c0 & 0x40 ? (c0 & 0x20 ? c0 - 0x57 : c0 - 0x37) : c0 - 0x30) << 4) |
			((c1 & 0x40 ? (c1 & 0x20 ? c1 - 0x57 : c1 - 0x37) : c1 - 0x30))
			);
		pOut[i / 2] = b;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetFileName(std::string &refstrInputFileName, std::string &refstrFileName, std::string &refstrFileExtension)
{
	DWORD dwPos;
	dwPos = refstrInputFileName.find_last_of("\\");
	if (dwPos == std::string::npos) {
		printf("Invalid File Name\n");
		return 0;
	}

	std::string strOriginName = refstrInputFileName.substr(dwPos + 1, refstrInputFileName.size() - dwPos);

	dwPos = strOriginName.find_first_of(".");
	refstrFileName = strOriginName.substr(0, dwPos);
	refstrFileExtension = strOriginName.substr(dwPos + 1, strOriginName.size() - dwPos);
	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD EncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath)
{
	CEncrypt *pEncrypt = NULL;
	pEncrypt = new CEncrypt();

	std::string strFileName, strFileExtension;
	GetFileName(refstrInputFullFileName, strFileName, strFileExtension);

	ST_FILE_ATTR stFileAttr;
	stFileAttr.strFilePath = "";
	stFileAttr.strFileName = strFileName;
	stFileAttr.strFileOriginExt = strFileExtension;
	stFileAttr.strFileEncExt = "enc";

	pEncrypt->SetFileAttr(stFileAttr);
	pEncrypt->SetFileHeader(refstFileLayerHeader);

	DWORD dwRet;
	try
	{
		dwRet = pEncrypt->ReadRequestedFile(refstrInputFullFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
		dwRet = pEncrypt->ExecuteCryptoOperation();
		if (dwRet == 0) {
			throw std::exception();
		}

		std::string strOutputFileName = "output";
		dwRet = pEncrypt->WriteToFile(refstrOutputFilePath, strOutputFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
	}
	catch (std::exception &e)
	{
		printf("%s", e.what());
		if (pEncrypt != NULL) {
			delete pEncrypt;
		}
		return 0;
	}

	delete pEncrypt;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD DecryptFileLayer(ST_FILE_ATTR &refstFileAttr, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath)
{
	CDecrypt *pDecrypt = NULL;
	pDecrypt = new CDecrypt();

	std::string strFileName, strFileExtension;
	GetFileName(refstrInputFullFileName, strFileName, strFileExtension);

	//FileLayer.SetFileAttr(refstFileAttr);

	DWORD dwRet;
	try
	{
		dwRet = pDecrypt->ReadRequestedFile(refstrInputFullFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
		dwRet = pDecrypt->ExecuteCryptoOperation();
		if (dwRet == 0) {
			throw std::exception();
		}

		std::string strOutputFileName = "output";
		dwRet = pDecrypt->WriteToFile(refstrOutputFilePath, strOutputFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		return 0;
	}
	return 1;
}
