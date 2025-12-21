//=============
// TcpSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include "Network/Tcp/TcpConnection.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tcp {


//============
// TCP-Socket
//============

class TcpSocket: public Object
{
public:
	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~TcpSocket() { Close(); }
	static inline Handle<TcpSocket> Create() { return new TcpSocket(); }

	// Common
	Handle<TcpConnection> Accept();
	VOID Close();
	Handle<TcpConnection> Connect(IP_ADDR Host, WORD Port);
	VOID Listen(WORD Port);

private:
	// Con-/Destructors
	TcpSocket();

	// Common
	SOCKET m_Socket;
};

}}