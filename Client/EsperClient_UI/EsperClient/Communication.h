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
#include "../../json/json.h"
using namespace std;
#pragma comment ( lib, "ws2_32.lib" )
#pragma pack( 1 ) // 구조체에서 1byte 단위로 메모리를 Align하라는 지시어

class Items {
public:
	string Id, Pw, Email, EmailAuthCode, UserId, FileName, FileId, WrappingResult;
	vector<string> Accessor;
	Items() {}
	string setId(string Id) { this->Id = Id; }
	string setPw(string Pw) { this->Pw = Pw; }
	string setEmail(string Email) { this->Email = Email; }
	string setEmailAuthCode(string EmailAuthCode) { this->EmailAuthCode = EmailAuthCode; }
	string setUserId(string UserId) { this->UserId = UserId; }
	string setFileName(string FileName) { this->FileName = FileName; }
	string setFileId(string FileId) { this->FileId = FileId; }
	string setWrappingResult(string WrappingResult) { this->WrappingResult = WrappingResult; }
	vector<string> setAccessor(vector<string> Accessor) { this->Accessor = Accessor; }
};

//int n = send(s,output.c_str(),size,0);
int socket_send(int socket, string Type, Items item) {
	Json::Value root;

	if (Type == "login") {
		root["Id"] = item.Id;
		root["pw"] = item.Pw;
	}
	else if (Type == "emailAuth") {
		root["Email"] = item.Email;
	}
	else if (Type == "signUp") {
		root["Id"] = item.Id;
		root["Pw"] = item.Pw;
		root["Email"] = item.Email;
		root["EmailAuthCode"] = item.EmailAuthCode;
	}
	else if (Type == "remoteDel") {
		root["UserId"] = item.UserId;
		root["FileName"] = item.FileName;
	}
	else if (Type == "notice") {}
	else if (Type == "termsOfUse") {}
	else if (Type == "wrapping1") {
		root["UserId"] = item.UserId;
		root["FileName"] = item.FileName;
		Json::Value accessorValue;
		for (int i = 0; i < item.Accessor.size(); i++)
			accessorValue.append(item.Accessor[i]);
		root["Accessor"] = accessorValue;
	}
	else if (Type == "wrapping2") {
		root["WrappingResult"] = item.WrappingResult;
	}
	else if (Type == "auth") {
		root["UserId"] = item.UserId;
		root["FIleId"] = item.FileId;
	}
	else if (Type == "authUpdate") {
		root["UserId"] = item.UserId;
		root["FileName"] = item.FileName;
		root["FileId"] = item.FileId;
	}
	else if (Type == "profileUpdate1") {
		root["Id"] = item.Id;
		root["Pw"] = item.Pw;
	}
	else if (Type == "profileEmailAuth") {
		root["Email"] = item.Email;
	}
	else if (Type == "profileUpdate2") {
		root["Id"] = item.Id;
		root["Pw"] = item.Pw;
		root["Email"] = item.Email;
		root["EmailAuthCode"] = item.EmailAuthCode;
	}
	else std::cout << "Wrong Type!" << endl;
	Json::StyledWriter writer;
	string sendText = writer.write(root);
	int n = send(socket, sendText.c_str(), sendText.size(), 0);
	cout << "전송한 문자열 : " << sendText << endl;
	return n;
}


class receiverIdclass {
public:
	string receiverid;
	string auth;
	receiverIdclass*next;
	receiverIdclass(string rid, string au) { receiverid = rid; next = NULL; auth = au; }
};

class packetfile {
public:
	packetfile*next;
	string filename;
	receiverIdclass*receivernamehead;
	packetfile(string fn) { filename = fn; next = NULL; receivernamehead = NULL; }

};
string resultpacketbuffer1;
string resultpacketbuffer2;
packetfile*packetfilehead;
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
