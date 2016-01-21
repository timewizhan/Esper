#include "FileLayer.h"

int main(int argc, char **argv)
{
	std::string strInputFile = "C:\\Users\\jmhan\\Desktop\\hello.hwp";
	std::string strInputEncFile = "C:\\Users\\jmhan\\Desktop\\hello.enc";
	std::string strOutputFile = "C:\\Users\\jmhan\\Desktop";

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
	return 0;
}
