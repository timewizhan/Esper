#include "FileLayer.h"

int main(int argc, char **argv)
{
	std::string strInputFile = "C:\\Users\\wooPC\\Desktop\\hello.hwp";
	std::string strInputEncFile = "C:\\Users\\wooPC\\Desktop\\hello.enc";
	std::string strOutputFile = "C:\\Users\\wooPC\\Desktop";

	DWORD dwChoice = (DWORD)::atoi(argv[1]);
	DWORD dwRet;
	switch (dwChoice)
	{
	case 0:
		ST_FILE_LAYER_HEADER stFileLayerHeader;
		stFileLayerHeader.dwServerId = 1;
		stFileLayerHeader.dwUserId = 1;

		dwRet = ExceuteEncryptFileLayer(stFileLayerHeader, strInputFile, strOutputFile);
		break;
	case 1:
		ST_FILE_ATTR stFileAttr;
		stFileAttr.strFilePath = "";
		stFileAttr.strFileName = "";
		stFileAttr.strFileEncExt = "enc";
		stFileAttr.strFileOriginExt = "hwp";
		dwRet = ExceuteDecryptFileLayer(stFileAttr, strInputEncFile, strOutputFile);
		break;

	}

	printf("Success");
	int a;
	std::cin >> a;
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
//	base64encodedciphertext.clear();
//	//base64encodedciphertext = 
//	//	"UcoubPf7ccTwMhg94sL9gJP6NY9qthyeezcb54Bi4j9QxfhzpB3DQJvID4AC1H2ya6EHoDAT.vzw1PHJvuj+o4Aav2Beo5QckIuewLldeziJlSKkBlE+7tCNEaWilxaabtO6Z1fT1+oC3upEH.EcSkOCwZ4qENw7YzhOQttMrsgB24aY2pEm4uU4qMZnTF6Q5C/jhDggoMd+ekSmWGfY9fZewc.LofK9RdJ7QIj6jnqlZ+iSw41H3Q5t6/vFzuB+RLpDD/x/phuMYtFgLEeBjIXAzBD9vUWxRyx.+RR+wH/Ddx/25JU5HrI2yzSSFUmIo2Wom1nMlRVGRJzxamufdNcp28HdTyPHXl3f8BwKsFaZ.gZsWdFJt5K2ncUViIifVR9FkTSL/3kZa+5Odby3IGwEmGwhAY+kVpNglQlGpBKEJ9xMrIsNf.EBg0jhiy+jbXq2dNlqlvdM7YCHcyzkZ3cM0J3j0O3/rS+fUOjULzCtvR/SyxaEcJO3SzG57x.f7qMETuzh99sK3tiHbGDRHYwvcT/RdmQmBD9QBawVShOX40c0twxNAFh925vBBz7hh04Ha+T.Wc3UA0yq6sanEY9sz5XIYndts/jyBXVmVkvbJlRuTeiMaWAb1aaU796ZKP9udVz1Nn0BsOlp.RTYMuKOt1NXrqU6ZbZ/Z8IMlK5/0NYYw4iJRi9z3FMQZPsy5EknY5WwMXC6N2Ibubm6bzkxn.YmFtdjFxohoYvzcRMrQnyc/ghPXiTmWHY3XKLcGjYOh+awsej+p5vBRqb+YC/o9wVICn3HEo.W9PMFwTNCx00GLXE7yn7ItC06cVVTcVz2k60E7Uv293pX8R9I45ZF2L2uh7/LCOY7QX+54qg.4ABikhJrIJrNCcGSVa8tP+XP57VYNjRfQbk1fVdwHXNjyDyd7Mz7a7D5PYYU8ISlyjD6k2wM.yU/a89KyAWBsodsiGIg2fmOXNA85e6EqL1TWRVYxYmclXPaBhmUUJNQrA3monG7Ah3M7W7Mi.yGaAznXCbIGTWU9eoYRQT/vsAN6b9Ofz735VpgQRsj4KSdXQzRCPAUS7su6VCWzH5x0+skfG.iWbjr2feT7F8E0qQIDz76Rg4t6/C9M+4vnXjFUobFbv4U89WePySdq4NfAeHzQl3oAkdsbTF.YwI/9d8BjDpEUMIB029pLSfs07fgC2FHwMCz1OC88bZGrLiylHjF0CI1Ggpx1BaeI9KjRVu9.uPfjtF5YMrKIRT4KAnMr069krVGYxIzNztiQV9E61BHbQLtd/0Ermxd0a378STWJQm/Gn+35.Rpmo+wbzi7O934LRPhsqC/qKTXR583ziQsvJxg5grngY9uF3jsBnKH8a5k+zFbNRSjUGUeR+.eJckW+YPTY336H6GJedQMzVs0ROBgjHjqdtF0Ldde1jpJaQDzbo3+ORx+ddXRTpdcCLBhVUq.4Fl5fpVX3ZbhZrGU9HZlgBwHv5Mrxnkz8UjpcBU2HglYNf39CfvqMNYb6wiR0f3AQpTonTok.3Y5SB7wdCiYt/4AQ3Y/hqunbYyQ1OJ52HxwWdU5W0GEmCWAq5joranEICu2SgCK9RkaaYG/e.iaC655z5jV0hioOtYNKCrCiYybkVW6U6GuNn/7LEw8gez1tQBQfi3hB0LgRzCotAOilpFVbf.QZdWCIwIBEvhepB58/aOILuy+3cJ4mAKqDRZ+D59O4XzaYyh++UQt3y/N84s1j+z/pN2WXTx.OJAo9l9X5UWoTMwO30XzYIc5Xved8dI+L+EGy9xapCNysDW1mjl66b4T3wBEyPU5wwgr+KR8./IuWUE/2QqJi5sIAw6BMuTgndUrO6tDIXiSxXhEjK6B8+VElFuA4CURJQPXx/CeXKFGRtBLJ.xxevITduOeifBLK8QO7FqN5yXNtKVUsLnEmGEBfmvcgcwlQLOz/ohcO4jRW/nPBatvroq12r.ge0B+p7AYZVCwW0ieAWXCgzTWMYwWsni4OYRJ3pHYqyPL+QLazyqBrZs6go0KElyaBZpMwtE.pm9+6/F/53vgJxXARZ/cuBPFa5xXUZPS+aX8txWfNC6+50BvDQpIJhUenl8clGujyrg3Rdsg.Skz26n1ldkxgPzXNcNjoCadWiL9+uE81XqnstXg+WOP9E+pLAZYRPk+ijknxSwQ3RIy/UNG2.ze3gPfnBDYPUFTHDeQVPqO3RDXu/iUF8yDgvRHzhL3FsGy1YPrl35v1zL7+/2rkQwJ9PaOTd.txn/3uUko+ce1TWGTTJjYY/Ov8j20ohTfroqFhIBBkR0Te1pnl5Rd+/zAlFbIoPHXSHCliit./+6D7+9zjE0tlCVLS6FCQH4zXfCJoiL6EISvYZe6O2buffNhQBX27TkbFr78BDycduF8lbQX.dYe4SdgZWGVGAJEl/JccTR38EkKYGxx2NNXtg+DQKrqaoNjHIkrpUsN/erE4fjL1cYXF842c.7qc6loTD1tFMg8gkSC+cK1I1TFP8qx6J3eon4cxx6WVKtxa+yqqaib6AUNRFmfbf4DiO+hkO.TVDkS8GAeNG75HGPMYJ8XYuOsQXXengecEjjrq8aKrI7DYW7lzJYizyF0xcu8L1rAt1pB+i4.ZCFl8i7IK8aQPmOlJpe0Vi1UhU31TK3kRRz0bM4YHGNiFLA9qUewKbohUgU3Ocbc5krGrnkI.Nx4DYg7/7HfO3VFftjif8ds0iRG4oL5pH9lGK5UAEA1NPBGNarWAmvxCnvDJdh3XqJy2F0O3.z4yxe0DoRDwygCf/RcVxwWwbCiiGXWVXh6RvrjKEfs0HznZ52ygBVqPjcU7/Oxy34hhtwj4V.VZNeVxqxatlniI/0udu+emBxFe78sPBY18fzHV5D8nlMjd8eGS2bZvYsrWcWhx9PvFeKAv9x.2MDKzM/7xaR4lBeVQFhiJtiYLcZIEJTlzdgppkj88+A3BoaNUI+NkxXBkkK2MO3RDQQjHADG.GKI5BPwQU8qkdtBpAaXpFTjeb1DdogeKUYbnfJwHylMNKSn1suFPCOZoGq4NkhxHQQxGCd90.HEjYtB4AimoRyqWe1u7sMtqjlKJ5AvCX2UsY/SOjyWwC+ofNFPIZucd+RJS0ZZK+mlOT/pr+.AUfVzy3jLGrZLTrzrfJfRCkib6Sb7klcfpKiTBveal5sB/9qFfSZ9qgN2CEPbbCThFOvVYzg.ujUN9ZhnhuAW8ph3IAG6Ec79CWTxzrQ6tlJRnAYSinB0E8bQNaE/ijT0lRrQdmFITJrahTkk.XKYaBEmnUpITuCsQU0Wpv77wFBX0SO4s85eDxtTUf4mI3eX4WU4bCoM8VSXXf5qKxK6o44KR.ZyNe3OM18gCnMvGRSZaQZC5KloO5vEANARtBc18NS0iRISTANyAwZFOo53ylcfvdIfT6/R22.ogrJW82LCjTge1snxhW4eTMjDlDIoX3GryaI30P0XVflAX3/ndGyvgxgrYud+26Et6fBl7RW.GTDXO1aMKPv/JQX5/IbTbjiYDUcg8hV9Yc3ezpzYoP8HndznRVJrsOpmZrWY2MC4xsR3lOyD.vuFdz3HzSS5fPTM1V68vNeWjLbi6OAzItnGNc6Epd5KkrfjVR4oZxXkeJNZAtzbMYz9hsHdm.Oc91Yb+TEWzaForavGYu/+Xn9gnwVgpD/Pt3ry1quPlx4lCltiAryzlH0LO+qp1YaWeXldGu.0UN7abMclvuwQRHrPpiqukEZuI8thxkXF093Qg9+kQNiaNVETRggLaWx+zY3pGb4/OBfGXIo.lK7W7utW4Nz4n97acPPQoMWRbLpkDrxgntps9wpL2DB1DWMgH8TfY0HWuSL9sCqJHJ5urBK0.nxF53nOE7HombRHY0tMHgwCRH15ozaAQkxTkwx6xPF2PHUHODAZW/HNW1WMxdKy7AydmkVzA.FWfBb4ydRTZeLYU6vdt92ezlCO5fWXPSgOGzCUTRppkuqAZMzxcpADlKuDYqjByVEg6b8BmU.wWheEMnqU/95MrhaTwK+KP7lqOi0UjIngwECkkCxmZ22CXDl3MVR6TtX3Y5Qtk8onrD9ZTD4.3JYxIgJcVouGOuNvMBgWr1B9B5TaHYiIAoMLbdZXBU1NR7nU/UfI4tnjn0qDTZK0XlHEMbl9.F9gbCTKBa1tWUfzDk5/UENbK+nKI0RJoFp1RwZNRWQOv7LWkiBvui2i/1vTslgl7YBnHrDY2.P2p95Hi4pUKGOsPTWByzT29salPJb2TUJauryI2ttPNkySdd6Ro72Rwo5nFx6vBccH2eMsq/.5O4V7v/CangVMbiwgkVdbFjN/jaHuXKA6vVpwv4mMZw6lRhohncBiNIHXPPOQ1IrqH1w05ll.g/Vw/2imu8wFNwPXhgQ4Oqt6Zu1AlUjlCxzHo6cpeHXD1XSiwGKl5gH9ZbL1F0e24TVX0/CS.EfX4FZJx1OSuGpPhpKER3Kj5lX5P3gdt2+QrSE4eC2J0T4t6aswinoP4831CGD8qSN6/0ENB.BcjC6yxSIINKoRLJAkCXaRp8AHEdFv8FBPMwak9GPXsNH6M+LywxcOPiEBLNswfLXWuahhtI.JWpK5UYWLL+dgTuelaVbPm1q4MXGodIC9UE8Xs5FitK9x7od/NHC+TIa1jXsac0+OfJsRRjn.k/1ObdMLYA5v5xOTfAuj2mEUzskUenGsFfgy0a3lOOb9ivPBwZ4uUv20uMT8UXpRCTti+mrI.mbOphp494O9Lvg4NB6veEOIlzPxRtPzmcl1y6At8F8edMeBHHg6Za+6K+D+CHwhT9mUW5MS2.5vrEP2R5nlwO1rgBcooZm45el01JSobLNh1+XgM5OLbG3zcEn89QhekrApObdC0H97LcfhN6.pvGZWZp87XgLycbi9x5iHYcuJ303/FhJPuYXcrv0HlQCEGJ5NBllb24cWwBbcO4fnmxolTMc.yRFQsYNgw+LfU3zARPO/KAicwzr6Y25kNcfovMkqymkZREgXee+wGnxXo5jDm59Ut5428v3Y.4LvXCj/puVhBN/oXw3f186ZqQbP5ko8wZTEhh6yneym3xDWo+gqhz8G4W3fAcpt55IEM+O7N.6eSOywtCjFp/3lvaAqutcnZF5/RLxa4dD8VnZ/q0aJkr5/V1UQME9yBTvAbNdqfaBv62yAqk.6srfXPa2uXLu4wmCMc7SEkphAExd5EK0GpEynGye1UNGOrXHhfkoW2PY10npP7YwcNbUeuxB.gxvQ1lYWUQO8catx4nI28c5ZrcUMH1yizYL4NTr3D2hCD/3Vif73pHMNENWEO/1U8QP0CKHb.2xMhz42SywnAZEbOuvhmrjZAoSqvTQV15FtuLVGYiUz2eGiInajT3aWp/eps5n+Aeg1eZ5XK.RjGoBMT3DM7UqemqXNKYyvSJuQahxLQxO5cd79q0zclX1NohhOp0ZfjOUGrTZLRYyHkCIvGs.BZi4SLirRBSH1/YNkxq8i3bbGPG1qHtS7ByMzVeqFLm8PF0yXArnmHCdGQJjN+exljhkn4cu.lBUH+JMNUpg=.";
//	base64encodedciphertext =
//		"LEopA/+BpacbbePb0HB/dlPvlR/JTCDd08uy/gXv+iGDIeebTnyhhj2Kr4sJKA6mYhsYuANV.MP3axTqtfk3rf4C+3rAMQwOsXlWtgAUWBQE0UPt7rpTolAlpyE2ARdNORuFMC2Cz/XZJZLPB.VS58+PmEdRIK48L9L2115/ViJb0c4nFwHh1ugWNlsyHrqVyFJTFzFMx3OFlbe+AOfH9tJQL8.I3CrTQms1FAZYNDvqGtKJJp5tthBvkFd5SdZLYYVLjs6Ai+hH+kLxWSsuSabtlz2THK/FSwP.9JtV64Fk0XNuG+voGxN7SR8kZ4dDnjK3VnAVXTK++6sru4yON6F1lO43e375iqz1XNEt2236.fmHB1HSnUMoG5mFWu2MI5H46Gzr/KzwmoPGFOXN3yAgQOpXyb9FGLsbjNALmYz6D3POZYI/+.w8AM9FMIbn+k7f+CpEKBm0w0g8SoTu27p1Z8dxcASQ4LSUZEdxpOBhu9nEBgL+XNJ8JixhAC.PRMX9mlQel/fbDmB3vqAlWM77jS4J7XS3Pk6HKwVX+iXTzxLXSSTRzCqrAHIkmCHnFiOXi2a.07PAxQJJBPesTQE0QN10oXSMVB27gXKb7Rfs8TJrkhBYJvKb87qCqPL2WheEmImLrhHWj0kC.CZNwFFYQpfUsq9QIMEFxyCytzH22ZnIhJJj8xTfQSIbQpsSVhCP2MpW4Te0nTPZEUElpY9dI.ruXwKgmXWwZSOKipMLqPa1QWzM46TKNFS99grSiwbybkdwAQ3LtJlCJ6+G/5Q0p+Kcczox4w.IHgaOLW50Hdpo5XrlMoE0eLyosJxCndBzgIjztjmGAcwuhFTIg/FcMZsu9LLdjah2zTJXKZB.w187Ijx69gSZNi8qCeAQqbNL+0osmTL+u+J1jbClIRP/xrOdHHH33EtHbO8Q036/7fQWnJRa.q9zhTnZ0dAH2VTWINyc/x0XLdXeknrDlwzhOZstzxMFx+pcWEUM/snVM72PyBdZeyYLDYIm4.1n7J7T5+651TafboLcIn3LYmN7SPMciBCp9S8alkFwmwfckejgum+DM+imyPqXyD3eNjXk/x.q5FmI/t20ZRWTZ4YMQ/xxQDMS2ykBlHxwmQg+FwIKew34fev3FoafTsod3YTiRTroQ2eX2/e.KTYepePN5YwFm1jDtEAkgvi5gMUMZWUEyHbcUrgOlpNKA+ZxRCO/HQ1Opes7b7LCqtbJTfhH.IBlyYHkiiF0BljR3NdCmex1tlOJL4p573IBwGWxO7y6wCuy93XsifMsWX20xaMYxfTL73KQz.6tkACURn9gp3jNQKSRnC1damXLLdP8cyPilAlb5akwsmkuXieF1ZK743C9EhS0L1CeCoSAoQ.qabiHD60Y6VIY5jEOwWGUo7pmWu9DuY5gxAGx11tbB/tNqsx2g3ZKvmsbmXvcdQTkRUS2De2.WYGm22qBgZ/BxhAYCVxK8cb/wOKJCGBt8Q9Wwi2AdM1D/pdTYYxReXECUuvK+JlM11Horvjq.3Au7N/R3J9akCP5Ju/zB1WAZUeaAZu8NB6cY1ceJgvWEs+iJganZ6qZ2v8H7rJ7NTqcn25r9.oAfIdkES4/4lJpz2dNOXoH1cDruThHfckZic8am7YwhqZcMG43jwaMgMo3mI3NB/AwVq9dmh.X9YEgbaAPoSfgeev9n6Od0Pkag1AC0gOwLRzxcA/Ga+ofhYeSuH51mt9wNlfETETXyR7lHO2.eTVnPTRc25JKRv/t+C4WkuJF1SW0Btte88zkG4BhP8nVOegoQgwoWJQL+QTOxOvKtlkgz2tO.GJK5Y9ws6JkBXkjbECtvOtdRNOPjH7RyzDA4FuPAodAktdWg7y+TFN8ZoIs++r9AJtjkF7zT.U0+fVrduyQhteFAlaSBo8jOHha0N/BTm/l8uOZX2ktX3ueEUaFt5THx062MZjFXHZCLaA5gS.KLF43lmFXF0vI0arj4ep1xTeFKZ/Xk0RGXstOo0QvJuPPUJQyQW4dKvN2/7JGYPbGAFejPQo.95xxnovmbwdO8tWMC/DHGJJ0Zj5juhEkkN2O12i7lTLKJhaAUKFKB6kCsaAAFthx4khx1WTv.1gBZcfoOvrsPY0KClvrWcI4BZYUY5k0H6OGQ3Dn6UA57lrFjHmDXigvn6cBHdfR1nWyvUsLp.n2eNHUePKefwQydqNtxTtyppWzLQ19VWywmvtJ/lB9fjPFz7McpF35HR6N8rIo8k+WZW/Bns.xujTfK7864ccEAwrV3B7qny/jn9eYdGFi2cyz7O9FCJHtHKKg2AHkfk6eyozWJUkGF33CXR9.TBTBKCyVN7cuMDkXUPOgmixK572bBfB/0qY2p7tiY2SiAWIv6w9qbS8DF6/wVtbJeDrH8kZE.bmYUqvaV9Ex12EgJeqRrjrpE559IQBTMOKWcUuxAqYiaE5FxztZaDfbCMIrvbwmOIdltuQ/B.HmYUmOZpgTkJ3jJRzJB+BWJIyRKVny9l/5zD0BhlX3Ck3VDqz1tsfeMaWF47gp12BhEiNEOD.0/pF2qW+mrTtCTVYTmfgkKWD0F4MlAMnlo8tMhdRMh7RkfU15inIYVHOt9BUidXZXVoVEvZs.1qBNlr6t8cX2ZHJDA7gCe6ObZS7B50l37ej7mrM4iz9s9fjA8sZvE0P9Cy6ULXVssoj633OY./Ct5XavUbkk4nROeiPagACVeordoMLSKd+5fAoAoVhFl+aPqiRNCUSGEgT6jubtZOwvgtvKC.UX/Dk4zZCPX/otOhahypUVsF0gmoKMUc455X5i1E35v8Ifgtj+LGgDc0/FH7/jB+z+6XPfi7.ZKrel3QeNtU6E1QiZamVhXD4Xv4esS36He8SgfnRXLehvBmr8bwn+j2gj4uPfRcZm93aO32O.7nGc+UE+m2EGFNkBX1QC94nUneL7HvRoP/0BEXmwezS6td2JawVroLYotVZ/McUB06YhVf+6.W0OFeoY+I/QpSCpaCaNNI1PhZKtMEkrcKSXwuskE1+4Uv0+T5vGq9EilgMeb1w8FVOhFxRJ5.DH8lq1rsraAE9jQlKd0dzlmHsF6Gouor+F9IyKOXBkspDEDpOsKdbeGAem62nAShLkplr7bN.tp844jtuFu8iHcXasrmgTHB0EKp8VNK4Rsq7vN8OyHMnIVWkNoebeLYPWcGyTJJrV09xbUF3.1PPe4nxJP+c/CLQ4qkkWB7mttTCHtqJoOOKrAMz4h2eB4dl9aQ41q/E9gxBlQZYi/Uq9YSyD.sO3PKvskaTjGSLD/G7eUn/fJoMazkIpW6qpYh4AsCkhovy4oG9UwqEKuzbx9Sw16I2i+xXad.x4QlQqW4b25iA178es8g+cfiLBuXk2LoU0hWPz7030wyUYDxbOpiDmcoAW/qvJM1W/Hjj9h7.F1C/ixyRGqNMICILsu6gdoP8kb09MEDd4r3vkp2T4FPOgZY9tzrN9eXSIUiZrDFKpNe2bhmu.NJB33yu20F8JhBt0TeqWYBRfSmcLpDw5yYOukdp3Q98/C+lE+Kv+6PBH6I9XI5Um6f1VyK2g.CsS4345XJ5V6vpem4RfTM+yAIYsEbimzb4oWBv8T6NJdv3K2tEyBBKJgH+uMTWZaocuGEgml.bIY4qIBWm7EAX5tAt90lXWXqCXVTxdow3Nz4n1GysVwTp9DmNx+JUgxSrJEtAXQdebgGQj9f.eZTDD2PpiI9zZilaQ3WxMyTaB+bdTB+Ammd2o0RkcCiRm4hxfdp3w/XQBNPU12HRh3oUSnuw.oYZ3sdHaDPY4zL1eHvNFky8lR+rD4deCaVpB75Qx2+U42yLivvULo4h5DZbv7gTybA1OYFaJ.PiTg5iXEknCwuWqda6iDXMRERyZpsQb4xDj3azT9EDBKUbFsQ7416YmZuaOBsvbGTJO/REOW.iE9JjfpjpHv5NNNK14tpOxSZxaupVfrm1BnHMv3jdAD+DKOPr6YUHVZ231QM/od9TQgrf7Gk.f/REdOfIk09AA8ltLdbkubA+d/0hT0R33TiTOdsReAFU3DZ98WpfrOlM8nCfICNo+X/jj8k7.+kfiWUG8sgAlSeWuLJyFyvSS3Cso2e8bAMRQMBV0mzybDvu3so+nvettNJcCbLCTTy+y/3BL.x3WakOPavpkFPZoX5bK/dL4AIj0j2rk7f+/KPqDnj+chfzrLvtJPUHiHMBC3/ZCCWrWOLis8.jnmG0wN5bz9RTqAHX7/5HGHTCxnoVHYfiISEgijI9iZCGi9w9lfvCVo3PP9NN3lv5BmUmRZn.fRSxCMeWpmccEmnPPd5esktlKC7j3Bb0ZMInlck2biicjq2iEG0m4q5f7EKFOuIItljYmnno.ixM4DYJnqby57Hb4JkCX9z0J4DDg/WDFjS0XWD4vuH6M4ArMON1t8FtSBwufYSDtYtxST67s.OtzqElcB7+T8BobG9bVrTge+cyCuaM/ZhUwxrHUucgUvYy1fkqQrfT5S7pfMd7AJDLL5kwJT.o4Oi1ZCK1DLURUowSeZ5qF/qy0L1+LcWWOSoWYDWMtBkNORcggc0DL0oEeCB6Uuh4JnmGvN6.fLFse4cTmlqeyfblMDarNFnrzMJl6gw4B6+vCkXM0CeKHlgTDfNG0N6VmjpRlCxnHClGEYaD.IRLNbO6wZLHRFpaX7masegM+VqvsXwhodyOxkPkc21wnL3WR6Pz1fIRRParARe6/1eIzG9hd.WzOdC+aseo7XTQLaGUVnleP6vaz8K6W8tb67olLVnujulZ+lGd2YXqdn676k9ZteKw+YvX7S.BrgQRerWWiovbYzKYye6PmEENcp99YEhpBa3eQKYsxtRtB1DW124dNT9oLe62SANc8VetTPb.euPBO6vd2U/zHYnmjI8MEI3wTap37ewEIOFMUN6JyhJjFxTMR/6FYSAD9WMLcQ2q7KpavFA9.7B6LjQus8ZkMFHPRs3Q6R+WTULQyWvz7kJYQp58qhHneEXzjmx3tU7dai2xEVN1CwXgGuKo0.5JFlIpmDCkc3I28nNvXbRLrkDkXHRaGX7yZY7O0SompAvLzUVZ8L8BBqluDRubZieA3/rfWq.vj9LP01wruu6qYWpeYRIzfKvwWjMEvb5TOpTdQDKaLZ6HIKn69GBd69PRTxs6tePp/Op/W9/.d+4qeUAYeIbjHfwzqdYjxIn8fxfiuVsaBDEVvDexuU8BGSLXbOUAP9VedS4A2bxfbKWSs3Yr.ZpX5SuowLfky6JOJItQQfFHdqJKh+O/EBhE4wC5eGrYuGBPONGaCNFt7UJPBxlRoNs0OD0Ry.5Yv8sLg7Evbw79F5o2YSUM5kCAa27wvBEAa8dt+sSz1PA49OGQNpip2w8ZRqALuVRdcYJ+4w.+UYAwBlP5zAPw12t0tGYEKllpPtFhCPPviNhpwlXzgMp4GrZSlY8O9TudVWa6lnTiDVNEQOE.923hZwjVa5MQmnbhXMr5T+JkcY+fHdP9hm0ZgZshLX7IUr8qoUSxCB5rOSnUt7VddRLBBPDw.9SlzccEi+2U=.";
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