#include "Server.h"

Server::Server(char *ip, char *port,  ServerCallback callBack, Robot* _bot)
{
	this->callBack = callBack;
	bot = _bot;

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

//serverthread function
char* Server::ServerRun()
{
	while (true) {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

		if (iResult <= 0)
		{
			// Parse the values you recieved and pass those to 'robot'
			isDone = false;
		}
		
	DWORD mResult;

	//wait for mutex
	mResult
		= WaitForSingleObject(
		bot->ghMutex,    // handle to mutex
		INFINITE);  // no time-out interval

	switch (mResult)
	{
		// The thread got ownership of the mutex
	case WAIT_OBJECT_0:
		try {
			//push to the queue
			bot->commands.push(recvbuf);

			// Release ownership of the mutex object
			if (!ReleaseMutex(bot->ghMutex))
			{
				// Handle error.
			}
		}

		catch(exception e){
			//handle it yo
		}
		break;

	}

	}

	return 0; //loops forever
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