#pragma once
#define WIN32_LEAN_AND_MEAN
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// windows.h 파일에서 자주 사용하지 않은 요소를 컴파일에서 제외시킨다.
// 컴파일 속도가 빨라지고 winsock2.h와의 충돌이 일어나지 않는다.

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <cassert>
#include <string>
#include <vector>
#include "Communication.h"

//#include "afxdialogex.h"

#include "../../json/json.h"
using namespace std;
#pragma comment ( lib, "ws2_32.lib" )
#pragma pack( 1 ) // 구조체에서 1byte 단위로 메모리를 Align하라는 지시어


//SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				  //int n = send(s,output.c_str(),size,0);
SOCKET socketCreate() {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return s;
}
int sockSetting(SOCKET s) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("165.132.144.98");
	return connect(s, (SOCKADDR*)&addr, sizeof(addr));
}

int socket_send(int socket, string Type, Items item) {
	Json::Value root;
	root["type"] = Type;
	if (Type == "checkID") {
		root["ID"] = item.Id;
	}
	else if (Type == "checkEmail") {
		root["email"] = item.Email;
	}
	else if (Type == "authEmail") {
		root["email"] = item.Email;
	}
	else if (Type == "checkAuthCode") {
		root["email"] = item.Email;
		root["authCode"] = item.EmailAuthCode;
	}
	else if (Type == "signUp") {
		root["ID"] = item.Id;
		root["passwd"] = item.Pw;
		root["name"] = item.Name;
		root["Email"] = item.Email;
		root["date"] = item.date;
	}
	else if (Type == "signIn") {
		root["ID"] = item.Id;
		root["passwd"] = item.Pw;
	}
	else if (Type == "signOut") {
		root["ID"] = item.Id;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "findID") {
		root["name"] = item.Name;
		root["Email"] = item.Email;
	}
	else if (Type == "findPasswd") {
		root["ID"] = item.Id;
		root["Email"] = item.Email;
	}
	else if (Type == "changePasswd") {
		root["ID"] = item.Id;
		root["changePasswd"] = item.Pw;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "withdrawal") {
		root["ID"] = item.Id;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "accessorCheck") {
		root["AccessorID"] = item.AccessorID;
		root["UserID"] = item.Id;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "wrappingReq") {
		root["UserID"] = item.Id;
		root["Accesor"] = item.AccessorID;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "wrappingRes") {
		root["FileId"] = item.FileId;
		root["AccessorID"] = item.Accessor[0];
		root["WrappingResult"] = "succ";
		root["UserID"] = item.Id;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "auth") {
		root["UserID"] = item.Id;
		root["SessionKey"] = item.SessionKey;
		root["Filed"] = item.Filed;
	}
	else if (Type == "remoteDel") {
		root["UserID"] = item.UserId;
		root["FileID"] = item.FileId;
		root["Del"] = item.del;
		root["AccessorId"] = item.AccessorID;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "accessorCheck") {
		root["AccessorId"] = item.AccessorID;
		root["UserID"] = item.UserId;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "authUpdate") {
		root["UserID"] = item.UserId;
		root["Filename"] = item.FileName;
		root["del"] = item.del;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "fileListReq") {
		root["UserID"] = item.UserId;
		root["SessionKey"] = item.SessionKey;
	}

	else if (Type == "notice") {}
	else if (Type == "termsOfUse") {}

	else std::cout << "Wrong Type!" << endl;
	Json::StyledWriter writer;
	string sendText = writer.write(root);
	int n = send(socket, sendText.c_str(), sendText.size(), 0);
	cout << "전송한 문자열 : " << sendText << endl;
	return n;
}


string resultpacketbuffer1;
string resultpacketbuffer2;
string resultpacketbuffer3;
packetfile *packetfilehead;


int socket_recv(int socket, std::string *str )
{
	int total_received;
	int received = 0;


	assert(socket);

	char buffer[1024];

	total_received = 0;
	/*
	while (size)
	{
	cout << size << endl;
	received = recv(socket, buffer, size, 0);

	// 리턴값이 = 0 이면 close되었음을 의미
	// 리턴값이 < 0 이면 오류 발생을 의미
	if (received <= 0) break;

	total_received += received;
	size -= received;

	buffer += received;
	}*/
	int i = 0;
	while (i < 1024)
	{
		buffer[i] = '\0';
		i++;
	}

	received = recv(socket, (char FAR*)buffer, sizeof(buffer), 0);
	
	if (received < 0)
	{
		return received;
	}

	string buff(buffer);
	//cout << "got" << buff << endl;
	*str = buff;
	
	Json::Value packet;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(buff, packet, false);
	if (!parsedSuccess)
	{
		// Report failures and their locations
		// in the document
		//cout << "Failed to parse JSON" << endl
		//	<< reader.getFormattedErrorMessages() << endl;
		resultpacketbuffer1 = "Fail to parse JSON    " + reader.getFormattedErrorMessages();
		
		return 1;
	}

	const Json::Value Type = packet["type"];
	if (!Type.isNull())
	{
		int switchcase;
		if (Type.asString() == "checkID")
		{
			switchcase = 1;
		}
		else if (Type.asString() == "checkEmail")
		{
			switchcase = 2;
		}
		else if (Type.asString() == "authEmail")
		{
			switchcase = 3;
		}
		else if (Type.asString() == "checkAuthCode")
		{
			switchcase = 4;
		}
		else if (Type.asString() == "signUp")
		{
			switchcase = 5;
		}
		else if (Type.asString() == "signIn")
		{
			switchcase = 6;
		}
		else if (Type.asString() == "signOut")
		{
			switchcase = 7;
		}
		else if (Type.asString() == "findID")
		{
			switchcase = 8;
		}
		else if (Type.asString() == "findPasswd")
		{
			switchcase = 9;
		}
		else if (Type.asString() == "changePasswd")
		{
			switchcase = 10;
		}
		else if (Type.asString() == "withdrawal")
		{
			switchcase = 11;
		}
		else if (Type.asString() == "accessorCheck")
		{
			switchcase = 12;
		}
		else if (Type.asString() == "wrapping1")
		{
			switchcase = 13;
		}
		else if (Type.asString() == "insertRes")
		{
			switchcase = 14;
		}
		else if (Type.asString() == "auth")
		{
			switchcase = 15;
		}
		else if (Type.asString() == "remoteDel")
		{
			switchcase = 16;
		}
		else if (Type.asString() == "accessorCheck")
		{
			switchcase = 17;
		}
		else if (Type.asString() == "authUpdate")
		{
			switchcase = 18;
		}
		else if (Type.asString() == "fileListReq")
		{
			switchcase = 19;
		}
		else if (Type.asString() == "notice")
		{
			switchcase = 20;
		}
		else if (Type.asString() == "termsOfUse")
		{
			switchcase = 21;
		}
		else
		{
			switchcase = -1;
		}

		Json::Value notAnArray;
		//Json::Value AnArray;
		Json::Value File;
		Json::Value FileName;
		Json::Value Receiver;
		Json::Value ReceiverId;
		Json::Value Auth;

		packetfile*walker = NULL;
		receiverIdclass*idwalker = NULL;
		switch (switchcase)
		{
		case 1://checkID
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 2://checkEmail
			notAnArray = packet["email"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 3://authEmail
			notAnArray = packet["email"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 4://checkAuthCode
			notAnArray = packet["email"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 5://signUp
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 6://signIn
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["sessionKey"];
			resultpacketbuffer2 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer3 = notAnArray.asString();
			break;
		case 7://signOut
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 8://findID
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["emial"];
			resultpacketbuffer2 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer3 = notAnArray.asString();
			break;
		case 9://findPasswd
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["email"];
			resultpacketbuffer2 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer3 = notAnArray.asString();
			break;
		case 10://changePasswd
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 11://withdrawal
			notAnArray = packet["ID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 12://accessorCheck
			notAnArray = packet["result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 13://wrapping1
			notAnArray = packet["Filed"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["Result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 14://insertRes
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 15://auth
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 16://remoteDel
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["msg"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 17://accessorCheck
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 18://authUpdate
			notAnArray = packet["FileID"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["Result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 19://fileListReq
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			if (resultpacketbuffer1 == "succ")
			{
			File = packet["File"];
			packetfilehead = NULL;
			walker = NULL;
			for (unsigned int index = 0; index < File.size(); ++index)
			{


				while (walker != NULL)
				{
					walker = walker->next;
				}

				walker = new packetfile(File[index]["Name"].asString());
				if (packetfilehead == NULL)
				{
					packetfilehead = walker;
				}
				cout << "index : " << index << endl;
				Receiver = File[index]["Receiver"];

				for (unsigned int indexj = 0; indexj < Receiver.size(); ++indexj)
				{
						ReceiverId = Receiver[indexj]["FileId"];
						Auth = Receiver[indexj]["AccessorId"];
					idwalker = walker->receivernamehead;
					while (idwalker != NULL)
					{
						idwalker = idwalker->next;
					}
					walker->receivernamehead = new receiverIdclass(ReceiverId.asString(), Auth.asString());

				}
			}
			}
			break;
		case 20:
			break;
		case 21:
			break;


		default:
			cout << "type error" << endl;

		}
	}
	return received > 0 ? total_received : received;
}
