#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <atlbase.h>
#include <iostream>
#include "FileLayer.h"

int main()
{
	USES_CONVERSION;
	LPWSTR cmdLine = A2W(GetCommandLine());
	int num = 0;
	int i;

	LPWSTR* argv = CommandLineToArgvW(cmdLine, &num);
	if (argv == NULL) return 1;

	for (i = 0; i<num; i++)
	{
		printf("%s \n", CW2A(argv[i]));
	}
	if (0)
	{
		DeleteFile(CW2A(argv[1]));
	}
	std::string strInputEncFile = CW2A(argv[1]);
	ST_FILE_ATTR stFileAttr;
	stFileAttr.strFilePath = "";
	stFileAttr.strFileName = "";
	stFileAttr.strFileEncExt = "enc";
	stFileAttr.strFileOriginExt = "hwp";
	std::string strOutputFile = "C:\\Users\\10\\Desktop\\EsperGit\\Client\\EsperOpener\\temp\\hello.hwp";

	DWORD dwRet = DecryptFileLayer(stFileAttr, strInputEncFile, strOutputFile);

	LocalFree(argv);

	STARTUPINFO StartupInfo = { 0 };
	PROCESS_INFORMATION ProcessInfo;
	StartupInfo.cb = sizeof(STARTUPINFO);

	ShellExecute(NULL,"open","C:\\Users\\10\\Desktop\\EsperGit\\Client\\EsperOpener\\temp\\hello.hwp", NULL, NULL, SW_SHOW);
	
	system("pause");
	return 0;
}
