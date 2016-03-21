#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// windows.h ���Ͽ��� ���� ������� ���� ��Ҹ� �����Ͽ��� ���ܽ�Ų��.
// ������ �ӵ��� �������� winsock2.h���� �浹�� �Ͼ�� �ʴ´�.

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <cassert>
#include <string>
#include <vector>
#include "../../json/json.h"
using namespace std;
#pragma comment ( lib, "ws2_32.lib" )
#pragma pack( 1 ) // ����ü���� 1byte ������ �޸𸮸� Align�϶�� ���þ�

class Items {
public:
	string Id, Pw, Email, EmailAuthCode, UserId, FileName, FileId, WrappingResult, Name, date, SessionKey, AccessorID, Filed, del;
	vector<string> Accessor;
	Items() {}
	void setId(string Id) { this->Id = Id; }
	void setPw(string Pw) { this->Pw = Pw; }
	void setEmail(string Email) { this->Email = Email; }
	void setEmailAuthCode(string EmailAuthCode) { this->EmailAuthCode = EmailAuthCode; }
	void setUserId(string UserId) { this->UserId = UserId; }
	void setFileName(string FileName) { this->FileName = FileName; }
	void setFileId(string FileId) { this->FileId = FileId; }
	void setWrappingResult(string WrappingResult) { this->WrappingResult = WrappingResult; }
	void setAccessor(vector<string> Accessor) { this->Accessor = Accessor; }
};

extern SOCKET s;
//extern Items item;//�α��ζ� id, ����Ű���� ��� ������ �д�.

		   //int n = send(s,output.c_str(),size,0);
int socket_send(int socket, string Type, Items item);


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
extern string resultpacketbuffer1;
extern string resultpacketbuffer2;
extern string resultpacketbuffer3;
extern packetfile *packetfilehead;
int socket_recv(int socket, char* buffer, int size);