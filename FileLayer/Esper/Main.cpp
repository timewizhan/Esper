#include "FileLayer.h"

int main()
{
	std::string strInputFile = "C:\\Users\\jmhan\\Desktop\\hello.hwp";
	std::string strOutputFile;

	CFileLayer FileLayer;
	ST_FILE_LAYER_HEADER stFileLayerHeader;
	stFileLayerHeader.dwServerId = 1;
	stFileLayerHeader.dwUserId = 1;

	DWORD dwRet;
	dwRet = FileLayer.EncryptFileLayer(stFileLayerHeader, strInputFile, strOutputFile);
	if (dwRet == 0) {
		printf("Fail to encrypt file layer");
		return 0;
	}

	printf("Success");
	return 0;
}
//
//#include <iostream>
//#include <iomanip>
//
//#include "cryptlib.h"
//#include "modes.h"
//#include "aes.h"
//#include "filters.h"
//#include "base64.h"
//
//typedef unsigned int uint;
//
//void hex2byte(const char *in, uint len, byte *out)
//{
//	for (uint i = 0; i < len; i += 2) {
//		char c0 = in[i + 0];
//		char c1 = in[i + 1];
//		byte c = (
//			((c0 & 0x40 ? (c0 & 0x20 ? c0 - 0x57 : c0 - 0x37) : c0 - 0x30) << 4) |
//			((c1 & 0x40 ? (c1 & 0x20 ? c1 - 0x57 : c1 - 0x37) : c1 - 0x30))
//			);
//		out[i / 2] = c;
//	}
//}
//
//int main(int argc, char* argv[]) {
//	// 키 할당
//	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
//	memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
//	char* rawKey = "f4150d4a1ac5708c29e437749045a39a";
//	hex2byte(rawKey, strlen(rawKey), key);
//
//	// 초기벡터 할당
//	byte iv[CryptoPP::AES::BLOCKSIZE];
//	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
//	char* rawIv = "86afc43868fea6abd40fbf6d5ed50905";
//	hex2byte(rawIv, strlen(rawIv), iv);
//
//	// 평문 할당
//	std::string plaintext = "http://sopt.org/";
//	std::string ciphertext;
//	std::string base64encodedciphertext;
//	std::string decryptedtext;
//	std::string base64decryptedciphertext;
//
//	// 평문 출력
//	std::cout << "Plain Text (" << plaintext.size() <<
//		" bytes)" << std::endl;
//	std::cout << plaintext;
//	std::cout << std::endl << std::endl;
//
//	unsigned int plainTextLength = plaintext.length();
//
//	// AES 암호화 수행
//	CryptoPP::AES::Encryption
//		aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
//	CryptoPP::CBC_Mode_ExternalCipher::Encryption
//		cbcEncryption(aesEncryption, iv);
//
//	CryptoPP::StreamTransformationFilter
//		stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
//	stfEncryptor.Put((const byte *)plaintext.c_str(), plainTextLength + 1);
//	stfEncryptor.MessageEnd();
//
//	// Base64 인코딩
//	CryptoPP::StringSource(ciphertext, true,
//		new CryptoPP::Base64Encoder(
//		new CryptoPP::StringSink(base64encodedciphertext)
//		) // Base64Encoder
//		); // StringSource
//
//	// Base64 인코딩 문자열 출력
//	std::cout << "Cipher Text (" << base64encodedciphertext.size()
//		<< " bytes)" << std::endl;
//	std::cout << "cipher : " << base64encodedciphertext << std::endl;
//	std::cout << std::endl << std::endl;
//
//	// Base64 디코딩
//	CryptoPP::StringSource(base64encodedciphertext, true,
//		new CryptoPP::Base64Decoder(
//		new CryptoPP::StringSink(base64decryptedciphertext)
//		) // Base64Encoder
//		); // StringSource
//
//	// AES 복호화
//	CryptoPP::AES::Decryption aesDecryption(key,
//		CryptoPP::AES::DEFAULT_KEYLENGTH);
//	CryptoPP::CBC_Mode_ExternalCipher::Decryption
//		cbcDecryption(aesDecryption, iv);
//
//	CryptoPP::StreamTransformationFilter
//		stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
//	stfDecryptor.Put((const byte *)base64decryptedciphertext.c_str(), base64decryptedciphertext.size());
//	stfDecryptor.MessageEnd();
//
//	// 복호화 문자열 출력
//	std::cout << "Decrypted Text: " << std::endl;
//	std::cout << decryptedtext;
//	std::cout << std::endl << std::endl;
//	return 0;
//}