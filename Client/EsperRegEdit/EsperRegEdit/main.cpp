//Program tested on Microsoft Visual Studio 2008 - Zahid Ghadialy
//This program shows example of reading and writing from registry
#include <windows.h>
#include <iostream>

using namespace std;

#define WIN_32_LEAN_AND_MEAN

void writeToRegistry(void)
{
	DWORD lRv;
	HKEY hKey;
	//First,create a key ESPER 
	//Check if the registry exists
	//http://msdn.microsoft.com/en-us/library/ms724897(VS.85).aspx
	lRv = RegOpenKeyEx(
		HKEY_CLASSES_ROOT,
		"*\\shell\\ESPER",
		0,
		KEY_WRITE,
		&hKey
		);

	if (lRv != ERROR_SUCCESS)
	{
		DWORD dwDisposition;

		cout << "ESPER is Created..." << endl;

		// Create a key if it did not exist
		//http://msdn.microsoft.com/en-us/library/ms724844(VS.85).aspx
		lRv = RegCreateKeyEx(
			HKEY_CLASSES_ROOT,
			"*\\shell\\ESPER", //"Use Multi-Byte Character Set" by using L
			0,
			"ESPER",
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			&dwDisposition
			);
	}
	else cout << "ESPER already exist" << endl;

		//http://msdn.microsoft.com/en-us/library/ms724837(VS.85).aspx
		RegCloseKey(hKey);
	

		//Second,create a key 'command' 
		
		//Check if the registry exists
		//http://msdn.microsoft.com/en-us/library/ms724897(VS.85).aspx
		lRv = RegOpenKeyEx(
			HKEY_CLASSES_ROOT,
			"*\\shell\\ESPER\\command",
			0,
			KEY_WRITE,
			&hKey
			);

		if (lRv != ERROR_SUCCESS)
		{
			DWORD dwDisposition;

			cout << "command is created..." << endl;
			// Create a key if it did not exist
			//http://msdn.microsoft.com/en-us/library/ms724844(VS.85).aspx
			lRv = RegCreateKeyEx(
				HKEY_CLASSES_ROOT,
				"*\\shell\\ESPER\\command", //"Use Multi-Byte Character Set" by using L
				0,
				"ESPER",
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				NULL,
				&hKey,
				&dwDisposition
				);
			char* path = "C:\\Users\\wooPC\\Source\\Repos\\Esper\\Client\\EsperClient_UI\\x64\\Debug\\EsperClient.exe %1";
			//http://msdn.microsoft.com/en-us/library/ms724923(VS.85).aspx
			RegSetValueEx(
				hKey,
				NULL,
				0,
				REG_SZ,
				//reinterpret_cast<BYTE *>(&string),
				(BYTE *)path,
				strlen(path)
				);
			//http://msdn.microsoft.com/en-us/library/ms724837(VS.85).aspx
		}
		RegCloseKey(hKey);
	}

// ----------------------       Esperopener ----------------------------------------------

void writeToRegistry2(void)
{
	DWORD lRv;
	HKEY hKey;
	//First,create a key .esp and update the default value. 
	//Check if the registry exists
	//http://msdn.microsoft.com/en-us/library/ms724897(VS.85).aspx
	lRv = RegOpenKeyEx(
		HKEY_CLASSES_ROOT,
		".esp",
		0,
		KEY_WRITE,
		&hKey
		);

	if (lRv != ERROR_SUCCESS)
	{
		DWORD dwDisposition;

		cout << ".esp key is created..." << endl;

		// Create a key if it did not exist
		//http://msdn.microsoft.com/en-us/library/ms724844(VS.85).aspx
		lRv = RegCreateKeyEx(
			HKEY_CLASSES_ROOT,
			".esp", //"Use Multi-Byte Character Set" by using L
			0,
			"ESPER OPENER",
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			&dwDisposition
			);

		RegSetValueEx(
			hKey,
			NULL,
			0,
			REG_SZ,
			//reinterpret_cast<BYTE *>(&string),
			(BYTE *)"EsperOpener_exe",
			strlen("EsperOpener_exe")
			);
	}
	//http://msdn.microsoft.com/en-us/library/ms724837(VS.85).aspx
	RegCloseKey(hKey);


	//Second,create a key 'command' 

	//Check if the registry exists
	//http://msdn.microsoft.com/en-us/library/ms724897(VS.85).aspx
	lRv = RegOpenKeyEx(
		HKEY_CLASSES_ROOT,
		"EsperOpener_exe\\shell\\open\\command",
		0,
		KEY_WRITE,
		&hKey
		);

	if (lRv != ERROR_SUCCESS)
	{
		DWORD dwDisposition;

		cout << "command is created..." << endl;
		// Create a key if it did not exist
		//http://msdn.microsoft.com/en-us/library/ms724844(VS.85).aspx
		lRv = RegCreateKeyEx(
			HKEY_CLASSES_ROOT,
			"EsperOpener_exe\\shell\\open\\command", //"Use Multi-Byte Character Set" by using L
			0,
			"ESPER",
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			&dwDisposition
			);
		//char* path = "C:\\Users\\wooPC\\Source\\Repos\\Esper\\Client\\EsperClient_UI\\x64\\Debug\\EsperClient.exe %1";
		char* path = "C:\\Users\\wooPC\\Source\\Repos\\Esper\\Client\\EsperOpener\\Debug\\EsperOpener.exe %1";
		//http://msdn.microsoft.com/en-us/library/ms724923(VS.85).aspx
		RegSetValueEx(
			hKey,
			NULL,
			0,
			REG_SZ,
			//reinterpret_cast<BYTE *>(&string),
			(BYTE *)path,
			strlen(path)
			);
		//http://msdn.microsoft.com/en-us/library/ms724837(VS.85).aspx
	}
	RegCloseKey(hKey);
}




void readValueFromRegistry(void)
{
	//Example from http://msdn.microsoft.com/en-us/library/ms724911(VS.85).aspx

	HKEY hKey;

	//Check if the registry exists
	DWORD lRv = RegOpenKeyEx(
		HKEY_CLASSES_ROOT,
		"*\\shell\\ESPER\\command",
		0,
		KEY_READ,
		&hKey
		);

	if (lRv == ERROR_SUCCESS)
	{
		DWORD BufferSize = sizeof(DWORD);
		DWORD dwRet;
		DWORD cbData;
		DWORD cbVal = 0;

		dwRet = RegQueryValueEx(
			hKey,
			NULL,
			NULL,
			NULL,
			(LPBYTE)&cbVal,
			&cbData
			);
		

		if (dwRet == ERROR_SUCCESS)
			cout << "\nValue is " << cbVal << endl;
		else cout << "\nRegQueryValueEx failed " << dwRet << endl;
	}
	else
	{
		cout << "RegOpenKeyEx failed " << lRv << endl;
	}
}

int main()
{
	writeToRegistry();
	writeToRegistry2();
	readValueFromRegistry();
	int a;
	cin >> a;
	return 0;
}