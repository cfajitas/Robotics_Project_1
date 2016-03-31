#include "Client.h"

Client::Client(char *ip, char *port)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo(ip, port, &hints, &result);
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Set Up Failed: %d\n", WSAGetLastError());
	}
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Connect Failed: %d\n", WSAGetLastError());
	}
	freeaddrinfo(result);
}

void Client::ClientSend(char *msg)
{
	iResult = send(ConnectSocket, msg, (int)strlen(msg), 0);
	if (iResult == SOCKET_ERROR) {
		printf("Send Failed: %d\n", WSAGetLastError());
	}
	printf("Sent: %s\n", msg);
}

void Client::ClientShutDown()
{
	iResult = shutdown(ConnectSocket, SD_BOTH);
	closesocket(ConnectSocket);
	WSACleanup();
}