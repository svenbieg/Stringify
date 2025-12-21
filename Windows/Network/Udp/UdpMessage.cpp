//================
// UdpMessage.cpp
//================

#include "Network/Udp/UdpMessage.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Udp {


//==================
// Con-/Destructors
//==================

UdpMessage::~UdpMessage()
{
}

Handle<UdpMessage> UdpMessage::Create(IP_ADDR ip)
{
return new UdpMessage(ip);
}


//========
// Common
//========

BYTE* UdpMessage::Begin()const
{
return nullptr;
}

WORD UdpMessage::GetSize()const
{
return 0;
}



//==========================
// Con-/Destructors Private
//==========================

UdpMessage::UdpMessage(IP_ADDR from):
m_From(from)
{}

}}