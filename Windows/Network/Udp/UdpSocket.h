//=============
// UdpSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include <WinSock2.h>


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//============
// UDP-Socket
//============

class UdpSocket: public Object
{
public:
	// Con-/Destructors
	UdpSocket();
	~UdpSocket();

	// Common
	static BOOL Broadcast(WORD Port, VOID const* Buffer, WORD Size);
	VOID Close();
	BOOL Listen(WORD Port);
	WORD Read(VOID* Buffer, WORD Size);

private:
	// Common
	SOCKET uSocket;
};

}}