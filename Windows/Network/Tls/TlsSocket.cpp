//===============
// TlsSocket.cpp
//===============

#include "TlsSocket.h"


//=======
// Using
//=======

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Secur32.lib")

#include <assert.h>
#include <schannel.h>


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//========
// Common
//========

Handle<TlsConnection> TlsSocket::Accept()
{
sockaddr_in addr;
INT addr_len=sizeof(sockaddr_in);
SOCKET client=accept(m_Socket, (sockaddr*)&addr, &addr_len);
if(client==INVALID_SOCKET)
	throw ConnectionFailedException();
return new TlsConnection(client, addr.sin_addr.S_un.S_addr);
}

VOID TlsSocket::Close()
{
if(m_Socket==INVALID_SOCKET)
	return;
shutdown(m_Socket, SD_BOTH);
closesocket(m_Socket);
m_Socket=INVALID_SOCKET;
}

Handle<TlsConnection> TlsSocket::Connect(IP_ADDR host, WORD port)
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
Handle<TlsConnection> con=new TlsConnection(m_Socket, host);
m_Socket=INVALID_SOCKET;
return con;
}

VOID TlsSocket::Listen(WORD port)
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

TlsSocket::TlsSocket():
m_Socket(INVALID_SOCKET)
{}

}}