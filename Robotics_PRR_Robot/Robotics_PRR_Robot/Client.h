#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "50000"
#define DEFAULT_BUFLEN 512

using namespace std;
class Client
{
public:
	WSADATA wsaData;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	Client(char *ip, char *port);
	void ClientSend(char *msg);
	void ClientShutDown();
};

#endif