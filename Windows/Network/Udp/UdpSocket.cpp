//===============
// UdpSocket.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#pragma comment (lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include "UdpSocket.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//==================
// Con-/Destructors
//==================

UdpSocket::UdpSocket():
uSocket(INVALID_SOCKET)
{}

UdpSocket::~UdpSocket()
{
Close();
}


//========
// Common
//========

BOOL UdpSocket::Broadcast(WORD port, VOID const* buf, WORD size)
{
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=0)
	return false;
SOCKET sock=socket(AF_INET, SOCK_DGRAM, 0);
if(sock==INVALID_SOCKET)
	return false;
CHAR broadcast='1';
status=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(CHAR));
if(status<0)
	{
	closesocket(sock);
	return false;
	}
sockaddr_in addr;
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
addr.sin_addr.S_un.S_addr=0xFFFFFFFF;
INT written=sendto(sock, (CHAR const*)buf, size, 0, (sockaddr*)&addr, sizeof(sockaddr_in));
closesocket(sock);
if(written!=size)
	return false;
return true;
}

VOID UdpSocket::Close()
{
if(uSocket!=INVALID_SOCKET)
	{
	shutdown(uSocket, SD_BOTH);
	closesocket(uSocket);
	uSocket=INVALID_SOCKET;
	}
}

BOOL UdpSocket::Listen(WORD port)
{
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=0)
	return false;
addrinfo addr;
ZeroMemory(&addr, sizeof(addr));
addr.ai_family=AF_INET;
addr.ai_socktype=SOCK_DGRAM;
addr.ai_protocol=IPPROTO_UDP;
addr.ai_flags=AI_PASSIVE;
addrinfo* paddr=nullptr;
CHAR pport[8];
StringPrint(pport, 8, "%u", port);
status=getaddrinfo(nullptr, pport, &addr, &paddr);
if(status!=0)
	return false;
SOCKET sock=socket(paddr->ai_family, paddr->ai_socktype, paddr->ai_protocol);
if(sock==INVALID_SOCKET)
	{
	freeaddrinfo(paddr);
	return false;
	}
status=bind(sock, paddr->ai_addr, (INT)paddr->ai_addrlen);
freeaddrinfo(paddr);
if(status==SOCKET_ERROR)
	{
	closesocket(sock);
	return false;
	}
uSocket=sock;
return true;
}

WORD UdpSocket::Read(VOID* buf, WORD size)
{
INT len=recv(uSocket, (CHAR*)buf, size, 0);
if(len<=0)
	throw E_FAIL;
return (WORD)len;
}

}}