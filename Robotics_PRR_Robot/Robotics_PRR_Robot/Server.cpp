#include "Server.h"

Server::Server(char *ip, char *port)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo Failed: %d\n", iResult);
	}
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("ListenSocket Failed %ld\n", WSAGetLastError());
	}
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Bind Failed %d\n", WSAGetLastError());
	}
	freeaddrinfo(result);
	listen(ListenSocket, SOMAXCONN);
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen Failed: %ld\n", WSAGetLastError());
	}
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("Accept Failed: %d\n", WSAGetLastError());
	}
}

char* Server::ServerRun()
{
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult <= 0)
	{
		isDone = false;
	}
	return recvbuf;
}

bool Server::isServerDone()
{
	return isDone;
}

void Server::ServerShutDown()
{
	iResult = shutdown(ClientSocket, SD_BOTH);
	closesocket(ClientSocket);
	WSACleanup();
}