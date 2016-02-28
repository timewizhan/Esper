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

		dwRet = EncryptFileLayer(stFileLayerHeader, strInputFile, strOutputFile);
		break;
	case 1:
		ST_FILE_ATTR stFileAttr;
		stFileAttr.strFilePath = "";
		stFileAttr.strFileName = "";
		stFileAttr.strFileEncExt = "enc";
		stFileAttr.strFileOriginExt = "hwp";
		dwRet = DecryptFileLayer(stFileAttr, strInputEncFile, strOutputFile);
		break;

	}

	printf("Success");
	int a;
	std::cin >> a;
	return 0;
}
