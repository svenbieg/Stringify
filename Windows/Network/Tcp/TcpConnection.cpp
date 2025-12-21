//===================
// TcpConnection.cpp
//===================

#include "TcpConnection.h"


//=======
// Using
//=======

#pragma comment (lib, "Ws2_32.lib")

#include <ws2tcpip.h>


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//========
// Common
//========

VOID TcpConnection::Close()
{
if(m_Socket==INVALID_SOCKET)
	return;
shutdown(m_Socket, SD_BOTH);
closesocket(m_Socket);
m_RemoteAddress=0;
m_Socket=INVALID_SOCKET;
}


//==============
// Input-Stream
//==============

SIZE_T TcpConnection::Available()
{
u_long available=0;
if(ioctlsocket(m_Socket, FIONREAD, &available)<0)
	available=0;
return available;
}

SIZE_T TcpConnection::Read(VOID* buf, SIZE_T size)
{
INT read=recv(m_Socket, (LPSTR)buf, (INT)size, 0);
if(read<=0)
	throw DeviceNotReadyException();
return read;
}


//===============
// Output-Stream
//===============

VOID TcpConnection::Flush()
{
}

SIZE_T TcpConnection::Write(VOID const* buf, SIZE_T size)
{
INT written=send(m_Socket, (CHAR const*)buf, (INT)size, 0);
if(written<=0)
	throw DeviceNotReadyException();
return written;
}


//==========================
// Con-/Destructors Private
//==========================

TcpConnection::TcpConnection(SOCKET sock, IP_ADDR remote_ip):
m_RemoteAddress(remote_ip),
m_Socket(sock)
{}

}}