#include "FileLayer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
CFileLayer::CFileLayer()
{
	m_strExtension = "enc";

	m_stEncKey.pRawKey = "f4150d4a1ac5708c29e437749045a39a";
	m_stEncKey.pRawIV = "86afc43868fea6abd40fbf6d5ed50905";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
CFileLayer::~CFileLayer()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::EncryptFileLayer(ST_FILE_LAYER_HEADER &refstFileLayerHeader, std::string &refstrInputFileName, std::string &refstrOutputFileName)
{
	DWORD dwRet;
	try
	{
		dwRet = ExtractFilePath(refstrInputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to extract File Name");
		}

		dwRet = ReadFileData(refstrInputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to read File");
		}
		
		dwRet = EncryptByAES();
		if (dwRet != 1) {
			throw std::exception("Fail to encrypt by AES");
		}

		dwRet = WriteFileData(refstrOutputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to Write File Data");
		}
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		return 0;
	}

	return dwRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::DecryptFileLayer(std::string &refstrInputFileName, std::string &refstrOutputFileName)
{
	DWORD dwRet;
	try
	{
		dwRet = ExtractFilePath(refstrInputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to extract File Name");
		}

		dwRet = ReadFileData(refstrInputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to read File");
		}

		dwRet = DecryptByAES();
		if (dwRet != 1) {
			throw std::exception("Fail to encrypt by AES");
		}

		dwRet = WriteFileData(refstrOutputFileName);
		if (dwRet != 1) {
			throw std::exception("Fail to Write File Data");
		}
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		return 0;
	}

	return dwRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::ExtractFilePath(std::string &refstrInputFileName)
{
	DWORD dwPos;
	dwPos = refstrInputFileName.find_last_of("\\");
	if (dwPos == std::string::npos) {
		printf("Invalid File Name\n");
		return 0;
	}

	m_strFilePath = refstrInputFileName.substr(0, dwPos);
	std::string strOriginName = refstrInputFileName.substr(dwPos + 1, refstrInputFileName.size() - dwPos);

	dwPos = strOriginName.find_first_of(".");
	m_strFileName = strOriginName.substr(0, dwPos);

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::ReadFileData(std::string &refstrInputFileName)
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
		ST_BLOCK_DATA stBlockData;
		readn = fread(stBlockData.szBuf, 1, sizeof(stBlockData.szBuf), pFile);
		if (readn < 0) {
			printf("Cannot read data from file\n");
			break;
		}
		stBlockData.dwUsedSize = readn;
		m_stFileLayer.stFileLayerBody.vecstBlockData.push_back(stBlockData);

		dwTotal += readn;
	} while (dwTotal < dwSize);

	::fclose(pFile);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CFileLayer::WriteFileData(std::string &refstrOutputFileName)
{
	FILE *pFile = NULL;
	DWORD dwErr;

	std::string strOutputFileName;
	strOutputFileName = m_strFilePath + "\\" + m_strFileName + "." + m_strExtension;

	dwErr = ::fopen_s(&pFile, strOutputFileName.c_str(), "wb");
	if (dwErr != 0) {
		printf("Fail to write file\n");
		return 0;
	}

	DWORD i;
	int nRet = 0;
	for (i = 0; i < m_stEncBlock.vecstrCipherEncodedText.size(); i++) {
		DWORD dwSizeOfEncodedText = m_stEncBlock.vecstrCipherEncodedText[i].size();
		nRet = fwrite(m_stEncBlock.vecstrCipherEncodedText[i].c_str(), 1, dwSizeOfEncodedText, pFile);
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
		ST_BLOCK_DATA stBlockData;
		DWORD dwSizeOfHeader = sizeof(m_stFileLayer.stFileLayerHeader);
		memcpy(stBlockData.szBuf, &m_stFileLayer.stFileLayerHeader, dwSizeOfHeader);

		std::string strCipherText;
		CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(strCipherText));

		stfEncryptor.Put((const byte *)stBlockData.szBuf, sizeof(stBlockData.szBuf));
		stfEncryptor.MessageEnd();

		// Encoding Base64
		std::string strBase64Text;
		CryptoPP::StringSource(strCipherText, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(strBase64Text)) // Base64Encoder
			); // StringSource

		m_stEncBlock.vecstrCipherEncodedText.push_back(strBase64Text);
	}

	DWORD i;
	for (i = 0; i < m_stFileLayer.stFileLayerBody.vecstBlockData.size(); i++) {
		std::string strCipherText;
		CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(strCipherText));
		
		ST_BLOCK_DATA stBlockData = m_stFileLayer.stFileLayerBody.vecstBlockData[i];
		stfEncryptor.Put((const byte *)stBlockData.szBuf, sizeof(stBlockData.szBuf));
		stfEncryptor.MessageEnd();

		// Encoding Base64
		std::string strBase64Text;
		CryptoPP::StringSource(strCipherText, true, 
								new CryptoPP::Base64Encoder(new CryptoPP::StringSink(strBase64Text)) // Base64Encoder
								); // StringSource

		m_stEncBlock.vecstrCipherEncodedText.push_back(strBase64Text);
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

	DWORD i;
	for (i = 0; i < m_stEncBlock.vecstrCipherEncodedText.size(); i++) {
		std::string strBaseEncodeCipherText = m_stEncBlock.vecstrCipherEncodedText[i];
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
		stfDecryptor.Put((const byte *)strBaseEncodeCipherText.c_str(), strBaseEncodeCipherText.size());
		stfDecryptor.MessageEnd();
	}

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


