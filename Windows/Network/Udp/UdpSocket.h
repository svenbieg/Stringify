//=============
// UdpSocket.h
//=============

#pragma once


//=======
// Using
//=======

#include "Network/Udp/UdpMessage.h"


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

	// Friends
	friend Object;

	// Con-/Destructors
	~UdpSocket() { Close(); }
	static inline Handle<UdpSocket> Create() { return Object::Create<UdpSocket>(); }

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