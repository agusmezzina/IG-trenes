#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h> 
#include <stdio.h>
//#pragma comment(lib, "libWS2_32.lib")

static SOCKET s = NULL;
static WSADATA wsa;
static char receiveBuffer[255];
static struct addrinfo *p, *servinfo;

int startUP()
{
	int iResult = WSAStartup(MAKEWORD(2, 0), &wsa);
	if (iResult != NO_ERROR) {
		return 1;
	}
	return 0;
}

int sendMessage(char* buf)
{
	int iResult = sendto(s, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		freeaddrinfo(servinfo);
		closesocket(s);
		WSACleanup();
		return WSAGetLastError();
	}
	else
		return iResult;
}

int receiveMessage()
{
	struct sockaddr_in sender_addr;
	int addr_len = sizeof(sender_addr);
	int iResult = recvfrom(s, receiveBuffer, 255, 0, (struct sockaddr *)&sender_addr, &addr_len);
	if (iResult == SOCKET_ERROR)
	{
		freeaddrinfo(servinfo);
		closesocket(s);
		WSACleanup();
		return WSAGetLastError();
	}
	else
	{
		receiveBuffer[iResult] = '\0';
		return iResult;
	}
}

int createSocket(const char* IP_ADDRESS, const char* port)
{
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	getaddrinfo(IP_ADDRESS, port, &hints, &servinfo);
	s = INVALID_SOCKET;
	p = servinfo;
	s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	if (s == INVALID_SOCKET)
	{
		freeaddrinfo(servinfo);
		WSACleanup();
		return WSAGetLastError();
	}
	return 0;
}

int cleanUp()
{
	freeaddrinfo(servinfo);
	closesocket(s);
	WSACleanup();
	return 0;
}