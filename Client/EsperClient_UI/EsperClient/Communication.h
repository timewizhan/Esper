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
//extern Items item;//로그인때 id, 세션키등을 모두 저장해 둔다.

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