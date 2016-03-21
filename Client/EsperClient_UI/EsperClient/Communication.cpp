#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// windows.h 파일에서 자주 사용하지 않은 요소를 컴파일에서 제외시킨다.
// 컴파일 속도가 빨라지고 winsock2.h와의 충돌이 일어나지 않는다.

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <cassert>
#include <string>
#include <vector>
#include "Communication.h"
#include "../../json/json.h"
using namespace std;
#pragma comment ( lib, "ws2_32.lib" )
#pragma pack( 1 ) // 구조체에서 1byte 단위로 메모리를 Align하라는 지시어


SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				  //int n = send(s,output.c_str(),size,0);

int socket_send(int socket, string Type, Items item) {
	Json::Value root;
	root["Type"] == Type;
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
		root["sessionKey"] = item.SessionKey;
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
		root["sessionKey"] = item.SessionKey;
	}
	else if (Type == "withdrawal") {
		root["ID"] = item.Id;
		root["sessionKey"] = item.SessionKey;
	}
	else if (Type == "accessorCheck") {
		root["AccesorId"] = item.AccessorID;
		root["UserID"] = item.Id;
		root["sessionKey"] = item.SessionKey;
	}
	else if (Type == "wrappingReq") {
		root["UserID"] = item.Id;
		root["Accesor"] = item.AccessorID;
		root["sessionKey"] = item.SessionKey;
	}
	else if (Type == "wrappingRes") {
		root["FileId"] = item.FileId;
		root["AccessorID"] = item.Accessor[0];
		root["WrappingResult"] = "succ";
		root["UserID"] = item.Id;
		root["sessionKey"] = item.SessionKey;
	}
	else if (Type == "auth") {
		root["UserID"] = item.Id;
		root["sessionKey"] = item.SessionKey;
		root["Filed"] = item.Filed;
	}
	else if (Type == "remoteDel") {
		root["UserId"] = item.UserId;
		root["FileID"] = item.FileId;
		root["Del"] = item.del;
		root["AccessorId"] = item.AccessorID;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "accessorCheck") {
		root["AccessorId"] = item.AccessorID;
		root["UserId"] = item.UserId;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "authUpdate") {
		root["UserId"] = item.UserId;
		root["Filename"] = item.FileName;
		root["del"] = item.del;
		root["SessionKey"] = item.SessionKey;
	}
	else if (Type == "fileListReq") {
		root["UserId"] = item.UserId;
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
packetfile *packetfilehead;


int socket_recv(int socket, char* buffer, int size)
{
	int total_received;
	int received = 0;

	assert(buffer);
	assert(size > 0);
	assert(socket);

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
	recv(socket, buffer, size, 0);
	if (received < 0)
	{
		return received;
	}

	string buff(buffer);
	cout << "got" << buff << endl;
	Json::Value packet;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(buff, packet, false);
	if (!parsedSuccess)
	{
		// Report failures and their locations
		// in the document.
		cout << "Failed to parse JSON" << endl
			<< reader.getFormattedErrorMessages() << endl;
		return 1;
	}

	const Json::Value Type = packet["Type"];
	if (!Type.isNull())
	{
		int switchcase;
		if (Type.asString() == "login")
		{
			switchcase = 1;
		}
		else if (Type.asString() == "emailAuth")
		{
			switchcase = 2;
		}
		else if (Type.asString() == "signUp")
		{
			switchcase = 3;
		}
		else if (Type.asString() == "remoteDel")
		{
			switchcase = 4;
		}
		else if (Type.asString() == "notice")
		{
			switchcase = 5;
		}
		else if (Type.asString() == "termsOfUse")
		{
			switchcase = 6;
		}
		else if (Type.asString() == "wrapping1")
		{
			switchcase = 7;
		}
		else if (Type.asString() == "wrapping2")
		{
			switchcase = 8;
		}
		else if (Type.asString() == "auth")
		{
			switchcase = 9;
		}
		else if (Type.asString() == "authUpdate")
		{
			switchcase = 10;
		}
		else if (Type.asString() == "profileUpdate1")
		{
			switchcase = 11;
		}
		else if (Type.asString() == "profileEmailAuth")
		{
			switchcase = 12;
		}
		else if (Type.asString() == "profileUpdate2")
		{
			switchcase = 13;
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
		case 1://
			notAnArray = packet["Id"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["Result"];
			resultpacketbuffer2 = notAnArray.asString();
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
					ReceiverId = Receiver[indexj]["ReceiverId"];
					Auth = Receiver[indexj]["Auth"];
					idwalker = walker->receivernamehead;
					while (idwalker != NULL)
					{
						idwalker = idwalker->next;
					}
					walker->receivernamehead = new receiverIdclass(ReceiverId.asString(), Auth.asString());

				}
			}
			cout << packetfilehead->filename << endl << packetfilehead->receivernamehead->receiverid << endl;

			break;
		case 2:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 3:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 4://
			notAnArray = packet["FileName"];
			resultpacketbuffer1 = notAnArray.asString();
			notAnArray = packet["Result"];
			resultpacketbuffer2 = notAnArray.asString();
			break;
		case 5:
			notAnArray = packet["Notice"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 6:
			notAnArray = packet["Contents"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 7:
			notAnArray = packet["FileId"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 8:
			notAnArray = packet["DbResult"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 9:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 10:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 11:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 12:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;
		case 13:
			notAnArray = packet["Result"];
			resultpacketbuffer1 = notAnArray.asString();
			break;

		default:
			cout << "type error" << endl;

		}
	}
	return received > 0 ? total_received : received;
}
