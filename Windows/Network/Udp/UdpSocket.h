//=============
// UdpSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include "UdpMessage.h"


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
	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~UdpSocket() { Close(); }
	static inline Handle<UdpSocket> Create() { return new UdpSocket(); }

	// Common
	VOID Broadcast(WORD Port, Handle<UdpMessage> Message);
	VOID Close();
	Handle<UdpMessage> Receive(WORD Port);
	VOID Send(IP_ADDR To, WORD Port, Handle<UdpMessage> Message);

private:
	// Con-/Destructors
	UdpSocket();
};

}}