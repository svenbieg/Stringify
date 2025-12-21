//=============
// TlsSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include "Network/Tls/TlsConnection.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Tls {


//============
// TLS-Socket
//============

class TlsSocket: public Object
{
public:
	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~TlsSocket() { Close(); }
	static inline Handle<TlsSocket> Create() { return new TlsSocket(); }

	// Common
	Handle<TlsConnection> Accept();
	VOID Close();
	Handle<TlsConnection> Connect(IP_ADDR Host, WORD Port);
	VOID Listen(WORD Port);

private:
	// Con-/Destructors
	TlsSocket();

	// Common
	SOCKET m_Socket;
};

}}