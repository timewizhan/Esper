
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <afxwin.h>
#include "afxdialogex.h"
#include <WinSock2.h>
//#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <atlbase.h>
#include <iostream>
#include "FileLayer.h"
#include "json.h"
#include "Communication.h"
#include <fstream>

Items item;
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
	ifstream fin;

	fin.open("../../EsperClient_UI/idsk.txt");
	fin >> item.Id >> item.SessionKey;

	SOCKET s = socketCreate();
	if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}

	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"), MB_OK);
	else {
		//item.UserId = id;
		//item.SessionKey = sessionkey;
		//item.Filed= "1";
		socket_send(s, "auth", item);
	}
	string* strtemp = NULL;
	socket_recv(s, strtemp);

	if (resultpacketbuffer1 == "succ")
	{
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

		ShellExecute(NULL, "open", "C:\\Users\\10\\Desktop\\EsperGit\\Client\\EsperOpener\\temp\\hello.hwp", NULL, NULL, SW_SHOW);
	}
	else if (resultpacketbuffer1 == "del")
	{
		DeleteFile(CW2A(argv[1]));
	}
	else
	{
		AfxMessageBox(_T("서버와 통신이 실패했습니다."));
	}

	closesocket(s);




	system("pause");
	return 0;
}
