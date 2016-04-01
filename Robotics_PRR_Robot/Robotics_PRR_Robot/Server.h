#pragma once
#ifndef SERVER_H
#define SERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "Robot.h"
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "50000"
#define DEFAULT_BUFLEN 512

typedef void(*ServerCallback)();

using namespace std;
class Server
{
public:
	WSADATA wsaData;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	bool isDone = true;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	Robot* bot;
	ServerCallback callBack;

	Server(char *ip, char *port, ServerCallback callBack, Robot* bot);
	char* ServerRun();
	bool isServerDone();
	void ServerShutDown();
};

#endif