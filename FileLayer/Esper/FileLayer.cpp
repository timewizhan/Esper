#include "FileLayer.h"

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
DWORD CFileLayer::ReadRawFileData(std::string &refstrInputFileName)
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
DWORD CFileLayer::ReadEncryptedFileData(std::string &refstrInputFileName)
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


//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::EncryptByAES()
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

	{
		/*
			Encrypt Header
		*/
		ST_BLOCK_RAW_DATA stBlockRawData;
		DWORD dwSizeOfHeader = sizeof(m_stFileLayer.stFileLayerHeader);
		memcpy(stBlockRawData.szBuf, &m_stFileLayer.stFileLayerHeader, dwSizeOfHeader);

		std::string strCipherText;
		CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(strCipherText));

		stfEncryptor.Put((const byte *)stBlockRawData.szBuf, sizeof(stBlockRawData.szBuf));
		stfEncryptor.MessageEnd();

		// Encoding Base64
		std::string strBase64Text;
		CryptoPP::StringSource(strCipherText, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(strBase64Text)) // Base64Encoder
			); // StringSource

		ST_BLOCK_ENC_DATA stBlockEncData;
		stBlockEncData.dwUsedSize = strBase64Text.size();
		memcpy(stBlockEncData.szBuf, strBase64Text.c_str(), sizeof(stBlockEncData.szBuf));	
		m_stFileLayer.stFileLayerBody.vecstBlockEncData.push_back(stBlockEncData);
	}

	DWORD i;
	for (i = 0; i < m_stFileLayer.stFileLayerBody.vecstBlockRawData.size(); i++) {
		std::string strCipherText;
		CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(strCipherText));

		ST_BLOCK_RAW_DATA stBlockData = m_stFileLayer.stFileLayerBody.vecstBlockRawData[i];
		stfEncryptor.Put((const byte *)stBlockData.szBuf, sizeof(stBlockData.szBuf));
		stfEncryptor.MessageEnd();

		// Encoding Base64
		std::string strBase64Text;
		CryptoPP::StringSource(strCipherText, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(strBase64Text)) // Base64Encoder
			); // StringSource

		ST_BLOCK_ENC_DATA stBlockEncData;
		stBlockEncData.dwUsedSize = strBase64Text.size();
		memcpy(stBlockEncData.szBuf, strBase64Text.c_str(), sizeof(stBlockEncData.szBuf));
		m_stFileLayer.stFileLayerBody.vecstBlockEncData.push_back(stBlockEncData);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::DecryptByAES()
{
	// Initialize Key Value
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	HexToByte(m_stEncKey.pRawKey, strlen(m_stEncKey.pRawKey), key);

	// Initialize IV(Initial Vector) Value
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	HexToByte(m_stEncKey.pRawIV, strlen(m_stEncKey.pRawIV), iv);

	DWORD i = 0;
	{
		/*
			Decrypt Header
		*/
		if (m_stFileLayer.stFileLayerBody.vecstBlockEncData.size() < 1) {
			printf("Enc block data is not exist");
			return 0;
		}
		ST_BLOCK_ENC_DATA stBlockEncData;
		stBlockEncData = m_stFileLayer.stFileLayerBody.vecstBlockEncData[i++];

		std::string strBaseEncodeCipherText = (char *)stBlockEncData.szBuf;
		strBaseEncodeCipherText.resize(sizeof(stBlockEncData.szBuf));

		std::string strBaseDecodeText;
		CryptoPP::StringSource(strBaseEncodeCipherText, true,
				new CryptoPP::Base64Decoder(
					new CryptoPP::StringSink(strBaseDecodeText)
					) // Base64Encoder
				 ); // StringSource

		CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

		std::string strDecryptedText;
		CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(strDecryptedText));
		stfDecryptor.Put((const byte *)strBaseDecodeText.c_str(), strBaseDecodeText.size());
		stfDecryptor.MessageEnd();

		::memcpy(&m_stFileLayer.stFileLayerHeader, strDecryptedText.c_str(), sizeof(m_stFileLayer.stFileLayerHeader));
	}
	
	for (; i < m_stFileLayer.stFileLayerBody.vecstBlockEncData.size(); i++) {
		ST_BLOCK_ENC_DATA stBlockEncData = m_stFileLayer.stFileLayerBody.vecstBlockEncData[i];

		std::string strBaseEncodeCipherText = (char *)stBlockEncData.szBuf;
		std::string strBaseDecodeText;
		CryptoPP::StringSource(strBaseEncodeCipherText, true,
			new CryptoPP::Base64Decoder(
			new CryptoPP::StringSink(strBaseDecodeText)
			) // Base64Encoder
			); // StringSource

		CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

		std::string strDecryptedText;		
		CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(strDecryptedText));
		stfDecryptor.Put((const byte *)strBaseDecodeText.c_str(), strBaseDecodeText.size());
		stfDecryptor.MessageEnd();

		ST_BLOCK_RAW_DATA stBlockRawData;
		::memcpy(stBlockRawData.szBuf, strDecryptedText.c_str(), sizeof(stBlockRawData.szBuf));
		m_stFileLayer.stFileLayerBody.vecstBlockRawData.push_back(stBlockRawData);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::WriteEncryptFileData(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName)
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

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::WriteDecryptFileData(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName)
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
	for (i = 0; i < m_stFileLayer.stFileLayerBody.vecstBlockRawData.size(); i++) {
		ST_BLOCK_RAW_DATA stBlockRawData = m_stFileLayer.stFileLayerBody.vecstBlockRawData[i];
		DWORD dwSizeOfEncodedText = sizeof(stBlockRawData.szBuf);
		nRet = fwrite(stBlockRawData.szBuf, 1, dwSizeOfEncodedText, pFile);
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

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::ReadRequestedFile(std::string &refstrInputFileName)
{
	DWORD dwRet;
	switch (m_eOPType)
	{
	case E_OP_ENCRYPT:
		dwRet = ReadRawFileData(refstrInputFileName);
		break;
	case E_OP_DECRYPT:
		dwRet = ReadEncryptedFileData(refstrInputFileName);
		break;
	default:
		dwRet = 0;
		break;
	}
	return dwRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::ExecuteCryptoOperation()
{
	DWORD dwRet;
	switch (m_eOPType)
	{
	case E_OP_ENCRYPT:
		dwRet = EncryptByAES();
		break;
	case E_OP_DECRYPT:
		dwRet = DecryptByAES();
		break;
	default:
		dwRet = 0;
		break;
	}
	return dwRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::WriteToFile(const std::string &refstrOutputFilePath, std::string &refstrOutputFileName)
{
	DWORD dwRet;
	switch (m_eOPType)
	{
	case E_OP_ENCRYPT:
		dwRet = WriteEncryptFileData(refstrOutputFilePath, refstrOutputFileName);
		break;
	case E_OP_DECRYPT:
		dwRet = WriteDecryptFileData(refstrOutputFilePath, refstrOutputFileName);
		break;
	default:
		dwRet = 0;
		break;
	}
	return dwRet;
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
DWORD ExceuteDecryptFileLayer(ST_FILE_ATTR &refstFileAttr, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath)
{
	CFileLayer FileLayer;
	FileLayer.SetOPType(E_OP_DECRYPT);

	std::string strFileName, strFileExtension;
	GetFileName(refstrInputFullFileName, strFileName, strFileExtension);

	FileLayer.SetFileAttr(refstFileAttr);

	DWORD dwRet;
	try
	{
		dwRet = FileLayer.ReadRequestedFile(refstrInputFullFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
		dwRet = FileLayer.ExecuteCryptoOperation();
		if (dwRet == 0) {
			throw std::exception();
		}

		std::string strOutputFileName = "output";
		dwRet = FileLayer.WriteToFile(refstrOutputFilePath, strOutputFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
	}
	catch (std::exception &e)
	{
		printf("%s", e.what());
		return 0;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD ExceuteEncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFullFileName, std::string &refstrOutputFilePath)
{
	CFileLayer FileLayer;
	FileLayer.SetOPType(E_OP_ENCRYPT);

	std::string strFileName, strFileExtension;
	GetFileName(refstrInputFullFileName, strFileName, strFileExtension);

	ST_FILE_ATTR stFileAttr;
	stFileAttr.strFilePath = "";
	stFileAttr.strFileName = strFileName;
	stFileAttr.strFileOriginExt = strFileExtension;
	stFileAttr.strFileEncExt = "enc";

	FileLayer.SetFileAttr(stFileAttr);
	FileLayer.SetFileHeader(refstFileLayerHeader);

	DWORD dwRet;
	try 
	{
		dwRet = FileLayer.ReadRequestedFile(refstrInputFullFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
		dwRet = FileLayer.ExecuteCryptoOperation();
		if (dwRet == 0) {
			throw std::exception();
		}

		std::string strOutputFileName = "output";
		dwRet = FileLayer.WriteToFile(refstrOutputFilePath, strOutputFileName);
		if (dwRet == 0) {
			throw std::exception();
		}
	}
	catch (std::exception &e)
	{
		printf("%s", e.what());
		return 0;
	}
	return 1;
}