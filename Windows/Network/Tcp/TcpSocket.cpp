//===============
// TcpSocket.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#pragma comment (lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include "TcpSocket.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//==================
// Con-/Destructors
//==================

TcpSocket::TcpSocket(SOCKET sock, UINT remote_ip):
RemoteIp(remote_ip),
uSocket(sock)
{}

TcpSocket::~TcpSocket()
{
Close();
}


//========
// Common
//========

Handle<TcpSocket> TcpSocket::Accept()
{
sockaddr_in addr;
INT addr_len=sizeof(sockaddr_in);
SOCKET sock=accept(uSocket, (sockaddr*)&addr, &addr_len);
if(sock==INVALID_SOCKET)
	return nullptr;
return new TcpSocket(sock, addr.sin_addr.S_un.S_addr);
}

VOID TcpSocket::Close()
{
if(uSocket!=INVALID_SOCKET)
	{
	shutdown(uSocket, SD_BOTH);
	closesocket(uSocket);
	uSocket=INVALID_SOCKET;
	}
}

BOOL TcpSocket::Connect(Handle<String> host_name, WORD port)
{
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=0)
	return false;
CHAR host[32];
StringHelper::Copy(host, 32, host_name->Begin());
addrinfo info;
ZeroMemory(&info, sizeof(info));
info.ai_family=AF_INET;
info.ai_socktype=SOCK_STREAM;
addrinfo* result=nullptr;
getaddrinfo(host, nullptr, &info, &result);
if(!result)
	return false;
sockaddr_in addr=*(sockaddr_in*)(result->ai_addr);
freeaddrinfo(result);
SOCKET sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(sock==INVALID_SOCKET)
	return false;
addr.sin_port=htons(port);
status=connect(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
if(status!=S_OK)
	{
	closesocket(sock);
	return false;
	}
uSocket=sock;
RemoteIp=addr.sin_addr.S_un.S_addr;
return true;
}

VOID TcpSocket::Listen(WORD port)
{
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=0)
	throw E_FAIL;
addrinfo addr;
ZeroMemory(&addr, sizeof(addr));
addr.ai_family=AF_INET;
addr.ai_socktype=SOCK_STREAM;
addr.ai_protocol=IPPROTO_TCP;
addr.ai_flags=AI_PASSIVE;
addrinfo* paddr=nullptr;
CHAR port_str[8];
StringHelper::Print(port_str, 8, "%u", (UINT)port);
status=getaddrinfo(NULL, port_str, &addr, &paddr);
if(status!=0)
	throw E_FAIL;
SOCKET sock=socket(paddr->ai_family, paddr->ai_socktype, paddr->ai_protocol);
if(sock==INVALID_SOCKET)
	{
	freeaddrinfo(paddr);
	throw E_FAIL;
	}
status=bind(sock, paddr->ai_addr, (INT)paddr->ai_addrlen);
if(status==SOCKET_ERROR)
	{
	closesocket(sock);
	freeaddrinfo(paddr);
	throw E_FAIL;
	}
freeaddrinfo(paddr);
status=listen(sock, SOMAXCONN);
if(status==SOCKET_ERROR)
	{
	closesocket(sock);
	throw E_FAIL;
	}
uSocket=sock;
}


//==============
// Input-Stream
//==============

SIZE_T TcpSocket::Available()
{
u_long available=0;
if(ioctlsocket(uSocket, FIONREAD, &available)<0)
	available=0;
return available;
}

SIZE_T TcpSocket::Read(VOID* buf, SIZE_T size)
{
INT read=recv(uSocket, (LPSTR)buf, (INT)size, 0);
if(read<=0)
	throw E_FAIL;
return read;
}


//===============
// Output-Stream
//===============

VOID TcpSocket::Flush()
{
}

SIZE_T TcpSocket::Write(VOID const* buf, SIZE_T size)
{
INT written=send(uSocket, (CHAR const*)buf, (INT)size, 0);
if(written<=0)
	throw E_FAIL;
return written;
}


}}