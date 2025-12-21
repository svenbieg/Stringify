//===============
// TcpSocket.cpp
//===============

#include "Network/Tcp/TcpSocket.h"


//=======
// Using
//=======

#pragma comment (lib, "Ws2_32.lib")

#include <assert.h>
#include <ws2tcpip.h>


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//========
// Common
//========

Handle<TcpConnection> TcpSocket::Accept()
{
sockaddr_in addr;
INT addr_len=sizeof(sockaddr_in);
SOCKET sock=accept(m_Socket, (sockaddr*)&addr, &addr_len);
if(sock==INVALID_SOCKET)
	return nullptr;
return new TcpConnection(sock, addr.sin_addr.S_un.S_addr);
}

VOID TcpSocket::Close()
{
if(m_Socket==INVALID_SOCKET)
	return;
shutdown(m_Socket, SD_BOTH);
closesocket(m_Socket);
m_Socket=INVALID_SOCKET;
}

Handle<TcpConnection> TcpSocket::Connect(IP_ADDR host, WORD port)
{
assert(m_Socket==INVALID_SOCKET);
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=ERROR_SUCCESS)
	throw DeviceNotReadyException();
m_Socket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(m_Socket==INVALID_SOCKET)
	throw DeviceNotReadyException();
sockaddr_in addr={ 0 };
addr.sin_addr.S_un.S_addr=host;
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
status=connect(m_Socket, (sockaddr*)&addr, sizeof(sockaddr_in));
if(status!=S_OK)
	throw DeviceNotReadyException();
Handle<TcpConnection> con=new TcpConnection(m_Socket, host);
m_Socket=INVALID_SOCKET;
return con;
}

VOID TcpSocket::Listen(WORD port)
{
assert(m_Socket==INVALID_SOCKET);
WSADATA wsa;
INT status=WSAStartup(MAKEWORD(2,2), &wsa);
if(status!=ERROR_SUCCESS)
	throw DeviceNotReadyException();
m_Socket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(m_Socket==INVALID_SOCKET)
	throw DeviceNotReadyException();
sockaddr_in addr={ 0 };
addr.sin_addr.S_un.S_addr=INADDR_ANY;
addr.sin_family=AF_INET;
addr.sin_port=htons(port);
status=bind(m_Socket, (sockaddr const*)&addr, sizeof(sockaddr_in));
if(status!=ERROR_SUCCESS)
	throw DeviceNotReadyException();
status=listen(m_Socket, SOMAXCONN);
if(status!=ERROR_SUCCESS)
	throw DeviceNotReadyException();
}


//==========================
// Con-/Destructors Private
//==========================

TcpSocket::TcpSocket():
m_Socket(INVALID_SOCKET)
{}

}}