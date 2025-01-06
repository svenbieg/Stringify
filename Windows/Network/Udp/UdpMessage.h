//==============
// UdpMessage.h
//==============

#pragma once


//=======
// Using
//=======

#include "Network/Ip/IpAddress.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//=============
// UDP-Message
//=============

class UdpMessage: public Object
{
public:
	// Using
	using IP_ADDR=Network::Ip::IP_ADDR;

	// Con-/Destructors
	~UdpMessage();
	static Handle<UdpMessage> Create(IP_ADDR From);

	// Common
	BYTE* Begin()const;
	inline IP_ADDR From()const { return m_From; }
	WORD GetSize()const;

private:
	// Con-/Destructors
	UdpMessage(IP_ADDR From);

	// Common
	IP_ADDR m_From;
};

}}